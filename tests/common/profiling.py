"""NPU runner setup, profiler sample mapping, and Mock session summaries."""

import json
import math
import os
from pathlib import Path
import platform
import re
import shutil
import signal
from statistics import mean
import subprocess

import numpy as np

from tests.common.cases import TOLERANCES
from tests.common.reference import STORAGE_DTYPES
from tests.common.tensor_io import file_sha256


ROOT = Path(__file__).resolve().parents[2]
RUNNER_SOURCE = ROOT / "tests/st/runner/fire_candidate_decode_fusion_runner.cpp"
DEFAULT_TOOLKIT = Path("/usr/local/Ascend/cann-9.0.0")
TARGET = {"system": "Linux", "architecture": "aarch64", "toolkit_version": "9.0.0",
          "compute_unit": "ascend910b"}


def save_json(path, value):
    Path(path).write_text(json.dumps(value, indent=2, allow_nan=False) + "\n", encoding="utf-8")


def validate_environment(build, toolkit):
    if platform.system() != TARGET["system"] or platform.machine() not in ("aarch64", "arm64"):
        raise RuntimeError("Mock profiling requires ARM Linux")
    toolkit = Path(toolkit).resolve()
    versions = (toolkit / "share/info/asc-devkit/version.info", toolkit / "version.info")
    version = None
    for path in versions:
        if path.is_file():
            match = re.search(r'^(?:Version|version)\s*=\s*["\']?([^"\'\s]+)',
                              path.read_text(), re.M) or re.search(
                r'^(?:required_package_runtime_version|version)\s*=\s*["\']?([^"\'\s]+)',
                path.read_text(), re.M)
            if match:
                version = match[1]
                break
    if version != TARGET["toolkit_version"]:
        raise RuntimeError(f"Mock profiling requires CANN 9.0.0; observed {version or 'missing SDK'} at {toolkit}")
    cache = (Path(build) / "CMakeCache.txt").read_text()
    recorded = re.search(r"^ASCEND_CANN_PACKAGE_PATH:[^=]+=([^\r\n]+)", cache, re.M)
    unit = re.search(r"^ASCEND_COMPUTE_UNIT:[^=]+=([^\r\n]+)", cache, re.M)
    if not recorded or Path(recorded[1]).resolve() != toolkit:
        raise ValueError("the operator must be built with the selected CANN 9.0.0 SDK")
    if unit and unit[1] != TARGET["compute_unit"]:
        raise ValueError("the operator must be built for ascend910b")
    for path in (toolkit / "include/acl/acl.h", Path(build) / "libcust_opapi.so",
                 Path(build) / "autogen/aclnn_fire_candidate_decode_fusion.h"):
        if not path.is_file():
            raise FileNotFoundError(path)
    for path in (toolkit / "bin/msprof", toolkit / "tools/profiler/bin/msprof"):
        if path.is_file() and os.access(path, os.X_OK) and path.resolve().is_relative_to(toolkit):
            return path
    raise FileNotFoundError(f"msprof is missing from {toolkit}")


def compile_runner(build, toolkit, output, compiler="g++"):
    destination = Path(output) / "runner"
    command = [compiler, "-std=c++17", "-O2", str(RUNNER_SOURCE),
               "-I", str(Path(build) / "autogen"), "-I", str(Path(toolkit) / "include"),
               "-L", str(build), "-L", str(Path(toolkit) / "lib64"),
               f"-Wl,-rpath,{build}", f"-Wl,-rpath,{Path(toolkit) / 'lib64'}",
               "-lcust_opapi", "-lascendcl", "-lnnopbase", "-o", str(destination)]
    save_json(Path(output) / "runner-command.json", command)
    with (Path(output) / "runner-build.log").open("w") as log:
        subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, check=True, timeout=120)
    return destination


def runtime_environment(build, toolkit):
    build, toolkit = Path(build), Path(toolkit)
    vendor = build / "tmp/vendors/custom"
    if not vendor.is_dir():
        raise FileNotFoundError(vendor)
    source = build / "op_host/libcustom_ascendc_cust_optiling.so"
    destination = vendor / "op_impl/ai_core/tbe/op_tiling/lib/linux/aarch64/libcust_opmaster_rt2.0.so"
    if source.is_file():
        destination.parent.mkdir(parents=True, exist_ok=True)
        if destination.exists() and file_sha256(destination) != file_sha256(source):
            raise ValueError("the vendor tiling library differs from this build")
        if not destination.exists():
            shutil.copyfile(source, destination)
    environment = dict(os.environ)
    environment.pop("LD_PRELOAD", None)
    environment["ASCEND_HOME_PATH"] = str(toolkit)
    environment["ASCEND_CANN_PACKAGE_PATH"] = str(toolkit)
    environment["ASCEND_CUSTOM_OPP_PATH"] = str(vendor)
    environment["LD_LIBRARY_PATH"] = ":".join((str(build), str(toolkit / "lib64"), environment.get("LD_LIBRARY_PATH", "")))
    return environment


def fixture_hashes(data, cases):
    result = {}
    for case in cases:
        folder = Path(data) / case.name.removeprefix("mock_")
        hashes = json.loads((folder / "fixture.json").read_text())
        if set(hashes) != {"raw_pred.bin", "priors.bin", "reference.bin"}:
            raise ValueError(f"incomplete fixture: {case.name}")
        for name, digest in hashes.items():
            if file_sha256(folder / name) != digest:
                raise ValueError(f"fixture changed: {case.name}/{name}")
        result[case.name] = {**hashes, "case.conf": file_sha256(folder / "case.conf")}
    return result


def parse_samples(log, numbers, repeat, device):
    """Associate each task with its case and kernel-local launch ordinal."""
    launches = {number: [] for number in numbers}
    starts, done, counters = [], set(), {}
    current = None
    for line in log.splitlines():
        begin = re.search(r"\bcase_begin=(\d+)", line)
        end = re.search(r"\bcase_done=(\d+)", line)
        launch = re.search(r"Start profiling on device (\d+), kernel:\s*(\S+)", line)
        if begin:
            current = int(begin[1])
            if len(starts) >= len(numbers) or current != numbers[len(starts)]:
                raise ValueError("runner case order differs from the selected cases")
            starts.append(current)
        if launch:
            kernel = launch[2]
            if int(launch[1]) != device or "firecandidatedecodefusion" not in kernel.lower().replace("_", ""):
                raise ValueError("unexpected profiler device or kernel")
            if current is None or len(launches[current]) >= repeat:
                raise ValueError("unmapped or extra profiler launch")
            ordinal = counters.get(kernel, 0)
            counters[kernel] = ordinal + 1
            launches[current].append((kernel, ordinal))
        if end:
            number = int(end[1])
            if current != number or number in done:
                raise ValueError("duplicate or out-of-order completed case")
            done.add(number)
            current = None
    records = list(re.finditer(r"\bkernel:\s*([^,\s]+),\s*name is:\s*(\d+)", log))
    durations = {}
    for index, record in enumerate(records):
        block = log[record.end():records[index + 1].start() if index + 1 < len(records) else len(log)]
        values = re.findall(r"Task Duration\(us\):\s*([^\s,]+)", block)
        key = (record[1], int(record[2]))
        if key in durations or len(values) != 1:
            raise ValueError("duplicate or missing profiling analysis")
        value = float(values[0])
        if not math.isfinite(value) or value <= 0:
            raise ValueError("kernel duration must be finite and positive")
        durations[key] = value
    mapped = {key for keys in launches.values() for key in keys}
    if set(durations) - mapped:
        raise ValueError("profiling analysis has no matching launch")
    return {number: [durations[key] for key in keys if key in durations] for number, keys in launches.items()}, starts, done


def compare_output(actual, reference, case):
    dtype = STORAGE_DTYPES[case.raw_pred_dtype]
    expected_size = int(np.prod(case.raw_pred_shape[:2])) * 5 * np.dtype(dtype).itemsize
    if Path(actual).stat().st_size != expected_size or Path(reference).stat().st_size != expected_size:
        raise ValueError("decoded output or reference byte size mismatch")
    expected = np.fromfile(reference, dtype=dtype).astype(np.float32).reshape(-1, 5)
    if not np.isfinite(expected).all():
        raise ValueError("Mock reference contains nonfinite values")
    got = np.fromfile(actual, dtype=dtype).astype(np.float32).reshape(-1, 5)
    atol, rtol = TOLERANCES[case.raw_pred_dtype]
    with np.errstate(invalid="ignore", over="ignore"):
        bad = ~np.isfinite(got) | (np.abs(got - expected) > atol + rtol * np.abs(expected))
    bad[:, 4] |= (got[:, 4] < 0) | (got[:, 4] > 1)
    count = int(bad.sum())
    return {"passed": count == 0, "mismatches": count, "error_ratio": f"{bad.mean() * 100:.2f}%",
            "actual_sha256": file_sha256(Path(actual))}


def run_command(command, environment, timeout):
    """Terminate the profiler and its runner together when a session times out."""
    process = subprocess.Popen(command, env=environment, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                               text=True, start_new_session=True)
    timed_out = False
    try:
        log, _ = process.communicate(timeout=timeout)
    except subprocess.TimeoutExpired:
        timed_out = True
        try:
            os.killpg(process.pid, signal.SIGTERM)
        except ProcessLookupError:
            pass
        try:
            log, _ = process.communicate(timeout=5)
        except subprocess.TimeoutExpired:
            try:
                os.killpg(process.pid, signal.SIGKILL)
            except ProcessLookupError:
                pass
            log, _ = process.communicate()
    return {"log": log, "pid": process.pid, "returncode": process.returncode, "timed_out": timed_out}


def summarize_session(log, actual, data, cases, repeat, device, returncode, timed_out=False, *, soc=None):
    numbers = [int(case.name.removeprefix("mock_")) for case in cases]
    mapping_error = None
    observed_soc = None
    try:
        soc_names = re.findall(r"^runtime_soc=(\S+)[ \t]*$", log, re.M)
        if len(soc_names) != 1:
            raise ValueError("runtime did not confirm a unique SoC")
        observed_soc = soc_names[0]
        if soc is not None and observed_soc != soc:
            raise ValueError(f"runtime SoC mismatch: expected {soc}; observed {observed_soc}")
        samples, started, completed = parse_samples(log, numbers, repeat, device)
    except ValueError as error:
        mapping_error = str(error)
        samples = {number: [] for number in numbers}
        started = [int(value) for value in re.findall(r"\bcase_begin=(\d+)", log)]
        completed = {int(value) for value in re.findall(r"\bcase_done=(\d+)", log)}
    failed_at = started[-1] if started and started[-1] not in completed else None
    rows = []
    for number, case in zip(numbers, cases):
        checks, errors = [], []
        folder = Path(actual) / f"{number:02d}"
        for sample in range(1, repeat + 1):
            path = folder / f"decoded-{sample}.bin"
            if path.is_file():
                try:
                    checks.append(compare_output(path, Path(data) / f"{number:02d}/reference.bin", case))
                except (OSError, ValueError) as error:
                    errors.append(str(error))
        if any(not check["passed"] for check in checks):
            status = "Wrong Answer"
        elif number in completed and len(checks) == repeat and not errors:
            if mapping_error or len(samples[number]) != repeat:
                status = "Profiling Error"
            elif timed_out and failed_at is None:
                status = "Timeout"
            elif returncode and failed_at is None:
                status = "Infrastructure Error"
            else:
                status = "Pass"
        elif number == failed_at:
            status = "Timeout" if timed_out else "Runtime Error"
        elif failed_at is not None and number not in started:
            status = "Skipped"
        else:
            status = "Infrastructure Error"
        values = samples[number]
        rows.append({"name": case.name, "number": number, "shape": list(case.raw_pred_shape),
                     "dtype": case.raw_pred_dtype, "status": status, "passed": status == "Pass",
                     "durations_us": values, "sample_count": len(values),
                     "elapsed_us": mean(values) if len(values) == repeat else None,
                     "checks": checks, "errors": errors,
                     "profiling_error": mapping_error, "runtime_soc": observed_soc, "returncode": returncode,
                     "runtime_codes": re.findall(r"failed with status=(\d+)", log) if number == failed_at else []})
    return rows


def aggregate_sessions(sessions, repeat):
    if not sessions or any(len(session) != len(sessions[0]) for session in sessions):
        raise ValueError("session case coverage differs")
    result = []
    for index, first in enumerate(sessions[0]):
        rows = [session[index] for session in sessions]
        if any(any(row[field] != first[field] for field in ("name", "shape", "dtype")) for row in rows):
            raise ValueError("session fixtures differ")
        values = [value for row in rows for value in row["durations_us"]]
        complete = all(len(row["durations_us"]) == repeat and row["elapsed_us"] is not None
                       and all(math.isfinite(value) and value > 0 for value in row["durations_us"]) for row in rows)
        statuses = {row["status"] for row in rows}
        result.append({"name": first["name"], "number": first["number"], "shape": first["shape"], "dtype": first["dtype"],
                       "status": next(iter(statuses)) if len(statuses) == 1 else "Unstable",
                       "passed": all(row["passed"] for row in rows) and complete,
                       "durations_us": values, "sample_count": len(values), "process_count": len(sessions),
                       "elapsed_us": mean(values) if complete else None,
                       "sessions": rows})
    return result

"""Run Coverage/Correctness on an NPU through the custom operator's ACLNN API."""

from __future__ import annotations

import argparse
from dataclasses import asdict
import json
import math
import os
from pathlib import Path
import shutil
import subprocess
from typing import Optional

from tests.common.artifacts import file_sha256, load_case_metadata, load_suite_manifest
from tests.common.generate_data import generate_suite
from tests.common.verify_result import verify_case_output


HEADER_NAME = "aclnn_fire_candidate_decode_fusion.h"
SOURCE = Path(__file__).with_name("runner.cpp")


def toolkit_root(build: Path, explicit: Optional[Path] = None) -> Path:
    candidates = []
    if explicit is not None:
        candidates.append(explicit)
    else:
        cache = build / "CMakeCache.txt"
        if cache.is_file():
            for line in cache.read_text(encoding="utf-8").splitlines():
                if line.startswith("ASCEND_CANN_PACKAGE_PATH:"):
                    candidates.append(Path(line.split("=", 1)[1]))
        candidates.extend(Path(value) for value in (
            os.environ.get("ASCEND_HOME_PATH"), os.environ.get("ASCEND_TOOLKIT_HOME"),
            "/usr/local/Ascend/ascend-toolkit/latest",
        ) if value)
    for path in candidates:
        if (path / "include/acl/acl.h").is_file() and (path / "include/aclnn/acl_meta.h").is_file():
            return path.resolve()
    raise RuntimeError("CANN headers unavailable; source its set_env.sh or pass --toolkit")


def operator_paths(build: Path) -> tuple:
    library = build / "libcust_opapi.so"
    header = build / "autogen" / HEADER_NAME
    if not library.is_file() or not header.is_file():
        raise ValueError(f"incomplete operator build: expected {library} and {header}")
    for vendor in (build / "tmp/vendors/custom", build / "vendors/custom"):
        if vendor.is_dir():
            return library, header, vendor
    raise ValueError("custom OPP directory unavailable: expected build/tmp/vendors/custom or build/vendors/custom")


def compile_runner(build: Path, output: Path, toolkit: Path, compiler: str) -> Path:
    runner = output / "runner"
    command = [
        compiler, "-std=c++17", "-O2", "-Wall", "-Wextra", str(SOURCE),
        "-I", str(build / "autogen"), "-I", str(toolkit / "include"),
        "-L", str(build), "-L", str(toolkit / "lib64"),
        f"-Wl,-rpath,{build}", f"-Wl,-rpath,{toolkit / 'lib64'}",
        "-lcust_opapi", "-lascendcl", "-lnnopbase", "-o", str(runner),
    ]
    (output / "runner-build-command.json").write_text(json.dumps(command, indent=2) + "\n", encoding="utf-8")
    with (output / "runner-build.log").open("w", encoding="utf-8") as log:
        result = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, check=False, timeout=120)
    if result.returncode:
        raise RuntimeError(f"runner compilation failed; see {output / 'runner-build.log'}")
    return runner


def runtime_environment(build: Path, toolkit: Path, vendor: Path) -> dict:
    environment = dict(os.environ)
    paths = (str(build), str(toolkit / "lib64"), "/usr/local/Ascend/driver/lib64",
             "/usr/local/Ascend/driver/lib64/common", "/usr/local/Ascend/driver/lib64/driver",
             environment.get("LD_LIBRARY_PATH"))
    environment["LD_LIBRARY_PATH"] = ":".join(value for value in paths if value)
    environment["ASCEND_CUSTOM_OPP_PATH"] = ":".join(value for value in
        (str(vendor), environment.get("ASCEND_CUSTOM_OPP_PATH")) if value)
    return environment


def fixture_config(metadata: dict) -> str:
    if metadata["expectation"] != "success":
        raise ValueError("only valid input-contract cases may run on the device")
    raw = metadata["input"]["raw_pred"]
    return ("fixture_format=1\n" + f"batch={raw['shape'][0]}\n" +
            f"candidates={raw['shape'][1]}\n" + f"dtype={raw['dtype']}\n")


def check_fixture_inputs(fixture: Path, metadata: dict) -> None:
    for name in ("raw_pred", "priors"):
        path = fixture / (name + ".bin")
        descriptor = metadata["files"][name]
        if path.stat().st_size != descriptor["bytes"] or file_sha256(path) != descriptor["sha256"]:
            raise ValueError(f"execution fixture {name} does not match the verified source")


def execute_case(runner: Path, source: Path, fixture: Path, device: int, environment: dict, timeout: float) -> dict:
    metadata = load_case_metadata(source)
    source_metadata_sha = file_sha256(source / "case.json")
    source_manifest_sha = file_sha256(source.parent / "manifest.json")
    fixture.mkdir(parents=True, exist_ok=False)
    (fixture / "case.conf").write_text(fixture_config(metadata), encoding="utf-8")
    for name in ("raw_pred", "priors"):
        shutil.copyfile(source / (name + ".bin"), fixture / (name + ".bin"))
    check_fixture_inputs(fixture, metadata)
    actual = fixture / "decoded.bin"
    command = [str(runner), "--fixture", str(fixture), "--output", str(actual), "--device", str(device)]
    (fixture / "command.json").write_text(json.dumps(command, indent=2) + "\n", encoding="utf-8")
    row = {"case": metadata["name"], "category": metadata["category"], "expectation": "success",
           "input_sha256": {name: metadata["files"][name]["sha256"] for name in ("raw_pred", "priors")},
           "runner_log": str(fixture / "runner.log"), "actual": str(actual)}
    try:
        process = subprocess.run(command, capture_output=True, text=True, env=environment, timeout=timeout, check=False)
        stdout, stderr = process.stdout, process.stderr
        row["runner_returncode"] = process.returncode
    except subprocess.TimeoutExpired as error:
        stdout = error.stdout or ""
        stderr = error.stderr or ""
        if isinstance(stdout, bytes):
            stdout = stdout.decode(errors="replace")
        if isinstance(stderr, bytes):
            stderr = stderr.decode(errors="replace")
        row.update(status="Timeout", passed=False, runner_returncode=None)
        (fixture / "runner.log").write_text(stdout + stderr + "\nexecution timeout\n", encoding="utf-8")
        return row
    except OSError as error:
        row.update(status="Runtime Error", passed=False, runner_returncode=None, detail=str(error))
        (fixture / "runner.log").write_text(str(error) + "\n", encoding="utf-8")
        return row
    (fixture / "runner.log").write_text(stdout + stderr, encoding="utf-8")
    if process.returncode or "device_output=PASS" not in stdout or not actual.is_file():
        row.update(status="Runtime Error", passed=False)
        return row
    # Both the execution copy and the original bundle must still be intact.
    check_fixture_inputs(fixture, metadata)
    if (file_sha256(source / "case.json") != source_metadata_sha
            or file_sha256(source.parent / "manifest.json") != source_manifest_sha):
        raise ValueError("source bundle metadata changed during NPU execution")
    result = verify_case_output(source, actual)
    row.update(status="Pass" if result.passed else "Wrong Answer", passed=result.passed,
               comparison=asdict(result), output_sha256=file_sha256(actual))
    return row


def replay_suites(build: Path, output: Path, *, category: str = "all", device: int = 0,
                  toolkit: Optional[Path] = None, data_root: Optional[Path] = None,
                  names: tuple = (), compiler: str = "c++", timeout: float = 300) -> dict:
    from tests.correctness.cases import CASES as correctness
    from tests.coverage.cases import CASES as coverage
    if category not in ("all", "coverage", "correctness") or not 0 <= device <= 2147483647 or not math.isfinite(timeout) or timeout <= 0:
        raise ValueError("invalid suite, device, or timeout")
    catalogs = {"coverage": coverage, "correctness": correctness}
    categories = ("coverage", "correctness") if category == "all" else (category,)
    known = {case.name for group in categories for case in catalogs[group]}
    requested = set(names)
    if requested - known:
        raise ValueError(f"unknown case names for selected suite: {sorted(requested - known)}")
    selected = {group: tuple(case for case in catalogs[group] if not requested or case.name in requested) for group in categories}
    if not any(case.expectation == "success" for cases in selected.values() for case in cases):
        raise ValueError("select at least one valid device case")
    build, output = Path(build).resolve(), Path(output).resolve()
    library, header, vendor = operator_paths(build)
    toolkit = toolkit_root(build, toolkit)
    if data_root is not None:
        data_root = Path(data_root).resolve()
        for group, cases in selected.items():
            if not cases:
                continue
            manifest = load_suite_manifest(data_root / group)
            available = {entry["name"] for entry in manifest["cases"]}
            if {case.name for case in cases} - available:
                raise ValueError(f"cached {group} data is missing selected cases; generate a fresh suite directory")
            for case in cases:
                metadata = load_case_metadata(data_root / group / case.name)
                if metadata["expectation"] != case.expectation:
                    raise ValueError("cached case expectation differs from the selected catalog")
    output.mkdir(parents=True, exist_ok=False)
    report = {"operator": "FireCandidateDecodeFusion", "device": device, "build": str(build),
              "toolkit": str(toolkit), "op_library_sha256": file_sha256(library),
              "aclnn_header_sha256": file_sha256(header), "runner_source_sha256": file_sha256(SOURCE),
              "manifests": {}, "cases": [], "summary": {}, "passed": False}
    try:
        runner = compile_runner(build, output, toolkit, compiler)
        report["runner_sha256"] = file_sha256(runner)
        environment = runtime_environment(build, toolkit, vendor)
        for group, cases in selected.items():
            if not cases:
                continue
            suite = data_root / group if data_root is not None else output / "data" / group
            if data_root is None:
                generate_suite(suite, cases)
            report["manifests"][group] = {"path": str(suite / "manifest.json"), "sha256": file_sha256(suite / "manifest.json")}
            rows = []
            for case in cases:
                source = suite / case.name
                try:
                    metadata = load_case_metadata(source)
                    if metadata["expectation"] == "reject":
                        row = {"case": case.name, "category": group, "expectation": "reject",
                               "status": "Skipped", "reason": "local input-contract fixture", "validation_errors": metadata["validation_errors"]}
                    else:
                        row = execute_case(runner, source, output / "cases" / group / case.name, device, environment, timeout)
                except (OSError, KeyError, TypeError, ValueError) as error:
                    row = {"case": case.name, "category": group, "expectation": case.expectation,
                           "status": "Data Error", "passed": False, "detail": str(error)}
                rows.append(row)
                print(f"{group} {case.name}: {row['status']}", flush=True)
            report["cases"].extend(rows)
            report["summary"][group] = {
                "total": len(rows), "passed": sum(row.get("passed") is True for row in rows),
                "failed": sum(row.get("passed") is False for row in rows),
                "skipped": sum(row["status"] == "Skipped" for row in rows),
            }
        report["passed"] = all(group["failed"] == 0 for group in report["summary"].values())
    except (OSError, subprocess.SubprocessError, RuntimeError, ValueError) as error:
        report["setup_error"] = str(error)
        raise
    finally:
        (output / "results.json").write_text(json.dumps(report, indent=2, sort_keys=True, allow_nan=False) + "\n", encoding="utf-8")
    return report


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", type=Path, default=Path("build"))
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--suite", choices=("all", "coverage", "correctness"), default="all")
    parser.add_argument("--device", type=int, default=0)
    parser.add_argument("--toolkit", type=Path)
    parser.add_argument("--data-root", type=Path, help="reuse category directories under this root")
    parser.add_argument("--case", action="append", default=[])
    parser.add_argument("--compiler", default="c++")
    parser.add_argument("--timeout", type=float, default=300, help="per-case process timeout in seconds")
    args = parser.parse_args()
    try:
        report = replay_suites(args.build, args.output, category=args.suite, device=args.device,
                               toolkit=args.toolkit, data_root=args.data_root, names=tuple(args.case),
                               compiler=args.compiler, timeout=args.timeout)
    except (OSError, KeyError, TypeError, ValueError, RuntimeError, subprocess.SubprocessError) as error:
        parser.error(str(error))
    print(json.dumps(report["summary"], sort_keys=True))
    return 0 if report["passed"] else 1


if __name__ == "__main__":
    raise SystemExit(main())

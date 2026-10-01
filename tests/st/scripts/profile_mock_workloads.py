"""Run Mock performance tests on ARM Linux and CANN 9.0.0 using the selected NPU."""

import argparse
import math
from pathlib import Path
import shlex
import subprocess

from tests.common.cases import MOCK_CASES, select_cases
from tests.common.mock_cases import generate_mock_suite
from tests.common.profiling import (
    DEFAULT_TOOLKIT, TARGET, RUNNER_SOURCE, aggregate_sessions, compile_runner,
    fixture_hashes, run_command, runtime_environment, save_json, summarize_session,
    validate_environment,
)
from tests.common.tensor_io import file_sha256


def profile_cases(build, output, cases=MOCK_CASES, *, toolkit=DEFAULT_TOOLKIT, device=0,
                  processes=3, repeat=5, warmup=5, timeout=240, compiler="g++", soc=None):
    if (type(processes) is not int or processes < 1 or type(repeat) is not int or not 1 <= repeat <= 10000
            or type(warmup) is not int or warmup < 0 or type(device) is not int or device < 0
            or not math.isfinite(timeout) or timeout <= 0):
        raise ValueError("invalid process count, repeat count, warmup, device, or timeout")
    if soc is not None and (not isinstance(soc, str) or not soc or any(character.isspace() for character in soc)):
        raise ValueError("soc must be a nonempty name without whitespace")
    build, output, toolkit = Path(build).resolve(), Path(output).resolve(), Path(toolkit).resolve()
    if output.exists() or output.is_symlink():
        raise FileExistsError(f"refusing to overwrite {output}")
    profiler = validate_environment(build, toolkit)
    output.mkdir(parents=True, exist_ok=False)
    report = {"passed": False, "target": {**TARGET, "soc": None}, "expected_soc": soc,
              "build": str(build), "toolkit": str(toolkit), "device": device,
              "protocol": {"processes": processes, "repeat": repeat, "warmup": warmup,
                           "metric": "msprof op BasicInfo Task Duration(us)", "statistic": "arithmetic mean"},
              "cases": [], "sessions": []}
    try:
        data = output / "data"
        generate_mock_suite(data, cases)
        hashes = fixture_hashes(data, cases)
        report["input_hashes"] = hashes
        runner = compile_runner(build, toolkit, output, compiler)
        environment = runtime_environment(build, toolkit)
        report["runner_source_sha256"] = file_sha256(RUNNER_SOURCE)
        report["runner_sha256"] = file_sha256(runner)
        report["library_sha256"] = file_sha256(build / "libcust_opapi.so")
        rows = []
        for index in range(1, processes + 1):
            session = output / f"process-{index}"
            session.mkdir()
            actual = session / "actual"
            actual.mkdir()
            application = [str(runner), "--suite", str(data), "--output-root", str(actual),
                           "--device", str(device), "--repeat", str(repeat)]
            if soc is not None:
                application.extend(("--soc", soc))
            command = [str(profiler), "op", "--application=" + shlex.join(application),
                       "--output=" + str(session / "profile"), "--aic-metrics=BasicInfo",
                       "--warm-up=" + str(warmup), "--launch-count=" + str(len(cases) * repeat)]
            save_json(session / "command.json", command)
            if file_sha256(build / "libcust_opapi.so") != report["library_sha256"]:
                raise ValueError("operator library changed between sessions")
            if fixture_hashes(data, cases) != hashes:
                raise ValueError("fixture changed between sessions")
            process = run_command(command, environment, timeout)
            (session / "profiler.log").write_text(process["log"], encoding="utf-8")
            if file_sha256(build / "libcust_opapi.so") != report["library_sha256"]:
                raise ValueError("operator library changed during device execution")
            if fixture_hashes(data, cases) != hashes:
                raise ValueError("fixture changed during device execution")
            observed = summarize_session(process["log"], actual, data, cases, repeat, device,
                                         process["returncode"], process["timed_out"], soc=soc)
            save_json(session / "results.json", observed)
            rows.append(observed)
            report["sessions"].append({"pid": process["pid"], "returncode": process["returncode"],
                                       "timed_out": process["timed_out"], "directory": str(session),
                                       "soc": observed[0]["runtime_soc"]})
            observed_soc = observed[0]["runtime_soc"]
            if report["target"]["soc"] is not None and observed_soc != report["target"]["soc"]:
                raise ValueError("runtime SoC changed between sessions")
            report["target"]["soc"] = observed_soc
            report["cases"] = aggregate_sessions(rows, repeat)
            save_json(output / "results.json", report)
        report["passed"] = len(rows) == processes and all(row["passed"] for row in report["cases"])
    except (OSError, ValueError, RuntimeError, subprocess.SubprocessError) as error:
        report["error"] = str(error)
        save_json(output / "results.json", report)
        raise
    save_json(output / "results.json", report)
    return report


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", type=Path, default=Path("build"))
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--toolkit", type=Path, default=DEFAULT_TOOLKIT, help="CANN 9.0.0 SDK directory")
    parser.add_argument("--device", type=int, default=0)
    parser.add_argument("--soc", help="require this exact runtime SoC name; default: detect from the selected device")
    parser.add_argument("--case", action="append", default=[])
    parser.add_argument("--processes", type=int, default=3)
    parser.add_argument("--repeat", type=int, default=5)
    parser.add_argument("--warmup", type=int, default=5)
    parser.add_argument("--timeout", type=float, default=240)
    parser.add_argument("--compiler", default="g++")
    args = parser.parse_args()
    try:
        cases = select_cases("mock", names=args.case)
        report = profile_cases(args.build, args.output, cases, toolkit=args.toolkit, device=args.device,
                               processes=args.processes, repeat=args.repeat, warmup=args.warmup,
                               timeout=args.timeout, compiler=args.compiler, soc=args.soc)
    except (OSError, ValueError, RuntimeError, subprocess.SubprocessError) as error:
        parser.exit(2, f"Mock profiling failed: {error}\n")
    for case in report["cases"]:
        duration = f"{case['elapsed_us']:.6f} us" if case["elapsed_us"] is not None else "unavailable"
        print(f"{case['name']}: {case['status']}; mean={duration}; samples={case['sample_count']}")
    return 0 if report["passed"] else 1


if __name__ == "__main__":
    raise SystemExit(main())

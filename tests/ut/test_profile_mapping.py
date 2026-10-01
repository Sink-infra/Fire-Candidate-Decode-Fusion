"""Profiler mapping, numerical checks, environment gates, and session execution."""

import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

import numpy as np

from tests.common.cases import MOCK_CASES
from tests.common.mock_cases import generate_mock_suite
from tests.common.profiling import (
    DEFAULT_TOOLKIT, RUNNER_SOURCE, TARGET, aggregate_sessions, compare_output, fixture_hashes,
    parse_samples, run_command, summarize_session, validate_environment,
)
from tests.st.scripts import profile_mock_workloads as profiling


def profile_log(numbers, repeat=5, trial=0, soc="Ascend910B1"):
    lines = [f"runtime_soc={soc}"]
    records, counters = [], {}
    for number in numbers:
        lines.append(f"case_begin={number:02d}")
        kernel = "FireCandidateDecodeFusion_fp32" if number <= 6 else "FireCandidateDecodeFusion_fp16"
        for sample in range(repeat):
            ordinal = counters.get(kernel, 0)
            counters[kernel] = ordinal + 1
            lines.append(f"Start profiling on device 0, kernel: {kernel}")
            records.append(f"kernel: {kernel}, name is: {ordinal}\nTask Duration(us): {5 + trial * 2 + sample}")
        lines.append(f"case_done={number:02d}")
    return "\n".join(lines + list(reversed(records))) + "\n"


class ProfileMappingTests(unittest.TestCase):
    def test_kernel_local_ordinals_and_nonconsecutive_case_numbers(self):
        samples, started, done = parse_samples(profile_log([1, 7, 12]), [1, 7, 12], 5, 0)
        self.assertEqual(started, [1, 7, 12])
        self.assertEqual(done, {1, 7, 12})
        self.assertEqual(samples, {number: [5, 6, 7, 8, 9] for number in (1, 7, 12)})
        self.assertEqual(parse_samples(profile_log([12]), [12], 5, 0)[0][12], [5, 6, 7, 8, 9])

    def test_duplicate_unmapped_and_nonfinite_measurements_are_rejected(self):
        original = profile_log([1])
        for log in (
            original + "kernel: FireCandidateDecodeFusion_fp32, name is: 0\nTask Duration(us): 5\n",
            original + "kernel: FireCandidateDecodeFusion_fp32, name is: 9\nTask Duration(us): 5\n",
            original.replace("Task Duration(us): 5", "Task Duration(us): nan"),
            original.replace("Task Duration(us): 5", "Task Duration(us): 0"),
            original.replace("Start profiling on device 0", "Start profiling on device 7"),
            original.replace("case_begin=01", "case_begin=12"),
        ):
            with self.subTest(log=log), self.assertRaises(ValueError):
                parse_samples(log, [1], 5, 0)

    def test_missing_measurement_cannot_pass(self):
        case = MOCK_CASES[0]
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            generate_mock_suite(root / "data", (case,))
            folder = root / "actual/01"
            folder.mkdir(parents=True)
            for sample in range(1, 6):
                shutil.copyfile(root / "data/01/reference.bin", folder / f"decoded-{sample}.bin")
            log = profile_log([1]).replace("Task Duration(us): 5", "Task Duration(us): 5.0")
            log = log[:log.rfind("kernel: FireCandidateDecodeFusion_fp32, name is: 0")]
            row = summarize_session(log, root / "actual", root / "data", (case,), 5, 0, 0)[0]
            self.assertFalse(row["passed"])
            self.assertEqual(row["status"], "Profiling Error")
            self.assertIsNone(row["elapsed_us"])

    def test_timed_out_profiler_cannot_pass_after_completed_kernel_outputs(self):
        case = MOCK_CASES[0]
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            generate_mock_suite(root / "data", (case,))
            folder = root / "actual/01"
            folder.mkdir(parents=True)
            for sample in range(1, 6):
                shutil.copyfile(root / "data/01/reference.bin", folder / f"decoded-{sample}.bin")
            row = summarize_session(profile_log([1]), root / "actual", root / "data", (case,), 5, 0, 0, True)[0]
            self.assertFalse(row["passed"])
            self.assertEqual(row["status"], "Timeout")

    def test_runtime_soc_is_detected_and_optional_constraint_is_checked(self):
        case = MOCK_CASES[0]
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            generate_mock_suite(root / "data", (case,))
            folder = root / "actual/01"
            folder.mkdir(parents=True)
            for sample in range(1, 6):
                shutil.copyfile(root / "data/01/reference.bin", folder / f"decoded-{sample}.bin")
            for soc in ("Ascend910B1", "Ascend910B4"):
                with self.subTest(soc=soc):
                    row = summarize_session(profile_log([1], soc=soc), root / "actual", root / "data",
                                            (case,), 5, 0, 0)[0]
                    self.assertTrue(row["passed"])
                    self.assertEqual(row["runtime_soc"], soc)
            log = profile_log([1])
            row = summarize_session(log, root / "actual", root / "data", (case,), 5, 0, 0,
                                    soc="Ascend910B1")[0]
            self.assertTrue(row["passed"])
            row = summarize_session(log, root / "actual", root / "data", (case,), 5, 0, 0,
                                    soc="Ascend910B4")[0]
            self.assertFalse(row["passed"])
            self.assertIn("SoC mismatch", row["profiling_error"])
            for invalid_log in (log.replace("runtime_soc=Ascend910B1\n", ""),
                                log.replace("runtime_soc=Ascend910B1", "runtime_soc="),
                                log + "runtime_soc=Ascend910B4\n"):
                with self.subTest(log=invalid_log):
                    row = summarize_session(invalid_log, root / "actual", root / "data", (case,), 5, 0, 0)[0]
                    self.assertFalse(row["passed"])
                    self.assertEqual(row["status"], "Profiling Error")

    def test_reference_comparison_rejects_bad_scores_nan_and_partial_files(self):
        case = MOCK_CASES[0]
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            generate_mock_suite(root / "data", (case,))
            reference = root / "data/01/reference.bin"
            expected = np.fromfile(reference, dtype="<f4").reshape(-1, 5)
            actual = root / "actual.bin"
            for channel, value in ((0, np.nan), (0, 10000), (4, -1e-6), (4, 1.000001)):
                wrong = expected.copy()
                wrong[0, channel] = value
                wrong.tofile(actual)
                self.assertFalse(compare_output(actual, reference, case)["passed"])
            actual.write_bytes(reference.read_bytes()[:-1])
            with self.assertRaises(ValueError):
                compare_output(actual, reference, case)
            expected.tofile(actual)
            expected[0, 0] = np.nan
            expected.tofile(reference)
            with self.assertRaisesRegex(ValueError, "reference"):
                compare_output(actual, reference, case)

    def test_runtime_failure_and_following_skips_are_preserved(self):
        cases = (MOCK_CASES[0], MOCK_CASES[6])
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            generate_mock_suite(root / "data", cases)
            log = "runtime_soc=Ascend910B1\ncase_begin=01\nrunner_error=kernel launch failed with status=161002\n"
            rows = summarize_session(log, root / "actual", root / "data", cases, 5, 0, 1)
            self.assertEqual([row["status"] for row in rows], ["Runtime Error", "Skipped"])
            self.assertEqual(rows[0]["runtime_codes"], ["161002"])
            self.assertFalse(any(row["passed"] for row in rows))

    def test_fixed_execution_environment_rejects_other_versions_and_architectures(self):
        self.assertEqual(str(DEFAULT_TOOLKIT), "/usr/local/Ascend/cann-9.0.0")
        with patch("tests.common.profiling.platform.machine", return_value="x86_64"):
            with self.assertRaisesRegex(RuntimeError, "ARM Linux"):
                validate_environment(Path("unused"), DEFAULT_TOOLKIT)
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            version = root / "share/info/asc-devkit/version.info"
            version.parent.mkdir(parents=True)
            version.write_text('required_package_runtime_version="8.5.0"\n')
            with patch("tests.common.profiling.platform.system", return_value="Linux"), \
                 patch("tests.common.profiling.platform.machine", return_value="aarch64"):
                with self.assertRaisesRegex(RuntimeError, "CANN 9.0.0"):
                    validate_environment(Path("unused"), root)

    def test_fixture_changes_are_detected(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "data"
            cases = (MOCK_CASES[0],)
            generate_mock_suite(root, cases)
            first = fixture_hashes(root, cases)
            (root / "01/case.conf").write_text("changed")
            self.assertNotEqual(fixture_hashes(root, cases), first)
            (root / "01/raw_pred.bin").write_bytes(b"changed")
            with self.assertRaises(ValueError):
                fixture_hashes(root, cases)

    def test_valid_sdk_and_build_identity_are_required(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            toolkit, build = root / "cann-9.0.0", root / "build"
            for directory in (toolkit / "share/info/asc-devkit", toolkit / "include/acl", toolkit / "bin", build / "autogen"):
                directory.mkdir(parents=True)
            (toolkit / "share/info/asc-devkit/version.info").write_text('required_package_runtime_version="9.0.0"\n')
            (toolkit / "include/acl/acl.h").write_text("unit-test SDK header")
            (build / "autogen/aclnn_fire_candidate_decode_fusion.h").write_text("unit-test generated header")
            (build / "libcust_opapi.so").write_bytes(b"unit-test library")
            profiler = toolkit / "bin/msprof"
            profiler.write_text("#!/bin/sh\nexit 1\n")
            profiler.chmod(0o700)
            cache = build / "CMakeCache.txt"
            cache.write_text(f"ASCEND_CANN_PACKAGE_PATH:PATH={toolkit}\nASCEND_COMPUTE_UNIT:UNINITIALIZED=ascend910b\n")
            with patch("tests.common.profiling.platform.system", return_value="Linux"), \
                 patch("tests.common.profiling.platform.machine", return_value="aarch64"):
                self.assertEqual(validate_environment(build, toolkit), profiler.resolve())
                cache.write_text("ASCEND_CANN_PACKAGE_PATH:PATH=/usr/local/Ascend/cann-8.5.0\n")
                with self.assertRaisesRegex(ValueError, "built with"):
                    validate_environment(build, toolkit)

    def test_incomplete_sessions_cannot_be_hidden_by_equal_total_sample_counts(self):
        def row(values):
            return {"name": "mock_01", "number": 1, "shape": [1, 32, 6], "dtype": "float32",
                    "durations_us": values, "elapsed_us": sum(values) / len(values), "passed": True, "status": "Pass"}
        result = aggregate_sessions([[row([1, 2, 3, 4])], [row([1, 2, 3, 4, 5, 6])]], 5)[0]
        self.assertEqual(result["sample_count"], 10)
        self.assertFalse(result["passed"])
        self.assertIsNone(result["elapsed_us"])

    @unittest.skipUnless(os.name == "posix", "POSIX process groups required")
    def test_timeout_terminates_the_session(self):
        result = run_command([sys.executable, "-c", "import time; time.sleep(30)"], dict(os.environ), .05)
        self.assertTrue(result["timed_out"])
        self.assertNotEqual(result["returncode"], 0)


class ProfileExecutionTests(unittest.TestCase):
    def execute(self, temporary, corrupt=False, *, soc=None, runtime_socs=("Ascend910B1",) * 3):
        root = Path(temporary)
        build = root / "build"
        build.mkdir()
        (build / "libcust_opapi.so").write_bytes(b"unit-test operator identity")
        output = root / "performance output"
        calls = []

        def compile_double(build, toolkit, output, compiler):
            path = output / "runner"
            path.write_bytes(b"orchestration test double; not an executable")
            return path

        def process_double(command, environment, timeout):
            application = shlex.split(next(arg.split("=", 1)[1] for arg in command if arg.startswith("--application=")))
            data = Path(application[application.index("--suite") + 1])
            actual = Path(application[application.index("--output-root") + 1])
            repeat = int(application[application.index("--repeat") + 1])
            if soc is None:
                self.assertNotIn("--soc", application)
            else:
                self.assertEqual(application[application.index("--soc") + 1], soc)
            self.assertIn("--warm-up=5", command)
            self.assertIn("--launch-count=10", command)
            rows = json.loads((data / "cases.json").read_text())
            trial = len(calls)
            for row in rows:
                number = row["number"]
                folder = actual / f"{number:02d}"
                folder.mkdir()
                for sample in range(1, repeat + 1):
                    payload = (data / f"{number:02d}/reference.bin").read_bytes()
                    if corrupt and trial == 0 and sample == 1:
                        payload = bytes(len(payload))
                    (folder / f"decoded-{sample}.bin").write_bytes(payload)
            calls.append(command)
            return {"log": profile_log([row["number"] for row in rows], repeat, trial, runtime_socs[trial]),
                    "pid": 100 + trial, "returncode": 0, "timed_out": False}

        with patch.object(profiling, "validate_environment", return_value=root / "msprof"), \
             patch.object(profiling, "compile_runner", side_effect=compile_double), \
             patch.object(profiling, "runtime_environment", return_value={}), \
             patch.object(profiling, "run_command", side_effect=process_double):
            report = profiling.profile_cases(build, output, (MOCK_CASES[0], MOCK_CASES[6]), soc=soc)
        return report, output, calls

    def test_three_independent_sessions_pool_all_fifteen_samples(self):
        with tempfile.TemporaryDirectory() as temporary:
            report, output, calls = self.execute(temporary)
            self.assertEqual(len(calls), 3)
            self.assertEqual([session["pid"] for session in report["sessions"]], [100, 101, 102])
            self.assertTrue(report["passed"])
            for case in report["cases"]:
                self.assertEqual(case["sample_count"], 15)
                self.assertEqual(case["durations_us"], [5, 6, 7, 8, 9, 7, 8, 9, 10, 11, 9, 10, 11, 12, 13])
                self.assertEqual(case["elapsed_us"], 9)
                self.assertEqual(len(case["sessions"]), 3)
                self.assertTrue(all(len(row["checks"]) == 5 for row in case["sessions"]))
            saved = json.loads((output / "results.json").read_text())
            self.assertEqual(saved["target"], {**TARGET, "soc": "Ascend910B1"})
            self.assertIsNone(saved["expected_soc"])
            self.assertTrue(all(session["soc"] == "Ascend910B1" for session in saved["sessions"]))

    def test_explicit_soc_is_forwarded_and_recorded(self):
        with tempfile.TemporaryDirectory() as temporary:
            report, _, _ = self.execute(temporary, soc="Ascend910B4", runtime_socs=("Ascend910B4",) * 3)
            self.assertTrue(report["passed"])
            self.assertEqual(report["expected_soc"], "Ascend910B4")
            self.assertEqual(report["target"]["soc"], "Ascend910B4")

    def test_soc_mismatch_cannot_pass(self):
        with tempfile.TemporaryDirectory() as temporary:
            report, _, _ = self.execute(temporary, soc="Ascend910B4")
            self.assertFalse(report["passed"])
            self.assertEqual(report["target"]["soc"], "Ascend910B1")
            self.assertTrue(all(case["status"] == "Profiling Error" for case in report["cases"]))

    def test_sessions_from_different_socs_cannot_be_pooled(self):
        with tempfile.TemporaryDirectory() as temporary:
            with self.assertRaisesRegex(ValueError, "SoC changed"):
                self.execute(temporary, runtime_socs=("Ascend910B1", "Ascend910B4", "Ascend910B1"))
            report = json.loads((Path(temporary) / "performance output/results.json").read_text())
            self.assertFalse(report["passed"])
            self.assertEqual([session["soc"] for session in report["sessions"]], ["Ascend910B1", "Ascend910B4"])

    def test_wrong_first_output_cannot_be_hidden_by_later_correct_outputs(self):
        with tempfile.TemporaryDirectory() as temporary:
            report, _, _ = self.execute(temporary, corrupt=True)
            self.assertFalse(report["passed"])
            self.assertTrue(all(case["status"] == "Unstable" for case in report["cases"]))
            self.assertTrue(all(case["sessions"][0]["status"] == "Wrong Answer" for case in report["cases"]))

    def test_invalid_counts_and_overwrite_fail_before_starting_a_profiler(self):
        with tempfile.TemporaryDirectory() as temporary:
            destination = Path(temporary) / "existing"
            destination.mkdir()
            with self.assertRaises(FileExistsError):
                profiling.profile_cases(Path("unused"), destination)
            for values in ({"processes": 0}, {"repeat": 0}, {"warmup": -1}, {"timeout": float("nan")},
                           {"soc": ""}, {"soc": "Ascend 910B4"}):
                with self.subTest(values=values), self.assertRaises(ValueError):
                    profiling.profile_cases(Path("unused"), Path(temporary) / "new", **values)

    @unittest.skipUnless(shutil.which("g++"), "C++ compiler required")
    def test_runner_soc_detection_and_optional_constraint(self):
        acl = '''#pragma once
#include <cstddef>
#include <cstdint>
using aclError = int;
using aclrtStream = void *;
enum aclDataType { ACL_FLOAT, ACL_FLOAT16 };
enum aclFormat { ACL_FORMAT_ND };
enum aclrtMemMallocPolicy { ACL_MEM_MALLOC_HUGE_FIRST };
enum aclrtMemcpyKind { ACL_MEMCPY_HOST_TO_DEVICE, ACL_MEMCPY_DEVICE_TO_HOST };
constexpr aclError ACL_SUCCESS = 0;
extern "C" {
aclError aclInit(const char *);
aclError aclFinalize();
aclError aclrtSetDevice(std::int32_t);
aclError aclrtResetDevice(std::int32_t);
const char *aclrtGetSocName();
aclError aclrtCreateStream(aclrtStream *);
aclError aclrtDestroyStream(aclrtStream);
aclError aclrtSynchronizeStream(aclrtStream);
aclError aclrtMalloc(void **, std::size_t, aclrtMemMallocPolicy);
aclError aclrtFree(void *);
aclError aclrtMemcpy(void *, std::size_t, const void *, std::size_t, aclrtMemcpyKind);
aclError aclrtMemset(void *, std::size_t, int, std::size_t);
}
'''
        meta = '''#pragma once
#include <acl/acl.h>
struct aclTensor;
struct aclOpExecutor;
extern "C" {
aclTensor *aclCreateTensor(const std::int64_t *, std::uint64_t, aclDataType,
                          const std::int64_t *, std::int64_t, aclFormat,
                          const std::int64_t *, std::uint64_t, void *);
int aclDestroyTensor(const aclTensor *);
}
'''
        operator = '''#pragma once
#include <aclnn/acl_meta.h>
extern "C" {
int aclnnFireCandidateDecodeFusionGetWorkspaceSize(const aclTensor *, const aclTensor *,
                                                  const aclTensor *, std::uint64_t *, aclOpExecutor **);
int aclnnFireCandidateDecodeFusion(void *, std::uint64_t, aclOpExecutor *, aclrtStream);
}
'''
        runtime = '''#include <cstdlib>
#include "aclnn_fire_candidate_decode_fusion.h"
extern "C" {
aclError aclInit(const char *) { return 0; }
aclError aclFinalize() { return 0; }
aclError aclrtSetDevice(std::int32_t) { return 0; }
aclError aclrtResetDevice(std::int32_t) { return 0; }
const char *aclrtGetSocName() { return std::getenv("TEST_SOC_NAME"); }
// Stop before device work; reaching this call proves the SoC gate accepted the device.
aclError aclrtCreateStream(aclrtStream *) { return 123; }
aclError aclrtDestroyStream(aclrtStream) { return 0; }
aclError aclrtSynchronizeStream(aclrtStream) { return 0; }
aclError aclrtMalloc(void **, std::size_t, aclrtMemMallocPolicy) { return 0; }
aclError aclrtFree(void *) { return 0; }
aclError aclrtMemcpy(void *, std::size_t, const void *, std::size_t, aclrtMemcpyKind) { return 0; }
aclError aclrtMemset(void *, std::size_t, int, std::size_t) { return 0; }
aclTensor *aclCreateTensor(const std::int64_t *, std::uint64_t, aclDataType, const std::int64_t *,
                          std::int64_t, aclFormat, const std::int64_t *, std::uint64_t, void *) { return nullptr; }
int aclDestroyTensor(const aclTensor *) { return 0; }
int aclnnFireCandidateDecodeFusionGetWorkspaceSize(const aclTensor *, const aclTensor *, const aclTensor *,
                                                  std::uint64_t *, aclOpExecutor **) { return 0; }
int aclnnFireCandidateDecodeFusion(void *, std::uint64_t, aclOpExecutor *, aclrtStream) { return 0; }
}
'''
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "acl").mkdir()
            (root / "aclnn").mkdir()
            (root / "acl/acl.h").write_text(acl)
            (root / "aclnn/acl_meta.h").write_text(meta)
            (root / "aclnn_fire_candidate_decode_fusion.h").write_text(operator)
            (root / "runtime.cpp").write_text(runtime)
            runner = root / "runner"
            result = subprocess.run(["g++", "-std=c++17", "-I", str(root), str(RUNNER_SOURCE),
                                     str(root / "runtime.cpp"), "-o", str(runner)],
                                    capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            for observed, expected, message in (
                ("Ascend910B1", None, "aclrtCreateStream failed with status=123"),
                ("Ascend910B4", None, "aclrtCreateStream failed with status=123"),
                ("Ascend910B1", "Ascend910B1", "aclrtCreateStream failed with status=123"),
                ("Ascend910B1", "Ascend910B4", "runtime SoC mismatch"),
                ("", None, "aclrtGetSocName did not return a SoC name"),
                (None, None, "aclrtGetSocName did not return a SoC name"),
            ):
                with self.subTest(observed=observed, expected=expected):
                    command = [str(runner), "--fixture", str(root), "--output", str(root / "actual.bin")]
                    if expected is not None:
                        command.extend(("--soc", expected))
                    environment = dict(os.environ)
                    environment.pop("TEST_SOC_NAME", None)
                    if observed is not None:
                        environment["TEST_SOC_NAME"] = observed
                    result = subprocess.run(command, env=environment, capture_output=True, text=True)
                    self.assertEqual(result.returncode, 1)
                    self.assertIn(message, result.stderr)
                    if "aclrtCreateStream" not in message:
                        self.assertNotIn("aclrtCreateStream", result.stderr)


if __name__ == "__main__":
    unittest.main()

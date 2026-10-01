"""Device-tool orchestration checks; test doubles never count as NPU validation."""

import contextlib
import io
import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
from unittest.mock import patch

from tests.common.artifacts import file_sha256
from tests.common.generate_data import generate_suite
from tests.common import replay
from tests.correctness.cases import CASES as CORRECTNESS
from tests.coverage.cases import CASES as COVERAGE


class ReplayTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        self.build = self.root / "build"
        (self.build / "autogen").mkdir(parents=True)
        (self.build / "tmp/vendors/custom").mkdir(parents=True)
        (self.build / "libcust_opapi.so").write_bytes(b"unit-test library identity")
        (self.build / "autogen" / replay.HEADER_NAME).write_text("unit-test header identity\n")
        self.toolkit = self.root / "toolkit"
        for relative in ("include/acl/acl.h", "include/aclnn/acl_meta.h"):
            path = self.toolkit / relative
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text("unit-test descriptor\n")
        self.data_root = self.root / "data"
        self.output = self.root / "run"

    def compile_double(self, build, output, toolkit, compiler):
        runner = output / "runner"
        runner.write_bytes(b"orchestration test double; not an executable")
        return runner

    def successful_process(self, command, **kwargs):
        fixture = Path(command[command.index("--fixture") + 1])
        actual = Path(command[command.index("--output") + 1])
        group = fixture.parent.name
        source = self.data_root / group / fixture.name
        # This return path is confined to the unit test, not replay.py/runner.cpp.
        actual.write_bytes((source / "reference_decoded.bin").read_bytes())
        return subprocess.CompletedProcess(command, 0, "device_output=PASS\n", "")

    def run_replay(self, cases, group="correctness", process=None):
        generate_suite(self.data_root / group, cases)
        with patch.object(replay, "compile_runner", self.compile_double), \
             patch.object(replay.subprocess, "run", side_effect=process or self.successful_process), \
             contextlib.redirect_stdout(io.StringIO()):
            return replay.replay_suites(self.build, self.output, category=group, toolkit=self.toolkit,
                                       data_root=self.data_root, names=tuple(case.name for case in cases))

    def test_success_report_records_binary_inputs_logs_and_comparison(self):
        cases = (CORRECTNESS[0], CORRECTNESS[1])
        report = self.run_replay(cases)
        self.assertTrue(report["passed"])
        self.assertEqual(report["summary"]["correctness"], {"total": 2, "passed": 2, "failed": 0, "skipped": 0})
        self.assertEqual(report["op_library_sha256"], file_sha256(self.build / "libcust_opapi.so"))
        stored = json.loads((self.output / "results.json").read_text())
        self.assertTrue(stored["passed"])
        for row in report["cases"]:
            self.assertTrue(row["comparison"]["passed"])
            self.assertTrue(Path(row["runner_log"]).is_file())
            self.assertEqual(set(row["input_sha256"]), {"raw_pred", "priors"})

    def test_wrong_output_is_reported_without_stopping_other_cases(self):
        def wrong_process(command, **kwargs):
            result = self.successful_process(command, **kwargs)
            path = Path(command[command.index("--output") + 1])
            path.write_bytes(bytes(len(path.read_bytes())))
            return result
        report = self.run_replay((CORRECTNESS[0], CORRECTNESS[1]), process=wrong_process)
        self.assertFalse(report["passed"])
        self.assertEqual(report["summary"]["correctness"]["failed"], 2)
        self.assertEqual({row["status"] for row in report["cases"]}, {"Wrong Answer"})

    def test_runtime_error_cannot_be_hidden_by_an_output_file(self):
        def failing_process(command, **kwargs):
            self.successful_process(command, **kwargs)
            return subprocess.CompletedProcess(command, 7, "", "workspace failure\n")
        report = self.run_replay((CORRECTNESS[0],), process=failing_process)
        self.assertFalse(report["passed"])
        self.assertEqual(report["cases"][0]["status"], "Runtime Error")
        self.assertEqual(report["cases"][0]["runner_returncode"], 7)

    def test_timeout_records_log_and_failure(self):
        def timeout(command, **kwargs):
            raise subprocess.TimeoutExpired(command, kwargs["timeout"], output=b"partial output\n")
        report = self.run_replay((CORRECTNESS[0],), process=timeout)
        self.assertFalse(report["passed"])
        row = report["cases"][0]
        self.assertEqual(row["status"], "Timeout")
        self.assertIn("execution timeout", Path(row["runner_log"]).read_text())

    def test_process_start_failure_is_a_runtime_error_with_a_log(self):
        def cannot_launch(command, **kwargs):
            raise OSError("cannot launch device executable")
        report = self.run_replay((CORRECTNESS[0],), process=cannot_launch)
        self.assertFalse(report["passed"])
        row = report["cases"][0]
        self.assertEqual(row["status"], "Runtime Error")
        self.assertIn("cannot launch", Path(row["runner_log"]).read_text())

    def test_source_bundle_changes_during_execution_are_detected(self):
        def change_bundle(command, **kwargs):
            result = self.successful_process(command, **kwargs)
            manifest = self.data_root / "correctness/manifest.json"
            manifest.write_text(manifest.read_text() + "\n")
            return result
        report = self.run_replay((CORRECTNESS[0],), process=change_bundle)
        self.assertFalse(report["passed"])
        self.assertEqual(report["cases"][0]["status"], "Data Error")
        self.assertIn("changed during", report["cases"][0]["detail"])

    def test_rejection_fixtures_remain_local_and_are_skipped_on_device(self):
        valid = COVERAGE[0]
        rejected = next(case for case in COVERAGE if case.expectation == "reject")
        report = self.run_replay((valid, rejected), "coverage")
        self.assertTrue(report["passed"])
        self.assertEqual(report["summary"]["coverage"]["skipped"], 1)
        self.assertEqual(report["cases"][1]["status"], "Skipped")
        self.assertFalse((self.output / "cases/coverage" / rejected.name).exists())

    def test_mutated_execution_inputs_are_detected_after_process(self):
        def corrupt_process(command, **kwargs):
            result = self.successful_process(command, **kwargs)
            fixture = Path(command[command.index("--fixture") + 1])
            path = fixture / "raw_pred.bin"
            path.write_bytes(b"changed input")
            return result
        report = self.run_replay((CORRECTNESS[0],), process=corrupt_process)
        self.assertFalse(report["passed"])
        self.assertEqual(report["cases"][0]["status"], "Data Error")

    def test_corrupt_cached_source_fails_before_compiling_or_launching(self):
        generate_suite(self.data_root / "correctness", (CORRECTNESS[0],))
        path = self.data_root / "correctness" / CORRECTNESS[0].name / "priors.bin"
        path.write_bytes(b"damaged")
        with patch.object(replay, "compile_runner") as compile_call:
            with self.assertRaises(ValueError):
                replay.replay_suites(self.build, self.output, category="correctness", toolkit=self.toolkit,
                                     data_root=self.data_root, names=(CORRECTNESS[0].name,))
            compile_call.assert_not_called()
        self.assertFalse(self.output.exists())

    def test_missing_cached_cases_cannot_silently_reduce_coverage(self):
        generate_suite(self.data_root / "correctness", (CORRECTNESS[0],))
        with self.assertRaisesRegex(ValueError, "missing selected cases"):
            replay.replay_suites(self.build, self.output, category="correctness", toolkit=self.toolkit,
                                 data_root=self.data_root)
        self.assertFalse(self.output.exists())

    def test_compile_failure_preserves_a_failure_report(self):
        with patch.object(replay, "compile_runner", side_effect=RuntimeError("compiler failed")):
            with self.assertRaises(RuntimeError):
                replay.replay_suites(self.build, self.output, category="correctness", toolkit=self.toolkit,
                                     names=(CORRECTNESS[0].name,))
        report = json.loads((self.output / "results.json").read_text())
        self.assertFalse(report["passed"])
        self.assertEqual(report["setup_error"], "compiler failed")

    def test_fresh_generation_runs_without_a_cached_data_root(self):
        # Resolve the references from the newly generated output/data directory
        # only inside this process double.
        self.data_root = self.output / "data"
        with patch.object(replay, "compile_runner", self.compile_double), \
             patch.object(replay.subprocess, "run", side_effect=self.successful_process), \
             contextlib.redirect_stdout(io.StringIO()):
            report = replay.replay_suites(self.build, self.output, category="correctness", toolkit=self.toolkit,
                                          names=(CORRECTNESS[0].name,))
        self.assertTrue(report["passed"])
        self.assertTrue((self.data_root / "correctness/manifest.json").is_file())

    def test_runtime_environment_preserves_existing_custom_paths(self):
        with patch.dict(replay.os.environ, {"ASCEND_CUSTOM_OPP_PATH": "/existing/vendor", "LD_LIBRARY_PATH": "/existing/lib"}):
            environment = replay.runtime_environment(self.build, self.toolkit, self.build / "tmp/vendors/custom")
        self.assertTrue(environment["ASCEND_CUSTOM_OPP_PATH"].endswith(":/existing/vendor"))
        self.assertTrue(environment["LD_LIBRARY_PATH"].endswith(":/existing/lib"))

    def test_toolkit_selection_uses_recorded_build_and_explicit_override(self):
        (self.build / "CMakeCache.txt").write_text("ASCEND_CANN_PACKAGE_PATH:PATH=" + str(self.toolkit) + "\n")
        self.assertEqual(replay.toolkit_root(self.build), self.toolkit.resolve())
        with self.assertRaises(RuntimeError):
            replay.toolkit_root(self.build, self.root / "missing")

    def test_invalid_selection_device_and_timeout_are_rejected(self):
        rejected = next(case for case in COVERAGE if case.expectation == "reject")
        for arguments in ({"names": ("missing",)}, {"category": "mock"}, {"device": -1},
                          {"timeout": float("nan")}, {"names": (rejected.name,), "category": "coverage"}):
            with self.subTest(arguments=arguments):
                with self.assertRaises(ValueError):
                    replay.replay_suites(self.build, self.output, toolkit=self.toolkit, **arguments)
        self.assertFalse(self.output.exists())

    def test_runner_source_compiles_against_declaration_only_contract(self):
        compiler = shutil.which("c++")
        if compiler is None:
            self.skipTest("C++ compiler unavailable")
        include = Path(__file__).with_name("runner_abi")
        result = subprocess.run([compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-fsyntax-only",
                                 "-I", str(include), str(replay.SOURCE)], capture_output=True, text=True, check=False)
        self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == "__main__":
    unittest.main()

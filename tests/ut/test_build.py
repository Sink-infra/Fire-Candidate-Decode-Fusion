"""Compiled package identities and profiler statistics."""

import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from tests.common.build import record_build, verify_build, host_configuration
from tests.common.mock_cases import MOCK_RUNTIME, load_mock_runtime
from tests.common.profiling import aggregate_sessions


class BuildIdentityTests(unittest.TestCase):
    def package(self, directory, steps=None):
        build = Path(directory)
        (build / "libcust_opapi.so").write_bytes(b"operator")
        folder = build / "binary"
        folder.mkdir()
        (folder / "device.o").write_bytes(b"kernel")
        source = {"commit": "a" * 40, "code_tree": "b" * 40}
        toolkit = {"path": "/sdk", "version": "9.0.0", "version_sha256": "c" * 64}
        return build, source, toolkit, steps or [{"command": ["cmake", "--build", str(build), "-j28"], "returncode": 0}]

    def test_host_standard_compiler_and_abi_are_checked_from_generated_flags(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "CMakeFiles/3.22.1").mkdir(parents=True)
            (root / "op_host/CMakeFiles/cust_optiling.dir").mkdir(parents=True)
            (root / "CMakeCache.txt").write_text("CMAKE_CXX_STANDARD:UNINITIALIZED=11\n")
            (root / "CMakeFiles/3.22.1/CMakeCXXCompiler.cmake").write_text(
                'set(CMAKE_CXX_COMPILER_ID "GNU")\nset(CMAKE_CXX_COMPILER_VERSION "11.4.0")\n')
            flags = root / "op_host/CMakeFiles/cust_optiling.dir/flags.make"
            flags.write_text("CXX_FLAGS = -std=c++11\nCXX_DEFINES = -D_GLIBCXX_USE_CXX11_ABI=0\n")
            self.assertEqual(host_configuration(root)["abi"], 0)
            flags.write_text(flags.read_text().replace("ABI=0", "ABI=1"))
            with self.assertRaisesRegex(ValueError, "ABI=0"):
                host_configuration(root)

    def test_kernel_corruption_is_not_hidden_by_an_unchanged_host_library(self):
        with tempfile.TemporaryDirectory() as directory:
            build, source, toolkit, steps = self.package(directory)
            with patch("tests.common.build.source_identity", return_value=source), \
                 patch("tests.common.build.toolkit_identity", return_value=toolkit):
                record_build(build, source["commit"], steps)
                self.assertEqual(verify_build(build)["source"], source)
                (build / "binary/device.o").write_bytes(b"different kernel")
                with self.assertRaisesRegex(ValueError, "artifacts changed"):
                    verify_build(build)

    def test_exact_commit_and_successful_final_build_are_required(self):
        with tempfile.TemporaryDirectory() as directory:
            build, source, toolkit, steps = self.package(directory)
            with patch("tests.common.build.source_identity", side_effect=lambda value: {**source, "commit": value}), \
                 patch("tests.common.build.toolkit_identity", return_value=toolkit):
                record_build(build, source["commit"], steps)
                with self.assertRaisesRegex(ValueError, "requested source"):
                    verify_build(build, "d" * 40)
                record_build(build, source["commit"], [{**steps[0], "returncode": 2}], complete=False)
                with self.assertRaisesRegex(ValueError, "successful final"):
                    verify_build(build)

    def test_new_runtime_configuration_is_shared_by_all_mock_cases(self):
        self.assertEqual((MOCK_RUNTIME["warmup"], MOCK_RUNTIME["repeat"], MOCK_RUNTIME["processes"]), (5, 5, 3))
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "mock.json"
            path.write_text(json.dumps({"runtime": MOCK_RUNTIME, "cases": [{"runtime": {"warmup": 1}}]}))
            with self.assertRaisesRegex(ValueError, "whole suite"):
                load_mock_runtime(path)
            for key, value in (("warmup", 0), ("metric", "BasicInfo")):
                path.write_text(json.dumps({"runtime": {**MOCK_RUNTIME, key: value}, "cases": []}))
                with self.assertRaisesRegex(ValueError, "invalid shared"):
                    load_mock_runtime(path)

    def test_skewed_samples_keep_official_mean_and_median_separate(self):
        row = {"name": "mock_01", "number": 1, "shape": [1, 32, 6], "dtype": "float32",
               "status": "Pass", "passed": True, "durations_us": [1, 1, 1, 1, 100],
               "elapsed_us": 1, "checks": [{"error_ratio": "0.00%"}]}
        result = aggregate_sessions([[row], [row], [row]], 5)[0]
        self.assertEqual(result["sample_count"], 15)
        self.assertEqual(result["median_us"], 1)
        self.assertAlmostEqual(result["mean_us"], 20.8)
        self.assertAlmostEqual(result["elapsed_us"], 20.8)
        self.assertEqual(result["iqr_us"], 0)
        self.assertEqual((result["min_us"], result["max_us"]), (1, 100))

"""Input materialization, binary layouts, metadata, and generation checks."""

from dataclasses import replace
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

import numpy as np

from tests.common.cases import COVERAGE_CASES as CASES, CORRECTNESS_CASES
from tests.common.tensor_io import generate_suite
from tests.common.reference import materialize_case
from tests.ut.helpers import check_generated_data


class DataGenerationTests(unittest.TestCase):
    def test_coverage_generated_data(self):
        check_generated_data(self, CASES, "coverage")

    def test_materialization_is_repeatable(self):
        for case in CASES:
            with self.subTest(case=case.name):
                first, second = materialize_case(case), materialize_case(case)
                np.testing.assert_array_equal(first.raw_pred.storage, second.raw_pred.storage)
                np.testing.assert_array_equal(first.priors.storage, second.priors.storage)

    def test_noncontiguous_fixtures_preserve_physical_padding(self):
        for case in CASES:
            if case.raw_pred_contiguous and case.priors_contiguous:
                continue
            data = materialize_case(case)
            tensor = data.raw_pred if not case.raw_pred_contiguous else data.priors
            self.assertFalse(tensor.values.flags.c_contiguous)
            self.assertEqual(tensor.storage.size, 2 * tensor.values.size)
            self.assertTrue(np.all(tensor.storage[..., 1::2] == 0))

    def test_generator_rejects_mixed_categories_duplicate_and_unsafe_names(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "suite"
            for cases in (
                (), (CASES[0], replace(CASES[1], category="correctness")),
                (CASES[0], CASES[0]), (replace(CASES[0], name="../outside"),),
            ):
                with self.subTest(cases=tuple(case.name for case in cases)):
                    with self.assertRaises(ValueError):
                        generate_suite(root, cases)
                    self.assertFalse(root.exists())

    def test_catalog_audit_refuses_incorrect_expectations(self):
        invalid = replace(CASES[0], priors_dtype="float16")
        wrong_error = replace(CASES[-1], expected_error="RAW_PRED_RANK")
        with tempfile.TemporaryDirectory() as temporary:
            for index, case in enumerate((invalid, wrong_error)):
                destination = Path(temporary) / str(index)
                with self.assertRaises(ValueError):
                    generate_suite(destination, (case,))
                self.assertFalse(destination.exists())
                self.assertEqual(list(Path(temporary).iterdir()), [])
                generate_suite(destination, (CASES[0],))
                self.assertTrue((destination / "manifest.json").is_file())
                # Use another temporary parent for the next audit/retry.
                shutil.rmtree(destination)

    def test_generation_write_failure_cleans_staging_and_can_retry(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "suite"
            with patch("tests.common.tensor_io._write_array", side_effect=OSError("injected disk failure")):
                with self.assertRaises(OSError):
                    generate_suite(root, (CASES[0],))
            self.assertFalse(root.exists())
            self.assertEqual(list(Path(temporary).iterdir()), [])
            generate_suite(root, (CASES[0],))
            self.assertTrue((root / "manifest.json").is_file())

    def test_publication_failure_cleans_owned_directory_and_can_retry(self):
        original_rename = Path.rename
        def fail_manifest(path, target):
            if path.name == "manifest.json":
                raise OSError("injected publication failure")
            return original_rename(path, target)
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "suite"
            with patch.object(Path, "rename", fail_manifest):
                with self.assertRaises(OSError):
                    generate_suite(root, (CASES[0],))
            self.assertFalse(root.exists())
            self.assertEqual(list(Path(temporary).iterdir()), [])
            generate_suite(root, (CASES[0],))
            self.assertTrue((root / "manifest.json").is_file())

    def test_correctness_generated_data(self):
        check_generated_data(self, CORRECTNESS_CASES, "correctness")

    def test_cli_lists_and_generates_named_suites(self):
        command = [sys.executable, "-m", "tests.st.scripts.gen_data"]
        with tempfile.TemporaryDirectory() as temporary:
            for category, count in (("coverage", 55), ("correctness", 28), ("mock", 12)):
                destination = Path(temporary) / category
                result = subprocess.run(
                    command + ["--suite", category, "--list", "--output", str(destination)],
                    capture_output=True, text=True,
                )
                self.assertEqual(result.returncode, 0, result.stderr)
                listed = json.loads(result.stdout)
                self.assertEqual(len(listed), count)
                self.assertTrue(all(case["category"] == category for case in listed))
                self.assertFalse(destination.exists())
            for category, name, filters in (
                ("coverage", "coverage_reject_n_mismatch", ["--expectation", "reject"]),
                ("correctness", "correctness_fp32_documented_zero", ["--tag", "example_1"]),
            ):
                destination = Path(temporary) / category
                result = subprocess.run(
                    command + ["--suite", category, "--case", name, "--output", str(destination)] + filters,
                    capture_output=True, text=True,
                )
                self.assertEqual(result.returncode, 0, result.stderr)
                manifest = json.loads((destination / "manifest.json").read_text())
                self.assertEqual([case["name"] for case in manifest["cases"]], [name])
                if category == "coverage":
                    self.assertFalse((destination / name / "reference_decoded.bin").exists())
                else:
                    reference = np.fromfile(destination / name / "reference_decoded.bin", dtype="<f4")
                    np.testing.assert_array_equal(reference, [16, 16, 64, 64, .25])


if __name__ == "__main__":
    unittest.main()

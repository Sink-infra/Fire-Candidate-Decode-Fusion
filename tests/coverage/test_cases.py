"""Catalog, input-contract validation, serialization, and generation checks."""

from dataclasses import replace
from pathlib import Path
import shutil
import tempfile
import unittest
from unittest.mock import patch

import numpy as np

from tests.__main__ import summary
from tests.common.checks import check_declarations, check_generated_data
from tests.common.generate_data import _select_cases, generate_suite
from tests.common.reference import CaseData, infer_output_shape, materialize_case, tensor_value, validate_case
from tests.common.verify_result import verify_case_output
from tests.coverage.cases import CASES


class CoverageTests(unittest.TestCase):
    def test_declarations(self):
        check_declarations(self, CASES, "coverage")

    def test_generated_data(self):
        check_generated_data(self, CASES, "coverage")

    def test_supported_range_and_tail_matrix(self):
        success = [case for case in CASES if case.expectation == "success"]
        self.assertEqual({case.raw_pred_shape[0] for case in success}, set(range(1, 9)))
        for dtype in ("float16", "float32"):
            shapes = {case.raw_pred_shape for case in success if case.raw_pred_dtype == dtype}
            for n in (1, 31, 32, 33, 63, 64, 65, 32768):
                self.assertTrue(any(shape[1] == n for shape in shapes), (dtype, n))
            self.assertIn((8, 32768, 6), shapes)

    def test_summary_has_only_the_two_requested_suites(self):
        report = summary()
        self.assertEqual(set(report), {"coverage", "correctness"})
        self.assertEqual(report["coverage"]["count"], len(CASES))
        self.assertEqual(report["coverage"]["max_total_candidates"], 8 * 32768)

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

    def test_fractional_grid_stride_and_anchors_are_legal(self):
        case = next(case for case in CASES if case.prior_pattern == "fractional")
        self.assertEqual(validate_case(case, materialize_case(case)).errors, ())

    def test_value_validation_checks_actual_arrays(self):
        case = CASES[0]
        data = materialize_case(case)
        raw = data.raw_pred.values.copy()
        raw[0, 0, 5] = np.nan
        self.assertIn("RAW_PRED_NONFINITE", validate_case(case, CaseData(tensor_value(raw, "float32"), data.priors)).errors)
        priors = data.priors.values.copy()
        priors[0, 2] = 0
        self.assertIn("STRIDE_NONPOSITIVE", validate_case(case, CaseData(data.raw_pred, tensor_value(priors, "float32"))).errors)

    def test_output_shape_uses_batch_and_candidate_dimensions(self):
        for case in CASES:
            if case.expectation == "success":
                self.assertEqual(infer_output_shape(case), case.raw_pred_shape[:2] + (5,))
        invalid = next(case for case in CASES if case.expected_error == "N_MISMATCH")
        with self.assertRaises(ValueError):
            infer_output_shape(invalid)

    def test_selection_filters_without_combining_categories(self):
        name = next(case.name for case in CASES if case.expectation == "reject")
        selected = _select_cases(CASES, "reject", (name,))
        self.assertEqual(tuple(case.name for case in selected), (name,))
        with self.assertRaisesRegex(ValueError, "unknown case"):
            _select_cases(CASES, "all", ("missing",))
        with self.assertRaisesRegex(ValueError, "no cases"):
            _select_cases(CASES, "success", (name,))

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
            with patch("tests.common.generate_data._write_array", side_effect=OSError("injected disk failure")):
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

    def test_verifier_refuses_rejection_fixtures(self):
        case = next(case for case in CASES if case.expectation == "reject")
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "suite"
            generate_suite(root, (case,))
            with self.assertRaisesRegex(ValueError, "rejection fixtures"):
                verify_case_output(root / case.name, root / "actual.bin")


if __name__ == "__main__":
    unittest.main()

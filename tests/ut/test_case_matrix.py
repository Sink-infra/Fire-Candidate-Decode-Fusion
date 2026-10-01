"""Catalog, input-contract validation, serialization, and generation checks."""

import json
from pathlib import Path
import tempfile
import unittest

import numpy as np

from tests.__main__ import summary
from tests.ut.helpers import check_declarations
from tests.common.cases import CASE_ROOT, load_cases, select_cases
from tests.common.reference import CaseData, infer_output_shape, materialize_case, tensor_value, validate_case
from tests.common.cases import COVERAGE_CASES as CASES


class CoverageTests(unittest.TestCase):
    def test_declarations(self):
        check_declarations(self, CASES, "coverage")

    def test_supported_range_and_tail_matrix(self):
        success = [case for case in CASES if case.expectation == "success"]
        self.assertEqual({case.raw_pred_shape[0] for case in success}, set(range(1, 9)))
        for dtype in ("float16", "float32"):
            shapes = {case.raw_pred_shape for case in success if case.raw_pred_dtype == dtype}
            for n in (1, 31, 32, 33, 63, 64, 65, 32768):
                self.assertTrue(any(shape[1] == n for shape in shapes), (dtype, n))
            self.assertIn((8, 32768, 6), shapes)

    def test_summary_reports_all_three_suites(self):
        report = summary()
        self.assertEqual(set(report), {"coverage", "correctness", "mock"})
        self.assertEqual(report["mock"]["count"], 12)
        self.assertEqual(report["coverage"]["count"], len(CASES))
        self.assertEqual(report["coverage"]["max_total_candidates"], 8 * 32768)

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
        selected = select_cases("coverage", expectation="reject", names=(name,))
        self.assertEqual(tuple(case.name for case in selected), (name,))
        with self.assertRaisesRegex(ValueError, "unknown case"):
            select_cases("coverage", names=("missing",))
        with self.assertRaisesRegex(ValueError, "no cases"):
            select_cases("coverage", expectation="success", names=(name,))

    def test_selection_requires_all_tags_and_stays_in_the_named_suite(self):
        cases = select_cases("correctness", tags=("batch_shared_priors", "tail"))
        self.assertEqual({case.name for case in cases}, {
            "correctness_fp32_batch_prior_tail", "correctness_fp16_batch_prior_tail",
        })
        with self.assertRaisesRegex(ValueError, "unknown case"):
            select_cases("coverage", names=("mock_01",))
        with self.assertRaisesRegex(ValueError, "no cases"):
            select_cases("correctness", tags=("batch_shared_priors", "missing"))
        with self.assertRaisesRegex(ValueError, "unknown suite"):
            select_cases("missing")

    def test_loader_rejects_malformed_catalogs(self):
        original = json.loads((CASE_ROOT / "coverage.json").read_text())
        for change in ("category", "empty", "duplicate", "unknown_field", "unsafe_name", "shape", "seed", "expectation"):
            with self.subTest(change=change), tempfile.TemporaryDirectory() as temporary:
                document = json.loads(json.dumps(original))
                case = document["cases"][0]
                if change == "category":
                    document["category"] = "correctness"
                elif change == "empty":
                    document["cases"] = []
                elif change == "duplicate":
                    document["cases"].append(case)
                elif change == "unknown_field":
                    case["unknown"] = 1
                elif change == "unsafe_name":
                    case["name"] = "../outside"
                elif change == "shape":
                    case["raw_pred_shape"] = [1, "1", 6]
                elif change == "seed":
                    case["seed"] = -1
                elif change == "expectation":
                    case["expectation"] = "reject"
                root = Path(temporary)
                (root / "coverage.json").write_text(json.dumps(document))
                with self.assertRaises(ValueError):
                    load_cases("coverage", root)


if __name__ == "__main__":
    unittest.main()

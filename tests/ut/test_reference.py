"""Independent formula, reference precision, and case sensitivity checks."""

from dataclasses import replace
import math
import unittest

import numpy as np

from tests.common.cases import TOLERANCES
from tests.ut.helpers import check_declarations, scalar_decode
from tests.common.reference import (
    CaseData, fire_candidate_decode_fusion_reference, materialize_case, sigmoid_fp32, tensor_value,
)
from tests.common.cases import CORRECTNESS_CASES as CASES


def named(name):
    return next(case for case in CASES if case.name == "correctness_" + name)


class CorrectnessTests(unittest.TestCase):
    def test_declarations(self):
        check_declarations(self, CASES, "correctness")

    def test_documented_zero_is_exact(self):
        for dtype in ("float32", "float16"):
            case = named(dtype.replace("float", "fp") + "_documented_zero")
            result = fire_candidate_decode_fusion_reference(case, materialize_case(case))
            np.testing.assert_array_equal(result.values, np.asarray([[[16, 16, 64, 64, 0.25]]], dtype=dtype))

    def test_reference_matches_independent_scalar_formula(self):
        patterns = {"zeros", "documented_positive", "documented_negative", "channel_probe", "indexed", "mixed_confidence"}
        for case in CASES:
            if case.input_pattern not in patterns or "overflow" in case.name:
                continue
            with self.subTest(case=case.name):
                data = materialize_case(case)
                result = fire_candidate_decode_fusion_reference(case, data)
                b, n, _ = case.raw_pred_shape
                for batch in range(b):
                    for candidate in sorted({0, n // 2, n - 1}):
                        expected = scalar_decode(data.raw_pred.values[batch, candidate], data.priors.values[candidate])
                        expected = expected.astype(case.raw_pred_dtype)
                        atol, rtol = TOLERANCES[case.raw_pred_dtype]
                        np.testing.assert_allclose(result.values[batch, candidate], expected, atol=atol, rtol=rtol)

    def test_negative_centers_are_preserved(self):
        case = named("fp32_documented_negative")
        result = fire_candidate_decode_fusion_reference(case, materialize_case(case))
        self.assertTrue(np.all(result.values[..., :2] < 0))
        self.assertTrue(np.all(result.values[..., 2:4] > 0))

    def test_confidence_is_a_product(self):
        case = named("fp32_confidence_product")
        data = materialize_case(case)
        result = fire_candidate_decode_fusion_reference(case, data).values[0, :, 4]
        expected = np.asarray([
            (1 / (1 + math.exp(-float(row[4])))) * (1 / (1 + math.exp(-float(row[5]))))
            for row in data.raw_pred.values[0]
        ])
        np.testing.assert_allclose(result, expected, atol=1e-5, rtol=1e-5)
        self.assertEqual(result[0], result[1])
        self.assertGreater(result[2], result[0])
        self.assertGreater(result[0], result[3])
        self.assertEqual(result[4], np.float32(0.25))

    def test_half_reference_promotes_actual_half_inputs_and_casts_only_at_end(self):
        for case in CASES:
            if case.raw_pred_dtype != "float16":
                continue
            with self.subTest(case=case.name):
                data = materialize_case(case)
                half_result = fire_candidate_decode_fusion_reference(case, data)
                promoted_data = CaseData(tensor_value(data.raw_pred.values, "float32"), data.priors)
                fp32_result = fire_candidate_decode_fusion_reference(replace(case, raw_pred_dtype="float32"), promoted_data)
                self.assertEqual(half_result.fp32.dtype, np.dtype("float32"))
                self.assertEqual(half_result.values.dtype, np.dtype("float16"))
                np.testing.assert_array_equal(half_result.fp32, fp32_result.values)
                with np.errstate(over="ignore"):
                    np.testing.assert_array_equal(half_result.values, fp32_result.values.astype(np.float16))
                self.assertEqual(data.priors.values.dtype, np.dtype("float32"))

    def test_half_cancellation_case_detects_half_intermediate_math(self):
        case = named("fp16_cancellation")
        data = materialize_case(case)
        result = fire_candidate_decode_fusion_reference(case, data)
        half_raw = data.raw_pred.values
        # Deliberately incorrect implementation: sigmoid and centers in FP16.
        with np.errstate(over="ignore", under="ignore"):
            activation = np.divide(np.float16(1), np.add(np.float16(1), np.exp(-half_raw), dtype=np.float16), dtype=np.float16)
            centers = np.subtract(np.multiply(activation[..., :2], np.float16(2), dtype=np.float16), np.float16(0.5), dtype=np.float16)
            wrong = result.values.copy()
            wrong[..., :2] = np.multiply(centers, np.float16(1024), dtype=np.float16)
        self.assertNotEqual(result.values[0, 0, 0], wrong[0, 0, 0])
        atol, rtol = TOLERANCES[case.raw_pred_dtype]
        self.assertTrue(np.any(~np.isclose(wrong[..., 0], result.values[..., 0], atol=atol, rtol=rtol)))

    def test_width_underflow_case_detects_half_sigmoid_and_square(self):
        case = named("fp16_width_underflow")
        data = materialize_case(case)
        expected = fire_candidate_decode_fusion_reference(case, data).values
        with np.errstate(over="ignore", under="ignore"):
            raw = data.raw_pred.values[..., 2:4]
            activation = np.divide(np.float16(1), np.add(np.float16(1), np.exp(-raw), dtype=np.float16), dtype=np.float16)
            doubled = np.multiply(np.float16(2), activation, dtype=np.float16)
            sizes = np.multiply(doubled, doubled, dtype=np.float16).astype(np.float32)
            wrong = expected.copy()
            wrong[..., 2:4] = sizes * data.priors.values[None, :, 3:5] * data.priors.values[None, :, 2:3]
        self.assertTrue(np.all(expected[..., 2:4] > 0.015))
        self.assertTrue(np.all(wrong[..., 2:4] == 0))
        atol, rtol = TOLERANCES[case.raw_pred_dtype]
        self.assertTrue(np.all(~np.isclose(wrong[..., 2:4], expected[..., 2:4], atol=atol, rtol=rtol)))

    def test_large_priors_case_detects_half_downcast(self):
        case = named("fp16_large_float32_priors")
        data = materialize_case(case)
        expected = fire_candidate_decode_fusion_reference(case, data).values
        with np.errstate(over="ignore"):
            priors = data.priors.values.astype(np.float16).astype(np.float32)
            activation = sigmoid_fp32(data.raw_pred.values)
            sizes = (np.float32(2) * activation[..., 2:4]) ** np.float32(2)
            wrong = expected.copy()
            wrong[..., 2:4] = sizes * priors[None, :, 3:5] * priors[None, :, 2:3]
        self.assertTrue(np.isfinite(expected).all())
        self.assertTrue(np.isposinf(wrong[..., 2:4]).all())

    def test_width_and_prior_precision_cases_match_scalar_formula(self):
        for name in ("fp16_width_underflow", "fp16_large_float32_priors"):
            case = named(name)
            data = materialize_case(case)
            result = fire_candidate_decode_fusion_reference(case, data).values
            expected = scalar_decode(data.raw_pred.values[0, 0], data.priors.values[0]).astype(np.float16)
            np.testing.assert_array_equal(result[0, 0], expected)

    def test_identical_raw_batches_reuse_identical_candidate_priors(self):
        case = named("fp32_batch_prior_tail")
        data = materialize_case(case)
        repeated = np.repeat(data.raw_pred.values[:1], case.raw_pred_shape[0], axis=0)
        repeated_data = CaseData(tensor_value(repeated, "float32"), data.priors)
        result = fire_candidate_decode_fusion_reference(case, repeated_data).values
        for batch in range(1, case.raw_pred_shape[0]):
            np.testing.assert_array_equal(result[batch], result[0])
        self.assertFalse(np.array_equal(result[0, 0], result[0, -1]))

    def test_candidate_permutation_keeps_pairing_and_output_order(self):
        for dtype in ("float32", "float16"):
            case = named(dtype.replace("float", "fp") + "_batch_prior_tail")
            data = materialize_case(case)
            expected = fire_candidate_decode_fusion_reference(case, data).values
            permutation = np.arange(case.raw_pred_shape[1])[::-1]
            paired = CaseData(
                tensor_value(data.raw_pred.values[:, permutation, :], dtype),
                tensor_value(data.priors.values[permutation], "float32"),
            )
            np.testing.assert_array_equal(fire_candidate_decode_fusion_reference(case, paired).values, expected[:, permutation])
            wrong_pairing = CaseData(data.raw_pred, paired.priors)
            wrong = fire_candidate_decode_fusion_reference(case, wrong_pairing).values
            self.assertFalse(np.array_equal(wrong[..., :4], expected[..., :4]))
            np.testing.assert_array_equal(wrong[..., 4], expected[..., 4])

    def test_low_scores_retain_all_coordinates_and_candidates(self):
        for dtype in ("float32", "float16"):
            case = named(dtype.replace("float", "fp") + "_no_filter")
            result = fire_candidate_decode_fusion_reference(case, materialize_case(case))
            self.assertEqual(result.values.shape, (4, 65, 5))
            self.assertTrue(np.all(result.fp32[..., 4] > 0))
            self.assertTrue(np.all(result.fp32[..., 4] < 1e-20))
            self.assertTrue(np.all(result.values[..., 2:4] > 0))
            if dtype == "float16":
                self.assertTrue(np.all(result.values[..., 4] == 0))

    def test_finite_extreme_logits_saturate_without_nan(self):
        for dtype in ("float32", "float16"):
            case = named(dtype.replace("float", "fp") + "_saturation")
            data = materialize_case(case)
            self.assertTrue(np.isfinite(data.raw_pred.values).all())
            result = fire_candidate_decode_fusion_reference(case, data)
            self.assertTrue(np.isfinite(result.values).all())
            np.testing.assert_array_equal(result.values[0, :3, 4], [0, 1, 0])
        activation = sigmoid_fp32(np.asarray([-1000, 0, 1000], dtype=np.float32))
        np.testing.assert_array_equal(activation, [0, 0.5, 1])
        self.assertEqual(activation.dtype, np.dtype("float32"))

    def test_output_overflow_is_distinct_from_invalid_nonfinite_inputs(self):
        for name in ("fp16_output_overflow", "fp32_output_overflow"):
            case = named(name)
            data = materialize_case(case)
            self.assertTrue(np.isfinite(data.raw_pred.values).all())
            self.assertTrue(np.isfinite(data.priors.values).all())
            result = fire_candidate_decode_fusion_reference(case, data)
            self.assertTrue(np.isposinf(result.values[..., :2]).all())
            self.assertTrue(np.isfinite(result.values[..., 4]).all())
            if case.raw_pred_dtype == "float16":
                self.assertTrue(np.isfinite(result.fp32).all())

    def test_reference_is_repeatable(self):
        for case in CASES:
            data = materialize_case(case)
            first = fire_candidate_decode_fusion_reference(case, data)
            second = fire_candidate_decode_fusion_reference(case, data)
            np.testing.assert_array_equal(first.values, second.values)


if __name__ == "__main__":
    unittest.main()

"""Independent formula checks and adversarial output-comparison self-checks."""

from dataclasses import replace
import json
import math
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

import numpy as np

from tests.common.case_spec import TOLERANCES
from tests.common.checks import check_declarations, check_generated_data
from tests.common.generate_data import generate_suite
from tests.common.reference import (
    CaseData, fire_candidate_decode_fusion_reference, materialize_case, sigmoid_fp32, tensor_value,
)
from tests.common.verify_result import verify_case_output
from tests.correctness.cases import CASES


def named(name):
    return next(case for case in CASES if case.name == "correctness_" + name)


def scalar_decode(raw, prior):
    """Independent scalar formula; round each intermediate to float32.

    math.exp is independent of NumPy's array exp; the result is rounded before
    addition and division. Comparisons allow the contract's numerical tolerance.
    """
    f32 = np.float32
    raw = [f32(value) for value in raw]
    gx, gy, stride, anchor_w, anchor_h = [f32(value) for value in prior]

    def sigmoid(value):
        exponent = float(f32(-value))
        exponential = f32(np.inf) if exponent > math.log(float(np.finfo(np.float32).max)) else f32(math.exp(exponent))
        return f32(f32(1) / f32(f32(1) + exponential))

    with np.errstate(over="ignore", under="ignore"):
        sx, sy, sw, sh, obj, fire = [sigmoid(value) for value in raw]
        x = f32(f32(f32(f32(2) * sx) - f32(0.5)) + gx) * stride
        y = f32(f32(f32(f32(2) * sy) - f32(0.5)) + gy) * stride
        ww, hh = f32(f32(2) * sw), f32(f32(2) * sh)
        w = f32(f32(f32(ww * ww) * anchor_w) * stride)
        h = f32(f32(f32(hh * hh) * anchor_h) * stride)
        score = f32(obj * fire)
    return np.asarray([x, y, w, h, score], dtype=np.float32)


class CorrectnessTests(unittest.TestCase):
    def test_declarations(self):
        check_declarations(self, CASES, "correctness")

    def test_generated_data(self):
        check_generated_data(self, CASES, "correctness")

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
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "suite"
            generate_suite(root, (case,))
            actual = root / "actual.bin"
            wrong.tofile(actual)
            result = verify_case_output(root / case.name, actual)
            self.assertFalse(result.passed)
            self.assertGreater(result.channel_mismatches["x"], 0)

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
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "suite"
            generate_suite(root, (case,))
            actual = root / "actual.bin"
            wrong.tofile(actual)
            result = verify_case_output(root / case.name, actual)
            self.assertFalse(result.passed)
            self.assertEqual(result.channel_mismatches["w"], 66)
            self.assertEqual(result.channel_mismatches["h"], 66)

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
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "suite"
            generate_suite(root, (case,))
            actual = root / "actual.bin"
            wrong.tofile(actual)
            result = verify_case_output(root / case.name, actual)
            self.assertFalse(result.passed)
            self.assertEqual(result.nonfinite_mismatch_count, 132)

    def test_new_precision_cases_match_independent_scalar_formula(self):
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

    def test_verifier_detects_coordinates_scores_candidate_order_and_missing_tail(self):
        case = named("fp32_batch_prior_tail")
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "suite"
            generate_suite(root, (case,))
            folder = root / case.name
            expected = np.fromfile(folder / "reference_decoded.bin", dtype="<f4").reshape(3, 33, 5)
            wrong_coordinate = expected.copy()
            wrong_coordinate[2, -1, 0] += np.float32(100)
            wrong_score = expected.copy()
            wrong_score[1, 0, 4] = np.float32(1)
            unwritten_tail = expected.copy()
            unwritten_tail[:, -1] = np.nan
            for wrong in (wrong_coordinate, wrong_score, expected[:, ::-1], unwritten_tail, np.zeros_like(expected)):
                actual = root / "actual.bin"
                wrong.tofile(actual)
                result = verify_case_output(folder, actual)
                self.assertFalse(result.passed)
                self.assertGreater(result.mismatch_count, 0)
                self.assertIsNotNone(result.first_mismatch)
                self.assertEqual(sum(result.channel_mismatches.values()), result.mismatch_count)
            wrong_coordinate.tofile(actual)
            self.assertEqual(verify_case_output(folder, actual).first_mismatch, (2, 32, 0))

    def test_verifier_requires_exact_bytes_including_no_partial_last_element(self):
        case = named("fp16_batch_prior_tail")
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "suite"
            generate_suite(root, (case,))
            folder = root / case.name
            payload = (folder / "reference_decoded.bin").read_bytes()
            actual = root / "actual.bin"
            for malformed in (payload[:-1], payload[:-10], payload + b"\x00", payload + payload):
                actual.write_bytes(malformed)
                with self.assertRaisesRegex(ValueError, "bytes"):
                    verify_case_output(folder, actual)

    def test_verifier_matches_infinity_positions_and_signs_and_rejects_nan(self):
        case = named("fp16_output_overflow")
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "suite"
            generate_suite(root, (case,))
            folder = root / case.name
            expected = np.fromfile(folder / "reference_decoded.bin", dtype="<f2")
            self.assertTrue(verify_case_output(folder, folder / "reference_decoded.bin").passed)
            for replacement in (-np.inf, np.nan, 60000):
                wrong = expected.copy()
                wrong[0] = np.float16(replacement)
                actual = root / "actual.bin"
                wrong.tofile(actual)
                result = verify_case_output(folder, actual)
                self.assertFalse(result.passed)
                self.assertEqual(result.nonfinite_mismatch_count, 1)
                self.assertEqual(result.mismatch_count, 1)

    def test_verifier_enforces_score_bounds_even_within_absolute_tolerance(self):
        case = named("fp32_saturation")
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "suite"
            generate_suite(root, (case,))
            folder = root / case.name
            expected = np.fromfile(folder / "reference_decoded.bin", dtype="<f4").reshape(2, 7, 5)
            for index, score in ((0, -1e-6), (1, 1 + 1e-6)):
                wrong = expected.copy()
                wrong[0, index, 4] = np.float32(score)
                actual = root / "actual.bin"
                wrong.tofile(actual)
                result = verify_case_output(folder, actual)
                self.assertFalse(result.passed)
                self.assertEqual(result.score_range_error_count, 1)

    def test_verifier_uses_dtype_limits_and_accepts_only_tighter_overrides(self):
        for dtype in ("float32", "float16"):
            case = named(dtype.replace("float", "fp") + "_documented_zero")
            with tempfile.TemporaryDirectory() as temporary:
                root = Path(temporary) / "suite"
                generate_suite(root, (case,))
                folder = root / case.name
                actual = folder / "reference_decoded.bin"
                result = verify_case_output(folder, actual)
                self.assertEqual((result.atol, result.rtol), TOLERANCES[dtype])
                self.assertTrue(verify_case_output(folder, actual, atol=0, rtol=0).passed)
                for override in ({"atol": -1}, {"rtol": np.inf}, {"atol": np.nan}, {"atol": 0.1}, {"rtol": 0.1}):
                    with self.assertRaises(ValueError):
                        verify_case_output(folder, actual, **override)

    def test_verifier_applies_combined_absolute_and_relative_error(self):
        case = named("fp32_documented_zero")
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "suite"
            generate_suite(root, (case,))
            folder = root / case.name
            expected = np.fromfile(folder / "reference_decoded.bin", dtype="<f4")
            wrong = expected.copy()
            wrong[0] += np.float32(1e-4)  # Above atol alone, below atol + rtol * 16.
            actual = root / "actual.bin"
            wrong.tofile(actual)
            self.assertTrue(verify_case_output(folder, actual).passed)
            self.assertFalse(verify_case_output(folder, actual, rtol=0).passed)
            wrong[0] = expected[0] + np.float32(1e-3)
            wrong.tofile(actual)
            self.assertFalse(verify_case_output(folder, actual).passed)

    def test_verifier_refuses_corrupted_reference_files(self):
        case = CASES[0]
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "suite"
            generate_suite(root, (case,))
            folder = root / case.name
            reference = folder / "reference_decoded.bin"
            payload = bytearray(reference.read_bytes())
            payload[0] ^= 1
            reference.write_bytes(payload)
            with self.assertRaisesRegex(ValueError, "SHA256"):
                verify_case_output(folder, reference)

    def test_verifier_cli_exit_status_matches_pass_and_failure(self):
        case = CASES[0]
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "suite"
            generate_suite(root, (case,))
            folder = root / case.name
            actual = root / "actual.bin"
            actual.write_bytes((folder / "reference_decoded.bin").read_bytes())
            command = [sys.executable, "-m", "tests.common.verify_result", "--case-dir", str(folder), "--actual", str(actual)]
            passed = subprocess.run(command, capture_output=True, text=True, check=False)
            self.assertEqual(passed.returncode, 0, passed.stderr)
            self.assertTrue(json.loads(passed.stdout)["passed"])
            np.zeros(5, dtype="<f4").tofile(actual)
            failed = subprocess.run(command, capture_output=True, text=True, check=False)
            self.assertEqual(failed.returncode, 1, failed.stderr)
            self.assertFalse(json.loads(failed.stdout)["passed"])


if __name__ == "__main__":
    unittest.main()

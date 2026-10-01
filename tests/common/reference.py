"""Deterministic input materialization and the contract's float32 reference.

CPU code in this module is a test oracle, never an implementation fallback.
Inputs are quantized to their declared dtype before any reference calculation.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Optional, Tuple

import numpy as np

from tests.common.cases import CaseSpec, SUPPORTED_RAW_DTYPES


STORAGE_DTYPES = {
    "float16": "<f2",
    "float32": "<f4",
    "float64": "<f8",
    "int32": "<i4",
}


@dataclass(frozen=True)
class TensorValue:
    logical_dtype: str
    values: np.ndarray
    # Physical backing, including padding for noncontiguous rejection fixtures.
    storage: np.ndarray


@dataclass(frozen=True)
class CaseData:
    raw_pred: TensorValue
    priors: TensorValue


@dataclass(frozen=True)
class ValidationResult:
    errors: Tuple[str, ...]


@dataclass(frozen=True)
class ReferenceResult:
    fp32: np.ndarray
    values: np.ndarray


def tensor_value(values: np.ndarray, dtype: str, contiguous: bool = True) -> TensorValue:
    """Quantize first, preserving logical shape and physical element strides."""
    quantized = np.array(values, dtype=STORAGE_DTYPES[dtype], order="C", copy=True)
    if contiguous or quantized.ndim == 0 or quantized.shape[-1] == 0:
        return TensorValue(dtype, quantized, quantized)
    backing = np.zeros(quantized.shape[:-1] + (quantized.shape[-1] * 2,), dtype=quantized.dtype)
    view = backing[..., ::2]
    view[...] = quantized
    return TensorValue(dtype, view, backing)


def _repeat_rows(rows: np.ndarray, shape: Tuple[int, ...]) -> np.ndarray:
    size = int(np.prod(shape, dtype=np.int64))
    return np.resize(np.asarray(rows, dtype=np.float32), size).reshape(shape)


def _input_values(case: CaseSpec, rng: np.random.Generator) -> np.ndarray:
    shape = case.raw_pred_shape
    pattern = case.input_pattern
    if pattern == "zeros":
        return np.zeros(shape, dtype=np.float32)
    if pattern == "documented_positive":
        return _repeat_rows([[2, 2, 0, 0, 2, 2]], shape)
    if pattern == "documented_negative":
        return _repeat_rows([[-2, -2, -1, -1, -2, -2]], shape)
    if pattern == "channel_probe":
        return _repeat_rows([
            [0, 0, 0, 0, 0, 0],
            [1.25, -2.5, 3, -0.75, 1, -2],
            [-4, 2, -1, 1.5, -3, 4],
            [8, -8, 0.03125, -0.03125, -8, 8],
            [0, 0.75, -2, 2, 6, -6],
            [0.01, -0.02, 0.2, -0.3, -0.4, 0.7],
        ], shape)
    if pattern == "mixed_confidence":
        return _repeat_rows([
            [0, 0, 0, 0, -2, 2], [0, 0, 0, 0, 2, -2],
            [0, 0, 0, 0, 2, 2], [0, 0, 0, 0, -2, -2],
            [0, 0, 0, 0, 0, 0], [0, 0, 0, 0, -16, 16],
        ], shape)
    if pattern == "low_confidence":
        values = rng.normal(0, 1.5, size=shape).astype(np.float32)
        values[..., 4:] = np.float32(-30)
        return values
    if pattern == "saturation":
        limit = np.finfo(np.dtype(case.raw_pred_dtype)).max
        return _repeat_rows([
            [-limit, limit, -limit, limit, -limit, limit],
            [limit, -limit, limit, -limit, limit, limit],
            [0, 0, 0, 0, -limit, -limit],
        ], shape)
    if pattern == "small_values":
        return rng.uniform(-1.0e-4, 1.0e-4, size=shape).astype(np.float32)
    if pattern == "cancellation":
        return _repeat_rows([
            [-1.0986328125, -1.0986328125, 0.33325, -0.6665, 0.143, -0.273],
            [-1.0986, -1.0987, 1.7692, -5.203, -1.143, 2.273],
            [-1.0986123, -1.0986122, -0.143, 1.097, 3.01, -2.02],
        ], shape)
    if pattern == "width_underflow":
        return _repeat_rows([[0, 0, -12, -12, 0, 0]], shape)
    if pattern == "large_prior_probe":
        return _repeat_rows([[0, 0, -8, -8, 0, 0]], shape)
    values = rng.normal(0, 2, size=shape).astype(np.float32)
    if pattern == "indexed":
        b, n, _ = shape
        index = np.arange(n, dtype=np.float32)[None, :]
        batch = np.arange(b, dtype=np.float32)[:, None]
        values[..., 0] = (index % np.float32(37)) / np.float32(11) - np.float32(1.5) + batch / np.float32(7)
        values[..., 1] = -(index % np.float32(19)) / np.float32(7) + batch / np.float32(5)
        return values
    if pattern in ("nan_one", "posinf_one", "neginf_one"):
        values.flat[0] = {"nan_one": np.nan, "posinf_one": np.inf, "neginf_one": -np.inf}[pattern]
    elif pattern != "random":
        raise ValueError(f"unknown raw_pred pattern: {pattern}")
    return values


def _prior_values(case: CaseSpec, rng: np.random.Generator) -> np.ndarray:
    shape = case.resolved_priors_shape
    pattern = case.prior_pattern
    if pattern == "documented":
        return _repeat_rows([[0, 0, 32, 2, 2]], shape)
    if pattern == "cancellation":
        return _repeat_rows([[0, 0, 1024, 0.75, 1.25]], shape)
    if pattern == "precision":
        return _repeat_rows([[0.12500003, 0.37500009, 31.123456, 0.33333334, 1.234567]], shape)
    if pattern == "large_anchors":
        return _repeat_rows([[0, 0, 1, 1e8, 1e8]], shape)
    if pattern == "fractional":
        return _repeat_rows([[0.125, 0.375, 0.25, 1.0e-4, 0.03125]], shape)
    if pattern == "large_coordinates":
        return _repeat_rows([[120, 127, 480, 15.125, 17.75]], shape)
    if pattern == "half_output_overflow":
        return _repeat_rows([[32768, 32769, 32, 2, 2]], shape)
    if pattern == "fp32_output_overflow":
        maximum = np.finfo(np.float32).max
        return _repeat_rows([[maximum, maximum, 2, maximum, maximum]], shape)
    values = rng.uniform(0.25, 8, size=shape).astype(np.float32)
    if len(shape) == 2 and shape[1] == 5:
        index = np.arange(shape[0], dtype=np.int32)
        values[:, 0] = (index % 127).astype(np.float32)
        values[:, 1] = (index // 127).astype(np.float32)
        values[:, 2] = np.asarray([1, 3.5, 8, 16, 32], dtype=np.float32)[index % 5]
    replacements = {
        "stride_zero": (2, 0), "stride_negative": (2, -1),
        "anchor_w_zero": (3, 0), "anchor_w_negative": (3, -1),
        "anchor_h_zero": (4, 0), "anchor_h_negative": (4, -1),
        "gx_negative": (0, -1), "gy_negative": (1, -1),
        "nan_one": (0, np.nan), "posinf_one": (2, np.inf), "neginf_one": (4, -np.inf),
    }
    if pattern in replacements:
        column, value = replacements[pattern]
        values[0, column] = np.float32(value)
    elif pattern != "varied":
        raise ValueError(f"unknown priors pattern: {pattern}")
    return values


def materialize_case(case: CaseSpec) -> CaseData:
    rng = np.random.default_rng(case.seed)
    return CaseData(
        tensor_value(_input_values(case, rng), case.raw_pred_dtype, case.raw_pred_contiguous),
        tensor_value(_prior_values(case, rng), case.priors_dtype, case.priors_contiguous),
    )


def validate_case(case: CaseSpec, data: Optional[CaseData] = None) -> ValidationResult:
    """Check the input contract locally; labels are not ACLNN error codes.

    Descriptor checks always run. Value checks run when materialized inputs are
    supplied; they do not require a device implementation to scan invalid values.
    """
    errors = []
    raw_shape = case.raw_pred_shape
    prior_shape = case.resolved_priors_shape
    if len(raw_shape) != 3:
        errors.append("RAW_PRED_RANK")
    else:
        if not 1 <= raw_shape[0] <= 8:
            errors.append("B_RANGE")
        if not 1 <= raw_shape[1] <= 32768:
            errors.append("N_RANGE")
        if raw_shape[2] != 6:
            errors.append("RAW_PRED_WIDTH")
    if len(prior_shape) != 2:
        errors.append("PRIORS_RANK")
    else:
        if prior_shape[1] != 5:
            errors.append("PRIORS_WIDTH")
        if len(raw_shape) == 3 and prior_shape[0] != raw_shape[1]:
            errors.append("N_MISMATCH")
    if case.raw_pred_dtype not in SUPPORTED_RAW_DTYPES:
        errors.append("RAW_PRED_DTYPE")
    if case.priors_dtype != "float32":
        errors.append("PRIORS_DTYPE")
    for name, format_name, contiguous in (
        ("RAW_PRED", case.raw_pred_format, case.raw_pred_contiguous),
        ("PRIORS", case.priors_format, case.priors_contiguous),
    ):
        if format_name != "ND":
            errors.append(name + "_FORMAT")
        if not contiguous:
            errors.append(name + "_CONTIGUOUS")
    if data is not None:
        for name, tensor, shape, dtype in (
            ("RAW_PRED", data.raw_pred, raw_shape, case.raw_pred_dtype),
            ("PRIORS", data.priors, prior_shape, case.priors_dtype),
        ):
            if tensor.values.shape != shape:
                errors.append(name + "_DATA_SHAPE")
            if tensor.logical_dtype != dtype or tensor.values.dtype != np.dtype(STORAGE_DTYPES[dtype]):
                errors.append(name + "_DATA_DTYPE")
            if not tensor.values.flags.c_contiguous:
                errors.append(name + "_CONTIGUOUS")
            if not np.isfinite(tensor.values).all():
                errors.append(name + "_NONFINITE")
        priors = data.priors.values
        if priors.ndim == 2 and priors.shape[1] == 5:
            for column, label in ((0, "GX_NEGATIVE"), (1, "GY_NEGATIVE")):
                if np.any(priors[:, column] < 0):
                    errors.append(label)
            for column, label in ((2, "STRIDE_NONPOSITIVE"), (3, "ANCHOR_W_NONPOSITIVE"), (4, "ANCHOR_H_NONPOSITIVE")):
                if np.any(priors[:, column] <= 0):
                    errors.append(label)
    return ValidationResult(tuple(dict.fromkeys(errors)))


def infer_output_shape(case: CaseSpec) -> Tuple[int, int, int]:
    validation = validate_case(case)
    if validation.errors:
        raise ValueError(f"invalid input descriptor: {validation.errors}")
    return (case.raw_pred_shape[0], case.raw_pred_shape[1], 5)


def sigmoid_fp32(values: np.ndarray) -> np.ndarray:
    """Evaluate 1 / (1 + exp(-z)) with float32 at every arithmetic step."""
    values = np.asarray(values, dtype=np.float32)
    with np.errstate(over="ignore", under="ignore"):
        exponential = np.exp(np.negative(values), dtype=np.float32)
        return np.divide(np.float32(1), np.add(np.float32(1), exponential), dtype=np.float32)


def fire_candidate_decode_fusion_reference(case: CaseSpec, data: CaseData) -> ReferenceResult:
    validation = validate_case(case, data)
    if validation.errors:
        raise ValueError(f"cannot evaluate invalid inputs for {case.name}: {validation.errors}")
    raw_pred = np.asarray(data.raw_pred.values, dtype=np.float32)
    priors = np.asarray(data.priors.values, dtype=np.float32)
    activations = sigmoid_fp32(raw_pred)
    decoded = np.empty(infer_output_shape(case), dtype=np.float32)
    # Parenthesization follows the stated formula. No fused multiply-add or
    # intermediate float16 conversion is used in this test oracle.
    with np.errstate(over="ignore", under="ignore"):
        doubled = np.multiply(activations[..., :4], np.float32(2))
        centers = np.add(np.subtract(doubled[..., :2], np.float32(0.5)), priors[None, :, :2])
        decoded[..., :2] = np.multiply(centers, priors[None, :, 2:3])
        sizes = np.multiply(doubled[..., 2:4], doubled[..., 2:4])
        sizes = np.multiply(sizes, priors[None, :, 3:5])
        decoded[..., 2:4] = np.multiply(sizes, priors[None, :, 2:3])
        decoded[..., 4] = np.multiply(activations[..., 4], activations[..., 5])
        output = decoded.astype(STORAGE_DTYPES[case.raw_pred_dtype])
    return ReferenceResult(decoded, output)


def element_strides(values: np.ndarray) -> Tuple[int, ...]:
    return tuple(int(stride // values.dtype.itemsize) for stride in values.strides)


def storage_description(dtype: str) -> dict:
    return {"logical_dtype": dtype, "storage_dtype": STORAGE_DTYPES[dtype], "endianness": "little"}

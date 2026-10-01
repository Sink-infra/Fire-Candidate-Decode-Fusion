"""Input-contract boundaries; no official or simulated performance catalog."""

from dataclasses import replace

from tests.common.case_spec import CaseSpec


def valid(name, b, n, dtype, **kwargs):
    return CaseSpec(
        name="coverage_" + name, category="coverage", expectation="success",
        raw_pred_shape=(b, n, 6), raw_pred_dtype=dtype, seed=104, **kwargs,
    )


def reject(name, error, **kwargs):
    return replace(
        valid("reject_" + name, 2, 33, "float32"),
        expectation="reject", expected_error=error, **kwargs,
    )


CASES = (
    valid("fp32_minimum", 1, 1, "float32", covers=("B_min", "N_min", "float32")),
    valid("fp16_minimum", 1, 1, "float16", covers=("B_min", "N_min", "float16")),
    valid("fp32_n31", 2, 31, "float32", covers=("tail_before_32",)),
    valid("fp16_n31", 2, 31, "float16", covers=("tail_before_32",)),
    valid("fp32_n32", 2, 32, "float32", covers=("aligned_32",)),
    valid("fp16_n32", 2, 32, "float16", covers=("aligned_32",)),
    valid("fp32_n33", 3, 33, "float32", input_pattern="indexed", covers=("tail_after_32", "batch_boundary")),
    valid("fp16_n33", 3, 33, "float16", input_pattern="indexed", covers=("tail_after_32", "batch_boundary")),
    valid("fp32_n63", 4, 63, "float32", covers=("tail_before_64",)),
    valid("fp16_n63", 4, 63, "float16", covers=("tail_before_64",)),
    valid("fp32_n64", 4, 64, "float32", covers=("aligned_64",)),
    valid("fp16_n64", 4, 64, "float16", covers=("aligned_64",)),
    valid("fp32_n65", 5, 65, "float32", covers=("tail_after_64",)),
    valid("fp16_n65", 5, 65, "float16", covers=("tail_after_64",)),
    valid("fp32_n127", 6, 127, "float32", covers=("odd_N", "B6")),
    valid("fp16_n129", 7, 129, "float16", covers=("odd_N", "B7")),
    valid("fp16_n4096", 1, 4096, "float16", covers=("large_N",)),
    valid("fp32_n16384", 4, 16384, "float32", covers=("large_N",)),
    valid("fp32_maximum", 8, 32768, "float32", covers=("B_max", "N_max")),
    valid("fp16_maximum", 8, 32768, "float16", covers=("B_max", "N_max")),
    valid("fp16_large_tail", 3, 32767, "float16", input_pattern="indexed", covers=("large_tail", "shared_priors")),
    valid("fp32_fractional_priors", 1, 7, "float32", prior_pattern="fractional",
          covers=("positive_fractional_stride", "small_anchors", "nonnegative_fractional_grid")),
    reject("raw_rank2", "RAW_PRED_RANK", raw_pred_shape=(33, 6), priors_shape=(33, 5)),
    reject("raw_rank4", "RAW_PRED_RANK", raw_pred_shape=(1, 2, 33, 6), priors_shape=(33, 5)),
    reject("priors_rank1", "PRIORS_RANK", priors_shape=(165,)),
    reject("priors_rank3", "PRIORS_RANK", priors_shape=(1, 33, 5)),
    reject("raw_width", "RAW_PRED_WIDTH", raw_pred_shape=(2, 33, 5)),
    reject("priors_width", "PRIORS_WIDTH", priors_shape=(33, 6)),
    reject("b_zero", "B_RANGE", raw_pred_shape=(0, 33, 6)),
    reject("b_above_max", "B_RANGE", raw_pred_shape=(9, 33, 6)),
    reject("n_zero", "N_RANGE", raw_pred_shape=(2, 0, 6)),
    reject("n_above_max", "N_RANGE", raw_pred_shape=(1, 32769, 6)),
    reject("n_mismatch", "N_MISMATCH", priors_shape=(32, 5)),
    reject("raw_int32", "RAW_PRED_DTYPE", raw_pred_dtype="int32"),
    reject("raw_float64", "RAW_PRED_DTYPE", raw_pred_dtype="float64"),
    reject("priors_float16", "PRIORS_DTYPE", priors_dtype="float16"),
    reject("priors_float64", "PRIORS_DTYPE", priors_dtype="float64"),
    reject("raw_format", "RAW_PRED_FORMAT", raw_pred_format="NCHW"),
    reject("priors_format", "PRIORS_FORMAT", priors_format="NCHW"),
    reject("raw_noncontiguous", "RAW_PRED_CONTIGUOUS", raw_pred_contiguous=False),
    reject("priors_noncontiguous", "PRIORS_CONTIGUOUS", priors_contiguous=False),
    reject("raw_nan", "RAW_PRED_NONFINITE", input_pattern="nan_one"),
    reject("raw_posinf", "RAW_PRED_NONFINITE", input_pattern="posinf_one"),
    reject("raw_neginf", "RAW_PRED_NONFINITE", input_pattern="neginf_one"),
    reject("priors_nan", "PRIORS_NONFINITE", prior_pattern="nan_one"),
    reject("priors_posinf", "PRIORS_NONFINITE", prior_pattern="posinf_one"),
    reject("priors_neginf", "PRIORS_NONFINITE", prior_pattern="neginf_one"),
    reject("stride_zero", "STRIDE_NONPOSITIVE", prior_pattern="stride_zero"),
    reject("stride_negative", "STRIDE_NONPOSITIVE", prior_pattern="stride_negative"),
    reject("anchor_w_zero", "ANCHOR_W_NONPOSITIVE", prior_pattern="anchor_w_zero"),
    reject("anchor_w_negative", "ANCHOR_W_NONPOSITIVE", prior_pattern="anchor_w_negative"),
    reject("anchor_h_zero", "ANCHOR_H_NONPOSITIVE", prior_pattern="anchor_h_zero"),
    reject("anchor_h_negative", "ANCHOR_H_NONPOSITIVE", prior_pattern="anchor_h_negative"),
    reject("gx_negative", "GX_NEGATIVE", prior_pattern="gx_negative"),
    reject("gy_negative", "GY_NEGATIVE", prior_pattern="gy_negative"),
)

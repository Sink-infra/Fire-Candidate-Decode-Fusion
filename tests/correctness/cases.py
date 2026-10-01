"""Formula, float32 intermediate precision, indexing, and output conversion."""

from tests.common.case_spec import CaseSpec


def case(name, b, n, dtype, pattern="random", priors="varied", **kwargs):
    return CaseSpec(
        name="correctness_" + name, category="correctness", expectation="success",
        raw_pred_shape=(b, n, 6), raw_pred_dtype=dtype, input_pattern=pattern,
        prior_pattern=priors, seed=140, **kwargs,
    )


CASES = (
    case("fp32_documented_zero", 1, 1, "float32", "zeros", "documented", covers=("example_1",)),
    case("fp16_documented_zero", 1, 1, "float16", "zeros", "documented", covers=("example_1",)),
    case("fp32_documented_positive", 1, 1, "float32", "documented_positive", "documented", covers=("example_2",)),
    case("fp16_documented_positive", 1, 1, "float16", "documented_positive", "documented", covers=("example_2",)),
    case("fp32_documented_negative", 1, 1, "float32", "documented_negative", "documented", covers=("example_3",)),
    case("fp16_documented_negative", 1, 1, "float16", "documented_negative", "documented", covers=("example_3",)),
    case("fp32_channel_formula", 2, 6, "float32", "channel_probe", covers=("output_channels", "square", "anchor_stride")),
    case("fp16_channel_formula", 2, 6, "float16", "channel_probe", covers=("output_channels", "square", "anchor_stride")),
    case("fp32_batch_prior_tail", 3, 33, "float32", "indexed", covers=("example_4", "batch_shared_priors", "candidate_order", "tail")),
    case("fp16_batch_prior_tail", 3, 33, "float16", "indexed", covers=("example_4", "batch_shared_priors", "candidate_order", "tail")),
    case("fp32_confidence_product", 1, 6, "float32", "mixed_confidence", covers=("obj_times_fire",)),
    case("fp16_confidence_product", 1, 6, "float16", "mixed_confidence", covers=("obj_times_fire",)),
    case("fp32_no_filter", 4, 65, "float32", "low_confidence", covers=("retain_low_score_candidates",)),
    case("fp16_no_filter", 4, 65, "float16", "low_confidence", covers=("retain_zero_rounded_scores",)),
    case("fp32_saturation", 2, 7, "float32", "saturation", covers=("finite_extreme_logits", "score_bounds")),
    case("fp16_saturation", 2, 7, "float16", "saturation", covers=("finite_extreme_logits", "score_bounds")),
    case("fp32_cancellation", 2, 65, "float32", "cancellation", "cancellation", covers=("near_zero_center", "absolute_tolerance")),
    case("fp16_cancellation", 2, 65, "float16", "cancellation", "cancellation", covers=("float32_intermediates", "final_half_cast")),
    case("fp32_small_logits", 1, 31, "float32", "small_values", covers=("small_logits",)),
    case("fp16_small_logits", 1, 31, "float16", "small_values", covers=("small_half_logits",)),
    case("fp32_prior_precision", 2, 17, "float32", "random", "precision", covers=("float32_priors",)),
    case("fp16_prior_precision", 2, 17, "float16", "random", "precision", covers=("float32_priors_with_half_raw",)),
    case("fp16_width_underflow", 2, 33, "float16", "width_underflow", "large_anchors",
         covers=("float32_width_sigmoid", "float32_square", "avoid_half_underflow")),
    case("fp16_large_float32_priors", 2, 33, "float16", "large_prior_probe", "large_anchors",
         covers=("priors_outside_half_range", "finite_half_sizes")),
    case("fp16_large_coordinates", 2, 9, "float16", "channel_probe", "large_coordinates", covers=("large_finite_half_output",)),
    case("fp16_output_overflow", 1, 3, "float16", "zeros", "half_output_overflow",
         covers=("finite_inputs", "half_cast_overflow"), note="Finite float32 coordinates overflow only at the final float16 cast."),
    case("fp32_output_overflow", 1, 3, "float32", "zeros", "fp32_output_overflow",
         covers=("finite_inputs", "fp32_multiply_overflow"), note="The input contract supplies no upper bound on priors; matching signed infinities are compared explicitly."),
    case("fp32_large_odd_layout", 4, 4097, "float32", "indexed", covers=("large_odd_N", "batch_offsets", "candidate_order")),
)

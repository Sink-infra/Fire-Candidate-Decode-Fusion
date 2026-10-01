"""Immutable descriptions; each category owns its own case catalog."""

from __future__ import annotations

from dataclasses import dataclass
from typing import Optional, Tuple


SUPPORTED_RAW_DTYPES = ("float32", "float16")
TOLERANCES = {
    "float32": (1.0e-5, 1.0e-5),
    "float16": (3.0e-3, 3.0e-3),
}
RAW_CHANNELS = ("tx", "ty", "tw", "th", "obj_logit", "fire_logit")
PRIOR_CHANNELS = ("gx", "gy", "stride", "anchor_w", "anchor_h")
OUTPUT_CHANNELS = ("x", "y", "w", "h", "fire_score")


@dataclass(frozen=True)
class CaseSpec:
    name: str
    category: str
    expectation: str
    raw_pred_shape: Tuple[int, ...]
    raw_pred_dtype: str
    input_pattern: str = "random"
    prior_pattern: str = "varied"
    seed: int = 0
    priors_shape: Optional[Tuple[int, ...]] = None
    priors_dtype: str = "float32"
    raw_pred_format: str = "ND"
    priors_format: str = "ND"
    raw_pred_contiguous: bool = True
    priors_contiguous: bool = True
    expected_error: Optional[str] = None
    covers: Tuple[str, ...] = ()
    note: str = ""

    @property
    def resolved_priors_shape(self) -> Tuple[int, ...]:
        if self.priors_shape is not None:
            return self.priors_shape
        n = self.raw_pred_shape[1] if len(self.raw_pred_shape) >= 2 else 0
        return (n, 5)

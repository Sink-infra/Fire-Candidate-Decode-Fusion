"""Compare a raw device output with the generated, dtype-correct reference."""

from __future__ import annotations

import argparse
from dataclasses import asdict, dataclass
import json
import math
from pathlib import Path
from typing import Optional, Tuple

import numpy as np

from tests.common.case_spec import OUTPUT_CHANNELS, TOLERANCES
from tests.common.artifacts import load_case_metadata
from tests.common.reference import STORAGE_DTYPES


@dataclass(frozen=True)
class VerificationResult:
    passed: bool
    element_count: int
    max_abs_error: float
    mismatch_count: int
    nonfinite_mismatch_count: int
    score_range_error_count: int
    first_mismatch: Optional[Tuple[int, int, int]]
    channel_mismatches: dict
    atol: float
    rtol: float
    message: str


def _load_values(path: Path, dtype: str, element_count: int) -> np.ndarray:
    storage_dtype = np.dtype(STORAGE_DTYPES[dtype])
    expected_bytes = element_count * storage_dtype.itemsize
    actual_bytes = path.stat().st_size
    if actual_bytes != expected_bytes:
        raise ValueError(f"{path} contains {actual_bytes} bytes, expected {expected_bytes} ({element_count} {dtype} elements)")
    return np.fromfile(path, dtype=storage_dtype).astype(np.float64)


def _resolve_tolerances(dtype: str, atol: Optional[float], rtol: Optional[float]) -> tuple:
    maximum_atol, maximum_rtol = TOLERANCES[dtype]
    atol = maximum_atol if atol is None else atol
    rtol = maximum_rtol if rtol is None else rtol
    for name, value, maximum in (("atol", atol, maximum_atol), ("rtol", rtol, maximum_rtol)):
        if not math.isfinite(value) or value < 0 or value > maximum:
            raise ValueError(f"{dtype} {name} must be finite and in [0, {maximum}]")
    return float(atol), float(rtol)


def verify_case_output(
    case_dir: Path, actual_path: Path, *,
    atol: Optional[float] = None, rtol: Optional[float] = None,
) -> VerificationResult:
    case_dir = Path(case_dir)
    actual_path = Path(actual_path)
    metadata = load_case_metadata(case_dir)
    if metadata.get("expectation") != "success":
        raise ValueError("rejection fixtures do not have an output to verify")
    expected_output = metadata["expected_output"]
    shape = tuple(expected_output["shape"])
    dtype = expected_output["dtype"]
    if len(shape) != 3 or not 1 <= shape[0] <= 8 or not 1 <= shape[1] <= 32768 or shape[2] != 5:
        raise ValueError("invalid decoded shape in metadata")
    if dtype != metadata["input"]["raw_pred"]["dtype"] or dtype not in TOLERANCES:
        raise ValueError("decoded dtype must match the supported raw_pred dtype")
    atol, rtol = _resolve_tolerances(dtype, atol, rtol)
    element_count = math.prod(shape)
    reference_file = metadata["files"]["reference_decoded"]
    if reference_file["logical_dtype"] != dtype:
        raise ValueError("the reference must be converted to the decoded dtype")
    reference_path = case_dir / reference_file["path"]
    expected = _load_values(reference_path, dtype, element_count).reshape(shape)
    actual = _load_values(actual_path, dtype, element_count).reshape(shape)
    finite = np.isfinite(expected) & np.isfinite(actual)
    nonfinite_match = (
        (np.isnan(expected) & np.isnan(actual))
        | (np.isposinf(expected) & np.isposinf(actual))
        | (np.isneginf(expected) & np.isneginf(actual))
    )
    nonfinite_mismatch = ~finite & ~nonfinite_match
    mismatch = nonfinite_mismatch.copy()
    diff = np.abs(actual[finite] - expected[finite])
    mismatch[finite] = diff > (atol + rtol * np.abs(expected[finite]))
    scores = actual[..., 4]
    score_range_error = ~np.isfinite(scores) | (scores < 0) | (scores > 1)
    mismatch[..., 4] |= score_range_error
    mismatch_count = int(np.count_nonzero(mismatch))
    first = None
    if mismatch_count:
        first = tuple(int(index) for index in np.unravel_index(np.flatnonzero(mismatch)[0], shape))
    channel_mismatches = {name: int(np.count_nonzero(mismatch[..., index])) for index, name in enumerate(OUTPUT_CHANNELS)}
    passed = mismatch_count == 0
    message = "pass" if passed else f"failed: {mismatch_count} mismatched elements; first [b,n,channel]={first} ({OUTPUT_CHANNELS[first[2]]})"
    return VerificationResult(
        passed, element_count, float(diff.max()) if diff.size else 0.0, mismatch_count,
        int(np.count_nonzero(nonfinite_mismatch)), int(np.count_nonzero(score_range_error)),
        first, channel_mismatches, atol, rtol, message,
    )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--case-dir", required=True, type=Path)
    parser.add_argument("--actual", required=True, type=Path)
    parser.add_argument("--atol", type=float, help="optional tighter absolute tolerance")
    parser.add_argument("--rtol", type=float, help="optional tighter relative tolerance")
    args = parser.parse_args()
    try:
        result = verify_case_output(args.case_dir.resolve(), args.actual.resolve(), atol=args.atol, rtol=args.rtol)
    except (OSError, KeyError, TypeError, ValueError) as error:
        parser.error(str(error))
    print(json.dumps(asdict(result), sort_keys=True, allow_nan=False))
    return 0 if result.passed else 1


if __name__ == "__main__":
    raise SystemExit(main())

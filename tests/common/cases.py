"""Typed case specifications, JSON catalogs, and category-aware selection."""

from __future__ import annotations

from dataclasses import dataclass
import json
from pathlib import Path
import re
from typing import Iterable, Optional, Tuple


SUPPORTED_RAW_DTYPES = ("float32", "float16")
TOLERANCES = {
    "float32": (1.0e-5, 1.0e-5),
    "float16": (3.0e-3, 3.0e-3),
}
RAW_CHANNELS = ("tx", "ty", "tw", "th", "obj_logit", "fire_logit")
PRIOR_CHANNELS = ("gx", "gy", "stride", "anchor_w", "anchor_h")
OUTPUT_CHANNELS = ("x", "y", "w", "h", "fire_score")
CASE_ROOT = Path(__file__).resolve().parents[1] / "st/cases"
SUITES = ("coverage", "correctness", "mock")


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


def load_cases(category: str, case_root: Path = CASE_ROOT) -> tuple:
    if category not in SUITES:
        raise ValueError(f"unknown suite: {category}")
    path = Path(case_root) / f"{category}.json"
    document = json.loads(path.read_text(encoding="utf-8"))
    if (not isinstance(document, dict) or document.get("category") != category
            or not isinstance(document.get("cases"), list) or not document["cases"]):
        raise ValueError(f"invalid {category} case catalog: {path}")
    result = []
    names = set()
    for item in document["cases"]:
        if not isinstance(item, dict):
            raise ValueError(f"invalid case declaration in {path}")
        values = dict(item)
        if values.get("category", category) != category:
            raise ValueError(f"case category differs from its catalog: {path}")
        values["category"] = category
        for field in ("raw_pred_shape", "priors_shape", "covers"):
            if field in values:
                if values[field] is None and field == "priors_shape":
                    continue
                if not isinstance(values[field], list):
                    raise ValueError(f"{field} must be an array in {path}")
                values[field] = tuple(values[field])
        try:
            case = CaseSpec(**values)
        except TypeError as error:
            raise ValueError(f"invalid case fields in {path}: {error}") from error
        if not isinstance(case.name, str) or not re.fullmatch(category + r"_[a-z0-9_]+", case.name):
            raise ValueError(f"invalid case name in {path}")
        if case.name in names:
            raise ValueError(f"duplicate case name: {case.name}")
        if case.expectation not in ("success", "reject"):
            raise ValueError(f"invalid expectation for {case.name}")
        if (case.expected_error is not None) != (case.expectation == "reject"):
            raise ValueError(f"only rejection cases must declare expected_error: {case.name}")
        if (any(type(dimension) is not int for dimension in case.raw_pred_shape)
                or (case.priors_shape is not None and any(type(dimension) is not int for dimension in case.priors_shape))
                or type(case.seed) is not int or case.seed < 0
                or any(not isinstance(tag, str) for tag in case.covers)):
            raise ValueError(f"invalid shape, seed, or coverage tags for {case.name}")
        names.add(case.name)
        result.append(case)
    return tuple(result)


CASE_GROUPS = {category: load_cases(category) for category in SUITES}
COVERAGE_CASES = CASE_GROUPS["coverage"]
CORRECTNESS_CASES = CASE_GROUPS["correctness"]
MOCK_CASES = CASE_GROUPS["mock"]


def select_cases(category: str, *, names: Iterable[str] = (), tags: Iterable[str] = (),
                 expectation: str = "all") -> tuple:
    if category not in CASE_GROUPS:
        raise ValueError(f"unknown suite: {category}")
    if expectation not in ("all", "success", "reject"):
        raise ValueError(f"unknown expectation filter: {expectation}")
    cases = CASE_GROUPS[category]
    requested = set(names)
    missing = requested - {case.name for case in cases}
    if missing:
        raise ValueError(f"unknown case names in {category}: {sorted(missing)}")
    required_tags = set(tags)
    selected = tuple(case for case in cases
                     if (not requested or case.name in requested)
                     and required_tags.issubset(case.covers)
                     and (expectation == "all" or case.expectation == expectation))
    if not selected:
        raise ValueError("selection contains no cases")
    return selected

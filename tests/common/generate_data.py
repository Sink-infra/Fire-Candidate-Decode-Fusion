"""Generate one independent suite's inputs, float32 references, and manifest."""

from __future__ import annotations

import argparse
from collections import Counter
import json
from pathlib import Path
import re
import shutil
import tempfile
from typing import Iterable, Sequence

import numpy as np

from tests.common.artifacts import file_sha256

from tests.common.case_spec import (
    CaseSpec, OUTPUT_CHANNELS, PRIOR_CHANNELS, RAW_CHANNELS, TOLERANCES,
)
from tests.common.reference import (
    STORAGE_DTYPES,
    element_strides, fire_candidate_decode_fusion_reference, infer_output_shape,
    materialize_case, storage_description, validate_case,
)


def _write_array(case_dir: Path, filename: str, values: np.ndarray, dtype: str) -> dict:
    path = case_dir / filename
    np.asarray(values, dtype=STORAGE_DTYPES[dtype]).ravel(order="C").tofile(path)
    return {
        "path": filename,
        "bytes": path.stat().st_size,
        "sha256": file_sha256(path),
        **storage_description(dtype),
    }


def comparison_policy(dtype: str) -> dict:
    atol, rtol = TOLERANCES[dtype]
    return {
        "absolute_tolerance": atol,
        "relative_tolerance": rtol,
        "finite_values": "abs(actual - expected) <= atol + rtol * abs(expected)",
        "reference": "float32 arithmetic on dtype-quantized inputs, then final cast to decoded dtype",
        "infinity": "positions and signs must match, including output overflow from finite inputs",
        "nan": "positions must match; nonfinite input fixtures are outside the valid input contract",
        "fire_score": "all actual scores must be finite and in [0, 1]",
        "source": "FireCandidateDecodeFusion.md section 4.1",
    }


def _case_metadata(case: CaseSpec, case_dir: Path) -> dict:
    data = materialize_case(case)
    validation = validate_case(case, data)
    if case.expectation == "success" and validation.errors:
        raise ValueError(f"success case {case.name} violates the contract: {validation.errors}")
    if case.expectation == "reject" and case.expected_error not in validation.errors:
        raise ValueError(f"reject case {case.name} does not exhibit {case.expected_error}: {validation.errors}")
    files = {}
    inputs = {}
    for name, tensor, shape, dtype, format_name, contiguous, channels in (
        ("raw_pred", data.raw_pred, case.raw_pred_shape, case.raw_pred_dtype,
         case.raw_pred_format, case.raw_pred_contiguous, RAW_CHANNELS),
        ("priors", data.priors, case.resolved_priors_shape, case.priors_dtype,
         case.priors_format, case.priors_contiguous, PRIOR_CHANNELS),
    ):
        files[name] = _write_array(case_dir, name + ".bin", tensor.storage, dtype)
        inputs[name] = {
            "shape": list(shape), "dtype": dtype, "format": format_name,
            "contiguous": contiguous, "channels": list(channels),
            "strides_in_elements": list(element_strides(tensor.values)),
            "storage_shape": list(tensor.storage.shape), "storage_offset_in_elements": 0,
        }
    expected_output = {"shape": None, "dtype": None, "channels": list(OUTPUT_CHANNELS), "comparison": None}
    if case.expectation == "success":
        result = fire_candidate_decode_fusion_reference(case, data)
        files["reference_decoded"] = _write_array(case_dir, "reference_decoded.bin", result.values, case.raw_pred_dtype)
        files["reference_decoded_fp32"] = _write_array(case_dir, "reference_decoded_fp32.bin", result.fp32, "float32")
        expected_output.update({
            "shape": list(infer_output_shape(case)), "dtype": case.raw_pred_dtype,
            "comparison": comparison_policy(case.raw_pred_dtype),
        })
    return {
        "schema_version": 1, "operator": "FireCandidateDecodeFusion",
        "name": case.name, "category": case.category, "expectation": case.expectation,
        "expected_error": case.expected_error, "validation_errors": list(validation.errors),
        "validation_scope": "local input-contract check; no prescribed ACLNN code or device scan of out-of-contract values",
        "input": inputs, "attributes": {},
        "patterns": {"raw_pred": case.input_pattern, "priors": case.prior_pattern, "seed": case.seed},
        "expected_output": expected_output, "covers": list(case.covers), "note": case.note,
        "files": files,
    }


def generate_suite(output_dir: Path, cases: Sequence[CaseSpec]) -> dict:
    """Create inputs once, refusing to overwrite any existing output directory."""
    categories = {case.category for case in cases}
    if len(categories) != 1 or not categories.issubset(("coverage", "correctness")):
        raise ValueError("generate exactly one nonempty named suite at a time")
    category = next(iter(categories))
    names = [case.name for case in cases]
    if len(names) != len(set(names)):
        raise ValueError("case names must be unique within a suite")
    for case in cases:
        if not re.fullmatch(category + r"_[a-z0-9_]+", case.name):
            raise ValueError(f"invalid case name: {case.name}")
        if case.expectation not in ("success", "reject"):
            raise ValueError(f"unknown expectation for {case.name}")
        if (case.expected_error is not None) != (case.expectation == "reject"):
            raise ValueError(f"only rejection cases must declare expected_error: {case.name}")
    output_dir = Path(output_dir)
    if output_dir.exists() or output_dir.is_symlink():
        raise FileExistsError(f"refusing to overwrite existing output directory: {output_dir}")
    output_dir.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="." + output_dir.name + ".tmp-", dir=output_dir.parent) as temporary:
        staging = Path(temporary)
        manifest = _generate_suite(staging, cases, category)
        # Exclusive mkdir never replaces another writer's directory. Consumers
        # accept a bundle only once manifest.json is published as the last file.
        output_dir.mkdir(exist_ok=False)
        try:
            for child in staging.iterdir():
                if child.name != "manifest.json":
                    child.rename(output_dir / child.name)
            (staging / "manifest.json").rename(output_dir / "manifest.json")
        except BaseException:
            shutil.rmtree(output_dir)
            raise
    return manifest


def _generate_suite(output_dir: Path, cases: Sequence[CaseSpec], category: str) -> dict:
    entries = []
    for case in cases:
        case_dir = output_dir / case.name
        case_dir.mkdir()
        metadata = _case_metadata(case, case_dir)
        case_json = case_dir / "case.json"
        case_json.write_text(json.dumps(metadata, indent=2, sort_keys=True, ensure_ascii=False) + "\n", encoding="utf-8")
        entries.append({
            "name": case.name, "expectation": case.expectation,
            "metadata": case.name + "/case.json", "case_metadata_sha256": file_sha256(case_json),
        })
    counts = Counter(case.expectation for case in cases)
    manifest = {
        "schema_version": 1, "operator": "FireCandidateDecodeFusion", "category": category,
        "schema_order": ["raw_pred", "priors", "decoded"],
        "generation": {
            "deterministic_for_recorded_environment": True, "numpy_version": np.__version__,
            "bit_generator": "PCG64", "endianness": "little", "overwrite_policy": "refuse",
            "reference_compute_dtype": "float32", "reference_output_dtype": "same as raw_pred",
        },
        "catalog_audit_failures": [],
        "counts": {"total": len(cases), "success": counts["success"], "reject": counts["reject"]},
        "cases": entries,
    }
    (output_dir / "manifest.json").write_text(
        json.dumps(manifest, indent=2, sort_keys=True, ensure_ascii=False) + "\n", encoding="utf-8",
    )
    return manifest


def _select_cases(cases: Sequence[CaseSpec], expectation: str, names: Iterable[str]) -> tuple:
    if expectation not in ("all", "success", "reject"):
        raise ValueError(f"unknown expectation filter: {expectation}")
    requested = set(names)
    missing = requested - {case.name for case in cases}
    if missing:
        raise ValueError(f"unknown case names: {sorted(missing)}")
    selected = tuple(case for case in cases
                     if (expectation == "all" or case.expectation == expectation)
                     and (not requested or case.name in requested))
    if not selected:
        raise ValueError("selection contains no cases")
    return selected


def main(cases: Sequence[CaseSpec], category: str) -> int:
    parser = argparse.ArgumentParser(description=f"Generate {category} data for FireCandidateDecodeFusion.")
    parser.add_argument("--output", required=True, type=Path, help="new directory to create")
    parser.add_argument("--expectation", choices=("all", "success", "reject"), default="all")
    parser.add_argument("--case", action="append", default=[], help="exact case name; repeatable")
    args = parser.parse_args()
    try:
        if any(case.category != category for case in cases):
            raise ValueError("entry point received cases from another category")
        selected = _select_cases(cases, args.expectation, args.case)
        manifest = generate_suite(args.output.resolve(), selected)
    except (OSError, KeyError, TypeError, ValueError) as error:
        parser.error(str(error))
    print(json.dumps(manifest["counts"], sort_keys=True))
    print(f"manifest: {args.output.resolve() / 'manifest.json'}")
    return 0

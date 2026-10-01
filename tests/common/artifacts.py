"""Validate generated manifests, metadata, and every recorded binary artifact."""

from __future__ import annotations

from collections import Counter
import hashlib
import json
import math
from pathlib import Path
import re

import numpy as np

from tests.common.reference import STORAGE_DTYPES


def file_sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def load_suite_manifest(suite_dir: Path) -> dict:
    manifest = json.loads((Path(suite_dir) / "manifest.json").read_text(encoding="utf-8"))
    if not isinstance(manifest, dict) or manifest.get("schema_version") != 1 or manifest.get("operator") != "FireCandidateDecodeFusion":
        raise ValueError("unrecognized manifest schema or operator")
    category = manifest.get("category")
    if category not in ("coverage", "correctness"):
        raise ValueError("invalid manifest category")
    entries = manifest["cases"]
    if not isinstance(entries, list) or not entries or any(not isinstance(entry, dict) for entry in entries):
        raise ValueError("manifest contains no cases")
    names = [entry["name"] for entry in entries]
    if len(set(names)) != len(names):
        raise ValueError("duplicate case names in manifest")
    for entry in entries:
        name = entry["name"]
        if not re.fullmatch(category + r"_[a-z0-9_]+", name):
            raise ValueError("invalid case name in manifest")
        if entry["metadata"] != name + "/case.json":
            raise ValueError("invalid metadata path in manifest")
        if entry["expectation"] not in ("success", "reject"):
            raise ValueError("invalid expectation in manifest")
    counts = Counter(entry["expectation"] for entry in entries)
    expected_counts = {"total": len(entries), "success": counts["success"], "reject": counts["reject"]}
    if manifest["counts"] != expected_counts:
        raise ValueError("manifest counts do not match its entries")
    return manifest


def _shape(value, label):
    if not isinstance(value, list) or any(type(dimension) is not int or dimension < 0 for dimension in value):
        raise ValueError(f"invalid {label} shape")
    return tuple(value)


def load_case_metadata(case_dir: Path) -> dict:
    """Require the parent manifest and validate bytes before execution/comparison."""
    case_dir = Path(case_dir)
    manifest = load_suite_manifest(case_dir.parent)
    entry = next((item for item in manifest["cases"] if item["name"] == case_dir.name), None)
    if entry is None:
        raise ValueError("case is not recorded in the parent manifest")
    case_json = case_dir / "case.json"
    if file_sha256(case_json) != entry["case_metadata_sha256"]:
        raise ValueError("case metadata SHA256 mismatch; regenerate the suite")
    metadata = json.loads(case_json.read_text(encoding="utf-8"))
    if (not isinstance(metadata, dict) or metadata.get("schema_version") != 1 or metadata.get("operator") != manifest["operator"]
            or metadata.get("category") != manifest["category"] or metadata.get("name") != entry["name"]
            or metadata.get("expectation") != entry["expectation"]):
        raise ValueError("case metadata does not match its manifest entry")
    required_files = {"raw_pred", "priors"}
    if metadata["expectation"] == "success":
        required_files.update(("reference_decoded", "reference_decoded_fp32"))
    if set(metadata["files"]) != required_files:
        raise ValueError("case artifact set does not match its expectation")
    for name, descriptor in metadata["files"].items():
        if descriptor["path"] != name + ".bin":
            raise ValueError(f"invalid artifact path for {name}")
        dtype = descriptor["logical_dtype"]
        if dtype not in STORAGE_DTYPES or descriptor["storage_dtype"] != STORAGE_DTYPES[dtype] or descriptor["endianness"] != "little":
            raise ValueError(f"invalid storage description for {name}")
        path = case_dir / descriptor["path"]
        if path.stat().st_size != descriptor["bytes"]:
            raise ValueError(f"{name} byte size mismatch; regenerate the suite")
        if file_sha256(path) != descriptor["sha256"]:
            raise ValueError(f"{name} SHA256 mismatch; regenerate the suite")
    for name in ("raw_pred", "priors"):
        tensor = metadata["input"][name]
        shape = _shape(tensor["shape"], name)
        storage_shape = _shape(tensor["storage_shape"], name + " storage")
        dtype = tensor["dtype"]
        descriptor = metadata["files"][name]
        if dtype != descriptor["logical_dtype"] or len(shape) != len(storage_shape):
            raise ValueError(f"inconsistent input storage for {name}")
        expected_bytes = math.prod(storage_shape) * np.dtype(STORAGE_DTYPES[dtype]).itemsize
        if descriptor["bytes"] != expected_bytes:
            raise ValueError(f"inconsistent storage size for {name}")
    if metadata["expectation"] == "success":
        raw = metadata["input"]["raw_pred"]
        priors = metadata["input"]["priors"]
        raw_shape = _shape(raw["shape"], "raw_pred")
        prior_shape = _shape(priors["shape"], "priors")
        if (len(raw_shape) != 3 or not 1 <= raw_shape[0] <= 8 or not 1 <= raw_shape[1] <= 32768
                or raw_shape[2] != 6 or prior_shape != (raw_shape[1], 5)
                or raw["dtype"] not in ("float16", "float32") or priors["dtype"] != "float32"):
            raise ValueError("success metadata violates the input contract")
        for name, tensor in (("raw_pred", raw), ("priors", priors)):
            shape = tensor["shape"]
            strides = [math.prod(shape[index + 1:]) for index in range(len(shape))]
            if (tensor["format"] != "ND" or tensor["contiguous"] is not True
                    or tensor["storage_shape"] != shape or tensor["storage_offset_in_elements"] != 0
                    or tensor["strides_in_elements"] != strides):
                raise ValueError(f"success metadata must describe contiguous ND {name}")
        output = metadata["expected_output"]
        if output["shape"] != list(raw_shape[:2]) + [5] or output["dtype"] != raw["dtype"]:
            raise ValueError("decoded metadata does not match the inputs")
        for name, dtype in (("reference_decoded", raw["dtype"]), ("reference_decoded_fp32", "float32")):
            descriptor = metadata["files"][name]
            size = math.prod(output["shape"]) * np.dtype(STORAGE_DTYPES[dtype]).itemsize
            if descriptor["logical_dtype"] != dtype or descriptor["bytes"] != size:
                raise ValueError(f"inconsistent output storage for {name}")
    return metadata

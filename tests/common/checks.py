"""Shared self-checks for declared cases and their portable generated assets."""

import json
from pathlib import Path
import tempfile

import numpy as np

from tests.common.case_spec import TOLERANCES
from tests.common.generate_data import file_sha256, generate_suite
from tests.common.reference import (
    fire_candidate_decode_fusion_reference, materialize_case, validate_case,
)
from tests.common.verify_result import verify_case_output


def check_declarations(test, cases, category):
    names = [case.name for case in cases]
    test.assertEqual(len(names), len(set(names)))
    for case in cases:
        with test.subTest(case=case.name):
            test.assertEqual(case.category, category)
            test.assertTrue(case.name.startswith(category + "_"))
            errors = validate_case(case, materialize_case(case)).errors
            if case.expectation == "reject":
                test.assertIn(case.expected_error, errors)
            else:
                test.assertEqual(case.expectation, "success")
                test.assertIsNone(case.expected_error)
                test.assertEqual(errors, ())


def check_generated_data(test, cases, category):
    with tempfile.TemporaryDirectory() as temporary:
        root = Path(temporary) / category
        manifest = generate_suite(root, cases)
        test.assertEqual(manifest["operator"], "FireCandidateDecodeFusion")
        test.assertEqual(manifest["category"], category)
        test.assertEqual(manifest["counts"]["total"], len(cases))
        test.assertEqual(manifest["catalog_audit_failures"], [])
        test.assertEqual(manifest["generation"]["reference_compute_dtype"], "float32")
        for entry, case in zip(manifest["cases"], cases):
            with test.subTest(case=case.name):
                case_dir = root / case.name
                test.assertEqual(file_sha256(root / entry["metadata"]), entry["case_metadata_sha256"])
                metadata = json.loads((case_dir / "case.json").read_text(encoding="utf-8"))
                data = materialize_case(case)
                test.assertEqual(metadata["category"], category)
                for name, descriptor in metadata["files"].items():
                    path = case_dir / descriptor["path"]
                    test.assertEqual(path.stat().st_size, descriptor["bytes"])
                    test.assertEqual(file_sha256(path), descriptor["sha256"])
                for name, tensor in (("raw_pred", data.raw_pred), ("priors", data.priors)):
                    description = metadata["input"][name]
                    stored = np.fromfile(case_dir / metadata["files"][name]["path"], dtype=tensor.storage.dtype)
                    np.testing.assert_array_equal(stored, tensor.storage.ravel())
                    reconstructed = np.ndarray(
                        shape=tuple(description["shape"]), dtype=tensor.values.dtype, buffer=stored,
                        offset=description["storage_offset_in_elements"] * stored.dtype.itemsize,
                        strides=tuple(stride * stored.dtype.itemsize for stride in description["strides_in_elements"]),
                    )
                    np.testing.assert_array_equal(reconstructed, tensor.values)
                if case.expectation == "reject":
                    test.assertIn(case.expected_error, metadata["validation_errors"])
                    test.assertIsNone(metadata["expected_output"]["shape"])
                    test.assertNotIn("reference_decoded", metadata["files"])
                    continue
                policy = metadata["expected_output"]["comparison"]
                test.assertEqual((policy["absolute_tolerance"], policy["relative_tolerance"]), TOLERANCES[case.raw_pred_dtype])
                result = fire_candidate_decode_fusion_reference(case, data)
                for name, values in (("reference_decoded", result.values), ("reference_decoded_fp32", result.fp32)):
                    descriptor = metadata["files"][name]
                    stored = np.fromfile(case_dir / descriptor["path"], dtype=descriptor["storage_dtype"])
                    np.testing.assert_array_equal(stored, values.ravel())
                actual = case_dir / "reference_decoded.bin"
                test.assertTrue(verify_case_output(case_dir, actual, atol=0, rtol=0).passed)
        with test.assertRaises(FileExistsError):
            generate_suite(root, cases)

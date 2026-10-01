"""Checks that input changes and stale metadata cannot pass verification."""

import json
from pathlib import Path
import tempfile
import unittest

from tests.common.artifacts import load_case_metadata, load_suite_manifest
from tests.common.generate_data import generate_suite
from tests.common.verify_result import verify_case_output
from tests.correctness.cases import CASES


class ArtifactTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name) / "suite"
        self.case = CASES[0]
        generate_suite(self.root, (self.case,))
        self.folder = self.root / self.case.name

    def test_every_input_and_reference_is_checked(self):
        for filename in ("raw_pred.bin", "priors.bin", "reference_decoded.bin", "reference_decoded_fp32.bin"):
            with self.subTest(file=filename):
                path = self.folder / filename
                original = path.read_bytes()
                damaged = bytearray(original)
                damaged[0] ^= 1
                path.write_bytes(damaged)
                with self.assertRaisesRegex(ValueError, "SHA256"):
                    verify_case_output(self.folder, self.folder / "reference_decoded.bin")
                path.write_bytes(original)

    def test_truncated_input_cannot_pass_with_old_output(self):
        path = self.folder / "raw_pred.bin"
        path.write_bytes(path.read_bytes()[:-1])
        with self.assertRaisesRegex(ValueError, "byte size"):
            verify_case_output(self.folder, self.folder / "reference_decoded.bin")

    def test_metadata_sha_is_checked_against_manifest(self):
        path = self.folder / "case.json"
        metadata = json.loads(path.read_text())
        metadata["patterns"]["seed"] += 1
        path.write_text(json.dumps(metadata))
        with self.assertRaisesRegex(ValueError, "metadata SHA256"):
            load_case_metadata(self.folder)

    def test_missing_manifest_is_an_error(self):
        (self.root / "manifest.json").unlink()
        with self.assertRaises(FileNotFoundError):
            load_case_metadata(self.folder)

    def test_manifest_entry_identity_expectation_and_counts_are_checked(self):
        path = self.root / "manifest.json"
        original = path.read_text()
        for change in ("expectation", "counts", "path"):
            with self.subTest(change=change):
                manifest = json.loads(original)
                if change == "expectation":
                    manifest["cases"][0]["expectation"] = "reject"
                    manifest["counts"].update(success=0, reject=1)
                elif change == "counts":
                    manifest["counts"]["total"] = 2
                else:
                    manifest["cases"][0]["metadata"] = "../case.json"
                path.write_text(json.dumps(manifest))
                with self.assertRaises(ValueError):
                    load_case_metadata(self.folder)
                path.write_text(original)

    def test_valid_manifest_and_case_can_be_loaded(self):
        self.assertEqual(load_suite_manifest(self.root)["counts"]["total"], 1)
        self.assertEqual(load_case_metadata(self.folder)["name"], self.case.name)


if __name__ == "__main__":
    unittest.main()

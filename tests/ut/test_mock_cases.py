"""Mock catalog, serialized fixtures, and independent formula checks."""

import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

import numpy as np

from tests.common.cases import TOLERANCES
from tests.common.tensor_io import file_sha256
from tests.common.reference import validate_case
from tests.ut.helpers import scalar_decode
from tests.common.cases import MOCK_CASES as CASES
from tests.common.mock_cases import generate_mock_suite as generate


class MockTests(unittest.TestCase):
    def test_case_numbers_shapes_and_dtypes(self):
        shapes = ((1, 32), (2, 33), (4, 4096), (4, 4097), (8, 32768), (8, 32769))
        self.assertEqual(len(CASES), 12)
        for number, case in enumerate(CASES, 1):
            self.assertEqual(case.name, f"mock_{number:02d}")
            self.assertEqual(case.category, "mock")
            self.assertEqual(case.expectation, "success")
            self.assertEqual(case.raw_pred_shape, shapes[(number - 1) % 6] + (6,))
            self.assertEqual(case.raw_pred_dtype, "float32" if number <= 6 else "float16")
            self.assertEqual(case.priors_dtype, "float32")
            self.assertEqual(case.seed, 140)
        for case in (CASES[5], CASES[11]):
            self.assertEqual(validate_case(case).errors, ("N_RANGE",))

    def test_generated_fixtures_match_independent_scalar_formula(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "mock"
            rows = generate(root)
            self.assertEqual(json.loads((root / "cases.json").read_text()), rows)
            for row, case in zip(rows, CASES):
                with self.subTest(case=case.name):
                    b, n, _ = case.raw_pred_shape
                    folder = root / f"{row['number']:02d}"
                    hashes = json.loads((folder / "fixture.json").read_text())
                    self.assertEqual(set(hashes), {"raw_pred.bin", "priors.bin", "reference.bin"})
                    for name, digest in hashes.items():
                        self.assertEqual(file_sha256(folder / name), digest)
                    raw = np.fromfile(folder / "raw_pred.bin", dtype=case.raw_pred_dtype).reshape(b, n, 6)
                    priors = np.fromfile(folder / "priors.bin", dtype="<f4").reshape(n, 5)
                    reference = np.fromfile(folder / "reference.bin", dtype=case.raw_pred_dtype).reshape(b, n, 5)
                    atol, rtol = TOLERANCES[case.raw_pred_dtype]
                    self.assertEqual((row["atol"], row["rtol"]), (atol, rtol))
                    self.assertEqual(row["decoded_shape"], [b, n, 5])
                    self.assertTrue(np.isfinite(raw).all())
                    self.assertTrue(np.all((raw >= -4) & (raw <= 4)))
                    self.assertTrue(np.all(priors[:, :2] >= 0))
                    self.assertTrue(np.all(priors[:, 2] == 8))
                    self.assertTrue(np.all((priors[:, 3:] >= 1) & (priors[:, 3:] <= 4)))
                    self.assertTrue(np.isfinite(reference).all())
                    for batch in (0, b - 1):
                        for candidate in (0, n // 2, n - 1):
                            expected = scalar_decode(raw[batch, candidate], priors[candidate]).astype(case.raw_pred_dtype)
                            np.testing.assert_allclose(reference[batch, candidate], expected, atol=atol, rtol=rtol)

    def test_generation_is_repeatable_and_refuses_overwrite(self):
        with tempfile.TemporaryDirectory() as temporary:
            first, second = (Path(temporary) / name for name in ("first", "second"))
            generate(first, (CASES[1], CASES[7]))
            generate(second, (CASES[1], CASES[7]))
            for path in first.rglob("*"):
                if path.is_file():
                    self.assertEqual(path.read_bytes(), (second / path.relative_to(first)).read_bytes())
            with self.assertRaises(FileExistsError):
                generate(first, (CASES[1], CASES[7]))

    def test_cli_selection_preserves_case_number_and_rejects_unknown_case(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "selected"
            command = [sys.executable, "-m", "tests.st.scripts.gen_data", "--suite", "mock", "--output", str(root)]
            result = subprocess.run(command + ["--case", "mock_12"], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            rows = json.loads((root / "cases.json").read_text())
            self.assertEqual([row["number"] for row in rows], [12])
            self.assertEqual(rows[0]["shape"], [8, 32769, 6])
            self.assertTrue((root / "12/reference.bin").is_file())
            unknown = Path(temporary) / "unknown"
            result = subprocess.run(
                [sys.executable, "-m", "tests.st.scripts.gen_data", "--suite", "mock", "--output", str(unknown), "--case", "mock_99"],
                capture_output=True, text=True,
            )
            self.assertNotEqual(result.returncode, 0)
            self.assertFalse(unknown.exists())


if __name__ == "__main__":
    unittest.main()

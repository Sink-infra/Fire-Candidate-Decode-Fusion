"""Mock catalog, serialized fixtures, and independent formula checks."""

import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

import numpy as np

from tests.common.cases import TOLERANCES, CASE_ROOT, load_cases
from tests.common.tensor_io import file_sha256
from tests.common.reference import validate_case
from tests.ut.helpers import scalar_decode
from tests.common.cases import MOCK_CASES as CASES
from tests.common.mock_cases import generate_mock_suite as generate, fixture


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
            self.assertEqual(case.error_ratio_mode, "complement-half-up")
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
                    synthetic = np.ones(raw.size, dtype=bool)
                    raw_bits = raw.view(np.uint16 if case.raw_pred_dtype == "float16" else np.uint32).reshape(-1)
                    for index, bits in case.observed_raw_pred_bits:
                        self.assertEqual(int(raw_bits[index]), bits)
                        synthetic[index] = False
                    remaining = raw.reshape(-1)[synthetic]
                    self.assertTrue(np.all((remaining >= -4) & (remaining <= 4)))
                    self.assertTrue(np.all(priors[:, :2] >= 0))
                    self.assertTrue(np.all(priors[:, 2] == 8))
                    self.assertTrue(np.all((priors[:, 3:] >= 1) & (priors[:, 3:] <= 4)))
                    self.assertTrue(np.isfinite(reference).all())
                    for batch in (0, b - 1):
                        for candidate in (0, 2, n // 2, n - 1):
                            expected = scalar_decode(raw[batch, candidate], priors[candidate]).astype(case.raw_pred_dtype)
                            np.testing.assert_allclose(reference[batch, candidate], expected, atol=atol, rtol=rtol)

    def test_observed_bits_change_only_declared_input_elements_and_their_references(self):
        for dtype, samples in (("float32", ((0, 0xc1400000), (17, 0x41400000))),
                               ("float16", ((0, 0x4901), (17, 0xc480)))):
            with self.subTest(dtype=dtype), tempfile.TemporaryDirectory() as temporary:
                root = Path(temporary)
                ordinary, observed = root / "ordinary", root / "observed"
                fixture(ordinary, 1, 32, dtype)
                fixture(observed, 1, 32, dtype, observed_raw_pred_bits=samples)
                word_type = np.uint16 if dtype == "float16" else np.uint32
                original = np.fromfile(ordinary / "raw_pred.bin", dtype=word_type)
                actual = np.fromfile(observed / "raw_pred.bin", dtype=word_type)
                expected = original.copy()
                for index, bits in samples:
                    expected[index] = bits
                np.testing.assert_array_equal(actual, expected)
                self.assertEqual((ordinary / "priors.bin").read_bytes(), (observed / "priors.bin").read_bytes())
                raw = np.fromfile(observed / "raw_pred.bin", dtype=dtype).reshape(32, 6)
                prior = np.fromfile(observed / "priors.bin", dtype="<f4").reshape(32, 5)
                reference = np.fromfile(observed / "reference.bin", dtype=dtype).reshape(32, 5)
                for candidate in (0, 2):
                    np.testing.assert_allclose(reference[candidate], scalar_decode(raw[candidate], prior[candidate]).astype(dtype),
                                               atol=TOLERANCES[dtype][0], rtol=TOLERANCES[dtype][1])
                unchanged = [candidate for candidate in range(32) if candidate not in (0, 2)]
                ordinary_reference = np.fromfile(ordinary / "reference.bin", dtype=dtype).reshape(32, 5)
                np.testing.assert_array_equal(reference[unchanged], ordinary_reference[unchanged])

    def test_observed_bit_declarations_are_finite_in_range_and_typed(self):
        document = json.loads((CASE_ROOT / "mock.json").read_text())
        first = document["cases"][0]
        for samples in ({"0": "0xc1400000", "17": "0x41400000"},
                        {"-1": "0x00000000"}, {"192": "0x00000000"},
                        {"0": "0x0000"}, {"0": "0x7fc00000"}, {"0": "0xff800000"}, []):
            with self.subTest(samples=samples), tempfile.TemporaryDirectory() as temporary:
                first["observed_raw_pred_bits"] = samples
                path = Path(temporary) / "mock.json"
                path.write_text(json.dumps(document))
                if samples == {"0": "0xc1400000", "17": "0x41400000"}:
                    case = load_cases("mock", Path(temporary))[0]
                    self.assertEqual(case.observed_raw_pred_bits, ((0, 0xc1400000), (17, 0x41400000)))
                else:
                    with self.assertRaises(ValueError):
                        load_cases("mock", Path(temporary))

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

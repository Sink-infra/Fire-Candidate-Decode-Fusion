"""Generate standalone synthetic Mock inputs and float32-based references."""
import json
from pathlib import Path

import numpy as np

from tests.common.cases import MOCK_CASES, TOLERANCES
from tests.common.tensor_io import file_sha256
from tests.common.reference import STORAGE_DTYPES, sigmoid_fp32


def fixture(path, b, n, dtype, seed=140):
    """Write synthetic inputs and references for a fixed Mock workload."""
    path.mkdir(parents=True, exist_ok=False)
    rng = np.random.default_rng(seed)
    raw = rng.uniform(-4, 4, (b, n, 6)).astype(STORAGE_DTYPES[dtype])
    priors = np.empty((n, 5), np.dtype(STORAGE_DTYPES["float32"]))
    index = np.arange(n)
    priors[:, 0] = index % 80
    priors[:, 1] = (index // 80) % 80
    priors[:, 2] = 8
    priors[:, 3:] = rng.uniform(1, 4, (n, 2))
    raw.tofile(path / "raw_pred.bin")
    priors.tofile(path / "priors.bin")
    activation = sigmoid_fp32(raw)
    doubled = np.float32(2) * activation[..., :4]
    reference = np.empty((b, n, 5), np.float32)
    reference[..., :2] = (doubled[..., :2] - np.float32(.5) + priors[None, :, :2]) * priors[None, :, 2:3]
    reference[..., 2:4] = (doubled[..., 2:4] * doubled[..., 2:4]) * priors[None, :, 3:5] * priors[None, :, 2:3]
    reference[..., 4] = activation[..., 4] * activation[..., 5]
    reference.astype(STORAGE_DTYPES[dtype]).tofile(path / "reference.bin")
    (path / "case.conf").write_text(
        f"fixture_format=1\nbatch={b}\ncandidates={n}\ndtype={dtype}\n", encoding="utf-8",
    )
    hashes = {p.name: file_sha256(p) for p in sorted(path.glob("*.bin"))}
    (path / "fixture.json").write_text(json.dumps(hashes, indent=2) + "\n", encoding="utf-8")


def generate_mock_suite(output, cases=MOCK_CASES):
    if not cases or any(case not in MOCK_CASES for case in cases):
        raise ValueError("select declared Mock cases")
    output = Path(output)
    output.mkdir(parents=True, exist_ok=False)
    rows = []
    for case in cases:
        number = int(case.name.removeprefix("mock_"))
        b, n, _ = case.raw_pred_shape
        path = output / f"{number:02d}"
        fixture(path, b, n, case.raw_pred_dtype, case.seed)
        atol, rtol = TOLERANCES[case.raw_pred_dtype]
        rows.append({"number": number, "name": case.name, "shape": list(case.raw_pred_shape),
                     "dtype": case.raw_pred_dtype, "seed": case.seed,
                     "priors_shape": [n, 5], "priors_dtype": "float32",
                     "decoded_shape": [b, n, 5], "decoded_dtype": case.raw_pred_dtype,
                     "atol": atol, "rtol": rtol})
    (output / "cases.json").write_text(json.dumps(rows, indent=2) + "\n", encoding="utf-8")
    return rows

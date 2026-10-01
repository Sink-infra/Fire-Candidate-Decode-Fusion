"""Generate only Correctness inputs and references."""

from tests.common.generate_data import main
from tests.correctness.cases import CASES

if __name__ == "__main__":
    raise SystemExit(main(CASES, "correctness"))

"""Generate only Coverage inputs and references."""

from tests.common.generate_data import main
from tests.coverage.cases import CASES

if __name__ == "__main__":
    raise SystemExit(main(CASES, "coverage"))

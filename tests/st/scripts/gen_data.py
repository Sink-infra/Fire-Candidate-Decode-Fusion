"""List and generate one named FireCandidateDecodeFusion test suite."""

import argparse
from collections import Counter
from dataclasses import asdict
import json
from pathlib import Path

from tests.common.cases import SUITES, select_cases
from tests.common.mock_cases import generate_mock_suite
from tests.common.tensor_io import generate_suite


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--suite", choices=SUITES, required=True)
    parser.add_argument("--list", action="store_true", help="list selected case specifications")
    parser.add_argument("--output", type=Path, help="new directory for input and reference data")
    parser.add_argument("--case", action="append", default=[], help="exact case name; repeatable")
    parser.add_argument("--tag", action="append", default=[], help="required coverage tag; repeatable")
    parser.add_argument("--expectation", choices=("all", "success", "reject"), default="all")
    args = parser.parse_args()
    if not args.list and args.output is None:
        parser.error("--output is required for data generation")
    try:
        cases = select_cases(args.suite, names=args.case, tags=args.tag, expectation=args.expectation)
        if args.list:
            print(json.dumps([asdict(case) for case in cases], indent=2, ensure_ascii=False))
            return 0
        output = args.output.resolve()
        if args.suite == "mock":
            generate_mock_suite(output, cases)
        else:
            generate_suite(output, cases)
    except (OSError, KeyError, TypeError, ValueError) as error:
        parser.error(str(error))
    counts = Counter(case.expectation for case in cases)
    print(json.dumps({"category": args.suite,
                      "counts": {"total": len(cases), "success": counts["success"], "reject": counts["reject"]},
                      "output": str(output)}, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

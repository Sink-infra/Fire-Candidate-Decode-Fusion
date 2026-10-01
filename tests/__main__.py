"""Report independent suite sizes and input scales without materializing data."""

import argparse
import json

from tests.common.cases import CASE_GROUPS


def summary():
    result = {}
    for category, cases in CASE_GROUPS.items():
        runnable = [case for case in cases if case.expectation == "success"]
        result[category] = {
            "count": len(cases),
            "runnable": len(runnable),
            "reject": sum(case.expectation == "reject" for case in cases),
            "dtypes": sorted({case.raw_pred_dtype for case in runnable}),
            "max_batch": max((case.raw_pred_shape[0] for case in runnable), default=0),
            "max_candidates": max((case.raw_pred_shape[1] for case in runnable), default=0),
            "max_total_candidates": max((case.raw_pred_shape[0] * case.raw_pred_shape[1] for case in runnable), default=0),
        }
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--details", action="store_true")
    args = parser.parse_args()
    result = summary()
    print(json.dumps(result if args.details else {key: value["count"] for key, value in result.items()},
                     indent=2, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

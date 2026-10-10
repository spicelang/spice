#!/usr/bin/env python3
"""Generate HTML and text coverage reports from the gcov data collected by the test suite.

There are two reports, both generated unless --only selects one:
- compiler: the compiler sources in src/ exercised by the test suite. Requires the test suite to have been run once with
  `python test/run-tests.py --instrument coverage`, which builds the compiler with Spice code coverage instrumentation and
  runs the test cases against it. The resulting .gcno/.gcda files land next to the compiler executable in
  build/test-tmp/compiler/ and next to its builtin tests in build/test-tmp/compiler-tests/. The report lands in
  build/coverage-compiler/ and build/coverage-compiler.txt.
- std: the std lib sources exercised by the test programs. Requires the test suite to have been run once with
  `python test/run-tests.py --coverage --work-dir build/test-tmp-coverage`, which instruments every Spice program compiled
  during the run (including std lib dependencies pulled in by test files) for gcov-compatible coverage output. The
  resulting .gcno/.gcda files land per test case under build/test-tmp-coverage/tests/, scattered across many separate
  compilations of the same std lib files - gcovr aggregates coverage across all of them into one per-line report. The
  report lands in build/coverage-spice/ and build/coverage-spice.txt.

The gcov data LLVM emits for Spice code is only understood by `llvm-cov gcov`, not GNU gcov (they disagree on the on-disk
data format version), so LLVM_COV must point at a matching llvm-cov build.
"""
import argparse
import os
import subprocess
from pathlib import Path

ROOT_DIR = Path(__file__).resolve().parent
BUILD_DIR = ROOT_DIR / "build"

# Report name -> (output name, source filter, dirs containing the gcov data), paths relative to the build dir
REPORTS = {
    "compiler": ("coverage-compiler", "../src/.*", ["test-tmp/compiler", "test-tmp/compiler-tests"]),
    # Only the test programs. The coverage-instrumented compiler also covers std lib files, which would mix the std lib
    # usage of the compiler into this report
    "std": ("coverage-spice", "../std/.*", ["test-tmp-coverage/tests"]),
}


def generate_report(output_name: str, source_filter: str, data_dirs: list[str]) -> None:
    llvm_cov = os.environ.get("LLVM_COV", "llvm-cov")
    base_args = [
        "gcovr",
        "--gcov-executable",
        f"{llvm_cov} gcov",
        # The failure branch of an assert should never be taken, so only the success branch has to be covered. The line
        # itself stays in the report, since its condition is executed. gcovr matches the pattern at the start of the line
        "--exclude-branches-by-pattern",
        r"\s*assert\b",
        "--gcov-ignore-parse-errors",
        "negative_hits.warn_once_per_file",
        "--filter",
        source_filter,
        "-r",
        "..",
        *data_dirs,
    ]
    (BUILD_DIR / output_name).mkdir(exist_ok=True)
    subprocess.run(base_args + ["--html", "--html-details", "-s", "-o", f"{output_name}/index.html"], cwd=BUILD_DIR, check=True)
    subprocess.run(base_args + ["--txt", "-o", f"{output_name}.txt"], cwd=BUILD_DIR, check=True)


def main() -> None:
    parser = argparse.ArgumentParser(description="Generate coverage reports from the gcov data collected by the test suite")
    parser.add_argument("--only", choices=list(REPORTS), help="only generate this report (default: all reports)")
    args = parser.parse_args()
    for report in [args.only] if args.only else REPORTS:
        output_name, source_filter, data_dirs = REPORTS[report]
        print(f"Generating {report} coverage report in build/{output_name}/", flush=True)
        generate_report(output_name, source_filter, data_dirs)


if __name__ == "__main__":
    main()

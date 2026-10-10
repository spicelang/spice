#!/usr/bin/env python3
"""Generate an HTML coverage report for std lib Spice source files exercised by the test suite.

Requires the test suite to have been run once with
`python test/run-tests.py --coverage --work-dir build/test-tmp-coverage`, which instruments every Spice
program compiled during the run (including std lib dependencies pulled in by test files) for
gcov-compatible coverage output. The resulting .gcno/.gcda files land per test case under
build/test-tmp-coverage/tests/, scattered across many separate compilations of the same std lib files -
gcovr aggregates coverage across all of them into one per-line report. Run it from the build/ dir.

The gcov data LLVM emits for Spice code is only understood by `llvm-cov gcov`, not GNU gcov (they
disagree on the on-disk data format version), so LLVM_COV must point at a matching llvm-cov build.
"""
import os
import subprocess
from pathlib import Path

Path("coverage-spice").mkdir(exist_ok=True)

llvm_cov = os.environ.get("LLVM_COV", "llvm-cov")

base_args = [
    "gcovr",
    "--gcov-executable",
    f"{llvm_cov} gcov",
    "--exclude-lines-by-pattern",
    "assert",
    "--gcov-ignore-parse-errors",
    "negative_hits.warn_once_per_file",
    "--filter",
    "../std/.*",
    "-r",
    "..",
    # Only the test programs. The coverage-instrumented compiler (see coverage-bootstrap.py) also covers std lib files,
    # which would mix the std lib usage of the compiler into this report
    "test-tmp-coverage/tests",
]

subprocess.run(
    base_args + ["--html", "--html-details", "-s", "-o", "coverage-spice/index.html"],
    check=True,
)
subprocess.run(
    base_args + ["--txt", "-o", "coverage-spice.txt"],
    check=True,
)

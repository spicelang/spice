#!/usr/bin/env python3
"""Generate HTML and text coverage reports for the bootstrap compiler sources exercised by the test suite.

Requires the test suite to have been run once with `spicetest --bootstrap-coverage`, which builds the bootstrap compiler
with Spice code coverage instrumentation and runs the test cases against it. The resulting .gcno/.gcda files land next to
the bootstrap compiler executable in test/test-tmp/bootstrap-compiler/ (see BootstrapUtil::buildBootstrapCompiler) and next
to the builtin tests of the bootstrap compiler in test/test-tmp/bootstrap-tests/ (see BootstrapUtil::buildBootstrapBuiltinTests).

The gcov data LLVM emits for Spice code is only understood by `llvm-cov gcov`, not GNU gcov (they disagree on the on-disk
data format version), so LLVM_COV must point at a matching llvm-cov build.
"""
import os
import subprocess
from pathlib import Path

Path("coverage-bootstrap").mkdir(exist_ok=True)

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
    "../src-bootstrap/.*",
    "-r",
    "..",
    "test/test-tmp/bootstrap-compiler",
    "test/test-tmp/bootstrap-tests",
]

subprocess.run(
    base_args + ["--html", "--html-details", "-s", "-o", "coverage-bootstrap/index.html"],
    check=True,
)
subprocess.run(
    base_args + ["--txt", "-o", "coverage-bootstrap.txt"],
    check=True,
)

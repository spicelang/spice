#!/usr/bin/env python3
"""Generate HTML and text coverage reports using gcovr."""
import subprocess
from pathlib import Path

Path("coverage").mkdir(exist_ok=True)

base_args = [
    "gcovr",
    "--exclude-lines-by-pattern",
    "assert",
    "--gcov-ignore-parse-errors",
    "negative_hits.warn_once_per_file",
    # test/test-tmp holds gcov data from spicetest --coverage (Spice-level std lib coverage, see
    # coverage-spice.py), emitted by LLVM's GCOVProfilerPass in a format GNU gcov can't parse. Left
    # unexcluded, gcovr aborts entirely the moment its search (which isn't limited by -r/filter) finds one.
    "--exclude-directories",
    ".*test-tmp.*",
    "-r",
    ".",
]

subprocess.run(
    base_args + ["--html", "--html-details", "-s", "-o", "coverage/index.html"],
    check=True,
)
subprocess.run(
    base_args + ["--txt", "-o", "coverage.txt"],
    check=True,
)

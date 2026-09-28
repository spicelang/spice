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
    # gcov has a known bug that miscounts hits in range-for loops over containers in header files
    # (e.g. CustomHashFunctions.h), reporting implausibly high "suspicious" hit counts.
    # See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=68080. gcovr treats this as a hard error
    # unless told to just warn about it, so ignore it the same way as negative hits above.
    # --gcov-ignore-parse-errors uses argparse's "append" action, so it must be repeated once per
    # value rather than combined into a single comma-separated argument.
    "--gcov-ignore-parse-errors",
    "suspicious_hits.warn_once_per_file",
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

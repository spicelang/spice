#!/usr/bin/env python3
"""Configure the Bencher project 'spice': names and units of the measures, and the plots of its perf page.
See media/specs/benchmarks.md, section 'History and visualization'.

Run it after the first upload of a new benchmark or measure, which Bencher creates without units and adds to no plot.
The benchmarks of a plot are selected by name pattern from the benchmarks known to Bencher. Plots are matched by
title: existing ones are updated, missing ones created, others left alone.

  BENCHER_API_KEY=bencher_user_... python benchmark/bencher-setup.py [--dry-run]

The key has to be a user API key: the project key of the nightly upload (bencher_run_*) can only upload.
"""
import argparse
import fnmatch
import json
import os
import sys
import urllib.error
import urllib.request

API_URL = "https://api.bencher.dev/v0/projects/spice"
BRANCH = "main"
TESTBED = "ubuntu-26.04-x86_64"
WINDOW = 365 * 24 * 3600  # Seconds
MAX_DIMENSION_ENTRIES = 8  # Benchmarks or measures per plot

# Bencher's built-in measures 'instructions' and 'file-size' have their names and units already
STAGES = {
    "lexer-and-parser": "Lexer and Parser",
    "import-collector": "Import Collector",
    "symbol-table-builder": "Symbol Table Builder",
    "type-checker-pre": "Type Checker (pre)",
    "type-checker-post": "Type Checker (post)",
    "ir-generator": "IR Generator",
    "ir-optimizer": "IR Optimizer",
    "bitcode-linker": "Bitcode Linker",
    "lto-optimizer": "LTO Optimizer",
    "object-emitter": "Object Emitter",
}
COUNTS = {
    "source-files": "Source Files",
    "lines": "Lines",
    "types": "Types",
    "type-checker-runs": "Type Checker Runs",
    "ir-functions": "IR Functions",
    "ir-instructions": "IR Instructions",
}
MEASURES = {
    "max-rss": ("Peak Memory", "bytes (B)"),
    "wall-time": ("Wall Time", "nanoseconds (ns)"),
    **{f"stage-{slug}": (f"Stage: {name}", "microseconds (µs)") for slug, name in STAGES.items()},
    **{f"count-{slug}": (f"Count: {name}", "count") for slug, name in COUNTS.items()},
}

FRONT_END_STAGES = [f"stage-{slug}" for slug in list(STAGES)[:5]]
BACK_END_STAGES = [f"stage-{slug}" for slug in list(STAGES)[5:]]
# Title, benchmark name pattern, measure slugs, y-axis scale. The order is the order on the perf page
PLOTS = [
    ("Compile -O0: instructions", "compile/*/O0-*", ["instructions"], "log"),
    ("Compile -O2: instructions", "compile/*/O2-*", ["instructions"], "log"),
    ("Compile -O0: wall time", "compile/*/O0-*", ["wall-time"], "log"),
    ("Compile -O2: wall time", "compile/*/O2-*", ["wall-time"], "log"),
    ("Compile -O0: peak memory", "compile/*/O0-*", ["max-rss"], "auto"),
    ("Compile -O2: peak memory", "compile/*/O2-*", ["max-rss"], "auto"),
    ("Runtime: instructions", "runtime/*", ["instructions"], "log"),
    ("Runtime: wall time", "runtime/*", ["wall-time"], "auto"),
    ("Runtime: peak memory", "runtime/*", ["max-rss"], "log"),
    ("Runtime: binary size", "runtime/*", ["file-size"], "auto"),
    ("Self-compile: instructions", "self-compile*", ["instructions"], "auto"),
    ("Self-compile: wall time", "self-compile*", ["wall-time"], "auto"),
    ("Self-compile: peak memory", "self-compile*", ["max-rss"], "auto"),
    ("Self-compile: front-end stage times", "self-compile/release", FRONT_END_STAGES, "log"),
    ("Self-compile: back-end stage times", "self-compile/release", BACK_END_STAGES, "log"),
    ("Self-compile: compiler binary size", "self-compile/release", ["file-size"], "auto"),
    ("Self-compile: IR size", "self-compile/release", ["count-ir-functions", "count-ir-instructions"], "log"),
]


def request(method: str, path: str, key: str | None = None, body: dict | None = None):
    headers = {"Content-Type": "application/json"}
    if key:
        headers["Authorization"] = f"Bearer {key}"
    data = json.dumps(body).encode() if body is not None else None
    req = urllib.request.Request(API_URL + path, data=data, headers=headers, method=method)
    try:
        with urllib.request.urlopen(req, timeout=30) as response:
            return json.load(response)
    except urllib.error.HTTPError as e:
        sys.exit(f"{method} {path} failed with {e.code}: {e.read().decode()}")


def list_all(path: str, key: str | None) -> list[dict]:
    items = []
    page = 1
    while True:
        batch = request("GET", f"{path}?per_page=255&page={page}", key)
        items += batch
        if len(batch) < 255:
            return items
        page += 1


def uuid_of(items: list[dict], what: str, name: str) -> str:
    for item in items:
        if name in (item["name"], item["slug"]):
            return item["uuid"]
    sys.exit(f"Unknown {what} '{name}' in the Bencher project, upload results first")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--dry-run", action="store_true", help="Only print the changes (no API key needed)")
    args = parser.parse_args()
    key = os.environ.get("BENCHER_API_KEY")
    if not key and not args.dry_run:
        sys.exit("BENCHER_API_KEY is not set")
    # The project is public, so reading needs no key
    read_key = None if args.dry_run else key

    measures = list_all("/measures", read_key)
    for slug, (name, units) in MEASURES.items():
        uuid_of(measures, "measure", slug)
        print(f"Measure {slug}: {name}, {units}")
        if not args.dry_run:
            request("PATCH", f"/measures/{slug}", key, {"name": name, "units": units})

    branch = uuid_of(list_all("/branches", read_key), "branch", BRANCH)
    testbed = uuid_of(list_all("/testbeds", read_key), "testbed", TESTBED)
    benchmarks = list_all("/benchmarks", read_key)
    existing = {plot["title"]: plot["uuid"] for plot in list_all("/plots", read_key)}
    for index, (title, pattern, measure_slugs, y_axis) in enumerate(PLOTS, start=1):
        selected = sorted(b["name"] for b in benchmarks if fnmatch.fnmatchcase(b["name"], pattern))
        if not selected:
            sys.exit(f"Plot '{title}': no benchmark matches '{pattern}'")
        if len(selected) > MAX_DIMENSION_ENTRIES or len(measure_slugs) > MAX_DIMENSION_ENTRIES:
            sys.exit(f"Plot '{title}': at most {MAX_DIMENSION_ENTRIES} benchmarks and measures, split the plot")
        plot = {
            "index": index,
            "title": title,
            "lower_value": False,
            "upper_value": False,
            "lower_boundary": False,
            "upper_boundary": False,
            "x_axis": "date_time",
            "y_axis": y_axis,
            "window": WINDOW,
            "branches": [branch],
            "testbeds": [testbed],
            "benchmarks": [uuid_of(benchmarks, "benchmark", name) for name in selected],
            "measures": [uuid_of(measures, "measure", slug) for slug in measure_slugs],
            # Sampled measures have a lower_value and an upper_value (min and max), which would be drawn as lines
            "metrics": ["value"],
        }
        action = "Update" if title in existing else "Create"
        print(f"{action} plot {index} '{title}': {len(selected)} benchmark(s), {', '.join(measure_slugs)}")
        if args.dry_run:
            continue
        if title in existing:
            request("PATCH", f"/plots/{existing[title]}", key, plot)
        else:
            request("POST", "/plots", key, plot)


if __name__ == "__main__":
    main()

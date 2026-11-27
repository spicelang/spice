#!/usr/bin/env python3
"""Samples the cases of the golden test of the diagnostics renderer (WP 1.4) into test/fixtures/diag-golden.txt.

The builtin test `testGoldenDiagnostics` of src-next/diag/golden-test.spice rebuilds the diagnostics of the listed cases from
their `exception.out` or `warning.out` (family, code, location and the length of the marker) and the source files next to
them, renders them again and compares the text with the reference file. A case qualifies if its reference file has the
grammar below and its sources are plain ASCII (the column of the legacy `Reader` counts code points, the golden test builds
the offset from the column by byte). The sample is seeded, so the list only changes if the test cases or this script change.

Grammar of an `exception.out`, which the test parser implements as well:
  file     := soft-wrapper | block
  soft     := "[Error|Compiler]:\\nUnresolved soft errors: There are unresolved errors. Please fix them and recompile.\\n\\n"
              semantic ("\\n\\n" semantic)*
  block    := semantic | located | unlocated
  semantic := "[Error|Semantic] " path ":" line ":" col ":\\n" prefix ": " message [ "\\n\\n" snippet ]
  snippet  := number "  " text "\\n" spaces "^"+
  located  := "[Error|Lexer] " | "[Error|Parser] " path ":" line ":" col ": " prefix ": " message
  unlocated:= "[Error|CLI] " | "[Error|Linker] " prefix ": " message  |  "[Error|Compiler]:\\n" prefix ": " message

    python tools/sample_diag_golden.py
"""
from __future__ import annotations

import random
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TEST_FILES = ROOT / "test" / "test-files"
OUT = ROOT / "test" / "fixtures" / "diag-golden.txt"
SEED = 1904
SOFT_HEADER = "[Error|Compiler]:\nUnresolved soft errors: There are unresolved errors. Please fix them and recompile.\n\n"
SEMANTIC_RE = re.compile(r"\[Error\|Semantic\] (\S+):(\d+):(\d+):\n([^\n]+)(?:\n\n(\d+)  ([^\n]*)\n( *)(\^+))?\Z", re.S)
LOCATED_RE = re.compile(r"\[Error\|(Lexer|Parser)\] (\S+):(\d+):(\d+): [^\n]+\Z")
UNLOCATED_RE = re.compile(r"\[Error\|(CLI|Linker)\] [^\n]+\Z|\[Error\|Compiler\]:\n[^\n]+\Z")
WARNING_RE = re.compile(r"\[Warning\] (\S+):(\d+):(\d+): [^\n]+\Z")

# Quotas of the 50 cases: (category, count)
QUOTAS = [("hard", 20), ("soft", 12), ("parser", 4), ("lexer", 3), ("cli", 3), ("warning", 8)]


def sources_ok(case: Path, path: str, line: int, col: int) -> bool:
    source = case / Path(path).name
    if not source.is_file():
        return False
    data = source.read_bytes()
    if any(b > 127 for b in data) or b"\r" in data:
        return False
    lines = data.split(b"\n")
    return 1 <= line <= len(lines) and 1 <= col <= len(lines[line - 1]) + 1


def semantic_ok(case: Path, block: str) -> bool:
    m = SEMANTIC_RE.match(block)
    return m is not None and sources_ok(case, m.group(1), int(m.group(2)), int(m.group(3)))


def categorize(case: Path) -> str | None:
    exception = case / "exception.out"
    warning = case / "warning.out"
    if exception.is_file():
        text = exception.read_text(encoding="utf-8", errors="strict")
        if text.startswith(SOFT_HEADER):
            blocks = text[len(SOFT_HEADER):].split("\n\n[Error|Semantic] ")
            blocks = [blocks[0]] + ["[Error|Semantic] " + b for b in blocks[1:]]
            return "soft" if all(semantic_ok(case, b) for b in blocks) else None
        if text.startswith("[Error|Semantic]"):
            return "hard" if semantic_ok(case, text) else None
        m = LOCATED_RE.match(text)
        if m:
            return m.group(1).lower() if sources_ok(case, m.group(2), int(m.group(3)), int(m.group(4))) else None
        if UNLOCATED_RE.match(text):
            return "cli" if text.startswith("[Error|CLI]") else None
        return None
    if warning.is_file():
        lines = [l for l in warning.read_text(encoding="utf-8").split("\n") if l]
        ok = lines and all((m := WARNING_RE.match(l)) and sources_ok(case, m.group(1), int(m.group(2)), int(m.group(3))) for l in lines)
        return "warning" if ok else None
    return None


def main() -> int:
    candidates: dict[str, list[str]] = {name: [] for name, _ in QUOTAS}
    for ref in sorted(list(TEST_FILES.rglob("exception.out")) + list(TEST_FILES.rglob("warning.out"))):
        case = ref.parent
        try:
            category = categorize(case)
        except UnicodeDecodeError:
            category = None
        if category in candidates:
            candidates[category].append(f"{'exception' if ref.name == 'exception.out' else 'warning'} {case.relative_to(ROOT).as_posix()}")
    rng = random.Random(SEED)
    chosen: list[str] = []
    for name, count in QUOTAS:
        pool = candidates[name]
        if len(pool) < count:
            print(f"error: only {len(pool)} candidates for '{name}', need {count}", file=sys.stderr)
            return 1
        chosen += rng.sample(pool, count)
    OUT.write_text("# Cases of the golden test of the diagnostics renderer, see tools/sample_diag_golden.py\n" + "\n".join(sorted(chosen)) + "\n", encoding="utf-8")
    print(f"{len(chosen)} cases written to {OUT.relative_to(ROOT)}; candidates: " + ", ".join(f"{k} {len(v)}" for k, v in candidates.items()))
    return 0


if __name__ == "__main__":
    sys.exit(main())

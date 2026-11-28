#!/usr/bin/env python3
"""One-off extraction of src-next/diag/diagnostics.def from the enums and prefix tables of the legacy compiler (WP 1.4).

The legacy compiler is frozen until the cutover, so the definition is extracted once and then maintained by hand (an existing
definition is only overwritten with --force). The script and its `--check`, which the unit tests of the tools run, stay
to document where the entries come from, and to check that nothing was lost: `--check` fails if the legacy sources and the
definition differ in the members, their order or their prefixes.

    python tools/gen/extract_diagnostics_def.py --force    # overwrite src-next/diag/diagnostics.def
    python tools/gen/extract_diagnostics_def.py --check    # compare only
"""
from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent.parent
DEF_PATH = "src-next/diag/diagnostics.def"

# family -> legacy source file. The order is the order in the definition
LEGACY_SOURCES = {
    "Lexer": "src/exception/lexer-error.spice",
    "Parser": "src/exception/parser-error.spice",
    "Semantic": "src/exception/semantic-error.spice",
    "Compiler": "src/exception/compiler-error.spice",
    "Linker": "src/exception/linker-error.spice",
    "Cli": "src/exception/cli-error.spice",
    "Warning": "src/util/compiler-warning.spice",
}

ENUM_RE = re.compile(r"type (\w+) enum \{(.*?)\}", re.S)
PREFIX_RE = re.compile(r'case \w+::(\w+): \{ return "((?:[^"\\]|\\.)*)"; \}')


def extract(root: Path) -> dict[str, tuple[str, list[tuple[str, str]]]]:
    """family -> (enum name, [(member, prefix)]) in the order of the legacy enum."""
    result = {}
    for family, rel in LEGACY_SOURCES.items():
        text = (root / rel).read_text(encoding="utf-8")
        match = ENUM_RE.search(text)
        if match is None:
            raise SystemExit(f"error: no enum in {rel}")
        members = [m.strip() for m in match.group(2).split(",") if m.strip()]
        prefixes = dict(PREFIX_RE.findall(text))
        missing = [m for m in members if m not in prefixes]
        if missing:
            raise SystemExit(f"error: {rel} has no prefix for {', '.join(missing)}")
        result[family] = (match.group(1), [(m, prefixes[m]) for m in members])
    return result


def render(entries: dict[str, tuple[str, list[tuple[str, str]]]]) -> str:
    lines = [
        "# Diagnostic catalog of the compiler re-implementation (media/specs/compiler-reimplementation.md, section 5.3).",
        "# Source of src-next/diag/codes.spice (python tools/gen/generate.py). A code is the index of its member within its family,",
        "# which is the value of the legacy enum, so the order must not change.",
        "#",
        "# [<Family>] starts a family, 'enum = <name>' gives the name of its enum. A member line is '<MEMBER> = <prefix>'. The prefix",
        "# is the text before the message ('<prefix>: <message>'). Extracted from the legacy sources by tools/gen/extract_diagnostics_def.py.",
        "",
    ]
    for family, (enum, members) in entries.items():
        lines += [f"[{family}]", f"enum = {enum}"]
        lines += [f"{member} = {prefix}" for member, prefix in members]
        lines.append("")
    return "\n".join(lines)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--check", action="store_true", help="only compare the legacy sources with the definition")
    parser.add_argument("--force", action="store_true", help="overwrite an existing definition, also if it was edited by hand")
    parser.add_argument("--root", type=Path, default=REPO_ROOT)
    args = parser.parse_args(argv)
    content = render(extract(args.root))
    path = args.root / DEF_PATH
    if args.check:
        if not path.is_file() or path.read_text(encoding="utf-8") != content:
            print(f"{DEF_PATH} differs from the legacy sources", file=sys.stderr)
            return 1
        return 0
    if path.exists() and not args.force:
        print(f"{DEF_PATH} exists and is maintained by hand. Use --force to overwrite it", file=sys.stderr)
        return 1
    path.write_text(content, encoding="utf-8")
    return 0


if __name__ == "__main__":
    sys.exit(main())

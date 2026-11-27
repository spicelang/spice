"""Generator of the diagnostic catalog (media/specs/compiler-reimplementation.md, section 5.3 and 6.6).

Reads src-next/diag/diagnostics.def and writes src-next/diag/codes.spice: one enum per family, the prefix of every code, the
number of codes, and a unit test per family that visits every code.
"""
from __future__ import annotations

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from generate import header  # noqa: E402

NAME = "diagnostics"
DEF_PATH = "src-next/diag/diagnostics.def"
OUT_PATH = "src-next/diag/codes.spice"
GENERATOR = "tools/gen/gen_diagnostics.py"


def parse(text: str) -> list[tuple[str, str, list[tuple[str, str]]]]:
    """Returns [(family, enum name, [(member, prefix)])] in file order. Raises SystemExit on a malformed definition."""
    families: list[tuple[str, str, list[tuple[str, str]]]] = []
    for number, raw in enumerate(text.split("\n"), 1):
        line = raw.strip()
        if not line or line.startswith("#"):
            continue
        where = f"{DEF_PATH}:{number}"
        if line.startswith("["):
            if not line.endswith("]") or not line[1:-1].isidentifier():
                raise SystemExit(f"error: {where}: malformed family '{line}'")
            if any(f[0] == line[1:-1] for f in families):
                raise SystemExit(f"error: {where}: family '{line[1:-1]}' is defined twice")
            families.append((line[1:-1], "", []))
            continue
        if not families:
            raise SystemExit(f"error: {where}: entry before the first family")
        key, sep, value = line.partition(" = ")
        if not sep or not value:
            raise SystemExit(f"error: {where}: expected '<key> = <value>'")
        family, enum, members = families[-1]
        if key == "enum":
            if not value.isidentifier() or enum:
                raise SystemExit(f"error: {where}: invalid or repeated enum name")
            families[-1] = (family, value, members)
            continue
        if not enum:
            raise SystemExit(f"error: {where}: member before 'enum = ...'")
        if not key.replace("_", "").isalnum() or key != key.upper():
            raise SystemExit(f"error: {where}: member '{key}' is not UPPER_SNAKE_CASE")
        if any(m == key for m, _ in members):
            raise SystemExit(f"error: {where}: member '{key}' is defined twice")
        if '"' in value or "\\" in value:
            raise SystemExit(f"error: {where}: a prefix must not contain quotes or backslashes")
        members.append((key, value))
    for family, enum, members in families:
        if not enum or not members:
            raise SystemExit(f"error: {DEF_PATH}: family '{family}' has no enum name or no members")
    return families


def emit_family(family: str, enum: str, members: list[tuple[str, str]]) -> list[str]:
    out = [f"// {family}: {len(members)} code(s)", f"public type {enum} enum {{"]
    out += [f"    {member}{',' if i < len(members) - 1 else ''}" for i, (member, _) in enumerate(members)]
    out += [
        "}",
        "",
        f"// Number of codes of {enum}",
        f"public f<unsigned int> get{enum}Count() {{",
        f"    return {len(members)}u;",
        "}",
        "",
        f"// Text before the message of a diagnostic with a code of {enum}",
        f"public f<string> get{enum}Prefix(const {enum} code) {{",
        "    switch code {",
    ]
    out += [f'        case {enum}::{member}: {{ return "{prefix}"; }}' for member, prefix in members]
    out += [
        "        default: { panic(Error(\"Unknown diagnostic code\")); }",
        "    }",
        "}",
        "",
        "#[test]",
        f"f<bool> test{enum}Codes() {{",
        f"    assert get{enum}Count() == {len(members)}u;",
        f"    for unsigned int i = 0u; i < get{enum}Count(); i++ {{",
        f"        assert len(get{enum}Prefix(cast<{enum}>(i))) > 0l;",
        "    }",
        f'    assert String(get{enum}Prefix({enum}::{members[0][0]})) == "{members[0][1]}";',
        f'    assert String(get{enum}Prefix({enum}::{members[-1][0]})) == "{members[-1][1]}";',
        "    return true;",
        "}",
        "",
    ]
    return out


def generate(root: Path) -> dict[str, str]:
    families = parse((root / DEF_PATH).read_text(encoding="utf-8"))
    lines = [
        header(GENERATOR, DEF_PATH).rstrip("\n"),
        "// Copyright (c) 2021-2026 ChilliBits. All rights reserved.",
        "",
        "// The codes of a family are numbered from 0 in the order of the definition, like the legacy enums",
        "",
    ]
    for family, enum, members in families:
        lines += emit_family(family, enum, members)
    return {OUT_PATH: "\n".join(lines).rstrip("\n") + "\n"}

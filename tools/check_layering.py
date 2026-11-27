#!/usr/bin/env python3
"""Layering check for the compiler re-implementation (media/specs/compiler-reimplementation.md, section 4.3).

Checks every `import "@/..."` line of the .spice files below a source root (default: src-next) against the table of allowed
dependencies. A violation fails the run (exit code 1). The size budgets of section 10.2 (files up to ~1,200 lines, functions
up to ~150 lines) only produce warnings, unless --strict-budgets is given.

Components are the directories below the source root. `sema`, `codegen` and `tools` are split one level further
(`sema/types`, `codegen/llvm`, `tools/format`), all other layers are one component. Files directly in the source root
(`main.spice`) are part of the driver layer and may import everything.

Decisions for points, that section 4.3 leaves open (keep in sync with the plan):
  * Components of the same tier of the sema order (`collect`/`resolve`, `ops`/`overload`/`instantiate`/`consteval`,
    `members`/`lifetime`) may import each other.
  * `codegen/*` components may import each other.
  * `tools/format` may import `syntax` and the layers below it. `tools/lint` and `tools/viz` may additionally import
    `modules` and the read-only sema views `sema/types`, `sema/model`, `sema/names` and `sema/sir`.
  * Imports of `@/spice_packages/...` (Spice Shaker packages) count like std imports.
  * The façade rule (imports across sub-components of a layer go through the façade) is not checked.

Examples:
  python tools/check_layering.py src-next
  python tools/check_layering.py src-next --strict-budgets
"""
from __future__ import annotations

import argparse
import re
import sys
from dataclasses import dataclass
from pathlib import Path

sys.dont_write_bytecode = True

SPLIT_LAYERS = {"sema", "codegen", "tools"}
EXTERNAL_DIRS = {"spice_packages"}
ROOT_COMPONENT = "driver"
MAX_FILE_LINES = 1200
MAX_FUNCTION_LINES = 150

# Layers, from the lowest to the highest
LAYER_ORDER = ["base", "source", "diag", "syntax", "modules", "sema", "codegen", "link", "tools", "driver"]
BASE_LAYERS = {"base", "source", "diag"}

SEMA_ORDER = [
    ["types"],
    ["model"],
    ["names"],
    ["collect", "resolve"],
    ["ops", "overload", "instantiate", "consteval"],
    ["members", "lifetime"],
    ["check"],
    ["lower"],
]
SEMA_TIER = {name: tier for tier, names in enumerate(SEMA_ORDER) for name in names}
SEMA_ALL = {f"sema/{name}" for name in [*SEMA_TIER, "sir", "dump", "warnings"]}


def allowed_imports(component: str) -> set[str] | None:
    """Components a component may import (besides itself). None means everything."""
    if component == "base":
        return set()
    if component == "source":
        return {"base"}
    if component == "diag":
        return {"base", "source"}
    if component == "syntax":
        return set(BASE_LAYERS)
    if component == "modules":
        return {*BASE_LAYERS, "syntax"}
    if component.startswith("sema/"):
        return sema_imports(component.split("/", 1)[1])
    if component.startswith("codegen/"):
        return {*BASE_LAYERS, "syntax", "modules", "sema/types", "sema/model", "sema/sir"} | {
            c for c in KNOWN_COMPONENTS if c.startswith("codegen/")
        }
    if component == "link":
        return {"base", "diag", "codegen/target"}
    if component == "tools/format":
        return {*BASE_LAYERS, "syntax"}
    if component in ("tools/lint", "tools/viz"):
        return {*BASE_LAYERS, "syntax", "modules", "sema/types", "sema/model", "sema/names", "sema/sir"}
    if component == "driver":
        return None
    return set()


def sema_imports(name: str) -> set[str]:
    lower = {*BASE_LAYERS, "syntax", "modules"}
    if name in ("dump", "warnings"):
        return lower | SEMA_ALL
    if name == "sir":
        return lower | {"sema/types", "sema/model"}
    tier = SEMA_TIER[name]
    allowed = lower | {f"sema/{other}" for other, t in SEMA_TIER.items() if t <= tier}
    if tier >= SEMA_TIER["check"]:  # sir is used from check onwards
        allowed.add("sema/sir")
    return allowed


KNOWN_COMPONENTS = {
    *LAYER_ORDER,
    *SEMA_ALL,
    *(f"codegen/{n}" for n in ["target", "abi", "mangle", "llvm", "opt", "emit"]),
    *(f"tools/{n}" for n in ["format", "lint", "viz"]),
} - {"sema", "codegen", "tools"}

# An import statement may be split over several lines
IMPORT_REGEX = re.compile(r'^[ \t]*import\s*"([^"\n]+)"', re.MULTILINE)
MODIFIERS = r"(?:[a-z]+\s+)*"
FUNCTION_START = re.compile(rf"^[ \t]*{MODIFIERS}(?:(f)\s*<|p[ \t])", re.MULTILINE)
# What follows the return type: the name of the function, e.g. 'Vector<T>.pushBack(' (a function type field has none)
NAME_AND_PARAMS = re.compile(r"\s*(?:[A-Za-z_][^(){};=]*\.)?operator\s*[^\w\s()]+\(|\s*[A-Za-z_][^(){};=]*\(")


@dataclass(frozen=True)
class Finding:
    path: Path
    line: int
    message: str

    def __str__(self) -> str:
        return f"{self.path}:{self.line}: {self.message}"


def component_of(rel_path: Path) -> str | None:
    """Component of a path relative to the source root, None for external code (std, packages)."""
    parts = rel_path.parts
    if len(parts) <= 1:
        return ROOT_COMPONENT
    if parts[0] in EXTERNAL_DIRS:
        return None
    if parts[0] in SPLIT_LAYERS:
        return f"{parts[0]}/{parts[1]}" if len(parts) > 2 else parts[0]
    return parts[0]


def resolve_import(target: str, file_rel: Path) -> tuple[Path | None, bool]:
    """Returns the path of the imported file relative to the source root and whether it is a source import. Std imports are not."""
    if target.startswith("@/"):
        return Path(target[2:]), True
    if target.startswith("./") or target.startswith("../"):
        resolved = (file_rel.parent / target).as_posix()
        norm: list[str] = []
        for part in resolved.split("/"):
            if part == "..":
                if not norm:
                    return None, True
                norm.pop()
            elif part not in ("", "."):
                norm.append(part)
        return Path(*norm), True
    return None, False


def check_imports(root: Path, files: list[Path]) -> list[Finding]:
    findings: list[Finding] = []
    for file in files:
        rel = file.relative_to(root)
        source = component_of(rel)
        if source is None:
            continue
        if source not in KNOWN_COMPONENTS:
            findings.append(Finding(file, 1, f"'{rel.as_posix()}' is not part of a known component (section 4.3)"))
            continue
        allowed = allowed_imports(source)
        text = blank_comments_and_literals(file.read_text(encoding="utf-8"), keep_literals=True)
        for match in IMPORT_REGEX.finditer(text):
            line_no = text.count("\n", 0, match.start()) + 1
            target, is_source = resolve_import(match.group(1), rel)
            if not is_source:  # std import
                continue
            if target is None:
                findings.append(Finding(file, line_no, f'import "{match.group(1)}" leaves the source root'))
                continue
            dep = component_of(target)
            if dep is None:
                if source == "base":
                    findings.append(Finding(file, line_no, f'base may only import std, not "{match.group(1)}"'))
                continue
            if dep not in KNOWN_COMPONENTS:
                findings.append(Finding(file, line_no, f'import "{match.group(1)}" targets an unknown component'))
                continue
            if dep == source or allowed is None or dep in allowed:
                continue
            findings.append(Finding(file, line_no, f"{source} must not import {dep}: import \"{match.group(1)}\""))
    return findings


def blank_comments_and_literals(text: str, keep_literals: bool = False) -> str:
    """Replaces comments and, unless keep_literals is set, the content of string and char literals by spaces. Newlines stay,
    so line numbers are kept."""
    out: list[str] = []
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("//", i):
            end = text.find("\n", i)
            end = n if end == -1 else end
            out.append(" " * (end - i))
            i = end
        elif text.startswith("/*", i):
            end = text.find("*/", i + 2)
            end = n if end == -1 else end + 2
            out.append("".join(ch if ch == "\n" else " " for ch in text[i:end]))
            i = end
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
            if keep_literals:
                out.append(text[i:j])
            else:
                out.append(c + "".join(ch if ch == "\n" else " " for ch in text[i + 1 : j - 1]) + (c if j - 1 > i else ""))
            i = j
        else:
            out.append(c)
            i += 1
    return "".join(out)


def function_lengths(text: str) -> list[tuple[int, int, str]]:
    """Returns (first line, line count, header) of every function, procedure and method with a body."""
    clean = blank_comments_and_literals(text)
    result = []
    pos = 0
    for match in FUNCTION_START.finditer(clean):
        if match.start() < pos:  # inside the body of the previous match
            continue
        i = match.end()
        if match.group(1):  # Skip the return type f<...>, it may contain nested generics
            angle = 1
            while i < len(clean) and angle:
                angle += {"<": 1, ">": -1}.get(clean[i], 0)
                i += 1
        if not NAME_AND_PARAMS.match(clean, i):  # No named declaration, e.g. a function type field or a lambda
            continue
        depth = 0
        while i < len(clean):
            c = clean[i]
            if c in "(":
                depth += 1
            elif c == ")":
                depth -= 1
            elif depth <= 0 and c in "{;":
                break
            i += 1
        if i >= len(clean) or clean[i] == ";":
            continue
        brace = 0
        j = i
        while j < len(clean):
            if clean[j] == "{":
                brace += 1
            elif clean[j] == "}":
                brace -= 1
                if brace == 0:
                    break
            j += 1
        first = clean.count("\n", 0, match.start()) + 1
        last = clean.count("\n", 0, j) + 1
        header = " ".join(clean[match.start() : i].split())
        result.append((first, last - first + 1, header))
        pos = j
    return result


def check_budgets(files: list[Path]) -> list[Finding]:
    findings: list[Finding] = []
    for file in files:
        text = file.read_text(encoding="utf-8")
        lines = text.count("\n") + (0 if text.endswith("\n") or not text else 1)
        if lines > MAX_FILE_LINES:
            findings.append(Finding(file, 1, f"file has {lines} lines (budget ~{MAX_FILE_LINES})"))
        for first, length, header in function_lengths(text):
            if length > MAX_FUNCTION_LINES:
                findings.append(Finding(file, first, f"'{header}' has {length} lines (budget ~{MAX_FUNCTION_LINES})"))
    return findings


def collect_files(root: Path) -> list[Path]:
    return sorted(p for p in root.rglob("*.spice") if not (EXTERNAL_DIRS & set(p.relative_to(root).parts)))


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Check the layering and size budgets of the compiler re-implementation")
    parser.add_argument("root", nargs="?", default="src-next", help="source root (default: src-next)")
    parser.add_argument("--strict-budgets", action="store_true", help="fail on size budget warnings, too")
    args = parser.parse_args(argv)

    root = Path(args.root)
    if not root.is_dir():
        print(f"Source root '{root}' not found", file=sys.stderr)
        return 2
    files = collect_files(root)
    violations = check_imports(root, files)
    warnings = check_budgets(files)

    for finding in violations:
        print(f"error: {finding}")
    for finding in warnings:
        print(f"warning: {finding}")
    print(f"{len(files)} files checked: {len(violations)} layering violations, {len(warnings)} budget warnings")
    return 1 if violations or (warnings and args.strict_budgets) else 0


if __name__ == "__main__":
    sys.exit(main())

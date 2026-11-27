#!/usr/bin/env python3
"""Classify the differences between two LLVM IR (or assembly) texts for triage.

The differential harness (tools/diff_compilers.py) compares the output of the legacy compiler and the compiler
re-implementation. The gate for the IR stays byte-identical (media/specs/compiler-reimplementation.md, section 8.3). This
module only tells *what kind* of difference there is, so that an agent can work through the differences class by class:

  identical   the texts are equal
  order       only the order of the functions, declarations, globals or metadata nodes differs
  names       only the names of local values and basic blocks differ
  metadata    only the numbering of metadata nodes (!N) and attribute groups (#N) differs
  structural  anything else

A text may differ in several of the first three ways at once. classify() returns all applicable classes then (in the order
above) if the text is equal after applying exactly these normalizations, and 'structural' otherwise.

Usage: ir_normalize.py <a.ll> <b.ll>   Prints the classes and the first differing lines.
"""
from __future__ import annotations

import argparse
import difflib
import re
import sys
from pathlib import Path

IDENTICAL = "identical"
ORDER = "order"
NAMES = "names"
METADATA = "metadata"
STRUCTURAL = "structural"
CLASSES = [IDENTICAL, ORDER, NAMES, METADATA, STRUCTURAL]

# Local values and labels: '%name', '%0', '%"quoted name"'. Global values start with '@' and are not renamed
LOCAL_REGEX = re.compile(r'%(?:"[^"]*"|[-a-zA-Z$._0-9]+)')
# Labels at the beginning of a line ('entry:', 'cond.true.L1C2:') and in the comment of the LLVM printer ('; preds = ...')
LABEL_DEF_REGEX = re.compile(r'^(?P<name>"[^"]*"|[-a-zA-Z$._0-9]+):(?P<rest>.*)$')
PREDS_COMMENT_REGEX = re.compile(r";\s*preds = .*$")
METADATA_REGEX = re.compile(r"(?<![\w.$-])!(\d+)\b")
ATTR_GROUP_REGEX = re.compile(r"(?<![\w.$-])#(\d+)\b")
# Local names of type definitions ('%struct.Vec') are global in LLVM. They must be kept, so they are collected up front
TYPE_DEF_REGEX = re.compile(r'^(%(?:"[^"]*"|[-a-zA-Z$._0-9]+)) = type\b')


def split_entities(text: str) -> list[str]:
    """Split a module into its top-level entities: a function definition with its body, or any other single line"""
    entities: list[str] = []
    body: list[str] | None = None
    for line in text.split("\n"):
        if body is not None:
            body.append(line)
            if line == "}":
                entities.append("\n".join(body))
                body = None
        elif line.startswith("define ") and line.rstrip().endswith("{"):
            body = [line]
        elif line.strip():
            entities.append(line)
    if body is not None:  # Truncated function: keep it, so the comparison still reports it
        entities.append("\n".join(body))
    return entities


def sort_entities(text: str) -> str:
    """Sort the top-level entities, so only their order is erased. Comment lines are dropped, since LLVM only places them
    relative to the entities"""
    entities = [e for e in split_entities(text) if not e.startswith(";")]
    return "\n".join(sorted(entities)) + "\n"


def _rename_locals(function: str, keep: set[str]) -> str:
    mapping: dict[str, str] = {}

    def rename(match: re.Match) -> str:
        name = match.group(0)
        if name in keep:
            return name
        if name not in mapping:
            mapping[name] = f"%v{len(mapping)}"
        return mapping[name]

    lines = function.split("\n")
    result = []
    for line in lines:
        if line.startswith("define "):
            result.append(LOCAL_REGEX.sub(rename, line))
            continue
        label = LABEL_DEF_REGEX.match(line)
        if label and not line.startswith((" ", "\t")):
            name = "%" + label.group("name")
            if name not in mapping:
                mapping[name] = f"%v{len(mapping)}"
            rest = PREDS_COMMENT_REGEX.sub("; preds", label.group("rest"))
            result.append(mapping[name][1:] + ":" + rest)
            continue
        result.append(LOCAL_REGEX.sub(rename, line))
    return "\n".join(result)


def erase_names(text: str) -> str:
    """Rename the local values and labels of each function by order of first occurrence"""
    keep = {m.group(1) for m in map(TYPE_DEF_REGEX.match, text.split("\n")) if m}
    return "\n".join(_rename_locals(e, keep) if e.startswith("define ") else e for e in split_entities(text)) + "\n"


def _renumber(text: str, regex: re.Pattern, prefix: str) -> str:
    mapping: dict[str, str] = {}

    def rename(match: re.Match) -> str:
        number = match.group(1)
        if number not in mapping:
            mapping[number] = str(len(mapping))
        return prefix + mapping[number]

    return regex.sub(rename, text)


def erase_metadata_numbering(text: str) -> str:
    """Renumber the metadata nodes (!N) and attribute groups (#N) by order of first occurrence"""
    return _renumber(_renumber(text, METADATA_REGEX, "!"), ATTR_GROUP_REGEX, "#")


def _normalizations(a_class_flags: tuple[bool, bool, bool]):
    order, names, metadata = a_class_flags

    def apply(text: str) -> str:
        if names:
            text = erase_names(text)
        # Sort first: the order of the functions decides which metadata node is numbered first
        if order:
            text = sort_entities(text)
        if metadata:
            text = erase_metadata_numbering(text)
        # Always strip the trailing whitespace of the file, it carries no information
        return text.rstrip("\n") + "\n"
    return apply


def classify(a: str, b: str) -> list[str]:
    """Classify the difference between the texts a and b. Returns a list of classes, see the module doc"""
    if a == b:
        return [IDENTICAL]
    # Try the combinations from the fewest to the most normalizations, the first match names the classes
    combinations = sorted(((o, n, m) for o in (False, True) for n in (False, True) for m in (False, True)
                           if o or n or m), key=lambda c: (sum(c), c[::-1]))
    for flags in combinations:
        apply = _normalizations(flags)
        if apply(a) == apply(b):
            return [name for name, on in zip((ORDER, NAMES, METADATA), flags) if on]
    return [STRUCTURAL]


def first_diff_lines(a: str, b: str, limit: int = 8, context: int = 0) -> list[str]:
    """The first differing lines as a unified diff body, at most 'limit' lines"""
    diff = difflib.unified_diff(a.split("\n"), b.split("\n"), "legacy", "next", n=context, lineterm="")
    # Escape undecodable bytes (surrogates), so printing to a strict UTF-8 stdout works. The comparison stays unchanged
    return [line.encode("utf-8", "backslashreplace").decode("utf-8") for line in list(diff)[:limit + 2]]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("a", type=Path)
    parser.add_argument("b", type=Path)
    parser.add_argument("--limit", type=int, default=8, help="Max. number of differing lines to print")
    args = parser.parse_args()
    a, b = (f.read_bytes().decode("utf-8", errors="surrogateescape") for f in (args.a, args.b))
    classes = classify(a, b)
    print(" + ".join(classes))
    if classes != [IDENTICAL]:
        print("\n".join(first_diff_lines(a, b, args.limit)))
    return 0 if classes == [IDENTICAL] else 1


if __name__ == "__main__":
    sys.exit(main())

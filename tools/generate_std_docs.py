#!/usr/bin/env python3
"""Generate Zensical/Markdown API documentation for the Spice standard library.

This is a standalone parser: it does not need the Spice compiler. It reads every
``std/**/*.spice`` file, pairs each ``/** ... */`` doc comment with the public
declaration that follows it, and emits one Markdown page per module (grouped by
std subdirectory), an index page, and a ``nav`` snippet for ``zensical.toml``.

Only the ``public`` API surface is documented. Methods (``Type.method``) are
grouped under the struct they belong to.

Usage:
    python tools/generate_std_docs.py [--std-dir std] [--out-dir docs/docs/std]
                                      [--doc-root docs/docs] [--exclude bindings]
                                      [--nav-out -]
"""

from __future__ import annotations

import argparse
import re
import shutil
import sys
from dataclasses import dataclass, field
from pathlib import Path

# Qualifiers that may precede a declaration keyword (f / p / type)
QUALIFIERS = {"public", "inline", "const", "signed", "unsigned", "heap", "composition"}
# Qualifiers that may precede a struct field; only "public" affects visibility,
# the others (signed/unsigned/heap/const/...) are part of the field type.
FIELD_QUALIFIERS = {"public", "const", "signed", "unsigned", "heap", "composition"}


# --------------------------------------------------------------------------- #
# Low-level scanning helpers
# --------------------------------------------------------------------------- #
def _skip_string(text: str, i: int) -> int:
    """Skip a string or char literal starting at the opening quote ``text[i]``."""
    quote = text[i]
    i += 1
    n = len(text)
    while i < n:
        if text[i] == "\\":
            i += 2
            continue
        if text[i] == quote:
            return i + 1
        i += 1
    return i


def _skip_line_comment(text: str, i: int) -> int:
    end = text.find("\n", i)
    return len(text) if end == -1 else end + 1


def _skip_block_comment(text: str, i: int) -> int:
    end = text.find("*/", i + 2)
    return len(text) if end == -1 else end + 2


def _read_braced(text: str, i: int) -> tuple[str, int]:
    """``text[i]`` is ``{``. Return (inner body, index right after the matching ``}``)."""
    depth = 0
    n = len(text)
    start = i
    while i < n:
        c = text[i]
        if c in "\"'":
            i = _skip_string(text, i)
            continue
        if text.startswith("//", i):
            i = _skip_line_comment(text, i)
            continue
        if text.startswith("/*", i):
            i = _skip_block_comment(text, i)
            continue
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return text[start + 1 : i], i + 1
        i += 1
    return text[start + 1 :], n


def _scan_header(text: str, i: int) -> tuple[int, str | None]:
    """Scan a declaration header from ``i``; stop at the first top-level ``{`` or ``;``."""
    paren = bracket = 0
    n = len(text)
    while i < n:
        c = text[i]
        if c in "\"'":
            i = _skip_string(text, i)
            continue
        if text.startswith("//", i):
            i = _skip_line_comment(text, i)
            continue
        if text.startswith("/*", i):
            i = _skip_block_comment(text, i)
            continue
        if c == "(":
            paren += 1
        elif c == ")":
            paren -= 1
        elif c == "[":
            bracket += 1
        elif c == "]":
            bracket -= 1
        elif paren == 0 and bracket == 0:
            if c == "{":
                return i, "{"
            if c == ";":
                return i, ";"
        i += 1
    return n, None


@dataclass
class RawItem:
    doc: str
    header: str
    body: str | None
    offset: int


def scan_definitions(text: str) -> list[RawItem]:
    """Split source into top-level definitions, each with its preceding doc comment."""
    items: list[RawItem] = []
    i, n = 0, len(text)
    pending_doc = ""
    while i < n:
        c = text[i]
        if c in " \t\r\n":
            i += 1
            continue
        if text.startswith("/**", i):
            end = _skip_block_comment(text, i)
            pending_doc = text[i:end]
            i = end
            continue
        if text.startswith("/*", i):  # ordinary block comment: treat as whitespace
            i = _skip_block_comment(text, i)
            continue
        if text.startswith("//", i):
            i = _skip_line_comment(text, i)
            continue
        if text.startswith("#!", i):
            # Module attribute (#![ ... ]): standalone, not attached to a definition. A
            # definition attribute (#[ ... ]) is left in place so it prefixes the next decl.
            lb = text.find("[", i)
            if lb == -1:
                i = _skip_line_comment(text, i)
                continue
            depth, j = 0, lb
            while j < n:
                if text[j] == "[":
                    depth += 1
                elif text[j] == "]":
                    depth -= 1
                    if depth == 0:
                        j += 1
                        break
                j += 1
            i = j
            continue
        # Start of a declaration
        start = i
        end, term = _scan_header(text, i)
        header = text[start:end].strip()
        body = None
        if term == "{":
            body, i = _read_braced(text, end)
        elif term == ";":
            i = end + 1
        else:
            i = end
        items.append(RawItem(pending_doc, header, body, start))
        pending_doc = ""
    return items


# --------------------------------------------------------------------------- #
# Doc comment parsing
# --------------------------------------------------------------------------- #
def parse_doc(raw: str) -> tuple[str, dict[str, str], str]:
    """Parse a raw ``/** ... */`` comment into (description, params, return-desc)."""
    if not raw:
        return "", {}, ""
    body = raw.strip()
    if body.startswith("/**"):
        body = body[3:]
    elif body.startswith("/*"):
        body = body[2:]
    if body.endswith("*/"):
        body = body[:-2]

    desc: list[str] = []
    params: dict[str, str] = {}
    ret = ""
    target = ("desc", "")
    for raw_line in body.splitlines():
        line = raw_line.strip()
        if line.startswith("*"):
            line = line[1:]
            if line.startswith(" "):
                line = line[1:]
        content = line.strip()

        mp = re.match(r"@param\s+(\S+)\s*(.*)$", content)
        mr = re.match(r"@return\s*(.*)$", content)
        if mp:
            params[mp.group(1)] = mp.group(2).strip()
            target = ("param", mp.group(1))
            continue
        if mr:
            ret = mr.group(1).strip()
            target = ("return", "")
            continue

        kind, key = target
        if kind == "desc":
            desc.append(line)
        elif kind == "param" and content:
            params[key] = (params[key] + " " + content).strip()
        elif kind == "return" and content:
            ret = (ret + " " + content).strip()

    while desc and not desc[0].strip():
        desc.pop(0)
    while desc and not desc[-1].strip():
        desc.pop()
    return "\n".join(desc), params, ret


def render_description(desc: str) -> str:
    """Render a doc description to Markdown, preserving list-like blocks (lines ending ':')."""
    if not desc.strip():
        return ""
    paragraphs = re.split(r"\n\s*\n", desc.strip())
    rendered: list[str] = []
    for para in paragraphs:
        lines = [ln.strip() for ln in para.splitlines() if ln.strip()]
        if not lines:
            continue
        # If a line introduces a block (ends with ':'), keep hard line breaks
        if any(ln.endswith(":") for ln in lines[:-1]):
            rendered.append("  \n".join(lines))
        else:
            rendered.append(" ".join(lines))
    return "\n\n".join(rendered)


# --------------------------------------------------------------------------- #
# Declaration parsing
# --------------------------------------------------------------------------- #
def strip_attributes(header: str) -> str:
    """Remove leading ``#[...]`` / ``#![...]`` attributes that may precede a declaration."""
    s = header.lstrip()
    while s.startswith("#"):
        m = re.match(r"#!?\[", s)
        if not m:
            break
        depth = 0
        i = m.end() - 1  # position of the opening '['
        while i < len(s):
            if s[i] == "[":
                depth += 1
            elif s[i] == "]":
                depth -= 1
                if depth == 0:
                    i += 1
                    break
            i += 1
        s = s[i:].lstrip()
    return s


def strip_qualifiers(header: str) -> tuple[list[str], str]:
    quals: list[str] = []
    s = header
    while True:
        m = re.match(r"^([A-Za-z_]\w*)\s+", s)
        if m and m.group(1) in QUALIFIERS:
            quals.append(m.group(1))
            s = s[m.end() :]
        else:
            break
    return quals, s


def _extract_angle(s: str, pos: int) -> tuple[str, int]:
    """``s[pos] == '<'``. Return (inner, index after the matching ``>``), handling nesting."""
    depth = 0
    for i in range(pos, len(s)):
        if s[i] == "<":
            depth += 1
        elif s[i] == ">":
            depth -= 1
            if depth == 0:
                return s[pos + 1 : i], i + 1
    return s[pos + 1 :], len(s)


def _read_parens(s: str, i: int) -> tuple[str, int]:
    depth = 0
    start = i
    while i < len(s):
        if s[i] in "\"'":
            i = _skip_string(s, i)
            continue
        if s[i] == "(":
            depth += 1
        elif s[i] == ")":
            depth -= 1
            if depth == 0:
                return s[start + 1 : i], i + 1
        i += 1
    return s[start + 1 :], len(s)


def _split_top_level(s: str, sep: str) -> list[str]:
    """Split ``s`` on ``sep`` ignoring separators nested in <>, (), []."""
    parts: list[str] = []
    depth = 0
    cur = []
    i = 0
    while i < len(s):
        c = s[i]
        if c in "\"'":
            j = _skip_string(s, i)
            cur.append(s[i:j])
            i = j
            continue
        if c in "<([":
            depth += 1
        elif c in ">)]":
            depth -= 1
        if c == sep and depth == 0:
            parts.append("".join(cur))
            cur = []
        else:
            cur.append(c)
        i += 1
    parts.append("".join(cur))
    return parts


@dataclass
class Param:
    name: str
    type: str
    default: str | None
    desc: str = ""


def parse_params(params_str: str, doc_params: dict[str, str]) -> list[Param]:
    out: list[Param] = []
    for raw in _split_top_level(params_str, ","):
        piece = raw.strip()
        if not piece:
            continue
        default = None
        eq = _find_assignment(piece)
        if eq is not None:
            decl, default = piece[:eq].strip(), piece[eq + 1 :].strip()
        else:
            decl = piece
        m = re.match(r"^(?P<type>.*?)(?P<name>[A-Za-z_]\w*)\s*$", decl, re.DOTALL)
        if m:
            ptype, pname = m.group("type").strip(), m.group("name")
        else:
            ptype, pname = decl, ""
        out.append(Param(pname, ptype, default, doc_params.get(pname, "")))
    return out


def _find_assignment(s: str) -> int | None:
    """Index of the parameter-default ``=`` at top level (not ==, <=, >=, !=, ...)."""
    depth = 0
    i = 0
    while i < len(s):
        c = s[i]
        if c in "\"'":
            i = _skip_string(s, i)
            continue
        if c in "<([":
            depth += 1
        elif c in ">)]":
            depth -= 1
        elif c == "=" and depth == 0:
            prev = s[i - 1] if i > 0 else ""
            nxt = s[i + 1] if i + 1 < len(s) else ""
            if nxt != "=" and prev not in "<>=!+-*/%&|^":
                return i
        i += 1
    return None


@dataclass
class Callable:
    kind: str          # "function" | "procedure"
    signature: str     # verbatim, whitespace-normalised
    struct: str | None
    name: str
    is_operator: bool
    params: list[Param]
    ret_type: str | None
    ret_desc: str
    desc: str
    line: int


def parse_callable(header: str, kind: str, doc: str, line: int) -> Callable:
    description, doc_params, ret_desc = parse_doc(doc)
    quals, s = strip_qualifiers(header)

    ret_type = None
    if kind == "function":
        lt = s.find("<")
        ret_type, after = _extract_angle(s, lt)
        rest = s[after:].strip()
    else:
        rest = s[1:].strip()  # drop leading 'p'

    # Split "name[<template>]" from "(params)"
    name_part, params_str = _split_name_and_params(rest)
    struct, name, is_operator = _parse_name(name_part)

    params = parse_params(params_str, doc_params)
    signature = " ".join(header.split())
    return Callable(kind, signature, struct, name, is_operator, params, ret_type, ret_desc, description, line)


def _split_name_and_params(s: str) -> tuple[str, str]:
    depth = 0
    i = 0
    while i < len(s):
        c = s[i]
        if c == "<":
            depth += 1
        elif c == ">":
            depth -= 1
        elif c == "(" and depth == 0:
            params, _ = _read_parens(s, i)
            return s[:i].strip(), params
        i += 1
    return s.strip(), ""


def _parse_name(name_part: str) -> tuple[str | None, str, bool]:
    lt = name_part.find("<")
    base = name_part[:lt].strip() if lt != -1 else name_part.strip()
    if base.startswith("operator"):
        return None, base, True
    if "." in base:
        struct, simple = base.split(".", 1)
        return struct, simple, False
    return None, base, False


@dataclass
class Field:
    name: str
    type: str
    desc: str


@dataclass
class Struct:
    name: str
    generics: str
    implements: str
    desc: str
    fields: list[Field]
    line: int
    ctors: list[Callable] = field(default_factory=list)
    methods: list[Callable] = field(default_factory=list)


def parse_struct_fields(body: str) -> list[Field]:
    fields: list[Field] = []
    for raw_line in body.splitlines():
        line = raw_line.strip()
        if not line or line.startswith("//") or line.startswith("/*") or line.startswith("*"):
            continue
        # Separate a trailing line comment, used as the field description
        desc = ""
        cpos = line.find("//")
        if cpos != -1:
            desc = line[cpos + 2 :].strip()
            line = line[:cpos].strip()
        if not line:
            continue
        # Drop a trailing default value
        eq = _find_assignment(line)
        if eq is not None:
            line = line[:eq].strip()
        # Only public fields are part of the documented API; private fields are
        # implementation details and are left out.
        tokens = line.split()
        leading = []
        for tok in tokens:
            if tok in FIELD_QUALIFIERS:
                leading.append(tok)
            else:
                break
        if "public" not in leading:
            continue
        line = " ".join(t for t in tokens if t != "public")  # keep type qualifiers, drop visibility
        m = re.match(r"^(?P<type>.*?)(?P<name>[A-Za-z_]\w*)\s*$", line, re.DOTALL)
        if not m:
            continue
        fields.append(Field(m.group("name"), m.group("type").strip(), desc))
    return fields


@dataclass
class Interface:
    name: str
    generics: str
    desc: str
    methods: list[Callable]
    line: int


@dataclass
class EnumItem:
    name: str
    value: str | None
    desc: str


@dataclass
class Enum:
    name: str
    desc: str
    items: list[EnumItem]
    line: int


def parse_enum_items(body: str) -> list[EnumItem]:
    items: list[EnumItem] = []
    for raw in _split_top_level(body, ","):
        desc = ""
        cpos = raw.find("//")
        if cpos != -1:
            desc = raw[cpos + 2 :].strip()
            raw = raw[:cpos]
        token = raw.strip()
        if not token:
            continue
        m = re.match(r"^([A-Za-z_]\w*)\s*(?:=\s*(.+))?$", token)
        if not m:
            continue
        items.append(EnumItem(m.group(1), (m.group(2) or "").strip() or None, desc))
    return items


@dataclass
class Alias:
    name: str
    aliased: str
    desc: str
    line: int


@dataclass
class Module:
    import_path: str
    subdir: str
    stem: str
    title: str
    structs: list[Struct] = field(default_factory=list)
    functions: list[Callable] = field(default_factory=list)
    operators: list[Callable] = field(default_factory=list)
    interfaces: list[Interface] = field(default_factory=list)
    enums: list[Enum] = field(default_factory=list)
    aliases: list[Alias] = field(default_factory=list)

    def is_empty(self) -> bool:
        return not (self.structs or self.functions or self.operators or self.interfaces or self.enums or self.aliases)

    def summary(self) -> str:
        candidates = [sym for src in (self.structs, self.interfaces, self.enums, self.functions)
                      for sym in src if sym.desc.strip()]
        if not candidates:
            return ""
        # Prefer the symbol whose name matches the module file name (the "primary" type)
        norm = re.sub(r"[-_]", "", self.stem).lower()
        chosen = next((s for s in candidates if s.name.lower() == norm), candidates[0])
        return re.split(r"(?<=[.!?])\s", chosen.desc.strip().replace("\n", " "), maxsplit=1)[0].strip()


# --------------------------------------------------------------------------- #
# Module parsing
# --------------------------------------------------------------------------- #
def prettify_title(stem: str) -> str:
    words = re.split(r"[-_]", stem)
    return " ".join(w[:1].upper() + w[1:] if w else w for w in words)


# Platform/OS suffix tokens used by the std library for per-platform source variants
# (e.g. socket_linux.spice). Note "_rt" runtime modules are deliberately NOT folded.
PLATFORM_TOKENS = {"darwin", "linux", "windows"}


def base_stem(stem: str) -> str:
    """Strip a trailing platform variant suffix (e.g. socket_linux -> socket, os_windows -> os)."""
    segments = stem.split("_")
    for i, segment in enumerate(segments):
        if segment in PLATFORM_TOKENS:
            return "_".join(segments[:i]) or stem
    return stem


def _merge_callables(dst: list[Callable], src: list[Callable], seen: set[str]) -> None:
    for call in src:
        if call.signature not in seen:
            seen.add(call.signature)
            dst.append(call)


def _merge_struct(dst: Struct, src: Struct) -> None:
    field_names = {f.name for f in dst.fields}
    for fld in src.fields:
        if fld.name not in field_names:
            field_names.add(fld.name)
            dst.fields.append(fld)
    _merge_callables(dst.ctors, src.ctors, {c.signature for c in dst.ctors})
    _merge_callables(dst.methods, src.methods, {c.signature for c in dst.methods})


def merge_modules(parts: list[Module], subdir: str, base: str) -> Module:
    """Fold per-platform source variants into a single module, de-duplicating identical symbols."""
    rel = f"{subdir}/{base}" if subdir else base
    merged = Module("std/" + rel, subdir, base, prettify_title(base))
    structs: dict[str, Struct] = {}
    interfaces: dict[str, Interface] = {}
    enum_names: set[str] = set()
    alias_names: set[str] = set()
    fn_sigs: set[str] = set()
    op_sigs: set[str] = set()

    for part in parts:
        for struct in part.structs:
            if struct.name in structs:
                _merge_struct(structs[struct.name], struct)
            else:
                structs[struct.name] = struct
                merged.structs.append(struct)
        for iface in part.interfaces:
            if iface.name in interfaces:
                _merge_callables(interfaces[iface.name].methods, iface.methods,
                                 {m.signature for m in interfaces[iface.name].methods})
            else:
                interfaces[iface.name] = iface
                merged.interfaces.append(iface)
        for enum in part.enums:
            if enum.name not in enum_names:
                enum_names.add(enum.name)
                merged.enums.append(enum)
        for alias in part.aliases:
            if alias.name not in alias_names:
                alias_names.add(alias.name)
                merged.aliases.append(alias)
        _merge_callables(merged.functions, part.functions, fn_sigs)
        _merge_callables(merged.operators, part.operators, op_sigs)

    return merged


def parse_module(path: Path, std_dir: Path) -> Module:
    text = path.read_text(encoding="utf-8")
    rel = path.relative_to(std_dir).with_suffix("")
    import_path = "std/" + rel.as_posix()
    module = Module(import_path, rel.parts[0] if len(rel.parts) > 1 else "", path.stem, prettify_title(path.stem))

    pending_callables: list[Callable] = []
    structs_by_name: dict[str, Struct] = {}

    for item in scan_definitions(text):
        header = strip_attributes(item.header)
        quals, s = strip_qualifiers(header)
        is_public = "public" in quals
        line = text.count("\n", 0, item.offset) + 1

        if re.match(r"^f\s*<", s):
            if is_public:
                pending_callables.append(parse_callable(header, "function", item.doc, line))
        elif re.match(r"^p\s", s):
            if is_public:
                pending_callables.append(parse_callable(header, "procedure", item.doc, line))
        elif re.match(r"^type\b", s):
            if not is_public:
                continue
            if re.search(r"\bstruct\b", s):
                module.structs.append(_make_struct(s, item, line, structs_by_name))
            elif re.search(r"\binterface\b", s):
                module.interfaces.append(_make_interface(s, item, line))
            elif re.search(r"\benum\b", s):
                desc, _, _ = parse_doc(item.doc)
                m = re.match(r"^type\s+(\w+)", s)
                module.enums.append(Enum(m.group(1), desc, parse_enum_items(item.body or ""), line))
            elif re.search(r"\balias\b", s):
                desc, _, _ = parse_doc(item.doc)
                m = re.match(r"^type\s+(\w+)\s+alias\s+(.*)$", s)
                if m:
                    module.aliases.append(Alias(m.group(1), m.group(2).strip(), desc, line))
        # generic type defs, ext decls, imports and globals are intentionally skipped

    # Attach methods to their structs; the rest become free functions / operators
    for call in pending_callables:
        if call.is_operator:
            module.operators.append(call)
        elif call.struct and call.struct in structs_by_name:
            target = structs_by_name[call.struct]
            (target.ctors if call.name in ("ctor", "dtor") else target.methods).append(call)
        else:
            module.functions.append(call)

    return module


def _make_struct(s: str, item: RawItem, line: int, registry: dict[str, Struct]) -> Struct:
    desc, _, _ = parse_doc(item.doc)
    m = re.match(r"^type\s+(?P<name>\w+)\s*(?P<gen><[^{]*?>)?\s*struct\s*(?::\s*(?P<impl>.*))?$", s)
    name = m.group("name") if m else "?"
    generics = (m.group("gen") or "").strip() if m else ""
    implements = (m.group("impl") or "").strip() if m else ""
    struct = Struct(name, generics, implements, desc, parse_struct_fields(item.body or ""), line)
    registry[name] = struct
    return struct


def _make_interface(s: str, item: RawItem, line: int) -> Interface:
    desc, _, _ = parse_doc(item.doc)
    m = re.match(r"^type\s+(?P<name>\w+)\s*(?P<gen><[^{]*?>)?\s*interface", s)
    name = m.group("name") if m else "?"
    generics = (m.group("gen") or "").strip() if m else ""
    methods: list[Callable] = []
    for sig in scan_definitions(item.body or ""):
        sig_header = strip_attributes(sig.header)
        q, ss = strip_qualifiers(sig_header)
        if re.match(r"^f\s*<", ss):
            methods.append(parse_callable(sig_header, "function", sig.doc, line))
        elif re.match(r"^p\s", ss):
            methods.append(parse_callable(sig_header, "procedure", sig.doc, line))
    return Interface(name, generics, desc, methods, line)


# --------------------------------------------------------------------------- #
# Markdown rendering
# --------------------------------------------------------------------------- #
def cell(s: str) -> str:
    return s.replace("|", "\\|").replace("\n", " ").strip()


def badge(kind: str) -> str:
    """A labeled badge (styled via .doc-badge in the docs stylesheet)."""
    return f'<span class="doc-badge doc-badge-{kind}">{kind}</span>'


def render_callable(out: list[str], call: Callable, heading: str) -> None:
    out.append(f"{heading} `{call.name}`\n")
    out.append("```spice\n" + call.signature + "\n```\n")
    if call.desc.strip():
        out.append(render_description(call.desc) + "\n")
    named = [p for p in call.params if p.name and p.name != "this"]
    if named:
        out.append("**Parameters**\n")
        out.append("| Name | Type | Description |")
        out.append("| --- | --- | --- |")
        for p in named:
            default = f" (default: `{cell(p.default)}`)" if p.default else ""
            out.append(f"| `{cell(p.name)}` | `{cell(p.type)}` | {cell(p.desc)}{default} |")
        out.append("")
    if call.ret_type and call.ret_type not in ("", "void"):
        suffix = f" — {cell(call.ret_desc)}" if call.ret_desc else ""
        out.append(f"**Returns:** `{cell(call.ret_type)}`{suffix}\n")
    elif call.ret_desc:
        out.append(f"**Returns:** {cell(call.ret_desc)}\n")


def render_module(module: Module) -> str:
    out: list[str] = []
    out.append("---")
    out.append(f"title: {module.title}")
    out.append("---\n")
    out.append(f"# {module.title}\n")
    out.append("```spice")
    out.append(f'import "{module.import_path}";')
    out.append("```\n")

    for struct in module.structs:
        title = struct.name + struct.generics
        out.append(f"## `{title}` {badge('struct')}\n")
        if struct.implements:
            out.append(f"*Implements: `{struct.implements}`*\n")
        if struct.desc.strip():
            out.append(render_description(struct.desc) + "\n")
        if struct.fields:
            out.append("### Fields\n")
            out.append("| Name | Type | Description |")
            out.append("| --- | --- | --- |")
            for f in struct.fields:
                out.append(f"| `{cell(f.name)}` | `{cell(f.type)}` | {cell(f.desc)} |")
            out.append("")
        if struct.ctors:
            out.append("### Constructors\n")
            for call in struct.ctors:
                render_callable(out, call, "####")
        if struct.methods:
            out.append("### Methods\n")
            for call in struct.methods:
                render_callable(out, call, "####")

    if module.functions:
        out.append("## Functions\n")
        for call in module.functions:
            render_callable(out, call, "###")

    if module.operators:
        out.append("## Operators\n")
        for call in module.operators:
            render_callable(out, call, "###")

    for iface in module.interfaces:
        title = iface.name + iface.generics
        out.append(f"## `{title}` {badge('interface')}\n")
        if iface.desc.strip():
            out.append(render_description(iface.desc) + "\n")
        for call in iface.methods:
            render_callable(out, call, "###")

    for enum in module.enums:
        out.append(f"## `{enum.name}` {badge('enum')}\n")
        if enum.desc.strip():
            out.append(render_description(enum.desc) + "\n")
        out.append("| Item | Value | Description |")
        out.append("| --- | --- | --- |")
        for it in enum.items:
            out.append(f"| `{cell(it.name)}` | {('`' + cell(it.value) + '`') if it.value else ''} | {cell(it.desc)} |")
        out.append("")

    for alias in module.aliases:
        out.append(f"## `{alias.name}` {badge('alias')}\n")
        out.append(f"Alias for `{cell(alias.aliased)}`.\n")
        if alias.desc.strip():
            out.append(render_description(alias.desc) + "\n")

    return "\n".join(out).rstrip() + "\n"


def render_index(modules_by_dir: dict[str, list[Module]]) -> str:
    out: list[str] = ["---", "title: Standard Library", "---\n", "# Standard Library\n"]
    out.append("Auto-generated reference for the Spice standard library. "
               "Each page corresponds to one importable module.\n")
    for subdir in sorted(modules_by_dir):
        out.append(f"## {subdir or 'root'}\n")
        for module in sorted(modules_by_dir[subdir], key=lambda m: m.stem):
            summary = module.summary()
            link = f"[{module.title}]({module.subdir + '/' if module.subdir else ''}{module.stem}.md)"
            out.append(f"- {link} — {summary}" if summary else f"- {link}")
        out.append("")
    return "\n".join(out).rstrip() + "\n"


# Markers delimiting the auto-generated std section inside the zensical.toml nav
NAV_BEGIN = "# >>> BEGIN std library nav (generated by tools/generate_std_docs.py) >>>"
NAV_END = "# <<< END std library nav <<<"


def build_nav_block(modules_by_dir: dict[str, list[Module]], out_dir: Path, doc_root: Path) -> list[str]:
    """Build the TOML nav entries for the Standard Library section.

    Returns the lines of a single ``{ "Standard Library" = [ ... ] },`` entry,
    indented to sit inside the ``nav`` array of ``zensical.toml`` (the entry
    itself at 2-space indentation, matching the surrounding nav entries).
    """
    def navpath(p: Path) -> str:
        return p.relative_to(doc_root).as_posix()

    lines = ['  { "Standard Library" = [']
    lines.append(f'    {{ "Overview" = "{navpath(out_dir / "index.md")}" }},')
    for subdir in sorted(modules_by_dir):
        modules = sorted(modules_by_dir[subdir], key=lambda m: m.stem)
        if subdir:
            lines.append(f'    {{ "{subdir}" = [')
            for module in modules:
                page = out_dir / subdir / f"{module.stem}.md"
                lines.append(f'      {{ "{module.title}" = "{navpath(page)}" }},')
            lines.append("    ] },")
        else:
            for module in modules:
                page = out_dir / f"{module.stem}.md"
                lines.append(f'    {{ "{module.title}" = "{navpath(page)}" }},')
    lines.append("  ] },")
    return lines


def render_nav(modules_by_dir: dict[str, list[Module]], out_dir: Path, doc_root: Path) -> str:
    """Render a standalone TOML nav snippet (for ``--nav-out``)."""
    lines = ["# Paste into the nav array of your zensical.toml (under [project])", "nav = ["]
    lines += build_nav_block(modules_by_dir, out_dir, doc_root)
    lines.append("]")
    return "\n".join(lines) + "\n"


def update_nav(path: Path, block_lines: list[str]) -> bool:
    """Replace the marker-delimited std nav block in zensical.toml. Returns False if markers are absent."""
    if not path.is_file():
        return False
    text = path.read_text(encoding="utf-8")
    lines = text.splitlines()
    begin = next((i for i, ln in enumerate(lines) if ln.strip() == NAV_BEGIN), None)
    end = next((i for i, ln in enumerate(lines) if ln.strip() == NAV_END), None)
    if begin is None or end is None or end < begin:
        return False
    new_lines = lines[: begin + 1] + block_lines + lines[end:]
    path.write_text("\n".join(new_lines) + ("\n" if text.endswith("\n") else ""), encoding="utf-8")
    return True


# --------------------------------------------------------------------------- #
# Entry point
# --------------------------------------------------------------------------- #
def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Generate Zensical Markdown docs for the Spice std library.")
    parser.add_argument("--std-dir", type=Path, default=Path("std"), help="Path to the std source directory")
    parser.add_argument("--out-dir", type=Path, default=Path("docs/docs/std"), help="Where to write Markdown pages")
    parser.add_argument("--doc-root", type=Path, default=Path("docs/docs"),
                        help="Docs content root, used to compute nav paths")
    parser.add_argument("--exclude", action="append", default=["bindings"],
                        help="Std subdirectory names to skip (repeatable)")
    parser.add_argument("--nav-out", default="-", help="Where to write the zensical nav snippet ('-' for stdout)")
    parser.add_argument("--config", type=Path, default=Path("docs/zensical.toml"),
                        help="zensical.toml whose std nav block is updated in place (between markers)")
    parser.add_argument("--skip-nav-update", action="store_true", help="Do not touch the zensical.toml nav")
    args = parser.parse_args(argv)

    if not args.std_dir.is_dir():
        print(f"error: std dir not found: {args.std_dir}", file=sys.stderr)
        return 1

    excluded = set(args.exclude)

    # Start from a clean output directory so renamed/folded pages don't leave orphans
    if args.out_dir.exists():
        shutil.rmtree(args.out_dir)

    # Group source files by (subdirectory, base stem) so per-platform variants
    # (socket_linux.spice, socket_windows.spice, ...) fold into a single page.
    groups: dict[tuple[str, str], list[Path]] = {}
    for path in sorted(args.std_dir.rglob("*.spice")):
        rel = path.relative_to(args.std_dir)
        if any(part in excluded for part in rel.parts):
            continue
        subdir = rel.parts[0] if len(rel.parts) > 1 else ""
        groups.setdefault((subdir, base_stem(path.stem)), []).append(path)

    modules_by_dir: dict[str, list[Module]] = {}
    count = 0
    for (subdir, base), paths in sorted(groups.items()):
        # Parse the base file (if any) first so its symbols lead, then the variants
        ordered = sorted(paths, key=lambda p: (p.stem != base, p.stem))
        module = merge_modules([parse_module(p, args.std_dir) for p in ordered], subdir, base)
        if module.is_empty():
            continue
        target = args.out_dir / subdir / f"{base}.md" if subdir else args.out_dir / f"{base}.md"
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(render_module(module), encoding="utf-8")
        modules_by_dir.setdefault(subdir, []).append(module)
        count += 1

    args.out_dir.mkdir(parents=True, exist_ok=True)
    (args.out_dir / "index.md").write_text(render_index(modules_by_dir), encoding="utf-8")

    nav = render_nav(modules_by_dir, args.out_dir, args.doc_root)
    if args.nav_out == "-":
        print(nav)
    elif args.nav_out:
        Path(args.nav_out).write_text(nav, encoding="utf-8")

    if not args.skip_nav_update:
        if update_nav(args.config, build_nav_block(modules_by_dir, args.out_dir, args.doc_root)):
            print(f"Updated std nav in {args.config}", file=sys.stderr)
        else:
            print(f"warning: nav markers not found in {args.config}; skipped nav update", file=sys.stderr)

    print(f"Generated {count} module pages + index under {args.out_dir}", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

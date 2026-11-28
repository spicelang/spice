"""Generator of the AST (media/specs/compiler-reimplementation.md, sections 4.4 and 6.6).

Reads src-next/syntax/ast/ast.def (the format is described in its header) and writes, below src-next/syntax/ast/:
  ast-enums.spice     the syntactic enums, TypeModifier, AstNodeKind and the kind names (the labels of the dot output)
  nodes-<group>.spice the node structs: a header, typed child fields (NodeId), list fields (ChildRange) and syntactic fields
  ast-store.spice     AstStore: one IdVec per kind, the node table, the child array, add<Kind> and get<Kind>
  ast-store-test.spice  a test that adds and reads back a node of every kind
  ast-children.spice  getHeader, setParent and collectChildren, a switch over the kinds
  ast-visitor.spice   IAstVisitor with a default traversal per kind, and acceptAstNode, the dispatch switch
"""
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from generate import header  # noqa: E402

NAME = "ast"
DEF_PATH = "src-next/syntax/ast/ast.def"
OUT_DIR = "src-next/syntax/ast/"
GENERATOR = "tools/gen/gen_ast.py"
COPYRIGHT = "// Copyright (c) 2021-2026 ChilliBits. All rights reserved."
GROUPS = ("misc", "decl", "stmt", "expr", "type")
KEYWORDS = {"type", "struct", "enum", "interface", "alias", "const", "signed", "unsigned", "public", "inline", "heap", "compose",
            "import", "as", "unsafe", "assert", "default", "case", "switch", "return", "nil", "true", "false", "this", "f", "p",
            "if", "else", "for", "foreach", "while", "do", "break", "continue", "fallthrough", "ext", "dyn", "sizeof", "alignof"}
SCALARS = {"bool": "false", "int": "0", "unsigned int": "0u", "long": "0l", "unsigned long": "0ul", "Symbol": "noSymbol()",
           "Span": None}


@dataclass
class Member:
    form: str  # child, list or field
    name: str
    type: str  # the Spice type of a field, or the allowed kinds of a child, e.g. "ASSIGN_EXPR|TERNARY_EXPR"
    optional: bool = False


@dataclass
class Node:
    kind: str
    group: str
    members: list[Member] = field(default_factory=list)

    @property
    def name(self) -> str:
        return camel(self.kind)

    @property
    def struct(self) -> str:
        return self.name + "Node"

    @property
    def var(self) -> str:
        return self.name[0].lower() + self.name[1:] + "Nodes"


@dataclass
class Definition:
    imports: list[str] = field(default_factory=list)
    enums: list[tuple[str, list[str]]] = field(default_factory=list)
    structs: list[tuple[str, list[tuple[str, str]]]] = field(default_factory=list)
    nodes: list[Node] = field(default_factory=list)


def camel(kind: str) -> str:
    return "".join(part.capitalize() for part in kind.split("_"))


def fail(number: int, message: str):
    raise SystemExit(f"error: {DEF_PATH}:{number}: {message}")


def parse(text: str) -> Definition:
    """Parses the definition. Raises SystemExit with the line number on a malformed definition."""
    d = Definition()
    for number, raw in enumerate(text.split("\n"), 1):
        line = raw.split("#", 1)[0].rstrip()
        if not line.strip():
            continue
        words = line.split()
        if not raw[0].isspace():
            if words[0] == "import" and len(words) == 2 and re.fullmatch(r'"[^"\\]+"', words[1]):
                d.imports.append(words[1])
            elif words[0] == "enum" and len(words) > 2 and words[1].endswith(":"):
                members = words[2:]
                if not words[1][:-1].isidentifier() or any(not re.fullmatch(r"[A-Z][A-Z0-9_]*", m) for m in members):
                    fail(number, "malformed enum")
                if len(set(members)) != len(members) or words[1][:-1] in (e for e, _ in d.enums):
                    fail(number, "duplicate enum or enum member")
                d.enums.append((words[1][:-1], members))
            elif words[0] == "struct" and ":" in line:
                name, _, rest = line[len("struct"):].partition(":")
                fields = [tuple(f.strip().rsplit(" ", 1)) for f in rest.split(",")]
                if not name.strip().isidentifier() or any(len(f) != 2 for f in fields):
                    fail(number, "malformed struct")
                d.structs.append((name.strip(), [(t, n) for t, n in fields]))
            elif words[0] == "node" and len(words) == 3:
                if not re.fullmatch(r"[A-Z][A-Z0-9_]*", words[1]) or words[2] not in GROUPS:
                    fail(number, f"malformed node or unknown group (known: {', '.join(GROUPS)})")
                if any(n.kind == words[1] for n in d.nodes):
                    fail(number, f"node {words[1]} is defined twice")
                d.nodes.append(Node(words[1], words[2]))
            else:
                fail(number, f"unknown directive '{line.strip()}'")
            continue
        if not d.nodes:
            fail(number, "member before the first node")
        form, _, rest = line.strip().partition(" ")
        name, sep, type_ = rest.partition(":")
        name, type_ = name.strip(), type_.strip()
        optional = name.endswith("?")
        name = name.rstrip("?")
        if form not in ("child", "list", "field") or not sep or not type_ or not name.isidentifier() or name in KEYWORDS:
            fail(number, f"malformed member '{line.strip()}'")
        if optional and form != "child":
            fail(number, "only a child can be optional")
        if any(m.name == name for m in d.nodes[-1].members) or name == "header":
            fail(number, f"member '{name}' is defined twice")
        d.nodes[-1].members.append(Member(form, name, type_, optional))
    validate(d)
    return d


def validate(d: Definition) -> None:
    kinds = {n.kind for n in d.nodes}
    enums = {e for e, _ in d.enums}
    structs = {s for s, _ in d.structs}
    for n in d.nodes:
        for m in n.members:
            if m.form != "field":
                for k in m.type.split("|"):
                    if k not in kinds:
                        raise SystemExit(f"error: {DEF_PATH}: {n.kind}.{m.name} refers to the unknown kind '{k}'")
            elif not (m.type in SCALARS or m.type in enums or m.type in structs or m.type.startswith(("SmallVec<", "Vector<"))):
                raise SystemExit(f"error: {DEF_PATH}: {n.kind}.{m.name} has the unsupported type '{m.type}'")


def file_start(d: Definition, imports: list[str]) -> list[str]:
    return [header(GENERATOR, DEF_PATH).rstrip("\n"), COPYRIGHT, ""] + [f"import {i};" for i in imports] + [""]


def own(path: str) -> str:
    return f'"@/syntax/ast/{path}"'


def emit_enums(d: Definition) -> str:
    out = file_start(d, d.imports)
    for name, members in d.enums:
        out += [f"public type {name} enum {{"] + [f"    {m}{',' if i < len(members) - 1 else ''}" for i, m in enumerate(members)] + ["}", ""]
    for name, fields in d.structs:
        out.append(f"public type {name} struct {{")
        out += [f"    public {t} {n}" for t, n in fields]
        out += ["}", "", f"public p {name}.ctor() {{"]
        for t, n in fields:
            out.append(f"    this.{n} = {default_of(d, t)};")
        out += ["}", ""]
        out.append(f"public p {name}.ctor(" + ", ".join(f"{t} {n}" for t, n in fields) + ") {")
        out += [f"    this.{n} = {n};" for _, n in fields] + ["}", ""]
    kinds = [n.kind for n in d.nodes]
    out += ["/**", " * Kind of an AST node. The order is the order of the legacy ASTNodeKind. INVALID stands for no node", " */",
            "public type AstNodeKind enum {", "    INVALID,"]
    out += [f"    {k}{',' if i < len(kinds) - 1 else ''}" for i, k in enumerate(kinds)] + ["}", ""]
    out += ["/**", " * @return Number of node kinds without INVALID", " */", "public f<unsigned int> getAstNodeKindCount() {",
            f"    return {len(kinds)}u;", "}", ""]
    out += ["/**", " * Name of a kind in the dot output of the AST: the legacy class name without the AST prefix, e.g. EntryNode", " *",
            " * @param kind Node kind", " * @return Name", " */", "public f<string> getAstNodeKindName(AstNodeKind kind) {", "    switch kind {",
            '        case AstNodeKind::INVALID: { return "InvalidNode"; }']
    out += [f'        case AstNodeKind::{n.kind}: {{ return "{n.struct}"; }}' for n in d.nodes]
    out += ["    }", '    panic(Error("Unknown AST node kind"));', "}", "", "#[test]", "f<bool> testAstNodeKinds() {",
            f"    assert getAstNodeKindCount() == {len(kinds)}u;",
            f"    assert cast<unsigned int>(AstNodeKind::{kinds[-1]}) == {len(kinds)}u;",
            f'    assert String(getAstNodeKindName(AstNodeKind::{kinds[0]})) == "{d.nodes[0].struct}";',
            f'    assert String(getAstNodeKindName(AstNodeKind::{kinds[-1]})) == "{d.nodes[-1].struct}";', "    return true;", "}"]
    return "\n".join(out) + "\n"


def default_of(d: Definition, type_: str) -> str:
    if type_ in SCALARS and SCALARS[type_]:
        return SCALARS[type_]
    if type_ == "Span":
        return "Span()"
    for name, members in d.enums:
        if name == type_:
            return f"{name}::{members[0]}"
    return f"{type_}()"


def emit_nodes(d: Definition, group: str) -> str:
    out = file_start(d, d.imports + [own("ast-enums"), own("ast-node")])
    for n in (n for n in d.nodes if n.group == group):
        out += [f"/**", f" * Node {n.kind}", " */", f"public type {n.struct} struct {{", "    public AstHeader header"]
        for m in n.members:
            kinds = m.type.replace("|", " or ") if m.form != "field" else ""
            if m.form == "child":
                out.append(f"    public NodeId {m.name} // {kinds}{' (optional)' if m.optional else ''}")
            elif m.form == "list":
                out.append(f"    public ChildRange {m.name} // {kinds}")
            else:
                out.append(f"    public {m.type} {m.name}")
        out += ["}", "", f"public p {n.struct}.ctor() {{", f"    this.header = AstHeader(AstNodeKind::{n.kind});"]
        for m in n.members:
            if m.form == "child":
                out.append(f"    this.{m.name} = noNodeId();")
            elif m.form == "list":
                out.append(f"    this.{m.name} = ChildRange();")
            elif not m.type.startswith(("SmallVec<", "Vector<")):
                out.append(f"    this.{m.name} = {default_of(d, m.type)};")
        out += ["}", ""]
    return "\n".join(out).rstrip("\n") + "\n"


def emit_store(d: Definition) -> str:
    groups = [own(f"nodes-{g}") for g in GROUPS]
    out = file_start(d, ['"std/data/vector"', '"@/base/id-vec"', own("ast-enums"), own("ast-node")] + groups + ['"@/source/span"'])
    out += ["/**", " * All nodes of a module. Every kind has an IdVec of its own, so that a node needs no more memory than its fields. The node",
            " * table maps a NodeId to the kind and the index in that vector, and the child array holds the lists of children. The store",
            " * is append-only: the parser adds the nodes, and every later stage only reads them.", " */", "public type AstStore struct {",
            "    IdVec<AstNode> nodes", "    Vector<NodeId> childArray"]
    out += [f"    IdVec<{n.struct}> {n.var}" for n in d.nodes] + ["}", ""]
    out += ["public const f<unsigned int> AstStore.getNodeCount() {", "    return cast<unsigned int>(this.nodes.size());", "}", "",
            "public const f<AstNodeKind> AstStore.getKind(NodeId id) {", "    return this.nodes.get(id).kind;", "}", "",
            "/**", " * Store the children of a list field next to each other", " *", " * @param items Ids of the children, in order",
            " * @return Range of the child array that holds them", " */",
            "public f<ChildRange> AstStore.addChildren(const Vector<NodeId>& items) {",
            "    const unsigned int start = cast<unsigned int>(this.childArray.getSize());",
            "    foreach NodeId item : items {", "        this.childArray.pushBack(item);", "    }",
            "    return ChildRange(start, cast<unsigned int>(items.getSize()));", "}", "",
            "public const f<NodeId> AstStore.getChild(const ChildRange& range, unsigned int i) {", "    assert i < range.count;",
            "    return this.childArray.get(range.start + i);", "}", "",
            "f<NodeId> AstStore.registerNode(AstNodeKind kind, unsigned int index) {",
            "    const NodeId id = makeNodeId(cast<unsigned int>(this.nodes.size()));",
            "    const Id<AstNode> _ = this.nodes.push(AstNode(kind, index));", "    return id;", "}", ""]
    for n in d.nodes:
        out += [f"// Add a {n.kind} node. The header is completed here: the id and the span",
                f"public f<NodeId> AstStore.add{n.name}({n.struct} node, const Span& span) {{",
                f"    const NodeId id = this.registerNode(AstNodeKind::{n.kind}, cast<unsigned int>(this.{n.var}.size()));",
                "    node.header.id = id;", "    node.header.span = span;", f"    const Id<{n.struct}> _ = this.{n.var}.push(node);",
                "    return id;", "}",
                f"public const f<{n.struct}&> AstStore.get{n.name}(NodeId id) {{",
                f"    assert this.nodes.get(id).kind == AstNodeKind::{n.kind};",
                f"    return this.{n.var}.get(Id<{n.struct}>(this.nodes.get(id).index));", "}", ""]
    return "\n".join(out).rstrip("\n") + "\n"


def emit_store_test(d: Definition) -> str:
    out = file_start(d, ['"std/data/vector"', '"@/source/span"', own("ast-enums"), own("ast-node")] + [own(f"nodes-{g}") for g in GROUPS] + [own("ast-store")])
    for part, start in enumerate(range(0, len(d.nodes), 20), 1):
        out += ["#[test]", f"f<bool> testAddAndGetEveryKind{part}() {{", "    AstStore store;"]
        for i, n in enumerate(d.nodes[start:start + 20], start):
            out += [f"    const NodeId id{i} = store.add{n.name}({n.struct}(), Span());",
                    f"    assert store.getKind(id{i}) == AstNodeKind::{n.kind} && id{i}.index == {i - start}u;",
                    f"    assert store.get{n.name}(id{i}).header.id == id{i} && store.get{n.name}(id{i}).header.kind == AstNodeKind::{n.kind};"]
        out += [f"    assert store.getNodeCount() == {min(20, len(d.nodes) - start)}u;", "    return true;", "}", ""]
    return "\n".join(out).rstrip("\n") + "\n"


def child_pushes(n: Node) -> list[str]:
    out = []
    for m in n.members:
        if m.form == "child":
            out.append(f"            pushChild(out, node.{m.name});")
        elif m.form == "list":
            out.append(f"            pushChildRange(store, out, node.{m.name});")
    return out


def emit_children(d: Definition) -> str:
    out = file_start(d, ['"std/data/vector"', own("ast-enums"), own("ast-node")] + [own(f"nodes-{g}") for g in GROUPS] + [own("ast-store")])
    out += ["p pushChild(Vector<NodeId>& out, NodeId child) {", "    if isNodeId(child) {", "        out.pushBack(child);", "    }", "}", "",
            "p pushChildRange(const AstStore& store, Vector<NodeId>& out, const ChildRange& range) {",
            "    for unsigned int i = 0u; i < range.count; i++ {", "        out.pushBack(store.getChild(range, i));", "    }", "}", "",
            ]
    for n in d.nodes:
        pushes = child_pushes(n)
        if pushes:
            out += [f"p collect{n.name}Children(const AstStore& store, NodeId id, Vector<NodeId>& out) {{",
                    f"    const {n.struct}& node = store.get{n.name}(id);"] + [p[8:] for p in pushes] + ["}", ""]
        else:
            out += [f"p collect{n.name}Children(const AstStore& store, NodeId id, Vector<NodeId>& out) {{}}", ""]
    out += [
            "/**", " * Header of a node", " *", " * @param store Store of the node", " * @param id Id of the node", " * @return Pointer to the header, valid until a node of the same kind is added",
            " */", "public f<AstHeader*> getHeader(const AstStore& store, NodeId id) {", "    switch store.getKind(id) {"]
    out += [f"        case AstNodeKind::{n.kind}: {{ return &store.get{n.name}(id).header; }}" for n in d.nodes]
    out += ["        default: { panic(Error(\"Unknown AST node kind\")); }", "    }", "}", "",
            "/**", " * Set the parent of a node. The parser does it when it attaches a node to its parent", " */",
            "public p setParent(const AstStore& store, NodeId child, NodeId parent) {", "    getHeader(store, child).parent = parent;", "}", "",
            "/**", " * Append the children of a node to `out`, in the order of `children` in the legacy AST. A missing optional child is skipped", " *",
            " * @param store Store of the node", " * @param id Id of the node", " * @param out Vector to append the ids to", " */",
            "public p collectChildren(const AstStore& store, NodeId id, Vector<NodeId>& out) {", "    switch store.getKind(id) {"]
    for n in d.nodes:
        out.append(f"        case AstNodeKind::{n.kind}: {{ collect{n.name}Children(store, id, out); }}")
    out += ["        default: {}", "    }", "}"]
    return "\n".join(out) + "\n"


def emit_visitor(d: Definition) -> str:
    out = file_start(d, ['"std/data/vector"', own("ast-enums"), own("ast-node")] + [own(f"nodes-{g}") for g in GROUPS] + [own("ast-store"), own("ast-children")])
    out += ["/**", " * Visitor of the AST for tools (the dot printer, the formatter, the linter). The default of every method visits the children.",
            " * The passes of the compiler dispatch with a switch on the kind instead (plan section 3.2), because no pass needs this indirection", " */",
            "public type IAstVisitor interface {", "    public f<const AstStore*> getAstStore();", "    public p visit(NodeId id);", "    public p visitChildren(NodeId id);"]
    out += [f"    public p visit{n.name}(NodeId id);" for n in d.nodes] + ["}", ""]
    out += ["public p IAstVisitor.visit(NodeId id) {", "    acceptAstNode(*this.getAstStore(), id, this);", "}", "",
            "public p IAstVisitor.visitChildren(NodeId id) {", "    Vector<NodeId> children;", "    collectChildren(*this.getAstStore(), id, children);",
            "    foreach NodeId child : children {", "        this.visit(child);", "    }", "}", ""]
    out += [f"public p IAstVisitor.visit{n.name}(NodeId id) {{\n    this.visitChildren(id);\n}}\n" for n in d.nodes]
    out += ["/**", " * Call the visit method of the kind of a node", " *", " * @param store Store of the node", " * @param id Id of the node",
            " * @param visitor Visitor to call", " */", "public p acceptAstNode(const AstStore& store, NodeId id, IAstVisitor* visitor) {",
            "    switch store.getKind(id) {"]
    out += [f"        case AstNodeKind::{n.kind}: {{ visitor.visit{n.name}(id); }}" for n in d.nodes]
    out += ["        default: { panic(Error(\"Unknown AST node kind\")); }", "    }", "}"]
    return "\n".join(out) + "\n"


def generate(root: Path) -> dict[str, str]:
    d = parse((root / DEF_PATH).read_text(encoding="utf-8"))
    files = {OUT_DIR + "ast-enums.spice": emit_enums(d)}
    for g in GROUPS:
        files[OUT_DIR + f"nodes-{g}.spice"] = emit_nodes(d, g)
    files[OUT_DIR + "ast-store.spice"] = emit_store(d)
    files[OUT_DIR + "ast-store-test.spice"] = emit_store_test(d)
    files[OUT_DIR + "ast-children.spice"] = emit_children(d)
    files[OUT_DIR + "ast-visitor.spice"] = emit_visitor(d)
    return files

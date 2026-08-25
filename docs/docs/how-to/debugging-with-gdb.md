---
title: Debugging with GDB
---

Spice compiles to native machine code via LLVM and, when built with [`-g`](../cli/build.md), emits real DWARF
debug info. This means you can debug a Spice executable with GDB just like a C or C++ program: set breakpoints,
step through source lines, and inspect local variables.

By default, though, GDB shows a standard library container by its raw internal layout rather than its logical
contents. For example, without any help, a `Vector<int>` holding `10, 20, 30` prints as:

```text
(gdb) print numbers
$1 = {contents = 0x555555559eb0, capacity = 5, size = 3}
```

That's the struct's fields, not the vector's elements. It gets worse for pointer-based structures such as lists,
trees and hash tables, where the interesting data lives behind chains of pointers GDB has no reason to follow.

## Loading the pretty printers

### Automatically (apk/deb/rpm/archlinux packages)

The Linux packages install a drop-in at `/etc/gdb/gdbinit.d/spice.gdb` that sources the printer script for you. GDB
picks it up automatically on distros whose GDB build reads that directory (Debian/Ubuntu do); nothing to do there.
If `print`ing a container still shows raw fields, your GDB build doesn't read `/etc/gdb/gdbinit.d`, and you can load
the script manually as described below.

### Manually

The repository ships a GDB pretty-printer script at `tools/gdb/spice_printers.py` that teaches GDB how to render
every data structure in `std/data/`, plus the runtime `String` type, in their logical form instead. Load it in a
running GDB session with:

```text
(gdb) source /path/to/spice/tools/gdb/spice_printers.py
```

or add that line to your `~/.gdbinit` to have it loaded for every session.

Where that path points depends on how you got Spice:

- **Built from a source checkout**: `<repo>/tools/gdb/spice_printers.py`.
- **Installed via apk/deb/rpm/archlinux package**: `/usr/lib/spice/gdb/spice_printers.py`, next to
  `/usr/lib/spice/std`.
- **Downloaded release archive or Homebrew cask**: `tools/gdb/spice_printers.py` alongside the `std` directory in
  the unpacked archive (or, for the cask, `$(brew --caskroom)/spice/<version>/tools/gdb/spice_printers.py`).

With the script loaded, the same `Vector<int>` prints as:

```text
(gdb) print numbers
$1 = Vector<int> of length 3 = {10, 20, 30}
```

Nested generics are handled recursively — a `Vector<Pair<int,int>>` prints its `Pair` elements through the `Pair`
printer too, and so on.

## Covered types

| Type(s) | Rendered as |
|---|---|
| `Vector`, `Stack`, `PriorityQueue` | `{e0, e1, ...}` in storage order |
| `Queue`, `Deque` | `{e0, e1, ...}` in logical front-to-back order |
| `LinkedList`, `DoublyLinkedList` | `{e0, e1, ...}` in push-back order |
| `HashTable`, `UnorderedMap` | `{key = value, ...}`, unordered |
| `RedBlackTree`, `Map` | `{key = value, ...}`, sorted by key |
| `Set`, `UnorderedSet` | `{v0, v1, ...}` (keys only) |
| `BinaryTree` | `{v0, v1, ...}`, sorted (in-order traversal) |
| `Pair`, `Triple` | `(first, second[, third])` |
| `Optional` | `Some(value)` or `None` |
| `BitSet` | A bit string, e.g. `BitSet of 8 bits = 01001000` |
| `Trie` | The stored words, e.g. `{car, cart, cat}` |
| `Graph`, `Vertex` | A directed/undirected summary plus the vertex set |
| `String` | A quoted string |

The dispatch is based on the DWARF struct name that the compiler emits for generic instantiations (e.g.
`Vector<int>`), so the printers apply to any concrete instantiation of these generic types automatically — there is
nothing to configure per element type.

## Limitations

- The printers only kick in when GDB prints a struct *value* (a local variable, a dereferenced pointer, `print
  *ptr`). Printing a pointer to one of these structs directly still shows a bare address, as usual in GDB.
- A `Graph`'s adjacency list is not expanded inline; inspect `graph.adjList` directly to see it (it gets the
  `UnorderedMap` printer too).

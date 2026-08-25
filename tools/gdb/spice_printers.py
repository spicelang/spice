"""GDB pretty printers for the Spice standard library data structures.

Spice (https://github.com/spicelang/spice) emits real DWARF debug info when compiled with
`-g`. Generic struct instantiations are named by their signature (e.g. `Vector<int>`,
`HashTable<String,int>`), which is what this module dispatches on. Without these printers,
GDB shows the raw internal layout of a container (e.g. `{contents = 0x555.., capacity = 5,
size = 5}`), which hides the actual contents and is unreadable for pointer-based structures
such as lists, trees and hash tables.

Usage
-----
In a running GDB session:

    (gdb) source /path/to/spice/tools/gdb/spice_printers.py

Or add that line to your `~/.gdbinit` to load it for every session.

Covered types (all of `std/data/` plus the runtime `String` type)
-------------------------------------------------------------------
Vector, Stack, Queue, Deque, PriorityQueue, LinkedList, DoublyLinkedList, HashTable,
RedBlackTree, BinaryTree, Map, Set, UnorderedMap, UnorderedSet, Pair, Triple, Optional,
BitSet, Trie, Graph, Vertex, String.
"""

import re
import gdb

# Slot hash sentinel from std/data/hash-table.spice - a slot is occupied iff hash > HASH_TOMBSTONE
_HASH_TOMBSTONE = 1

_BITS_PER_WORD = 64


def _is_null(ptr):
    return int(ptr) == 0


def _elements(count):
    return f"{count} element" if count == 1 else f"{count} elements"


def _rb_inorder(root_ptr):
    """In-order traversal of a RedBlackTree/BinaryTree-style node chain (childLeft/childRight)."""
    if _is_null(root_ptr):
        return
    node = root_ptr.dereference()
    yield from _rb_inorder(node["childLeft"])
    yield node
    yield from _rb_inorder(node["childRight"])


def _hash_slots(hash_table_val):
    """Yield (key, value) gdb.Value pairs for every occupied slot of a HashTable value."""
    capacity = int(hash_table_val["capacity"])
    if capacity == 0:
        return
    slots = hash_table_val["slots"]
    for i in range(capacity):
        slot = slots[i]
        if int(slot["hash"]) <= _HASH_TOMBSTONE:
            continue
        entry = slot["entry"].dereference()
        yield entry["key"], entry["value"]


def _linked_list_values(list_val):
    """Yield element values of a LinkedList/DoublyLinkedList in push-back order."""
    node_ptr = list_val["tail"]
    while not _is_null(node_ptr):
        node = node_ptr.dereference()
        yield node["value"]
        node_ptr = node["next"]


class SpiceArrayPrinter:
    """Vector<T>, Stack<T>, PriorityQueue<T> - flat contiguous storage."""

    def __init__(self, val, type_name):
        self.val = val
        self.type_name = type_name

    def to_string(self):
        return f"{self.type_name} of length {int(self.val['size'])}"

    def children(self):
        contents = self.val["contents"]
        for i in range(int(self.val["size"])):
            yield str(i), contents[i]

    def display_hint(self):
        return "array"


class SpiceCircularBufferPrinter:
    """Queue<T>, Deque<T> - circular buffer addressed via idxFront/capacity."""

    def __init__(self, val, type_name):
        self.val = val
        self.type_name = type_name

    def to_string(self):
        return f"{self.type_name} of length {int(self.val['size'])}"

    def children(self):
        size = int(self.val["size"])
        capacity = int(self.val["capacity"])
        front = int(self.val["idxFront"])
        contents = self.val["contents"]
        for i in range(size):
            yield str(i), contents[(front + i) % capacity]

    def display_hint(self):
        return "array"


class SpiceLinkedListPrinter:
    """LinkedList<T>, DoublyLinkedList<T> - singly-linked chain, owning pointer in `tail`."""

    def __init__(self, val, type_name):
        self.val = val
        self.type_name = type_name

    def to_string(self):
        return f"{self.type_name} of length {int(self.val['size'])}"

    def children(self):
        for i, value in enumerate(_linked_list_values(self.val)):
            yield str(i), value

    def display_hint(self):
        return "array"


class SpiceHashTablePrinter:
    """HashTable<K,V> - open-addressed slot array."""

    def __init__(self, val, type_name):
        self.val = val
        self.type_name = type_name

    def to_string(self):
        return f"{self.type_name} with {_elements(int(self.val['size']))}"

    def children(self):
        for i, (key, value) in enumerate(_hash_slots(self.val)):
            yield f"key{i}", key
            yield f"value{i}", value

    def display_hint(self):
        return "map"


class SpiceRedBlackTreePrinter:
    """RedBlackTree<K,V> - printed as an ordered key:value map."""

    def __init__(self, val, type_name):
        self.val = val
        self.type_name = type_name

    def to_string(self):
        return f"{self.type_name} with {_elements(int(self.val['size']))}"

    def children(self):
        for i, node in enumerate(_rb_inorder(self.val["rootNode"])):
            yield f"key{i}", node["key"]
            yield f"value{i}", node["value"]

    def display_hint(self):
        return "map"


class SpiceBinaryTreePrinter:
    """BinaryTree<T> - binary search tree, printed in sorted (in-order) order."""

    def __init__(self, val, type_name):
        self.val = val
        self.type_name = type_name

    def to_string(self):
        return self.type_name

    def children(self):
        for i, node in enumerate(_rb_inorder(self.val["rootNode"])):
            yield str(i), node["value"]

    def display_hint(self):
        return "array"


class SpiceMapPrinter:
    """Map<K,V> - thin RedBlackTree<K,V> wrapper, printed as an ordered key:value map."""

    def __init__(self, val, type_name):
        self.val = val
        self.type_name = type_name

    def to_string(self):
        return f"{self.type_name} with {_elements(int(self.val['tree']['size']))}"

    def children(self):
        for i, node in enumerate(_rb_inorder(self.val["tree"]["rootNode"])):
            yield f"key{i}", node["key"]
            yield f"value{i}", node["value"]

    def display_hint(self):
        return "map"


class SpiceSetPrinter:
    """Set<V> - thin RedBlackTree<V,bool> wrapper, printed as an ordered set of keys."""

    def __init__(self, val, type_name):
        self.val = val
        self.type_name = type_name

    def to_string(self):
        return f"{self.type_name} with {_elements(int(self.val['tree']['size']))}"

    def children(self):
        for i, node in enumerate(_rb_inorder(self.val["tree"]["rootNode"])):
            yield str(i), node["key"]

    def display_hint(self):
        return "array"


class SpiceUnorderedMapPrinter:
    """UnorderedMap<K,V> - thin HashTable<K,V> wrapper, printed as a key:value map."""

    def __init__(self, val, type_name):
        self.val = val
        self.type_name = type_name

    def to_string(self):
        return f"{self.type_name} with {_elements(int(self.val['hashTable']['size']))}"

    def children(self):
        for i, (key, value) in enumerate(_hash_slots(self.val["hashTable"])):
            yield f"key{i}", key
            yield f"value{i}", value

    def display_hint(self):
        return "map"


class SpiceUnorderedSetPrinter:
    """UnorderedSet<V> - thin HashTable<V,bool> wrapper, printed as a set of keys."""

    def __init__(self, val, type_name):
        self.val = val
        self.type_name = type_name

    def to_string(self):
        return f"{self.type_name} with {_elements(int(self.val['hashTable']['size']))}"

    def children(self):
        for i, (key, _value) in enumerate(_hash_slots(self.val["hashTable"])):
            yield str(i), key

    def display_hint(self):
        return "array"


class SpicePairPrinter:
    """Pair<V1,V2>."""

    def __init__(self, val, _type_name):
        self.val = val

    def to_string(self):
        return f"({self.val['first']}, {self.val['second']})"


class SpiceTriplePrinter:
    """Triple<V1,V2,V3>."""

    def __init__(self, val, _type_name):
        self.val = val

    def to_string(self):
        return f"({self.val['first']}, {self.val['second']}, {self.val['third']})"


class SpiceOptionalPrinter:
    """Optional<T>."""

    def __init__(self, val, _type_name):
        self.val = val

    def to_string(self):
        if bool(self.val["isPresent"]):
            return f"Some({self.val['data']})"
        return "None"


class SpiceBitSetPrinter:
    """BitSet - fixed-size bit vector, LSB-first within each 64-bit word."""

    def __init__(self, val, _type_name):
        self.val = val

    def to_string(self):
        num_bits = int(self.val["numBits"])
        if num_bits == 0:
            return "BitSet of 0 bits"
        words = self.val["words"]
        bits = []
        for i in range(num_bits):
            word = words[i // _BITS_PER_WORD]
            bit = (int(word) >> (i % _BITS_PER_WORD)) & 1
            bits.append("1" if bit else "0")
        return f"BitSet of {num_bits} bits = {''.join(bits)}"


class SpiceTriePrinter:
    """Trie - reconstructs the stored words by walking the edge-labeled node tree."""

    def __init__(self, val, _type_name):
        self.val = val

    def to_string(self):
        return f"Trie with {int(self.val['wordCount'])} words"

    def _words(self, node_ptr, prefix):
        if _is_null(node_ptr):
            return
        node = node_ptr.dereference()
        if bool(node["isEndOfWord"]):
            yield "".join(prefix)
        keys = node["keys"]
        children = node["children"]
        key_contents = keys["contents"]
        child_contents = children["contents"]
        for i in range(int(keys["size"])):
            prefix.append(chr(int(key_contents[i]) & 0xFF))
            yield from self._words(child_contents[i], prefix)
            prefix.pop()

    def children(self):
        for i, word in enumerate(self._words(self.val["root"], [])):
            yield str(i), word

    def display_hint(self):
        return "array"


class SpiceGraphPrinter:
    """Graph<T> - summarized as directed/undirected plus its vertex set; adjacency stays
    reachable through the (separately pretty-printed) `adjList` field."""

    def __init__(self, val, type_name):
        self.val = val
        self.type_name = type_name

    def to_string(self):
        kind = "Directed" if bool(self.val["directed"]) else "Undirected"
        vertex_count = int(self.val["vertices"]["size"])
        return f"{kind} {self.type_name} with {vertex_count} vertices"

    def children(self):
        vertices = self.val["vertices"]
        contents = vertices["contents"]
        for i in range(int(vertices["size"])):
            yield str(i), contents[i]["value"]

    def display_hint(self):
        return "array"


class SpiceVertexPrinter:
    """Vertex<T> - transparent wrapper, printed as just its wrapped value."""

    def __init__(self, val, _type_name):
        self.val = val

    def to_string(self):
        return str(self.val["value"])


class SpiceStringPrinter:
    """String - heap-allocated, non-null-terminated-length-tracked string."""

    def __init__(self, val, _type_name):
        self.val = val

    def to_string(self):
        length = int(self.val["length"])
        contents = self.val["contents"]
        if length == 0 or _is_null(contents):
            return '""'
        return contents.lazy_string(length=length)


# Base struct name (before the first '<') -> printer class
_PRINTERS = {
    "Vector": SpiceArrayPrinter,
    "Stack": SpiceArrayPrinter,
    "PriorityQueue": SpiceArrayPrinter,
    "Queue": SpiceCircularBufferPrinter,
    "Deque": SpiceCircularBufferPrinter,
    "LinkedList": SpiceLinkedListPrinter,
    "DoublyLinkedList": SpiceLinkedListPrinter,
    "HashTable": SpiceHashTablePrinter,
    "RedBlackTree": SpiceRedBlackTreePrinter,
    "BinaryTree": SpiceBinaryTreePrinter,
    "Map": SpiceMapPrinter,
    "Set": SpiceSetPrinter,
    "UnorderedMap": SpiceUnorderedMapPrinter,
    "UnorderedSet": SpiceUnorderedSetPrinter,
    "Pair": SpicePairPrinter,
    "Triple": SpiceTriplePrinter,
    "Optional": SpiceOptionalPrinter,
    "BitSet": SpiceBitSetPrinter,
    "Trie": SpiceTriePrinter,
    "Graph": SpiceGraphPrinter,
    "Vertex": SpiceVertexPrinter,
    "String": SpiceStringPrinter,
}

_TAG_RE = re.compile(r"^([A-Za-z_][A-Za-z0-9_]*)(?:<.*>)?$")


def spice_lookup_function(val):
    type_ = val.type
    if type_.code == gdb.TYPE_CODE_REF:
        type_ = type_.target()
    type_ = type_.strip_typedefs()
    if type_.code != gdb.TYPE_CODE_STRUCT:
        return None

    tag = type_.tag
    if not tag:
        return None

    match = _TAG_RE.match(tag)
    if not match:
        return None

    printer_cls = _PRINTERS.get(match.group(1))
    if printer_cls is None:
        return None

    return printer_cls(val, tag)


def register_spice_printers(obj_file=None):
    target = obj_file if obj_file is not None else gdb
    printers = getattr(target, "pretty_printers", None)
    if printers is None:
        printers = []
        target.pretty_printers = printers
    if spice_lookup_function not in printers:
        printers.append(spice_lookup_function)


register_spice_printers(gdb.current_objfile())

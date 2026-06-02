# Spice Standard Library

This subdirectory is contained within every installation of Spice. It contains common functionality that can be made use
of by importing it via `import "std/...";`.

The standard library is itself written in Spice. Each package is a collection of `.spice` source files grouped by topic.
Files whose names end in a platform suffix (e.g. `_linux`, `_windows`, `_darwin`) or an architecture suffix
(e.g. `_x86_64`, `_aarch64`) provide platform-specific implementations; the compiler selects the matching variant for the
target.

## Usage
Import a module and use its exported symbols:

```spice
import "std/data/vector";
import "std/io/file";

f<int> main() {
    Vector<int> numbers;
    numbers.pushBack(1);
    numbers.pushBack(2);
    printf("Sum: %d\n", numbers.get(0) + numbers.get(1));
}
```

## Packages

### `std/bindings`
Spice bindings for common external libraries, allowing them to be called from Spice code.

| Module            | Description                                                            |
|-------------------|------------------------------------------------------------------------|
| `gtk/gtk4`        | Bindings for the GTK 4 GUI toolkit.                                    |
| `libcurl/libcurl` | Bindings for libcurl, used for network transfers (see `std/net/http`). |
| `llvm/llvm`       | Bindings for the LLVM C API, plus linker flags and a target wrapper.   |

### `std/data`
Common data structures for organizing and storing data.

| Module                               | Description                                           |
|--------------------------------------|-------------------------------------------------------|
| `vector`                             | Dynamic, growable array.                              |
| `deque`                              | Double-ended queue.                                   |
| `queue` / `stack`                    | FIFO queue and LIFO stack.                            |
| `priority-queue`                     | Binary max-heap served by priority.                   |
| `linked-list` / `doubly-linked-list` | Singly and doubly linked lists.                       |
| `map` / `unordered-map`              | Ordered and hash-based key/value maps.                |
| `set` / `unordered-set`              | Ordered and hash-based sets.                          |
| `hash-table`                         | Hash table backing the unordered containers.          |
| `binary-tree` / `red-black-tree`     | Binary search tree and self-balancing red-black tree. |
| `trie`                               | Prefix tree for fast prefix-based string lookups.     |
| `graph`                              | Generic graph structure.                              |
| `bitset`                             | Fixed-size set of bits.                               |
| `pair` / `triple`                    | Tuples holding two or three values.                   |
| `optional`                           | Container that may or may not hold a value.           |

### `std/io`
Reading and writing files, working with paths and directories, command-line parsing, and logging.

| Module                                         | Description                                                    |
|------------------------------------------------|----------------------------------------------------------------|
| `file`                                         | Reading from and writing to files.                             |
| `dir`                                          | Directory listing and manipulation.                            |
| `filepath`                                     | Path construction and inspection.                              |
| `cli-parser` / `cli-option` / `cli-subcommand` | Building command-line interfaces with options and subcommands. |
| `logging`                                      | Leveled logging utilities.                                     |

### `std/iterator`
The `Iterable` interface and commonly used iterators that integrate with Spice's `foreach` loops.

| Module            | Description                   |
|-------------------|-------------------------------|
| `iterable`        | The `Iterable` interface.     |
| `iterator`        | Base iterator support.        |
| `array-iterator`  | Iterator over arrays.         |
| `number-iterator` | Iterator over numeric ranges. |

### `std/math`
Mathematical operations, constants, hashing, and randomness.

| Module   | Description                                         |
|----------|-----------------------------------------------------|
| `const`  | Mathematical constants (e.g. pi, e).                |
| `fct`    | Math functions (powers, roots, trigonometry, etc.). |
| `hash`   | Hash functions used by hash-based containers.       |
| `rand`   | Pseudo-random number generation.                    |

### `std/net`
Network communication via sockets and HTTP.

| Module   | Description                                                                        |
|----------|------------------------------------------------------------------------------------|
| `socket` | Cross-platform socket abstraction (with `_linux`, `_darwin`, `_windows` variants). |
| `http`   | HTTP client built on top of `std/bindings/libcurl`.                                |

### `std/os`
Interacting with the underlying operating system: processes, threads, memory, environment, and raw syscalls.

| Module                   | Description                                             |
|--------------------------|---------------------------------------------------------|
| `cmd`                    | Running external programs and capturing their output.   |
| `env`                    | Reading and setting environment variables.              |
| `filesystem`             | Filesystem operations.                                  |
| `cpu`                    | CPU information.                                        |
| `system` / `os`          | System and OS information (platform-specific variants). |
| `thread` / `thread-pool` | Spawning threads and managing pools of workers.         |
| `mutex` / `atomic`       | Synchronization primitives.                             |
| `allocator`              | Memory allocation utilities.                            |
| `syscall`                | Architecture-specific raw system call interfaces.       |

### `std/runtime`
Internal package linked automatically into any Spice executable when required by the compiler. It backs core language
features and is not normally imported directly.

| Module      | Description                        |
|-------------|------------------------------------|
| `error_rt`  | Error handling runtime support.    |
| `memory_rt` | Memory management runtime support. |
| `result_rt` | `Result` type runtime support.     |
| `rtti_rt`   | Runtime type information.          |
| `string_rt` | String runtime support.            |

### `std/test`
Basic mechanisms for testing in Spice.

| Module       | Description                       |
|--------------|-----------------------------------|
| `assertions` | Assertion helpers for unit tests. |
| `bench`      | Benchmarking utilities.           |

### `std/text`
Text processing, parsing, and formatted output.

| Module                                      | Description                     |
|---------------------------------------------|---------------------------------|
| `print`                                     | Formatted printing helpers.     |
| `format`                                    | String formatting.              |
| `stringstream`                              | Stream-style string building.   |
| `string-ext`                                | Extra string utility functions. |
| `analysis`                                  | Text analysis helpers.          |
| `csv-parser` / `json-parser` / `xml-parser` | Parsers for CSV, JSON, and XML. |

### `std/time`
Accessing system time in different formats and measuring durations.

| Module     | Description                                     |
|------------|-------------------------------------------------|
| `time`     | Low-level time access (with platform variants). |
| `datetime` | Date and time representation.                   |
| `timer`    | Measuring elapsed time.                         |
| `delay`    | Sleeping/delaying execution.                    |

### `std/type`
Type-related work such as type conversions and inspection.

| Module                   | Description                    |
|--------------------------|--------------------------------|
| `types`                  | Type-related helpers.          |
| `type-conversion`        | Conversions between types.     |
| `any`                    | A type-erased `Any` container. |
| `short` / `int` / `long` | Integer type helpers.          |

© ChilliBits 2021-2026

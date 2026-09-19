---
title: Stack traces
---

Spice can print the call stack of a running program, or hand it to you as data. Two functions from the stack
trace runtime do this; like the other `s`-prefixed runtime functions they are auto-imported, so no `import` is
needed:

- `sDumpStacktrace(bool includeAddresses = true, bool hideNonSpiceFrames = false)` prints the calling function's stack
  to stderr.
- `sGetStacktrace() -> StackTrace` returns it as a value you can walk yourself.

```spice
p levelC() {
    sDumpStacktrace();
}

p levelB() {
    levelC();
}

f<int> main() {
    levelB();
}
```

Built with [`-g`](../cli/build.md), this prints something like:

```text
Stack trace:
  #0  0x000055a3f1c012c3  levelC() + 0xd at /home/me/trace.spice:2
  #1  0x000055a3f1c012d3  levelB() + 0xd at /home/me/trace.spice:6
  #2  0x000055a3f1c012e3  main + 0xc at /home/me/trace.spice:10
  #3  0x00007f2c9ba2a1c9  __libc_start_call_main + 0x7a
  #4  0x00007f2c9ba2a28a  __libc_start_main_impl + 0x8a
  #5  0x000055a3f1c011b4  _start + 0x24
```

One line per frame, most recent call first, each with:

1. the frame number, counting up from `#0` at the most recent call,
2. the address the frame will return to,
3. the demangled function name, followed by `+` and the byte offset from where that function starts,
4. `at <file>:<line>`, when the program carries debug info.

Frames below `main` belong to the C runtime and differ per platform. A frame whose symbol could not be resolved
prints as `<unknown>`; a frame in a program without debug info simply has no `at ...` part.

Pass `false` as the first argument to leave the address column out, which is what you want for output you intend to
compare: addresses move between runs under ASLR.

```text
Stack trace:
  #0  levelC() + 0xd at /home/me/trace.spice:2
  #1  levelB() + 0xd at /home/me/trace.spice:6
  #2  main + 0xc at /home/me/trace.spice:10
```

### Hiding frames that are not Spice code

Not every frame belongs to your program: the C runtime below `main` shows up in every trace, and so does any C
function that calls back into Spice, such as `qsort` calling your comparison function. Pass `true` as the second
argument to leave those frames out:

```spice
sDumpStacktrace(true, true);
```

```text
Stack trace:
  #0  0x000055a3f1c012c3  levelC() + 0xd at /home/me/trace.spice:2
  #1  0x000055a3f1c012d3  levelB() + 0xd at /home/me/trace.spice:6
  #2  0x000055a3f1c012e3  main + 0xc at /home/me/trace.spice:10
```

The frames that remain keep the number they have in the full trace, so a gap in the numbering shows where frames were
hidden, and each number still matches `trace[i]`.

How a frame is recognized as Spice code is described under [`isSpice`](#working-with-a-trace-as-data) below.

## Working with a trace as data

`sGetStacktrace()` returns a `StackTrace`, which can be looped over frame by frame:

```spice
const StackTrace trace = sGetStacktrace();
foreach const StackTraceEntry& entry : trace {
    printf("%s (%s:%d)\n", entry.functionName, entry.fileName, entry.lineNumber);
}
```

The frame number is available as the loop index, exactly as `dump()` prints it:

```spice
foreach unsigned long frameNumber, const StackTraceEntry& entry : trace {
    printf("#%lu %s\n", frameNumber, entry.functionName);
}
```

| Field          | Type            | Meaning                                                                    |
|----------------|-----------------|----------------------------------------------------------------------------|
| `address`      | `byte*`         | Return address within the function                                         |
| `offset`       | `unsigned long` | Bytes from the start of that function, `0` if unresolved                   |
| `functionName` | `String`        | Demangled name, empty if the symbol could not be resolved                  |
| `fileName`     | `String`        | Source file, empty without debug info                                      |
| `lineNumber`   | `unsigned int`  | Line within that file, `0` if unknown                                      |
| `isSpice`      | `bool`          | Whether the frame is Spice code, as opposed to C, C++ or other native code |

`isSpice` is decided from what the trace knows about the frame:

- With debug info, the source file decides: a frame is Spice code if and only if its file ends in `.spice`.
- Without it, the symbol name does. Spice mangles the name of every function except `main`, and a name that reads as
  a Spice mangling, or is `main`, counts as Spice. Plain C symbols, and the mangled names of other languages, do not.
- A frame whose symbol could not be resolved - which is every frame of an executable built with
  [`--strip-symbols`](../cli/build.md) - has nothing to go by and is not Spice.

A function opted out of mangling with `#[core.compiler.mangle = false]` looks like a C function without debug info, and
is treated like one.

`StackTrace` also offers `getSize()`, `isEmpty()`, indexing via `trace[i]` or `getEntry(i)`, and
`dump(bool includeAddresses, bool hideNonSpiceFrames)` for the whole trace. A trace holds at most
`STACK_TRACE_CAPACITY` (128) frames; anything deeper is dropped, since the frames nearest the capture point are the
interesting ones.

To capture the stack on behalf of a caller - from a logging helper, say, whose own frame should not show up -
build a `StackTrace` yourself and tell `capture()` how many frames to skip:

```spice
import "std/runtime/stack_trace_rt";

p logWithTrace(string message) {
    StackTrace trace;
    trace.capture(1); // hide logWithTrace's own frame
    printf("%s\n", message);
    trace.dump();
}
```

## What you need for symbols

Resolving an address to a name happens at run time, against the executable's own symbol table and debug info, so
what ends up in the binary decides how much of a trace is readable:

- **Names and offsets** come from the symbol table, which every executable keeps unless you build it with
  [`--strip-symbols`](../cli/build.md). Nothing else is needed. In a stripped executable every frame prints as
  `<unknown>`, so strip only what you do not expect to read a stack trace from.
- **File names and line numbers** come from debug info, so they only appear in a build made with
  [`-g`](../cli/build.md).

The work itself is done by [libbacktrace](https://github.com/ianlancetaylor/libbacktrace), which the linker
pulls in as `-lbacktrace`. GCC builds it as part of its own runtime, so where a program is linked through GCC -
Linux, most of the time - it is already there and nothing needs doing. Elsewhere, it may have to be pointed at,
and linking a program that takes a stack trace otherwise fails with an undefined reference to
`backtrace_create_state`.

**Linux with Clang** resolves it too, since Clang searches GCC's directories.

**Windows**: some MinGW-w64 distributions ship `libbacktrace.a` alongside GCC and some do not, and Clang's GNU
driver does not search GCC's own library directory either way. Ask GCC whether it has one - it echoes the bare
file name back when it does not - and put the directory on `LIBRARY_PATH`, which Clang reads for `-l` search
dirs:

```powershell
$lib = gcc -print-file-name=libbacktrace.a
if (Test-Path $lib) { $env:LIBRARY_PATH = "$(Split-Path -Parent $lib);$env:LIBRARY_PATH" }
```

If that comes up empty - as it does on the GitHub Actions runner, which is why this repository's CI builds its
own - build the library in an [MSYS2](https://www.msys2.org) MINGW64 shell and point `LIBRARY_PATH` at the
result:

```sh
pacman -S --needed git make mingw-w64-x86_64-gcc
git clone https://github.com/ianlancetaylor/libbacktrace.git
cd libbacktrace && ./configure --prefix=/c/libbacktrace --disable-shared && make && make install
```

```powershell
$env:LIBRARY_PATH = "C:\libbacktrace\lib;$env:LIBRARY_PATH"
```

**macOS** has none at all: the Apple toolchain does not include libbacktrace, and Homebrew has no formula for it.
Either install the MacPorts port (`sudo port install libbacktrace`) or build it yourself:

```sh
git clone https://github.com/ianlancetaylor/libbacktrace.git
cd libbacktrace && ./configure --prefix="$HOME/.local" --disable-shared && make && make install
export LIBRARY_PATH="$HOME/.local/lib:$LIBRARY_PATH"
```

## Limitations

- In an optimized build a function that was inlined into another still gets a frame of its own, recovered from
  the debug info, but its offset is counted from the start of the function it was inlined into. Without debug
  info, inlined functions do not appear at all - only the physical frames do.
- `spice build -static` leaves no dynamic symbol information behind for shared-library frames, so frames outside
  the executable itself resolve less well.
- On Windows, a frame in a system DLL that carries no symbols of its own can come back named after a function in
  a different module, with a nonsensical offset. libbacktrace searches each loaded module's symbol table in turn
  and treats the last entry of one as covering every address above it, so a lookup that finds nothing where it
  should can still match something where it should not. Frames in your own executable are unaffected; the ones
  below `main` are the ones to distrust.

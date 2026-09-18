---
title: Stack traces
---

Spice can print the call stack of a running program, or hand it to you as data. Two functions from the stack
trace runtime do this; like the other `s`-prefixed runtime functions they are auto-imported, so no `import` is
needed:

- `sDumpStacktrace(bool includeAddresses = true)` prints the calling function's stack to stderr.
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
    return 0;
}
```

Built with [`-g`](../cli/build.md), this prints something like:

```text
Stack trace:
  0x000055a3f1c012c3  levelC() + 0xd at /home/me/trace.spice:2
  0x000055a3f1c012d3  levelB() + 0xd at /home/me/trace.spice:6
  0x000055a3f1c012e3  main + 0xc at /home/me/trace.spice:10
  0x00007f2c9ba2a1c9  __libc_start_call_main + 0x7a
  0x00007f2c9ba2a28a  __libc_start_main_impl + 0x8a
  0x000055a3f1c011b4  _start + 0x24
```

One line per frame, most recent call first, each with:

1. the address the frame will return to,
2. the demangled function name, followed by `+` and the byte offset from where that function starts,
3. `at <file>:<line>`, when the program carries debug info.

Frames below `main` belong to the C runtime and differ per platform. A frame whose symbol could not be resolved
prints as `<unknown>`; a frame in a program without debug info simply has no `at ...` part.

Pass `false` to leave the address column out, which is what you want for output you intend to compare: addresses
move between runs under ASLR.

```text
Stack trace:
  levelC() + 0xd at /home/me/trace.spice:2
  levelB() + 0xd at /home/me/trace.spice:6
  main + 0xc at /home/me/trace.spice:10
```

## Working with a trace as data

`sGetStacktrace()` returns a `StackTrace`, which is a list of `StackTraceEntry` values:

```spice
const StackTrace trace = sGetStacktrace();
for long i = 0l; i < trace.getSize(); i++ {
    const StackTraceEntry& entry = trace.getEntry(i);
    printf("%s (%s:%d)\n", entry.functionName.getRaw(), entry.fileName.getRaw(), entry.lineNumber);
}
```

| Field | Type | Meaning |
| --- | --- | --- |
| `address` | `byte*` | Return address within the function |
| `offset` | `unsigned long` | Bytes from the start of that function, `0` if unresolved |
| `functionName` | `String` | Demangled name, empty if the symbol could not be resolved |
| `fileName` | `String` | Source file, empty without debug info |
| `lineNumber` | `int` | Line within that file, `0` if unknown |

`StackTrace` also offers `getSize()`, `isEmpty()`, indexing via `trace[i]`, and `dump(bool includeAddresses)` for
the whole trace. A trace holds at most `STACK_TRACE_CAPACITY` (64) frames; anything deeper is dropped, since the
frames nearest the capture point are the interesting ones.

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
pulls in as `-lbacktrace`. GCC ships it as part of its own runtime, so it is present on a stock Linux or MinGW
toolchain and Clang finds it there too. On a system where it is missing, linking a program that takes a stack
trace fails with an undefined reference to `backtrace_create_state`; install your distribution's `libbacktrace`
package to fix that.

## Limitations

- In an optimized build a function that was inlined into another still gets a frame of its own, recovered from
  the debug info, but its offset is counted from the start of the function it was inlined into. Without debug
  info, inlined functions do not appear at all - only the physical frames do.
- `spice build -static` leaves no dynamic symbol information behind for shared-library frames, so frames outside
  the executable itself resolve less well.

# Stack Traces in Spice

Spice programs can capture their call stack as data (`sGetStacktrace()`) or print it (`sDumpStacktrace()`).
Capture and symbol resolution are done by [libbacktrace](https://github.com/ianlancetaylor/libbacktrace), behind one
shared runtime with no per-platform variants. User-facing documentation: `docs/docs/how-to/stack-traces.md`.

| File                                  | Role                                                                 |
|---------------------------------------|----------------------------------------------------------------------|
| `std/runtime/stack_trace_rt.spice`    | Public API, frame classification, printing                           |
| `std/runtime/stack-trace-backtrace.c` | C shim over libbacktrace, linked via `core.linker.additionalSource`  |
| `std/text/demangle.spice`             | Demangler for Spice's name mangling (`media/specs/name-mangling.md`) |

`sGetStacktrace()` and `sDumpStacktrace()` are auto-imported by name (`RuntimeModuleManager`) and can also be imported
explicitly. Only the explicit path applies OS-suffixing to module names, so an OS-suffixed variant of
`stack_trace_rt.spice` could be loaded by one path and not the other and collide at link time. The file therefore has
none, and must not gain any.

## API

```
sGetStacktrace() -> StackTrace
sDumpStacktrace(bool includeAddresses = true, bool hideNonSpiceFrames = false)
```

`StackTraceEntry` describes one frame:

| Field          | Type            | Meaning                                                          |
|----------------|-----------------|------------------------------------------------------------------|
| `address`      | `byte*`         | Return address within the function                               |
| `offset`       | `unsigned long` | Bytes from the start of that function, `0` if unresolved         |
| `functionName` | `String`        | Demangled, empty if unresolved                                   |
| `fileName`     | `String`        | Source file, empty without debug info                            |
| `lineNumber`   | `unsigned int`  | Line within that file, `0` if unknown                            |
| `isSpice`      | `bool`          | Spice code, as opposed to C, C++ or other native code            |

`StackTrace` holds a `Vector<StackTraceEntry>`, most recent call first, capped at `STACK_TRACE_CAPACITY` (128) frames;
deeper frames are dropped. It offers `getSize()`, `isEmpty()`, `getEntry(i)`, `trace[i]`,
`capture(unsigned int skipFrames = 0u)` and `dump(bool includeAddresses, bool hideNonSpiceFrames)`.
`capture()` first clears any previous content.

`StackTrace` implements `IIterable<StackTraceEntry>` through `StackTraceIterator<StackTraceEntry>`, so it works with
`foreach`, with or without the frame number. The item type is a generic argument only because
`TypeChecker::visitForeachLoop()` rejects iterators without generic arguments (pinned by
`typechecker/foreach-loops/error-foreach-non-generic-iterator`).

### Output format

```
  #<n>  <address>  <name> + 0x<offset> at <file>:<line>
```

- `<n>` is left-aligned in a field of three, so the columns line up for every index a trace can reach.
- `<address>` is printed with `%p`, whose format is platform-specific, and is left out for `includeAddresses = false`.
  That is the deterministic output, since addresses move under ASLR/PIE.
- An unresolved frame prints `<unknown>` without an offset. `at <file>:<line>` is left out without a file name.

## Capture and symbolization

The shim exports two functions:

```c
int  spiceCaptureStackTrace(SpiceStackFrame *frames, int capacity, int skipNativeFrames, int skipEntries);
void spiceReleaseStackTrace(SpiceStackFrame *frames, int count);
```

A shim is needed because every libbacktrace entry point takes a callback, and a Spice function converted to a raw
pointer becomes a `.fatthunk` with an extra capture argument, which C callers do not pass (spicelang/spice#1392).

- `SpiceStackFrame` mirrors `NativeStackFrame` in `stack_trace_rt.spice` field for field. Its name pointers are heap
  copies, because libbacktrace's strings may be invalid after the callback returns. `capture()` copies them into
  `String`s and hands the originals back through `spiceReleaseStackTrace()`.
- **Unwinding** uses the unwind tables that `IRGenerator` emits for every function (`uwtable`), so no frame pointers
  are needed and every optimization level works.
- **Resolution:** `backtrace_full()` yields function, file and line, plus one entry per function inlined into the
  frame. It does not report where the function starts, so a `backtrace_syminfo()` lookup per frame supplies the offset,
  and the name when there is no debug info. For an inlined frame the offset counts from the function it was inlined
  into. If `backtrace_full()` reports nothing, because it cannot open the executable, a `backtrace_simple()` fallback
  keeps the bare addresses.
- **Skipping:** the runtime's own frames are dropped as physical frames (`skipNativeFrames`, always `RUNTIME_FRAMES` = 2:
  the shim and `capture()`). The caller's `skipFrames` counts entries (`skipEntries`), since debug info can expand one
  physical frame into several entries. `sGetStacktrace()` and `sDumpStacktrace()` skip one to hide themselves.
  `RUNTIME_FRAMES` is exact because the shim and `capture()` sit in separate object files and cannot be inlined into the
  caller; `-lto` lifts that guarantee.
- **State:** one process-wide `backtrace_state`, created on first use with the threaded flag, published by
  compare-exchange and never freed. libbacktrace's errors are dropped, as they only mean missing information, which
  already shows as an unresolved frame.
- **Windows:** libbacktrace builds the PE symbol table at the image base recorded in the module's file, but the loader
  relocates the module under ASLR. The shim subtracts the module's relocation distance from the address before the
  `backtrace_syminfo()` lookup, so names and offsets resolve. The frame keeps its real address.
- **Demangling:** `capture()` runs each name through `demangle()`. It reads exactly what the compiler emits, including
  function types (`PF...E`), `.fatthunk` suffixes and RTTI symbols, and returns anything else unchanged.

## Spice frames vs native frames

`isSpice` is inferred in `capture()`, because a compiled Spice function carries no marker a run-time lookup could read:

1. **File, when the program has debug info:** Spice exactly when the file ends in `.spice`. This is checked first and
   settles the frame either way. It must be an extension test, because some C libraries ship debug info too
   (`../csu/libc-start.c`).
2. **Otherwise the symbol name:** Spice if it is `main` (the one unmangled Spice function) or the demangler changed it.
   This looks at the raw symbol, before demangling.
3. **Neither:** not Spice. This includes every frame of a stripped executable.

`hideNonSpiceFrames` leaves out frames that are not Spice. The remaining frames keep their position in the trace, so a
gap in the numbering marks hidden frames and each number still matches `getEntry()`.

Blind spots, all without debug info: a Spice function with `#[core.compiler.mangle = false]` reads as C, and a C++
symbol the demangler happens to accept reads as Spice.

## Symbols and debug info

- **Names and offsets** come from the executable's symbol table, which is kept by default. `--strip-symbols` adds
  `-Wl,-s` (not on Darwin, whose linker has no equivalent) and leaves every frame `<unknown>`.
- **File and line** come from debug line info: `--debug-info=line-only` is enough, `-g` / `--debug-info=full` also works.
- `main` prints as `main` from the symbol table and as `main()` from full DWARF. Cosmetic; no other function is affected.

## Linking libbacktrace

`stack_trace_rt.spice` links it with `core.linker.flag = "-lbacktrace"`. Linker flags are appended behind the object
files, so a `-l` naming a static archive resolves.

- **Linux:** GCC ships `libbacktrace.a` inside its own library directory, and Clang finds it there too.
- **macOS:** the Apple toolchain has none and Homebrew has no formula. Use MacPorts or build it from source.
- **Windows:** MinGW-w64 GCC may or may not ship one, and Clang does not search GCC's library directory either way.
  Point `LIBRARY_PATH` at a copy, or build it in MSYS2.
- **CI** builds it from the pinned commit `LIBBACKTRACE_VERSION` on macOS and Windows and caches it.

Vendoring the sources into `std/` would remove this requirement on all platforms, at the cost of about 17k lines of
third-party C in the tree and a little link time.

## Limitations

- Without debug info, inlined functions do not appear; only physical frames do. With it, they are recovered.
- `spice build -static` resolves the executable's own frames, but frames of code that would have come from a shared
  library are not attributable to one.
- On Windows, a frame in a system DLL without symbols can come back named after a function in another module, because
  libbacktrace treats the last entry of a symbol table as covering every address above it. Frames in the program's own
  executable are unaffected; the ones below `main` are not.

## Tests

- `std/runtime/stack-trace-capture-basic`: invariants that hold on every platform and optimization level - skipping `n`
  frames drops exactly `n`, offset and line number only appear with their name and file, `isSpice` on frame 0 and the
  bottom of the stack. Frame counts are not asserted, as they depend on the C runtime and on inlining.
- `std/runtime/stack-trace-dump-basic`: dumps with and without addresses and with native frames hidden, printed back
  with addresses and offsets masked. Only the three frames the test owns are compared, as everything below `main` is
  C runtime.
- `std/runtime/stack-trace-native-frames`: `qsort` calling back into Spice, asserting the Spice, native, Spice, native
  shape of the stack. Skipped on Windows because of the misattribution above.
- `std/text/demangle`: demangler cases seeded from the mangled names in the `.ll` reference files.

# Stack Traces in Spice

Spice programs can capture their call stack as data (`sGetStacktrace()`) or print it (`sDumpStacktrace()`).
Capture and symbol resolution are done by [libbacktrace](https://github.com/ianlancetaylor/libbacktrace), behind one
shared runtime with no per-platform variants. User-facing documentation: `docs/docs/how-to/stack-traces.md`.

| File                                                 | Role                                                                 |
|------------------------------------------------------|----------------------------------------------------------------------|
| `std/runtime/stack_trace_rt.spice`                   | Public API, frame classification, printing                           |
| `std/runtime/impl/stack_trace_native.spice`          | Capture and symbol resolution, calling libbacktrace directly         |
| `std/runtime/impl/stack_trace_address.spice`         | Address translation for symbol lookups: the identity                 |
| `std/runtime/impl/stack_trace_address_windows.spice` | The same for Windows, undoing the loader's relocation of a module    |
| `std/runtime/impl/stack_trace_lock.spice`            | Serializes captures: nothing to do                                   |
| `std/runtime/impl/stack_trace_lock_windows.spice`    | The same for Windows, which needs it, see below                      |
| `std/text/demangle.spice`                            | Demangler for Spice's name mangling (`media/specs/name-mangling.md`) |
| `deps/libbacktrace`                                  | Vendored libbacktrace, as a git submodule                            |
| `setup-deps.py`                                      | Builds it, emitting `std/runtime/lib/libbacktrace.a`                 |

Everything under `std/runtime/impl/` is internal to `stack_trace_rt.spice` and not meant to be imported directly.

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

## Panics and failed assertions

The compiler calls `sDumpStacktrace()` (`stack_trace_rt.spice`) with its default arguments - addresses on, no frames
hidden - right before the `exit()` of a `panic`, of a failed `assert` and of a read of an inactive union field. Like
`sErrTraceDump()`, it is reached through `StdFunctionManager::getDumpStacktraceFct()`, which rebuilds its mangled name.
In a `panic` the call comes after the error return trace and before the scope cleanup, so the trace shows the panicking
function rather than the cleanup.

- **When:** `CliOptions::printsStackTraceOnAbort()`, i.e. for a native target and not for `comparableOutput`. The first
  is because `libbacktrace.a` is offered to the linker for a native target only (see
  [Linking libbacktrace](#linking-libbacktrace)); with a cross target the call is not emitted, so nothing is pulled in
  that could not be linked. The second is the test runner, whose outputs are compared across platforms and whose
  reference IR would otherwise change in every one of the 100+ tests using `assert`. A trace holds addresses and offsets
  that differ per platform, and everything below `main` is the C runtime. The runner therefore never observes the
  calls, and the wiring is checked by hand, with `spice run`.
- **Loading the runtime:** the type checker requests `STACK_TRACE_RT` for a file at every `panic`, assert statement (not
  in release builds, which do not generate assertions) and union member access, as it does for other runtime modules.
  `assert` needs this: `panic` only got `error_trace_rt` because `Error` pulls it in. The runtime needs `String`,
  `Vector` and the iterators, which use `panic` and `assert` themselves, so the requests form an import cycle. The
  compiler handles circular imports, and a panic in `Vector.get()` prints its trace like any other.
- **Re-entrancy:** capturing and printing use the std, and a panic raised there (`String` running out of memory, say)
  would call `sDumpStacktrace()` again, and again until the stack is gone. A global flag, set while a dump is under way
  and cleared when it ends, makes a call it causes return, so the panic only ends the program, as it did before traces.
  It has to be cleared, unlike a one-shot flag, because `sDumpStacktrace()` is also called by programs, repeatedly. The
  flag is a plain `bool` since there are no atomics (see [What Spice cannot express](#what-spice-cannot-express)), so
  two threads dumping at the same time may cost one of them its trace, or let a nested panic through. No test can reach
  it: a nested call only comes from a panic inside the trace machinery.
- **Output order:** a `panic` and a failed `assert` both print their message and then the trace to stderr, in that order.
  The assert message used to go to stdout (`printf`), which put the trace ahead of it whenever stdout was redirected.
  The message is the format string of an `fprintf`, so `%` in the source text of the condition is escaped to `%%`
  (`assert x %d == 0` is valid Spice). What the program wrote to stdout before is still block-buffered when stdout is
  redirected, so a stream that merges both shows it after the failure, as it does for a `panic`.

## Capture and symbolization

`impl/stack_trace_native.spice` exports two functions and the frame type they fill:

```spice
public f<unsigned int> captureNativeFrames(NativeStackFrame* frames, unsigned int capacity, unsigned int skipNativeFrames, unsigned int skipEntries)
public p releaseNativeFrame(NativeStackFrame& frame)
```

It is written in Spice and calls libbacktrace directly; it used to be a C file linked through
`core.linker.additionalSource`, because a Spice function converted to a raw pointer became a `.fatthunk` with a
*leading* capture argument, which shifted the arguments of a C caller (spicelang/spice#1392). The capture pointer is a
trailing argument since #1396, which a C caller does not pass and the thunk ignores, so Spice callbacks work. See
[Calling libbacktrace from Spice](#calling-libbacktrace-from-spice) for how they have to be declared.

- `NativeStackFrame` holds the raw values of one frame. Its name pointers are heap copies, because libbacktrace's
  strings may be invalid after the callback returns. `capture()` copies them into `String`s and hands the originals
  back through `releaseNativeFrame()`, one frame at a time.
- **Unwinding** uses the unwind tables that `IRGenerator` emits for every function (`uwtable`), so no frame pointers
  are needed and every optimization level works.
- **Resolution:** `backtrace_full()` yields function, file and line, plus one entry per function inlined into the
  frame. It does not report where the function starts, so a `backtrace_syminfo()` lookup per frame supplies the offset,
  and the name when there is no debug info. For an inlined frame the offset counts from the function it was inlined
  into. If `backtrace_full()` reports nothing, because it cannot open the executable, a `backtrace_simple()` fallback
  keeps the bare addresses.
- **Skipping:** the runtime's own frames are dropped as physical frames (`skipNativeFrames`, always `RUNTIME_FRAMES` = 2:
  `captureNativeFrames()` and `capture()`). The caller's `skipFrames` counts entries (`skipEntries`), since debug info can expand one
  physical frame into several entries. `sGetStacktrace()` and `sDumpStacktrace()` skip one to hide themselves.
  `RUNTIME_FRAMES` is exact because `impl/stack_trace_native.spice` and `capture()` sit in separate object files and cannot
  be inlined into the caller; `-lto` lifts that guarantee. This is why the native code is a file of its own.
- **State:** one process-wide `backtrace_state`, created on first use with the threaded flag and never freed.
  libbacktrace's errors are dropped, as they only mean missing information, which already shows as an unresolved frame.
  It is published with a plain load and store - see [What Spice cannot express](#what-spice-cannot-express) for what
  that costs.
- **Windows:** libbacktrace builds the PE symbol table at the image base recorded in the module's file, but the loader
  relocates the module under ASLR. `impl/stack_trace_address_windows.spice`, which the compiler picks there by its OS
  suffix, subtracts the module's relocation distance from the address before the `backtrace_syminfo()` lookup, so names
  and offsets resolve. The frame keeps its real address. The distance is kept in the capture's own state for the module
  of the last frame, so that consecutive frames in one module do not reread its file. Other platforms get
  `impl/stack_trace_address.spice`, which returns the address as is.
- **Windows, locking:** captures are serialized there by an `SRWLOCK` (`impl/stack_trace_lock_windows.spice`), which is
  held from creating the state to the end of the walk. Several threads taking their first stack trace at the same time
  crash the process on Windows, while a first trace on one thread followed by concurrent ones does not, as shown by
  `stack-trace-concurrent-capture` on CI. libbacktrace's initialization there reads every loaded module and registers a
  DLL notification, but what exactly fails was not tracked down. An `SRWLOCK` is one pointer that starts as zero, so it
  fits in a global of primitive type, which a `Mutex` does not. Other platforms take no lock.
- **Demangling:** `capture()` runs each name through `demangle()`. It reads exactly what the compiler emits, including
  function types (`PF...E`), `.fatthunk` suffixes and RTTI symbols, and returns anything else unchanged.

## Calling libbacktrace from Spice

- **Callbacks are declared `byte*`.** A parameter of function type is a fat pointer (`{ptr, ptr, i64}`) and reaches a C
  function as three separate arguments, shifting every argument behind it. Every libbacktrace entry point takes its
  callbacks in the middle of its parameter list, so each is declared as `byte*` and given `cast<byte*>(function)`, which
  yields the function's `.fatthunk`. (`qsort` in `stack-trace-native-frames` gets away with a function-typed parameter
  only because the callback is its last one, so the surplus words land in registers it never reads.)
- **Callback data is a `byte*`** that the callback casts back to its state struct.
- **Opaque handles are `unsigned long`.** They pass exactly like pointers, which saves a cast at every use: the
  `backtrace_state` here, and the `HMODULE` and `HANDLE`s of the Windows module. The state also has to be an integer
  because Spice only supports global variables of primitive type.
- **`unsafe` is kept to single statements:** the pointer casts of the callback data and callbacks, and the subscript of
  the frame buffer. `strdup` and `free` are declared with `string`, which needs no cast.
- **`unsigned long - unsigned long` is `long`,** so the Windows module casts differences back to `unsigned long`.

## What Spice cannot express

Spice has no atomic operations: no atomic load, store or compare-exchange, and `std/os/atomic.spice` is a mutex around
a plain value. The C implementation published the `backtrace_state` with a compare-exchange and read it with an acquire
load. The Spice one is a plain load and store of `BACKTRACE_STATE`, which changes two things for a program whose first
stack trace is taken by several threads at once:

- On a weakly ordered CPU such as AArch64, the pointer can in theory be seen before the writes that initialized the
  state behind it. On x86-64 stores are not reordered like that, and a race of 16 threads over 600 runs found nothing.
  Both threads creating a state is harmless, since one is simply dropped.
- ThreadSanitizer sees the race, which it could not see in the uninstrumented C. A race report raised while another
  thread is inside libbacktrace's first-use initialization can deadlock inside ThreadSanitizer itself, between its
  symbolizer and libbacktrace's `dl_iterate_phdr` call: a multi-threaded program taking its first stack trace on several
  threads under `--sanitizer=thread` hung in 11 of 12 runs, against none with the C implementation. Taking one stack
  trace before starting the threads avoids it.

On Windows the capture lock above rules both out. Elsewhere, neither a `Mutex` nor `pthread_once` can stand in: a global cannot be a struct, and the storage a `pthread_once_t` needs
differs per platform. Closing the gap needs atomic loads and stores in the language, at which point `obtainState()` is
the only place to change.

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

libbacktrace is vendored as a git submodule in `deps/libbacktrace` and built by `setup-deps.py`, into
`std/runtime/lib/libbacktrace.a` - inside the std tree, next to the runtime sources that need it.

`stack_trace_rt.spice` links it with `core.linker.flag = "-lbacktrace"`. Linker flags are appended behind the object
files, so a `-l` naming a static archive resolves. `ExternalLinkerInterface::link()` puts `std/runtime/lib` on the
linker's search path ahead of those flags, so `-lbacktrace` finds the bundled archive; search directories are tried in
the order given, which keeps the std's own copy ahead of any directory a binding's `-L` flag adds.

### Releasing it

The archive is a host binary, so the release pipeline cannot build one copy centrally: `build-artifacts` packages
`std/` from a plain checkout and has no compiler of its own. Each `publish.yml` build job therefore uploads the
`libbacktrace.a` it built next to its own compiler binary, and the packaging step places the matching one - never
another platform's - into each output:

| Output                      | How it gets there                                                                  |
|-----------------------------|------------------------------------------------------------------------------------|
| Archives (tar.gz / zip)     | goreleaser `archives.files`, `src: bin/spice-{{ .Os }}-{{ .Arch }}/libbacktrace.a` |
| deb / rpm / apk / archlinux | nfpm `contents`, `src: bin/spice-linux-{{ .Arch }}/libbacktrace.a`, mode `0644`    |
| Container image             | `docker-libs/<os>/<arch>/` staged by the workflow, picked by `$TARGETPLATFORM`     |
| Windows MSI, Homebrew cask  | Nothing to do - both are built from the archive above                              |

`build-artifacts` also deletes `std/runtime/lib` before packaging: on a clean runner there is none, but a stale one
would be copied into every platform's output by the `std` entry, alongside the matching copy.

libbacktrace is BSD-3-Clause licensed, which requires its copyright notice and license text to accompany binary
redistributions. Its `LICENSE` is therefore shipped as `std/runtime/lib/LICENSE`, right next to the archive, through
the same four routes. Unlike the archive it is identical on every platform, so it is not uploaded by the build jobs:
`build-artifacts` initializes just the `deps/libbacktrace` submodule (the only one it needs) and the archives and
packages take `deps/libbacktrace/LICENSE` from there, while the container build gets a copy staged into each
`docker-libs/<os>/<arch>/` directory, which the Dockerfile copies over as a whole.

Vendoring replaced the previous arrangement, under which each platform had to supply the library: Linux got it from
GCC's own runtime directory, while macOS (no system copy, no Homebrew formula) and Windows (MinGW-w64 may or may not
ship one, and it cannot be built with MSVC at all) needed a source build that CI cached. The cost is about 17k lines
of third-party C in the tree; the gain is that every platform links the same known-good library.

The archive is built for the host, so it is only offered when `cliOptions.isNativeTarget` says the target is that
same host. Cross-compiling a Spice program that takes a stack trace therefore still needs a libbacktrace built for
the target, reachable through the toolchain's own search path - as it did before vendoring. Offering the host copy
regardless would not help and would actively hurt: lld rejects every member of a mismatched archive
(`is incompatible with aarch64linux`) rather than passing over it the way GNU ld does.

Upstream builds with autotools, and `setup-deps.py` runs that build rather than reimplementing it: `./configure`
alone decides which object-format reader to compile (`elf.c`, `macho.c`, `pecoff.c`, `xcoff.c` or `unknown.c`),
whether `mmap` backs the file reader and allocator, and what goes into `config.h` and `backtrace-supported.h`. A
submodule bump therefore needs no work on our side. It is built out of tree, into `deps/libbacktrace-build`, so the
submodule checkout stays pristine, and from scratch each time, so a bump re-runs configure instead of reusing the
previous commit's cached answers. A stamp file next to the build records the commit it came from, so a repeat run of
`setup-deps.py` is a no-op.

Two configure flags are not optional:

- `--with-pic`, because the archive is linked into Spice programs, which may themselves be shared libraries
  (`--output-container=shared`); without it the link fails outright with a `R_X86_64_PC32` relocation error.
- `CFLAGS=-O2`, which drops the `-g` half of autotools' default `-g -O2`. libbacktrace's own debug info is dead
  weight in every Spice program that links it - about 300 KB each.

The cost of using upstream's build is a POSIX shell and `make`. Every platform but Windows has both; there,
`setup-deps.py` looks for MSYS2 (preferring the shell next to `make`, since Git for Windows supplies an `sh` but no
`make`), and the Windows CI jobs install it before running the script.

## Limitations

- Without debug info, inlined functions do not appear; only physical frames do. With it, they are recovered.
- `spice build -static` resolves the executable's own frames, but frames of code that would have come from a shared
  library are not attributable to one.
- On Windows, a frame in a system DLL without symbols can come back named after a function in another module, because
  libbacktrace treats the last entry of a symbol table as covering every address above it. Frames in the program's own
  executable are unaffected; the ones below `main` are not.

## Tests

- `std/runtime/stack-trace-capture-basic`: invariants that hold on every platform and optimization level - skipping `n`
  frames drops exactly `n`, offset and line number only appear with their name and file, `isSpice` agrees with the file
  name and is set on frame 0. Frame counts are not asserted, as they depend on the C runtime and on inlining.
- `std/runtime/stack-trace-dump-basic`: dumps with and without addresses and with native frames hidden, printed back
  with addresses and offsets masked. Only the three frames the test owns are compared, as everything below `main` is
  C runtime.
- `std/runtime/stack-trace-concurrent-capture`: sixteen threads taking the first stack trace of the process at once, each
  asserting it got a trace with at least one resolved name. Every run is a fresh process, so every run races the state's
  creation.
- `std/runtime/stack-trace-native-frames`: `qsort` calling back into Spice, asserting that the stack leaves Spice for
  `qsort` and returns to Spice in `main`. What sits below `main` is not asserted, as some unwinders stop there. Skipped
  on Windows because of the misattribution above.
- `std/text/demangle`: demangler cases seeded from the mangled names in the `.ll` reference files.

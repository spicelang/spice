# Cross-Platform Stack Traces in Spice

## Status

Implemented, on top of [libbacktrace](https://github.com/ianlancetaylor/libbacktrace). One shared
`std/runtime/stack_trace_rt.spice` plus one C shim, `std/runtime/stack-trace-backtrace.c`, with no per-platform
variants of either.

Output, without debug info:

```
Stack trace:
  #0  0x00005f2c1d4a12c3  levelC() + 0xd
  #1  0x00005f2c1d4a12d3  levelB() + 0xd
  #2  0x00005f2c1d4a12e3  main + 0xd
```

and with it, the same plus the source location each frame came from:

```
Stack trace:
  #0  0x00005f2c1d4a12c3  levelC() + 0xd at /home/me/trace.spice:2
  #1  0x00005f2c1d4a12d3  levelB() + 0xd at /home/me/trace.spice:6
  #2  0x00005f2c1d4a12e3  main() + 0xd at /home/me/trace.spice:10
```

i.e. per frame: **frame number**, **frame address**, **demangled** function name, **hex offset from the symbol
start**, and - when the program carries debug info - **file and line**. User-facing documentation lives in
`docs/docs/how-to/stack-traces.md`.

## Background: what the hand-written implementation got wrong

The first implementation captured frames with a hand-rolled frame-pointer walk and symbolized them with
`dladdr()` on POSIX and DbgHelp on Windows. On the four CI targets (`linux-amd64`, `linux-aarch64`,
`windows-amd64` (MinGW), `darwin-arm64`) that produced three different results, none of them the intended one.
The measurements below are why it was replaced; they are kept because they are the reason the current design
looks the way it does.

### 1. Frame-pointer walking cannot be made equivalent

`capture()` read `frame[0]` as the caller's saved frame pointer and `frame[1]` as the return address. That layout
is a System V / AAPCS convention. It does not hold everywhere, and it does not hold at all once frame pointers
are omitted:

- **Windows x64 has no frame-pointer chain at all.** Its prologue is `push rbp; sub rsp, N; lea rbp, [rsp+N]`,
  so `rbp` points into the *middle* of the frame, not at the saved `rbp`. `frame[0]`/`frame[1]` are ordinary
  locals. Windows x64 unwinds from `.pdata`/`.xdata` tables instead. Verified with
  `llc -O2 -mtriple=x86_64-pc-windows-msvc`.
- **The frame pointer was not actually enabled for Spice functions on any target.** `IRGenerator` called
  `module->setFramePointer(llvm::FramePointerKind::All)`, which sets the `"frame-pointer"` *module flag*. The
  LLVM backend does not read that flag — it reads the per-function `"frame-pointer"="all"` **attribute**.
  Verified with `llc` on minimal IR carrying only the module flag:

  | target | module flag only | with `"frame-pointer"="all"` fn attr |
  | --- | --- | --- |
  | `x86_64-unknown-linux-gnu` (`-O0` *and* `-O2`) | `pushq %rax` — no FP | `pushq %rbp; movq %rsp, %rbp` |
  | `arm64-apple-darwin` `-O2` | `stp x29, x30, [sp, #-16]!` — record spilled, **`x29` never updated** | same `stp`, plus `mov x29, sp` |
  | `x86_64-pc-windows-msvc` `-O2` | — | `pushq %rbp; subq $32, %rsp; leaq 32(%rsp), %rbp` (still not a chain) |

- **The Linux result that looked right was garbage.** `llvm.frameaddress` sets
  `MachineFrameInfo::isFrameAddressTaken()`, which forces a frame pointer for `capture()` *only*. Its callers had
  none, so `capture()`'s prologue saved whatever `rbp` happened to hold — a leftover libc value — and the walk
  jumped straight into libc's chain.

Conclusion: the walk had to be replaced by a table-driven unwinder. Spice is set up for this — `IRGenerator.cpp`
puts `uwtable` on every function, so `.eh_frame`/`.pdata` is emitted regardless of the `nounwind` attribute on
the same line.

### 2. `dladdr()` cannot see a Spice program's own symbols on ELF

`dladdr()` searches `.dynsym` only. Spice does not pass `-rdynamic`, so on Linux a Spice program's own functions
were absent from it. Measured on a three-deep C call chain:

| link flags | own frames resolved | libc frames resolved |
| --- | --- | --- |
| default | **no** (`dli_sname == NULL`) | partially |
| `-rdynamic` | **yes**, with exact `dli_saddr` for offsets | yes |
| `-rdynamic -Wl,-s` | **yes** | yes |

`-rdynamic` was measured to export only `public` Spice functions, so it would still have missed everything that
stayed local to its object file — which is why the current design keeps the whole symbol table instead
(see [Symbol availability](#symbol-availability)).

### 3. There was no way to compute an offset in Spice

`OpRuleManager::getCastResultType()` allowed `any* -> any*` in unsafe mode but had no pointer↔integer rule, and
`-` was not defined for `ptr - ptr`, so `address - symbolStart` was inexpressible. Fixed; see
[Work items](#work-items).

### 4. There is no C++ demangler available at runtime

Spice mangles with an Itanium-compatible scheme (`media/specs/name-mangling.md`), so frames would otherwise print
as `_ZN13ArrayIteratorIiE4nextEv`. Checking every mangled name appearing in the committed `.ll` reference files
(330 distinct names) through `c++filt`, **329 demangle correctly**; the sole failure is `_ZN6Thread4ctorEPFvE` —
the `PF...E` function-type encoding that `NameMangling.cpp` already documents as not valid Itanium.

`abi::__cxa_demangle` would therefore mostly work, but it lives in `libstdc++`/`libc++abi`, not libc: linking it
would add a `libstdc++.so.6`/`libc++abi.dylib`/`libstdc++-6.dll` runtime dependency to every Spice program that
touches a stack trace, and it is unavailable on musl-only and MSVC targets. Since Spice *emits* the mangling and
it is a small, closed subset, a Spice-side demangler is the equivalent-everywhere option and additionally covers
the `PF...E` case. It ships as `std/text/demangle`.

### 5. Output formatting was not equivalent

`dump()` printed addresses with `%p`. glibc renders `0x55c8de89c2c3`; MinGW's MSVCRT `%p` renders
`00000000004012C3` (no `0x`, uppercase, zero-padded). Since the test masks the address column anyway, and a
`%p` value is only ever read by a human next to the binary it came from, `%p` was kept.

## Design

### Capture and symbolization — libbacktrace

libbacktrace does both halves in one library and the same way on every platform it supports. It unwinds through
the unwind tables `uwtable` already produces — so it needs no frame pointers and works at every optimization
level — and resolves each program counter against the executable's own debug info and symbol table: DWARF inside
ELF and Mach-O, PDB/COFF inside PE. That is strictly more than `dladdr()`/DbgHelp gave: file and line numbers,
and functions that were inlined away, both of which the previous design listed as out of scope.

It is reached through a C shim, `std/runtime/stack-trace-backtrace.c`, pulled in with
`core.linker.additionalSource`. The shim is needed because every libbacktrace entry point takes a callback, and a
Spice function converted to a raw pointer becomes a `.fatthunk` carrying an extra leading captures pointer, which
shifts every argument (spicelang/spice#1392). It exposes two plain functions:

```c
int  spiceCaptureStackTrace(SpiceStackFrame *frames, int capacity, int skipFrames);
void spiceReleaseStackTrace(SpiceStackFrame *frames, int count);
```

`SpiceStackFrame` mirrors `NativeStackFrame` in `stack_trace_rt.spice` field for field. Its two name pointers are
heap copies: libbacktrace documents the strings it hands to a callback as possibly invalid once the callback
returns, so nothing may be kept by pointer, and the second function hands them back once `capture()` has copied
them into `String`s.

Frames are collected with `backtrace_full()`, which yields the name, file and line, plus one extra entry per
function inlined into the frame. The offset needs a second lookup per frame, `backtrace_syminfo()`, since
`backtrace_full()` reports the name but not where the function starts; that lookup also supplies the name itself
when there was no debug info to take one from. `backtrace_full()` reports nothing at all when it cannot open the
executable, so a `backtrace_simple()` fallback keeps the bare addresses in that case.

libbacktrace's own `<backtrace.h>` is not installed by the toolchains that ship the library — GCC builds it as an
internal support library — so the shim mirrors the handful of declarations it needs rather than including it.

The process-wide `backtrace_state` is created on first use and never freed, which is what libbacktrace prescribes
(it caches the parsed debug info). Creation is guarded by an atomic compare-exchange, and the state carries the
threaded flag so libbacktrace guards its own caches.

### Symbol availability

`ExternalLinkerInterface::prepare()` used to add `-Wl,-s` to every non-Darwin executable built without debug
info, which strips `.symtab` and leaves a run-time symbolizer nothing to resolve against — measured: every frame
of a stripped binary comes back `<unknown>`, including with libbacktrace. Stripping is now off by default and
opt-in through a new `--strip-symbols` build flag, so an ordinary build carries its symbol table and prints
readable traces, and only someone who asked for the smaller binary gives that up.

This is what `-rdynamic` was considered for and is strictly better than it: the whole symbol table covers
file-local functions too, and `.dynsym` does not grow, so `--gc-sections` is not inhibited. It also costs nothing
at run time, and the executable cache key already covers it, since it is an ordinary linker flag.

This flips the default for every executable Spice links, not only those that take a stack trace: an ordinary
build is now larger by whatever its symbol table weighs (measured on the three-frame example below: 140 KB
against 124 KB, so about 13%). That is the price of a readable trace from any program, and `--strip-symbols`
buys it back. Darwin never had the flag applied (its linker has no equivalent of `-Wl,-s`) and is unaffected.

### Linker argument order

`-lbacktrace` is contributed as an ordinary `core.linker.flag`. Linker flags used to be placed *ahead* of the
object files on the linker invoker's command line, which works for every shared library the std bindings link
but not for a static archive: a `-l` naming one is only searched for symbols that are still undefined where it
appears. They are now appended behind the object files instead, which is the conventional order and fixes that
for any static library, not just this one.

### Demangling — in Spice

`std/text/demangle` handles exactly the grammar in `media/specs/name-mangling.md` and hands anything that does
not parse cleanly back unchanged. `capture()` demangles each resolved name before storing it.

### Formatting

```
  #%-3lu%p  %s + %#lx at %s:%d\n
```

with the `at ...` part left out when the frame has no file name, and `<unknown>` in place of the name and offset
for a frame that did not resolve. `includeAddresses = false` suppresses the address column so tests stay
deterministic under ASLR/PIE; the frame number is reproducible and always printed. Left-aligning it in a field of
three keeps the columns behind it lined up for every index a trace can reach, since `STACK_TRACE_CAPACITY` caps
it at `#63`.

### Iteration

`StackTrace` implements `IIterable<StackTraceEntry>` and hands out a `StackTraceIterator<StackTraceEntry>`, so a
trace can be walked with a `foreach` loop, with or without the frame number. It deliberately does not expose the
iterator of the `Vector` it currently stores its frames in, so that storage choice stays its own business.

The item type is a generic argument only because the language requires it: `TypeChecker::visitForeachLoop()`
rejects an iterator whose struct carries no generic arguments, even though it then takes the item type from the
iterator's `get()`/`getIdx()` return type rather than from those arguments. `StackTrace` is std's first
non-generic iterable and so the first type to run into this; `T` is always `StackTraceEntry`. Relaxing the check
looks safe - `iteratorTemplateTypes` is used for nothing else - but
`test-files/typechecker/foreach-loops/error-foreach-non-generic-iterator` pins the current behavior, so it was
left alone.

## Work items

**Compiler (`src/`)**

- [x] Add pointer↔integer casts in unsafe mode: `cast<long>(ptr)` → `ptrtoint`, `cast<byte*>(long)` → `inttoptr`.
      Only `long` is accepted on the integer side, since it is the only Spice integer guaranteed to be 64 bit wide.
- [x] Emit the per-function `"frame-pointer"="all"` attribute, behind `--keep-frame-pointers` (default off).
      Independent of the rest: the unwinder route needs no frame pointer, but `__frame_address()` callers may.
- [x] Correct the `__frame_address()` doc comments in `GenBuiltinFunctions.cpp` / `TypeCheckerBuiltinFunctions.cpp`.
- [x] Symbol stripping turned off by default and moved behind a `--strip-symbols` flag on `spice build`, next to
      the other link-time options.
- [x] Append linker flags behind the object files, so a `-l` naming a static archive resolves.

**Standard library (`std/runtime/`)**

- [x] `stack-trace-backtrace.c` — capture and symbolization through libbacktrace.
- [x] `stack_trace_rt.spice` — the public API: `StackTraceEntry` (address, offset, demangled name, file, line)
      with its own `dump()`, `StackTrace` wrapping a `Vector<StackTraceEntry>`, `StackTraceIterator` for
      `foreach`, and the auto-imported `sGetStacktrace()` / `sDumpStacktrace()`.
- [x] Deleted with the rewrite: `stack_trace_capture_rt.spice`, `stack_trace_capture_rt_windows.spice`,
      `stack_trace_symbol_rt.spice`, `stack_trace_symbol_rt_windows.spice`, `stack-trace-unwind.c`.
- [x] Demangling — shipped as `std/text/demangle`, wired into `StackTrace.capture()`.

**Tests (`test/`)**

- [x] `test-files/std/runtime/stack-trace-capture-basic` is built around platform- and opt-level-independent
      invariants. An exact frame count is not portable — it depends on how many frames the C runtime puts below
      `main`, and on inlining — so the test asserts that skipping n frames drops exactly n instead.
- [x] `test-files/std/runtime/stack-trace-dump-basic` covers `sDumpStacktrace()`. Addresses (ASLR) and offsets
      (codegen) are not reproducible, and the dump goes to stderr, which the test runner folds into the compared
      output — so the test points stderr at a file, then prints the trace back to stdout with those two columns
      masked. Only the three frames the test owns are printed; everything below `main` is C runtime and differs
      per platform. What remains — which symbols resolve and how they demangle — is now the same everywhere, so
      the three `cout-<os>.out` overrides collapsed into one `cout.out`.
- [x] Demangler test seeded from the mangled names in the `.ll` reference files.

**Docs**

- [x] `docs/docs/language/casts.md` — pointer↔integer casts.
- [x] `docs/docs/cli/*.md` — the `--keep-frame-pointers` flag.
- [x] `docs/docs/cli/build.md` — the `--strip-symbols` flag.
- [x] `docs/docs/language/attributes.md` — the two module attributes that were missing from the list
      (`core.darwin.linker.flag`, `core.linker.additionalSource`).
- [x] `docs/docs/how-to/stack-traces.md` — `sDumpStacktrace()`, the `StackTrace` API and the output format.

## Open questions / risks

- **libbacktrace must be present at link time, and macOS has no package for it.** GCC ships `libbacktrace.a`
  inside its own lib directory, so `-lbacktrace` resolves on a stock Linux or MinGW toolchain, and Clang finds it
  there too (measured with both drivers on Ubuntu; on Ubuntu the file belongs to `libgcc-13-dev`, i.e. it is
  upstream GCC's own, not Debian packaging). It is *not* part of the Apple toolchain. MacPorts has
  `devel/libbacktrace`, but **Homebrew has no formula** — checked against the full homebrew-core tree, 8602
  formulae, no match — so the usual macOS install route does not exist. The CI job therefore builds it from a
  pinned commit and caches it, and `docs/docs/how-to/stack-traces.md` tells users to use MacPorts or do the same.
  That is a real papercut for macOS users. Vendoring the library into `std/` would remove the dependency
  altogether at the cost of ~17k lines of third-party C in the tree and a second or two added to every link;
  worth revisiting if the manual step proves annoying.
- **Windows has no libbacktrace either.** Measured on CI in two rounds: the first failed with
  `lld: error: unable to find library -lbacktrace`, only in the two stack trace cases (2/734). The guess was that
  MinGW-w64's GCC carries it the way Linux GCC does - it is what the job builds LLVM with - and that clang's GNU
  driver simply does not search GCC's internal library directory. The second round disproved that:
  `gcc -print-file-name=libbacktrace.a` finds nothing on the runner, so there is no copy to point at. vcpkg
  cannot fill the gap either - its `libbacktrace` port is marked `"supports": "!windows"`, because the library
  does not build with MSVC at all. The job now builds it from source with MSYS2's MinGW GCC, the same recipe and
  the same pinned commit as the macOS job.
- **`-lbacktrace` turned out to be out-of-the-box on one platform of three.** Linux resolves it with no help;
  macOS and Windows both need libbacktrace built from source, which CI now does for them. That is considerably
  more friction than this route looked like it carried when it was chosen over vendoring, and CI is the easy
  half - a user on macOS or Windows has to do the same build by hand, every machine, before a program that takes
  a stack trace will link. Vendoring the sources into `std/` would make all three platforms work with nothing
  installed and no CI plumbing, at the cost of ~17k lines of third-party C in the tree and a second or two added
  to every link. On the evidence gathered since, that now looks like the better trade.
- **Static linking.** `spice build -static` still resolves the executable's own frames, since libbacktrace reads
  the on-disk symbol table rather than the loader's, but frames in code that would have come from a shared
  library are no longer attributable to one.
- **Inlining without debug info.** At `-O2` the unwinder reports physical frames only; inlined Spice functions
  reappear in the trace when the program carries debug info and stay invisible when it does not.
- **`main` prints differently with and without `-g`.** Without debug info the name comes from the symbol table
  (`main`); with it, from DWARF, which carries the Spice signature (`main()`). Cosmetic, and only `main` is
  affected, since every other Spice function is mangled the same way in both places.

## Verification performed

Everything below was measured on this repository's Linux/x86-64 container, against LLVM 23.1.1 (the release
tarball; apt.llvm.org is blocked by this environment's network policy):

- The `dladdr()` link-flag matrix, the `llc` frame-pointer table and the `c++filt` sweep over the 330 mangled
  names in `test/test-files/**/*.ll` — from the earlier round of work, unchanged.
- libbacktrace resolution measured directly from C in three link configurations before any Spice code was
  written: with `-g` (names, offsets, file and line), without `-g` (names and offsets from `.symtab`), and with
  `-Wl,-s` (nothing resolves) — which is why stripping had to stop being the default.
- The static-archive link-order failure reproduced with `gcc -lbacktrace main.c shim.c`, and the fix confirmed.
- `--strip-symbols` in both directions: without it the executable keeps its `.symtab` and every Spice frame
  resolves; with it the section is gone and every frame prints `<unknown>`. Passed together with `-g` it also
  removes all eight `.debug_*` sections, which is what `gcc -g -s` does and what the flag's help text says.
- The rewritten runtime run end-to-end at `-O0` through `-Oz`, and at `-O0 -g` and `-O2 -g`. Names and offsets
  resolve everywhere; file and line appear in the two `-g` builds; the three inlined frames of `-O2 -g` are
  recovered and share one physical address, and collapse to `main` alone from `-O1` upwards without debug info.
- Four threads capturing concurrently, 50 traces each — the state is created once and libbacktrace's own threaded
  flag covers the rest.
- `valgrind --leak-check=full` on a program that takes two traces: *All heap blocks were freed - no leaks are
  possible*, 0 errors. The shim's `malloc`/`free` pairing is what this checks; libbacktrace's permanent state is
  `mmap`-backed and does not show up as heap at all.
- The macOS CI recipe (`./configure --prefix=... --disable-shared && make && make install`) run here on Linux at
  the pinned commit: it produces `lib/libbacktrace.a` and the headers, and `clang -###` confirms that a
  `LIBRARY_PATH` entry is appended to the linker's `-L` list, which is how the Spice linker invocation will find
  it. The platform differs, so this validates the recipe, not the macOS build itself.
- `spicetest` in full, before and after the change. Both runs fail the same 58 tests, none of them related: the
  bootstrap-compiler suite (needs `SPICE_BOOTSTRAP_DIR`), the sanitizer cases (no compiler-rt in this container),
  the LLVM and libcurl bindings (no `LLVM_LIB_DIR`, no network), graphviz, and a set of `.ll` reference mismatches
  that come from building the compiler with clang rather than the project's usual toolchain. The two stack trace
  cases pass.

What is **not** verified by execution: everything about Windows and macOS. Those claims rest on libbacktrace's
own platform support (`macho.c`, `pecoff.c`) and toolchain documentation, since only a Linux/x86-64 runner was
available here. The macOS link-time availability of libbacktrace noted above is the biggest of these.

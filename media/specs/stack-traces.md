# Cross-Platform Stack Traces in Spice

## Status

Partially implemented, and **not** equivalent across platforms today. `std/runtime/stack_trace_rt.spice`
captures via a hand-rolled frame-pointer walk and symbolizes via `dladdr()`/DbgHelp. On the four CI targets
(`linux-amd64`, `linux-aarch64`, `windows-amd64` (MinGW), `darwin-arm64`) this produces three different
results, none of which is the intended one.

Target output, identical on every platform:

```
Stack trace:
  #0  0x00005f2c1d4a12c3  levelC() + 0xd
  #1  0x00005f2c1d4a12d3  levelB() + 0xd
  #2  0x00005f2c1d4a12e3  main() + 0xd
```

i.e. per frame: **frame address**, **hex offset from the symbol start**, and the **demangled** function name.

Remaining work is tracked in [Work items](#work-items) below.

## Background: what is broken today

### 1. Frame-pointer walking cannot be made equivalent

`StackTraceBuffer.capture()` reads `frame[0]` as the caller's saved frame pointer and `frame[1]` as the return
address. That layout is a System V / AAPCS convention. It does not hold everywhere, and it does not hold at all
once frame pointers are omitted:

- **Windows x64 has no frame-pointer chain at all.** Its prologue is `push rbp; sub rsp, N; lea rbp, [rsp+N]`,
  so `rbp` points into the *middle* of the frame, not at the saved `rbp`. `frame[0]`/`frame[1]` are ordinary
  locals. Windows x64 unwinds from `.pdata`/`.xdata` tables instead. Verified with
  `llc -O2 -mtriple=x86_64-pc-windows-msvc`.
- **The frame pointer is not actually enabled for Spice functions on any target.** `IRGenerator.cpp:34` calls
  `module->setFramePointer(llvm::FramePointerKind::All)`, which sets the `"frame-pointer"` *module flag*. The
  LLVM backend does not read that flag — it reads the per-function `"frame-pointer"="all"` **attribute**, which
  the IR generator never emits (there is no `"frame-pointer"` string anywhere in `src/`). Verified with `llc`
  on minimal IR carrying only the module flag:

  | target | module flag only | with `"frame-pointer"="all"` fn attr |
  | --- | --- | --- |
  | `x86_64-unknown-linux-gnu` (`-O0` *and* `-O2`) | `pushq %rax` — no FP | `pushq %rbp; movq %rsp, %rbp` |
  | `arm64-apple-darwin` `-O2` | `stp x29, x30, [sp, #-16]!` — record spilled, **`x29` never updated** | same `stp`, plus `mov x29, sp` |
  | `x86_64-pc-windows-msvc` `-O2` | — | `pushq %rbp; subq $32, %rsp; leaq 32(%rsp), %rbp` (still not a chain) |

  The missing `mov x29, sp` is exactly why macOS/AArch64 observes a captured count of 1: the frame record is
  written but the chain is never linked.

- **The Linux result that looks right is garbage.** `llvm.frameaddress` sets `MachineFrameInfo::isFrameAddressTaken()`,
  which forces a frame pointer for `capture()` *only*. Its callers (`levelA`/`levelB`/`levelC`) have none, so
  `capture()`'s prologue saves whatever `rbp` happened to hold — a leftover libc value — and the walk jumps
  straight into libc's chain. That is visible in the committed reference file
  `test/test-files/std/runtime/stack-trace-capture-basic/cout-linux-amd64.out`, whose very first frame of
  `sDumpStacktrace()` is `__libc_start_main` rather than `levelC`. The test passes because it only asserts
  non-nil addresses.

Conclusion: the walk must be replaced by the platform unwinder. Spice is already set up for this —
`IRGenerator.cpp:807` puts `uwtable` on every function, so `.eh_frame`/`.pdata` is emitted regardless of the
`nounwind` attribute on the same line.

### 2. `dladdr()` cannot see a Spice program's own symbols on ELF

`dladdr()` searches `.dynsym` only. Spice does not pass `-rdynamic`, so on Linux a Spice program's own
functions are absent from it. Measured on a three-deep C call chain:

| link flags | own frames resolved | libc frames resolved |
| --- | --- | --- |
| default | **no** (`dli_sname == NULL`) | partially |
| `-rdynamic` | **yes**, with exact `dli_saddr` for offsets | yes |
| `-rdynamic -Wl,-s` | **yes** | yes |

The third row matters: `ExternalLinkerInterface::prepare()` adds `-Wl,-s` for non-Darwin executables without
debug info. That strips `.symtab` but not `.dynsym`, so it does **not** conflict with `-rdynamic`. Darwin is
already excluded from `-Wl,-s` and keeps a full `LC_SYMTAB`, which its `dladdr()` reads, so macOS needs no
link-flag change.

### 3. There is no way to compute an offset in Spice

`OpRuleManager::getCastResultType()` allows `any* -> any*` in unsafe mode but has no pointer↔integer rule
(`CAST_OP_RULES` in `OpRuleManager.h:562` is primitives only), and `-` is not defined for `ptr - ptr`. So
`address - symbolStart` is currently inexpressible — as the comment on `StackTraceBuffer.dump()` already notes.

### 4. There is no demangler at runtime

Spice mangles with an Itanium-compatible scheme (`media/specs/name-mangling.md`), so frames currently print as
`_ZN13ArrayIteratorIiE4nextEv`. Checking every mangled name appearing in the committed `.ll` reference files
(330 distinct names) through `c++filt`, **329 demangle correctly**; the sole failure is `_ZN6Thread4ctorEPFvE`
— the `PF...E` function-type encoding that `NameMangling.cpp` already documents as not valid Itanium.

`abi::__cxa_demangle` would therefore work, but it lives in `libstdc++`/`libc++abi`, not libc: linking it
would add a `libstdc++.so.6`/`libc++abi.dylib`/`libstdc++-6.dll` runtime dependency to every Spice program that
touches a stack trace, and it is unavailable on musl-only and MSVC targets. Since Spice *emits* the mangling and
it is a small, closed subset, a Spice-side demangler is the equivalent-everywhere option and additionally covers
the `PF...E` case.

### 5. Output formatting is not equivalent

`dump()` prints addresses with `%p`. glibc renders `0x55c8de89c2c3`; MinGW's MSVCRT `%p` renders
`00000000004012C3` (no `0x`, uppercase, zero-padded). Once a pointer→integer cast exists, formatting the
address explicitly (e.g. `%#018lx`) removes the difference.

## Design

### Capture — platform unwinder

Split `capture()`'s platform half out the same way symbol resolution already is, into
`std/runtime/stack_trace_capture_rt.spice` (POSIX) and `..._windows.spice`, both exposing one ordinary function
`captureReturnAddresses(byte** out, int capacity, int skipFrames) -> int`. `StackTraceBuffer.capture()` keeps
living in the shared `stack_trace_rt.spice` and just calls it, preserving the single-file arrangement that
`RuntimeModuleManager`'s non-OS-suffixed auto-import requires.

- **POSIX** — `_Unwind_Backtrace(callback, ctx)` from libgcc/libunwind, implicitly linked by the gcc/clang
  driver on both Linux and macOS. Verified working on `-O2 -fomit-frame-pointer` code, producing a complete
  and correct chain. **Spice cannot express the callback**: a function converted to a raw pointer becomes a
  `.fatthunk` with an extra leading captures pointer, shifting every argument (spicelang/spice#1392), so the
  call is made from a C shim instead.
  `backtrace()` from `<execinfo.h>` looks simpler (no callback) but is not equivalent across platforms: Apple's
  implementation walks the frame-pointer chain rather than unwinding, and captures nothing when frame pointers
  are omitted. It is also absent on musl.
- **Windows** — `RtlCaptureStackBackTrace(DWORD skip, DWORD capture, void** out, ULONG* hash)` from kernel32.
  Single call, no callback, table-driven, correct on x64 where the FP walk cannot be. Works under MinGW and
  MSVC alike.

`__frame_address()` is then no longer needed by the runtime. It can stay as an undocumented builtin, but its
doc comments in `GenBuiltinFunctions.cpp:276` and `TypeCheckerBuiltinFunctions.cpp:633` must stop claiming the
compiler guarantees a frame-pointer chain, since it does not.

### Frame pointers (independent fix) — done

Implemented behind a new `--keep-frame-pointers` CLI flag, off by default, since forcing a frame pointer into
every function costs a register and some performance and the unwinder route below does not need one.

`IRGenerator::addCommonFctAttrs()` emits `"frame-pointer"="all"` per function when the flag is set, which is the
only thing the backend reads. The `frame-pointer` module flag is set from the same option, because it covers a
different set of functions: `Function::createWithDefaultAttr()` stamps it onto the functions LLVM itself
synthesizes, such as the sanitizer module ctors. Neither is emitted when the flag is off — matching what LLVM
does for its own synthesized functions, and leaving targets whose ABI mandates a frame pointer (AArch64 on
Darwin) to keep theirs. The flag is part of the compile cache key, since it changes the emitted object.

### Symbolization — name + exact offset

Keep the existing `resolveSymbolName()` split, but widen the contract to return both the name and the offset:

```spice
public type SymbolInfoResult struct {
    public String name       // mangled; empty if unresolved
    public long   offset     // address - symbol start; 0 if unresolved
    public bool   resolved
}
public f<SymbolInfoResult> resolveSymbol(byte* address)
```

- **POSIX** — `dladdr()` already fills `dliSAddr`; the offset is `cast<long>(address) - cast<long>(info.dliSAddr)`.
  Add `#![core.linux.linker.flag = "-rdynamic"]` to the POSIX symbol file so only programs that actually pull in
  the stack-trace runtime pay for it. Darwin needs no flag.
- **Windows** — `SymFromAddr()`'s third parameter is the displacement out-param; the current code passes
  `nil<unsigned long*>` and throws it away. Pass a real `unsigned long` and use it directly.

### Demangling — in Spice

New `std/runtime/stack_trace_demangle_rt.spice`, one shared file, no platform variants:
`public f<String> demangleSpiceName(const String& mangled)`. It needs to handle exactly the grammar in
`media/specs/name-mangling.md`:

- `_Z` prefix (anything else → return input unchanged),
- optional `N <this-type> ... E` method wrapper → `Type::name`,
- `<len><name>` length-prefixed identifiers,
- `I ... E` template argument lists → `name<A, B>`,
- the primitive type letters (`d i j s t l m a h c b v`), `P`/`R` wrappers, `A<n>_`, `<len><name>` structs,
- `PF...E` function types — the one case `__cxa_demangle` cannot do.

Roughly 200–250 lines of Spice, no external dependency, identical on every target. `dump()` demangles each
resolved name before printing and falls back to the raw mangled name on any parse failure.

### Formatting

```
  #%d  %#018lx  %s + %#lx\n
```
with the address formatted from `cast<long>(address)` rather than `%p`. `includeAddresses = false` continues to
suppress the address (and should also suppress the offset) so tests stay deterministic under ASLR/PIE.

## Work items

**Compiler (`src/`)**

- [x] Add pointer↔integer casts in unsafe mode: `cast<long>(ptr)` → `ptrtoint`, `cast<byte*>(long)` → `inttoptr`.
      Only `long` is accepted on the integer side, since it is the only Spice integer guaranteed to be 64 bit wide.
- [x] Emit the per-function `"frame-pointer"="all"` attribute, behind `--keep-frame-pointers` (default off).
- [x] Correct the `__frame_address()` doc comments in `GenBuiltinFunctions.cpp` / `TypeCheckerBuiltinFunctions.cpp`.

**Standard library (`std/runtime/`)**

- [x] `stack_trace_capture_rt.spice` — capture via `_Unwind_Backtrace`, called through a small C shim
      (`stack-trace-unwind.c`, pulled in with `core.linker.additionalSource`). The shim is needed because the
      unwinder's callback cannot be driven from Spice: a function converted to a raw pointer becomes a
      `.fatthunk` carrying an extra leading captures pointer, which shifts every argument (spicelang/spice#1392).
      `backtrace(3)` was tried first and had to be abandoned — glibc's routes through the same unwinder, but
      Apple's walks the frame-pointer chain, so it captured **zero** frames on macOS once frame pointers were
      omitted, which is the very dependency this work removes. Going through the unwinder directly also covers
      musl, which has no `<execinfo.h>`.
- [x] `stack_trace_capture_rt_windows.spice` — `RtlCaptureStackBackTrace`-based capture.
- [x] `stack_trace_rt.spice` — the public API: `StackTraceEntry` (address, offset, demangled name) with its own
      `dump()`, `StackTrace` wrapping a `Vector<StackTraceEntry>`, and the auto-imported `sGetStacktrace()` /
      `sDumpStacktrace()`.
- [x] `stack_trace_symbol_rt.spice` — `resolveSymbol()` returns name **and** offset, from `dladdr`'s `dli_saddr`.
- [x] `stack_trace_symbol_rt_windows.spice` — same, from the `SymFromAddr` displacement out-param. The
      `SYMBOL_INFO` layout is still unverified against a real `<dbghelp.h>`.
- [x] Demangling — shipped as `std/text/demangle`, wired into `StackTrace.capture()`.

**Tests (`test/`)**

- [x] `test-files/std/runtime/stack-trace-capture-basic` rewritten around platform- and opt-level-independent
      invariants, with both `cout-linux-*.out` overrides deleted. An exact frame count turned out not to be
      portable after all — it depends on how many frames the C runtime puts below `main`, and on inlining — so the
      test asserts that skipping n frames drops exactly n instead, which does hold everywhere.
- [x] Demangler test seeded from the mangled names in the `.ll` reference files.
- [x] Capture verified at every optimization level (`-O0` through `-Oz`).

**Docs**

- [x] `docs/docs/language/casts.md` — pointer↔integer casts.
- [x] `docs/docs/cli/*.md` — the `--keep-frame-pointers` flag.
- [ ] Document `sDumpStacktrace()` and the output format.

## Open questions / risks

- **`-rdynamic` is not enough, and is not applied.** Measured: it exports only `public` Spice functions, so
  `dladdr` still misses everything that stayed local to its object file. It also grows `.dynsym`, inhibits
  `--gc-sections`, and does nothing for `-static`. It is deliberately not added to the runtime; resolution today
  covers libc, shared libraries and a program's `public` functions.
  An alternative that avoids it entirely is a compiler-emitted `(function start, name)` side table in a
  dedicated section, binary-searched at runtime — fully self-contained and identical on every platform, at the
  cost of binary size and real IR-generator work. Worth considering if `-rdynamic` proves too blunt.
- **Static linking.** `dladdr()` returns 0 in a fully static glibc binary, so `spice build -static` will lose
  symbol names on Linux. The side-table approach above is the only fix.
- **MinGW + DbgHelp.** `SymFromAddr()` reads PDB/COFF symbols, not the DWARF that the MinGW toolchain emits,
  and `-Wl,-s` strips what is left. Windows symbolization likely needs `-Wl,--export-all-symbols` (so DbgHelp
  falls back to PE export symbols) and/or dropping `-Wl,-s` there. This needs to be confirmed on a real Windows
  runner — it could not be tested here.
- **Inlining.** At `-O2` the unwinder reports physical frames only; inlined Spice functions vanish from the
  trace. Recovering them requires reading DWARF/PDB inline records, which is out of scope here.

## Verification performed

The analysis above was measured on this repository's Linux/x86-64 container: the `dladdr()` link-flag matrix,
the `_Unwind_Backtrace` run at `-O2 -fomit-frame-pointer`, the `llc` frame-pointer table, and the `c++filt`
sweep over the 330 mangled names in `test/test-files/**/*.ll`.

The compiler changes listed above as done were then built and tested for real, against LLVM 23.1.1 built from
source (apt.llvm.org is blocked by this environment's network policy, so the packaged fast path was
unavailable). `spicetest` compiles clean, and the suite is at **731/732**. The one failure,
`StdTests.bindings_libcurlFileDownload`, downloads a file over the network and fails on the blocked proxy; it
is unrelated to these changes.

What is still **not** verified by execution: everything about Windows and macOS. Those claims rest on toolchain
documentation and `llc` cross-target output, since only a Linux/x86-64 runner was available here. The Windows
`SYMBOL_INFO` layout in particular remains the least-verified part of the existing code.

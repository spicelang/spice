# Cross-Platform Stack Traces in Spice

## Status

Implemented; see the "done" work items below for what shipped and in what order. Capture goes through the
platform unwinder (`_Unwind_Backtrace` on POSIX, `RtlCaptureStackBackTrace` on Windows) rather than a
hand-rolled frame-pointer walk. Symbolization falls back through, in order: libbacktrace (ELF/Mach-O/PE-COFF
alike, whenever a prebuilt archive exists for the target - the binary's own symbol table is never stripped) →
the platform's own resolver (`dladdr()`/DbgHelp) → unresolved. Everything below the "Symbolization — libbacktrace" section is the
historical design/investigation record this file grew from, including one superseded design (a
compiler-emitted symbol table) kept for context rather than deleted.

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

### Symbolization — libbacktrace (supersedes the compiler-emitted symbol table below)

The compiler-emitted symbol table described as "done" further down (`IRGenerator::generateSymbolTable()`,
`stack_trace_symtab_rt.spice`) shipped in #1395 and was then replaced end to end by
[libbacktrace](https://github.com/ianlancetaylor/libbacktrace) before this superseding note was written - the
table approach never reached a tagged release. Both directions solve the same problem (resolving a Spice
function the platform's own resolver cannot see, because it was never exported or the binary was stripped), but
libbacktrace reads the symbol table that is already in the binary rather than having the compiler duplicate it
into a second, custom section:

- No compiler-side codegen at all: `IRGenerator` no longer collects functions or emits anything symbol-table
  related. `ExternalLinkerInterface::prepare()` never emits `-Wl,-s` either, so the binary's own symbol table
  always survives to be read back - there is no flag gating this any more.
- `backtrace_syminfo()` returns the symbol's real **size** on ELF/Mach-O (not on PE/COFF, which has no size
  field to report), so a lookup can reject an address that is past the end of the function it nominally
  follows, rather than reporting a bogus large offset the way the old table (and `dladdr()`) had to on every
  platform. This removed the "keep whichever resolver reports the smaller offset" workaround the table needed
  (see the now-removed work item below) - `StackTrace.capture()` just tries libbacktrace first and falls
  through to `resolveSymbol()` on failure.
- One prebuilt static library per supported target (`std/runtime/lib/<arch>-<os>/libbacktrace.a`) ships with the
  std lib and is linked in whenever an archive is available for the target by `ExternalLinkerInterface::prepare()`,
  since the small C shim that calls into it (`std/runtime/stack-trace-libbacktrace.c`) is part of the build
  whenever a program imports `stack_trace_rt.spice` - only whether an archive exists gates resolution now, not
  a flag. `deps/libbacktrace` (a git submodule) is a build-time-only dependency used to produce that archive
  (locally via `build-libbacktrace.py`, in CI via `.github/workflows/publish.yml`); nothing under `deps/` ships.
- `backtrace_create_state(NULL, ...)` scopes the resulting state to the running executable's own file - but on
  ELF and Mach-O, `backtrace_initialize()` also walks every shared library already loaded at the time of the
  first lookup (`dl_iterate_phdr()` on ELF, the dyld image APIs on Mach-O) and folds each one's own symbol table
  into the same state, so `backtrace_syminfo()` resolves libc/`.so` frames too on those two platforms - not just
  the main executable, contrary to what an earlier draft of this doc (and the shim's own comment) claimed;
  confirmed empirically by resolving `&printf` through this exact call pattern. PE/COFF's `pecoff.c` has no
  equivalent module enumeration, so on Windows the state really is main-executable-only. Two gaps remain even on
  ELF/Mach-O: a library `dlopen()`'d after that first lookup is not in the snapshot (`resolveSymbol()`'s
  `dladdr()` re-scans live, so it still catches that case), and a fully stripped shared library with no symbol
  table left resolves nothing either way.
- **Windows is covered too, via libbacktrace's own `pecoff.c` backend** - not, as first assumed, left to DbgHelp
  alone. `SymFromAddr()` reads a PDB, which this project's clang/MinGW builds do not emit, so it does not
  resolve a program's own frames at all (confirmed by `stack-trace-dump-basic`'s own Windows reference).
  libbacktrace instead reads the PE/COFF symbol table the MinGW linker already writes into the `.exe` itself -
  the exact same kind of "read it directly out of the binary" trick as ELF/Mach-O, just a different container
  format. Verified end to end: cross-compiled a Spice program to `x86_64-pc-windows-gnu`, ran it under Wine, and
  watched `innerFrame`/`middleFrame`/`main` resolve by name, reading the same COFF symbol table `-Wl,-s` used to
  strip before stripping was removed entirely, on Windows exactly like ELF/Mach-O. One difference from a
  MinGW GCC build: clang's `x86_64-w64-windows-gnu` driver does not auto-link pthread the way GCC's `-posix`
  runtime variant does, so `ExternalLinkerInterface::prepare()` adds `-pthread` alongside the archive itself
  (harmless on Linux/macOS, where pthread symbols already live in libc).
- **The Windows archive is cross-compiled from Linux, not built natively on the Windows runner.**
  libbacktrace's build is autotools-based (`configure` + `make`), which a bare Windows runner does not have;
  `build-libbacktrace.py --target x86_64-windows` instead cross-compiles it using `gcc-mingw-w64-x86-64`, run
  from the `build-compiler-linux-x86` release job, and uploads it as its own artifact
  (`libbacktrace-x86_64-windows`) that `build-artifacts`' existing generic `libbacktrace-*` download/assemble
  step already picks up unmodified. Only wired into the release pipeline (`publish.yml`) so far - the regular
  PR test workflow (`ci-cpp.yml`) does not build this archive, so `stack-trace-dump-libbacktrace`'s Windows
  reference still expects `<unknown>` there; extending it would mean giving the Windows CI job a dependency on
  the Linux job finishing first, which was judged not worth losing job parallelism for given the mechanism was
  already verified locally via Wine.

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
- [x] ~~`IRGenerator::generateSymbolTable()`~~ — implemented behind a `--keep-symbol-table` flag, then removed
      again once libbacktrace replaced it (see [Symbolization — libbacktrace](#symbolization--libbacktrace-supersedes-the-compiler-emitted-symbol-table-below)
      above). `IRGenerator` no longer collects functions or emits a table at all.
- [x] ~~`--keep-symbol-table`~~ — the flag itself is gone. `ExternalLinkerInterface::prepare()` links the
      prebuilt `libbacktrace.a` for the current target whenever `SystemUtil::findLibbacktraceStaticLib()` finds
      one (maps `cliOptions.targetTriple` to `std/runtime/lib/<arch>-<os>/libbacktrace.a`, empty path/no linkage
      for any target without a prebuilt archive), and never emits `-Wl,-s` any more, so the binary's own symbol
      table always survives for libbacktrace to read back - keeping it is no longer optional.
- [x] `CacheManager` — no longer needs a cache-key entry for this at all, since there is no flag left whose
      value could vary between two otherwise-identical builds.

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
- [x] ~~`stack_trace_symtab_rt.spice`~~ / `stack-trace-symtab.c` — the compiler-emitted-table approach described
      in the previous point of this list; implemented, merged in #1395, then deleted outright and replaced by
      the two items below. Nothing from it remains in the tree.
- [x] `stack_trace_libbacktrace_rt.spice` / `stack-trace-libbacktrace.c` — `resolveFromLibbacktrace()`, which
      `StackTrace.capture()` tries before `resolveSymbol()`. The C shim wraps `backtrace_create_state()` /
      `backtrace_syminfo()` (deps/libbacktrace) behind a `pthread_once`-guarded lazily-initialized state scoped
      to the running executable's own file (`backtrace_create_state(NULL, ...)`); needed as a shim for the same
      reason the unwind shim is (`backtrace_syminfo()` takes a C callback, and a Spice function pointer is a
      `.fatthunk`, not a bare pointer - spicelang/spice#1392). `backtrace.h` is vendored as
      `std/runtime/backtrace.h` (unmodified, license text intact) so the shim compiles from what the std lib
      ships, without needing `deps/libbacktrace` (build-time only) present. Unlike the removed table,
      libbacktrace reports each symbol's **real size**, so `resolveFromLibbacktrace()` rejects an address past
      the end of the function it nominally follows outright, instead of needing the "keep whichever resolver
      reports the smaller offset" comparison the table required to avoid misattributing a libc/shared-library
      frame to the wrong Spice function. Verified end to end on Linux x86_64 and (link-only, cross-compiled)
      aarch64: `innerFrame`/`middleFrame` (neither `public`) and `main` all resolve to their demangled names
      without `-rdynamic`, reading the ELF symbol table directly out of the binary - covered by
      `stack-trace-dump-basic` itself now, since the symbol table is always kept.
- [x] ~~`stack_trace_libbacktrace_rt_windows.spice`~~ — implemented as an always-unresolved stub, on the initial
      (mistaken) assumption that DbgHelp already covered what libbacktrace would add on Windows. Deleted once
      that assumption was checked and found wrong: DbgHelp's `SymFromAddr()` needs a PDB this project's builds
      never emit, so it resolves nothing of the program's own, while libbacktrace's `pecoff.c` reads the PE/COFF
      symbol table the linker already writes into the `.exe` - the same "read it out of the binary itself"
      trick as ELF/Mach-O. `stack_trace_libbacktrace_rt.spice` (the base file, no longer POSIX-only) now covers
      all three formats; nothing Windows-specific remained to keep once that was true.
- [x] `std/runtime/lib/<arch>-<os>/libbacktrace.a` — one prebuilt static archive per supported target
      (`x86_64-linux`, `aarch64-linux`, `aarch64-macos`, `x86_64-windows`), built from the `deps/libbacktrace`
      submodule and resolved at link time by `SystemUtil::findLibbacktraceStaticLib()`. Not committed to the
      repository (`/std/runtime/lib/` is gitignored, since these are build outputs, not source) -
      `build-libbacktrace.py` produces the host's own archive for local dev (wired into `dev-setup.py`), and
      `.github/workflows/publish.yml` builds one per release target in its own job and assembles them into
      `std/` in `build-artifacts` right before GoReleaser packages it, so every release artifact carries the
      full set. The Windows archive is the one exception to "built on that target's own job": libbacktrace's
      autotools build needs a real shell/make toolchain a bare Windows runner doesn't have, so it's
      cross-compiled from the `build-compiler-linux-x86` job instead (`gcc-mingw-w64-x86-64`, `--target
      x86_64-windows`) and uploaded as its own artifact, picked up by `build-artifacts`' existing generic
      `libbacktrace-*` download/assemble step unmodified. macOS/x86_64 has no CI job at all today (Apple Silicon
      only), so it has no prebuilt archive either - same as any other target with no matching archive,
      resolution there just falls through to `resolveSymbol()`.
- [x] `ExternalLinkerInterface::prepare()` links `-pthread` alongside the archive whenever one is found. Needed
      for Windows: clang's `x86_64-w64-windows-gnu` driver does not auto-link pthread the way a MinGW GCC
      `-posix` runtime build does, and the shim's `pthread_once()` call needs it. A no-op everywhere else
      (Linux/macOS already have pthread symbols in libc), so this isn't platform-gated.

**Tests (`test/`)**

- [x] `test-files/std/runtime/stack-trace-capture-basic` rewritten around platform- and opt-level-independent
      invariants, with both `cout-linux-*.out` overrides deleted. An exact frame count turned out not to be
      portable after all — it depends on how many frames the C runtime puts below `main`, and on inlining — so the
      test asserts that skipping n frames drops exactly n instead, which does hold everywhere.
- [x] `test-files/std/runtime/stack-trace-dump-basic` covers `sDumpStacktrace()` on every platform. Addresses
      (ASLR) and offsets (codegen) are not reproducible, and the dump goes to stderr, which the test runner
      folds into the compared output — so the test points stderr at a file, then prints the trace back to
      stdout with those two columns masked. Only the three frames the test owns are printed; everything below
      `main` is C runtime and differs per platform. What remains — which symbols resolve and how they
      demangle — is pinned per platform in `cout-linux.out`, `cout-macos.out` and `cout-windows.out`.
      `innerFrame`/`middleFrame` (neither `public`) resolving by name on Linux and macOS *is* the libbacktrace
      path being exercised now that the symbol table is always kept - `cout-linux.out` was updated accordingly
      once stripping was removed (macOS already expected resolution, via `dladdr()`, since Darwin was already
      excluded from `-Wl,-s` beforehand). `cout-windows.out` still expects `<unknown>`, since the regular test
      workflow (`ci-cpp.yml`) does not build a Windows libbacktrace archive (see the work item above) and
      DbgHelp resolves nothing without a PDB either.
- [x] Demangler test seeded from the mangled names in the `.ll` reference files.
- [x] Capture verified at every optimization level (`-O0` through `-Oz`).
- [x] ~~`test-files/std/runtime/stack-trace-dump-libbacktrace`~~ — added to exercise `--keep-symbol-table`
      specifically, then deleted once that flag was removed: with the symbol table always kept,
      `stack-trace-dump-basic` already exercises the identical scenario (a non-`public`, unexported function
      resolving via libbacktrace), making this a redundant copy rather than a distinct case.

**Docs**

- [x] `docs/docs/language/casts.md` — pointer↔integer casts.
- [x] `docs/docs/cli/*.md` — `--keep-frame-pointers`; `--keep-symbol-table` was documented too, then its row was
      removed again once the flag itself was.
- [ ] Document `sDumpStacktrace()` and the output format.

## Open questions / risks

- **`-rdynamic` is not enough, and is not applied.** Measured: it exports only `public` Spice functions, so
  `dladdr` still misses everything that stayed local to its object file. It also grows `.dynsym`, inhibits
  `--gc-sections`, and does nothing for `-static`. It is deliberately not added to the runtime; resolution today
  covers libc, shared libraries and a program's `public` functions via `dladdr()`/DbgHelp - plus, via
  libbacktrace reading the binary's own symbol table directly, every function regardless of visibility or
  linkage, on any target with a prebuilt archive, unconditionally now that the symbol table is never stripped
  (see [Symbolization — libbacktrace](#symbolization--libbacktrace-supersedes-the-compiler-emitted-symbol-table-below)).
- **Static linking.** `dladdr()` returns 0 in a fully static glibc binary, so `spice build -static` would lose
  symbol names on Linux if libbacktrace could not step in - it reads the symbol table directly out of the
  binary's own file and does not go through `dladdr()` at all, so a statically linked binary on a target with a
  prebuilt archive still resolves.
- **MinGW + DbgHelp — resolved, by not depending on DbgHelp for this.** `SymFromAddr()` reads PDB/COFF symbols,
  not the DWARF the MinGW toolchain emits, and this project's builds emit no PDB either, so DbgHelp alone
  resolves nothing of a Spice program's own frames (confirmed on a real Windows CI runner, via
  stack-trace-dump-basic's `cout-windows.out`). Rather than chase `-Wl,--export-all-symbols` or a PDB, libbacktrace
  covers Windows the same way it covers ELF/Mach-O: `pecoff.c` reads the PE/COFF symbol table directly, which
  `-Wl,-s` still gates the same way it gates ELF's `.symtab` - dropping it Windows-only is not needed. Verified
  by cross-compiling to `x86_64-pc-windows-gnu` and running under Wine, though not (yet) inside this repo's own
  Windows CI job - see the libbacktrace archive work item above for that gap.
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

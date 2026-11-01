# Technical Specification for Freestanding Builds in Spice

A freestanding build produces code that runs without a hosted libc, without crt0 and without an operating system:
kernels, bootloaders, firmware, UEFI applications and `wasm32-unknown-unknown` modules. This is the equivalent of
`-ffreestanding -nostdlib` in C or `#![no_std]` in Rust.

Most of the low-level building blocks already exist: the `syscall` and `asm<R>()` builtins, `__volatile_load` and
`__volatile_store`, `core.compiler.noReturn`, `core.compiler.mangle`, `--no-red-zone`, `--relocation-model`,
`--code-model kernel`, `--cpu-features` and pointer globals. What is missing is a way to get rid of the hosted
dependencies, that the compiler, the runtime modules and the linker add implicitly.

## Implementation steps:

- [x] Phase 1: freestanding mode without heap types
    - [x] `--freestanding` CLI option, implied by a target OS of `none`/`unknown`
    - [x] `_freestanding` file suffix in the import collector
    - [x] Single panic entry point, user-defined panic handler with a trap fallback
    - [x] Reject the `printf` and `stderr` builtins
    - [x] No stack traces, no `uwtable`, static relocation model per default
    - [x] Optional `main` function, user-defined entry point
    - [x] Linker: `-nostdlib -static`, no PIE/RELRO, `--linker-script`
    - [x] Tests and docs
- [x] Phase 2: heap types
    - [x] Allocator hook
    - [x] ~~`memory_rt_freestanding.spice` and `string_rt_freestanding.spice`~~ libc shim `libc_rt_freestanding.spice`
    - [x] `mem*` implementations in std, `core.compiler.noBuiltins` attribute
    - [x] Tests and docs
- [x] Phase 3: std tiering and embedded targets
    - [x] `#![core.compiler.requiresOs]` module attribute and a matching semantic error
    - [x] Link compiler-rt builtins for embedded triples
    - [x] QEMU based executable tests
    - [x] Migrate the Wasm special case in the linker to freestanding mode

## Inventory of hosted dependencies

### Implicit calls emitted by the IR generator

| Origin                                                 | Hosted dependency                                       | Location                                     |
|--------------------------------------------------------|---------------------------------------------------------|----------------------------------------------|
| `printf` builtin, test main                            | `printf`                                                | `ir-generator.spice`, `std-function-manager` |
| `panic`, `assert`, union tag check                     | `fprintf(stderr, ...)` + `exit`                         | `ir-generator.spice`                         |
| `stderr` builtin, `getStdErrValue`                     | `stderr` / `__stderrp` global, `__acrt_iob_func`        | `ir-generator.spice`                         |
| Freeing heap locals in the string runtime              | `free`                                                  | `ir-generator.spice`                         |
| String comparison operators                            | `memcmp`                                                | `op-rule-conversion-manager.spice`           |
| Every function / module                                | `uwtable` attribute and module flag (`.eh_frame`)       | `ir-generator.spice`                         |
| `main` function                                        | `i32 main(argc, argv)`, called by crt0                  | `IRGenerator.visitMainFctDef`                |

`llvm.memcpy`/`llvm.memset` intrinsics are lowered to calls to `memcpy`/`memset` by the backend. This cannot be
avoided and is also part of the C freestanding contract (see "mem* functions").

### Auto-imported runtime modules

| Module                             | Hosted dependency                                                     |
|------------------------------------|-----------------------------------------------------------------------|
| `memory_rt`                        | `malloc`, `realloc`, `free`, `memcpy`, `memcmp`                       |
| `string_rt`                        | `malloc`, `realloc`, `free`, `memcpy`, `memmove`, `memcmp`, `strlen`, `strcmp` |
| `error_rt`, `error_trace_rt`       | `fprintf`                                                             |
| `stack_trace_rt`                   | libbacktrace, OS locks, `fprintf`                                     |

### Linker

`ExternalLinkerInterface` invokes the gcc/clang driver, which adds the host crt files and `-lc`. On top of that, it adds
`-pie`/`-no-pie`/`-static-pie` and full RELRO on Linux. Wasm is the only special case today
(`-nostdlib -Wl,--no-entry -Wl,--export-all`).

## Phase 1: freestanding mode without heap types

### CLI option

- New `CliOptions.freestanding`, set by `--freestanding` on `build`, `run`, `test` and `install`.
- Implied, if the OS of the target triple is not known (`Triple.getOSTypeName()` returns `unknown`), e.g.
  `x86_64-unknown-none`, `aarch64-none-elf`, `thumbv7em-none-eabihf`, `riscv32-unknown-none-elf`,
  `wasm32-unknown-unknown`. Kernel code for a Linux triple passes `--freestanding` explicitly.
- `run` and `test` with `--freestanding` are only allowed for the native target, since the resulting executable has to
  run on the host (e.g. a Linux `_start` that exits via `syscall`). Both subcommands have no target options anyway.
  `test` (and `--test-main`) is rejected in freestanding mode in phase 1, see "Code generation".
- The `syscall` builtin is rejected for targets without OS, since there is no kernel to call.
- Rejected with `--sanitizer` and `--coverage`, since their runtimes need a hosted libc.
- Defaults that change in freestanding mode: `--relocation-model=static`. Explicit options still win.

### Import collector

The import collector picks platform specific files by suffix (`foo_linux.spice`, `foo_x86_64.spice`,
`foo_linux_x86_64.spice`). For a `none` OS, `getOSTypeName()` returns `unknown`, so the lookup would end up at
`foo_unknown.spice`. In freestanding mode, the OS part of the suffix is `freestanding` instead, so that std can ship
freestanding variants of its files (`memory_rt_freestanding.spice`) with the existing mechanism. This also applies to
the combined suffix (`foo_freestanding_aarch64.spice`).

### Panic entry point

> **Implementation note:** Phase 1 implements the single entry point for freestanding mode only, in
> `IRGenerator.generateFreestandingPanic`. Hosted panics keep their inlined `fprintf` + `exit` sequences, so the hosted IR
> refs did not change. The hosted `sPanic` runtime function below is left for later: with the signature
> `(message, file, line)`, it could not reproduce today's output (the panic output contains the code snippet, the assert
> output has no code location at all). The message, that the panic handler receives, is the message of the error for
> `panic`, `Assertion failed: Condition '<condition>' evaluated to false` for a failed assertion and
> `Active field mismatch on union field access '<field>'` for an inactive union field. In freestanding mode, the panic does
> not clean up the scope, since the handler never returns. The runtime modules get a `_freestanding` variant via the same
> suffix mechanism as imports, which is used for `error_rt_freestanding.spice` (`Error` without the printing methods).

The `panic` builtin, `assert` and the union tag check each emit their own `fprintf` + `exit`
sequence today. They are routed through a single `noReturn` runtime function:

```spice
#[core.compiler.noReturn, core.compiler.cold]
public p sPanic(string message, string file, unsigned int line)
```

- Hosted: implemented in the runtime with today's behavior (message to stderr, error return trace, stack trace,
  `exit(1)`). The exact output stays the same, so existing `exception.out` refs remain valid. The IR refs change once,
  since the inlined `fprintf` sequences become calls.
- Freestanding: the user can define a panic handler with a new attribute:

  ```spice
  #[core.compiler.panicHandler]
  p onPanic(string message, string file, unsigned int line) {
      // Write to a serial port, blink an LED, halt the CPU, ...
  }
  ```

  Without a user-defined handler, panics lower to `llvm.trap`. At most one panic handler per program, otherwise a
  semantic error. The handler must be a `noReturn` procedure with exactly this signature.

### Builtins

- `printf` and `stderr` report a new semantic error `UNSUPPORTED_IN_FREESTANDING_MODE` (analog to
  `UNSUPPORTED_BY_BACKEND` for the TPDE backend).
- `syscall`, `asm`, `__volatile_load` and `__volatile_store` stay available. They are the main way to talk to hardware
  or a host kernel in this mode.

### Code generation

- No stack traces: the stack trace runtime is never requested, like for cross targets today.
- No `uwtable` function attribute and module flag, so no `.eh_frame` is emitted. Spice has no exceptions, so the tables
  are only needed for stack traces and debuggers. A later `--unwind-tables` option can turn them back on.
- The test main (`spice test`) needs `printf`, so `test --freestanding` is limited to the native target and links the
  hosted libc for the test runner only. Alternatively, `test` is not supported in freestanding mode in phase 1.

### Entry point

- `main` becomes optional, like with `--no-entry`. If present, it is generated as today, which is useful if the user
  brings their own crt0.
- The user defines the entry point as an unmangled, `noReturn` procedure:

  ```spice
  #[core.compiler.mangle = false, core.compiler.noReturn]
  public p _start() {
      unsafe {
          syscall(60l /* exit */, 0l);
      }
  }
  ```

- Follow-ups, not part of phase 1: `core.linker.section` to place reset vectors and boot code, a `naked` attribute for
  entry points that have to set up the stack themselves.

### Linker

- In freestanding mode, link with `-nostdlib -static` and skip the PIE, RELRO and `-static-pie` handling.
- Prefer `ld.lld` (via clang with `--target=<triple> -fuse-ld=lld`), since the host gcc driver cannot link for
  arbitrary bare-metal triples. Implemented for cross targets: freestanding builds for the host keep the usual linker.
  Clang only uses its bare-metal toolchain, if the triple has an environment, and hands the link to gcc otherwise. So
  `-elf` is appended for the linker invoker to triples like `x86_64-unknown-none`.
- New option `--linker-script <path>`, passed as `-T <path>` to the linker invoker, verbatim, without the expansion of the
  linker flags. `core.linker.flag` already covers other ad-hoc
  flags.
- `--output-container=object` already works for users, that want to link themselves.
- Raw binary output (`llvm-objcopy -O binary`) is a possible follow-up.

### Tests

- IR tests with `--target=x86_64-unknown-none` and `--target=aarch64-none-elf`, that check that no libc function is
  declared (panic lowers to `llvm.trap`, no `uwtable`).
- Error tests: `printf` / `stderr` in freestanding mode, two panic handlers, wrong panic handler signature,
  `--freestanding` with a sanitizer.
- One executable test on Linux x86_64: a `_start`, that writes a message via the `write` syscall and exits via the
  `exit` syscall, linked with `-nostdlib`. This runs on the CI host without QEMU.
- One executable test with a user-defined panic handler, that exits with a specific exit code via `syscall`.

### Docs

- New how-to page `docs/docs/how-to/freestanding.md`, linked from the Wasm how-to.
- `--freestanding` and `--linker-script` in `docs/docs/cli/build.md`.
- `core.compiler.panicHandler` in the attribute docs.

## Phase 2: heap types

> **Implementation note:** Phase 2 deviates from the plan below in two points:
>
> - **Allocator hook:** a struct with `#[core.compiler.globalAllocator]` does not work, since Spice globals can only be
>   primitives or pointers, so there is no instance for the runtime to call. Instead, the program marks three functions
>   with `#[core.compiler.allocFunction]`, `#[core.compiler.reallocFunction]` and `#[core.compiler.deallocFunction]`.
>   The type checker checks their signatures (`f<heap byte*>(unsigned long)`, `f<heap byte*>(heap byte*, unsigned long)`,
>   `p(heap byte*)`), allows at most one of each kind per program and only in freestanding mode
>   (`TypeChecker.registerAllocatorFunction`). They are exported as `malloc`, `realloc` and `free`. A program, that loads
>   the memory or string runtime without all three, gets the semantic error `MISSING_ALLOCATOR_FUNCTIONS` after the type
>   checking (`SourceFile.checkForAllocatorFunctions`), instead of a link error.
> - **Runtime modules:** instead of `_freestanding` copies of `memory_rt` (161 lines) and `string_rt` (991 lines), the
>   runtime modules stay unchanged and get the few C functions, that they call, from a libc shim:
>   `std/runtime/libc_rt_freestanding.spice` (runtime module `LIBC_RT`), which every freestanding program loads. It
>   defines `memcpy`, `memmove`, `memset`, `memcmp`, `bcmp`, `strlen` and `strcmp` in Spice with `core.compiler.noBuiltins`
>   (LLVM `"no-builtins"`). `bcmp` is needed, since LLVM turns `memcmp(...) == 0` into `bcmp` calls. The functions get weak
>   linkage, so the program can bring its own, and the exported symbol collision check skips them. The IR generator keeps
>   calling `free` for the heap locals of the string runtime, which now is the program's dealloc function. `calloc` is not
>   provided, since its definition would need `malloc` in every freestanding program, also in ones without heap types.
>
> Error return tracing is disabled in freestanding mode (the `errorReturnTracing` module attribute is ignored), since the
> trace is stored thread-local and printed to stderr. `error_rt_freestanding.spice` gets a no-op `Error.dumpTrace()`, so
> that `result_rt` compiles unchanged.
>
> The heap test, which allocates via `mmap` at `-O2`, uncovered a bug of the `syscall` builtin: the constraints did not
> declare `rcx` and `r11` as clobbered, which the x86_64 `syscall` instruction overwrites, nor the memory, that the kernel
> reads and writes. The operands are now bound to the registers of the syscall ABI via constraints, instead of `r` operands
> plus `mov` instructions, which also leaves enough registers for six args.

### Allocator hook

`memory_rt_freestanding.spice` implements `sAlloc`, `sAllocUnsafe`, `sRealloc`, `sReallocUnsafe` and `sDealloc`
on top of an allocator, that the user provides. Two options:

1. Symbol convention: the runtime declares `ext` functions (`spice_alloc`, `spice_realloc`, `spice_dealloc`) and the
   user defines them unmangled. Simple, but errors show up at link time.
2. Registration via a pointer global: the user sets the allocator at startup. Errors at run time (panic on an
   unregistered allocator), costs an indirect call.

Recommendation: an attribute like `#[core.compiler.globalAllocator]` on a struct, that the type checker verifies, so
that a missing or malformed allocator is a compile-time error. `std/os/offset-allocator` can serve as a ready-made
allocator over a static arena.

### Runtime modules

- `string_rt_freestanding.spice`: `strlen` and `strcmp` are replaced by Spice implementations, allocation goes
  through the allocator hook.
- The IR generator frees heap locals of the string runtime with `free` directly. In freestanding mode, it calls
  `sDealloc` like everywhere else.
- `error_rt`/`error_trace_rt`: the error return trace is either disabled or printed via the panic handler.

### mem* functions

LLVM emits calls to `memcpy`, `memmove`, `memset` and `memcmp` in any code, freestanding or not. std ships
implementations in `std/runtime/mem_freestanding.spice`, that are linked automatically in freestanding mode and are
marked with a new `core.compiler.noBuiltins` attribute (LLVM `no-builtins`). Otherwise, the loop idiom recognizer turns
the loop in `memcpy` into a call to `memcpy`. Users can opt out to bring optimized implementations.

## Phase 3: std tiering and embedded targets

> **Implementation note:** Phase 3 deviates from the plan below in these points:
>
> - **`requiresOs`:** only the modules, that call into the OS or libc themselves, get the attribute. A module, that depends on
>   one of them, is found transitively (`SourceFile.checkForOsRequiringImports`): the error `IMPORT_REQUIRES_OS` is reported at
>   the import in a non-std file, so that the user sees their own import and the module with the attribute, e.g. `std/net/http`
>   via `std/net/socket.spice`. Imports in std files are not checked themselves. The import collector does not collect the
>   imports of a module with the attribute in freestanding mode, since they might not resolve there (e.g. `std/os/os` has no
>   freestanding variant) and would hide the error. The attribute on the main source file is reported as
>   `UNSUPPORTED_IN_FREESTANDING_MODE`.
> - **std tagging is partial:** the stage0 compiler builds `src/` against the std of the repository and rejects the unknown
>   attribute. So only the OS tier modules outside the (all-platform) import closure of the compiler have the attribute yet:
>   `std/net/socket*`, `std/net/tls`, `std/io/logging`, `std/time/datetime`, `std/os/cmd`, `std/os/cpu` and
>   `std/test/lifetime-object`. After the stage0 bump, the remaining ones follow: `std/io/dir*`, `std/io/file`,
>   `std/io/impl/*`, `std/io/progress-bar`, `std/io/cli-subcommand`, `std/os/env*`, `std/os/filesystem*`, `std/os/mutex`,
>   `std/os/thread`, `std/os/process`, `std/os/system_*`, `std/os/impl/process_native*`, `std/time/time*`, `std/time/delay`,
>   `std/type/type-conversion` and `std/text/print`. Their dependants (e.g. `std/io/filepath`, `std/os/atomic`,
>   `std/os/message-queue`, `std/math/fct` or the JSON/TOML/YAML modules) are then covered transitively. `std/os/syscall_*`
>   only need a kernel, not a hosted OS, and stay without it.
> - **compiler-rt:** the linker invoker reports the builtins library for the target (`--rtlib=compiler-rt
>   -print-libgcc-file-name` for clang, `-print-libgcc-file-name` for GCC, which reports its `libgcc`). It is linked in every
>   freestanding build, if the file exists. Distributions mostly ship it for the host only. `std/math/fct` does not lower to
>   libm calls, it is OS tier only via `std/type/type-conversion`.
> - **QEMU tests:** the LLVM of Spice is built for AArch64, X86 and WebAssembly only, so there are no 32-bit Arm triples.
>   `irgenerator/freestanding/success-qemu-aarch64` runs on the `virt` machine of `qemu-system-aarch64`: a `start.S` (via
>   `core.linker.additionalSource`) enables FP/SIMD and sets up the stack, a linker script places the image in the RAM, the
>   program writes to the PL011 UART via `volatileStore`, uses heap types with a bump allocator and stops QEMU from its panic
>   handler via semihosting (`SYS_EXIT`). The runner boots programs of test cases with a `qemu.txt` control file (the QEMU
>   command line) and skips them locally, if QEMU is missing. The Linux CI installs `lld` and `qemu-system-arm`. x86_64 bare
>   metal is not covered, since QEMU only boots 32-bit multiboot kernels directly.
> - **Wasm:** besides moving `-nostdlib` to the freestanding setup, Wasm builds now always link with `wasm-ld`. Before, the
>   linker search picked an ELF linker like `ld.lld`, so linking a Wasm module failed. Hosted Wasm targets (e.g. WASI)
>   no longer get `-nostdlib`, `--no-entry` and `--export-all`, but are linked against the libc of the Wasm toolchain.
>   `common/success-link-wasm32` links a module and checks the Wasm header.

### std tiers

| Tier      | Requirement            | Examples                                                                 |
|-----------|------------------------|--------------------------------------------------------------------------|
| Core      | none                   | primitive types, `Result`/`Error` with literal messages, array iterators, text analysis, `std/os/volatile` |
| Alloc     | allocator hook         | `String`, `Vector`, `Map`, `UnorderedMap`, `Queue`, `algorithm`           |
| OS        | hosted OS              | `std/io`, `std/net`, `std/os/thread`, `std/time`, `std/os/env`            |

OS tier modules get a `#![core.compiler.requiresOs]` module attribute. Importing one in freestanding mode is a semantic
error (`IMPORT_REQUIRES_OS`), instead of unresolved symbols at link time. Modules that only need a different
implementation in freestanding mode use the `_freestanding` suffix.

### compiler-rt builtins

On targets without hardware division or floating point (e.g. `thumbv6m`), LLVM emits calls to compiler-rt builtins
(`__aeabi_*`, `__udivti3`, soft-float). In freestanding mode, Spice links `libclang_rt.builtins-<arch>.a` if it can find
it and otherwise expects the user to pass it via `core.linker.flag`. Math functions like `sin` lower to libm calls,
which are not available. The affected std functions get the OS tier attribute until there are pure Spice versions.

### Embedded tests

QEMU (`qemu-system-arm`, `qemu-system-aarch64`) with semihosting to report the exit code, for one or two embedded
triples.

### Wasm

`wasm32-unknown-unknown` is a freestanding target. The Wasm special case in `ExternalLinkerInterface.prepare` becomes
part of the freestanding linker setup, keeping `--no-entry` and `--export-all` as Wasm specifics.

## Open questions

- Panic handler: user-defined with a trap fallback (recommended), or always trap.
- ~~Allocator hook: symbol convention, pointer global registration, or a type-checked `globalAllocator` attribute
  (recommended).~~ Type-checked function attributes, see the implementation note of phase 2.
- Activation: only the explicit `--freestanding` option, or also implied by an unknown target OS (recommended).
- Should `spice test` support freestanding mode at all, or only for the native target with a hosted test runner?

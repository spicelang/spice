# Technical Specification for Freestanding Builds in Spice

A freestanding build produces code that runs without a hosted libc, without crt0 and without an operating system:
kernels, bootloaders, firmware, UEFI applications and `wasm32-unknown-unknown` modules. This is the equivalent of
`-ffreestanding -nostdlib` in C or `#![no_std]` in Rust.

The low-level building blocks are the `syscall` and `asm<R>()` builtins, `__volatile_load` and `__volatile_store`,
`core.compiler.noReturn`, `core.compiler.mangle`, `--no-red-zone`, `--relocation-model`, `--code-model kernel`,
`--cpu-features` and pointer globals. The freestanding mode removes the hosted dependencies, that the compiler, the runtime
modules and the linker add implicitly.

User-facing documentation: `docs/docs/how-to/freestanding.md`.

## Implementation steps:

- [x] Phase 1: freestanding mode without heap types
    - [x] `--freestanding` CLI option, implied by a target without OS
    - [x] `_freestanding` file suffix in the import collector
    - [x] Panic entry point, user-defined panic handler with a trap fallback
    - [x] Reject the `printf` and `stderr` builtins
    - [x] No stack traces, no `uwtable`, static relocation model per default
    - [x] Optional `main` function, user-defined entry point
    - [x] Linker: `-nostdlib -static`, no PIE/RELRO, `--linker-script`
    - [x] Tests and docs
- [x] Phase 2: heap types
    - [x] Allocator functions
    - [x] libc shim `libc_rt_freestanding.spice`
    - [x] `core.compiler.noBuiltins` attribute
    - [x] Tests and docs
- [x] Phase 3: std tiering and embedded targets
    - [x] `#![core.compiler.requiresOs]` module attribute and a matching semantic error
    - [x] Link compiler-rt builtins for embedded triples
    - [x] QEMU based executable tests
    - [x] Migrate the Wasm special case in the linker to freestanding mode

## Hosted dependencies

These are the hosted dependencies of a regular build and how the freestanding mode deals with them:

| Origin                                         | Hosted dependency                                  | Freestanding mode                                    |
|------------------------------------------------|----------------------------------------------------|------------------------------------------------------|
| `printf` builtin, test main                    | `printf`                                           | `printf` rejected, `test` rejected                   |
| `stderr` builtin                               | `stderr` / `__stderrp` global, `__acrt_iob_func`   | rejected                                             |
| `panic`, `assert`, union tag check             | `fprintf(stderr, ...)` + `exit`                    | panic handler or `llvm.trap`                         |
| Every function / module                        | `uwtable` attribute and module flag (`.eh_frame`)  | not emitted                                          |
| `main` function                                | `i32 main(argc, argv)`, called by crt0             | optional, user-defined entry point                   |
| `memory_rt`, `string_rt`                       | `malloc`, `realloc`, `free`, `mem*`, `str*`        | allocator functions + libc shim                      |
| String comparison operators, LLVM intrinsics   | `memcmp`, `bcmp`, `memcpy`, `memmove`, `memset`    | libc shim                                            |
| `error_rt`, `error_trace_rt`                   | `fprintf`, thread-local trace                      | `error_rt_freestanding.spice`, tracing disabled      |
| `stack_trace_rt`                               | libbacktrace, OS locks, `fprintf`                  | never requested                                      |
| Linker invoker                                 | host crt files, `-lc`, PIE, RELRO                  | `-nostdlib -static`, no PIE/RELRO                    |

## CLI

- `CliOptions.freestanding`, set by `--freestanding` on `build`, `run` and `install`.
- Implied, if the OS of the normalized target triple is unknown (`CliOptions.targetHasOs` is false, see `Driver.enrich`),
  e.g. `x86_64-unknown-none`, `aarch64-none-elf`, `thumbv7em-none-eabihf`, `riscv32-unknown-none-elf`,
  `wasm32-unknown-unknown`. Kernel code for a Linux triple passes `--freestanding` explicitly.
- `run` has no target options, so it always runs a freestanding executable on the host (e.g. a Linux `_start`, that exits
  via `syscall`).
- Rejected with `--sanitizer`, `--coverage` and in test mode (`test`, `--test-main`), since their runtimes need a hosted
  libc and the test main prints with `printf`.
- `--relocation-model` defaults to `static`. Shared libraries stay `pic`; the TPDE backend, Windows and Darwin stay `pie`.
  An explicit option wins.
- The build var `spice.is_freestanding` is set accordingly.

## Import collector and runtime modules

- The import collector picks platform specific files by suffix (`foo_linux.spice`, `foo_x86_64.spice`,
  `foo_linux_x86_64.spice`). In freestanding mode, the OS part of the suffix is `freestanding`
  (`foo_freestanding_x86_64.spice`, `foo_freestanding.spice`).
- Runtime modules are resolved the same way (`RuntimeModuleManager.loadModule`), so `_freestanding` variants win.
  `std/runtime/error_rt_freestanding.spice` is `Error` without the printing methods and with a no-op `dumpTrace()`, so that
  `result_rt` compiles unchanged.
- Error return tracing is disabled in freestanding mode (the `errorReturnTracing` module attribute is ignored), since the
  trace is stored thread-local and printed to stderr.

## Panic handler

- `panic`, failed assertions and accesses to an inactive union field call `IRGenerator.generateFreestandingPanic` in
  freestanding mode. It calls the panic handler of the program or lowers to `llvm.trap`. The scope is not cleaned up, since
  the handler never returns. Hosted panics keep their inlined `fprintf` + `exit` sequences.
- The panic handler is a procedure with the `core.compiler.panicHandler` attribute:

  ```spice
  #[core.compiler.panicHandler, core.compiler.noReturn]
  p onPanic(string message, string file, unsigned int line) {
      // Write to a serial port, blink an LED, halt the CPU, ...
  }
  ```

  It must be `noReturn` and have exactly this signature (`INVALID_PANIC_HANDLER`). There is at most one per program
  (`MULTIPLE_PANIC_HANDLERS`), registered in `GlobalResourceManager.panicHandler`. It is always exported
  (`Function.isPanicHandler`), so it can live in any source file.
- The message is the message of the error for `panic`, `Assertion failed: Condition '<condition>' evaluated to false` for a
  failed assertion and `Active field mismatch on union field access '<field>'` for an inactive union field.

## Builtins

- `printf` and `stderr` report the semantic error `UNSUPPORTED_IN_FREESTANDING_MODE`.
- `syscall` is rejected for targets without OS, since there is no kernel to call. `asm`, `__volatile_load` and
  `__volatile_store` stay available.

## Code generation and entry point

- No stack traces: the stack trace runtime is never requested (`CliOptions.printsStackTraceOnAbort`).
- No `uwtable` function attribute and module flag, so no `.eh_frame` is emitted.
- `main` is optional. If present, it is generated as usual, e.g. for a self-written crt0. The user defines the entry point
  as an unmangled, `noReturn` procedure:

  ```spice
  #[core.compiler.mangle = false, core.compiler.noReturn]
  public p _start() {
      unsafe {
          syscall(60s /* exit */, 0l);
      }
      while true {}
  }
  ```

## Heap types

### Allocator functions

The program marks three functions with `#[core.compiler.allocFunction]`, `#[core.compiler.reallocFunction]` and
`#[core.compiler.deallocFunction]`:

```spice
#[core.compiler.allocFunction]
f<heap byte*> arenaAlloc(unsigned long size) { ... }

#[core.compiler.reallocFunction]
f<heap byte*> arenaRealloc(heap byte* ptr, unsigned long size) { ... }

#[core.compiler.deallocFunction]
p arenaDealloc(heap byte* ptr) { ... }
```

- The type checker checks their signatures and allows at most one of each kind per program, only in freestanding mode
  (`TypeChecker.registerAllocatorFunction`).
- They are exported as `malloc`, `realloc` and `free`, which the unchanged `memory_rt` and `string_rt` call. The IR
  generator keeps calling `free` for the heap locals of the string runtime, which is the dealloc function then.
- A program, that loads the memory or string runtime without all three, gets the semantic error
  `MISSING_ALLOCATOR_FUNCTIONS` after the type checking (`SourceFile.checkForAllocatorFunctions`), instead of a link error.

A struct with a `core.compiler.globalAllocator` attribute is not possible, since Spice globals can only be primitives or
pointers, so there would be no instance for the runtime to call.

### libc shim

- `std/runtime/libc_rt_freestanding.spice` (runtime module `LIBC_RT`) is loaded for every freestanding program. It defines
  `memcpy`, `memmove`, `memset`, `memcmp`, `bcmp`, `strlen` and `strcmp` in Spice. LLVM emits calls to the `mem*` functions
  in any code and turns `memcmp(...) == 0` into `bcmp` at `-O2`.
- The functions have weak linkage, so the program can bring its own, and the exported symbol collision check skips them.
- `calloc` is not provided, since its definition would need `malloc` in every freestanding program, also in ones without
  heap types.

### `core.compiler.noBuiltins`

The function attribute sets the LLVM `"no-builtins"` attribute, so that the loop idiom recognizer does not turn the loop of
a `memset` implementation into a call to `memset` itself. The libc shim uses it, user-provided `mem*` implementations can
use it as well.

## std tiering

| Tier  | Requirement         | Examples                                                                                   |
|-------|---------------------|--------------------------------------------------------------------------------------------|
| Core  | none                | primitive types, `Result`/`Error`, iterators, `std/text/analysis`, `std/os/volatile`       |
| Alloc | allocator functions | `String`, `Vector`, `Map`, `UnorderedMap`, `Queue` and the other types of `std/data`       |
| OS    | hosted OS           | `std/io`, `std/net`, `std/time`, `std/os/thread`, `std/os/env`, `std/type/type-conversion` |

- Modules, that call into the OS or libc themselves, get the `#![core.compiler.requiresOs]` module attribute. Modules, that
  only need a different implementation in freestanding mode, use the `_freestanding` suffix instead.
- Dependants are found transitively (`SourceFile.checkForOsRequiringImports`): the error `IMPORT_REQUIRES_OS` is reported at
  the import in a non-std file, naming the imported module and the module with the attribute, e.g. `std/net/http` via
  `std/net/socket.spice`. Imports in std files are not checked themselves.
- The import collector does not collect the imports of a module with the attribute in freestanding mode, since they might
  not resolve there and would hide the error.
- The attribute on the main source file is reported as `UNSUPPORTED_IN_FREESTANDING_MODE`.
- Tagged so far: `std/net/socket*`, `std/net/tls`, `std/io/logging`, `std/time/datetime`, `std/os/cmd`, `std/os/cpu` and
  `std/test/lifetime-object`. Modules in the import closure of the compiler can only be tagged after a stage0 compiler bump,
  since the stage0 compiler rejects the unknown attribute. `std/os/syscall_*` only need a kernel, not a hosted OS, and stay
  untagged.

## Linker

- `ExternalLinkerInterface.prepare` links with `-nostdlib -static` and skips the PIE, RELRO and `-static-pie` handling.
- Cross freestanding builds use `ld.lld` (`requiresLLD` in `findLinker`), since the host linkers cannot link for bare-metal
  targets. Freestanding builds for the host keep the usual linker. Clang only uses its bare-metal toolchain, if the triple
  has an environment, and hands the link to gcc otherwise. So `-elf` is appended for the linker invoker to triples like
  `x86_64-unknown-none` (`getInvokerTargetTriple`).
- `--linker-script <path>` is passed as `-T <path>` to the linker invoker, verbatim.
- compiler-rt builtins: `-nostdlib` leaves them out, so the linker invoker is asked for the builtins library of the target
  (`--rtlib=compiler-rt -print-libgcc-file-name` for clang, `-print-libgcc-file-name` for GCC, which reports its `libgcc`).
  It is linked in every freestanding build, if the file exists. Otherwise, the user passes it via `core.linker.flag`.
- Wasm: builds always link with `wasm-ld`. Freestanding Wasm modules get `-nostdlib`, `--no-entry` and `--export-all`,
  without `-static`. Freestanding Wasm shared libraries get `--no-entry` as well, since clang would make `_initialize` of
  the Wasm toolchain libc their entry point. Hosted Wasm targets (e.g. WASI) are linked against the libc of the Wasm
  toolchain.
- `--output-container=obj` emits an object file for users, that want to link themselves.

## `syscall` builtin fix

The x86_64 `syscall` instruction overwrites `rcx` and `r11`, and the kernel reads and writes memory. The constraints did not
declare these clobbers, which corrupted values kept in `rcx` at `-O1` and above. The operands are now bound to the registers
of the syscall ABI via constraints (`{rdi}`, ...), instead of `r` operands plus `mov` instructions, which also leaves enough
registers for six args at `-O0`.

## Tests

- `test/test-files/irgenerator/freestanding/`: IR tests for bare-metal triples (no libc declarations, panic lowers to
  `llvm.trap` or calls the handler, no `uwtable`) and executable tests on Linux x86_64 (`_start` with `write`/`exit`
  syscalls, panic handler with a specific exit code, heap types via an `mmap` allocator at `-O2`, user-provided `mem*`).
- `test/test-files/typechecker/freestanding/`: error tests for `printf`/`stderr`, `syscall` without OS, `--sanitizer`,
  panic handlers, allocator functions and `requiresOs` imports.
- `success-qemu-aarch64` runs on the `virt` machine of `qemu-system-aarch64`: a `start.S` (via
  `core.linker.additionalSource`) enables FP/SIMD and sets up the stack, a linker script places the image in the RAM. With
  the MMU off, unaligned accesses fault, so the test is built with `--cpu-features=+strict-align` and enables the alignment
  check (`SCTLR_EL1.A`). The program writes to the PL011 UART via `volatileStore`, uses heap types with a bump allocator and
  stops QEMU from its panic handler via semihosting (`SYS_EXIT`). The runner boots programs of test cases with a `qemu.txt`
  control file (the QEMU command line) and skips them, if QEMU is missing. The Linux CI installs `lld` and
  `qemu-system-arm`.
- The LLVM of Spice is built for AArch64, X86 and WebAssembly only, so 32-bit Arm and RISC-V triples are not covered. x86_64
  bare metal is not covered by QEMU tests, since QEMU only boots 32-bit multiboot kernels directly.
- `common/success-link-wasm32` and `common/success-link-wasm32-dylib` link a Wasm module and a shared library and check the
  Wasm header.

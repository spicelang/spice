---
title: Freestanding builds
---

A freestanding build produces code that runs without a hosted C standard library, without crt0 and without an operating
system, e.g. kernels, bootloaders, firmware or `wasm32-unknown-unknown` modules. This is the equivalent of
`-ffreestanding -nostdlib` in C or `#![no_std]` in Rust.

## Enabling the freestanding mode

The freestanding mode is enabled with the `--freestanding` option of the [`build`](../cli/build.md), [`run`](../cli/run.md)
and [`install`](../cli/install.md) subcommands. Targets without an operating system enable it implicitly, e.g.
`x86_64-unknown-none`, `aarch64-none-elf`, `thumbv7em-none-eabihf`, `riscv32-unknown-none-elf` or
`wasm32-unknown-unknown`. Kernel code for a Linux target passes `--freestanding` explicitly.

```sh
$ spice build --target=aarch64-none-elf --linker-script=kernel.ld kernel.spice
```

In freestanding mode, the compiler:

- does not require a `main` function. The user defines the entry point instead (see below). A `main` function is still
  generated, if there is one, e.g. for a self-written crt0.
- rejects the `printf` and `stderr` builtins, since they need a hosted libc. The `syscall` and `asm` builtins as well as
  `__volatile_load` and `__volatile_store` stay available. They are the main way to talk to the hardware or to a host
  kernel. The `syscall` builtin is only available for targets with an operating system.
- routes panics, failed assertions to the panic handler (see below).
- prints no stack traces and emits no unwind tables (no `.eh_frame` section).
- generates static code by default (`--relocation-model=static`). Shared libraries stay `pic`, and the TPDE backend as
  well as Windows and Darwin targets stay `pie`, since they only support position independent code. An explicitly given
  relocation model wins, as long as it is supported for the output container and the target.
- links with `-nostdlib -static` and without the PIE and RELRO handling. For other targets than the host, it links with
  `ld.lld`, since the linkers of the host cannot link for bare-metal targets. WebAssembly modules are linked with
  `wasm-ld`, without `-static`, but without entry point and with all symbols exported (`--no-entry --export-all`), see
  [Compile for WebAssembly](web-assembly.md).
- links the builtins of the compiler runtime, if they are available for the target (see
  [Compiler runtime builtins](#compiler-runtime-builtins)).
- rejects imports of modules, that need an operating system (see [Standard library tiers](#standard-library-tiers)).

Sanitizers (`--sanitizer`), code coverage instrumentation (`--coverage`) and the `test` subcommand are not supported in
freestanding mode, since their runtimes need a hosted libc. `--output-container=obj` emits an object file, if you want to
link it yourself.

## Entry point

The entry point is an unmangled procedure, that never returns. Its name depends on the linker script or the platform,
for a Linux executable it is `_start`:

```spice
#[core.compiler.mangle = false, core.compiler.noReturn]
public p _start() {
    const string message = "Hello from freestanding Spice!\n";
    unsafe {
        syscall(/* write */ 1s, /* stdout */ 1l, cast<long>(cast<byte*>(message)), 31l);
        syscall(/* exit */ 60s, 0l);
    }
    while true {}
}
```

This program runs on x86_64 Linux without libc:

```sh
$ spice run --freestanding main.spice
Hello from freestanding Spice!
```

## Panic handler

Without an operating system, there is no stderr to print a panic message to and no process to exit. Therefore, panics,
failed assertions and accesses to inactive union fields call the panic handler of the program. It is a procedure with the
`core.compiler.panicHandler` attribute:

```spice
#[core.compiler.panicHandler, core.compiler.noReturn]
p onPanic(string message, string file, unsigned int line) {
    // Write to a serial port, blink an LED, halt the CPU, ...
    while true {}
}
```

The handler must have exactly this signature and must not return. There is at most one panic handler per program, but
it can live in any source file. Without a panic handler, panics lower to a trap instruction (`llvm.trap`).

## Heap allocations

The heap types of the standard library, e.g. `String`, `Vector` or `Result`, allocate their memory on the heap. In
freestanding mode, there is no libc, that could allocate it. So the program provides the allocator itself, as three
functions with these attributes and signatures:

```spice
#[core.compiler.allocFunction]
f<heap byte*> arenaAlloc(unsigned long size) {
    // Return a block of at least size bytes, or nil if out of memory
}

#[core.compiler.reallocFunction]
f<heap byte*> arenaRealloc(heap byte* ptr, unsigned long size) {
    // Resize the block (ptr may be nil), copying its contents, or return nil if out of memory
}

#[core.compiler.deallocFunction]
p arenaDealloc(heap byte* ptr) {
    // Give the block back (ptr may be nil)
}
```

They can be defined in any source file, but there is at most one of each kind per program. The compiler exports them
under the names of the C functions `malloc`, `realloc` and `free`, which the runtime of the standard library calls. If
the program uses a heap type without defining all three, the compiler reports an error. How the allocator gets its memory
depends on the platform, e.g. from a static region, that the linker script reserves, or from a host kernel via `mmap`.

## C functions

The code, that LLVM generates, may call `memcpy`, `memmove`, `memset`, `memcmp` and `bcmp`, even if the program never
calls them, e.g. for copying a struct. The runtime of the standard library also calls `strlen` and `strcmp`. In
freestanding mode, the compiler links implementations of these functions into the program. They are weak symbols, so the
program can bring its own implementations instead, e.g. optimized ones:

```spice
#[core.compiler.mangle = false, core.compiler.noBuiltins]
public f<byte*> memcpy(byte* dest, const byte* src, unsigned long size) {
    // ...
}
```

The `core.compiler.noBuiltins` attribute keeps LLVM from turning the loop of such an implementation into a call to the
function itself.

Error return tracing (`#![core.compiler.errorReturnTracing]`) is disabled in freestanding mode, since the trace is stored
thread-local and printed to stderr. `Error.dumpTrace()` and `Result.dumpTrace()` do nothing in freestanding mode.

## Platform specific source files

The compiler picks platform specific source files by their suffix, e.g. `foo_linux.spice` or `foo_linux_x86_64.spice`.
In freestanding mode, the OS part of the suffix is `freestanding`. So an `import "foo";` resolves to the first one of
these files, that exists:

1. `foo_freestanding_x86_64.spice`
2. `foo_freestanding.spice`
3. `foo_x86_64.spice`
4. `foo.spice`

## Standard library tiers

The modules of the standard library fall into three tiers:

| Tier  | Requirement             | Examples                                                                                     |
|-------|-------------------------|----------------------------------------------------------------------------------------------|
| Core  | none                    | primitive types, `Result` and `Error`, iterators, `std/text/analysis`, `std/os/volatile`     |
| Alloc | allocator functions     | `String`, `Vector`, `Map`, `UnorderedMap`, `Queue` and the other types of `std/data`         |
| OS    | hosted operating system | `std/io`, `std/net`, `std/time`, `std/os/thread`, `std/os/env`, `std/type/type-conversion`   |

The modules of the OS tier, that call into the operating system or the C standard library, have the
`#![core.compiler.requiresOs]` module attribute. Importing one of them in freestanding mode is a compile error, as well as
importing a module, that depends on one of them:

```
[Error|Semantic] ./main.spice:1:1:
Import requires an operating system: The module 'std/net/http' cannot be imported in freestanding mode, because it depends on 'std/net/socket.spice', which requires an operating system
```

Your own modules can have the attribute as well, e.g. the hosted part of a library, that supports both modes. Since such a
module is rejected anyway, its own imports are not resolved in freestanding mode.

!!! note
    Not all modules of the OS tier have the attribute yet, e.g. `std/io/file`, `std/os/thread` and
    `std/type/type-conversion` do not. The compiler itself uses them and is built with the previous release, which does
    not know the attribute. Using them in freestanding mode fails later, e.g. with unresolved symbols like `snprintf` at link
    time.

## Compiler runtime builtins

LLVM lowers some operations to calls into the builtins library of the compiler runtime, if the target has no instructions
for them, e.g. 128-bit division (`__udivti3`), floating point arithmetic on targets without FPU or the `__aeabi_*` helpers of
32-bit Arm. Hosted builds get the library from the linker invoker. In freestanding mode, the compiler asks the linker invoker
for the library of the target (`clang --target=<triple> --rtlib=compiler-rt -print-libgcc-file-name`, or `libgcc` for GCC)
and links it, if it exists. Distributions often ship compiler-rt for the host only. For other targets, build compiler-rt
for the target or take the library of the toolchain of the target, and pass it yourself:

```spice
#![core.linker.flag = "/opt/toolchain/lib/libclang_rt.builtins-aarch64.a"]
```

## Embedded targets

This program runs on the `virt` machine of QEMU, emulating a bare-metal AArch64 board. It writes to the UART of the board
and stops QEMU via [semihosting](https://github.com/ARM-software/abi-aa/blob/main/semihosting/semihosting.rst), which
reports the exit code of the program:

```spice
#![core.linker.additionalSource = "start.S"]

import "std/os/volatile";

const unsigned long UART_DATA_REGISTER = 0x09000000l;

p writeToUart(string text) {
    unsafe {
        char* dataRegister = cast<char*>(UART_DATA_REGISTER);
        for long i = 0l; text[i] != '\0'; i++ {
            volatileStore(dataRegister, text[i]);
        }
    }
}

#[core.compiler.noReturn]
p exitQemu(int exitCode) {
    // SYS_EXIT with the reason ADP_Stopped_ApplicationExit
    long[2] parameterBlock = [0x20026l, cast<long>(exitCode)];
    unsafe {
        asm("hlt #0xf000", "{x0},{x1},~{memory}", 0x18l, &parameterBlock[0]);
    }
    while true {}
}

// Called by _start in start.S
#[core.compiler.mangle = false, core.compiler.noReturn]
public p kmain() {
    writeToUart("Hello from bare-metal AArch64!\n");
    exitQemu(0);
}
```

QEMU starts the program without a stack and with the MMU off, so a few lines of assembly set up the stack, before they
call the Spice code. They also
allow FP and SIMD instructions, which LLVM uses e.g. for copying memory. The file is compiled by the linker invoker via the
`core.linker.additionalSource` attribute:

```asm
    .section .text.boot, "ax"
    .global _start
_start:
    mov x0, #(3 << 20)
    msr cpacr_el1, x0 // Allow FP and SIMD instructions
    isb
    ldr x0, =__stack_top
    mov sp, x0
    bl kmain
1:  wfe
    b 1b
```

The linker script places the program in the RAM of the board, which starts at `0x40000000`, and reserves the stack:

```
ENTRY(_start)

SECTIONS
{
    . = 0x40080000;
    .text : { KEEP(*(.text.boot)) *(.text .text.*) }
    .rodata : { *(.rodata .rodata.*) }
    .data : { *(.data .data.*) }
    .bss : { *(.bss .bss.* COMMON) }
    . = ALIGN(16);
    . += 0x10000;
    __stack_top = .;
}
```

With the MMU off, all data accesses are Device memory accesses, which fault, if they are unaligned. So the code is
generated without unaligned accesses (`--cpu-features=+strict-align`), until the program sets up the MMU:

```sh
$ spice build -O2 --target=aarch64-none-elf --cpu-features=+strict-align --linker-script=link.ld main.spice
$ qemu-system-aarch64 -machine virt -cpu cortex-a53 -nographic -semihosting-config enable=on,target=native -kernel main
Hello from bare-metal AArch64!
```

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
- generates static code by default (`--relocation-model=static`). An explicitly given relocation model still wins.
- links with `-nostdlib -static` and without the PIE and RELRO handling. For other targets than the host, it links with
  `ld.lld`, since the linkers of the host cannot link for bare-metal targets.

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

## Platform specific source files

The compiler picks platform specific source files by their suffix, e.g. `foo_linux.spice` or `foo_linux_x86_64.spice`.
In freestanding mode, the OS part of the suffix is `freestanding`. So an `import "foo";` resolves to the first one of
these files, that exists:

1. `foo_freestanding_x86_64.spice`
2. `foo_freestanding.spice`
3. `foo_x86_64.spice`
4. `foo.spice`

## Limitations

The freestanding mode is in an early stage. Heap allocations need an allocator, which a freestanding program does not
have yet. So the types of the standard library, that allocate memory (e.g. `String`, `Vector` or `Result`), and the
modules, that need an operating system (e.g. `std/io` or `std/os`), cannot be used in freestanding mode yet. Note that
LLVM may emit calls to `memcpy`, `memmove` and `memset` in any code, which the program then has to provide, like in C.

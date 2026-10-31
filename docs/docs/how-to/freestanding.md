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
  `ld.lld`, since the linkers of the host cannot link for bare-metal targets. WebAssembly is the exception: it keeps its
  own linker setup (`-nostdlib -Wl,--no-entry -Wl,--export-all`, without `-static`) and the linker search of hosted
  builds, see [Compile for WebAssembly](web-assembly.md).

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

## Limitations

The modules of the standard library, that need an operating system (e.g. `std/io` or `std/os`), cannot be used in
freestanding mode. Math functions like `sin` are lowered to calls into the C math library, which is not available either.

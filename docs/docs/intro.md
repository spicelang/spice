---
title: Introduction
---

Spice is a compiled, statically typed systems programming language that targets performance and practicality. It compiles
directly to native machine code via [LLVM](https://llvm.org), producing lean binaries with no runtime overhead.

## Design goals

- **Performance first** — zero-cost abstractions, value semantics by default, and LLVM optimizations at every level.
- **Familiar syntax** — borrows the curly-brace style of C/C++ so existing systems programmers feel at home.
- **Safety where it counts** — strong static typing, immutability via `const`, and automatic memory management
  through deterministic destructors, without a garbage collector.
- **Practical interop** — can call C and C++ libraries directly and cross-compile to any LLVM-supported target.
- **Self-contained toolchain** — a single binary covers compilation, running, testing, and installing packages.

## What Spice is good for

Spice is a systems language. It shines for:

- CLI tools and utilities
- Compilers and interpreters
- Embedded and low-level drivers
- Performance-critical libraries
- WebAssembly modules

It is not aimed at user-interface development — for GUI applications, a higher-level language is usually a better fit.

## A first look

```spice
import "std/text/print";

f<int> main() {
    println("Hello, Spice!");
    return 0;
}
```

Functions return values (`f<ReturnType>`), procedures do not (`p`). The entry point is always `main`, returning `int`.
The standard library is imported with `import "std/..."`.

## Next steps

- [Install Spice](install/linux.md) on your platform.
- Work through the [Hello World](language/hello-world.md) example.
- Browse the [language documentation](language/main-function.md) for a full reference.

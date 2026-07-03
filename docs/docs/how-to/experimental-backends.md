---
title: Experimental TPDE backend
---

Spice ships with the LLVM code generator as its default and stable compilation backend. Alongside it, an
**experimental** alternative backend is available: [**TPDE**](https://github.com/tpde2/tpde), a fast compiler
back-end framework that consumes LLVM IR directly and emits ELF objects at roughly `-O0` code quality but
**10-20× faster than `clang -O0`** — useful for tight edit/compile iteration loops.

This backend is **not** a drop-in replacement for LLVM CodeGen. It performs no optimization and supports only a
narrow slice of the targets LLVM does. It is opt-in at both build time and runtime.

!!! warning "Experimental"
    The TPDE backend is under active development and disabled by default. Expect rough edges, unsupported
    language features, and possible codegen bugs. Do not use for production builds. Bug reports welcome.

## Supported configurations

| Property                | Value                                      |
|-------------------------|--------------------------------------------|
| Host operating systems  | Linux (ELF only)                           |
| Target architectures    | `x86_64`, `aarch64`                        |
| Code model              | small only                                 |
| Relocation model        | PIC only                                   |
| Optimization            | none — `-O` flags are accepted but ignored |
| Link-time optimization  | not supported (`-lto` is rejected)         |

## Building Spice with TPDE support

TPDE is pulled in as a git submodule at `deps/tpde/`. To include it in your Spice build, configure CMake with
the `SPICE_ENABLE_TPDE` option turned on:

```sh
cmake -S . -B cmake-build-debug -DSPICE_ENABLE_TPDE=ON
cmake --build cmake-build-debug --target spice
```

Requirements:

- The same LLVM version Spice is built against must be one of TPDE's supported versions (currently 19.1, 20.1,
  21.1, 22.1). Spice's regular LLVM build already satisfies this on modern systems.
- A matching `clang` binary must be discoverable at configure time; TPDE uses it to compile its encoding
  templates. `dev-setup.py` installs a suitable one alongside LLVM.

If either requirement is missing, the CMake configuration will fail with a clear error — no partial build.

## Selecting the backend

Once built with `-DSPICE_ENABLE_TPDE=ON`, choose the backend per compilation with `--backend`:

```sh
# Default — LLVM CodeGen
spice build main.spice
spice build --backend=llvm main.spice

# Experimental TPDE backend
spice build --backend=tpde main.spice
spice run   --backend=tpde main.spice
```

If the compiler was built without `SPICE_ENABLE_TPDE`, passing `--backend=tpde` produces a CLI error directing
you to rebuild with the option turned on.

## Limitations you may hit

- `--dump-assembly` and `--dump-object-file` are not meaningful under TPDE; they return placeholder text.
- Any Spice code that requires wide vectors, exception handling, or non-`small` code model will fail to
  compile.
- Debug info emission (`-g`) works but produces `-O0`-quality info regardless of the `--build-mode`.

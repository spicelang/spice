---
title: Experimental TPDE backend
---

Spice ships with the LLVM code generator as its default and stable compilation backend. Alongside it, an
**experimental** alternative backend is available: [**TPDE**](https://github.com/tpde2/tpde), a fast compiler
back-end framework that consumes LLVM IR directly and emits ELF objects at roughly `-O0` code quality but
**10-20× faster than `clang -O0`** — useful for tight edit/compile iteration loops.

This backend is **not** a drop-in replacement for LLVM CodeGen. It performs no optimization and supports only a
narrow slice of the targets LLVM does. It is opt-in at runtime. The Linux release packages of Spice come with it; for
other builds, it is opt-in at build time as well.

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

The Linux release packages of Spice (`x86_64` and `aarch64`) are built with TPDE support, so this is only needed when
building Spice from source.

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

The self-hosted bootstrap compiler (`src/`) supports `--backend=tpde` as well. It emits objects via the std
TPDE bindings (see [Using TPDE from Spice code](#using-tpde-from-spice-code)), so it is only backed by TPDE if
`TPDE_FLAGS` was set while building it. Otherwise, `--backend=tpde` produces a CLI error.

## Limitations you may hit

- `--dump-assembly` and `--dump-object-file` are not meaningful under TPDE; they return placeholder text.
- Any Spice code that requires wide vectors, exception handling, or non-`small` code model will fail to
  compile.
- Debug info emission (`-g`) works but produces `-O0`-quality info regardless of the `--build-mode`.

## Using TPDE from Spice code

Besides being a backend of the Spice compiler, TPDE can also be used from Spice programs, which build LLVM modules with
the LLVM bindings (`std/bindings/llvm`). The TPDE bindings (`std/bindings/tpde`) compile such a module to an ELF object,
in a file or in memory:

```spice
import "std/bindings/llvm/llvm" as llvm;
import "std/bindings/tpde/tpde" as tpde;

f<int> main() {
    llvm::LLVMContext context;
    llvm::Module module = llvm::Module("example", context);
    // ... build the module with the LLVM bindings ...

    tpde::Compiler compiler = tpde::Compiler(llvm::getDefaultTargetTriple());
    if !compiler.isValid() { return 1; } // TPDE does not support the target or is not available
    String errorMessage;
    if compiler.compileToFile(module, "example.o", errorMessage) {
        printf("%s\n", errorMessage.getRaw());
        return 1;
    }
}
```

TPDE only offers a C++ API, so the bindings compile a small C API wrapper (`std/bindings/tpde/tpde-wrapper.cpp`) along
with the program. They need the TPDE libraries and headers, in addition to what the LLVM bindings need.

The Linux release packages ship them inside the std, at `std/bindings/tpde/lib` and `std/bindings/tpde/include`, and the
compiler points the bindings to them on its own. The shipped libraries are built against the LLVM version of the release
(see `LLVM_VERSION` in the release workflow), so the LLVM the program links against has to be the same version.

When building Spice from source, a build with `-DSPICE_ENABLE_TPDE=ON` produces the TPDE libraries. Point the bindings
to them with the `TPDE_FLAGS` environment variable, which also takes precedence over the libraries shipped with the std.
It holds the include flag for the TPDE headers and the paths of the TPDE static libraries, space-separated:

```sh
TPDE_FLAGS="-I<spice-src>/deps/tpde/tpde-llvm/include \
  <build>/deps/tpde/tpde-llvm/libtpde_llvm.a <build>/deps/tpde/tpde/libtpde.a \
  <build>/deps/tpde/tpde/deps/fadec/libfadec.a <build>/deps/tpde/tpde/deps/disarm/libdisarm64.a \
  <build>/deps/tpde/tpde/deps/spdlog/libspdlog.a" # spdlog only with TPDE logging enabled (default)
```

Without TPDE libraries, or on platforms other than Linux, the bindings still compile and link, but `tpde::isAvailable()` returns
`false` and every compilation fails with an error message.

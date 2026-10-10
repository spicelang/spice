---
name: spice-build
description: Build the Spice compiler (`spice`) from its Spice sources in `src/` with the stage0 compiler. Use when the user wants to compile/rebuild the compiler, produce a release/debug build, bootstrap the compiler to a fixed point, or set up the environment before running or testing.
---

# Build Spice

The compiler is self-hosted: it is written in Spice (`src/`, entry point `src/main.spice`) and built by a released Spice
compiler, the **stage0 compiler**. Its version is pinned in `.github/stage0-version`; `python fetch-stage0.py` downloads it
for the current platform into `build/stage0/spice`.

## Prerequisites

The compiler links LLVM through the std LLVM bindings (`std/bindings/llvm`), so building it needs an LLVM build. The
scripts find it via `LLVM_DIR` (`<llvm-build>/lib/cmake/llvm`) or `llvm-config`, or take `LLVM_LIB_DIR` and
`LLVM_INCLUDE_DIRS` (`-I<dir1> -I<dir2>`) directly. The local dev setup builds LLVM into `./llvm/build-release`:

```sh
export LLVM_DIR=$PWD/llvm/build-release/lib/cmake/llvm
```

If LLVM and third-party libs are missing, run `python dev-setup.py` once (slow: it clones and builds LLVM).
`python setup-deps.py` alone updates the submodules and builds libbacktrace into the std. `python setup-deps.py --tpde`
(Linux, needs `LLVM_DIR` or `llvm-config`) also builds TPDE with its own CMake project and installs it into
`std/bindings/tpde/`, where the compiler picks it up for `--backend=tpde` and the std TPDE bindings.

## Build

```sh
# Optimized with LTO, to build/spice (downloads the stage0 compiler first, if missing)
python build.py

# Unoptimized with debug info, for debugging the compiler
python build.py --build-type Debug

# Other output path or compiler to build with
python build.py --output build/spice-dev --compiler build/spice
```

The build bypasses the compilation cache (`--ignore-cache`), since a cached object can go stale when only an imported file
changes (issue #1417). An optimized LTO build of the compiler takes a few minutes.

To build by hand, set `SPICE_STD_DIR=<repo>/std` and `SPICE_BOOTSTRAP_DIR=<repo>/src` and run e.g.
`build/stage0/spice build -O0 --ignore-cache --output build/spice src/main.spice`.

## Bootstrap to a fixed point

```sh
python bootstrap.py                      # stage0 builds stage1, which builds stage2, ...
python bootstrap.py --output build/spice # copy the fixed point compiler
```

It succeeds as soon as two consecutive self-compiled stages are bit-identical. CI runs it on every push.

## Tips

- `python test/run-tests.py` builds the compiler under test on its own (into `build/test-tmp/bootstrap-compiler`); pass
  `--compiler build/spice` to reuse a build instead. See the `spice-test` skill.
- See the `spice-dump` skill for inspecting the IR/assembly output of the built compiler.
- When `src/` starts to use a language feature, that the pinned stage0 compiler does not support yet, the stage0 version
  has to be bumped to a release that does (see `fetch-stage0.py`).

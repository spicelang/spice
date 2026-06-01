---
name: spice-build
description: Build the Spice compiler (`spice`) and/or the test runner (`spicetest`) with CMake/Ninja. Use when the user wants to compile/rebuild the project, build a specific target, produce a release/debug build, or set up the CMake build directory before running or testing.
---

# Build Spice

The Spice repo is a CMake project (top-level `CMakeLists.txt`) that produces two
main artifacts:

- `spice` — the compiler CLI (`src/`, output at `<build-dir>/src/spice`)
- `spicetest` — the GoogleTest runner (`test/`, output at `<build-dir>/test/spicetest`)

## Build directory convention

Reuse one of these directories so artifacts stay organized (per `AGENTS.md`):

- `cmake-build-debug/` — day-to-day development and testing
- `cmake-build-release/` — optimized builds for benchmarking

The `./build.sh` helper instead builds into `build/` with `Release` + Ninja and
moves the binaries to the repo root; prefer the directories above for agent work
unless the user explicitly wants `build.sh`.

## Prerequisites

A configured LLVM build must be reachable. `find_package(LLVM)` locates it via
`LLVM_DIR` (the local dev setup builds LLVM into `./llvm/build-release`):

```sh
export LLVM_DIR=$PWD/llvm/build-release/lib/cmake/llvm
```

If LLVM and third-party libs are missing, run `./dev-setup.sh` once (slow: it
clones and builds LLVM). `./setup-deps.sh` alone fetches just the header-only
deps (json, CLI11) into `lib/`.

## Configure (first time / after CMakeLists changes)

```sh
cmake -S . -B cmake-build-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
# release:
cmake -S . -B cmake-build-release -G Ninja -DCMAKE_BUILD_TYPE=Release
```

## Build targets

Run from inside the build dir, or pass `--build <dir>`:

```sh
# Everything (compiler + tests)
cmake --build cmake-build-debug

# Just the compiler
cmake --build cmake-build-debug --target spice

# Just the test runner
cmake --build cmake-build-debug --target spicetest

# Both, parallel
cmake --build cmake-build-debug --target spice spicetest -j

# Optional leak-check target (needs valgrind)
cmake --build cmake-build-debug --target spicetest_leakcheck
```

## Tips

- Incremental rebuilds: just re-run `cmake --build <dir> --target <t>`; Ninja
  only recompiles what changed. No need to reconfigure unless CMake files change.
- After building, the compiler is at `cmake-build-debug/src/spice` — see the
  `spice-dump` skill for inspecting its IR/assembly output and `spice-test` for
  running the suite.
- Configure options live in `Options.cmake` / `Conditionals.cmake` /
  `LLVMOptions.cmake` if the user needs non-default flags.

# AGENTS.md

Guidance for coding agents working in this repository.

## Introduction

The Spice programming language is a general-purpose language designed for performance and safety, with a focus on
systems programming. This repository contains the compiler implementation, standard library, tests, and documentation
for the Spice language.

## Repository at a glance

- Project: **Spice Programming Language** compiler + standard library.
- Build system: **CMake** (top-level `CMakeLists.txt`) with `src/` and `test/` subdirectories.
- Main areas:
    - `src/`: compiler implementation, written in C++
        - `src/spice/`: main compiler executable and CLI handling
        - `src/typechecker/`: type checking and inference, overload resolution, and related components
        - `src/irgenerator/`: LLVM IR code generation
    - `src-bootstrap/`: early-stage self-hosted compiler, written in Spice
    - `test/`: GoogleTest-based test executable `spicetest`
        - `test/test-files/`: test input files for various reference integration tests
        - `test/unittest/`: GoogleTest-based unit tests for compiler components
    - `std/`: standard library source files (`.spice`)
    - `docs/`: documentation site sources
    - `media/specs`: design documents and notes on language features and implementation details

## Initial setup

There are helper scripts for common tasks, but the underlying CMake commands are also available for manual use.
The scripts are available on Linux (file extension `.sh`) and Windows (file extension `.bat`), but the CMake commands
are cross-platform.

- First-time local setup (can be skipped if environment is already configured):
    - `./dev-setup.sh`

## Building the project

Please reuse one of the following two cmake build directories for all build tasks, to keep build artifacts organized and
avoid confusion:

- `cmake-build-debug/`: general build directory for development and testing
- `cmake-build-release/`: build directory for release builds (optimized, no debug symbols) for performance benchmarking

Instructions how to build when inside the respective build directory:

- Build the entire project (compiler + tests):
    - `cmake --build .`
- Build only the compiler:
    - `cmake --build . --target spice`
- Build only the test executable:
    - `cmake --build . --target spicetest`
- Optional leak check target (requires valgrind installed):
    - `cmake --build . --target spicetest_leakcheck`

## Test and verification

- Run all tests:
    - `<build-directory>/test/spicetest`
- Run a specific test case (replace `TestSuiteName.TestCaseName` with the actual test name):
    - `<build-directory>/test/spicetest --gtest_filter=TestSuiteName.TestCaseName`
- Run tests with valgrind for memory leak detection:
    - `valgrind --leak-check=full <build-directory>/test/spicetest`
    - Note: This can be slow, so it's recommended to run only a focused set of tests when using valgrind.

## Running Spice programs

- Compile a Spice source file to an executable:
    - `<build-directory>/src/spice build <source-file.spice> -o <output-executable>`
- Run the compiled executable:
    - `./<output-executable>`
- For quick testing, you can also use the `run` command to compile and execute in one step:
    - `<build-directory>/src/spice run <source-file.spice>`
- Use the `--help` option to see all available commands and options:
    - `<build-directory>/src/spice --help`
- You can use an available sanitizer of your choice on Spice code (e.g. ASAN, TSAN, TYSAN, etc.)
    - `<build-directory>/src/spice run --sanitizer=address <source-file.spice>`

## Debugging

- Use a debugger like `gdb` or `lldb` to debug the compiler executable:
    - `gdb <build-directory>/src/spice`
    - Set breakpoints, run the program, and inspect variables as needed.
- For debugging test failures, you can run the test executable under the debugger:
    - `gdb <build-directory>/test/spicetest`
    - Use the `--gtest_filter` option to focus on specific tests when running under the debugger.
- For debugging memory issues, use valgrind as described in the testing section.

## Editing expectations

- Keep changes minimal and scoped to the task.
- Prefer existing project patterns and naming conventions in nearby files.
- Do not introduce large refactors unless explicitly requested.
- Update docs when behavior/CLI/user-facing output changes.

## Safety and quality

- Prefer incremental, reviewable commits.
- Before opening a PR, run at least a focused build or tests relevant to changed files.
- If environment limitations prevent running checks, document that clearly in the PR.

## Pull request guidance

Include in PR description:

1. What changed.
2. Why it changed.
3. How it was validated (exact commands + outcomes).
4. Any follow-up tasks or known limitations.

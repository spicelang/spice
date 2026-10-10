# AGENTS.md

Guidance for coding agents working in this repository.

## Introduction

The Spice programming language is a general-purpose language designed for performance and safety, with a focus on
systems programming. This repository contains the compiler implementation, standard library, tests, and documentation
for the Spice language.

## Repository at a glance

- Project: **Spice Programming Language** compiler + standard library.
- The compiler is self-hosted: it is written in Spice and built by a released Spice compiler (the stage0 compiler, pinned
  in `.github/stage0-version` and downloaded by `fetch-stage0.py` into `build/stage0/`).
- Build and test tooling: Python scripts in the repo root and `test/run-tests.py`.
- Main areas:
    - `src/`: the compiler, written in Spice
        - `src/main.spice`: entry point
        - `src/driver.spice`: CLI handling
        - `src/lexer/`, `src/parser/`: hand-written lexer and parser
        - `src/typechecker/`: type checking and inference, overload resolution, and related components
        - `src/irgenerator/`: LLVM IR code generation (via the LLVM C API bindings in `std/bindings/llvm`)
    - `test/`: tests
        - `test/run-tests.py`: test runner
        - `test/test-files/`: test input files for various reference integration tests
        - Unit tests are `#[test]` functions next to the code in `src/`
    - `std/`: standard library source files (`.spice`)
    - `docs/`: documentation site sources
    - `media/specs`: design documents and notes on language features and implementation details

## Initial setup

The helper scripts are Python-based and cross-platform.

- First-time local setup (can be skipped if environment is already configured):
    - `python dev-setup.py`
- The scripts find LLVM via `LLVM_DIR` (`<llvm-build>/lib/cmake/llvm`) or `llvm-config`. Alternatively, set
  `LLVM_LIB_DIR` and `LLVM_INCLUDE_DIRS` (`-I<dir1> -I<dir2>`).
- Download the stage0 compiler (also done by `build.py` and `dev-setup.py`, if it is missing):
    - `python fetch-stage0.py`
- Optional: build the TPDE libraries into the std, for the experimental TPDE backend (Linux only):
    - `python setup-deps.py --tpde`

## Building the project

- Build the compiler with the stage0 compiler to `build/spice` (optimized, with LTO):
    - `python build.py`
- Unoptimized build with debug info:
    - `python build.py --build-type Debug`
- Bootstrap the compiler until it reaches a fixed point (the compiler builds itself reproducibly):
    - `python bootstrap.py`

## Test and verification

- Run all tests (builds the compiler under test with the stage0 compiler first):
    - `python test/run-tests.py`
- Run the tests against an already built compiler:
    - `python test/run-tests.py --compiler build/spice`
- Run specific test cases (GoogleTest-style filter, `TestSuiteName.TestCaseName`):
    - `python test/run-tests.py --compiler build/spice --filter 'TestSuiteName.TestCaseName'`
- Update the refs of test cases with the actual output:
    - `python test/run-tests.py --compiler build/spice --filter '...' --update-refs`
- Build the compiler under test with AddressSanitizer, failing every test case that produces a sanitizer report:
    - `python test/run-tests.py --instrument asan`
- See `python test/run-tests.py --help` for all options.

## Running Spice programs

- Compile a Spice source file to an executable:
    - `build/spice build <source-file.spice> -o <output-executable>`
- Run the compiled executable:
    - `./<output-executable>`
- For quick testing, you can also use the `run` command to compile and execute in one step:
    - `build/spice run <source-file.spice>`
- Use the `--help` option to see all available commands and options:
    - `build/spice --help`
- You can use an available sanitizer of your choice on Spice code (e.g. ASAN, TSAN, TYSAN, etc.)
    - `build/spice run --sanitizer=address <source-file.spice>`

## Debugging

- Build the compiler with debug info (`python build.py --build-type Debug`) and use a debugger like `gdb` or `lldb`:
    - `gdb --args build/spice build <source-file.spice>`
    - Set breakpoints, run the program, and inspect variables as needed.
- For debugging a failing test case, run the compiler on the test case with the args the runner prints with `--verbose`.
- For debugging memory issues, run the tests against an ASAN-instrumented compiler (see above) or use valgrind on the
  compiler.

## Coding style

Follow the project [Coding Style Guide](STYLE_GUIDE.md) for all Spice changes (`src/`, `std/`, `test/test-files/`). Read it
before writing or modifying code.

## Editing expectations

- Keep changes minimal and scoped to the task.
- Prefer existing project patterns and naming conventions in nearby files.
- Do not introduce large refactors unless explicitly requested.
- Update docs when behavior/CLI/user-facing output changes.

## Safety and quality

- Prefer incremental, reviewable commits.
- Before opening a PR, run at least a focused build or tests relevant to changed files.
- If environment limitations prevent running checks, document that clearly in the PR.

## Branch naming

- Never use auto-generated or `claude/...` branch names. Name every branch `<type>/<slug>` with a short, lowercase,
  kebab-case slug, e.g. `fix/1409-condition-temporaries`.
- Allowed types: `feature/`, `fix/` (or `bug/`), `chore/`, `ci/`, `std/`, `bootstrap/`, `test/`, `docs/`,
  `security/`. See the `spice-contribute` skill for when to use which.
- If the session starts on a pre-created branch, rename it with `git branch -m <type>/<slug>` before the first push.
- This is enforced by the `.claude/hooks/check-branch-name.py` hook, which blocks git commands that create, rename or
  push a branch with a non-conforming name.

## Pull request guidance

Include in PR description:

1. What changed.
2. Why it changed.
3. How it was validated (exact commands + outcomes).
4. Any follow-up tasks or known limitations.

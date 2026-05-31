---
name: spice-run
description: Compile and/or run a Spice program with the `spice` CLI — build to an executable, run a file directly, run with a sanitizer, run its enclosed tests, or cross-compile. Use when the user wants to execute a `.spice` file or produce a binary from one.
---

# Compile & run Spice programs

Uses the `spice` compiler at `cmake-build-debug/src/spice` (build it via the
`spice-build` skill first).

```sh
SPICE=cmake-build-debug/src/spice
```

## Subcommands

| Command | Alias | Purpose |
|---------|-------|---------|
| `spice build <file.spice>` | `b` | Compile to an output container (default: executable) |
| `spice run <file.spice>` | `r` | Compile and execute immediately |
| `spice test <file.spice>` | `t` | Compile with a test entry point and run enclosed tests |
| `spice install <file.spice>` | `i` | Build and install to a PATH dir |
| `spice uninstall <file.spice>` | `u` | Remove an installed program |

## Common usage

```sh
# Compile to a named executable
$SPICE build path/to/main.spice -o ./myprog
./myprog

# Compile + run in one step (best for quick checks)
$SPICE run path/to/main.spice

# Optimized build
$SPICE build -O2 path/to/main.spice -o ./myprog

# Run the program's own tests
$SPICE test path/to/main.spice
```

## Useful options (build / run / test)

- `-o, --output <path>` — output file path (build).
- `-O0`..`-O3`, `-Os`, `-Oz` — optimization level; `-lto` enables LTO.
- `-m, --build-mode debug|release|test` — build mode.
- `--sanitizer=address` (and friends: TSAN, TYSAN, …) — run under a sanitizer,
  e.g. `$SPICE run --sanitizer=address file.spice`.
- `-b, --build-var key=value` — parametrize the compiled program.
- `-j, --jobs <n>` — parallel compile jobs.
- `--output-container exec|obj|lib|dylib` — output format (build).
- `--static` — link statically.
- `--no-entry` — don't generate a `main`.
- `-d, --debug-output` — verbose compiler debug output.
- `--ignore-cache` — force full recompilation.

## Cross-compiling

```sh
# By target triple
$SPICE build --target aarch64-linux-gnu path/to/main.spice -o ./myprog

# Or piecewise
$SPICE build --target-arch aarch64 --target-os linux path/to/main.spice -o ./myprog
```

Stable targets: linux/x86_64, linux/aarch64, windows/x86_64, darwin/aarch64,
webassembly. Others (`x86`, `armv7`, `riscv`, `mips`, `powerpc`, `nvptx`,
`amdgpu`, …) compile without stability guarantees.

To inspect the IR/assembly the compiler produces instead of running, use the
`spice-dump` skill. Run `$SPICE --help` for the complete, current option list.

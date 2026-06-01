---
name: spice-profiling
description: Profile and benchmark the Spice compiler (the host C++ `spice` program) and the native binaries it emits from `.spice` files — using the built-in per-stage timers, the dedicated CMake profiling build options, and external tools (perf, valgrind/callgrind, flamegraph, hyperfine). Covers the preferred build/compile settings for representative profiling of each layer. Use when the user wants to find where compile time or runtime is spent, benchmark a change, or chase a performance regression.
---

# Spice — Profile & Benchmark

There are two distinct things to profile — pick the layer first:

1. **The host compiler** — the `spice` C++ program itself. "Why is *compilation* slow?"
   Use the built-in per-stage timers first, then a sampling profiler on a
   profiling build of `spice`.
2. **The emitted Spice program** — the native binary `spice` produces from a
   `.spice` file. "Why is the *generated code* slow?" Compile it with the right
   opt level + debug info, then profile the binary like any other native exe.

```sh
SPICE=cmake-build-release/src/spice   # build via the spice-build skill
```

## Preferred compile settings (read this first)

**Profiling is only meaningful on a representative build.** The defaults differ
per layer:

| Goal | Build / compile settings |
|------|--------------------------|
| Benchmark compiler wall-clock | `cmake-build-release` (`CMAKE_BUILD_TYPE=Release` → `-O3`), optionally `-DSPICE_LTO=ON` and `-DSPICE_DEV_COMPILE=ON` (adds `-march=native` + mold linker) |
| Sample-profile the compiler | `RelWithDebInfo` (keeps `-O2` **and** `-g`) — optimized but with symbols. Add `-DSPICE_DEV_COMPILE=ON` for faster iteration |
| Profile the compiler with clean call graphs | `-DSPICE_PROF_COMPILE=ON` — forces `-g -fno-omit-frame-pointer -O0`. Accurate line/frame attribution but **not** representative of release speed; use to understand structure, not to measure |
| Benchmark an emitted Spice binary | `$SPICE build -O2 …` (or `-O3` for max perf) |
| Sample-profile an emitted Spice binary | `$SPICE build -O2 -g -o prog file.spice` — optimized **with** debug info for symbol/line attribution |

Rule of thumb: never profile a `Debug`/`-O0` compiler build or an `-O0`
`.spice` binary to draw performance conclusions — `-O0` time goes to noise the
optimizer would have removed. Use `-O0` only when you need every source line to
map cleanly to a frame.

Per `AGENTS.md`, reuse only the two approved build directories — don't create
extra build trees. Profiling builds go in **`cmake-build-release`** (the
directory designated for performance work); reconfigure it with the
profiling-specific options below as needed. The standard `cmake-build-debug` is
for development/testing and is never the right thing to profile.

## 1. Built-in compiler timers — always start here

The compiler instruments every pipeline stage (`src/util/Timer.h`, results in
`CompilerOutput::times`). `-d` / `--debug-output` prints each stage's runtime
plus an end-of-run summary:

```sh
$SPICE build -d file.spice
```

Per-stage lines look like `[Type Checker Post] for file.spice: AST --> AST
(12 ms; 2 run(s))`, covering: Lexer, Parser, CST/AST Visualizers, AST Builder,
Import Collector, Symbol Table Builder, Type Checker Pre/Post, Dep-Graph
Visualizer, IR Generator, IR Optimizer, Object Emitter. The summary
(`dumpCompilationStats`) reports source-file and line counts, BlockAllocator
bytes/allocations, total type count, and **Total compile time**.

This tells you *which stage* dominates before you reach for a sampling profiler.
Pair with `--dump-cache-stats` to see the function/struct/interface lookup-cache
hit rates (a low hit rate points at redundant resolution work).

## 2. Sampling-profile the compiler (perf)

Build a profiling-capable `spice` (see the table above), then sample it:

```sh
# Reconfigure the release dir as optimized + symbols (per AGENTS.md, reuse it)
cmake -S . -B cmake-build-release -G Ninja \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo -DSPICE_DEV_COMPILE=ON
cmake --build cmake-build-release --target spice

perf record -g --call-graph dwarf -- $SPICE build -O2 some-large-input.spice
perf report            # interactive; or: perf report --stdio | head -50
```

- `perf stat -- $SPICE build file.spice` for cycles/IPC/branch-miss/cache-miss
  counters without a full profile.
- For clean call graphs where O2 inlining obscures the structure, rebuild with
  `-DSPICE_PROF_COMPILE=ON` (O0 + frame pointers) and use
  `--call-graph fp` instead of `dwarf` (cheaper, accurate with frame pointers).
- Flamegraph:
  ```sh
  perf script | stackcollapse-perf.pl | flamegraph.pl > spice.svg
  ```

### valgrind / callgrind (instruction-exact, no rebuild for representative code)

```sh
valgrind --tool=callgrind --callgrind-out-file=cg.out $SPICE build file.spice
callgrind_annotate cg.out | head -60     # or open cg.out in kcachegrind
```

Callgrind is ~20–50× slower but deterministic and gives exact
instruction/call counts — good for comparing two compiler revisions on the same
input. (For leak/memory-error hunting rather than CPU profiling, that's the
`spice-debugging` skill's valgrind section instead.)

## 3. C++ build-time profiling (`-ftime-report`)

To profile *building the compiler itself* (which TUs / template instantiations
are slow to compile), not its runtime, use the dedicated option:

```sh
cmake -S . -B cmake-build-release -G Ninja \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo -DSPICE_PROFILE_COMPILATION=ON
cmake --build cmake-build-release --target spice
```

This adds `-ftime-report`, so each C++ TU prints where the host compiler spent
its time. Useful when iteration speed (rebuild time) is the complaint, e.g.
before deciding on `-DSPICE_UNITY_BUILD=ON`.

## 4. Profile an emitted Spice binary

Compile the program optimized **with** debug info so the profiler can attribute
samples to source, then treat it as any native binary:

```sh
$SPICE build -O2 -g -o /tmp/prog file.spice

perf record -g --call-graph dwarf -- /tmp/prog <args>
perf report
# or instruction-exact:
valgrind --tool=callgrind --callgrind-out-file=cg.out /tmp/prog <args>
```

- Compare opt levels to see what the optimizer buys: `-O0`/`-O2`/`-O3`/`-Os`/`-Oz`.
- Cross-reference hot spots with `$SPICE build -O2 -ir file.spice` (IR) and
  `-s` (assembly) from the `spice-dump` skill to see whether a hot loop is being
  lowered as expected, or whether a missed optimization in IRGenerator/IROptimizer
  is the real cause.

## 5. Wall-clock benchmarking (hyperfine)

For A/B timing of the compiler or an emitted binary, use a statistical
benchmarker rather than a single `time` run:

```sh
# Double quotes so the outer shell expands $SPICE before hyperfine's child shell
# runs each command (a non-exported var is unset inside the child shell).
hyperfine --warmup 3 \
  "$SPICE build -O2 file.spice" \
  "old-spice build -O2 file.spice"

hyperfine --warmup 3 '/tmp/prog <args>'      # emitted binary
```

Always `--warmup` (filesystem cache) and benchmark a `Release` compiler — a
`Debug` build's numbers are not comparable to anything shipped.

## 6. Choosing a tool

| Question | Reach for |
|----------|-----------|
| Which compiler *stage* is slow? | `$SPICE build -d` (built-in timers) |
| Which C++ function in the compiler is hot? | `perf record` on RelWithDebInfo build |
| Exact instruction/call counts, compare two revisions | callgrind + `callgrind_annotate` |
| Why is the *rebuild* slow? | `-DSPICE_PROFILE_COMPILATION=ON` (`-ftime-report`) |
| Which function in the *emitted* program is hot? | `$SPICE build -O2 -g`, then perf/callgrind |
| Is a change actually faster (wall clock)? | hyperfine on a Release build |
| Redundant symbol resolution? | `--dump-cache-stats` |

## Related skills

- `spice-build` — configure the build dirs and the `SPICE_*` CMake options used
  above (`SPICE_PROF_COMPILE`, `SPICE_PROFILE_COMPILATION`, `SPICE_LTO`,
  `SPICE_DEV_COMPILE`, `SPICE_UNITY_BUILD`).
- `spice-run` — compile/run a `.spice` file, opt levels and `-g`.
- `spice-dump` — IR/assembly dumps to confirm whether a hot spot is a codegen issue.
- `spice-debugging` — valgrind for *memory* errors/leaks (vs. CPU profiling here).
- `spice-architecture` — map a slow stage to the class/file that owns it.

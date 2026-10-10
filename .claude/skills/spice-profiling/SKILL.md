---
name: spice-profiling
description: Profile and benchmark the Spice compiler (the `spice` program, itself written in Spice) and the native binaries it emits from `.spice` files — using the built-in per-stage timers and external tools (perf, valgrind/callgrind, flamegraph, hyperfine). Covers the preferred build/compile settings for representative profiling of each layer. Use when the user wants to find where compile time or runtime is spent, benchmark a change, or chase a performance regression.
---

# Spice — Profile & Benchmark

There are two distinct things to profile — pick the layer first:

1. **The compiler** — the `spice` program itself, built from `src/`. "Why is *compilation* slow?" Use the built-in per-stage
   timers first, then a sampling profiler on an optimized build of `spice` with debug info.
2. **The emitted Spice program** — the native binary `spice` produces from a `.spice` file. "Why is the *generated code*
   slow?" Compile it with the right opt level + debug info, then profile the binary like any other native exe.

Since the compiler is a compiled Spice program as well, both layers are profiled the same way; only the build differs.

```sh
SPICE=build/spice   # build via the spice-build skill
```

## Preferred compile settings (read this first)

**Profiling is only meaningful on a representative build.**

| Goal | Build / compile settings |
|------|--------------------------|
| Benchmark compiler wall-clock | `python build.py` (`-O3 -lto`, like the release; the release additionally uses `--static`) |
| Sample-profile the compiler | Build it by hand with `-O2 -g` (optimized **and** symbols), see below |
| Profile the compiler with clean call graphs | `python build.py --build-type Debug` (`-O0 -g`) — accurate line/frame attribution but **not** representative of release speed |
| Benchmark an emitted Spice binary | `$SPICE build -O2 …` (or `-O3` for max perf) |
| Sample-profile an emitted Spice binary | `$SPICE build -O2 -g -o prog file.spice` — optimized **with** debug info |

Rule of thumb: never profile an `-O0` build to draw performance conclusions — `-O0` time goes to noise the optimizer would
have removed. Use `-O0` only when you need every source line to map cleanly to a frame.

Building the compiler by hand (from the repo root, with `LLVM_LIB_DIR`/`LLVM_INCLUDE_DIRS` set or derivable, see
`spice-build`):

```sh
SPICE_STD_DIR=$PWD/std SPICE_BOOTSTRAP_DIR=$PWD/src \
  build/stage0/spice build -O2 -g --native-features --ignore-cache --output build/spice-prof src/main.spice
```

`--native-features` tunes the code for the host CPU (the default is a generic CPU, like the release).

## 1. Built-in compiler timers — always start here

The compiler measures every pipeline stage (results in the compiler output's times). `-d` / `--debug-output` prints each
stage's runtime plus an end-of-run summary:

```sh
$SPICE build -d file.spice
```

Per-stage lines look like `[Type Checker Post] for file.spice: ... (12 ms; 2 run(s))`, covering: Parser, Import Collector,
Symbol Table Builder, Type Checker Pre/Post, IR Generator, IR Optimizer, Object Emitter. The summary reports
BlockAllocator bytes/allocations, the total type count, and **Total compile time** split into front end, middle end and
back end.

This tells you *which stage* dominates before you reach for a sampling profiler. Pair with `--dump-cache-stats` to see the
function/struct/interface lookup-cache hit rates (a low hit rate points at redundant resolution work). A good
representative input is the compiler itself: `$SPICE build -d -O0 src/main.spice` (with the env vars above).

## 2. Sampling-profile the compiler (perf)

```sh
perf record -g --call-graph dwarf -- build/spice-prof build -O2 --ignore-cache some-large-input.spice
perf report            # interactive; or: perf report --stdio | head -50
```

- `perf stat -- $SPICE build file.spice` for cycles/IPC/branch-miss/cache-miss counters without a full profile.
- Flamegraph:
  ```sh
  perf script | stackcollapse-perf.pl | flamegraph.pl > spice.svg
  ```
- The back end runs one pipeline per source file on a thread pool (`-j`); profile with `-j 1` to get a serial picture.

### valgrind / callgrind (instruction-exact)

```sh
valgrind --tool=callgrind --callgrind-out-file=cg.out $SPICE build --ignore-cache file.spice
callgrind_annotate cg.out | head -60     # or open cg.out in kcachegrind
```

Callgrind is ~20–50× slower but deterministic and gives exact instruction/call counts — good for comparing two compiler
revisions on the same input. (For leak/memory-error hunting rather than CPU profiling, see the `spice-debugging` skill.)

Memory usage: `valgrind --tool=massif` or `/usr/bin/time -v` (max RSS) on the same invocations.

## 3. Profile an emitted Spice binary

Compile the program optimized **with** debug info so the profiler can attribute samples to source, then treat it as any
native binary:

```sh
$SPICE build -O2 -g -o /tmp/prog file.spice

perf record -g --call-graph dwarf -- /tmp/prog <args>
perf report
# or instruction-exact:
valgrind --tool=callgrind --callgrind-out-file=cg.out /tmp/prog <args>
```

- Compare opt levels to see what the optimizer buys: `-O0`/`-O2`/`-O3`/`-Os`/`-Oz`.
- Cross-reference hot spots with `$SPICE build -O2 -ir file.spice` (IR) and `-s` (assembly) from the `spice-dump` skill to
  see whether a hot loop is being lowered as expected, or whether a missed optimization in the IR generator/optimizer is
  the real cause.

## 4. Wall-clock benchmarking (hyperfine)

For A/B timing of the compiler or an emitted binary, use a statistical benchmarker rather than a single `time` run:

```sh
# Double quotes so the outer shell expands $SPICE before hyperfine's child shell runs each command
hyperfine --warmup 3 \
  "$SPICE build -O2 --ignore-cache file.spice" \
  "old-spice build -O2 --ignore-cache file.spice"

hyperfine --warmup 3 '/tmp/prog <args>'      # emitted binary
```

Always `--warmup` (filesystem cache), pass `--ignore-cache` (otherwise the compilation cache skips the work) and benchmark
an optimized compiler — an `-O0` build's numbers are not comparable to anything shipped.

## 5. Choosing a tool

| Question | Reach for |
|----------|-----------|
| Which compiler *stage* is slow? | `$SPICE build -d` (built-in timers) |
| Which function in the compiler is hot? | `perf record` on an `-O2 -g` build |
| Exact instruction/call counts, compare two revisions | callgrind + `callgrind_annotate` |
| Which function in the *emitted* program is hot? | `$SPICE build -O2 -g`, then perf/callgrind |
| Is a change actually faster (wall clock)? | hyperfine on an optimized build |
| Redundant symbol resolution? | `--dump-cache-stats` |

## Related skills

- `spice-build` — building the compiler with the stage0 compiler.
- `spice-run` — compile/run a `.spice` file, opt levels and `-g`.
- `spice-dump` — IR/assembly dumps to confirm whether a hot spot is a codegen issue.
- `spice-debugging` — valgrind for *memory* errors/leaks (vs. CPU profiling here).
- `spice-architecture` — map a slow stage to the file that owns it.

# Benchmarks

Benchmark suite of the Spice compiler. It measures the compiler (compile time, memory usage, stage times) and the code it
emits (runtime, memory usage, executable size). The design is described in `media/specs/benchmarks.md`.

- `benchmarks.toml`: registry of the benchmarks, the default thresholds and the number of runs
- `compile/<name>/source.spice`: programs of the compile-time benchmarks
- `runtime/<name>/source.spice`: programs of the runtime benchmarks. Each takes its workload size as first argument
- `generators.py`: generators of the synthetic compile-time benchmarks
- `run-benchmarks.py`: the runner

## Requirements

Linux, Python 3.11+, `valgrind` (instruction counts), GNU `time` (peak memory usage) and `setarch` (disables ASLR). The
compilers under test need the `--dump-stats` option. The runner uses the std of the checkout, each compiler was built
from, and the LLVM installation like `build.py`.

## Usage

```sh
# Measure the current compiler
python benchmark/run-benchmarks.py run --compiler head=build/spice

# Compare against another checkout, e.g. a worktree of main
git worktree add ../spice-main main
(cd ../spice-main && python build.py)
python benchmark/run-benchmarks.py run --compiler base=../spice-main/build/spice --compiler head=build/spice
python benchmark/run-benchmarks.py compare build/benchmark-results.json --baseline base --candidate head

# Only some benchmarks, only instruction counts or only the timed runs
python benchmark/run-benchmarks.py run --compiler head=build/spice --filter 'runtime/*' --measures instructions

# Noise of the gated measures over repeated runs of the same compiler
python benchmark/run-benchmarks.py noise --compiler build/spice --repetitions 10

# Results of one compiler in the Bencher Metric Format
python benchmark/run-benchmarks.py export build/benchmark-results.json --compiler head
```

`compare` exits with 1, if a gated measure (instructions, peak memory usage, file size) regressed more than its
threshold. Wall times and stage times are only reported. The instruction counts are stable across runs and machines, the
times are not: compare times only within one `run` invocation.

## Adding a benchmark

- Compile time: add `compile/<name>/source.spice` (or a generator function in `generators.py`) and a `[[compile]]` entry.
- Runtime: add `runtime/<name>/source.spice`, that reads its workload size from `argv[1]` and prints a result, so that
  the work cannot be optimized away. Add a `[[runtime]]` entry with a small workload for the instruction count (a few
  seconds under cachegrind) and a larger one for the wall time (around 0.5 s).

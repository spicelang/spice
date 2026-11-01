# Technical Specification for the Benchmark Suite of Spice

The benchmark suite measures the performance of the compiler and of the code it emits: end-to-end compile time, the time
of each compile stage, peak memory usage of the compiler, runtime and size of the emitted executables. The results are
stored as a history per benchmark and visualized as trend graphs. Every pull request is benchmarked against its merge
base, and the check fails, if a benchmark regresses more than a configured threshold.

## Implementation steps:

- [x] Phase 1: compiler statistics
    - [x] `--dump-stats <file>` CLI option, that writes a JSON file
    - [x] Stage timers in microseconds, exclusive front-end and middle-end stage times, timers for LTO and linking
    - [x] Peak memory usage of the compiler process (`getPeakMemoryUsage` in `std/os/system`)
    - [x] Counters (source files, lines, types, type checker runs, IR functions and instructions)
    - [x] Tests and docs
- [ ] Phase 2: benchmark runner and benchmarks
    - [ ] `benchmark/run-benchmarks.py` with the `run` and `compare` commands
    - [ ] Benchmark registry `benchmark/benchmarks.toml`
    - [ ] Compile-time, runtime and self-compile benchmarks
    - [ ] A/A noise measurement, thresholds derived from it
- [ ] Phase 3: pre-merge check
    - [ ] `.github/workflows/benchmark.yml`, comparing the PR head against the merge base
    - [ ] Report as job summary and PR comment, `perf-regression-ok` label
- [ ] Phase 4: history and visualization
    - [ ] Bencher project, API token as repository secret
    - [ ] Upload the results of every push to `main` and of the nightly run
    - [ ] Trend graphs per benchmark and measure

## Design principles

- **Gate only on stable measures.** GitHub-hosted runners are shared VMs, wall-clock times vary by 5-20 % between runs.
  Only measures that are stable across runs (instruction counts, peak RSS with one job, sizes) can fail a check. Wall
  times are recorded and graphed, but never fail a check.
- **Compare within one job.** The pre-merge check builds the compilers of the merge base and of the PR head in the same
  job and benchmarks them on the same machine. It never compares against numbers from the history store, which come from
  other machines. The history is for trends only.
- **The gate does not depend on the history store.** The comparison and the pass/fail decision are done by the runner
  script, so the store can be exchanged and an outage of the store does not block merging.

## Measures

| Measure        | Unit         | Source                                                  | Stability | Gated | Default threshold |
|----------------|--------------|---------------------------------------------------------|-----------|-------|-------------------|
| `instructions` | instructions | `valgrind --tool=cachegrind --cache-sim=no`, `Ir` total | ~±0.1 %   | yes   | 2 %               |
| `max-rss`      | bytes        | compiler: `--dump-stats`; executables: `os.wait4`       | ~±1 %     | yes   | 5 %               |
| `file-size`    | bytes        | size of the emitted executable                          | exact     | yes   | 3 %               |
| `wall-time`    | ns           | runner, median of N interleaved runs                    | noisy     | no    | -                 |
| `stage-<name>` | µs           | `--dump-stats`                                          | noisy     | no    | -                 |
| `count-<name>` | count        | `--dump-stats`                                          | exact     | no    | -                 |

- The thresholds are the defaults. A benchmark can override them in the registry. The final values are derived from the
  A/A noise measurement (see below), e.g. `max(default, 5 σ)`.
- A measure regresses, if the candidate value exceeds the baseline value by more than the threshold. Improvements beyond
  the threshold are reported, but never fail the check.
- The counters are not gated, but shown in the report next to a regression, since they often explain it (e.g. more
  manifestations of a generic function).
- `instructions` is measured without `--trace-children`, so the linker invoked by the compiler is not counted.
- Wall times are compared with a Mann-Whitney U test. A difference of more than 10 % with `p < 0.01` is highlighted in
  the report as a warning.

### Determinism

The instruction count of a compiler run must not depend on anything else than the compiler and the input:

- All compiler runs pass `-j 1`. The parallel back end makes RSS and the order of work depend on thread scheduling.
- The runner disables ASLR (`setarch -R`), so that the iteration order of pointer-keyed hash maps is stable.
- Output files are written to fixed paths in a temporary directory with the same path length for every compiler.
- The base and the head compiler use the std of their own checkout. Different path lengths of the two checkouts change
  the instruction count by far less than the threshold.

The A/A measurement (the same commit benchmarked against itself, 10 times) verifies this before the gate is enabled and
after changes to the runner.

## Compiler statistics (`--dump-stats`)

The option `--dump-stats <file>` of `build`, `run`, `test`, `lint` and `install` writes the statistics of the
compilation as JSON file, after the compilation and the linker have finished. Implemented in
`src/global/compile-stats.spice`.

```json
{
  "version": 1,
  "compiler": { "version": "0.30.0", "gitHash": "9ec464eb", "llvmVersion": "23.1.4" },
  "options": { "optLevel": "O0", "lto": false, "backend": "llvm", "jobs": 1 },
  "wallTime": { "frontEnd": 6566, "middleEnd": 48347, "backEnd": 79376, "linker": 22889, "total": 157476 },
  "stages": {
    "lexerAndParser": 14019, "importCollector": 1909, "symbolTableBuilder": 2527, "typeCheckerPre": 10246,
    "typeCheckerPost": 25212, "irGenerator": 25489, "irOptimizer": 2519, "bitcodeLinker": 0, "ltoOptimizer": 0,
    "objectEmitter": 50048
  },
  "counters": {
    "sourceFiles": 19, "lines": 3812, "types": 360, "typeCheckerRuns": 29, "irFunctions": 271, "irInstructions": 8615
  },
  "peakMemoryBytes": 104304640
}
```

- All times are in µs. `wallTime` is measured by the global timers of the `GlobalResourceManager` and by a timer around
  the whole compilation (`total`, from the start of `compileProject` until the statistics are written).
- `stages` are the sums of the per-file stage times (`TimerOutput` in `src/source-file.spice`) over all source files.
- The front-end and middle-end stages nest: the import collector runs the front ends of the imported files, and the type
  checker loads runtime modules, which runs their front end and type checker. `SourceFile.startStageTimer` pauses the
  timer of the enclosing stage, while a nested stage runs (stack of running timers in the `GlobalResourceManager`), so
  each stage is measured exclusively and the stage times add up to at most the wall time. A stage time accumulates over
  repeated runs of the stage (e.g. type checker revisits). The back-end stages do not nest and run in parallel, so with
  `-j > 1` they add up to more than `wallTime.backEnd`.
- With LTO, `irOptimizer` is the pre-link optimization of the source files, `bitcodeLinker` and `ltoOptimizer` are the
  linking and the post-link optimization of the whole program.
- `irFunctions` (functions with a body) and `irInstructions` are counted on the module, that the object emitter emits:
  one module per source file, or the LTO module. They are only counted, if the statistics are requested.
- `peakMemoryBytes` is the peak resident set size of the compiler process (`getPeakMemoryUsage`: `getrusage` on Linux
  and macOS, `K32GetProcessMemoryInfo` on Windows). The linker runs in a process of its own and is not included.
- The format is versioned. Fields can be added without a version bump, the runner ignores unknown fields.

## Benchmarks

Layout:

```
benchmark/
├── run-benchmarks.py
├── benchmarks.toml
├── compile/<name>/source.spice
└── runtime/<name>/source.spice
```

The programs in `test/test-files/benchmark` are tests with small inputs and reference outputs. The benchmark programs
are separate, but can start out as copies of them.

### Kinds

- **`compile`**: compiles a program and measures the compiler. Measures: `instructions`, `max-rss`, `wall-time`,
  `stage-*`, `count-*`. Runs once per configured optimization level and backend.
- **`runtime`**: compiles a program (not measured) and measures the emitted executable. Measures: `instructions`,
  `max-rss`, `wall-time`, `file-size`. Each program takes its workload size as a command line argument, so that the
  instruction count can be measured on a small workload and the wall time on a large one.
- **`self-compile`**: compiles `src/main.spice` with the flags that `build.py` uses for a Release build (the runner
  takes them from `BUILD_FLAGS` in `build.py`, which moves behind a `__main__` guard, so both stay in sync). The output
  is the stage1 compiler. Measures: `max-rss`, `wall-time`, `stage-*`, `count-*`, `file-size` of the stage1 compiler;
  `instructions` only in the nightly run, because of the cachegrind slowdown.
- **`self-compile-stage1`**: the same compilation, but done by the stage1 compiler of the `self-compile` benchmark. The
  compiler under test is built by the stage0 compiler, so changes to the code generation do not show in its own
  performance. The stage1 compiler shows them on a large real-world program.

### Initial set

| Name                     | Kind     | Covers                                                    |
|--------------------------|----------|-----------------------------------------------------------|
| `hello-world`            | compile  | startup, std and runtime module import overhead           |
| `std-generics`           | compile  | manifestations of `Vector`, `Map`, `String`, `Result`     |
| `many-instantiations`    | compile  | synthetic: thousands of generic function/struct instances |
| `deep-imports`           | compile  | synthetic: long import chains, many source files          |
| `large-function`         | compile  | synthetic: one function with thousands of statements      |
| `binary-trees`           | runtime  | heap allocation, ctors/dtors                              |
| `n-body`                 | runtime  | floating point arithmetic, structs                        |
| `fannkuch`               | runtime  | arrays, integer arithmetic                                |
| `pidigits`               | runtime  | arbitrary precision arithmetic                            |
| `hash-map`               | runtime  | `std/data/hash-table` inserts, lookups, removals          |
| `string-builder`         | runtime  | `String` appends, reallocation                            |
| `regex`                  | runtime  | `std/text/regex` matching                                 |
| `self-compile`           | special  | the whole compiler                                        |
| `self-compile-stage1`    | special  | codegen quality on a large program                        |

The compile benchmarks run with `-O0` and `-O2`, with the LLVM backend and, where supported, the TPDE backend. The
benchmark ID is `<kind>/<name>/<config>`, e.g. `compile/std-generics/O2-llvm`.

### Registry

```toml
[defaults.thresholds]
instructions = 2.0
max-rss = 5.0
file-size = 3.0

[[compile]]
name = "std-generics"
configs = ["O0-llvm", "O2-llvm", "O0-tpde"]

[[runtime]]
name = "binary-trees"
opt-level = "O2"
args-instructions = ["12"]
args-wall-time = ["18"]
thresholds = { max-rss = 10.0 }
```

## Runner (`benchmark/run-benchmarks.py`)

```
python benchmark/run-benchmarks.py run --compiler base=build/bench-base/spice --compiler head=build/spice \
    [--suite pr|full] [--filter 'compile/*'] [--runs 5] --output results.json
python benchmark/run-benchmarks.py compare results.json --baseline base --candidate head \
    [--markdown report.md] [--report-only]
```

- `run` measures every benchmark with every given compiler. Wall-time runs are interleaved between the compilers
  (`ABAB...`), after one warm-up run each, so that drifts of the machine hit both compilers equally.
- `--suite pr` skips the nightly-only measures (`instructions` of `self-compile`), `--suite full` runs everything.
- The output contains, per compiler, the results in the Bencher Metric Format (BMF) plus metadata (commit, stage0
  version, LLVM version, runner image, CPU model):

  ```json
  {
    "metadata": { "head": { "commit": "...", "stage0": "0.29.2", "llvm": "23.1.0" }, "base": { } },
    "results": {
      "head": {
        "compile/std-generics/O2-llvm": {
          "instructions": { "value": 1234567890 },
          "wall-time": { "value": 812000000, "lower_value": 801000000, "upper_value": 840000000 }
        }
      }
    }
  }
  ```

  BMF is the native upload format of Bencher and can be converted for other stores.
- `compare` prints a table of all gated measures, the counters and the highlighted wall-time differences. It exits with
  1, if a gated measure regressed, unless `--report-only` is given.
- Only the Python standard library is used (`tomllib`, `statistics`, `os.wait4`). The Mann-Whitney U test is implemented
  in the script.
- The runner only supports Linux, since the gate runs on Linux only. Running it locally gives the same numbers as CI
  only for `instructions`.

## Pre-merge check (`.github/workflows/benchmark.yml`)

```
on: pull_request (opened, synchronize, reopened, labeled, unlabeled), push to main, schedule (nightly),
    workflow_dispatch
```

- Runs on `ubuntu-26.04` (Linux/x86_64) only. One platform is enough to detect regressions, and instruction counts on
  different platforms are not comparable anyway.
- `paths-ignore` like in `ci.yml` (`docs/**`, `media/**`, `**.md`).
- A `concurrency` group per PR cancels outdated runs.

### Pull requests

1. Check out the PR head and the merge base (`git worktree add build/bench-base <merge-base>`).
2. Set up the toolchain and LLVM with the existing composite actions, `setup-deps.py --tpde`, install `valgrind`.
3. Build both compilers with `build.py`, each with the stage0 compiler pinned in its own checkout. A PR that bumps the
   stage0 version is thereby benchmarked correctly.
4. `run-benchmarks.py run --suite pr` with both compilers.
5. `run-benchmarks.py compare`. The report is written to the job summary (`$GITHUB_STEP_SUMMARY`) and uploaded as an
   artifact together with `results.json`.
6. The step fails, if a gated measure regressed and the PR does not have the `perf-regression-ok` label. With the label,
   `--report-only` is passed. Adding or removing the label re-runs the workflow.

The PR comment is posted by a second workflow (`benchmark-comment.yml`, triggered by `workflow_run`), which downloads
the report artifact. The token of the benchmark workflow has no write access for PRs from forks. The comment is updated
in place on new runs instead of adding a new one.

Time budget: about 30 minutes, dominated by the two compiler builds and the two self-compile benchmarks.

### Pushes to `main` and nightly runs

- Only the head compiler is benchmarked. Pushes to `main` run `--suite pr`, the nightly run `--suite full`.
- The results are uploaded to the history store. Nothing is gated.

## History and visualization

The history is stored in [Bencher](https://bencher.dev): BMF is its native upload format, it supports multiple measures
per benchmark, draws the trend graphs and is free for open-source projects. It can be self-hosted later, if needed.

- The `main` push and nightly runs upload their BMF results with `bencher run --file results.json --adapter json`, on
  branch `main` and testbed `ubuntu-26.04-x86_64`, with the commit hash.
- The API token is the repository secret `BENCHER_API_TOKEN`. Only the upload on `main` uses it, never a PR run.
- The custom measures (`instructions`, `max-rss`, `file-size`, `stage-*`, `count-*`) are created in the Bencher project
  with their units. Bencher's own thresholds are not used, the gate is done by the runner (see design principles).

A stage0 bump changes all compile-time numbers at once. The metadata contains the stage0 version, so that such steps in
the graphs can be attributed.

## Future work

- Per-stage instruction counts with `callgrind --toggle-collect` on the stage entry functions, which would make the
  stage measures gateable.
- AArch64 runs, in case of architecture-specific regressions.
- A self-hosted runner with a fixed CPU frequency, to gate on wall time.
- More benchmarks, e.g. extracted from real-world Spice projects.

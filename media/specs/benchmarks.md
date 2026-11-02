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
- [x] Phase 2: benchmark runner and benchmarks
    - [x] `benchmark/run-benchmarks.py` with the `run`, `compare`, `noise` and `export` commands
    - [x] Benchmark registry `benchmark/benchmarks.toml`
    - [x] Compile-time, runtime and self-compile benchmarks
    - [x] A/A noise measurement (local), thresholds derived from it. To be repeated on the CI runner in phase 3
- [ ] Phase 3: pre-merge check
    - [x] `.github/workflows/benchmark.yml`, comparing the PR head against the merge base
    - [x] Report as job summary and PR comment (`benchmark-comment.yml`), `perf-regression-ok` label
      (`benchmark-label.yml`)
    - [ ] A/A noise measurement on the CI runner (manual run of the workflow), before `Benchmark gate` is made a
      required check
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
| `max-rss`      | bytes        | compiler: `--dump-stats`; executables: GNU `time`       | ~±1 %     | yes   | 5 %               |
| `file-size`    | bytes        | size of the emitted executable                          | exact     | yes   | 3 %               |
| `wall-time`    | ns           | runner, median of N interleaved runs                    | noisy     | no    | -                 |
| `stage-<name>` | µs           | `--dump-stats`                                          | noisy     | no    | -                 |
| `count-<name>` | count        | `--dump-stats`                                          | exact     | no    | -                 |

- The thresholds are the defaults. A benchmark can override them in the registry. The values are checked against the
  A/A noise measurement (see below): `max(default, 5 σ)`.
- A measure regresses, if the candidate value exceeds the baseline value by more than the threshold and by more than the
  minimum delta of the measure (`max-rss`: 1 MiB). Improvements beyond both are reported, but never fail the check.
- The counters are not gated, but shown in the report next to a regression, since they often explain it (e.g. more
  manifestations of a generic function).
- `instructions` is measured without `--trace-children`, so the linker invoked by the compiler is not counted.
- Wall times are compared with a Mann-Whitney U test. A difference of more than 10 % and 1 ms with `p < 0.01` is
  highlighted in the report as a warning. The absolute minimum keeps the stage times of small programs (a few µs) out.

### Determinism

The instruction count of a compiler run must not depend on anything else than the compiler and the input:

- All compiler runs pass `-j 1`. The parallel back end makes RSS and the order of work depend on thread scheduling.
- The runner disables ASLR (`setarch -R`), so that the iteration order of pointer-keyed hash maps is stable.
- The compiler embeds the paths of the source files relative to its working directory, e.g. in panic messages. So every
  compiler runs in a work dir in its own checkout (`build/bench-tmp/c<idx>/<benchmark>`) and compiles a copy of the
  benchmark sources in that work dir. The relative paths to the sources and to the std of its checkout are then the
  same for every compiler. With a shared work dir, the base compiler saw `../../../../../base/std` instead of
  `../../../../std`, which changed the instruction counts by up to 0.36 % and the file sizes by 64 to 128 bytes in the
  first CI run.
- The base and the head compiler use the std of their own checkout. The CI checks them out to paths of the same length
  (`head/` and `base/`), so that the absolute paths do not differ in length either. Their content still changes the
  instruction counts a little (e.g. hashes of path strings): `compile/std-generics/O0-llvm` executes 0.04 % more
  instructions with the std in `.../base/std` than in `.../head/std`. In an A/A comparison of two checkouts of the same
  commit, the instruction counts differ by up to 0.13 %, the file sizes not at all. The thresholds cover that; the same
  absolute path for both compilers would need a mount namespace per process.

The A/A measurement (the same commit benchmarked against itself, 10 times) verifies this before the gate is enabled and
after changes to the runner.

### A/A noise measurement (local)

`run-benchmarks.py noise --repetitions 10` on a Ryzen 9 5950X (2026-11-01, 1b0b3f8d):

- `instructions`: identical in all 10 repetitions for every benchmark (σ = 0).
- `file-size`: identical for every benchmark, also between independent builds of the same program.
- `max-rss`: σ ≤ 0.9 % and a maximum deviation ≤ 2 % for everything above 10 MiB (the compiler always uses more than
  48 MiB, the self-compile 592 MiB with σ = 0.08 %). The small runtime programs fluctuate more in relative terms:
  `regex` (4.4 MiB) by up to 11.7 %, `n-body` (1.9 MiB) by up to 6.2 %, but by at most 0.52 MiB in absolute terms. The
  minimum delta of 1 MiB covers them, so the default thresholds hold for all benchmarks.

The measurement has to be repeated on the CI runner (manual run of the `Benchmark` workflow, see below), before the
check is made a required status check.

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
├── README.md
├── run-benchmarks.py
├── benchmarks.toml
├── generators.py
├── compile/<name>/source.spice
└── runtime/<name>/source.spice
```

The programs in `test/test-files/benchmark` are tests with small inputs and reference outputs. The benchmark programs
are separate.

### Kinds

- **`compile`**: compiles a program and measures the compiler. Measures: `instructions`, `max-rss`, `wall-time`,
  `stage-*`, `count-*`. Runs once per configured optimization level and backend. The source is either
  `benchmark/compile/<name>/source.spice` or generated by a function in `benchmark/generators.py` with a given scale, so
  that large synthetic programs need not be checked in.
- **`runtime`**: compiles a program (not measured) and measures the emitted executable. Measures: `instructions`,
  `max-rss`, `wall-time`, `file-size`. Each program takes its workload size as first command line argument, so that the
  instruction count is measured on a small workload and the wall time on a large one. Each program prints a result, so
  that the work cannot be optimized away.
- **`self-compile`**: compiles `src/main.spice` of the compiler's own checkout with the flags of the Release build type
  (`BUILD_TYPE_FLAGS` in `bootstrap.py`, which `build.py` uses as well). The output is the stage1 compiler. Measures:
  `max-rss`, `wall-time`, `stage-*`, `count-*`, `file-size` of the stage1 compiler; `instructions` only in the `full`
  suite, because of the cachegrind slowdown. A PR that changes the compiler sources changes the input as well, the
  counters (`count-lines`, `count-ir-instructions`) show that.
- **`self-compile-stage1`**: the same compilation, but done by the stage1 compiler of the `self-compile` benchmark. The
  compiler under test is built by the stage0 compiler, so changes to the code generation do not show in its own
  performance. The stage1 compiler shows them on a large real-world program.

### Initial set

| Name                  | Kind    | Covers                                                                      |
|-----------------------|---------|-----------------------------------------------------------------------------|
| `hello-world`         | compile | fixed cost: startup, runtime modules, object emission, linking              |
| `std-generics`        | compile | manifestations of `Vector`, `Map`, `UnorderedMap`, `Set`, `PriorityQueue`, `Pair`, `Optional`, `Result`, algorithms with lambdas |
| `many-instantiations` | compile | generated: 100 struct types, each with a generic struct, function and `Vector` |
| `deep-imports`        | compile | generated: chain of 100 source files, each importing the next one          |
| `large-function`      | compile | generated: one function with 1000 (O0) or 200 (O2) blocks of statements     |
| `binary-trees`        | runtime | heap allocation, recursion                                                  |
| `n-body`              | runtime | floating point arithmetic, struct fields                                    |
| `fannkuch`            | runtime | array accesses, integer arithmetic                                          |
| `sieve`               | runtime | `Vector<bool>` accesses in tight loops                                      |
| `hash-map`            | runtime | `UnorderedMap` inserts, lookups, removals                                   |
| `string-builder`      | runtime | `String` appends, reallocation, number formatting                           |
| `regex`               | runtime | `std/text/regex` matching                                                   |
| `self-compile`        | special | the whole compiler                                                          |
| `self-compile-stage1` | special | codegen quality on a large program                                          |

The compile benchmarks run with `-O0` and `-O2` with the LLVM backend, the small ones with `-O0` and the TPDE backend as
well. TPDE benchmarks are skipped, if the std of a compiler under test does not ship TPDE. The benchmark ID is
`<kind>/<name>/<config>`, e.g. `compile/std-generics/O2-llvm`, `runtime/sieve/O2` or `self-compile/release`.

The O2 compile time of `large-function` grows steeply with the size of the function: about 1 s for 200 blocks, 5.5 s for
400 and 44 s for 1000. This is worth an investigation of its own.

### Registry (`benchmark/benchmarks.toml`)

```toml
[defaults]
runs = 5   # Measured wall-time runs per benchmark and compiler
warmup = 1 # Unmeasured runs before

[defaults.thresholds] # Percent
instructions = 2.0
max-rss = 5.0
file-size = 3.0

[defaults.min-deltas] # Absolute, in the unit of the measure
max-rss = 1048576

[defaults.warnings] # Highlighting of time differences
percent = 10.0
min-delta-ms = 1.0
p-value = 0.01

[[compile]]
name = "large-function"
generator = "large_function"
scale = 1000
configs = ["O0-llvm"]

[[runtime]]
name = "binary-trees"
opt-level = "O2"
args-instructions = ["12"]
args-wall-time = ["17"]
thresholds = { max-rss = 10.0 } # Optional override

[self-compile]
build-type = "Release"
runs = 5
warmup = 0
```

## Runner (`benchmark/run-benchmarks.py`)

```
python benchmark/run-benchmarks.py run --compiler base=../base/build/spice --compiler head=build/spice \
    [--suite pr|full] [--filter 'compile/*'] [--exclude '...'] [--measures instructions,timed] [--runs 5] \
    [--output build/benchmark-results.json]
python benchmark/run-benchmarks.py compare build/benchmark-results.json --baseline base --candidate head \
    [--markdown report.md] [--report-only]
python benchmark/run-benchmarks.py noise --compiler build/spice [--repetitions 10]
python benchmark/run-benchmarks.py export build/benchmark-results.json --compiler head
```

- `run` measures every benchmark with every given compiler. The timed runs are interleaved between the compilers
  (`ABAB...`), after the warm-up runs, so that drifts of the machine hit all compilers equally. Then the instructions
  are counted, one cachegrind run per compiler.
- Each compiler uses the std of the checkout it lives in (the first parent directory with `std/` and `src/main.spice`),
  via `SPICE_STD_DIR`. The self-compile compiles the sources of that checkout.
- `--suite pr` skips the instruction count of the self-compile benchmarks, `--suite full` measures everything.
  `--measures` restricts the measure groups: `instructions` (cachegrind) and `timed` (all other measures).
- The results file contains the metadata of each compiler (commit, stage0 version, CPU, kernel, runner image), the
  thresholds of each benchmark, and per compiler the results in BMF and the raw samples of the timed measures:

  ```json
  {
    "version": 1,
    "metadata": { "head": { "commit": "...", "stage0": "0.29.2", "cpu": "...", "runnerImage": "..." } },
    "config": { "suite": "pr", "thresholds": { "runtime/sieve/O2": { "instructions": 2.0 } }, "warnings": { } },
    "results": {
      "head": {
        "compile/std-generics/O2-llvm": {
          "instructions": { "value": 5120098304 },
          "wall-time": { "value": 815200000, "lower_value": 801000000, "upper_value": 840000000 }
        }
      }
    },
    "samples": { "head": { "compile/std-generics/O2-llvm": { "wall-time": [815200000, ...] } } }
  }
  ```

- `compare` prints a Markdown report: the gated measures, that changed, the time differences above the warning limit,
  all wall times (collapsed) and the changed counters. It exits with 1, if a gated measure regressed, unless
  `--report-only` is given. Benchmarks with results of only one compiler are listed, but not compared.
- `noise` runs the suite repeatedly with the same compiler (one timed run per repetition, interleaved) and prints the
  standard deviation and the maximum deviation of every gated measure, with the suggested threshold
  `max(default, 5 σ)`.
- `export` prints the BMF results of one compiler, the input of the Bencher upload.
- Only the Python standard library is used (`tomllib`, `statistics`). The Mann-Whitney U test is implemented in the
  script: exact for small samples without ties, normal approximation otherwise.
- The runner needs Linux, `valgrind`, GNU `time` and `setarch`. Running it locally gives the same numbers as CI only for
  `instructions`.

### Peak memory usage

Python starts processes with `vfork`, and Linux keeps the peak RSS across `exec`. So the peak RSS of a direct child of
the runner, and the one the compiler reads with `getrusage`, is at least the RSS of the Python process (about 12 MiB
for a program, that needs 2 MiB). The runner therefore launches every timed run via GNU `time`, which forks from a small
process: for executables it takes the peak RSS reported by `time`, for the compiler the `peakMemoryBytes` of
`--dump-stats`, which does not include the linker.

### Wall time

The runner waits for the processes with a blocking wait and a timer for the timeout. `Popen.wait` with a timeout polls
with sleeps of up to 50 ms, which quantizes the wall times.

### Duration

On a Ryzen 9 5950X, the `pr` suite takes about 8.5 minutes per compiler, of which the two self-compile benchmarks take
about 5 minutes (28 s per compilation, 5 runs each) and the O2 compile benchmarks under cachegrind most of the rest. The
self-compile needs 5 runs, because the Mann-Whitney U test cannot reach p < 0.01 with fewer samples (the minimum p for 3
vs. 3 samples is 0.1).

## Pre-merge check (`.github/workflows/benchmark.yml`)

```
on: pull_request (opened, synchronize, reopened), workflow_dispatch (A/A noise measurement)
    Phase 4 adds: push to main, schedule (nightly)
```

- Runs on `ubuntu-26.04` (Linux/x86_64) only. One platform is enough to detect regressions, and instruction counts on
  different platforms are not comparable anyway.
- A `concurrency` group per PR cancels outdated runs.
- Three jobs: `Benchmark changes` decides, whether the PR changes anything, that affects the benchmarks.
  `Benchmark - Linux/x86_64` measures and reports. `Benchmark gate` decides pass or fail and is the check to require.
- No `paths-ignore`, because a required check, whose workflow is filtered out, stays pending. Instead, the changes job
  lists the files of the PR via the API, and the benchmark job is skipped, if all of them match `docs/**`, `media/**`
  or `**.md` (like the `paths-ignore` of `ci.yml`). The gate passes then.
- No `labeled`/`unlabeled` triggers. Every label event (e.g. the size label of `pull-request-size[bot]`) would create a
  run, and the skipped check run of that run would hide the result of the real one in the PR, and count as passed for a
  required check. The label is handled by `benchmark-label.yml` instead (see below).

### Pull requests

1. Check out the PR head (not the merge commit) to `head/`, with the full history, and the merge base of the head and
   the base branch to `base/` (`git worktree add`). The two paths have the same length (see determinism).
2. Set up the toolchain and LLVM with the existing composite actions of the head checkout, `setup-deps.py --tpde` (in
   both checkouts), install `valgrind` and `time`. `CCACHE_BASEDIR` lets the TPDE build of the base checkout reuse the
   ccache entries of the head checkout.
3. Build both compilers with `build.py`, each with the stage0 compiler pinned in its own checkout. A PR that bumps the
   stage0 version is thereby benchmarked correctly. LLVM is the one of the head checkout for both, so the effect of an
   LLVM bump on the compiler is not measured.
4. `head/benchmark/run-benchmarks.py run --suite pr` with both compilers. Both are measured with the benchmarks and
   the runner of the head checkout.
5. `run-benchmarks.py compare`. The report is written to the job summary (`$GITHUB_STEP_SUMMARY`) and uploaded as the
   artifact `benchmark-report`, together with `results.json` and the flag, whether a gated measure regressed. The
   benchmark job itself does not fail on regressions.
6. The gate fails, if the benchmark job failed, or if a gated measure regressed and the PR does not have the label
   `perf-regression-ok`. It reads the current labels of the PR via the API, not the ones of the event payload.

Adding or removing `perf-regression-ok` triggers `benchmark-label.yml` (`pull_request_target`, so that it can re-run
jobs for PRs from forks as well; it never checks out code of the PR). It re-runs only the gate job of the latest
completed benchmark run of the PR head, which reuses the outputs of the benchmark job, instead of benchmarking again.
A run, that is still in progress, needs no re-run: its gate reads the labels, once it runs.

The PR comment is posted by a second workflow (`benchmark-comment.yml`, triggered by `workflow_run`), which downloads
the report artifact. The token of the benchmark workflow has no write access for PRs from forks. The comment workflow
runs in the context of the default branch and never executes code of the PR. The artifact is written by the PR's code,
so it is only used as comment text: the PR is the unique open PR of the head repository and branch of the run, whose
head is still the benchmarked commit (which also drops reports of outdated commits). The status line (regression
accepted by the label or not) is computed from the current labels, so a re-run of the gate updates it. The comment is
identified by a marker and updated in place on new runs. `benchmark-comment.yml` and `benchmark-label.yml` only run,
once they are on the default branch.

Time budget: about 30 minutes, dominated by the two compiler builds and the two self-compile benchmarks. The first run
(PR #1704) took 29.5 minutes.

### A/A noise measurement

The manual run (`workflow_dispatch`, input `repetitions`, default 10) builds only the head compiler and runs
`run-benchmarks.py noise --suite pr`. The noise report is written to the job summary and uploaded as artifact. It is
repeated after changes to the runner or the runner image, and the thresholds in `benchmarks.toml` are raised, if they do
not cover the noise.

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

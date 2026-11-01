#!/usr/bin/env python3
"""Benchmark runner for the Spice compiler. See media/specs/benchmarks.md for the design.

Commands:
  run      Measure the benchmarks with one or more compilers and write the results as JSON
  compare  Compare the results of two compilers, print a Markdown report and fail on regressions of gated measures
  noise    Measure the benchmarks repeatedly with the same compiler, to derive the thresholds of the gated measures
  export   Print the results of one compiler in the Bencher Metric Format (BMF)
"""
import argparse
import fnmatch
import json
import math
import os
import platform
import re
import shutil
import statistics
import subprocess
import sys
import threading
import time
import tomllib
from dataclasses import dataclass, field
from pathlib import Path

sys.dont_write_bytecode = True
BENCHMARK_DIR = Path(__file__).resolve().parent
ROOT_DIR = BENCHMARK_DIR.parent
sys.path.insert(0, str(ROOT_DIR))
sys.path.insert(0, str(BENCHMARK_DIR))
import bootstrap  # noqa: E402
import generators  # noqa: E402

RESULTS_FORMAT_VERSION = 1
GATED_MEASURES = ("instructions", "max-rss", "file-size")
# Measures with multiple samples, that are compared with the Mann-Whitney U test
SAMPLED_MEASURE_PREFIXES = ("wall-time", "stage-")
MEASURE_GROUPS = ("instructions", "timed")
EXE_SUFFIX = ".exe" if sys.platform == "win32" else ""


def log(msg: str) -> None:
    print(msg, file=sys.stderr, flush=True)


def fail(msg: str) -> None:
    bootstrap.fail(msg)


def to_kebab_case(name: str) -> str:
    return re.sub(r"(?<!^)(?=[A-Z])", "-", name).lower()


# ─── Compilers ───────────────────────────────────────────────────────────────


@dataclass
class Compiler:
    name: str
    path: Path
    root: Path  # Checkout, the compiler was built from. Its std is used, and its sources are compiled by the self-compile
    work_dir: Path
    env: dict[str, str]
    supports_tpde: bool


def find_checkout_root(compiler_path: Path) -> Path | None:
    for parent in compiler_path.parents:
        if (parent / "std").is_dir() and (parent / "src" / "main.spice").is_file():
            return parent
    return None


def std_ships_tpde(root: Path) -> bool:
    tpde_dir = root / "std" / "bindings" / "tpde"
    return (tpde_dir / "include" / "tpde-llvm" / "LLVMCompiler.hpp").is_file() and (tpde_dir / "lib" / "libtpde_llvm.a").is_file()


def compiler_env(root: Path) -> dict[str, str]:
    env = dict(os.environ)
    env["SPICE_STD_DIR"] = str(root / "std")
    # The self-compile links the LLVM bindings of the std, which need the LLVM libraries and headers (like build.py)
    if "LLVM_LIB_DIR" not in env and (llvm_lib_dir := bootstrap.find_llvm_lib_dir()):
        env["LLVM_LIB_DIR"] = llvm_lib_dir
    if "LLVM_INCLUDE_DIRS" not in env and (llvm_include_dirs := bootstrap.find_llvm_include_dirs()):
        env["LLVM_INCLUDE_DIRS"] = llvm_include_dirs
    return env


def parse_compilers(specs: list[str], work_dir: Path) -> list[Compiler]:
    compilers = []
    for idx, spec in enumerate(specs):
        name, sep, path_str = spec.partition("=")
        if not sep or not name or not path_str:
            fail(f"Invalid compiler '{spec}', expected <name>=<path>")
        if any(c.name == name for c in compilers):
            fail(f"Compiler name '{name}' is used twice")
        path = Path(path_str).resolve()
        if not path.is_file():
            fail(f"Compiler '{name}' not found: {path}")
        root = find_checkout_root(path)
        if root is None:
            fail(f"No Spice checkout (with std/ and src/main.spice) found above the compiler '{name}': {path}")
        # Same path length for every compiler, so that the paths in the compiler do not change the instruction count
        compiler_work_dir = work_dir / f"c{idx:02d}"
        compilers.append(Compiler(name, path, root, compiler_work_dir, compiler_env(root), std_ships_tpde(root)))
    return compilers


def git_commit(root: Path) -> str:
    result = subprocess.run(["git", "-C", str(root), "rev-parse", "HEAD"], capture_output=True, text=True)
    return result.stdout.strip() if result.returncode == 0 else "unknown"


def cpu_model() -> str:
    try:
        for line in Path("/proc/cpuinfo").read_text().splitlines():
            if line.startswith("model name"):
                return line.split(":", 1)[1].strip()
    except OSError:
        pass
    return platform.processor() or "unknown"


def compiler_metadata(compiler: Compiler) -> dict:
    stage0_version_file = compiler.root / ".github" / "stage0-version"
    return {
        "path": str(compiler.path),
        "commit": git_commit(compiler.root),
        "stage0": stage0_version_file.read_text().strip() if stage0_version_file.is_file() else "unknown",
        "cpu": cpu_model(),
        "kernel": platform.release(),
        "runnerImage": f"{os.environ.get('ImageOS', 'local')}-{os.environ.get('ImageVersion', '')}".rstrip("-"),
    }


# ─── Benchmarks ──────────────────────────────────────────────────────────────


@dataclass
class Benchmark:
    id: str
    kind: str  # compile, runtime, self-compile, self-compile-stage1
    flags: list[str]
    source: Path | None = None  # Fixed source (compile/runtime). The self-compile takes the sources of each compiler
    generator: str | None = None
    scale: int = 0
    args_instructions: list[str] = field(default_factory=list)
    args_wall_time: list[str] = field(default_factory=list)
    runs: int = 5
    warmup: int = 1
    thresholds: dict[str, float] = field(default_factory=dict)
    uses_tpde: bool = False


def config_flags(config: str) -> tuple[list[str], bool]:
    opt_level, sep, backend = config.partition("-")
    if not sep or opt_level not in ("O0", "O1", "O2", "O3", "Os", "Oz") or backend not in ("llvm", "tpde"):
        fail(f"Invalid benchmark config '{config}', expected '<opt level>-<backend>', e.g. 'O2-llvm'")
    return [f"-{opt_level}", f"--backend={backend}"], backend == "tpde"


def load_registry(path: Path) -> tuple[list[Benchmark], dict]:
    with path.open("rb") as f:
        registry = tomllib.load(f)
    defaults = registry.get("defaults", {})
    default_runs = defaults.get("runs", 5)
    default_warmup = defaults.get("warmup", 1)
    default_thresholds = defaults.get("thresholds", {})
    benchmarks = []

    for entry in registry.get("compile", []):
        name = entry["name"]
        source = None
        if "generator" not in entry:
            source = BENCHMARK_DIR / "compile" / name / "source.spice"
            if not source.is_file():
                fail(f"Source of the compile benchmark '{name}' not found: {source}")
        elif not hasattr(generators, entry["generator"]):
            fail(f"Generator '{entry['generator']}' of the compile benchmark '{name}' not found in generators.py")
        for config in entry["configs"]:
            flags, uses_tpde = config_flags(config)
            benchmarks.append(Benchmark(
                id=f"compile/{name}/{config}", kind="compile", flags=flags, source=source, generator=entry.get("generator"),
                scale=entry.get("scale", 0), runs=entry.get("runs", default_runs), warmup=entry.get("warmup", default_warmup),
                thresholds={**default_thresholds, **entry.get("thresholds", {})}, uses_tpde=uses_tpde))

    for entry in registry.get("runtime", []):
        name = entry["name"]
        source = BENCHMARK_DIR / "runtime" / name / "source.spice"
        if not source.is_file():
            fail(f"Source of the runtime benchmark '{name}' not found: {source}")
        opt_level = entry.get("opt-level", "O2")
        benchmarks.append(Benchmark(
            id=f"runtime/{name}/{opt_level}", kind="runtime", flags=[f"-{opt_level}"], source=source,
            args_instructions=entry.get("args-instructions", []), args_wall_time=entry.get("args-wall-time", []),
            runs=entry.get("runs", default_runs), warmup=entry.get("warmup", default_warmup),
            thresholds={**default_thresholds, **entry.get("thresholds", {})}))

    if self_compile := registry.get("self-compile"):
        build_type = self_compile.get("build-type", "Release")
        if build_type not in bootstrap.BUILD_TYPE_FLAGS:
            fail(f"Unknown build type '{build_type}' of the self-compile benchmark")
        for kind in ("self-compile", "self-compile-stage1"):
            benchmarks.append(Benchmark(
                id=f"{kind}/{build_type.lower()}", kind=kind, flags=list(bootstrap.BUILD_TYPE_FLAGS[build_type]),
                runs=self_compile.get("runs", default_runs), warmup=self_compile.get("warmup", 0),
                thresholds={**default_thresholds, **self_compile.get("thresholds", {})}))

    return benchmarks, defaults


def matches_any(benchmark_id: str, patterns: list[str]) -> bool:
    return any(fnmatch.fnmatchcase(benchmark_id, pattern) for pattern in patterns)


def select_benchmarks(benchmarks: list[Benchmark], filter_str: str | None, exclude_str: str | None) -> list[Benchmark]:
    selected = benchmarks
    if filter_str:
        selected = [b for b in selected if matches_any(b.id, filter_str.split(","))]
    if exclude_str:
        selected = [b for b in selected if not matches_any(b.id, exclude_str.split(","))]
    # The stage1 self-compile needs the compiler, that the self-compile builds
    ids = {b.id for b in selected}
    for b in selected:
        if b.kind == "self-compile-stage1" and b.id.replace("self-compile-stage1/", "self-compile/") not in ids:
            fail(f"'{b.id}' needs '{b.id.replace('self-compile-stage1/', 'self-compile/')}', select it as well")
    return selected


# ─── Process execution ───────────────────────────────────────────────────────


GNU_TIME = "/usr/bin/time"


@dataclass
class ProcessResult:
    exit_code: int
    wall_time_ns: int
    max_rss_bytes: int  # 0, if not measured
    output_file: Path


def run_process(cmd: list[str], env: dict[str, str], cwd: Path, output_file: Path, timeout: int, disable_aslr: bool,
                measure_rss: bool = False) -> ProcessResult:
    if disable_aslr:
        cmd = ["setarch", "-R", *cmd]
    rss_file = output_file.with_suffix(".rss")
    if measure_rss:
        # Python starts processes with vfork, and Linux keeps the peak RSS across exec. So the peak RSS of a direct child
        # (and the one, that the compiler reads with getrusage) would be at least the one of this Python process. GNU time
        # forks from a small process and reports the peak RSS of its child
        cmd = [GNU_TIME, "-f", "%M", "-o", str(rss_file), *cmd]
    cwd.mkdir(parents=True, exist_ok=True)
    with output_file.open("wb") as out:
        start = time.perf_counter_ns()
        proc = subprocess.Popen(cmd, env=env, cwd=cwd, stdout=out, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL)
        # A blocking wait, because Popen.wait with a timeout polls with sleeps of up to 50 ms, which quantizes the wall time
        killer = threading.Timer(timeout, proc.kill)
        killer.start()
        try:
            proc.wait()
        finally:
            killer.cancel()
        elapsed = time.perf_counter_ns() - start
    max_rss_bytes = 0
    if measure_rss and rss_file.is_file():
        # GNU time prints a note before the format, if the command failed. The last line is the peak RSS in KiB
        lines = rss_file.read_text().strip().splitlines()
        if lines and lines[-1].strip().isdigit():
            max_rss_bytes = int(lines[-1]) * 1024
    return ProcessResult(proc.returncode, elapsed, max_rss_bytes, output_file)


def check_success(result: ProcessResult, cmd: list[str], what: str) -> None:
    if result.exit_code == 0:
        return
    output = result.output_file.read_text(errors="replace")
    tail = "\n".join(output.splitlines()[-30:])
    fail(f"{what} failed with exit code {result.exit_code}: {' '.join(cmd)}\n{tail}")


def count_instructions(cmd: list[str], env: dict[str, str], cwd: Path, timeout: int, disable_aslr: bool, what: str) -> int:
    out_file = cwd / "cachegrind.out"
    valgrind_cmd = ["valgrind", "--tool=cachegrind", "--cache-sim=no", f"--cachegrind-out-file={out_file}", *cmd]
    result = run_process(valgrind_cmd, env, cwd, cwd / "cachegrind.log", timeout, disable_aslr)
    check_success(result, valgrind_cmd, what)
    for line in out_file.read_text().splitlines():
        if line.startswith("summary:"):
            return int(line.split()[1])
    fail(f"No summary in the cachegrind output {out_file}")
    return 0


# ─── Measurements ────────────────────────────────────────────────────────────


@dataclass
class RunContext:
    work_dir: Path
    timeout: int
    disable_aslr: bool
    measure_instructions: bool
    measure_timed: bool
    suite: str


class Samples:
    """Collected samples of the measures of one benchmark and compiler"""

    def __init__(self) -> None:
        self.values: dict[str, list[float]] = {}

    def add(self, measure: str, value: float) -> None:
        self.values.setdefault(measure, []).append(value)

    def to_results(self) -> tuple[dict, dict]:
        results = {}
        samples = {}
        for measure, values in self.values.items():
            if measure.startswith("count-") or measure in ("file-size", "instructions"):
                # Exact values. Multiple samples have to agree, otherwise the median is taken
                value = statistics.median(values)
                results[measure] = {"value": value}
            else:
                results[measure] = {"value": statistics.median(values), "lower_value": min(values),
                                    "upper_value": max(values)}
                samples[measure] = values
        return results, samples


def bench_dir(compiler: Compiler, benchmark: Benchmark) -> Path:
    return compiler.work_dir / benchmark.id.replace("/", "_")


def compile_cmd(compiler_path: Path, flags: list[str], source: Path, output: Path, stats_file: Path | None) -> list[str]:
    cmd = [str(compiler_path), "build", *flags, "-j", "1"]
    if stats_file is not None:
        cmd += ["--dump-stats", str(stats_file)]
    return cmd + ["--output", str(output), str(source)]


def add_compile_stats(samples: Samples, stats_file: Path) -> None:
    stats = json.loads(stats_file.read_text())
    samples.add("max-rss", stats["peakMemoryBytes"])
    for stage, micros in stats["stages"].items():
        samples.add(f"stage-{to_kebab_case(stage)}", micros)
    for counter, value in stats["counters"].items():
        samples.add(f"count-{to_kebab_case(counter)}", value)


class BenchmarkRunner:
    def __init__(self, ctx: RunContext, compilers: list[Compiler]) -> None:
        self.ctx = ctx
        self.compilers = compilers
        self.generated_sources: dict[str, Path] = {}
        self.stage1_compilers: dict[str, Path] = {}  # Compiler name -> compiler built by its self-compile

    def source_of(self, benchmark: Benchmark, compiler: Compiler) -> Path:
        if benchmark.kind in ("self-compile", "self-compile-stage1"):
            return compiler.root / "src" / "main.spice"
        if benchmark.generator is None:
            return benchmark.source
        key = f"{benchmark.generator}-{benchmark.scale}"
        if key not in self.generated_sources:
            out_dir = self.ctx.work_dir / "generated" / key
            shutil.rmtree(out_dir, ignore_errors=True)
            out_dir.mkdir(parents=True)
            self.generated_sources[key] = getattr(generators, benchmark.generator)(out_dir, benchmark.scale)
        return self.generated_sources[key]

    def compiler_path_for(self, benchmark: Benchmark, compiler: Compiler) -> Path:
        if benchmark.kind == "self-compile-stage1":
            return self.stage1_compilers[compiler.name]
        return compiler.path

    def output_path(self, benchmark: Benchmark, compiler: Compiler) -> Path:
        name = "spice" if benchmark.kind.startswith("self-compile") else "out"
        return bench_dir(compiler, benchmark) / f"{name}{EXE_SUFFIX}"

    def timed_run(self, benchmark: Benchmark, compiler: Compiler, samples: Samples | None) -> None:
        """One run of the benchmark, whose wall time and memory usage are measured. Without samples, it is a warm-up run"""
        directory = bench_dir(compiler, benchmark)
        if benchmark.kind == "runtime":
            executable = self.output_path(benchmark, compiler)
            cmd = [str(executable), *benchmark.args_wall_time]
            result = run_process(cmd, compiler.env, directory, directory / "run.log", self.ctx.timeout, self.ctx.disable_aslr,
                                 measure_rss=True)
            check_success(result, cmd, f"{benchmark.id} ({compiler.name})")
            if samples is not None:
                samples.add("wall-time", result.wall_time_ns)
                samples.add("max-rss", result.max_rss_bytes)
            return

        stats_file = directory / "stats.json"
        output = self.output_path(benchmark, compiler)
        cmd = compile_cmd(self.compiler_path_for(benchmark, compiler), benchmark.flags, self.source_of(benchmark, compiler),
                          output, stats_file)
        # Launched via GNU time as well, so that the peak RSS, that the compiler reports, is not distorted (see run_process).
        # The compiler's own value is taken, because it does not include the linker
        result = run_process(cmd, compiler.env, directory, directory / "compile.log", self.ctx.timeout, self.ctx.disable_aslr,
                             measure_rss=True)
        check_success(result, cmd, f"{benchmark.id} ({compiler.name})")
        if samples is not None:
            samples.add("wall-time", result.wall_time_ns)
            add_compile_stats(samples, stats_file)
            if benchmark.kind == "self-compile":
                samples.add("file-size", output.stat().st_size)

    def prepare(self, benchmark: Benchmark, compiler: Compiler) -> None:
        directory = bench_dir(compiler, benchmark)
        shutil.rmtree(directory, ignore_errors=True)
        directory.mkdir(parents=True)
        if benchmark.kind == "runtime":
            # Build the executable once. The compilation is not measured
            executable = self.output_path(benchmark, compiler)
            cmd = compile_cmd(compiler.path, benchmark.flags, benchmark.source, executable, None)
            result = run_process(cmd, compiler.env, directory, directory / "compile.log", self.ctx.timeout, False)
            check_success(result, cmd, f"Building {benchmark.id} ({compiler.name})")

    def measure_instructions(self, benchmark: Benchmark, compiler: Compiler, samples: Samples) -> None:
        directory = bench_dir(compiler, benchmark)
        if benchmark.kind == "runtime":
            cmd = [str(self.output_path(benchmark, compiler)), *benchmark.args_instructions]
        else:
            # A separate output, so that the stage1 compiler of the timed runs is not overwritten while it is in use
            cmd = compile_cmd(self.compiler_path_for(benchmark, compiler), benchmark.flags,
                              self.source_of(benchmark, compiler), directory / f"instructions-out{EXE_SUFFIX}", None)
        samples.add("instructions", count_instructions(cmd, compiler.env, directory / "cachegrind", self.ctx.timeout,
                                                       self.ctx.disable_aslr, f"{benchmark.id} ({compiler.name}) under cachegrind"))

    def run(self, benchmark: Benchmark) -> dict[str, Samples]:
        samples = {compiler.name: Samples() for compiler in self.compilers}
        for compiler in self.compilers:
            self.prepare(benchmark, compiler)
            if benchmark.kind == "runtime":
                samples[compiler.name].add("file-size", self.output_path(benchmark, compiler).stat().st_size)

        # The self-compile always runs at least once, because it builds the stage1 compiler
        timed = self.ctx.measure_timed or benchmark.kind == "self-compile"
        if timed:
            for _ in range(benchmark.warmup):
                for compiler in self.compilers:
                    self.timed_run(benchmark, compiler, None)
            # Interleave the compilers, so that a drift of the machine hits all of them equally
            for _ in range(max(benchmark.runs, 1)):
                for compiler in self.compilers:
                    self.timed_run(benchmark, compiler, samples[compiler.name])
            if not self.ctx.measure_timed:
                samples = {compiler.name: Samples() for compiler in self.compilers}

        if benchmark.kind == "self-compile":
            for compiler in self.compilers:
                self.stage1_compilers[compiler.name] = self.output_path(benchmark, compiler)

        # The self-compile under cachegrind takes long, so it only runs in the full suite
        skip_instructions = benchmark.kind.startswith("self-compile") and self.ctx.suite != "full"
        if self.ctx.measure_instructions and not skip_instructions:
            for compiler in self.compilers:
                self.measure_instructions(benchmark, compiler, samples[compiler.name])
        return samples


def run_benchmarks(compilers: list[Compiler], benchmarks: list[Benchmark], ctx: RunContext) -> tuple[dict, dict]:
    if ctx.measure_instructions and shutil.which("valgrind") is None:
        fail("valgrind is required to count instructions. Install it or pass '--measures timed'")
    if ctx.measure_timed and not Path(GNU_TIME).is_file():
        fail(f"GNU time ({GNU_TIME}) is required to measure the peak memory usage. Install it (package 'time')")
    if ctx.disable_aslr and shutil.which("setarch") is None:
        fail("setarch is required to disable ASLR. Install it (util-linux) or pass '--keep-aslr'")

    # Only compare benchmarks, that all compilers can run
    if not all(compiler.supports_tpde for compiler in compilers):
        skipped = [b.id for b in benchmarks if b.uses_tpde]
        if skipped:
            log(f"Skipping the TPDE benchmarks, because not all compilers ship TPDE: {', '.join(skipped)}")
        benchmarks = [b for b in benchmarks if not b.uses_tpde]

    runner = BenchmarkRunner(ctx, compilers)
    results = {compiler.name: {} for compiler in compilers}
    samples = {compiler.name: {} for compiler in compilers}
    for idx, benchmark in enumerate(benchmarks, 1):
        log(f"[{idx}/{len(benchmarks)}] {benchmark.id}")
        start = time.monotonic()
        for compiler_name, benchmark_samples in runner.run(benchmark).items():
            results[compiler_name][benchmark.id], samples[compiler_name][benchmark.id] = benchmark_samples.to_results()
        log(f"    done in {time.monotonic() - start:.1f}s")
    return results, samples


# ─── Statistics ──────────────────────────────────────────────────────────────


def rank(values: list[float]) -> list[float]:
    order = sorted(range(len(values)), key=lambda i: values[i])
    ranks = [0.0] * len(values)
    i = 0
    while i < len(order):
        j = i
        while j + 1 < len(order) and values[order[j + 1]] == values[order[i]]:
            j += 1
        for k in range(i, j + 1):
            ranks[order[k]] = (i + j) / 2 + 1  # Average rank of ties
        i = j + 1
    return ranks


def mann_whitney_p_value(a: list[float], b: list[float]) -> float:
    """Two-sided p-value of the Mann-Whitney U test. Exact for small samples without ties, else normal approximation"""
    n1, n2 = len(a), len(b)
    if n1 == 0 or n2 == 0:
        return 1.0
    combined = a + b
    ranks = rank(combined)
    u1 = sum(ranks[:n1]) - n1 * (n1 + 1) / 2
    u = min(u1, n1 * n2 - u1)
    has_ties = len(set(combined)) < len(combined)
    if not has_ties and n1 * n2 <= 400:
        # Number of orderings of the samples, that yield each U value
        counts = {(0, 0): [1]}

        def distribution(i: int, j: int) -> list[int]:
            if (i, j) in counts:
                return counts[(i, j)]
            dist = [0] * (i * j + 1)
            if i > 0:
                for k, c in enumerate(distribution(i - 1, j)):
                    dist[k + j] += c
            if j > 0:
                for k, c in enumerate(distribution(i, j - 1)):
                    dist[k] += c
            counts[(i, j)] = dist
            return dist

        dist = distribution(n1, n2)
        tail = sum(dist[:int(u) + 1]) / math.comb(n1 + n2, n1)
        return min(1.0, 2 * tail)
    # Normal approximation with tie correction
    n = n1 + n2
    tie_sum = sum(t ** 3 - t for t in (combined.count(v) for v in set(combined)))
    sigma = math.sqrt(n1 * n2 / 12 * ((n + 1) - tie_sum / (n * (n - 1))))
    if sigma == 0:
        return 1.0
    z = (abs(u1 - n1 * n2 / 2) - 0.5) / sigma
    return min(1.0, math.erfc(max(z, 0) / math.sqrt(2)))


def percent_change(baseline: float, candidate: float) -> float:
    if baseline == 0:
        return 0.0 if candidate == 0 else math.inf
    return (candidate - baseline) / baseline * 100


# ─── Reports ─────────────────────────────────────────────────────────────────


def format_value(measure: str, value: float) -> str:
    if measure == "wall-time":
        return f"{value / 1e6:.1f} ms"
    if measure.startswith("stage-"):
        return f"{value / 1e3:.1f} ms"
    if measure in ("max-rss", "file-size"):
        return f"{value / 2 ** 20:.2f} MiB" if value >= 2 ** 20 else f"{value / 2 ** 10:.1f} KiB"
    if measure == "instructions":
        return f"{value / 1e6:.2f} M"
    return f"{int(value)}"


def format_change(change: float) -> str:
    return "∞" if math.isinf(change) else f"{change:+.2f} %"


def compare_results(data: dict, baseline: str, candidate: str) -> tuple[str, int]:
    results = data["results"]
    for name in (baseline, candidate):
        if name not in results:
            fail(f"No results for the compiler '{name}', available: {', '.join(results)}")
    config = data["config"]
    warn_percent = config["warnings"].get("percent", 10.0)
    warn_p_value = config["warnings"].get("p-value", 0.01)
    min_deltas = config.get("min-deltas", {})
    base_results, cand_results = results[baseline], results[candidate]
    base_samples, cand_samples = data["samples"][baseline], data["samples"][candidate]

    gated_rows, wall_rows, counter_rows, missing = [], [], [], []
    regressions = improvements = 0
    for benchmark_id in sorted(set(base_results) | set(cand_results)):
        if benchmark_id not in base_results or benchmark_id not in cand_results:
            missing.append(benchmark_id)
            continue
        thresholds = config["thresholds"].get(benchmark_id, {})
        base, cand = base_results[benchmark_id], cand_results[benchmark_id]
        for measure in sorted(set(base) & set(cand)):
            base_value, cand_value = base[measure]["value"], cand[measure]["value"]
            change = percent_change(base_value, cand_value)
            if measure in GATED_MEASURES:
                threshold = thresholds.get(measure)
                # Small values (e.g. the peak memory usage of a small program) fluctuate more in relative terms, so the
                # absolute change has to exceed the minimum delta as well
                exceeds_min_delta = abs(cand_value - base_value) > min_deltas.get(measure, 0)
                status = ""
                if threshold is not None and change > threshold and exceeds_min_delta:
                    status = "❌ regression"
                    regressions += 1
                elif threshold is not None and change < -threshold and exceeds_min_delta:
                    status = "✅ improvement"
                    improvements += 1
                gated_rows.append((benchmark_id, measure, base_value, cand_value, change, status))
            elif measure.startswith(SAMPLED_MEASURE_PREFIXES):
                p_value = mann_whitney_p_value(base_samples.get(benchmark_id, {}).get(measure, []),
                                               cand_samples.get(benchmark_id, {}).get(measure, []))
                significant = abs(change) > warn_percent and p_value < warn_p_value
                wall_rows.append((benchmark_id, measure, base_value, cand_value, change, p_value, significant))
            elif measure.startswith("count-") and base_value != cand_value:
                counter_rows.append((benchmark_id, measure, base_value, cand_value, change))

    lines = [f"## Benchmark results: `{candidate}` vs. `{baseline}`", ""]
    lines.append(f"**{regressions} regression(s)**, {improvements} improvement(s) of gated measures beyond their thresholds"
                 if regressions else f"No regressions of gated measures beyond their thresholds ({improvements} improvement(s))")
    lines.append("")
    base_meta, cand_meta = data["metadata"].get(baseline, {}), data["metadata"].get(candidate, {})
    lines.append(f"`{baseline}`: {base_meta.get('commit', '?')[:10]}, `{candidate}`: {cand_meta.get('commit', '?')[:10]}, "
                 f"CPU: {cand_meta.get('cpu', '?')}, suite: {config.get('suite', '?')}")
    lines.append("")

    def table(header: list[str], rows: list[list[str]]) -> list[str]:
        out = ["| " + " | ".join(header) + " |", "|" + "|".join("---" for _ in header) + "|"]
        out += ["| " + " | ".join(row) + " |" for row in rows]
        return out + [""]

    changed_gated = [row for row in gated_rows if row[4] != 0]
    lines += ["### Gated measures", ""]
    if changed_gated:
        lines += table(["Benchmark", "Measure", baseline, candidate, "Change", ""],
                       [[f"`{b}`", m, format_value(m, bv), format_value(m, cv), format_change(c), s]
                        for b, m, bv, cv, c, s in changed_gated])
    unchanged = len(gated_rows) - len(changed_gated)
    if unchanged:
        lines += [f"{unchanged} gated measure(s) unchanged.", ""]

    significant_rows = [row for row in wall_rows if row[6]]
    lines += ["### Times (not gated)", ""]
    if significant_rows:
        lines += [f"⚠️ {len(significant_rows)} time(s) changed by more than {warn_percent:g} % with p < {warn_p_value:g}:", ""]
        lines += table(["Benchmark", "Measure", baseline, candidate, "Change", "p-value"],
                       [[f"`{b}`", m, format_value(m, bv), format_value(m, cv), format_change(c), f"{p:.3f}"]
                        for b, m, bv, cv, c, p, _ in significant_rows])
    else:
        lines += ["No significant time differences.", ""]
    wall_time_rows = [row for row in wall_rows if row[1] == "wall-time"]
    if wall_time_rows:
        lines += ["<details><summary>All wall times (medians)</summary>", ""]
        lines += table(["Benchmark", baseline, candidate, "Change", "p-value"],
                       [[f"`{b}`", format_value(m, bv), format_value(m, cv), format_change(c), f"{p:.3f}"]
                        for b, m, bv, cv, c, p, _ in wall_time_rows])
        lines += ["</details>", ""]

    if counter_rows:
        lines += ["### Changed counters", ""]
        lines += table(["Benchmark", "Counter", baseline, candidate, "Change"],
                       [[f"`{b}`", m, format_value(m, bv), format_value(m, cv), format_change(c)]
                        for b, m, bv, cv, c in counter_rows])
    if missing:
        lines += ["### Not compared", "", "Results of only one compiler: " + ", ".join(f"`{b}`" for b in missing), ""]
    return "\n".join(lines), regressions


def noise_report(data: dict, names: list[str], defaults: dict) -> str:
    lines = [f"## Noise of the gated measures over {len(names)} repetitions", ""]
    lines += ["Suggested threshold: max(default, 5 σ), rounded up to 0.5 %. Not raised, if the maximum deviation stays below "
              "the minimum delta of the measure.", ""]
    lines += ["| Benchmark | Measure | Mean | σ | Max deviation | Default | Suggested |", "|---|---|---|---|---|---|---|"]
    first = data["results"][names[0]]
    default_thresholds = defaults.get("thresholds", {})
    min_deltas = defaults.get("min-deltas", {})
    for benchmark_id in sorted(first):
        for measure in GATED_MEASURES:
            values = [data["results"][name][benchmark_id][measure]["value"] for name in names
                      if measure in data["results"][name].get(benchmark_id, {})]
            if len(values) < 2:
                continue
            mean = statistics.mean(values)
            stdev_percent = statistics.stdev(values) / mean * 100 if mean else 0.0
            max_dev = max(abs(v - mean) for v in values)
            max_dev_percent = max_dev / mean * 100 if mean else 0.0
            default = default_thresholds.get(measure, 0.0)
            below_min_delta = max_dev <= min_deltas.get(measure, 0)
            suggested = default if below_min_delta else max(default, math.ceil(5 * stdev_percent * 2) / 2)
            note = " (below min delta)" if below_min_delta and suggested < math.ceil(5 * stdev_percent * 2) / 2 else ""
            lines.append(f"| `{benchmark_id}` | {measure} | {format_value(measure, mean)} | {stdev_percent:.3f} % | "
                         f"{max_dev_percent:.3f} % ({format_value(measure, max_dev)}) | {default:g} % | {suggested:g} %{note} |")
    return "\n".join(lines) + "\n"


# ─── Commands ────────────────────────────────────────────────────────────────


def build_results(compilers: list[Compiler], benchmarks: list[Benchmark], defaults: dict, ctx: RunContext) -> dict:
    results, samples = run_benchmarks(compilers, benchmarks, ctx)
    return {
        "version": RESULTS_FORMAT_VERSION,
        "metadata": {compiler.name: compiler_metadata(compiler) for compiler in compilers},
        "config": {
            "suite": ctx.suite,
            "thresholds": {b.id: b.thresholds for b in benchmarks},
            "warnings": defaults.get("warnings", {}),
            "min-deltas": defaults.get("min-deltas", {}),
        },
        "results": results,
        "samples": samples,
    }


def make_context(args: argparse.Namespace) -> RunContext:
    measures = set(args.measures.split(","))
    if unknown := measures - set(MEASURE_GROUPS):
        fail(f"Unknown measures: {', '.join(sorted(unknown))}, expected: {', '.join(MEASURE_GROUPS)}")
    work_dir = args.work_dir.resolve()
    work_dir.mkdir(parents=True, exist_ok=True)
    return RunContext(work_dir, args.timeout, not args.keep_aslr, "instructions" in measures, "timed" in measures, args.suite)


def load_and_select(args: argparse.Namespace) -> tuple[list[Benchmark], dict]:
    benchmarks, defaults = load_registry(args.registry)
    if args.runs is not None:
        for benchmark in benchmarks:
            benchmark.runs = args.runs
    selected = select_benchmarks(benchmarks, args.filter, args.exclude)
    if not selected:
        fail("No benchmarks selected")
    return selected, defaults


def write_json(data: dict, output: Path) -> None:
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(data, indent=2) + "\n")
    log(f"Results written to {output}")


def cmd_run(args: argparse.Namespace) -> None:
    benchmarks, defaults = load_and_select(args)
    ctx = make_context(args)
    compilers = parse_compilers(args.compiler, ctx.work_dir)
    write_json(build_results(compilers, benchmarks, defaults, ctx), args.output)


def cmd_noise(args: argparse.Namespace) -> None:
    benchmarks, defaults = load_and_select(args)
    for benchmark in benchmarks:
        benchmark.runs = 1
        benchmark.warmup = 0
    ctx = make_context(args)
    compilers = parse_compilers([f"r{i}={args.compiler}" for i in range(args.repetitions)], ctx.work_dir)
    data = build_results(compilers, benchmarks, defaults, ctx)
    if args.output:
        write_json(data, args.output)
    print(noise_report(data, [compiler.name for compiler in compilers], defaults))


def cmd_compare(args: argparse.Namespace) -> None:
    data = json.loads(args.results.read_text())
    report, regressions = compare_results(data, args.baseline, args.candidate)
    print(report)
    if args.markdown:
        args.markdown.write_text(report + "\n")
    if regressions and not args.report_only:
        sys.exit(1)


def cmd_export(args: argparse.Namespace) -> None:
    data = json.loads(args.results.read_text())
    if args.compiler not in data["results"]:
        fail(f"No results for the compiler '{args.compiler}', available: {', '.join(data['results'])}")
    print(json.dumps(data["results"][args.compiler], indent=2))


def add_run_options(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--registry", type=Path, default=BENCHMARK_DIR / "benchmarks.toml", help="Benchmark registry")
    parser.add_argument("--filter", help="Comma-separated globs of the benchmark ids to run, e.g. 'compile/*,runtime/sieve/*'")
    parser.add_argument("--exclude", help="Comma-separated globs of the benchmark ids to skip")
    parser.add_argument("--suite", choices=("pr", "full"), default="pr",
                        help="pr: skip the instruction count of the self-compile, full: measure everything (default: pr)")
    parser.add_argument("--measures", default=",".join(MEASURE_GROUPS),
                        help="Comma-separated measure groups: instructions (cachegrind), timed (wall time, memory, stage "
                             "times, counters, file size). Default: both")
    parser.add_argument("--runs", type=int, default=None, help="Override the number of measured runs of every benchmark")
    parser.add_argument("--work-dir", type=Path, default=ROOT_DIR / "build" / "bench-tmp", help="Directory for artifacts")
    parser.add_argument("--timeout", type=int, default=3600, help="Timeout per process in seconds (default: 3600)")
    parser.add_argument("--keep-aslr", action="store_true", help="Do not disable ASLR with setarch")


def main() -> None:
    parser = argparse.ArgumentParser(description="Benchmark runner for the Spice compiler (see media/specs/benchmarks.md)")
    subparsers = parser.add_subparsers(dest="command", required=True)

    run_parser = subparsers.add_parser("run", help="Measure the benchmarks with one or more compilers")
    run_parser.add_argument("--compiler", action="append", required=True, metavar="NAME=PATH",
                            help="Compiler under test, can be passed multiple times, e.g. base=../base/build/spice")
    run_parser.add_argument("-o", "--output", type=Path, default=ROOT_DIR / "build" / "benchmark-results.json",
                            help="Results file (default: build/benchmark-results.json)")
    add_run_options(run_parser)
    run_parser.set_defaults(func=cmd_run)

    noise_parser = subparsers.add_parser("noise", help="Measure the gated measures repeatedly with the same compiler")
    noise_parser.add_argument("--compiler", required=True, type=Path, help="Compiler under test")
    noise_parser.add_argument("--repetitions", type=int, default=10, help="Number of repetitions (default: 10)")
    noise_parser.add_argument("-o", "--output", type=Path, default=None, help="Also write the raw results to this file")
    add_run_options(noise_parser)
    noise_parser.set_defaults(func=cmd_noise)

    compare_parser = subparsers.add_parser("compare", help="Compare the results of two compilers")
    compare_parser.add_argument("results", type=Path, help="Results file of the run command")
    compare_parser.add_argument("--baseline", required=True, help="Name of the baseline compiler")
    compare_parser.add_argument("--candidate", required=True, help="Name of the candidate compiler")
    compare_parser.add_argument("--markdown", type=Path, default=None, help="Also write the report to this file")
    compare_parser.add_argument("--report-only", action="store_true", help="Do not fail on regressions")
    compare_parser.set_defaults(func=cmd_compare)

    export_parser = subparsers.add_parser("export", help="Print the results of one compiler in the Bencher Metric Format")
    export_parser.add_argument("results", type=Path, help="Results file of the run command")
    export_parser.add_argument("--compiler", required=True, help="Name of the compiler")
    export_parser.set_defaults(func=cmd_export)

    args = parser.parse_args()
    # Like build.py, find LLVM in the checkout by default
    os.environ.setdefault("LLVM_DIR", str(ROOT_DIR / "llvm" / "build-release" / "lib" / "cmake" / "llvm"))
    if not sys.platform.startswith("linux"):
        log("Warning: the benchmark runner is only supported on Linux")
    args.func(args)


if __name__ == "__main__":
    main()

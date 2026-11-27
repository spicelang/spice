#!/usr/bin/env python3
"""Differential harness: run the legacy compiler and the compiler re-implementation on a corpus and compare everything
that is observable (media/specs/compiler-reimplementation.md, section 8.1).

Corpus (--scope, comma separated):
  tests      every test case of test/test-files with the '// TEST:' flags of its source.spice (+ the imported files)
  <Suite>    only the cases of one suite, e.g. ParserTests
  std        every std module, compiled through a small driver program that imports it
  compiler   the compiler sources: src/main.spice and src-next/main.spice (each is compiled with both compilers)
  docs       all complete programs in docs/
  all        everything above (default)

Compared (a difference in a gating stage fails the run, the white-box dumps are only reported, decision D2):
  ir-O0, ir-O2   exit code, console output and the IR of EVERY module (--dump-ir --dump-to-files)
  asm-O0         the assembly at -O0
  exec           output and exit code of the compiled program, for cases with a cout.out or exit-code.out ref
  whitebox       --dump-ast, --dump-dependency-graph (console), --dump-symtab, --dump-types (files). Not gating.

Each difference is classified (identical, order, names, metadata, structural, see tools/ir_normalize.py). The normalizer is
for triage only, the gate is byte-identical.

Examples:
  python tools/diff_compilers.py --legacy build/spice --next build/spice-next --scope ParserTests
  python tools/diff_compilers.py --legacy build/spice --next build/spice --scope all    # sanity check: zero differences
"""
from __future__ import annotations

import argparse
import fnmatch
import importlib.util
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass, field
from pathlib import Path

ROOT_DIR = Path(__file__).resolve().parent.parent
sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parent))
import ir_normalize  # noqa: E402

EXE_SUFFIX = ".exe" if os.name == "nt" else ""
COMPILE_TIMEOUT = 900
RUN_TIMEOUT = 60
TIMEOUT_PREFIX = "<timeout after "
OPT_LEVELS = ["0", "2"]
ANSI_ESCAPE = re.compile(r"\x1b\[[0-9;]*m")
SCOPES = ["all", "tests", "std", "compiler", "docs"]
# Stages, whose differences are only reported. The formats of the white-box dumps may change (D2)
NON_GATING_STAGES = {"whitebox"}


@dataclass(frozen=True)
class Case:
    scope: str  # 'tests', 'std', 'compiler' or 'docs'
    suite: str  # Suite name for tests, else the scope
    name: str  # Unique name, e.g. 'ParserTests.parser_foo'
    source: Path  # Absolute path of the main source file (docs: the markdown file the example is from)
    args: tuple[str, ...] = ()  # Extra compiler args, e.g. from the '// TEST:' header
    cwd: Path | None = None  # Working dir of the compiler, None: a fresh dir per run
    execute: bool = False  # Run the compiled program and compare its output
    text: str | None = None  # Source text of a generated case (docs examples), written unchanged per run


@dataclass
class Difference:
    stage: str  # e.g. 'ir-O2'
    kind: str  # 'exit-code', 'output', 'files', 'content', 'exec'
    classes: list[str]  # see ir_normalize
    subject: str = ""  # e.g. the dump file name
    lines: list[str] = field(default_factory=list)  # First differing lines

    @property
    def gating(self) -> bool:
        return self.stage not in NON_GATING_STAGES


@dataclass
class CaseResult:
    case: Case
    differences: list[Difference] = field(default_factory=list)
    error: str | None = None  # The harness itself failed on this case


# ---------------------------------------------------------------------------------------------------------------- corpus

def load_run_tests():
    """Import test/run-tests.py as a module, to share the suite table and the test case discovery"""
    spec = importlib.util.spec_from_file_location("run_tests", ROOT_DIR / "test" / "run-tests.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def test_cases(suite_filter: str | None) -> list[Case]:
    rt = load_run_tests()
    cases = []
    for suite in rt.SUITES:
        if suite.linter or suite.formatter or (suite_filter and suite.name != suite_filter):
            continue
        for tc in rt.collect_test_cases(suite):
            rel = Path(tc.path.removeprefix(rt.PATH_TEST_FILES + "/"))
            case_dir = ROOT_DIR / "test" / "test-files" / rel
            if any((case_dir / marker).exists() for marker in (rt.CTL_SKIP_DISABLED, rt.CTL_NEXT_ONLY)):
                continue
            source = case_dir / rt.REF_NAME_SOURCE
            if not source.exists():
                continue
            args = [a for a in rt.parse_test_args(str(source))]
            # Cases for other targets, sanitizers and backends need extra setup (QEMU, TPDE). The runner covers them
            if any(a.startswith(("--sanitizer", "--target", "--backend", "--linker-script")) for a in args):
                continue
            execute = (case_dir / rt.REF_NAME_EXECUTION_OUTPUT).exists() or (case_dir / rt.REF_NAME_EXIT_CODE).exists()
            cases.append(Case("tests", suite.name, tc.full_name, source, tuple(args), cwd=ROOT_DIR / "test" / "test-files",
                              execute=execute))
    return cases


def std_cases() -> list[Case]:
    cases = []
    for path in sorted((ROOT_DIR / "std").rglob("*.spice")):
        rel = path.relative_to(ROOT_DIR / "std").with_suffix("").as_posix()
        # The driver programs are written lazily per run into the case's work dir, see prepare_case_source
        cases.append(Case("std", "std", f"std.{rel.replace('/', '_')}", Path(f"std/{rel}")))
    return cases


def compiler_cases() -> list[Case]:
    # Both compilers compile both sources: the legacy compiler sources and the sources of the re-implementation
    return [Case("compiler", "compiler", f"compiler.{d.replace('-', '_')}_main", ROOT_DIR / d / "main.spice")
            for d in ("src", "src-next") if (ROOT_DIR / d / "main.spice").exists()]


def docs_cases() -> list[Case]:
    sys.path.insert(0, str(ROOT_DIR / "tools"))
    import check_doc_examples as docs
    cases = []
    problems: list = []
    for md_file in sorted((ROOT_DIR / "docs").rglob("*.md")):
        for example in docs.parse_md_file(md_file, problems):
            if example.directive == "skip":
                continue
            name = f"docs.{docs.display_path(md_file).replace('/', '_')}_{example.line}"
            # The source is materialized per run, see prepare_case_source
            cases.append(Case("docs", "docs", name, md_file, text=example.code, execute=False))
    return cases


def collect_cases(scopes: list[str], name_filter: str) -> list[Case]:
    cases: list[Case] = []
    wanted = set(scopes)
    if "all" in wanted:
        wanted |= {"tests", "std", "compiler", "docs"}
    for scope in sorted(wanted):
        if scope == "tests":
            cases += test_cases(None)
        elif scope == "std":
            cases += std_cases()
        elif scope == "compiler":
            cases += compiler_cases()
        elif scope == "docs":
            cases += docs_cases()
        elif scope not in SCOPES:
            cases += test_cases(scope)  # A suite name
    seen = set()
    unique = [c for c in cases if not (c.name in seen or seen.add(c.name))]
    return [c for c in unique if matches_filter(c.name, name_filter)]


def matches_filter(name: str, filter_str: str) -> bool:
    """GoogleTest-style filter: 'pos1:pos2-neg1:neg2'"""
    positive, _, negative = filter_str.partition("-")
    pos = [p for p in positive.split(":") if p] or ["*"]
    neg = [p for p in negative.split(":") if p]
    return any(fnmatch.fnmatchcase(name, p) for p in pos) and not any(fnmatch.fnmatchcase(name, p) for p in neg)


# --------------------------------------------------------------------------------------------------------------- running

def prepare_case_source(case: Case, case_dir: Path) -> Path:
    """Materialize generated sources (std driver programs, docs examples). Returns the source path to compile"""
    if case.scope == "std":
        source = case_dir / "driver.spice"
        source.write_text(f'import "{case.source.as_posix()}";\n\nf<int> main() {{\n    return 0;\n}}\n', encoding="utf-8")
        return source
    if case.scope == "docs":
        source = case_dir / "example.spice"
        source.write_text(case.text or "", encoding="utf-8", newline="")
        return source
    return case.source


def run(cmd: list[str], cwd: Path, timeout: int) -> tuple[str, int]:
    try:
        proc = subprocess.run(cmd, cwd=cwd, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                              timeout=timeout)
    except subprocess.TimeoutExpired as e:
        return f"{TIMEOUT_PREFIX}{timeout}s>\n" + (e.output or b"").decode("utf-8", errors="replace"), 124
    code = 128 - proc.returncode if proc.returncode < 0 else proc.returncode
    return proc.stdout.decode("utf-8", errors="surrogateescape").replace("\r\n", "\n"), code


@dataclass
class StageRun:
    exit_code: int
    output: str  # Console output with the out dir erased
    files: dict[str, str]  # Dump files by name


def run_stage(compiler: Path, case: Case, source: Path, case_dir: Path, side: str, stage: str,
              flags: list[str], dump_glob: str | None) -> StageRun:
    out_dir = case_dir / side / stage
    out_dir.mkdir(parents=True)
    cwd = case.cwd or out_dir
    # Run in the shared test-files parent, as the test runner does: diagnostics print paths relative to the working dir
    args = ["build", "--test-mode", *case.args, "--output-container", "obj", *flags, "--output", str(out_dir / "object.o"),
            str(source)]
    output, code = run([str(compiler), *args], cwd, COMPILE_TIMEOUT)
    if output.startswith(TIMEOUT_PREFIX):  # Two timeouts must not count as equal behavior
        raise RuntimeError(f"{side} compiler timed out in stage {stage}")
    output = ANSI_ESCAPE.sub("", output).replace(str(out_dir), "<out>").replace(str(case_dir), "<case>")
    files = {}
    if dump_glob:
        for path in sorted(out_dir.glob(dump_glob)):
            files[path.name] = path.read_bytes().decode("utf-8", errors="surrogateescape")
    return StageRun(code, output, files)


def run_exec(compiler: Path, case: Case, source: Path, case_dir: Path, side: str) -> tuple[int, str]:
    out_dir = case_dir / side / "exec"
    out_dir.mkdir(parents=True)
    exe = out_dir / f"source{EXE_SUFFIX}"
    output, code = run([str(compiler), "build", "--test-mode", *case.args, "--output", str(exe), str(source)],
                       case.cwd or out_dir, COMPILE_TIMEOUT)
    if output.startswith(TIMEOUT_PREFIX):
        raise RuntimeError(f"{side} compiler timed out building the executable")
    if code == 0 and not exe.exists():
        raise RuntimeError(f"{side} compiler succeeded, but did not produce {exe.name}")
    if code != 0:
        return code, "<compilation failed>\n" + ANSI_ESCAPE.sub("", output)
    return run([str(exe)], out_dir, RUN_TIMEOUT)[::-1]


# Stage name -> (flags, glob of the dump files to compare, whether the dumped files are IR)
STAGES = {
    **{f"ir-O{o}": ([f"-O{o}", "--dump-ir", "--dump-to-files"], "*-ir-code*.ll") for o in OPT_LEVELS},
    "asm-O0": (["-O0", "--dump-assembly", "--dump-to-files"], "*-assembly-code.s"),
    "whitebox-console": (["-O0", "--dump-ast", "--dump-dependency-graph"], None),
    "whitebox-files": (["-O0", "--dump-symtab", "--dump-types", "--dump-to-files"], "*-symbol-table.json"),
}


def stage_group(stage: str) -> str:
    return "whitebox" if stage.startswith("whitebox") else stage


def compare_stage(stage: str, legacy: StageRun, nxt: StageRun) -> list[Difference]:
    group = stage_group(stage)
    diffs = []
    if legacy.exit_code != nxt.exit_code:
        diffs.append(Difference(group, "exit-code", [ir_normalize.STRUCTURAL], "",
                                [f"legacy: {legacy.exit_code}", f"next:   {nxt.exit_code}"]))
    if legacy.output != nxt.output:
        diffs.append(Difference(group, "output", [ir_normalize.STRUCTURAL], "console",
                                ir_normalize.first_diff_lines(legacy.output, nxt.output)))
    if legacy.files.keys() != nxt.files.keys():
        only_l, only_n = sorted(legacy.files.keys() - nxt.files.keys()), sorted(nxt.files.keys() - legacy.files.keys())
        diffs.append(Difference(group, "files", [ir_normalize.STRUCTURAL], "",
                                [f"only legacy: {only_l}", f"only next: {only_n}"]))
    for name in sorted(legacy.files.keys() & nxt.files.keys()):
        a, b = legacy.files[name], nxt.files[name]
        if a == b:
            continue
        classes = ir_normalize.classify(a, b) if name.endswith(".ll") else [ir_normalize.STRUCTURAL]
        diffs.append(Difference(group, "content", classes, name, ir_normalize.first_diff_lines(a, b)))
    return diffs


def run_case(legacy: Path, nxt: Path, case: Case, work_dir: Path, stages: list[str]) -> CaseResult:
    result = CaseResult(case)
    case_dir = work_dir / case.name
    shutil.rmtree(case_dir, ignore_errors=True)
    case_dir.mkdir(parents=True)
    try:
        source = prepare_case_source(case, case_dir)
        for stage in stages:
            flags, dump_glob = STAGES[stage]
            runs = [run_stage(c, case, source, case_dir, side, stage, flags, dump_glob)
                    for side, c in (("legacy", legacy), ("next", nxt))]
            result.differences += compare_stage(stage, *runs)
        if case.execute:
            (lc, lo), (nc, no) = (run_exec(c, case, source, case_dir, side) for side, c in (("legacy", legacy), ("next", nxt)))
            if (lc, lo) != (nc, no):
                result.differences.append(Difference("exec", "exec", [ir_normalize.STRUCTURAL], "",
                                                     [f"exit code: legacy {lc}, next {nc}",
                                                      *ir_normalize.first_diff_lines(lo, no)]))
    except Exception as e:  # noqa: BLE001 - a harness failure on one case must not stop the run
        result.error = f"{type(e).__name__}: {e}"
    return result


# ------------------------------------------------------------------------------------------------------------- reporting

def class_label(classes: list[str]) -> str:
    return " + ".join(classes)


def report(results: list[CaseResult], max_cases: int, max_lines: int) -> int:
    """Print the summary and the first differing lines. Returns the number of gating differences"""
    per_suite: dict[str, Counter] = defaultdict(Counter)
    per_class: Counter = Counter()
    gating_total = 0
    differing = []
    for r in results:
        gating = [d for d in r.differences if d.gating]
        gating_total += len(gating) + (1 if r.error else 0)
        per_suite[r.case.suite]["cases"] += 1
        if gating or r.error:
            per_suite[r.case.suite]["differing"] += 1
        if r.error:
            per_suite[r.case.suite]["errors"] += 1
        for d in r.differences:
            per_class[(d.stage if d.gating else "whitebox (not gating)", class_label(d.classes))] += 1
        if r.differences or r.error:
            differing.append(r)

    print("Summary per suite:")
    for suite in sorted(per_suite):
        c = per_suite[suite]
        print(f"  {suite:28} {c['cases']:5} cases, {c['differing']:5} differ, {c['errors']:3} harness errors")
    print("\nDifferences per stage and class:")
    if not per_class:
        print("  none")
    for (stage, classes), count in sorted(per_class.items()):
        print(f"  {stage:24} {classes:28} {count:6}")

    for r in differing[:max_cases]:
        print(f"\n=== {r.case.name}")
        if r.error:
            print(f"  harness error: {r.error}")
        for d in r.differences:
            tag = "" if d.gating else " (not gating)"
            subject = f" {d.subject}" if d.subject else ""
            print(f"  [{d.stage}] {d.kind}{subject}: {class_label(d.classes)}{tag}")
            for line in d.lines[:max_lines]:
                print(f"      {line}")
    if len(differing) > max_cases:
        print(f"\n... and {len(differing) - max_cases} more cases with differences (see --json)")
    print(f"\n{len(results)} cases, {gating_total} gating differences")
    return gating_total


def to_json(results: list[CaseResult]) -> list[dict]:
    return [{"case": r.case.name, "suite": r.case.suite, "error": r.error,
             "differences": [{"stage": d.stage, "kind": d.kind, "classes": d.classes, "subject": d.subject,
                              "gating": d.gating, "lines": d.lines} for d in r.differences]} for r in results]


# ----------------------------------------------------------------------------------------------------------------- main

def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--legacy", type=Path, required=True, help="The legacy compiler (the oracle)")
    p.add_argument("--next", type=Path, required=True, help="The compiler re-implementation")
    p.add_argument("--scope", default="all", help="Comma separated: all, tests, std, compiler, docs or a suite name")
    p.add_argument("--filter", default="*", help="GoogleTest-style filter on the case names")
    p.add_argument("--stages", default=",".join(STAGES), help=f"Comma separated, default: all ({', '.join(STAGES)})")
    p.add_argument("--list", action="store_true", help="List the selected cases and exit")
    p.add_argument("-j", "--jobs", type=int, default=os.cpu_count() or 1)
    p.add_argument("--work-dir", type=Path, default=None, help="Default: a temp dir, removed afterwards")
    p.add_argument("--json", type=Path, default=None, help="Write all differences to this file")
    p.add_argument("--max-cases", type=int, default=30, help="Max. number of cases to detail in the report")
    p.add_argument("--max-lines", type=int, default=6, help="Max. number of differing lines per difference")
    return p.parse_args()


def main() -> int:
    args = parse_args()
    cases = collect_cases([s for s in args.scope.split(",") if s], args.filter)
    if args.list:
        print("\n".join(c.name for c in cases))
        return 0
    stages = [s for s in args.stages.split(",") if s]
    unknown = [s for s in stages if s not in STAGES]
    if unknown:
        sys.exit(f"Unknown stages: {unknown}")
    for compiler in (args.legacy, args.next):
        if not compiler.exists():
            sys.exit(f"Compiler not found: {compiler}")
    if not stages:
        sys.exit("No stages selected")
    if not cases:
        sys.exit("No cases selected")
    legacy, nxt = args.legacy.resolve(), args.next.resolve()
    work_dir = (args.work_dir or Path(tempfile.mkdtemp(prefix="diff-compilers-"))).resolve()
    work_dir.mkdir(parents=True, exist_ok=True)
    print(f"Comparing {len(cases)} cases, work dir: {work_dir}\n", flush=True)
    try:
        with ThreadPoolExecutor(args.jobs) as pool:
            results = list(pool.map(lambda c: run_case(legacy, nxt, c, work_dir, stages), cases))
    finally:
        if args.work_dir is None:
            shutil.rmtree(work_dir, ignore_errors=True)
    if args.json:
        args.json.write_text(json.dumps(to_json(results), indent=1), encoding="utf-8")
    return 1 if report(results, args.max_cases, args.max_lines) else 0


if __name__ == "__main__":
    sys.exit(main())

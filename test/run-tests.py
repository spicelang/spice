#!/usr/bin/env python3
"""Run the reference test suites in test/test-files against the self-hosted Spice compiler.

The compiler under test is invoked like a user would invoke it ('spice build ...'), and its dumps, diagnostics and the
behavior of the compiled test programs are compared against the reference files of each test case. Either pass an already
built compiler via --compiler, or let the runner build it from the sources in src/ first via --build-compiler (e.g. with
the released stage0 compiler from fetch-stage0.py or the host compiler).

Besides the reference test cases, the runner builds the compiler sources in test build mode and runs their builtin tests
(#[test] functions), and runs the linter test cases against the 'lint' subcommand.
"""
import argparse
import difflib
import fnmatch
import os
import platform
import re
import shlex
import shutil
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import dataclass, field
from pathlib import Path
from typing import Callable

ROOT_DIR = Path(__file__).resolve().parent.parent
TEST_DIR = ROOT_DIR / "test"

# Reuse the environment detection of the bootstrap script, without leaving a __pycache__ dir in the repo root
sys.dont_write_bytecode = True
sys.path.insert(0, str(ROOT_DIR))
import bootstrap  # noqa: E402

GREEN = "\033[0;92m"
RED = "\033[0;91m"
YELLOW = "\033[0;93m"
NC = "\033[0m"

IS_WINDOWS = sys.platform == "win32"
IS_MACOS = sys.platform == "darwin"
EXE_SUFFIX = ".exe" if IS_WINDOWS else ""
TARGET_OS = "windows" if IS_WINDOWS else "macos" if IS_MACOS else "linux"
TARGET_ARCH = {"x86_64": "amd64", "amd64": "amd64", "aarch64": "aarch64", "arm64": "aarch64"}.get(platform.machine().lower())
IS_X86_64 = TARGET_ARCH == "amd64"

# The paths of the test cases are relative to the work dir, which links to the test files. Like for the host test runner, it is
# the working directory of the compiler and the test programs, which may create files in it
PATH_TEST_FILES = "./test-files"

INPUT_NAME_CLI_FLAGS = "cli-flags.txt"
REF_NAME_SOURCE = "source.spice"
REF_NAME_SYNTAX_TREE = "syntax-tree.dot"
REF_NAME_DEP_GRAPH = "dependency-graph.dot"
REF_NAME_SYMBOL_TABLE = "symbol-table.json"
REF_NAME_TYPE_REGISTRY = "type-registry.out"
REF_NAME_CACHE_STATS = "cache-stats.out"
REF_NAME_ASM = "assembly.asm"
REF_NAME_OPT_IR = ["ir-code.ll", "ir-code-O1.ll", "ir-code-O2.ll", "ir-code-O3.ll", "ir-code-Os.ll", "ir-code-Oz.ll"]
REF_NAME_EXECUTION_OUTPUT = "cout.out"
REF_NAME_GDB_OUTPUT = "debug.out"
REF_NAME_ERROR_OUTPUT = "exception.out"
REF_NAME_WARNING_OUTPUT = "warning.out"
REF_NAME_LINT_OUTPUT = "lint.out"
REF_NAME_EXIT_CODE = "exit-code.out"

CTL_SKIP_DISABLED = "disabled"
CTL_SKIP_GH = "skip-gh-actions"
CTL_SKIP_WINDOWS = "skip-windows"
CTL_SKIP_MACOS = "skip-macos"
CTL_SKIP_BOOTSTRAP = "skip-bootstrap"
CTL_SKIP_WITHOUT_TPDE = "skip-without-tpde"
CTL_RUN_BUILTIN_TESTS = "run-builtin-tests"
CTL_DEBUG_SCRIPT = "debug.gdb"

# Names of the opt levels (in the order of REF_NAME_OPT_IR) for the -O cli option
OPT_LEVEL_NAMES = "0123sz"

GRAPH_NAME_AST = "AST"
GRAPH_NAME_DEP_GRAPH = "Dependency Graph"
GDB_READING_SYMBOLS_MESSAGE = "Reading symbols from "
GDB_INFERIOR_MESSAGE = "[Inferior"
# The compilers name themselves in the producer string of the IR and assembly code. The refs are shared with the host compiler
PRODUCER_MARKER_SELF_HOSTED = " [self-hosted] (https://github.com/spicelang/spice)"
PRODUCER_MARKER_HOST = " [host] (https://github.com/spicelang/spice)"

ERROR_HEADER = "[Error|"
PANIC_HEADER = "Program panicked at "
WARNING_REGEX = re.compile(r"\x1B\[33m(\[Warning\] [^\n]*?)\x1B\[0m")
ANSI_ESCAPE_REGEX = re.compile(r"\x1B\[[0-9;]*m")
SANITIZER_REPORT_REGEX = re.compile(r"==\d+==ERROR: (AddressSanitizer|LeakSanitizer)")


@dataclass(frozen=True)
class Suite:
    name: str  # Name of the test suite, e.g. 'ParserTests'
    dir_name: str  # Dir in test/test-files
    grouped: bool  # Test cases are grouped in sub dirs
    linter: bool = False  # Test cases run against the 'lint' subcommand


SUITES = [
    Suite("CommonTests", "common", False),
    Suite("LexerTests", "lexer", False),
    Suite("ParserTests", "parser", False),
    Suite("SymbolTableBuilderTests", "symboltablebuilder", True),
    Suite("TypeCheckerTests", "typechecker", True),
    Suite("IRGeneratorTests", "irgenerator", True),
    Suite("StdTests", "std", True),
    Suite("BenchmarkTests", "benchmark", False),
    Suite("ExampleTests", "examples", False),
    Suite("LinterTests", "linter", False, linter=True),
]
BUILTIN_TESTS_NAME = "BootstrapTests.BuiltinTests"


@dataclass(frozen=True)
class TestCase:
    suite: Suite
    name: str  # e.g. 'parser_errorExtraneousInput'
    path: str  # Relative to the work dir, e.g. './test-files/parser/error-extraneous-input'

    @property
    def full_name(self) -> str:
        return f"{self.suite.name}.{self.name}"

    def file(self, file_name: str) -> str:
        return os.path.join(self.path, file_name)


class TestFailed(Exception):
    pass


@dataclass
class TestResult:
    name: str
    failures: list[str] = field(default_factory=list)
    skipped: bool = False
    duration: float = 0.0

    def expect(self, condition: bool, message: str) -> None:
        if not condition:
            self.failures.append(message)

    def fail(self, message: str) -> None:
        self.failures.append(message)
        raise TestFailed()


@dataclass
class Options:
    compiler: Path | None  # Set after building the compiler under test, if not given
    build_compiler: Path | None
    work_dir: Path
    update_refs: bool
    coverage: bool
    asan: bool
    instrument: str | None
    is_github_actions: bool
    skip_sanitizer_tests: bool
    verbose: bool
    tpde: bool
    timeout: int


OPTS: Options  # Set in main()


# -------------------- Utilities --------------------


def to_camel_case(text: str) -> str:
    return re.sub(r"[-_](.)", lambda m: m.group(1).upper(), text)


def read_file(path: str | Path) -> str:
    with open(path, encoding="utf-8", errors="surrogateescape") as f:
        return f.read()


def write_file(path: str | Path, content: str) -> None:
    with open(path, "w", encoding="utf-8", errors="surrogateescape") as f:
        f.write(content)


def exit_code_of(return_code: int) -> int:
    # Like a shell, report the termination by a signal as 128 + signal number
    return 128 - return_code if return_code < 0 else return_code


def decode_output(output: bytes) -> str:
    # Like reading a file in text mode on Windows, convert the line endings
    if IS_WINDOWS:
        output = output.replace(b"\r\n", b"\n")
    return output.decode("utf-8", errors="surrogateescape")


def run_process(cmd: list[str] | str, capture_stderr: bool = True, shell: bool = False) -> tuple[str, int, str]:
    """Run the given command. Returns the output (incl. stderr, if captured), the exit code and the separately captured
    stderr output otherwise"""
    try:
        result = subprocess.run(cmd, shell=shell, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT if capture_stderr else subprocess.PIPE, timeout=OPTS.timeout or None)
    except subprocess.TimeoutExpired:
        raise TimeoutError(f"Timed out after {OPTS.timeout}s: {cmd if isinstance(cmd, str) else shlex.join(cmd)}")
    return decode_output(result.stdout), exit_code_of(result.returncode), decode_output(result.stderr or b"")


def parse_test_args(source_path: str) -> list[str]:
    """Get the compiler args from the '// TEST: ' header in the first line of the given source file"""
    if not os.path.exists(source_path):
        return []
    with open(source_path, encoding="utf-8", errors="surrogateescape") as f:
        first_line = f.readline().strip()
    if not first_line.startswith("// TEST: "):
        return []
    return [arg.strip() for arg in first_line[first_line.index(":") + 1:].strip().split(" ") if arg.strip()]


def contains_sanitizer_report(output: str) -> bool:
    """Check if the output contains a report of the AddressSanitizer or LeakSanitizer"""
    return SANITIZER_REPORT_REGEX.search(output) is not None


def emits_debug_info(args: list[str]) -> bool:
    """Check if the given compiler args request debug info. Like for the compiler, the last debug info arg wins"""
    debug_info = False
    for arg in args:
        if arg == "-g" or arg.startswith("--debug-info"):
            debug_info = arg != "--debug-info=none"
    return debug_info


def extract_error_message(output: str, exit_code: int) -> str | None:
    """
    The compiler prints the message of a compile error and exits with a non-zero exit code. The message is the last output,
    so it reaches from its header (e.g. '[Error|Semantic]') to the end of the output. It may be preceded by other output
    (e.g. the dumped AST).

    Internal compiler errors are raised as panics. They are no compile errors, so no error message is extracted for them,
    and the test case fails with the complete output.
    """
    # Only check for the header at the beginning of a line. The code snippet of a diagnostic may contain the text as well
    if exit_code == 0 or output.startswith(PANIC_HEADER) or "\n" + PANIC_HEADER in output:
        return None
    # The message begins at the first error header at the beginning of a line. Nested errors (e.g. the soft errors of an
    # 'Unresolved soft errors' compiler error) come after it
    message_start = 0
    if not output.startswith(ERROR_HEADER):
        message_start = output.find("\n" + ERROR_HEADER)
        if message_start == -1:
            return None
        message_start += 1
    message = output[message_start:]
    # Remove the line break, that terminates the message
    message = message.removesuffix("\n").removesuffix("\r")
    # Normalize path separators on Windows
    return message.replace("\\", "/")


def extract_serialized_graph(output: str, graph_name: str) -> str | None:
    """Extract a serialized graph (e.g. the AST from '--dump-ast') from the console output of the compiler"""
    caption = f"Serialized {graph_name}:\n\n"
    start = output.find(caption)
    if start == -1:
        return None
    start += len(caption)
    # All lines of the dot code are indented, except the first one and the closing brace
    end = output.find("\n}", start)
    if end == -1:
        return None
    return output[start:end + 2]


def extract_warnings(output: str) -> str:
    """
    Extract the warnings of the main source file from the console output of the compiler. It prints every warning in its own
    line, colored yellow, after the warnings of the source files the main source file depends on. These are skipped.
    """
    warnings = []
    for match in WARNING_REGEX.finditer(output):
        warning = match.group(1)
        if warning.startswith("[Warning] ./") and not warning.startswith("[Warning] ./source.spice:"):
            continue
        warnings.append(warning + "\n")
    return "".join(warnings)


def erase_lines_by_substring(code: str, needle: str) -> str:
    """Remove the lines containing the given substring. Like the host test runner, keep the line breaks"""
    return "\n".join("" if needle in line else line for line in code.split("\n"))


def erase_gdb_header(gdb_output: str) -> str:
    """Remove the GDB header up to and including the 'Reading symbols from <path>' line, and the inferior message"""
    pos = gdb_output.find(GDB_READING_SYMBOLS_MESSAGE)
    if pos != -1:
        line_end = gdb_output.find("\n", pos)
        gdb_output = gdb_output[line_end + 1:] if line_end != -1 else ""
    pos = gdb_output.find(GDB_INFERIOR_MESSAGE)
    if pos != -1:
        gdb_output = gdb_output[:pos]
    return gdb_output


def normalize_producer_string(code: str) -> str:
    return code.replace(PRODUCER_MARKER_SELF_HOSTED, PRODUCER_MARKER_HOST)


def erase_dso_local_markers(ir_code: str) -> str:
    # The LLVM C API, which the self-hosted compiler uses, cannot mark global values as dso_local
    return ir_code.replace(" dso_local ", " ")


# -------------------- Reference files --------------------


def get_bootstrap_ref_path(ref_path: str) -> str:
    """Get the path of the bootstrap variant of the given ref file, e.g. 'ir-code-bootstrap.ll' for 'ir-code.ll'"""
    stem, ext = os.path.splitext(ref_path)
    return f"{stem}-bootstrap{ext}"


def expand_ref_paths(ref_path: str) -> list[str]:
    """
    Get all variants of the given ref file, ordered from the most to the least specific one. The bootstrap variants
    (e.g. 'ir-code-bootstrap.ll') come first, for references where the self-hosted compiler differs from the host compiler.
    """
    ref_paths = []
    for base_ref_path in (get_bootstrap_ref_path(ref_path), ref_path):
        stem, ext = os.path.splitext(base_ref_path)
        ref_paths += [f"{stem}-{TARGET_OS}-{TARGET_ARCH}{ext}", f"{stem}-{TARGET_OS}{ext}", base_ref_path]
    return ref_paths


def does_ref_exist(ref_path: str) -> bool:
    return any(os.path.exists(path) for path in expand_ref_paths(ref_path))


def is_compared_in_current_mode(ref_path: str) -> bool:
    """
    The coverage mode does not compare any reference file. The ASAN mode only compares the output and exit code of the
    compiled test program, since the sanitizer instrumentation changes the generated code, but must not change the behavior
    """
    if OPTS.coverage:
        return False
    if OPTS.asan:
        return os.path.basename(ref_path) in (REF_NAME_EXECUTION_OUTPUT, REF_NAME_EXIT_CODE)
    return True


def check_ref_match(result: TestResult, original_ref_path: str, get_actual_output: Callable[[], str],
                    modify_output: Callable[[str, str], tuple[str, str]] | None = None, x86_only: bool = False) -> bool:
    """
    Check if the actual output matches the most specific variant of the given ref file, if one exists

    :return: True, if a variant of the ref file was found
    """
    for ref_path in expand_ref_paths(original_ref_path):
        if not os.path.exists(ref_path):
            continue
        if OPTS.verbose:
            print(f"{result.name}: checking ref file {ref_path}")
        # Get the actual output. This also drives the compiler runs, that produce it
        actual_output = get_actual_output()
        # Cancel early, before comparing or updating the refs
        if x86_only and not IS_X86_64 and ref_path in (original_ref_path, get_bootstrap_ref_path(original_ref_path)):
            return True
        # Only the bootstrap refs may be updated. The other refs hold the output of the host compiler
        bootstrap_stem = Path(get_bootstrap_ref_path(original_ref_path)).stem
        if OPTS.update_refs and os.path.basename(ref_path).startswith(bootstrap_stem):
            write_file(ref_path, actual_output)
        elif is_compared_in_current_mode(original_ref_path):
            expected_output = read_file(ref_path)
            if modify_output is not None:
                expected_output, actual_output = modify_output(expected_output, actual_output)
            if expected_output != actual_output:
                result.failures.append(f"Output does not match the reference file: {ref_path}\n"
                                       f"{diff(expected_output, actual_output, ref_path)}")
        return True
    return False


def diff(expected: str, actual: str, ref_path: str) -> str:
    lines = difflib.unified_diff(expected.splitlines(keepends=True), actual.splitlines(keepends=True),
                                 fromfile=f"{ref_path} (expected)", tofile="actual", n=2)
    text = "".join(line if line.endswith("\n") else line + "\n" for line in lines)
    # Do not flood the console with huge diffs (e.g. of IR code)
    max_lines = 80
    text_lines = text.splitlines(keepends=True)
    if len(text_lines) > max_lines:
        text = "".join(text_lines[:max_lines]) + f"... ({len(text_lines) - max_lines} more diff lines)\n"
    return text


# -------------------- Test cases --------------------


def collect_test_cases(suite: Suite) -> list[TestCase]:
    suite_path = TEST_DIR / "test-files" / suite.dir_name

    def subdirs(path: Path) -> list[str]:
        return sorted(entry.name for entry in path.iterdir() if entry.is_dir()) if path.is_dir() else []

    test_cases = []
    if suite.grouped:
        for group in subdirs(suite_path):
            for case in subdirs(suite_path / group):
                path = os.path.join(PATH_TEST_FILES, suite.dir_name, group, case)
                test_cases.append(TestCase(suite, f"{to_camel_case(group)}_{to_camel_case(case)}", path))
    else:
        for case in subdirs(suite_path):
            path = os.path.join(PATH_TEST_FILES, suite.dir_name, case)
            test_cases.append(TestCase(suite, f"{to_camel_case(suite.dir_name)}_{to_camel_case(case)}", path))
    return test_cases


def matches_filter(name: str, filter_str: str) -> bool:
    """Check the name against a GoogleTest-style filter: 'pos1:pos2-neg1:neg2'"""
    positive, _, negative = filter_str.partition("-")
    positive_patterns = [p for p in positive.split(":") if p] or ["*"]
    negative_patterns = [p for p in negative.split(":") if p]
    return (any(fnmatch.fnmatchcase(name, p) for p in positive_patterns) and
            not any(fnmatch.fnmatchcase(name, p) for p in negative_patterns))


def is_disabled(test_case: TestCase) -> bool:
    def exists(file_name: str) -> bool:
        return os.path.exists(test_case.file(file_name))

    if exists(CTL_SKIP_DISABLED):
        return True
    if OPTS.is_github_actions and exists(CTL_SKIP_GH):
        return True
    # Some test cases check host specifics, that the self-hosted compiler does not replicate
    if exists(CTL_SKIP_BOOTSTRAP):
        return True
    test_args = parse_test_args(test_case.file(REF_NAME_SOURCE))
    # In ASAN mode, every test program is built with AddressSanitizer, which cannot be combined with another sanitizer
    if (OPTS.skip_sanitizer_tests or OPTS.asan) and any(arg.startswith("--sanitizer") for arg in test_args):
        return True
    # Code coverage instrumentation is rejected in combination with LTO or the TPDE backend, and is not tested together with
    # the sanitizers
    if OPTS.coverage and any(arg in ("-lto", "--backend=tpde", "--backend=TPDE") or arg.startswith("--sanitizer")
                             for arg in test_args):
        return True
    # Test cases, that use TPDE, need a compiler with TPDE support
    if not OPTS.tpde and (exists(CTL_SKIP_WITHOUT_TPDE) or any(arg in ("--backend=tpde", "--backend=TPDE")
                                                                for arg in test_args)):
        return True
    if IS_WINDOWS and exists(CTL_SKIP_WINDOWS):
        return True
    if IS_MACOS and exists(CTL_SKIP_MACOS):
        return True
    return False


def prepare_artifact_dir(test_case: TestCase) -> Path:
    artifact_dir = OPTS.work_dir / "tests" / test_case.suite.name / test_case.name
    shutil.rmtree(artifact_dir, ignore_errors=True)
    artifact_dir.mkdir(parents=True)
    return artifact_dir


def run_compiler(result: TestResult, args: list[str]) -> tuple[str, int]:
    """Run the compiler under test with the given args, capturing stdout and stderr"""
    output, exit_code, _ = run_process([str(OPTS.compiler), *args])
    if OPTS.verbose:
        print(f"{result.name}: compiler output of '{shlex.join(args)}':\n{output}")
    # An ASAN-instrumented compiler reports memory errors, even if it compiles the test case like expected
    result.expect(not contains_sanitizer_report(output), f"Sanitizer report:\n{output}")
    return output, exit_code


def check_execution(result: TestResult, test_case: TestCase, executable_path: Path) -> None:
    """Execute the compiled test program and check its output and exit code against the references, if there are any"""
    check_output = does_ref_exist(test_case.file(REF_NAME_EXECUTION_OUTPUT))
    check_exit_code = does_ref_exist(test_case.file(REF_NAME_EXIT_CODE))
    if not check_output and not check_exit_code:
        return

    # Like a user would do, run the program via the shell, so the cli flags are split like the shell does it
    cmd = subprocess.list2cmdline([str(executable_path)]) if IS_WINDOWS else shlex.quote(str(executable_path))
    cli_flags_file = test_case.file(INPUT_NAME_CLI_FLAGS)
    if os.path.exists(cli_flags_file):
        cli_flags = [line for line in read_file(cli_flags_file).splitlines() if line]
        if cli_flags:
            cmd += " " + cli_flags[0]
    # In ASAN mode, always capture stderr as well: a sanitizer report has to fail the test case, even if it only checks an exit
    # code, that might match the one of the sanitizer
    capture_stderr = check_output or OPTS.asan
    output, exit_code, stderr = run_process(cmd, capture_stderr=capture_stderr, shell=True)
    if OPTS.asan:
        result.expect(not contains_sanitizer_report(output), f"Sanitizer report:\n{output}")

    check_ref_match(result, test_case.file(REF_NAME_EXECUTION_OUTPUT), lambda: output)

    # Windows does not give us the exit code, so we cannot check it on Windows
    if not IS_WINDOWS:
        ref_exists = check_ref_match(result, test_case.file(REF_NAME_EXIT_CODE), lambda: str(exit_code))
        if not ref_exists:
            result.expect(exit_code == 0, f"Program exited with non-zero exit code {exit_code}:\n{output}{stderr}")


def check_debugger_output(result: TestResult, test_case: TestCase, executable_path: Path) -> None:
    """Run the debug script of the test case on the compiled test program and check the debugger output"""
    if OPTS.is_github_actions:  # GDB tests are currently not supported on GitHub Actions
        return

    def get_gdb_output() -> str:
        gdb_script_path = test_case.file(CTL_DEBUG_SCRIPT)
        result.expect(os.path.exists(gdb_script_path), "Debug output requested, but debug script not found")
        # -nx: ignore any local/system .gdbinit, so a developer's own GDB setup cannot change the test output
        output, exit_code, _ = run_process(["gdb", "-nx", "-x", gdb_script_path, str(executable_path)], capture_stderr=False)
        if not IS_WINDOWS:
            result.expect(exit_code == 0, "GDB exited with non-zero exit code when running debug script")
        return output

    check_ref_match(result, test_case.file(REF_NAME_GDB_OUTPUT), get_gdb_output,
                    lambda expected, actual: (erase_gdb_header(expected), erase_gdb_header(actual)))


def exec_test_case(result: TestResult, test_case: TestCase) -> None:
    """
    Run a reference test case against the compiler under test. It checks all reference outputs: the serialized AST, the
    symbol table, the raised error, the dependency graph, the IR code of all opt levels, the assembly code, the warnings of
    the main source file, the type registry, the cache stats, the execution output and exit code of the compiled program,
    and the debugger output. Beyond that, each test case checks that the compiler runs through all stages without crashing
    or raising an unexpected error.
    """
    main_source_path = test_case.file(REF_NAME_SOURCE)
    artifact_dir = prepare_artifact_dir(test_case)
    executable_path = artifact_dir / f"source{EXE_SUFFIX}"
    check_ast = does_ref_exist(test_case.file(REF_NAME_SYNTAX_TREE))
    check_dep_graph = does_ref_exist(test_case.file(REF_NAME_DEP_GRAPH))
    needs_executable = (does_ref_exist(test_case.file(REF_NAME_EXECUTION_OUTPUT)) or
                        does_ref_exist(test_case.file(REF_NAME_EXIT_CODE)) or
                        does_ref_exist(test_case.file(REF_NAME_GDB_OUTPUT)))
    test_args = parse_test_args(main_source_path)

    def build_args(output_path: Path) -> list[str]:
        # Bypass the compilation cache: the dumps would be empty for files restored from it
        args = ["build", "--test-mode", "--ignore-cache", *test_args]
        if os.path.exists(test_case.file(CTL_RUN_BUILTIN_TESTS)):
            args.append("--test-main")
        if OPTS.coverage:
            args.append("--coverage")
        if OPTS.asan:
            args += ["--sanitizer", "address"]
        return args + ["--output", str(output_path)]

    def read_dump(dump_dir: Path, dump_name: str) -> str:
        # The compiler writes the dumps of the main source file to '<output dir>/source-<dump name>'
        dump_path = dump_dir / f"source-{dump_name}"
        return read_file(dump_path) if dump_path.exists() else ""

    args = build_args(executable_path)
    # Only link an executable if it gets executed afterwards. If an error is expected, keep the executable output container:
    # some errors (e.g. a missing main function) are only raised for executables
    error_ref_path = test_case.file(REF_NAME_ERROR_OUTPUT)
    expects_error = does_ref_exist(error_ref_path)
    if not needs_executable and not expects_error:
        args += ["--output-container", "obj"]
    # The graphs are dumped to the console, since dumping them to files requires Graphviz to render them
    if check_ast:
        args.append("--dump-ast")
    if check_dep_graph:
        args.append("--dump-dependency-graph")

    output, exit_code = run_compiler(result, [*args, main_source_path])

    # Check if the compiler raised an error
    error_message = extract_error_message(output, exit_code)
    if error_message is not None:
        if not expects_error:
            result.fail(f"Expected no error, but got: {error_message}")
        check_ref_match(result, error_ref_path, lambda: error_message)
        return
    if exit_code != 0:
        result.fail(f"Compiler exited with code {exit_code}:\n{output}")

    # Check AST
    def get_ast() -> str:
        ast_string = extract_serialized_graph(output, GRAPH_NAME_AST)
        result.expect(ast_string is not None, f"Compiler did not dump the AST:\n{output}")
        return ast_string or ""

    check_ref_match(result, test_case.file(REF_NAME_SYNTAX_TREE), get_ast)

    # Dump the outputs, the compiler can only write to files, in a separate run, to not dump the graphs to files as well. The
    # assembly code is not checked when running on GitHub Actions
    check_symbol_table = does_ref_exist(test_case.file(REF_NAME_SYMBOL_TABLE))
    check_assembly = not OPTS.is_github_actions and does_ref_exist(test_case.file(REF_NAME_ASM))
    check_type_registry = does_ref_exist(test_case.file(REF_NAME_TYPE_REGISTRY))
    check_cache_stats = does_ref_exist(test_case.file(REF_NAME_CACHE_STATS))
    dump_dir = artifact_dir / "dumps"
    if check_symbol_table or check_assembly or check_type_registry or check_cache_stats:
        dump_dir.mkdir()
        dump_args = build_args(dump_dir / "object.o") + ["--output-container", "obj", "--dump-to-files"]
        # The IR is checked for each opt level, for which a reference exists. Emit the assembly code with the last of these
        # opt levels, like the host test runner does
        checked_opt_levels = [i for i, ref in enumerate(REF_NAME_OPT_IR) if does_ref_exist(test_case.file(ref))]
        if checked_opt_levels:
            dump_args.append(f"-O{OPT_LEVEL_NAMES[checked_opt_levels[-1]]}")
        if check_symbol_table:
            dump_args.append("--dump-symtab")
        if check_assembly:
            dump_args.append("--dump-assembly")
        if check_type_registry:
            dump_args.append("--dump-types")
        if check_cache_stats:
            dump_args.append("--dump-cache-stats")
        dump_output, dump_exit_code = run_compiler(result, [*dump_args, main_source_path])
        result.expect(dump_exit_code == 0, f"Compiler exited with code {dump_exit_code}:\n{dump_output}")

    # Check symbol table
    check_ref_match(result, test_case.file(REF_NAME_SYMBOL_TABLE), lambda: read_dump(dump_dir, "symbol-table.json"))

    # Fail if an error was expected
    if expects_error:
        result.fail("Expected error, but got no error")

    # Check dependency graph
    def get_dep_graph() -> str:
        dep_graph_string = extract_serialized_graph(output, GRAPH_NAME_DEP_GRAPH)
        result.expect(dep_graph_string is not None, f"Compiler did not dump the dependency graph:\n{output}")
        return dep_graph_string or ""

    check_ref_match(result, test_case.file(REF_NAME_DEP_GRAPH), get_dep_graph)

    # Check IR code. The compiler dumps the optimized IR of every source file into the output dir, so it is compiled once per
    # opt level
    debug_info = emits_debug_info(test_args)
    for opt_level in range(len(REF_NAME_OPT_IR)):
        def get_ir(i: int = opt_level) -> str:
            ir_artifact_dir = artifact_dir / f"ir-O{i}"
            ir_artifact_dir.mkdir()
            ir_args = build_args(ir_artifact_dir / "object.o")
            ir_args += [f"-O{OPT_LEVEL_NAMES[i]}", "--output-container", "obj", "--dump-ir", "--dump-to-files"]
            ir_output, ir_exit_code = run_compiler(result, [*ir_args, main_source_path])
            result.expect(ir_exit_code == 0, f"Compiler exited with code {ir_exit_code}:\n{ir_output}")
            # With LTO, the compiler dumps the IR of the LTO module after the post-link optimization
            return read_dump(ir_artifact_dir, "ir-code-lto-post-link.ll" if "-lto" in ir_args else f"ir-code-O{i}.ll")

        def modify_ir(expected: str, actual: str) -> tuple[str, str]:
            if debug_info:
                # Remove the lines, containing paths on the local file system
                expected = erase_lines_by_substring(expected, " = !DIFile(filename:")
                actual = erase_lines_by_substring(actual, " = !DIFile(filename:")
            expected = normalize_producer_string(erase_dso_local_markers(expected))
            actual = normalize_producer_string(erase_dso_local_markers(actual))
            return expected, actual

        check_ref_match(result, test_case.file(REF_NAME_OPT_IR[opt_level]), get_ir, modify_ir, x86_only=True)

    # Check assembly code
    if check_assembly:
        check_ref_match(result, test_case.file(REF_NAME_ASM), lambda: read_dump(dump_dir, "assembly-code.s"),
                        lambda expected, actual: (expected, normalize_producer_string(actual)))

    # Check warnings
    check_ref_match(result, test_case.file(REF_NAME_WARNING_OUTPUT), lambda: extract_warnings(output))

    # Check type registry and cache stats output
    check_ref_match(result, test_case.file(REF_NAME_TYPE_REGISTRY), lambda: read_dump(dump_dir, "type-registry.out"))
    check_ref_match(result, test_case.file(REF_NAME_CACHE_STATS), lambda: read_dump(dump_dir, "cache-stats.out"))

    if needs_executable:
        check_execution(result, test_case, executable_path)
        check_debugger_output(result, test_case, executable_path)


def exec_linter_test_case(result: TestResult, test_case: TestCase) -> None:
    """
    Run a lint test case: invoke the 'lint' subcommand and compare the emitted findings against lint.out. The compiler
    colorizes its findings, so the captured output is de-colorized before being matched against the plain-text reference.
    """
    output, exit_code = run_compiler(result, ["lint", test_case.file(REF_NAME_SOURCE)])

    error_ref_path = test_case.file(REF_NAME_ERROR_OUTPUT)
    error_message = extract_error_message(output, exit_code)
    if error_message is not None:
        if not does_ref_exist(error_ref_path):
            result.fail(f"Expected no error, but got: {error_message}")
        check_ref_match(result, error_ref_path, lambda: error_message)
        return
    if exit_code != 0:
        result.fail(f"Compiler exited with code {exit_code}:\n{output}")
    if does_ref_exist(error_ref_path):
        result.fail("Expected error, but got no error")

    check_ref_match(result, test_case.file(REF_NAME_LINT_OUTPUT), lambda: ANSI_ESCAPE_REGEX.sub("", output))


# -------------------- Compiler builds --------------------


def instrumentation_flags(builtin_tests: bool) -> list[str]:
    # Coverage instrumentation does not support LTO. Without optimizations, the coverage counters map best to the source
    if OPTS.instrument == "coverage":
        return ["-O0", "--coverage"]
    # Without LTO and with fewer optimizations, the sanitizer reports point to the right source locations
    if OPTS.instrument == "asan":
        return ["-O1", "--sanitizer", "address"]
    # The builtin tests are built without optimizations, which keeps the build fast
    return ["-O0"] if builtin_tests else ["-O3", "-lto"]


def build_compiler_sources(compiler: Path, output_dir: Path, executable_name: str, builtin_tests: bool) -> Path:
    """
    Build the compiler sources in src/ with the given compiler into a clean output dir

    :return: Path of the built executable
    """
    shutil.rmtree(output_dir, ignore_errors=True)
    output_dir.mkdir(parents=True)
    executable_path = output_dir / f"{executable_name}{EXE_SUFFIX}"
    cmd = [str(compiler), "build", *instrumentation_flags(builtin_tests)]
    # The test build mode generates a main function, which runs all builtin tests of the source files
    if builtin_tests:
        cmd += ["--build-mode", "test"]
    cmd += ["--ignore-cache", "--output", str(executable_path), str(ROOT_DIR / "src" / "main.spice")]
    result = subprocess.run(cmd, cwd=ROOT_DIR, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            encoding="utf-8", errors="surrogateescape")
    # The compiler sources emit lots of warnings, so the output is only of interest if something went wrong
    if result.returncode != 0 or not executable_path.is_file():
        raise RuntimeError(f"Building {executable_path.name} failed with exit code {exit_code_of(result.returncode)}: "
                           f"{shlex.join(cmd)}\n{result.stdout}")
    return executable_path


def exec_builtin_tests(result: TestResult) -> None:
    """
    Build the compiler sources in test build mode and run their builtin tests (#[test] functions, e.g. in src/driver.spice),
    with the same instrumentation as the compiler under test. They are built by the compiler, that built the compiler under
    test, or by the compiler under test itself, if it was built elsewhere.
    """
    compiler = OPTS.build_compiler or OPTS.compiler
    try:
        executable_path = build_compiler_sources(compiler, OPTS.work_dir / "bootstrap-tests", "spice-tests", True)
    except RuntimeError as error:
        result.fail(f"Could not build the builtin tests of the compiler sources: {error}")
    output, exit_code, _ = run_process([str(executable_path)])
    if OPTS.verbose:
        print(f"Builtin tests output:\n{output}")
    result.expect(not contains_sanitizer_report(output), f"Sanitizer report:\n{output}")
    result.expect(exit_code == 0, f"Builtin tests failed:\n{output}")


# -------------------- Main --------------------


def run_test(test_case: TestCase | None) -> TestResult:
    result = TestResult(test_case.full_name if test_case else BUILTIN_TESTS_NAME)
    start = time.monotonic()
    try:
        if test_case is None:
            exec_builtin_tests(result)
        elif is_disabled(test_case):
            result.skipped = True
        elif test_case.suite.linter:
            exec_linter_test_case(result, test_case)
        else:
            exec_test_case(result, test_case)
    except TestFailed:
        # Raised by TestResult.fail() to stop the test case, after it recorded the failure
        pass
    except Exception as error:  # noqa: BLE001 - report any runner error as test failure, like GoogleTest does
        result.failures.append(f"Test runner error: {type(error).__name__}: {error}")
    result.duration = time.monotonic() - start
    return result


def prepare_work_dir() -> None:
    """Create the work dir with a link to the test files and make it the working directory"""
    OPTS.work_dir.mkdir(parents=True, exist_ok=True)
    link = OPTS.work_dir / "test-files"
    target = TEST_DIR / "test-files"
    if link.is_symlink() or link.exists():
        if link.resolve() != target.resolve():
            bootstrap.fail(f"{link} exists, but does not link to {target}")
    else:
        try:
            link.symlink_to(target, target_is_directory=True)
        except OSError:
            if not IS_WINDOWS:
                raise
            # Creating a symlink requires extra privileges on Windows, a junction does not. Prefer the symlink though: Windows
            # resolves relative symlink targets within the test files (e.g. in the import path traversal test case) against
            # the path through the junction, so they would point outside the test dir
            import _winapi
            _winapi.CreateJunction(str(target), str(link))
    os.chdir(OPTS.work_dir)


def is_tpde_available() -> bool:
    """
    The TPDE backend of the compiler under test and the std TPDE bindings, that some test cases use, need the TPDE libraries.
    The compilers take them from TPDE_FLAGS or, if it is not set, from the std (see 'setup-deps.py --tpde'). An empty TPDE_FLAGS
    disables them. TPDE only supports ELF targets
    """
    if "TPDE_FLAGS" in os.environ:
        return bool(os.environ["TPDE_FLAGS"])
    return sys.platform.startswith("linux") and bootstrap.std_ships_tpde()


def setup_environment() -> None:
    """Set up the environment, that the compilers need to build the compiler sources and the test cases"""
    # Always test the std and compiler sources of this checkout
    os.environ["SPICE_STD_DIR"] = str(ROOT_DIR / "std")
    os.environ["SPICE_BOOTSTRAP_DIR"] = str(ROOT_DIR / "src")
    if "LLVM_LIB_DIR" not in os.environ:
        llvm_lib_dir = bootstrap.find_llvm_lib_dir()
        if llvm_lib_dir is None:
            bootstrap.fail("LLVM library directory not found. Set LLVM_LIB_DIR or LLVM_DIR, or put llvm-config on the PATH")
        os.environ["LLVM_LIB_DIR"] = llvm_lib_dir
    if "LLVM_INCLUDE_DIRS" not in os.environ:
        llvm_include_dirs = bootstrap.find_llvm_include_dirs()
        if llvm_include_dirs is None:
            bootstrap.fail("LLVM include directories not found. Set LLVM_INCLUDE_DIRS (e.g. '-I<dir1> -I<dir2>') or "
                           "LLVM_DIR, or put llvm-config on the PATH")
        os.environ["LLVM_INCLUDE_DIRS"] = llvm_include_dirs


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Run the reference test suites against the self-hosted Spice compiler.")
    compiler_group = parser.add_mutually_exclusive_group()
    compiler_group.add_argument("--compiler", type=Path, default=None,
                                help="Run the test cases against the given, already built compiler")
    compiler_group.add_argument("--build-compiler", type=Path, default=None,
                                help="Build the compiler under test from src/ with the given compiler first, e.g. the host "
                                     "compiler or the released stage0 compiler (default: the stage0 compiler from 'fetch-stage0.py' in "
                                     "build/stage0/, else the host compiler, first found in "
                                     "build/, cmake-build-release/, cmake-build-debug/)")
    parser.add_argument("--build-only", action="store_true",
                        help="Only build the compiler under test into <work-dir>/bootstrap-compiler, without running any test")
    parser.add_argument("--instrument", choices=["coverage", "asan"], default=None,
                        help="Build the compiler under test and its builtin tests with Spice code coverage or AddressSanitizer "
                             "instrumentation. With asan, every test case fails, for which the sanitizer reports an error. "
                             "With --compiler, only the builtin tests get instrumented")
    parser.add_argument("--update-refs", action="store_true",
                        help="Update the bootstrap refs (e.g. ir-code-bootstrap.ll) of the selected test cases. The other "
                             "refs hold the output of the host compiler and are never overwritten")
    parser.add_argument("--coverage", action="store_true",
                        help="Compile and run the test programs with Spice code coverage instrumentation, skipping all "
                             "reference output comparisons")
    parser.add_argument("--asan", action="store_true",
                        help="Compile and run the test programs with AddressSanitizer, only comparing the execution output "
                             "and exit code")
    parser.add_argument("--is-github-actions", action="store_true",
                        help="Skip test cases and checks that are not supported on GitHub Actions")
    parser.add_argument("--skip-sanitizer-tests", action="store_true",
                        help="Skip test cases that exercise Spice language sanitizers")
    parser.add_argument("--filter", "--gtest_filter", dest="filter", default="*",
                        help="Only run the test cases matching this GoogleTest-style filter, e.g. "
                             "'ParserTests.*:LexerTests.*-*Error*'")
    parser.add_argument("--list", action="store_true", help="List the selected test cases instead of running them")
    parser.add_argument("-j", "--jobs", type=int, default=os.cpu_count() or 1,
                        help="Number of test cases to run in parallel (default: number of CPUs)")
    parser.add_argument("--timeout", type=int, default=1800,
                        help="Timeout in seconds for a single process, e.g. a compiler run (default: 1800, 0 = none)")
    parser.add_argument("--work-dir", type=Path, default=ROOT_DIR / "build" / "test-tmp",
                        help="Directory for the build artifacts (default: build/test-tmp)")
    parser.add_argument("-v", "--verbose", action="store_true", help="Print the output of every compiler run")
    args = parser.parse_args()

    # Coverage and ASAN mode never compare against reference files, so they cannot update them either
    for flag in ("coverage", "asan", "instrument", "build_only"):
        if args.update_refs and getattr(args, flag):
            parser.error(f"--update-refs cannot be combined with --{flag.replace('_', '-')}")
    if args.coverage and args.asan:
        parser.error("--coverage cannot be combined with --asan")
    # The coverage data of the compiler under test would mix with the one of the instrumented test programs
    if args.coverage and args.instrument == "coverage":
        parser.error("--coverage cannot be combined with --instrument coverage")
    if args.build_only and args.compiler:
        parser.error("--build-only cannot be combined with --compiler")
    if args.compiler is None and args.build_compiler is None:
        args.build_compiler = bootstrap.find_stage0_compiler() or bootstrap.find_host_compiler()
        if args.build_compiler is None:
            parser.error("No compiler found to build the compiler under test with. Download the stage0 compiler "
                         "('python fetch-stage0.py'), or pass --build-compiler or --compiler")
    for path in (args.compiler, args.build_compiler):
        if path is not None and not path.is_file():
            parser.error(f"Compiler not found: {path}")
    return args


def main() -> None:
    global OPTS
    args = parse_args()
    if TARGET_ARCH is None:
        bootstrap.fail(f"Unsupported architecture: {platform.machine()}")
    setup_environment()
    OPTS = Options(
        compiler=args.compiler.resolve() if args.compiler else None,
        build_compiler=args.build_compiler.resolve() if args.build_compiler else None,
        work_dir=args.work_dir.resolve(),
        update_refs=args.update_refs,
        coverage=args.coverage,
        asan=args.asan,
        instrument=args.instrument,
        is_github_actions=args.is_github_actions,
        skip_sanitizer_tests=args.skip_sanitizer_tests,
        verbose=args.verbose,
        tpde=is_tpde_available(),
        timeout=args.timeout,
    )
    prepare_work_dir()

    # Collect the selected test cases. None stands for the builtin tests of the compiler sources
    test_cases: list[TestCase | None] = [None] if matches_filter(BUILTIN_TESTS_NAME, args.filter) else []
    for suite in SUITES:
        test_cases += [tc for tc in collect_test_cases(suite) if matches_filter(tc.full_name, args.filter)]
    if args.list:
        for test_case in test_cases:
            print(test_case.full_name if test_case else BUILTIN_TESTS_NAME)
        return

    # Build the compiler under test
    if OPTS.compiler is None:
        bootstrap.log(f"Building the compiler under test with {OPTS.build_compiler} ...")
        start = time.monotonic()
        try:
            OPTS.compiler = build_compiler_sources(OPTS.build_compiler, OPTS.work_dir / "bootstrap-compiler", "spice", False)
        except RuntimeError as error:
            bootstrap.fail(str(error))
        bootstrap.log(f"Built {OPTS.compiler} in {time.monotonic() - start:.1f}s.")
        if args.build_only:
            return
    if not test_cases:
        bootstrap.fail(f"No test case matches the filter '{args.filter}'")

    # Run the test cases in parallel. The builtin tests take the longest, so they are started first
    bootstrap.log(f"Running {len(test_cases)} test cases against {OPTS.compiler} with {args.jobs} jobs ...")
    start = time.monotonic()
    results: list[TestResult] = []
    with ThreadPoolExecutor(max_workers=max(args.jobs, 1)) as executor:
        futures = [executor.submit(run_test, test_case) for test_case in test_cases]
        for future in as_completed(futures):
            result = future.result()
            results.append(result)
            progress = f"[{len(results)}/{len(test_cases)}]"
            if result.failures:
                print(f"{progress} {RED}FAILED{NC}  {result.name} ({result.duration:.1f}s)")
                for failure in result.failures:
                    print("  " + failure.rstrip("\n").replace("\n", "\n  "))
            elif result.skipped:
                print(f"{progress} {YELLOW}SKIPPED{NC} {result.name}")
            else:
                print(f"{progress} {GREEN}PASSED{NC}  {result.name} ({result.duration:.1f}s)")
            sys.stdout.flush()

    failed = sorted(result.name for result in results if result.failures)
    skipped = sum(result.skipped for result in results)
    passed = len(results) - len(failed) - skipped
    print(f"\n{passed} passed, {len(failed)} failed, {skipped} skipped in {time.monotonic() - start:.1f}s")
    if failed:
        print(f"{RED}Failed test cases:{NC}")
        for name in failed:
            print(f"  {name}")
        sys.exit(1)


if __name__ == "__main__":
    main()

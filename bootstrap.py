#!/usr/bin/env python3
"""Bootstrap the self-hosted Spice compiler until it reaches a fixed point.

Stage 0 is the compiler, that the bootstrapping starts from: the released self-hosted compiler, that fetch-stage0.py
downloads, or the host compiler (src-host/). Every following stage n is the bootstrap compiler (src/) built by stage n-1.
Stage 1 and stage 2 naturally differ, because they come from different compilers. From stage 3 on, every stage is built by
a compiler that was built from the very same sources, so stage n and stage n-1 have to be bit-identical. The script
succeeds as soon as two consecutive stages have the same hash (the fixed point) and fails if this does not happen within
the given number of iterations or if any stage fails to build.
"""
import argparse
import hashlib
import os
import re
import shutil
import subprocess
import sys
import time
from pathlib import Path

GREEN = "\033[0;92m"
RED = "\033[0;91m"
NC = "\033[0m"

ROOT_DIR = Path(__file__).resolve().parent
EXE_NAME = "spice.exe" if sys.platform == "win32" else "spice"
# Optimized with LTO, like the test runner builds the bootstrap compiler (see test/run-tests.py)
DEFAULT_BUILD_FLAGS = ["-O3", "-lto", "--native-features", "--strip-symbols"]


def log(msg: str) -> None:
    print(f"{GREEN}{msg}{NC}", flush=True)


def fail(msg: str) -> None:
    print(f"{RED}{msg}{NC}", file=sys.stderr, flush=True)
    sys.exit(1)


def find_stage0_compiler() -> Path | None:
    # Downloaded by fetch-stage0.py. A download of another version than the pinned one (e.g. from before a version bump or
    # from another checkout) is not used silently
    stage0_dir = ROOT_DIR / "build" / "stage0"
    candidate = stage0_dir / EXE_NAME
    if not candidate.is_file():
        return None
    pinned_version = (ROOT_DIR / ".github" / "stage0-version").read_text().strip()
    stamp = stage0_dir / "version"
    downloaded_version = stamp.read_text().strip() if stamp.is_file() else "unknown"
    if downloaded_version != pinned_version:
        fail(f"The stage0 compiler in {stage0_dir} has version {downloaded_version}, but {pinned_version} is pinned. "
             "Download it again with 'python fetch-stage0.py'")
    return candidate


def find_host_compiler() -> Path | None:
    for build_dir in ("build", "cmake-build-release", "cmake-build-debug"):
        candidate = ROOT_DIR / build_dir / "src-host" / EXE_NAME
        if candidate.is_file():
            return candidate
    return None


def find_llvm_lib_dir() -> str | None:
    # Derive it from LLVM_DIR (<llvm-build>/lib/cmake/llvm), which the CMake build of the host compiler uses as well
    if llvm_dir := os.environ.get("LLVM_DIR"):
        lib_dir = Path(llvm_dir).resolve().parent.parent
        if lib_dir.is_dir():
            return str(lib_dir)
    if llvm_config := shutil.which("llvm-config"):
        return subprocess.run([llvm_config, "--libdir"], check=True, capture_output=True, text=True).stdout.strip()
    return None


def find_llvm_include_dirs() -> str | None:
    # The LLVM std bindings compile a C wrapper, which needs the LLVM headers. In an LLVM build tree, they live in two
    # directories (source and build tree), which LLVMConfig.cmake lists in LLVM_INCLUDE_DIRS
    if llvm_dir := os.environ.get("LLVM_DIR"):
        config_file = Path(llvm_dir) / "LLVMConfig.cmake"
        if config_file.is_file():
            match = re.search(r'set\(LLVM_INCLUDE_DIRS "([^"$]+)"\)', config_file.read_text())
            if match:
                return " ".join(f"-I{include_dir}" for include_dir in match.group(1).split(";"))
        # An installed LLVMConfig.cmake refers to ${LLVM_INSTALL_PREFIX}/include instead (LLVM_DIR = <prefix>/lib/cmake/llvm)
        include_dir = Path(llvm_dir).resolve().parent.parent.parent / "include"
        if (include_dir / "llvm-c").is_dir():
            return f"-I{include_dir}"
    if llvm_config := shutil.which("llvm-config"):
        include_dir = subprocess.run([llvm_config, "--includedir"], check=True, capture_output=True, text=True).stdout.strip()
        return f"-I{include_dir}"
    return None


def uses_tpde_backend(build_flags: list[str]) -> bool:
    for i, flag in enumerate(build_flags):
        if flag.lower() == "--backend=tpde":
            return True
        if flag == "--backend" and i + 1 < len(build_flags) and build_flags[i + 1].lower() == "tpde":
            return True
    return False


def std_ships_tpde() -> bool:
    # The compilers derive TPDE_FLAGS from the TPDE libraries in the std, unless TPDE_FLAGS is set (see getStdTPDEFlags in
    # src/util/system-util.spice). 'setup-deps.py --tpde' builds them into the std, like the Linux release packages ship them
    tpde_dir = ROOT_DIR / "std" / "bindings" / "tpde"
    return (tpde_dir / "include" / "tpde-llvm" / "LLVMCompiler.hpp").is_file() and (tpde_dir / "lib" / "libtpde_llvm.a").is_file()


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            digest.update(chunk)
    return digest.hexdigest()


def print_stage_diff(previous: Path, current: Path, max_ranges: int = 10) -> None:
    # Show where two stage executables differ, which hints at the source of the non-determinism (e.g. a few bytes in the
    # file header for a link timestamp vs. many ranges across the code for non-deterministic code generation)
    old, new = previous.read_bytes(), current.read_bytes()
    print(f"\nDifferences between {previous.parent.name} ({len(old)} bytes) and {current.parent.name} ({len(new)} bytes):",
          file=sys.stderr)
    # Only the first ranges are kept for printing, the totals are counted over all of them
    shown_ranges = []
    range_count = 0
    diff_bytes = 0

    def add_range(begin: int, end: int) -> None:
        nonlocal range_count, diff_bytes
        if len(shown_ranges) < max_ranges:
            shown_ranges.append((begin, end))
        range_count += 1
        diff_bytes += end - begin

    common_length = min(len(old), len(new))
    start = None
    for offset in range(common_length):
        if old[offset] != new[offset]:
            if start is None:
                start = offset
        elif start is not None:
            add_range(start, offset)
            start = None
    # The bytes beyond the end of the shorter executable differ as well
    if len(old) != len(new):
        add_range(common_length if start is None else start, max(len(old), len(new)))
    elif start is not None:
        add_range(start, common_length)

    for begin, end in shown_ranges:
        print(f"  0x{begin:08x}-0x{end:08x} ({end - begin} bytes)", file=sys.stderr)
    if range_count > max_ranges:
        print(f"  ... {range_count - max_ranges} more ranges", file=sys.stderr)
    print(f"  {range_count} differing ranges, {diff_bytes} differing bytes in total", file=sys.stderr)


def build_stage(compiler: Path, stage: int, work_dir: Path, build_flags: list[str], timeout: int, verbose: bool) -> Path:
    stage_dir = work_dir / f"stage{stage}"
    shutil.rmtree(stage_dir, ignore_errors=True)
    stage_dir.mkdir(parents=True)
    output = stage_dir / EXE_NAME
    cmd = [str(compiler), "build", *build_flags, "--ignore-cache", "--output", str(output),
           str(ROOT_DIR / "src" / "main.spice")]

    log(f"[Stage {stage}] Building with {compiler} ...")
    start = time.monotonic()
    try:
        result = subprocess.run(cmd, cwd=ROOT_DIR, capture_output=not verbose, text=True, timeout=timeout)
    except subprocess.TimeoutExpired:
        fail(f"[Stage {stage}] Build timed out after {timeout}s: {' '.join(cmd)}")
    # The bootstrap compiler sources emit lots of warnings, so only print the output if something went wrong
    if result.returncode != 0 or not output.is_file():
        if not verbose:
            print(result.stdout, end="")
            print(result.stderr, end="", file=sys.stderr)
        fail(f"[Stage {stage}] Build failed with exit code {result.returncode}: {' '.join(cmd)}")
    log(f"[Stage {stage}] Done in {time.monotonic() - start:.1f}s.")
    return output


def main() -> None:
    parser = argparse.ArgumentParser(description="Bootstrap the self-hosted Spice compiler until it reaches a fixed point.")
    parser.add_argument("--stage0-compiler", "--host-compiler", dest="stage0_compiler", type=Path, default=None,
                        help="Path to the stage0 compiler, that builds stage 1 (default: the released stage0 compiler from "
                             "'fetch-stage0.py' in build/stage0/, else the host compiler, first found in build/, cmake-build-release/, "
                             "cmake-build-debug/)")
    parser.add_argument("--work-dir", type=Path, default=ROOT_DIR / "build" / "bootstrap",
                        help="Directory for the stage executables (default: build/bootstrap)")
    parser.add_argument("--max-iterations", type=int, default=5,
                        help="Maximum number of self-compilations, after stage 1 was built by the stage0 compiler (default: 5)")
    parser.add_argument("--build-flags", default=" ".join(DEFAULT_BUILD_FLAGS),
                        help=f"Flags passed to every 'spice build' invocation (default: '{' '.join(DEFAULT_BUILD_FLAGS)}'). "
                             "With '--backend=tpde', the TPDE libraries are taken from the std (see 'setup-deps.py --tpde'), "
                             "unless TPDE_FLAGS is set")
    parser.add_argument("--stage-timeout", type=int, default=1800,
                        help="Timeout in seconds for building a single stage (default: 1800)")
    parser.add_argument("--output", type=Path, default=None,
                        help="Copy the fixed point compiler executable to this path")
    parser.add_argument("-v", "--verbose", action="store_true", help="Print the compiler output of every stage")
    # argparse treats a value starting with '-' as an option, so glue the value to the option name, to also allow e.g.
    # '--build-flags "--backend=tpde"' and not only '--build-flags="--backend=tpde"'
    argv = sys.argv[1:]
    for i in range(len(argv) - 1):
        if argv[i] == "--build-flags" and argv[i + 1] not in ("-v", "--verbose"):
            argv[i:i + 2] = [f"--build-flags={argv[i + 1]}"]
            break
    args = parser.parse_args(argv)

    if args.max_iterations < 2:
        fail("At least two iterations are required to compare two self-compiled stages")

    stage0_compiler = args.stage0_compiler or find_stage0_compiler() or find_host_compiler()
    if stage0_compiler is None or not stage0_compiler.is_file():
        fail("Stage0 compiler not found. Download it first ('python fetch-stage0.py') or pass --stage0-compiler")
    stage0_compiler = stage0_compiler.resolve()

    # Environment, the stage0 and bootstrap compilers need to compile the bootstrap compiler
    os.environ.setdefault("SPICE_STD_DIR", str(ROOT_DIR / "std"))
    os.environ.setdefault("SPICE_BOOTSTRAP_DIR", str(ROOT_DIR / "src"))
    if "LLVM_LIB_DIR" not in os.environ:
        llvm_lib_dir = find_llvm_lib_dir()
        if llvm_lib_dir is None:
            fail("LLVM library directory not found. Set LLVM_LIB_DIR or LLVM_DIR, or put llvm-config on the PATH")
        os.environ["LLVM_LIB_DIR"] = llvm_lib_dir
    if "LLVM_INCLUDE_DIRS" not in os.environ:
        llvm_include_dirs = find_llvm_include_dirs()
        if llvm_include_dirs is None:
            fail("LLVM include directories not found. Set LLVM_INCLUDE_DIRS (e.g. '-I<dir1> -I<dir2>') or LLVM_DIR, or put "
                 "llvm-config on the PATH")
        os.environ["LLVM_INCLUDE_DIRS"] = llvm_include_dirs

    work_dir = args.work_dir.resolve()
    build_flags = args.build_flags.split()

    # The TPDE backend is used to build every stage from stage 2 on, so the stage compilers must be backed by TPDE as well
    if uses_tpde_backend(build_flags):
        if not sys.platform.startswith("linux"):
            fail("The TPDE backend is only supported on Linux")
        if "-lto" in build_flags:
            fail("The TPDE backend does not support LTO. Remove -lto from --build-flags")
        if not os.environ.get("TPDE_FLAGS") and ("TPDE_FLAGS" in os.environ or not std_ships_tpde()):
            fail("TPDE libraries not found. Build them with 'python setup-deps.py --tpde' or set TPDE_FLAGS "
                 "(e.g. '-I<tpde-include-dir> <libtpde_llvm.a> <libtpde.a> ...')")

    # Stage 1: built by the stage0 compiler
    compiler = build_stage(stage0_compiler, 1, work_dir, build_flags, args.stage_timeout, args.verbose)
    hashes = [sha256(compiler)]
    print(f"  sha256: {hashes[0]}")

    # Stage 2..n: built by the previous stage, until two consecutive self-compiled stages are identical
    for stage in range(2, args.max_iterations + 2):
        compiler = build_stage(compiler, stage, work_dir, build_flags, args.stage_timeout, args.verbose)
        hashes.append(sha256(compiler))
        print(f"  sha256: {hashes[-1]}")
        if stage >= 3 and hashes[-1] == hashes[-2]:
            log(f"Fixed point reached: stage {stage - 1} and stage {stage} are identical.")
            if args.output:
                args.output.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(compiler, args.output)
                log(f"Copied the fixed point compiler to {args.output}")
            return

    print("\nStage hashes:", file=sys.stderr)
    for stage, digest in enumerate(hashes, start=1):
        print(f"  stage{stage}: {digest}", file=sys.stderr)
    print_stage_diff(work_dir / f"stage{args.max_iterations}" / EXE_NAME, compiler)
    fail(f"No fixed point reached within {args.max_iterations} iterations. The stage executables are kept in {work_dir}")


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""Bootstrap the self-hosted Spice compiler until it reaches a fixed point.

Stage 0 is the bootstrap compiler (src-bootstrap/) built by the host compiler (src/). Every following stage n is the
bootstrap compiler built by stage n-1. Stage 0 and stage 1 naturally differ, because they come from different compilers.
From stage 2 on, every stage is built by a compiler that was built from the very same sources, so stage n and stage n-1
have to be bit-identical. The script succeeds as soon as two consecutive stages have the same hash (the fixed point) and
fails if this does not happen within the given number of iterations or if any stage fails to build.
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
# Same flags the test runner uses to build the bootstrap compiler (see test/util/BootstrapUtil.cpp)
DEFAULT_BUILD_FLAGS = ["-O3", "-lto"]


def log(msg: str) -> None:
    print(f"{GREEN}{msg}{NC}", flush=True)


def fail(msg: str) -> None:
    print(f"{RED}{msg}{NC}", file=sys.stderr, flush=True)
    sys.exit(1)


def find_host_compiler() -> Path | None:
    for build_dir in ("build", "cmake-build-release", "cmake-build-debug"):
        candidate = ROOT_DIR / build_dir / "src" / EXE_NAME
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


def find_tpde_flags(host_compiler: Path) -> str | None:
    # Each stage compiler only supports the TPDE backend, if the TPDE libraries are linked into it via TPDE_FLAGS (see
    # std/bindings/tpde). Take them from the CMake build tree of the host compiler (built with -DSPICE_ENABLE_TPDE=ON) and
    # build the flags with the same layout the host derives from a std that ships them (see SystemUtil::getStdTPDEFlags)
    include_dir = ROOT_DIR / "deps" / "tpde" / "tpde-llvm" / "include"
    if not (include_dir / "tpde-llvm" / "LLVMCompiler.hpp").is_file():
        return None
    build_dirs = [host_compiler.parent.parent] + [ROOT_DIR / d for d in ("build", "cmake-build-release", "cmake-build-debug")]
    for build_dir in build_dirs:
        tpde_dir = build_dir / "deps" / "tpde"
        libs = [tpde_dir / "tpde-llvm" / "libtpde_llvm.a", tpde_dir / "tpde" / "libtpde.a",
                tpde_dir / "tpde" / "deps" / "fadec" / "libfadec.a", tpde_dir / "tpde" / "deps" / "disarm" / "libdisarm64.a"]
        if not all(lib.is_file() for lib in libs[:3]):
            continue
        libs = [lib for lib in libs if lib.is_file()]
        # spdlog carries a 'd' suffix in debug builds
        spdlog_dir = tpde_dir / "tpde" / "deps" / "spdlog"
        libs += [lib for lib in (spdlog_dir / "libspdlog.a", spdlog_dir / "libspdlogd.a") if lib.is_file()][:1]
        # The libraries reference each other, so they go into a group, that the linker rescans until all references resolve
        return f"-I{include_dir} -Wl,--start-group {' '.join(str(lib) for lib in libs)} -Wl,--end-group"
    return None


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            digest.update(chunk)
    return digest.hexdigest()


def build_stage(compiler: Path, stage: int, work_dir: Path, build_flags: list[str], timeout: int, verbose: bool) -> Path:
    stage_dir = work_dir / f"stage{stage}"
    shutil.rmtree(stage_dir, ignore_errors=True)
    stage_dir.mkdir(parents=True)
    output = stage_dir / EXE_NAME
    cmd = [str(compiler), "build", *build_flags, "--ignore-cache", "--output", str(output),
           str(ROOT_DIR / "src-bootstrap" / "main.spice")]

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
    parser.add_argument("--host-compiler", type=Path, default=None,
                        help="Path to the host compiler executable (default: first found in build/, cmake-build-release/, "
                             "cmake-build-debug/)")
    parser.add_argument("--work-dir", type=Path, default=ROOT_DIR / "build" / "bootstrap",
                        help="Directory for the stage executables (default: build/bootstrap)")
    parser.add_argument("--max-iterations", type=int, default=5,
                        help="Maximum number of self-compilations, after stage 0 was built by the host (default: 5)")
    parser.add_argument("--build-flags", default=" ".join(DEFAULT_BUILD_FLAGS),
                        help=f"Flags passed to every 'spice build' invocation (default: '{' '.join(DEFAULT_BUILD_FLAGS)}'). "
                             "With '--backend=tpde', the TPDE libraries are taken from the host compiler's build tree, "
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

    host_compiler = args.host_compiler or find_host_compiler()
    if host_compiler is None or not host_compiler.is_file():
        fail("Host compiler not found. Build it first (e.g. 'python build.py') or pass --host-compiler")
    host_compiler = host_compiler.resolve()

    # Environment, the host and bootstrap compilers need to compile the bootstrap compiler
    os.environ.setdefault("SPICE_STD_DIR", str(ROOT_DIR / "std"))
    os.environ.setdefault("SPICE_BOOTSTRAP_DIR", str(ROOT_DIR / "src-bootstrap"))
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

    # The TPDE backend is used to build every stage from stage 1 on, so the stage compilers must be backed by TPDE as well
    if uses_tpde_backend(build_flags):
        if not sys.platform.startswith("linux"):
            fail("The TPDE backend is only supported on Linux")
        if "-lto" in build_flags:
            fail("The TPDE backend does not support LTO. Remove -lto from --build-flags")
        if not os.environ.get("TPDE_FLAGS"):
            tpde_flags = find_tpde_flags(host_compiler)
            if tpde_flags is None:
                fail("TPDE libraries not found. Build the host compiler with -DSPICE_ENABLE_TPDE=ON or set TPDE_FLAGS "
                     "(e.g. '-I<tpde-include-dir> <libtpde_llvm.a> <libtpde.a> ...')")
            os.environ["TPDE_FLAGS"] = tpde_flags

    # Stage 0: built by the host compiler
    compiler = build_stage(host_compiler, 0, work_dir, build_flags, args.stage_timeout, args.verbose)
    hashes = [sha256(compiler)]
    print(f"  sha256: {hashes[0]}")

    # Stage 1..n: built by the previous stage, until two consecutive self-compiled stages are identical
    for stage in range(1, args.max_iterations + 1):
        compiler = build_stage(compiler, stage, work_dir, build_flags, args.stage_timeout, args.verbose)
        hashes.append(sha256(compiler))
        print(f"  sha256: {hashes[-1]}")
        if stage >= 2 and hashes[-1] == hashes[-2]:
            log(f"Fixed point reached: stage {stage - 1} and stage {stage} are identical.")
            if args.output:
                args.output.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(compiler, args.output)
                log(f"Copied the fixed point compiler to {args.output}")
            return

    print("\nStage hashes:", file=sys.stderr)
    for stage, digest in enumerate(hashes):
        print(f"  stage{stage}: {digest}", file=sys.stderr)
    fail(f"No fixed point reached within {args.max_iterations} iterations. The stage executables are kept in {work_dir}")


if __name__ == "__main__":
    main()

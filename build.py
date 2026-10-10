#!/usr/bin/env python3
"""Build the Spice compiler from src/ into build/spice, with the released stage0 compiler (see fetch-stage0.py)."""
import argparse
import os
import subprocess
import sys
from pathlib import Path

sys.dont_write_bytecode = True
import bootstrap  # noqa: E402

ROOT_DIR = bootstrap.ROOT_DIR
BUILD_FLAGS = {
    "Release": ["-O3", "-lto"],
    "Debug": ["-O0", "-g"],
}

parser = argparse.ArgumentParser(description="Build the Spice compiler.")
parser.add_argument(
    "-b", "--build-type",
    choices=list(BUILD_FLAGS),
    default="Release",
    help="Release: optimized with LTO, Debug: unoptimized with debug info (default: Release)",
)
parser.add_argument(
    "--compiler",
    type=Path,
    default=None,
    help="Compiler to build with (default: the stage0 compiler in build/stage0/, downloaded via fetch-stage0.py if missing)",
)
parser.add_argument(
    "-o", "--output",
    type=Path,
    default=ROOT_DIR / "build" / bootstrap.EXE_NAME,
    help=f"Path of the built compiler (default: build/{bootstrap.EXE_NAME})",
)
args = parser.parse_args()

compiler = args.compiler or bootstrap.find_stage0_compiler()
if compiler is None:
    subprocess.run([sys.executable, str(ROOT_DIR / "fetch-stage0.py")], check=True)
    compiler = bootstrap.find_stage0_compiler()
if compiler is None or not compiler.is_file():
    bootstrap.fail("No compiler found to build with. Download the stage0 compiler ('python fetch-stage0.py') or pass --compiler")

# Environment, the compiler needs to build the compiler sources. Always build with the std and compiler sources of this checkout
os.environ.setdefault("LLVM_DIR", str(ROOT_DIR / "llvm" / "build-release" / "lib" / "cmake" / "llvm"))
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
        bootstrap.fail("LLVM include directories not found. Set LLVM_INCLUDE_DIRS (e.g. '-I<dir1> -I<dir2>') or LLVM_DIR, or put "
                       "llvm-config on the PATH")
    os.environ["LLVM_INCLUDE_DIRS"] = llvm_include_dirs

output = args.output.resolve()
output.parent.mkdir(parents=True, exist_ok=True)
bootstrap.log(f"Building {output} with {compiler} ...")
# Bypass the compilation cache, which can go stale when only an imported file changes (see issue #1417)
cmd = [str(compiler), "build", *BUILD_FLAGS[args.build_type], "--ignore-cache", "--output", str(output),
       str(ROOT_DIR / "src" / "main.spice")]
if subprocess.run(cmd, cwd=ROOT_DIR).returncode != 0:
    bootstrap.fail(f"Building the compiler failed: {' '.join(cmd)}")
bootstrap.log("done.")

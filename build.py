#!/usr/bin/env python3
"""Build the Spice compiler from src/ (or another source root) into build/ with the released stage0 compiler (fetch-stage0.py)"""
import argparse
import os
import subprocess
import sys
import tomllib
from pathlib import Path

sys.dont_write_bytecode = True
import bootstrap  # noqa: E402

ROOT_DIR = bootstrap.ROOT_DIR
BUILD_FLAGS = bootstrap.BUILD_TYPE_FLAGS

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
    "--sources",
    type=Path,
    default=Path("src"),
    help="Source root of the compiler to build, relative to the repo root, e.g. src-next (default: src)",
)
parser.add_argument(
    "--output-name",
    default="spice",
    help="Name of the built compiler in build/, without the executable suffix (default: spice)",
)
parser.add_argument(
    "-o", "--output",
    type=Path,
    default=None,
    help="Path of the built compiler (default: build/<output-name>[.exe])",
)
args = parser.parse_args()

source_dir = (ROOT_DIR / args.sources).resolve()
if not (source_dir / "main.spice").is_file():
    bootstrap.fail(f"No main.spice found in the source root {source_dir}")
exe_suffix = ".exe" if sys.platform == "win32" else ""
args.output = args.output or ROOT_DIR / "build" / f"{args.output_name}{exe_suffix}"

compiler = args.compiler or bootstrap.find_stage0_compiler()
if compiler is None:
    subprocess.run([sys.executable, str(ROOT_DIR / "fetch-stage0.py")], check=True)
    compiler = bootstrap.find_stage0_compiler()
if compiler is None or not compiler.is_file():
    bootstrap.fail("No compiler found to build with. Download the stage0 compiler ('python fetch-stage0.py') or pass --compiler")

# The packages, that the compiler sources build on (<sources>/spice.lock). Source roots without a lock file have none
if (source_dir / "spice.lock").is_file():
    bootstrap.install_packages(source_dir=source_dir)


def needs_llvm() -> bool:
    """Only source roots, that depend on the llvm package, need the LLVM libraries and headers"""
    try:
        manifest = tomllib.loads((source_dir / "spice.toml").read_text(encoding="utf-8"))
    except (OSError, tomllib.TOMLDecodeError):
        return True
    return "llvm" in manifest.get("dependencies", {})


# Environment, the compiler needs to build the compiler sources. Always build with the std and compiler sources of this checkout
os.environ.setdefault("LLVM_DIR", str(ROOT_DIR / "llvm" / "build-release" / "lib" / "cmake" / "llvm"))
os.environ["SPICE_STD_DIR"] = str(ROOT_DIR / "std")
llvm_needed = needs_llvm()
if llvm_needed and "LLVM_LIB_DIR" not in os.environ:
    llvm_lib_dir = bootstrap.find_llvm_lib_dir()
    if llvm_lib_dir is None:
        bootstrap.fail("LLVM library directory not found. Set LLVM_LIB_DIR or LLVM_DIR, or put llvm-config on the PATH")
    os.environ["LLVM_LIB_DIR"] = llvm_lib_dir
if llvm_needed and "LLVM_INCLUDE_DIRS" not in os.environ:
    llvm_include_dirs = bootstrap.find_llvm_include_dirs()
    if llvm_include_dirs is None:
        bootstrap.fail("LLVM include directories not found. Set LLVM_INCLUDE_DIRS (e.g. '-I<dir1> -I<dir2>') or LLVM_DIR, or put "
                       "llvm-config on the PATH")
    os.environ["LLVM_INCLUDE_DIRS"] = llvm_include_dirs

output = args.output.resolve()
output.parent.mkdir(parents=True, exist_ok=True)
bootstrap.log(f"Building {output} with {compiler} ...")
cmd = [str(compiler), "build", *BUILD_FLAGS[args.build_type], "--output", str(output), str(source_dir / "main.spice")]
if subprocess.run(cmd, cwd=ROOT_DIR).returncode != 0:
    bootstrap.fail(f"Building the compiler failed: {' '.join(cmd)}")
bootstrap.log("done.")

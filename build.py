#!/usr/bin/env python3
"""Build the Spice compiler and test runner into the build/ directory."""
import argparse
import os
import subprocess
import sys
from pathlib import Path

parser = argparse.ArgumentParser(description="Build the Spice compiler and test runner.")
parser.add_argument(
    "-b", "--build-type",
    choices=["Release", "Debug", "DebWithRelInfo"],
    default="Release",
    help="CMake build type (default: Release)",
)
args = parser.parse_args()

build_dir = Path("build")
build_dir.mkdir(exist_ok=True)

os.environ.setdefault("LLVM_DIR", str(Path("llvm/build-release/lib/cmake/llvm").resolve()))

subprocess.run(
    ["cmake", f"-DCMAKE_BUILD_TYPE={args.build_type}", "-GNinja", ".."],
    cwd=build_dir,
    check=True,
)
subprocess.run(
    ["cmake", "--build", ".", "--target", "spice", "spicetest"],
    cwd=build_dir,
    check=True,
)

spice_src = build_dir / "src" / ("spice.exe" if sys.platform == "win32" else "spice")
spice_link = build_dir / ("spice.exe" if sys.platform == "win32" else "spice")
spicetest_src = build_dir / "test" / ("spicetest.exe" if sys.platform == "win32" else "spicetest")
spicetest_link = build_dir / ("spicetest.exe" if sys.platform == "win32" else "spicetest")

for link, target in [(spice_link, spice_src), (spicetest_link, spicetest_src)]:
    if not link.exists():
        link.symlink_to(target.relative_to(build_dir))

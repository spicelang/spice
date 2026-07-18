#!/usr/bin/env python3
"""Full first-time dev environment setup: deps, LLVM, third-party libs, initial build."""
import shutil
import subprocess
import sys
from pathlib import Path

GREEN = "\033[0;92m"
NC = "\033[0m"

def log(msg: str) -> None:
    print(f"{GREEN}{msg}{NC}", flush=True)

def run(*args, **kwargs) -> None:
    subprocess.run(list(args), check=True, **kwargs)

if sys.platform == "win32":
    # Step 1: check required tools
    log("[Step 1] Checking installations ...")
    missing = [t for t in ("g++", "lld", "ninja", "cmake") if not shutil.which(t)]
    if missing:
        print(f"Missing tools: {', '.join(missing)}. Please install them and re-run.", file=sys.stderr)
        sys.exit(1)
    log("done.")
else:
    # Step 1: install Linux packages
    log("[Step 1] Installing dependencies via Linux packages (could take a while) ...")
    run("sudo", "add-apt-repository", "-y", "ppa:ubuntu-toolchain-r/test")
    run("sudo", "apt", "update")
    run(
        "sudo", "apt-get", "install", "-y",
        "cmake", "make", "ninja-build", "valgrind", "ccache",
        "uuid-dev", "pkg-config", "openjdk-11-jre-headless", "clang", "lld",
    )
    log("done.")

# Step 2: clone LLVM
log("[Step 2] Cloning LLVM (could take a while) ...")
run("git", "clone", "--depth", "1", "--branch", "llvmorg-23.1.0-rc1",
    "https://github.com/llvm/llvm-project", "llvm")
log("done.")

# Step 3: build LLVM
log("[Step 3] Building LLVM (could take a whole while, please be patient) ...")
llvm_build = Path("llvm/build-release")
llvm_build.mkdir(parents=True, exist_ok=True)
run(
    "cmake",
    "-DCMAKE_BUILD_TYPE=RelWithDebInfo",
    "-DCMAKE_C_COMPILER_LAUNCHER=ccache",
    "-DCMAKE_CXX_COMPILER_LAUNCHER=ccache",
    "-DCMAKE_CXX_FLAGS_RELWITHDEBINFO=-O3",
    "-DLLVM_USE_LINKER=lld",
    "-DLLVM_ENABLE_PROJECTS=clang",
    "-DLLVM_ENABLE_RUNTIMES=libcxx;libcxxabi;compiler-rt;libunwind",
    "-DLLVM_TARGETS_TO_BUILD=AArch64;X86;WebAssembly",
    "-DCOMPILER_RT_BUILD_SANITIZERS=ON",
    "-DLLVM_INCLUDE_TESTS=OFF",
    "-DLLVM_INCLUDE_EXAMPLES=OFF",
    "-DLLVM_INCLUDE_BENCHMARKS=OFF",
    "-GNinja",
    "../llvm",
    cwd=llvm_build,
)
run("cmake", "--build", ".", cwd=llvm_build)
log("done.")

# Step 4: download third-party libs
log("[Step 4] Downloading third-party libraries ...")
run(sys.executable, "setup-deps.py")
log("done.")

# Step 5: build Spice
log("[Step 5] Building Spice ...")
run(sys.executable, "build.py")
log("done.")

log("The setup is done. Have fun coding!")
log("Whenever you need to build Spice again, use: python build.py")

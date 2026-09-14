#!/bin/bash
# SessionStart hook for Claude Code on the web.
# Installs the toolchain and builds LLVM so the spicetest/spice CMake targets
# can be configured immediately. Mirrors the Linux job in
# .github/workflows/ci-cpp.yml. Idempotent: safe to re-run, and cheap on a
# cached container since it skips work that is already done.
set -euo pipefail

if [ "${CLAUDE_CODE_REMOTE:-}" != "true" ]; then
  exit 0
fi

LLVM_VERSION="llvmorg-23.1.1"
GCC_VERSION="16"
LLVM_SRC_DIR="$CLAUDE_PROJECT_DIR/llvm"
LLVM_BUILD_DIR="$LLVM_SRC_DIR/build"

# 1. System dependencies (matches "Setup Dependencies" step in ci-cpp.yml)
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install -y \
  ccache ninja-build graphviz uuid-dev libcurl4-openssl-dev libsqlite3-dev \
  libssl-dev "gcc-${GCC_VERSION}" "g++-${GCC_VERSION}" clang
sudo update-alternatives --install /usr/bin/gcc gcc "/usr/bin/gcc-${GCC_VERSION}" "1${GCC_VERSION}"
sudo update-alternatives --install /usr/bin/g++ g++ "/usr/bin/g++-${GCC_VERSION}" "1${GCC_VERSION}"
sudo update-alternatives --install /usr/bin/gcov gcov "/usr/bin/gcov-${GCC_VERSION}" "1${GCC_VERSION}"
mkdir -p ~/.cache/ccache

# 2. Clone LLVM (matches "Clone LLVM" step; skipped once /llvm exists)
if [ ! -d "$LLVM_SRC_DIR" ]; then
  git clone --depth 1 --branch "$LLVM_VERSION" https://github.com/llvm/llvm-project "$LLVM_SRC_DIR"
fi

# 3. Build LLVM (matches "Build LLVM" step; skipped once already built, so
#    the cached container never pays for this again)
if [ ! -f "$LLVM_BUILD_DIR/lib/cmake/llvm/LLVMConfig.cmake" ]; then
  cmake -GNinja -S "$LLVM_SRC_DIR/llvm" -B "$LLVM_BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_COMPILER_LAUNCHER=ccache \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    -DCMAKE_C_COMPILER=gcc \
    -DCMAKE_CXX_COMPILER=g++ \
    -DCMAKE_CXX_FLAGS_RELEASE="-O3" \
    -DLLVM_TARGETS_TO_BUILD="AArch64;X86;WebAssembly" \
    -DLLVM_BUILD_TOOLS=OFF \
    -DLLVM_INCLUDE_TESTS=OFF \
    -DLLVM_INCLUDE_EXAMPLES=OFF \
    -DLLVM_INCLUDE_BENCHMARKS=OFF
  cmake --build "$LLVM_BUILD_DIR"
fi

# 4. Persist LLVM_DIR for the rest of the session (matches the LLVM_DIR env
#    var used by the "Build test target" step in ci-cpp.yml)
echo "export LLVM_DIR=\"$LLVM_BUILD_DIR/lib/cmake/llvm\"" >> "$CLAUDE_ENV_FILE"

# 5. Third-party deps (matches "Download Libs" step)
python3 "$CLAUDE_PROJECT_DIR/setup-deps.py"

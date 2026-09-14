#!/bin/bash
# SessionStart hook for Claude Code on the web.
# Installs the toolchain and gets LLVM 23 ready so the spicetest/spice CMake
# targets can be configured immediately. Mirrors the Linux job in
# .github/workflows/ci-cpp.yml. Idempotent: safe to re-run, and cheap on a
# cached container since it skips work that is already done.
set -euo pipefail

if [ "${CLAUDE_CODE_REMOTE:-}" != "true" ]; then
  exit 0
fi

LLVM_VERSION="llvmorg-23.1.1"
LLVM_MAJOR="${LLVM_VERSION#llvmorg-}"
LLVM_MAJOR="${LLVM_MAJOR%%.*}"
GCC_VERSION="16"
LLVM_PKG_CMAKE_DIR="/usr/lib/llvm-${LLVM_MAJOR}/lib/cmake/llvm"
LLVM_SRC_DIR="$CLAUDE_PROJECT_DIR/llvm"
LLVM_BUILD_DIR="$LLVM_SRC_DIR/build"

# 1. System dependencies (matches "Setup Dependencies" step in ci-cpp.yml,
#    plus a few tools llvm.sh itself needs to add the apt.llvm.org repo).
#    software-properties-common is installed first since it provides
#    add-apt-repository itself, which a fresh image may not have yet.
sudo apt-get update
sudo apt-get install -y software-properties-common
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install -y \
  ccache ninja-build graphviz uuid-dev libcurl4-openssl-dev libsqlite3-dev \
  libssl-dev "gcc-${GCC_VERSION}" "g++-${GCC_VERSION}" clang \
  wget gnupg lsb-release
sudo update-alternatives --install /usr/bin/gcc gcc "/usr/bin/gcc-${GCC_VERSION}" "1${GCC_VERSION}"
sudo update-alternatives --install /usr/bin/g++ g++ "/usr/bin/g++-${GCC_VERSION}" "1${GCC_VERSION}"
sudo update-alternatives --install /usr/bin/gcov gcov "/usr/bin/gcov-${GCC_VERSION}" "1${GCC_VERSION}"
mkdir -p ~/.cache/ccache

# 2. Fast path: install prebuilt LLVM 23 from apt.llvm.org instead of
#    building it. Skipped once already installed. Falls through to a
#    from-source build (matching CI) if the package install fails, e.g.
#    because apt.llvm.org hasn't picked up this major/patch version yet or
#    the environment has no outbound network access to it.
if [ ! -f "$LLVM_PKG_CMAKE_DIR/LLVMConfig.cmake" ]; then
  set +e
  wget -O /tmp/llvm.sh https://apt.llvm.org/llvm.sh \
    && chmod +x /tmp/llvm.sh \
    && sudo /tmp/llvm.sh "$LLVM_MAJOR"
  llvm_pkg_status=$?
  set -e
  rm -f /tmp/llvm.sh
else
  llvm_pkg_status=0
fi

if [ "$llvm_pkg_status" -eq 0 ] && [ -f "$LLVM_PKG_CMAKE_DIR/LLVMConfig.cmake" ]; then
  LLVM_DIR="$LLVM_PKG_CMAKE_DIR"
else
  echo "apt.llvm.org install of LLVM ${LLVM_MAJOR} unavailable, building from source instead" >&2

  # 2b. Clone LLVM (matches "Clone LLVM" step; skipped once /llvm already
  #     holds this exact version). A stale checkout from a previous
  #     LLVM_VERSION is wiped (build dir included, since it lives under
  #     LLVM_SRC_DIR) so it can't silently be reused.
  LLVM_VERSION_MARKER="$LLVM_SRC_DIR/.claude-llvm-version"
  if [ -d "$LLVM_SRC_DIR" ] && [ "$(cat "$LLVM_VERSION_MARKER" 2>/dev/null)" != "$LLVM_VERSION" ]; then
    echo "Cached LLVM checkout does not match LLVM_VERSION=$LLVM_VERSION, rebuilding" >&2
    rm -rf "$LLVM_SRC_DIR"
  fi
  if [ ! -d "$LLVM_SRC_DIR" ]; then
    git clone --depth 1 --branch "$LLVM_VERSION" https://github.com/llvm/llvm-project "$LLVM_SRC_DIR"
    echo "$LLVM_VERSION" > "$LLVM_VERSION_MARKER"
  fi

  # 2c. Build LLVM (matches "Build LLVM" step; skipped once already built, so
  #     the cached container never pays for this again)
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

  LLVM_DIR="$LLVM_BUILD_DIR/lib/cmake/llvm"
fi

# 3. Persist LLVM_DIR for the rest of the session (matches the LLVM_DIR env
#    var used by the "Build test target" step in ci-cpp.yml)
echo "export LLVM_DIR=\"$LLVM_DIR\"" >> "$CLAUDE_ENV_FILE"

# 4. Third-party deps (matches "Download Libs" step)
python3 "$CLAUDE_PROJECT_DIR/setup-deps.py"

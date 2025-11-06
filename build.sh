#!/bin/sh
set -eu

# Prepare
if [ ! -d build ]; then
    mkdir -p build
fi
export LLVM_DIR=./llvm/build-release/lib/cmake/llvm

# Build
(
  cd ./build || exit
  cmake -DCMAKE_BUILD_TYPE=Release -GNinja ..
  cmake --build . --target spice spicetest
  mv ./src/spice spice
  mv ./test/spicetest spicetest
)
#!/bin/sh
set -eu

# Prepare
if [ ! -d bin ]; then
    mkdir -p bin
fi
export LLVM_DIR=./llvm/build-release/lib/cmake/llvm

# Build
(
  cd ./bin || exit
  cmake -DCMAKE_BUILD_TYPE=Release -GNinja -DCMAKE_CXX_FLAGS_RELEASE="-O2" ..
  cmake --build . --target spice spicetest
  mv ./src/spice spice
  mv ./test/spicetest spicetest
)
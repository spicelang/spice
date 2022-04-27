#!/bin/sh
mkdir bin

# Check for LLVM_DIR env var
export LLVM_DIR=./llvm/build-release/lib/cmake/llvm

# Build
(
  cd ./bin || exit
  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -GNinja -DCMAKE_CXX_FLAGS_RELEASE="-O2" ..
  cmake --build . --target Spice_run
  mv ./src/Spice_run spice
)
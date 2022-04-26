#!/bin/sh
mkdir bin

(
  cd ./bin || exit
  cmake -DCMAKE_BUILD_TYPE=Release -GNinja -DCMAKE_CXX_FLAGS_RELEASE="-O2" ..
  cmake --build . --target Spice_run
  mv ./src/Spice_run spice
)
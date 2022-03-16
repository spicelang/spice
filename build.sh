#!/bin/sh
mkdir bin

cd ./cli
go build -o ../bin/spice .

cd ../bin
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -GNinja -DCMAKE_CXX_FLAGS_RELEASE="-O2" ../compiler
cmake --build . --target Spice_run
mv ./src/Spice_run spicec

cd ..
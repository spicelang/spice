@echo off
mkdir bin 2> NUL

cd cli
go build -o ../bin/spice.exe .

cd ../bin
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -GNinja -DCMAKE_CXX_FLAGS_RELEASE="-O2" ../compiler
cmake --build . --target Spice_run
move src\Spice_run.exe spicec.exe

cd ..
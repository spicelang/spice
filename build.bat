@echo off
mkdir bin 2> NUL
cd bin

cmake -DCMAKE_BUILD_TYPE=Release -G "CodeBlocks - MinGW Makefiles" ../compiler
cmake --build . --target Spice_run -- -j 6

move src\Spice_run.exe spicec.exe

cd ..
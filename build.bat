@echo off
mkdir bin 2> NUL
cd bin

cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ../compiler
mingw32-make

move src\Spice_run.exe spicec.exe

cd ..
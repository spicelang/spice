@echo off
mkdir bin 2> NUL

cd ../bin
cmake -DCMAKE_BUILD_TYPE=Release -GNinja -DCMAKE_CXX_FLAGS_RELEASE="-O2" ..
cmake --build . --target Spice_run
move src\Spice_run.exe spice.exe

cd ..
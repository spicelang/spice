@echo off
mkdir bin 2> NUL

REM - Check for LLVM_DIR env var
set LLVM_DIR=".\llvm\build-release\lib\cmake\llvm"

REM - Build
pushd bin
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -GNinja -DCMAKE_CXX_FLAGS_RELEASE="-O2" ..
cmake --build . --target Spice_run
move src\Spice_run.exe spice.exe
popd
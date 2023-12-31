@echo off

:: Check for LLVM_DIR env var
set LLVM_DIR=".\llvm\build-release\lib\cmake\llvm"

:: Prepare
if not exist bin (
    mkdir bin
)
pushd bin

:: Build
cmake -DCMAKE_BUILD_TYPE=Release -GNinja -DCMAKE_CXX_FLAGS_RELEASE="-O2" ..
cmake --build . --target spice spicetest

:: Cleanup
move src\spice.exe spice.exe
move test\spicetest.exe spicetest.exe
popd
@echo off

:: Check for LLVM_DIR env var
set LLVM_DIR=".\llvm\build-release\lib\cmake\llvm"

:: Prepare
if not exist build (
    mkdir build
)
pushd build

:: Build
cmake -DCMAKE_BUILD_TYPE=Release -GNinja ..
cmake --build . --target spice spicetest

:: Cleanup
mklink /spice.exe src\spice.exe
mklink /spicetest.exe test\spicetest.exe
popd
@echo off

echo [Step 1] Checking installations ...

REM - Check if MinGW is installed
where g++ > nul 2>&1
if errorlevel 1 (
    echo MinGW-w64 was not found. Please install the latest version from https://winlibs.com
    goto end
)

REM - Check if Ninja is installed
where ninja > nul 2>&1
if errorlevel 1 (
    echo Ninja was not found. Please install the latest version from https://github.com/ninja-build/ninja/releases
    goto end
)

REM - Check if CMake is installed
where cmake > nul 2>&1
if errorlevel 1 (
    echo CMake was not found. Please install the latest version from https://cmake.org/download
    goto end
)

echo done.

REM - Clone LLVM
echo [Step 2] Cloning LLVM (Could take a while) ...
git clone --depth 1 --branch llvmorg-14.0.3 https://github.com/llvm/llvm-project llvm
echo done.

REM - Build LLVM
echo [Step 3] Building LLVM (Could take a whole while, please be patient) ...
mkdir .\llvm\build-release
pushd .\llvm\build-release
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_CXX_FLAGS_RELEASE="-O2" -GNinja ../llvm
cmake --build .
popd
echo done.

REM - Download third-party libs
echo [Step 4] Downloading third-party libraries ...
cmd /c .\setup-libs.bat > nul 2>&1
echo done.

REM - Build in dev context
echo [Step 5] Building Spice ...
cmd /c .\build.bat
echo done.

REM - End message
echo The setup is done. Have fun coding!
echo Whenever you need to build Spice again, use the ./build.sh script.

:end
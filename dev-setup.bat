@echo off

echo [Step 1] Checking installations ...

:: Check if MinGW is installed
where g++ > nul 2>&1
if errorlevel 1 (
    echo MinGW-w64 was not found. Please install the latest version from https://winlibs.com
    goto end
)

:: Check if lld is installed
where lld > nul 2>&1
if errorlevel 1 (
    echo LLD linker was not found. Please install the latest version from https://winlibs.com
    goto end
)

:: Check if Ninja is installed
where ninja > nul 2>&1
if errorlevel 1 (
    echo Ninja was not found. Please install the latest version from https://github.com/ninja-build/ninja/releases
    goto end
)

:: Check if CMake is installed
where cmake > nul 2>&1
if errorlevel 1 (
    echo CMake was not found. Please install the latest version from https://cmake.org/download
    goto end
)

echo done.

:: Clone LLVM
echo [Step 2] Cloning LLVM (Could take a while) ...
git clone --depth 1 --branch llvmorg-18.1.5 https://github.com/llvm/llvm-project llvm
echo done.

:: Build LLVM
echo [Step 3] Building LLVM (Could take a whole while, please be patient) ...
mkdir .\llvm\build-release
pushd .\llvm\build-release
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_FLAGS_RELWITHDEBINFO="-O2 -fuse-ld=lld" -DLLVM_ENABLE_RTTI=ON -GNinja ../llvm
cmake --build .
popd
echo done.

:: Download third-party libs
echo [Step 4] Downloading third-party libraries ...
cmd /c .\setup-libs.bat > nul 2>&1
echo done.

:: Build in dev context
echo [Step 5] Building Spice ...
cmd /c .\build.bat
echo done.

:: End message
echo The setup is done. Have fun coding!
echo Whenever you need to build Spice again, use the ./build.sh script.

:end
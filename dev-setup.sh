#!/bin/sh

colored_echo(){
    RED="\033[0;31m"
    GREEN="\033[0;32m"
    YELLOW="\033[1;33m"
    NC="\033[0m" # No Color
    printf "${(P)1}${2} ${NC}\n"
}

# Install Linux dependencies
colored_echo "GREEN" "[Step 1] Installing dependencies via Linux packages (Could take a while) ... "
sudo apt-get install cmake make ninja-build uuid-dev opendjk-11-jre-headless
colored_echo "GREEN" "done."

# Clone LLVM
colored_echo "GREEN" "[Step 2] Cloning LLVM (Could take a while) ... "
git clone --depth 1 --branch llvmorg-14.0.1 https://github.com/llvm/llvm-project llvm
colored_echo "GREEN" "done."

# Build LLVM
colored_echo "GREEN" "[Step 3] Building LLVM (Could take a whole while, please be patient) ... "
mkdir ./llvm/build-release
(
  cd ./llvm/build-release || exit
  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="-O2" -GNinja ../llvm
  cmake --build .
)
colored_echo "GREEN" "done."

# Download third-party libs
colored_echo "GREEN" "[Step 4] Downloading third-party libraries ... "
sh ./setup-libs.sh > /dev/null 2>&1
colored_echo "done."

# Build in dev context
colored_echo "GREEN" "[Step 5] Building Spice ... "
sh ./build.sh > /dev/null 2>&1
colored_echo "GREEN" "done."

# End message
colored_echo "GREEN" "The setup is done. Have fun coding!"
colored_echo "GREEN" "Whenever you need to build Spice again, use the ./build.sh script."
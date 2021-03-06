#!/bin/sh

colored_echo() {
    GREEN='\e[0;92m' # Green color
    NC='\e[0m' # No Color
    echo "${GREEN}${1}${NC}"
}

# Install Linux dependencies
colored_echo "[Step 1] Installing dependencies via Linux packages (Could take a while) ... "
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt update -y
sudo apt-get install -y cmake make ninja-build uuid-dev openjdk-11-jre-headless gcc-11 g++-11
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 110 --slave /usr/bin/g++ g++ /usr/bin/g++-11 --slave /usr/bin/gcov gcov /usr/bin/gcov-11 --slave /usr/bin/gcc-ar gcc-ar /usr/bin/gcc-ar-11 --slave /usr/bin/gcc-ranlib gcc-ranlib /usr/bin/gcc-ranlib-11
colored_echo "done."

# Clone LLVM
colored_echo "[Step 2] Cloning LLVM (Could take a while) ... "
git clone --depth 1 --branch llvmorg-14.0.6 https://github.com/llvm/llvm-project llvm
colored_echo "done."

# Build LLVM
colored_echo "[Step 3] Building LLVM (Could take a whole while, please be patient) ... "
mkdir ./llvm/build-release 2>/dev/null
(
  cd ./llvm/build-release || exit
  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_CXX_FLAGS_RELEASE="-O2" -GNinja ../llvm
  cmake --build .
)
colored_echo "done."

# Download third-party libs
colored_echo "[Step 4] Downloading third-party libraries ... "
sh ./setup-libs.sh > /dev/null 2>&1
colored_echo "done."

# Build in dev context
colored_echo "[Step 5] Building Spice ... "
sh ./build.sh > /dev/null 2>&1
colored_echo "done."

# End message
colored_echo "The setup is done. Have fun coding!"
colored_echo "Whenever you need to build Spice again, use the ./build.sh script."
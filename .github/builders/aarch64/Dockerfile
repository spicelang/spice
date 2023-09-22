# This image can be used to build Spice for AArch64.
# This is relevant for releasing new versions, that have to support this platform.

FROM ubuntu:latest
WORKDIR /src

# Build arguments
ARG VERSION=dev
ARG BUILT_BY=ghactions

# Install dependencies
RUN apt-get update &&  \
    apt-get install -y software-properties-common && \
    add-apt-repository ppa:ubuntu-toolchain-r/ppa && \
    apt install gcc-13 g++-13 build-essential git cmake ninja-build curl && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 110 --slave /usr/bin/g++ g++ /usr/bin/g++-13 --slave /usr/bin/gcov gcov /usr/bin/gcov-13 --slave /usr/bin/gcc-ar gcc-ar /usr/bin/gcc-ar-13 --slave /usr/bin/gcc-ranlib gcc-ranlib /usr/bin/gcc-ranlib-13 && \
    apt-get rm

# Build LLVM
RUN git clone --depth 1 --branch llvmorg-17.0.1 https://github.com/llvm/llvm-project /llvm && \
    mkdir /llvm/build && \
    cd /llvm/build && \
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_CXX_FLAGS_RELEASE="-O2" -DLLVM_ENABLE_RTTI=ON -GNinja ../llvm && \
    cmake --build .

# Use the start command to build Spice
CMD ["/bin/bash", "-c", "cmake -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DSPICE_LINK_STATIC=ON -DSPICE_LTO=ON -DCMAKE_CXX_FLAGS_RELEASE=\"-O2\" -DSPICE_VERSION=\"${VERSION}\" -DSPICE_BUILT_BY=\"${BUILT_BY}\" .. && cmake --build ."]
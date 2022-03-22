<p align="center">
  <img alt="Spice Logo" src="./docs/docs/static/avatar.png" height="220" />
  <h3 align="center">Spice Programming Language</h3>
  <p align="center">Compiler and standard library for the Spice programming language.</p>
  <p align="center">
    <a target="_blank" href="https://github.com/spicelang/spice/releases/latest"><img src="https://img.shields.io/github/v/release/spicelang/spice?include_prereleases"></a>
    <a target="_blank" href="https://hub.docker.com/r/chillibits/spice"><img src="https://img.shields.io/docker/pulls/chillibits/spice"></a>
	<a target="_blank" href="./.github/workflows/ci-cpp.yml"><img src="https://github.com/spicelang/spice/actions/workflows/ci-cpp.yml/badge.svg"></a>
	<a target="_blank" href="./.github/workflows/codeql-analysis.yml"><img src="https://github.com/spicelang/spice/actions/workflows/codeql-analysis.yml/badge.svg"></a>
    <a target="_blank" href="https://makeapullrequest.com"><img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg"></a>
    <a target="_blank" href="./LICENSE.md"><img src="https://img.shields.io/github/license/spicelang/spice"></a>
  </p>
</p>

---

This is the home repo of the Spice programming language. Spice is a compiled language which sets a focus on performance and practicality. It is considered as a systems language, which means it is neither suitable for coding user interfaces nor any interface with the user, except cli tools. Spice supports cross-compilation to all target platforms, listed below.

If you like Spice, please consider a GitHub star! ⭐

|:warning: | The compiler of the Spice language is still in alpha stages and under development. Please note that code may behave differently in the future or even break due to the volatility of the compiler.  |
|----------|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

## Documentation
Please visit the documentation at [spicelang.com](https://www.spicelang.com).

## Setup guide for contributors
#### Clone the project
```sh
git clone https://github.com/spicelang/spice.git
cd spice
```

#### Download third-party libraries
There is a batch / sh script to help you with that. Use this command to run it:

**Linux:** <br>
```sh
./setup-libs.sh
```

**Windows:** <br>
```bat
.\setup-libs.bat
```

#### Build LLVM
Spice needs the LLVM 14.0.0 sources to compile successfully. Before you clone LLVM, you should navigate outside of the Spice repository.

**Clone:** <br>
```sh
git clone --depth 1 --branch llvmorg-14.0.0-rc4 https://github.com/llvm/llvm-project.git
cd llvm-project
```

**Compile:** <br>
```sh
mkdir build-release
cd build-release
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_CXX_FLAGS_RELEASE="-O2" -GNinja ../llvm
cmake --build .
```

**Set env:** <br>
```sh
export LLVM_DIR=<your-llvm-root-dir>/build-release/lib/cmake/llvm
```

#### Build Spice
To build Spice there is also a bat/sh file to help you with that. Use the following command to run it

**Linux:** <br>
```sh
./build.sh
```

**Windows:** <br>
```bat
.\build.bat
```

You can find the build output in the `./bin` subdirectory.

## Grammar
The Spice grammar can be found [here](./src/grammar/Spice.g4) as a ANTLR grammar configuration file.

## Available target platforms

- `aarch64`
- `amdgpu`
- `armv5`, `armv6`, `armv7`
- `avr`
- `bpf`
- `hexagon`
- `lanai`
- `mips`
- `msp430`
- `nvptx`
- `powerpc`
- `riscv`
- `sparc`
- `systemz`
- `webassembly`
- `x86`
- `x86_64`
- `xcore`

## Contribute to the project
If you want to contribute to this project, please ensure you comply with the [contribution guidelines](./CONTRIBUTING.md).

© ChilliBits 2021-2022
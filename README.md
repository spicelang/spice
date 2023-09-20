
<p align="center">
  <img alt="Spice Logo" src="./docs/docs/static/logo_large_white_background_blur.png" height="230" />
  <h3 align="center">Spice Programming Language</h3>
  <p align="center">Compiler and standard library for the Spice programming language.</p>
  <p align="center">
    <a target="_blank" href="https://github.com/spicelang/spice/releases/latest"><img src="https://img.shields.io/github/v/release/spicelang/spice?include_prereleases"></a>
    <a target="_blank" href="https://hub.docker.com/r/chillibits/spice"><img src="https://img.shields.io/docker/pulls/chillibits/spice"></a>
	<a target="_blank" href="https://github.com/spicelang/spice/actions/workflows/ci-cpp.yml"><img src="https://github.com/spicelang/spice/actions/workflows/ci-cpp.yml/badge.svg"></a>
	<a target="_blank" href="https://github.com/spicelang/spice/actions/workflows/codeql-analysis.yml"><img src="https://github.com/spicelang/spice/actions/workflows/codeql-analysis.yml/badge.svg"></a>
    <a target="_blank" href="https://makeapullrequest.com"><img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg"></a>
    <a target="_blank" href="./LICENSE.md"><img src="https://img.shields.io/github/license/spicelang/spice"></a>
  </p>
</p>

---

This is the home repo of the Spice programming language.
Spice is a compiled language which sets a focus on performance and practicality. It is considered as a systems language, which
means it is especially useful to write cli tools or device drivers.
Spice supports cross-compilation to all target platforms, listed [below](#available-target-platforms).

If you like Spice, please consider a GitHub star! ⭐

| :warning: | The compiler of the Spice language is still in alpha stages and under development. Please note that code may behave differently in the future or even break due to the volatility of the compiler. |
|-----------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

## Documentation
Please visit the documentation at [spicelang.com](https://www.spicelang.com). <br>
We also have a Spice playground where you can try things: [play.spicelang.com](https://play.spicelang.com) <br>
Code coverage can be found at [coverage.spicelang.com](http://coverage.spicelang.com).

## Supported host systems & file downloads
There are downloadable packages available for all supported platforms:

| **Platform**                | **amd64**                                                                                         | **arm64**                                                                                         |
|-----------------------------|---------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------|
| **Alpine (apk)**            | [download](https://github.com/spicelang/spice/releases/latest/download/spice_amd64.apk)           | [download](https://github.com/spicelang/spice/releases/latest/download/spice_arm64.apk)           |
| **CentOS (rpm)**            | [download](https://github.com/spicelang/spice/releases/latest/download/spice_amd64.rpm)           | [download](https://github.com/spicelang/spice/releases/latest/download/spice_arm64.rpm)           |
| **Debian (deb)**            | [download](https://github.com/spicelang/spice/releases/latest/download/spice_amd64.deb)           | [download](https://github.com/spicelang/spice/releases/latest/download/spice_arm64.deb)           |
| **Fedora (rpm)**            | [download](https://github.com/spicelang/spice/releases/latest/download/spice_amd64.rpm)           | [download](https://github.com/spicelang/spice/releases/latest/download/spice_arm64.rpm)           |
| **Raspbian (deb)**          | [download](https://github.com/spicelang/spice/releases/latest/download/spice_amd64.deb)           | [download](https://github.com/spicelang/spice/releases/latest/download/spice_arm64.deb)           |
| **Ubuntu (deb)**            | [download](https://github.com/spicelang/spice/releases/latest/download/spice_amd64.deb)           | [download](https://github.com/spicelang/spice/releases/latest/download/spice_arm64.deb)           |
| **Windows Installer (msi)** | [download](https://github.com/spicelang/spice/releases/latest/download/spice_x64_setup.msi)       | -                                                                                                 |
| **Windows Portable (zip)**  | [download](https://github.com/spicelang/spice/releases/latest/download/spice_windows_amd64.zip)   | -                                                                                                 |
| **MacOS / Darwin (tar.gz)** | [download](https://github.com/spicelang/spice/releases/latest/download/spice_darwin_amd64.tar.gz) | [download](https://github.com/spicelang/spice/releases/latest/download/spice_darwin_arm64.tar.gz) |


## Setup guide for contributors
#### Clone the project
```sh
git clone https://github.com/spicelang/spice.git
cd spice
```

#### Setup dev environment
There is a batch/shell script called `dev-setup` to help you with that. It will do the following things:

- Make sure you have all dependencies
- Clone and build the required LLVM version to the `llvm` subdirectory
- Download all required third-party libs to the `lib` subdirectory
- Build Spice for the first time into the `bin` subdirectory

Use this command to run it:

```sh
# Linux
./dev-setup.sh
# Windows
.\dev-setup.bat
```

#### Re-build Spice
There is also a batch/shell script to rebuild Spice. Use the following command to run it:

```sh
# Linux
./build.sh
# Windows
.\build.bat
```

You can find the build output in the `bin` subdirectory.

## Available target platforms
Currently, Spice only offers stable support for linux/x86_64, linux/aarch64 and windows/x86_64.
But you can try to compile to the following architectures without any stability promises:

`aarch64`, `amdgpu`, `armv5`, `armv6`, `armv7`, `avr`, `bpf`, `hexagon`, `lanai`, `loongarch`, `mips`, `msp430`, `nvptx`,
`powerpc`, `riscv`, `sparc`, `systemz`, `ve`, `webassembly`, `x86`, `x86_64`, `xcore`

## Contribute to the project
If you want to contribute to this project, please ensure you comply with the [contribution guidelines](./CONTRIBUTING.md).

© ChilliBits 2021-2023
<p align="center">
  <img alt="Spice Logo" src="./docs/docs/static/avatar.png" height="220" />
  <h3 align="center">Spice Programming Language</h3>
  <p align="center">Compiler and standard library for the Spice programming language.</p>
  <p align="center">
    <a target="_blank" href="https://github.com/chillibits/spice/releases/latest"><img src="https://img.shields.io/github/v/release/chillibits/spice?include_prereleases"></a>
    <a target="_blank" href="https://hub.docker.com/r/chillibits/spice"><img src="https://img.shields.io/docker/pulls/chillibits/spice"></a>
    <a target="_blank" href="./.github/workflows/ci-go.yml"><img src="https://github.com/chillibits/spice/actions/workflows/ci-go.yml/badge.svg"></a>
	<a target="_blank" href="./.github/workflows/ci-cpp.yml"><img src="https://github.com/chillibits/spice/actions/workflows/ci-cpp.yml/badge.svg"></a>
	<a target="_blank" href="./.github/workflows/codeql-analysis.yml"><img src="https://github.com/chillibits/spice/actions/workflows/codeql-analysis.yml/badge.svg"></a>
    <a target="_blank" href="https://goreportcard.com/report/github.com/chillibits/spice"><img src="https://goreportcard.com/badge/github.com/chillibits/spice"></a>
    <a target="_blank" href="https://makeapullrequest.com"><img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg"></a>
    <a target="_blank" href="./LICENSE.md"><img src="https://img.shields.io/github/license/chillibits/spice"></a>
  </p>
</p>

---

This is the home repo of the Spice programming language. Spice is a compiled language which sets a focus on performance and practicality. It is considered as a systems language, which means it is neither suitable for coding user interfaces nor any interface with the user, except cli tools. Spice supports cross-compilation to all target platforms, listed below.

## Documentation
Please visit the documentation at [spicelang.com](https://www.spicelang.com).

## Grammar
The Spice grammar can be found [here](./compiler/src/grammar/Spice.g4) as a ANTLR grammar configuration file.

## Available target platforms
*All LLVM output platforms. To be extended ...*

## CMake instructions for building LLVM (required for building Spice itself)
**Configure:**
`cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_CXX_FLAGS_RELEASE="-O2" -G "CodeBlocks - MinGW Makefiles" ../llvm`

**Build:**
`cmake --build .`

## Contribute to the project
If you want to contribute to this project, please ensure you comply with the [contribution guidelines](./CONTRIBUTING.md).

© ChilliBits 2021
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

## Semantic analysis checks
- [x] Assignments have the same type
- [x] If condition is bool
- [x] While condition is bool
- [x] For condition is bool
- [x] Condition of ternery is bool
- [x] True type and false type of ternary are matching
- [x] Variables are declared before referenced
- [x] Variables are declared exactly once
- [x] Variables with `const` attached are not modified later
- [x] Functions/Procedures are defined before called
- [x] Can only call functions/procedures and no variables
- [x] Only use parameters, that exist in the declaration
- [x] Parameter types are matching
- [x] Return statement only inside of function
- [x] Return data type matches function def
- [x] Logical operators are only applied to booleans
- [x] Bitwise operators are only applied to booleans or integers
- [x] Equality operators are only applied to some type combinations
- [x] Relational operators are only applied to doubles or integers
- [x] Additive operators are only applied to some type combinations
- [x] Multiplicative operators are only applied to some type combinations
- [x] Prefix unary operators are only applied to integers
- [x] Postfix unary operators are only applied to intentifiers of type integer
- [x] Program contains main function
- [x] Dyn data type for function args with default value
- [x] Dyn data type as function return types

## Support missing / to be tested
- [x] [x] Functions can be overloaded
- [ ] [ ] Optional parameters can be omitted when calling functions/procedures
- [ ] [ ] Variables also can be declared in the root scope

## Special language features
- Something like `"Test" * 3` is valid and will evaluate to `"TestTestTest"` (not implemented yet)
- Alternatively to the return statement in a function, you can also assign variable `result` with a value, which was automatically declared by the function head
- Binary minus has to be applied with a space between (e.g.: `n - 5`) and unary minus has to be applied without a space between (e.g.: `-3.4`)
- Default values of function/procedure parameters are possible e.g.: `f<int> test(int param = 2) {...}`
- Function/procedure parameters with a default value have to be last in the param list

## Available target platforms
*All LLVM output platforms. To be extended ...*

## CMake instructions for building LLVM (required for building Spice itself)
`cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_CXX_FLAGS_RELEASE="-O2" -G "CodeBlocks - MinGW Makefiles" ../llvm` <br>
`cmake --build .`

## Contribute to the project
If you want to contribute to this project, please ensure you comply with the [contribution guidelines](./CONTRIBUTING.md).

© ChilliBits 2021
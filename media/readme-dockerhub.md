<p align="center">
  <img alt="Spice Logo" src="./docs/docs/static/avatar.png" height="220" />
  <h3 align="center">Spice Programming Language</h3>
  <p align="center">Compiler and standard library for the Spice programming language.</p>
  <p align="center">
    <a target="_blank" href="https://github.com/spicelang/spice/releases/latest"><img src="https://img.shields.io/github/v/release/chillibits/spice?include_prereleases"></a>
    <a target="_blank" href="https://hub.docker.com/r/chillibits/spice"><img src="https://img.shields.io/docker/pulls/chillibits/spice"></a>
    <a target="_blank" href="https://github.com/spicelang/spice/blob/main/.github/workflows/ci-go.yml"><img src="https://github.com/spicelang/spice/actions/workflows/ci-go.yml/badge.svg"></a>
	<a target="_blank" href="https://github.com/spicelang/spice/blob/main/.github/workflows/ci-cpp.yml"><img src="https://github.com/spicelang/spice/actions/workflows/ci-cpp.yml/badge.svg"></a>
	<a target="_blank" href="https://github.com/spicelang/spice/blob/main/.github/workflows/codeql-analysis.yml"><img src="https://github.com/spicelang/spice/actions/workflows/codeql-analysis.yml/badge.svg"></a>
    <a target="_blank" href="https://goreportcard.com/report/github.com/spicelang/spice"><img src="https://goreportcard.com/badge/github.com/spicelang/spice"></a>
    <a target="_blank" href="https://makeapullrequest.com"><img src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg"></a>
    <a target="_blank" href="https://github.com/spicelang/spice/blob/main/LICENSE.md"><img src="https://img.shields.io/github/license/spicelang/spice"></a>
  </p>
</p>

---

This is the home repo of the Spice programming language. Spice is a compiled language which sets a focus on performance and practicality. It is considered as a systems language, which means it is neither suitable for coding user interfaces nor any interface with the user, except cli tools. Spice supports cross-compilation to all target platforms, listed below.

## Documentation
Please visit the documentation at [spicelang.com](https://www.spicelang.com).

## Usage
You can use Spice by directly installing it on your system or by using the Docker container.

### Install Spice
For installation instructions for Linux, Windows, etc., please visit the [installation guide](https://www.spicelang.com/install/linux).

## QuickStart with Docker
*Note for Windows users: This command does not work with Windows CMD command line. Please use Windows PowerShell instead.*

```sh
$ docker run --rm -it -v ${pwd}:/spice/out chillibits/spice
```

## Contribute otherwise to the project
If you want to contribute to this project, please ensure you comply with the [contribution guidelines](https://github.com/spicelang/spice/blob/main/CONTRIBUTING.md).

© ChilliBits 2021-2022
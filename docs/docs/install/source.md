---
icon: fontawesome/solid/hammer
title: Build from source
tags:
  - Installation
  - Build
---

### Setup
Before being able to compile the source code, you first have to download it from GitHub.
Furthermore, you need Python 3, as well as a common C++ compiler (preferably GCC), CMake and Ninja to build LLVM, which the
compiler links against.

The compiler is written in Spice itself. It is built by a released Spice compiler (the stage0 compiler), whose version is
pinned in `.github/stage0-version`. The build scripts download it for your platform into `build/stage0/`.

#### Clone from GitHub
```sh
git clone https://github.com/spicelang/spice.git
cd spice
```

### Run setup script for setting up your dev environment
To build Spice, you can use the `dev-setup.py` script. This will prepare your dev environment, install the correct LLVM version, download the stage0 compiler and build the Spice executable for the first time to the `build` directory.

=== ":fontawesome-brands-linux: Linux"
    ```sh
    python dev-setup.py
    ```
=== ":fontawesome-brands-apple: macOS"
    ```sh
    python dev-setup.py
    ```
=== ":fontawesome-brands-windows: Windows"
    ```sh
    python dev-setup.py
    ```

### Use
=== ":fontawesome-brands-linux: Linux"
    ```sh
    cd build
    ./spice [options] <input>
    ```
=== ":fontawesome-brands-apple: macOS"
    ```sh
    cd build
    ./spice [options] <input>
    ```
=== ":fontawesome-brands-windows: Windows"
    ```sh
    cd build
    .\spice [options] <input>
    ```

### Rebuild
To build the compiler again after changing its sources in `src/`, run:
```sh
python build.py
```
It builds the compiler with the stage0 compiler, optimized and with link-time optimization, to `build/spice`. Pass
`--build-type Debug` for an unoptimized build with debug info.

### Bootstrap
To check that the compiler builds itself reproducibly, run:
```sh
python bootstrap.py --output build/spice
```
`bootstrap.py` builds the compiler from `src/` with the stage0 compiler, lets it compile itself until two consecutive builds
are identical, and copies the result to the given path.

### Run the tests
```sh
python test/run-tests.py
```
The test runner builds the compiler with the stage0 compiler and runs all test cases in `test/test-files` against it. Pass
`--compiler build/spice` to test an already built compiler instead, or `--filter` to select test cases (see `--help`).

### Optional: TPDE backend
The experimental [TPDE backend](../how-to/experimental-backends.md) needs the TPDE libraries (Linux only). To build them
into the std, run:
```sh
python setup-deps.py --tpde
```

---
icon: fontawesome/solid/hammer
title: Build from source
tags:
  - Installation
  - Build
---

### Setup
Before being able to compile the source code, you first have to download it from GitHub.
Furthermore, you need a common C++ compiler (preferably GCC) and CMake to build the compiler executable.

#### Clone from GitHub
```sh
git clone https://github.com/spicelang/spice.git
cd spice
```

### Run setup script for setting up your dev environment
To build Spice, you can use the `dev-setup.py` script. This will prepare your dev environment, install the correct LLVM version and builds the Spice executable for the first time to the `build` directory.

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

### Build the self-hosted compiler
The steps above build the host compiler, which is written in C++. The release packages ship the self-hosted compiler as
`spice` instead, which is written in Spice itself. It is built by a released self-hosted compiler (the stage0 compiler),
whose version is pinned in `.github/stage0-version`. To build it as well, run:
```sh
python fetch-stage0.py
python bootstrap.py --output build/spice-self-hosted
```
`fetch-stage0.py` downloads the stage0 compiler for your platform into `build/stage0/`. `bootstrap.py` builds the self-hosted
compiler from `src/` with it, lets it compile itself until two consecutive builds are identical, and copies the result to
the given path. Without a downloaded stage0 compiler, `bootstrap.py` starts from the host compiler instead.

### Optional build flags

The following CMake options are available when configuring the build (pass them as `-D<name>=ON` to `cmake`):

| Flag                      | Description                                                                                            |
|---------------------------|--------------------------------------------------------------------------------------------------------|
| `SPICE_UNITY_BUILD`       | Enable CMake unity builds for the compiler executable.                                                 |
| `SPICE_LTO`               | Link-time optimization for the compiler executable.                                                    |
| `SPICE_LINK_STATIC`       | Statically link the compiler executable.                                                               |
| `SPICE_ENABLE_TPDE`       | Enable the experimental [TPDE backend](../how-to/experimental-backends.md) as an alternative to LLVM.  |

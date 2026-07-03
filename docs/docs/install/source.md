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

### Optional build flags

The following CMake options are available when configuring the build (pass them as `-D<name>=ON` to `cmake`):

| Flag                      | Description                                                                                            |
|---------------------------|--------------------------------------------------------------------------------------------------------|
| `SPICE_UNITY_BUILD`       | Enable CMake unity builds for the compiler executable.                                                 |
| `SPICE_LTO`               | Link-time optimization for the compiler executable.                                                    |
| `SPICE_LINK_STATIC`       | Statically link the compiler executable.                                                               |
| `SPICE_ENABLE_TPDE`       | Enable the experimental [TPDE backend](../how-to/experimental-backends.md) as an alternative to LLVM.  |

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
To build Spice, you can use the `dev-setup.sh` for Linux/macOS or the `dev-setup.bat` for Windows. This will prepare your dev environment, install the correct LLVM version and builds the Spice executable for the first time to the `build` directory.

=== ":fontawesome-brands-linux: Linux"
    ```sh
    ./dev-setup.sh
    ```
=== ":fontawesome-brands-apple: macOS"
    ```sh
    ./dev-setup.bat
    ```
=== ":fontawesome-brands-windows: Windows"
    ```sh
    .\dev-setup.bat
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

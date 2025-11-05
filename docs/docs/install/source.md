---
title: Build or install from source
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

=== "Linux/macOS"
    ```sh
    ./dev-setup.sh
    ```
=== "Windows"
    ```sh
    .\dev-setup.bat
    ```

### Use
```sh
cd build
./spice [options] <input>
```
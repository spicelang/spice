---
title: Build or install from source
---

### Setup
Before being able to compile the source code, you first have to download it from GitHub and install Golang (if you don't have it already).
Furthermore you need a common C++ compiler and CMake to build the compiler executable.

#### Clone from GitHub
```sh
git clone https://github.com/spicelang/spice.git
cd spice
```

#### Install Go
For compiling Spice from source, you need to have Go 1.16 or higher installed on your system. Please refer to the [official guide](https://golang.org/doc/install) on how to install Go.

### Build from source
To build Spice, you can use the `build.sh` for Linux or the `build.bat` for Windows. This will build the executable to the `bin` directory. You can then add the bin directory to the path to use the compiler from elsewhere.

=== "Linux"
    ```sh
    ./build.sh
    ```
=== "Windows"
    ```sh
    .\build.bat
    ```

### Use
```sh
spice [options] <input>
```
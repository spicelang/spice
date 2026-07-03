cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(Spice
    DESCRIPTION "Spice Programming Language"
    HOMEPAGE_URL "https://www.spicelang.com"
    LANGUAGES CXX C
)

# CMake settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set more CMake settings conditionally
include(Conditionals.cmake)

# Parse CMake args
include(Options.cmake)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Spice: Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Spice: Using LLVMConfig.cmake from ${LLVM_DIR}")
message(STATUS "Spice: LLVM include dirs: ${LLVM_INCLUDE_DIRS}")

# Include llvm options to determine which libs to link
include(LLVMOptions.cmake)

# Third-party header-only dependencies (git submodules in deps/)
add_subdirectory(deps/json EXCLUDE_FROM_ALL)
add_subdirectory(deps/cli11 EXCLUDE_FROM_ALL)

# Experimental TPDE backend (opt-in via SPICE_ENABLE_TPDE)
if (SPICE_ENABLE_TPDE)
    # Skip TPDE's own test suite (requires `lit`) and force LLVM support ON.
    set(TPDE_INCLUDE_TESTS OFF CACHE BOOL "" FORCE)
    set(TPDE_ENABLE_LLVM ON CACHE BOOL "" FORCE)
    set(TPDE_ENABLE_ENCODEGEN ON CACHE BOOL "" FORCE)
    # Spice itself links LLVM statically (see LLVMOptions.cmake). TPDE defaults to shared LLVM; align it
    # with Spice's convention so both sides can be linked into the same executable.
    set(TPDE_LINK_LLVM_STATIC ON CACHE BOOL "" FORCE)
    add_subdirectory(deps/tpde EXCLUDE_FROM_ALL)
    message(STATUS "Spice: TPDE submodule added; will link tpde::tpde_llvm")
endif ()

# Coverage
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/coverage.py ${CMAKE_CURRENT_BINARY_DIR}/coverage.py SYMBOLIC)
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/gcovr.cfg ${CMAKE_CURRENT_BINARY_DIR}/gcovr.cfg SYMBOLIC)

# Include main module
add_subdirectory(src)

# Include test module
enable_testing()
add_subdirectory(test)

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(Spice
    DESCRIPTION "Spice Programming Language"
    HOMEPAGE_URL "https://www.spicelang.com"
    LANGUAGES CXX
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
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Include llvm options to determine which libs to link
include(LLVMOptions.cmake)

# Coverage
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/coverage.bat ${CMAKE_CURRENT_BINARY_DIR}/coverage.bat SYMBOLIC)
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/coverage.sh ${CMAKE_CURRENT_BINARY_DIR}/coverage.sh SYMBOLIC)
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/gcovr.cfg ${CMAKE_CURRENT_BINARY_DIR}/gcovr.cfg SYMBOLIC)

# Include main module
include_directories(src)
add_subdirectory(src)

# Include test module
enable_testing()
add_subdirectory(test)

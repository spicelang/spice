cmake_minimum_required(VERSION 3.20)
project(Spice)

# Set constants
set(CMAKE_CXX_STANDARD 20)
set(LLVM_DIR $ENV{LLVM_DIR})

# Include cli options parsing
include(Options.cmake)

# Set release flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -O2")

# Find ANTLR
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/cmake)
add_definitions(-DANTLR4CPP_STATIC)
include(ExternalAntlr4Cpp)
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/antlr-4.13.1-complete.jar)
find_package(ANTLR REQUIRED)
message(STATUS "Spice: Found ANTLR ${ANTLR_VERSION}")

# Find Google Test
add_subdirectory(lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Spice: Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Spice: Using LLVMConfig.cmake from ${LLVM_DIR}")
message(STATUS "Spice: LLVM include dirs: ${LLVM_INCLUDE_DIRS}")
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Map LLVM components to lib names
llvm_map_components_to_libnames(LLVM_LIBS aarch64codegen amdgpucodegen armcodegen avrcodegen bpfcodegen hexagoncodegen
        lanaicodegen loongarchcodegen mcjit mipscodegen msp430codegen nativecodegen nvptxcodegen powerpccodegen riscvcodegen
        sparccodegen systemzcodegen target vecodegen webassemblycodegen x86codegen xcorecodegen)

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
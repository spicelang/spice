cmake_minimum_required(VERSION 3.17)
project(Spice)

# Set constants
set(CMAKE_CXX_STANDARD 20)
set(LLVM_DIR $ENV{LLVM_DIR})

# Get options
option(SPICE_LINK_STATIC "Link the compiler executable statically" OFF)

# Link statically
if (SPICE_LINK_STATIC)
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

# Coverage
SET(CMAKE_CXX_FLAGS_DEBUG "-g -O0 --coverage -fprofile-arcs -ftest-coverage ${CMAKE_CXX_FLAGS_DEBUG}")

# Find ANTLR
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/cmake)
add_definitions(-DANTLR4CPP_STATIC)
include(ExternalAntlr4Cpp)
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/antlr/antlr-4.9.3-complete.jar)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake from ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Map LLVM components to lib names
llvm_map_components_to_libnames(LLVM_LIBS aarch64asmparser aarch64codegen aarch64desc aarch64disassembler
        aarch64info aarch64utils aggressiveinstcombine amdgpuasmparser amdgpucodegen amdgpudesc
        amdgpudisassembler amdgpuinfo amdgpuutils analysis armasmparser armcodegen armdesc armdisassembler arminfo
        armutils asmparser asmprinter avrasmparser avrcodegen avrdesc avrdisassembler avrinfo binaryformat bitreader
        bitstreamreader bitwriter bpfasmparser bpfcodegen bpfdesc bpfdisassembler bpfinfo cfguard codegen core
        coroutines coverage debuginfocodeview debuginfodwarf debuginfogsym debuginfomsf debuginfopdb demangle
        dlltooldriver dwarflinker executionengine extensions filecheck frontendopenacc frontendopenmp fuzzmutate
        globalisel hexagonasmparser hexagoncodegen hexagondesc hexagondisassembler hexagoninfo instcombine
        instrumentation interfacestub interpreter ipo irreader jitlink lanaiasmparser lanaicodegen lanaidesc
        lanaidisassembler lanaiinfo libdriver lineeditor linker lto mc mca mcdisassembler mcjit mcparser
        mipsasmparser mipscodegen mipsdesc mipsdisassembler mipsinfo mirparser msp430asmparser msp430codegen
        msp430desc msp430disassembler msp430info native nativecodegen nvptxcodegen nvptxdesc nvptxinfo objcarcopts
        object objectyaml option orcjit orcshared orctargetprocess passes powerpcasmparser powerpccodegen
        powerpcdesc powerpcdisassembler powerpcinfo profiledata remarks riscvasmparser riscvcodegen riscvdesc
        riscvdisassembler riscvinfo runtimedyld scalaropts selectiondag sparcasmparser sparccodegen sparcdesc
        sparcdisassembler sparcinfo support symbolize systemzasmparser systemzcodegen systemzdesc
        systemzdisassembler systemzinfo tablegen target textapi transformutils vectorize veasmparser vecodegen vedesc
        vedisassembler veinfo webassemblyasmparser webassemblycodegen webassemblydesc webassemblydisassembler
        webassemblyinfo webassemblyutils windowsmanifest x86asmparser x86codegen x86desc x86disassembler x86info
        xcorecodegen xcoredesc xcoredisassembler xcoreinfo xray)

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
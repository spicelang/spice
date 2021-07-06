set(BINARY ${CMAKE_PROJECT_NAME})
set(LLVM_DIR C:/Users/i516467/Documents/JustForFunGitHubClones/llvm-project/build/lib/cmake/llvm)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_definitions(-DANTLR4CPP_STATIC)

include(ExternalAntlr4Cpp)
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/antlr/antlr-4.9.2-complete.jar)
find_package(ANTLR REQUIRED)
antlr_target(Spice ${CMAKE_CURRENT_SOURCE_DIR}/grammar/Spice.g4 VISITOR)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

set(SOURCES
        main.cpp
        analyzer/AnalyzerVisitor.cpp
        analyzer/AnalyzerVisitor.h
        analyzer/SymbolTable.cpp
        analyzer/SymbolTable.h
        analyzer/SymbolTableEntry.cpp
        analyzer/SymbolTableEntry.h
        ir/GeneratorVisitor.cpp
        ir/GeneratorVisitor.h
        exception/SemanticError.cpp
        exception/SemanticError.h)

add_executable(${BINARY}_run ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})

llvm_map_components_to_libnames(LLVM_LIBS core support irreader)

include_directories(${ANTLR4CPP_INCLUDE_DIRS})
include_directories(${ANTLR_Spice_OUTPUT_DIR})
include_directories(../lib/antlr4/runtime/Cpp/runtime/src)

target_link_libraries(${BINARY}_run antlr4_static)
target_link_libraries(${BINARY}_run ${LLVM_LIBS})

add_library(${BINARY}_lib STATIC ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})
set(BINARY ${CMAKE_PROJECT_NAME})

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_definitions(-DANTLR4CPP_STATIC)

include(ExternalAntlr4Cpp)
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/antlr/antlr-4.9.2-complete.jar)
find_package(ANTLR REQUIRED)
antlr_target(Spice ${CMAKE_CURRENT_SOURCE_DIR}/grammar/Spice.g4 VISITOR)

set(SOURCES
        main.cpp
        analyzer/AnalyzerVisitor.cpp
        analyzer/AnalyzerVisitor.h
        analyzer/SymbolTable.cpp
        analyzer/SymbolTable.h
        analyzer/SymbolTableEntry.cpp
        analyzer/SymbolTableEntry.h
        exception/SemanticError.cpp
        exception/SemanticError.h)

add_executable(${BINARY}_run ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})

include_directories(${ANTLR4CPP_INCLUDE_DIRS})
include_directories(${ANTLR_Spice_OUTPUT_DIR})
include_directories(../lib/antlr4/runtime/Cpp/runtime/src)

target_link_libraries(${BINARY}_run antlr4_static)
add_library(${BINARY}_lib STATIC ${SOURCES} exception/SemanticError.cpp exception/SemanticError.h analyzer/SymbolTableEntry.cpp analyzer/SymbolTableEntry.h)
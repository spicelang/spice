set(BINARY ${CMAKE_PROJECT_NAME})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib/)

set(SOURCES
        main.cpp
        antlr/SpiceLexer.cpp
        antlr/SpiceLexer.h
        antlr/SpiceParser.cpp
        antlr/SpiceParser.h
        antlr/SpiceBaseVisitor.cpp
        antlr/SpiceBaseVisitor.h
        antlr/SpiceVisitor.cpp
        antlr/SpiceVisitor.h
        analyzer/AnalyzerVisitor.cpp
        analyzer/AnalyzerVisitor.h
        analyzer/SymbolTable.cpp
        analyzer/SymbolTable.h)

add_executable(${BINARY}_run ${SOURCES})

include_directories(../lib/antlr4/runtime/Cpp/runtime/src)
find_library(ANTLR antlr4-runtime HINTS ../lib/antlr4-prebuilt/lib/vs-2015/x64/ReleaseDLL)
target_link_libraries(${BINARY}_run ${ANTLR})

add_library(${BINARY}_lib STATIC ${SOURCES})
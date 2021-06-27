set(BINARY ${CMAKE_PROJECT_NAME})

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

include_directories(../lib/antlr4/antlr4-runtime)

add_library(${BINARY}_lib STATIC ${SOURCES})
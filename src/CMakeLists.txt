set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES
        main.cpp
        main.h
        antlr/SpiceLexer.cpp
        antlr/SpiceLexer.h
        antlr/SpiceParser.cpp
        antlr/SpiceParser.h
        antlr/SpiceBaseVisitor.cpp
        antlr/SpiceBaseVisitor.h
        antlr/SpiceVisitor.cpp
        antlr/SpiceVisitor.h)

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})
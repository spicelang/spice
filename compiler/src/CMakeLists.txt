# Sources to compile
set(SOURCES
        main.cpp
        CompilerInstance.cpp
        CompilerInstance.h
        analyzer/AnalyzerVisitor.cpp
        analyzer/AnalyzerVisitor.h
        analyzer/OpRuleManager.cpp
        analyzer/OpRuleManager.h
        generator/GeneratorVisitor.cpp
        generator/GeneratorVisitor.h
        generator/OpRuleConversionsManager.cpp
        generator/OpRuleConversionsManager.h
        symbol/SymbolTable.cpp
        symbol/SymbolTable.h
        symbol/SymbolTableEntry.cpp
        symbol/SymbolTableEntry.h
        symbol/SymbolType.cpp
        symbol/SymbolType.h
        symbol/ScopePath.cpp
        symbol/ScopePath.h
        symbol/SymbolSpecifiers.cpp
        symbol/SymbolSpecifiers.h
        exception/SemanticError.cpp
        exception/SemanticError.h
        exception/IRError.cpp
        exception/IRError.h
        exception/LexerParserError.cpp
        exception/LexerParserError.h
        exception/AntlrThrowingErrorListener.cpp
        exception/AntlrThrowingErrorListener.h
        util/ScopeIdUtil.cpp
        util/ScopeIdUtil.h
        util/ModuleRegistry.cpp
        util/ModuleRegistry.h
        util/FunctionSignature.cpp
        util/FunctionSignature.h
        util/FileUtil.cpp
        util/FileUtil.h
        util/CompilerWarning.cpp
        util/CompilerWarning.h)

# Find ANTLR
find_package(ANTLR REQUIRED)
antlr_target(Spice ${CMAKE_CURRENT_SOURCE_DIR}/grammar/Spice.g4 VISITOR)

# Include Antlr runtime and the generated components like Lexer and Parser
include_directories(${ANTLR_Spice_OUTPUT_DIR})
include_directories(../lib/antlr4/runtime/Cpp/runtime/src)

# Add executable target: Spice_run
add_executable(${CMAKE_PROJECT_NAME}_run ${ANTLR_Spice_CXX_OUTPUTS} ${SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME}_run antlr4_static ${LLVM_LIBS})

# Add library target: Spice_lib
add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})
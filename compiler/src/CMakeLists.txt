# Find ANTLR
find_package(ANTLR REQUIRED)
antlr_target(Spice ${CMAKE_CURRENT_SOURCE_DIR}/grammar/Spice.g4 VISITOR)

set(SOURCES
        main.cpp
        CompilerInstance.cpp
        CompilerInstance.h
        cli/CliInterface.cpp
        cli/CliInterface.h
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
        symbol/Capture.cpp
        symbol/Capture.h
        symbol/SymbolType.cpp
        symbol/SymbolType.h
        symbol/ScopePath.cpp
        symbol/ScopePath.h
        symbol/SymbolSpecifiers.cpp
        symbol/SymbolSpecifiers.h
        exception/ErrorFactory.cpp
        exception/ErrorFactory.h
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
        util/ThreadFactory.cpp
        util/ThreadFactory.h
        util/FunctionSignature.cpp
        util/FunctionSignature.h
        util/FileUtil.cpp
        util/FileUtil.h
        util/CompilerWarning.cpp
        util/CompilerWarning.h)

add_executable(${CMAKE_PROJECT_NAME}_run ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})

# Include Antlr components
include_directories(${ANTLR_Spice_OUTPUT_DIR})
include_directories(../lib/antlr4/runtime/Cpp/runtime/src)

target_link_libraries(${CMAKE_PROJECT_NAME}_run antlr4_static ${LLVM_LIBS})
add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})
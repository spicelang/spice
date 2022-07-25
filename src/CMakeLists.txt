# Find ANTLR
find_package(ANTLR REQUIRED)
antlr_target(Spice ${CMAKE_CURRENT_SOURCE_DIR}/Spice.g4 VISITOR)

set(SOURCES
        main.cpp
        cli/CliInterface.cpp
        cli/CliInterface.h
        analyzer/PreAnalyzerVisitor.cpp
        analyzer/PreAnalyzerVisitor.h
        analyzer/AnalyzerVisitor.cpp
        analyzer/AnalyzerVisitor.h
        analyzer/OpRuleManager.cpp
        analyzer/OpRuleManager.h
        generator/GeneratorVisitor.cpp
        generator/GeneratorVisitor.h
        generator/OpRuleConversionsManager.cpp
        generator/OpRuleConversionsManager.h
        linker/LinkerInterface.cpp
        linker/LinkerInterface.h
        symbol/SymbolTable.cpp
        symbol/SymbolTable.h
        symbol/SymbolTableEntry.cpp
        symbol/SymbolTableEntry.h
        symbol/Function.cpp
        symbol/Function.h
        symbol/Struct.cpp
        symbol/Struct.h
        symbol/Capture.cpp
        symbol/Capture.h
        symbol/SymbolType.cpp
        symbol/SymbolType.h
        symbol/GenericType.cpp
        symbol/GenericType.h
        symbol/ScopePath.cpp
        symbol/ScopePath.h
        symbol/SymbolSpecifiers.cpp
        symbol/SymbolSpecifiers.h
        ast/AstNode.cpp
        ast/AstNode.h
        dependency/SourceFile.cpp
        dependency/SourceFile.h
        exception/ErrorFactory.cpp
        exception/ErrorFactory.h
        exception/CliError.cpp
        exception/CliError.h
        exception/SemanticError.cpp
        exception/SemanticError.h
        exception/IRError.cpp
        exception/IRError.h
        exception/LinkerError.cpp
        exception/LinkerError.h
        exception/LexerParserError.cpp
        exception/LexerParserError.h
        exception/AntlrThrowingErrorListener.cpp
        exception/AntlrThrowingErrorListener.h
        visualizer/CSTVisualizerVisitor.cpp
        visualizer/CSTVisualizerVisitor.h
        util/CommonUtil.cpp
        util/CommonUtil.h
        util/ScopeIdUtil.cpp
        util/ScopeIdUtil.h
        util/ThreadFactory.cpp
        util/ThreadFactory.h
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
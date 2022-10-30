# Find ANTLR
find_package(ANTLR REQUIRED)
antlr_target(Spice ${CMAKE_CURRENT_SOURCE_DIR}/Spice.g4 VISITOR)

set(SOURCES
        # Global source files
        main.cpp
        CompilerPass.h
        SourceFile.cpp
        SourceFile.h
        # Global resource
        global/GlobalResourceManager.h
        global/CacheManager.cpp
        global/CacheManager.h
        global/RuntimeModuleManager.cpp
        global/RuntimeModuleManager.h
        # CLI
        cli/CliInterface.cpp
        cli/CliInterface.h
        # CST visualizer
        visualizer/CSTVisualizerVisitor.cpp
        visualizer/CSTVisualizerVisitor.h
        # AST builder
        ast/AbstractAstVisitor.cpp
        ast/AbstractAstVisitor.h
        ast/AstVisitor.cpp
        ast/AstVisitor.h
        ast/AstNodes.h
        ast/AstBuilder.cpp
        ast/AstBuilder.h
        # AST visualizer
        visualizer/ASTVisualizerVisitor.cpp
        visualizer/ASTVisualizerVisitor.h
        # Import collector
        importcollector/ImportCollector.cpp
        importcollector/ImportCollector.h
        # Semantic analyzer
        semanticanalyzer/SemanticAnalyzer.cpp
        semanticanalyzer/SemanticAnalyzer.h
        # Type checker
        typechecker/TypeChecker.cpp
        typechecker/TypeChecker.h
        typechecker/OpRuleManager.cpp
        typechecker/OpRuleManager.h
        # Borrow checker
        borrowchecker/BorrowChecker.cpp
        borrowchecker/BorrowChecker.h
        # Escape analyzer
        escapeanalyzer/EscapeAnalyzer.cpp
        escapeanalyzer/EscapeAnalyzer.h
        # IR generator
        irgenerator/IRGenerator.cpp
        irgenerator/IRGenerator.h
        # IR optimizer
        iroptimizer/IROptimizer.cpp
        iroptimizer/IROptimizer.h
        # Object emitter
        objectemitter/ObjectEmitter.cpp
        objectemitter/ObjectEmitter.h
        # Linker
        linker/LinkerInterface.cpp
        linker/LinkerInterface.h
        # Symbol table management
        symbol/SymbolTable.cpp
        symbol/SymbolTable.h
        symbol/SymbolTableEntry.cpp
        symbol/SymbolTableEntry.h
        symbol/Function.cpp
        symbol/Function.h
        symbol/Struct.cpp
        symbol/Struct.h
        symbol/Interface.cpp
        symbol/Interface.h
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
        # Exception handling
        exception/CliError.cpp
        exception/CliError.h
        exception/SemanticError.cpp
        exception/SemanticError.h
        exception/IRError.cpp
        exception/IRError.h
        exception/LinkerError.cpp
        exception/LinkerError.h
        exception/LexerError.cpp
        exception/LexerError.h
        exception/ParserError.cpp
        exception/ParserError.h
        exception/AntlrThrowingErrorListener.cpp
        exception/AntlrThrowingErrorListener.h
        # Utility
        util/CommonUtil.cpp
        util/CommonUtil.h
        util/ThreadFactory.cpp
        util/ThreadFactory.h
        util/FileUtil.cpp
        util/FileUtil.h
        util/CodeLoc.cpp
        util/CodeLoc.h
        util/CompilerWarning.cpp
        util/CompilerWarning.h

        # ToDo: Remove
        analyzer/PreAnalyzerVisitor.cpp
        analyzer/PreAnalyzerVisitor.h
        analyzer/AnalyzerVisitor.cpp
        analyzer/AnalyzerVisitor.h
        generator/GeneratorVisitor.cpp
        generator/GeneratorVisitor.h
        generator/StdFunctionManager.cpp
        generator/StdFunctionManager.h
        generator/OpRuleConversionsManager.cpp
        generator/OpRuleConversionsManager.h
)

add_executable(spice ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})

# Include Antlr components
include_directories(${ANTLR_Spice_OUTPUT_DIR})
include_directories(../lib/antlr4/runtime/Cpp/runtime/src)

target_link_libraries(spice antlr4_static ${LLVM_LIBS})
add_library(spicelib STATIC ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})
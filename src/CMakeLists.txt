# Find ANTLR
find_package(ANTLR REQUIRED)
antlr_target(Spice ${CMAKE_CURRENT_SOURCE_DIR}/Spice.g4 VISITOR)

set(SOURCES
        # Global source files
        main.cpp
        CompilerPass.cpp
        CompilerPass.h
        SourceFile.cpp
        SourceFile.h
        # Global resource
        global/GlobalResourceManager.cpp
        global/GlobalResourceManager.h
        global/CacheManager.cpp
        global/CacheManager.h
        global/RuntimeModuleManager.cpp
        global/RuntimeModuleManager.h
        # CLI
        cli/CLIInterface.cpp
        cli/CLIInterface.h
        # CST visualizer
        visualizer/CSTVisualizer.cpp
        visualizer/CSTVisualizer.h
        # AST builder
        ast/AbstractASTVisitor.cpp
        ast/AbstractASTVisitor.h
        ast/ASTVisitor.cpp
        ast/ASTVisitor.h
        ast/ASTNodes.h
        ast/ASTBuilder.cpp
        ast/ASTBuilder.h
        # AST visualizer
        visualizer/ASTVisualizer.cpp
        visualizer/ASTVisualizer.h
        # AST optimizer
        astoptimizer/ASTOptimizer.cpp
        astoptimizer/ASTOptimizer.h
        # Import collector
        importcollector/ImportCollector.cpp
        importcollector/ImportCollector.h
        # Symbol table builder
        symboltablebuilder/SymbolTableBuilder.cpp
        symboltablebuilder/SymbolTableBuilder.h
        symboltablebuilder/SymbolTable.cpp
        symboltablebuilder/SymbolTable.h
        symboltablebuilder/SymbolTableEntry.cpp
        symboltablebuilder/SymbolTableEntry.h
        symboltablebuilder/Capture.cpp
        symboltablebuilder/Capture.h
        symboltablebuilder/SymbolType.cpp
        symboltablebuilder/SymbolType.h
        symboltablebuilder/SymbolSpecifiers.cpp
        symboltablebuilder/SymbolSpecifiers.h
        # Type checker
        typechecker/TypeChecker.cpp
        typechecker/TypeChecker.h
        typechecker/TypeCheckerLookup.cpp
        typechecker/TypeCheckerAnalyze.cpp
        typechecker/OpRuleManager.cpp
        typechecker/OpRuleManager.h
        # Borrow checker
        borrowchecker/BorrowChecker.cpp
        borrowchecker/BorrowChecker.h
        borrowchecker/Lifecycle.cpp
        borrowchecker/Lifecycle.h
        # Escape analyzer
        escapeanalyzer/EscapeAnalyzer.cpp
        escapeanalyzer/EscapeAnalyzer.h
        # IR generator
        irgenerator/IRGenerator.cpp
        irgenerator/IRGenerator.h
        irgenerator/StdFunctionManager.cpp
        irgenerator/StdFunctionManager.h
        irgenerator/OpRuleConversionsManager.cpp
        irgenerator/OpRuleConversionsManager.h
        irgenerator/DebugInfoGenerator.cpp
        irgenerator/DebugInfoGenerator.h
        # IR optimizer
        iroptimizer/IROptimizer.cpp
        iroptimizer/IROptimizer.h
        # Object emitter
        objectemitter/ObjectEmitter.cpp
        objectemitter/ObjectEmitter.h
        # Linker
        linker/LinkerInterface.cpp
        linker/LinkerInterface.h
        # Scope management
        scope/Scope.cpp
        scope/Scope.h
        # Model
        model/Function.cpp
        model/Function.h
        model/Struct.cpp
        model/Struct.h
        model/Interface.cpp
        model/Interface.h
        model/GenericType.cpp
        model/GenericType.h
        model/ScopePath.cpp
        model/ScopePath.h
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
        util/Timer.h

        # ToDo: Remove
        #analyzer/PreAnalyzerVisitor.cpp
        #analyzer/PreAnalyzerVisitor.h
        #analyzer/AnalyzerVisitor.cpp
        #analyzer/AnalyzerVisitor.h
        #generator/GeneratorVisitor.cpp
        #generator/GeneratorVisitor.h
        )

add_executable(spice ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})

# Include Antlr components
include_directories(${ANTLR_Spice_OUTPUT_DIR})
include_directories(../lib/antlr4/runtime/Cpp/runtime/src)

target_link_libraries(spice antlr4_static ${LLVM_LIBS})
add_library(spicelib STATIC ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})
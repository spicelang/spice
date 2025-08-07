# Set ANTLR grammar
antlr_target(Spice Spice.g4 VISITOR PACKAGE spice::compiler)

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
        global/TypeRegistry.cpp
        global/TypeRegistry.h
        # Driver
        driver/Driver.cpp
        driver/Driver.h
        # CST visualizer
        visualizer/CSTVisualizer.cpp
        visualizer/CSTVisualizer.h
        # AST builder
        ast/ASTNodes.cpp
        ast/ASTNodes.h
        ast/AbstractASTVisitor.cpp
        ast/AbstractASTVisitor.h
        ast/ASTVisitor.cpp
        ast/ASTVisitor.h
        ast/ParallelizableASTVisitor.cpp
        ast/ParallelizableASTVisitor.h
        ast/ASTBuilder.cpp
        ast/ASTBuilder.h
        ast/Attributes.h
        # AST visualizer
        visualizer/ASTVisualizer.h
        # Import collector
        importcollector/ImportCollector.cpp
        importcollector/ImportCollector.h
        # Symbol table builder
        symboltablebuilder/SymbolTableBuilder.cpp
        symboltablebuilder/SymbolTableBuilder.h
        symboltablebuilder/Scope.cpp
        symboltablebuilder/Scope.h
        symboltablebuilder/SymbolTable.cpp
        symboltablebuilder/SymbolTable.h
        symboltablebuilder/SymbolTableEntry.cpp
        symboltablebuilder/SymbolTableEntry.h
        symboltablebuilder/QualType.cpp
        symboltablebuilder/QualType.h
        symboltablebuilder/Capture.cpp
        symboltablebuilder/Capture.h
        symboltablebuilder/Type.cpp
        symboltablebuilder/Type.h
        symboltablebuilder/TypeChain.cpp
        symboltablebuilder/TypeChain.h
        symboltablebuilder/TypeQualifiers.cpp
        symboltablebuilder/TypeQualifiers.h
        symboltablebuilder/Lifecycle.cpp
        symboltablebuilder/Lifecycle.h
        symboltablebuilder/ScopeHandle.cpp
        symboltablebuilder/ScopeHandle.h
        # Type checker
        typechecker/TypeChecker.cpp
        typechecker/TypeChecker.h
        typechecker/TypeCheckerTopLevelDefinitions.cpp
        typechecker/TypeCheckerTopLevelDefinitionsPrepare.cpp
        typechecker/TypeCheckerTopLevelDefinitionsCheck.cpp
        typechecker/TypeCheckerControlStructures.cpp
        typechecker/TypeCheckerStatements.cpp
        typechecker/TypeCheckBuiltinFunctions.cpp
        typechecker/TypeCheckerExpressions.cpp
        typechecker/TypeCheckerValues.cpp
        typechecker/TypeCheckMeta.cpp
        typechecker/TypeCheckerImplicit.cpp
        typechecker/OpRuleManager.cpp
        typechecker/OpRuleManager.h
        typechecker/FunctionManager.cpp
        typechecker/FunctionManager.h
        typechecker/StructManager.cpp
        typechecker/StructManager.h
        typechecker/InterfaceManager.cpp
        typechecker/InterfaceManager.h
        typechecker/TypeMatcher.cpp
        typechecker/TypeMatcher.h
        typechecker/ExprResult.h
        typechecker/MacroDefs.h
        # Dependency graph visualizer
        visualizer/DependencyGraphVisualizer.cpp
        visualizer/DependencyGraphVisualizer.h
        # IR generator
        irgenerator/IRGenerator.cpp
        irgenerator/IRGenerator.h
        irgenerator/GenTopLevelDefinitions.cpp
        irgenerator/GenControlStructures.cpp
        irgenerator/GenStatements.cpp
        irgenerator/GenBuiltinFunctions.cpp
        irgenerator/GenExpressions.cpp
        irgenerator/GenValues.cpp
        irgenerator/GenImplicit.cpp
        irgenerator/GenTargetDependent.cpp
        irgenerator/GenVTable.cpp
        irgenerator/StdFunctionManager.cpp
        irgenerator/StdFunctionManager.h
        irgenerator/OpRuleConversionManager.cpp
        irgenerator/OpRuleConversionManager.h
        irgenerator/DebugInfoGenerator.cpp
        irgenerator/DebugInfoGenerator.h
        irgenerator/NameMangling.cpp
        irgenerator/NameMangling.h
        irgenerator/LLVMExprResult.h
        # IR optimizer
        iroptimizer/IROptimizer.cpp
        iroptimizer/IROptimizer.h
        # Object emitter
        objectemitter/ObjectEmitter.cpp
        objectemitter/ObjectEmitter.h
        # Linker
        linker/BitcodeLinker.cpp
        linker/BitcodeLinker.h
        linker/ExternalLinkerInterface.cpp
        linker/ExternalLinkerInterface.h
        # Model
        model/Function.cpp
        model/Function.h
        model/StructBase.cpp
        model/StructBase.h
        model/Struct.cpp
        model/Struct.h
        model/Interface.h
        model/GenericType.cpp
        model/GenericType.h
        # Exception handling
        exception/ErrorManager.cpp
        exception/ErrorManager.h
        exception/CompilerError.cpp
        exception/CompilerError.h
        exception/CliError.cpp
        exception/CliError.h
        exception/SemanticError.cpp
        exception/SemanticError.h
        exception/LinkerError.cpp
        exception/LinkerError.h
        exception/LexerError.cpp
        exception/LexerError.h
        exception/ParserError.cpp
        exception/ParserError.h
        exception/AntlrThrowingErrorListener.cpp
        exception/AntlrThrowingErrorListener.h
        # Utility
        util/BlockAllocator.h
        util/CommonUtil.cpp
        util/CommonUtil.h
        util/FileUtil.cpp
        util/FileUtil.h
        util/CustomHashFunctions.cpp
        util/CustomHashFunctions.h
        util/CodeLoc.cpp
        util/CodeLoc.h
        util/CompilerWarning.cpp
        util/CompilerWarning.h
        util/GlobalDefinitions.h
        util/Timer.h
        util/DeferredLogic.h
        util/Memory.h
        util/RawStringOStream.cpp
        util/RawStringOStream.h
)

add_executable(spice ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})

# Enable pedantic warnings
target_compile_options(spice PRIVATE -Wpedantic -Wall -Wno-unknown-pragmas)

# Include Antlr components
include_directories(
        ${ANTLR_Spice_OUTPUT_DIR}
        ../lib/antlr4/runtime/Cpp/runtime/src
)

target_link_libraries(spice antlr4_static ${LLVM_LIBS})
add_library(spicecore STATIC ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})

# Add leak check target
add_custom_target(spiceleakcheck COMMAND valgrind --leak-check=full --error-exitcode=1 ./spice DEPENDS spice)
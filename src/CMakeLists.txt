# Set ANTLR grammar
antlr_target(Spice Spice.g4 VISITOR PACKAGE spice::compiler)

set(SOURCES
        # Global source files
        main.cpp
        CompilerPass.cpp
        SourceFile.cpp
        # Global resource
        global/GlobalResourceManager.cpp
        global/CacheManager.cpp
        global/RuntimeModuleManager.cpp
        global/TypeRegistry.cpp
        # Driver
        driver/Driver.cpp
        # CST visualizer
        visualizer/CSTVisualizer.cpp
        # AST builder
        ast/ASTNodes.cpp
        ast/AbstractASTVisitor.cpp
        ast/ASTVisitor.cpp
        ast/ParallelizableASTVisitor.cpp
        ast/ASTBuilder.cpp
        # Import collector
        importcollector/ImportCollector.cpp
        # Symbol table builder
        symboltablebuilder/SymbolTableBuilder.cpp
        symboltablebuilder/Scope.cpp
        symboltablebuilder/SymbolTable.cpp
        symboltablebuilder/SymbolTableEntry.cpp
        symboltablebuilder/QualType.cpp
        symboltablebuilder/Capture.cpp
        symboltablebuilder/Type.cpp
        symboltablebuilder/TypeChain.cpp
        symboltablebuilder/TypeQualifiers.cpp
        symboltablebuilder/Lifecycle.cpp
        symboltablebuilder/ScopeHandle.cpp
        # Type checker
        typechecker/TypeChecker.cpp
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
        typechecker/FunctionManager.cpp
        typechecker/StructManager.cpp
        typechecker/InterfaceManager.cpp
        typechecker/TypeMatcher.cpp
        # Dependency graph visualizer
        visualizer/DependencyGraphVisualizer.cpp
        # IR generator
        irgenerator/IRGenerator.cpp
        irgenerator/GenTopLevelDefinitions.cpp
        irgenerator/GenControlStructures.cpp
        irgenerator/GenStatements.cpp
        irgenerator/GenBuiltinFunctions.cpp
        irgenerator/GenExpressions.cpp
        irgenerator/GenValues.cpp
        irgenerator/GenImplicit.cpp
        irgenerator/GenTargetDependent.cpp
        irgenerator/GenVTable.cpp
        irgenerator/GenInstrumentation.cpp
        irgenerator/MetadataGenerator.cpp
        irgenerator/StdFunctionManager.cpp
        irgenerator/OpRuleConversionManager.cpp
        irgenerator/DebugInfoGenerator.cpp
        irgenerator/NameMangling.cpp
        # IR optimizer
        iroptimizer/IROptimizer.cpp
        # Object emitter
        objectemitter/ObjectEmitter.cpp
        # Linker
        linker/BitcodeLinker.cpp
        linker/ExternalLinkerInterface.cpp
        # Model
        model/Function.cpp
        model/StructBase.cpp
        model/Struct.cpp
        model/Interface.cpp
        model/GenericType.cpp
        # Exception handling
        exception/ErrorManager.cpp
        exception/CompilerError.cpp
        exception/CliError.cpp
        exception/SemanticError.cpp
        exception/LinkerError.cpp
        exception/LexerError.cpp
        exception/ParserError.cpp
        exception/AntlrThrowingErrorListener.cpp
        # Utility
        util/CommonUtil.cpp
        util/FileUtil.cpp
        util/SystemUtil.cpp
        util/CustomHashFunctions.cpp
        util/CodeLoc.cpp
        util/CompilerWarning.cpp
        util/RawStringOStream.cpp
)

add_executable(spice ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})

# Enable pedantic warnings
target_compile_options(spice PRIVATE -Wpedantic -Wall -Werror -Wno-unknown-pragmas)

# Include Antlr components
include_directories(
        ${ANTLR_Spice_OUTPUT_DIR}
        ../lib/antlr4/runtime/Cpp/runtime/src
)

target_link_libraries(spice antlr4_static ${LLVM_LIBS})
add_library(spicecore STATIC ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})

# Add leak check target
add_custom_target(spiceleakcheck COMMAND valgrind --leak-check=full --error-exitcode=1 ./spice DEPENDS spice)
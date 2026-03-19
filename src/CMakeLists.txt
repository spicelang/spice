# Initialize ANTLR
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(ExternalAntlr4Cpp)
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/antlr-4.13.2-complete.jar)
find_package(ANTLR REQUIRED)
message(STATUS "Spice: Found ANTLR ${ANTLR_VERSION}")

#################### libspicecore ####################

set(SOURCES
        # Global source files
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
        typechecker/TypeCheckerBuiltinFunctions.cpp
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

# Initialize ANTLR
antlr_target(Spice Spice.g4 VISITOR PACKAGE spice::compiler)
# Create libspicecore library
add_library(spicecore STATIC ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})
# Link ANTLR publicly
target_link_libraries(spicecore PUBLIC antlr4_static)
# Include ANTLR components
target_include_directories(spicecore SYSTEM PUBLIC ${ANTLR4_INCLUDE_DIRS} ${ANTLR_Spice_OUTPUT_DIR})
# Enable pedantic warnings
target_compile_options(spicecore PRIVATE -Wpedantic -Wall -Werror -Wno-unknown-pragmas)

################## spice executable ##################

add_executable(spice main.cpp)
# Link with spicecore, antlr and llvm
target_link_libraries(spice PRIVATE spicecore ${LLVM_LIBS})
# Enable pedantic warnings
target_compile_options(spice PRIVATE -Wpedantic -Wall -Werror -Wno-unknown-pragmas)

################### spice_leakcheck ##################

add_custom_target(spice_leakcheck COMMAND valgrind --leak-check=full --error-exitcode=1 ./spice DEPENDS spice)

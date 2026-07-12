# Initialize ANTLR
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
add_definitions(-DANTLR4CPP_STATIC)

# Build the ANTLR4 C++ runtime from the in-tree submodule (deps/antlr4)
set(ANTLR_BUILD_CPP_TESTS OFF CACHE BOOL "" FORCE)
set(ANTLR_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(ANTLR_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(WITH_DEMO OFF CACHE BOOL "" FORCE)
set(ANTLR4_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/deps/antlr4/runtime/Cpp ${CMAKE_BINARY_DIR}/antlr4_runtime EXCLUDE_FROM_ALL)
set(ANTLR4_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/deps/antlr4/runtime/Cpp/runtime/src)

# Locate the ANTLR code generator jar (downloaded by setup-deps)
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
        global/TypeNameDisambiguator.cpp
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
        typechecker/PostTypeCheckingVerifier.cpp
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
        objectemitter/LLVMObjectEmitter.cpp
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
# Add the current directory as include dir
target_include_directories(spicecore SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Link ANTLR runtime and header-only dependencies
target_link_libraries(spicecore PUBLIC antlr4_static nlohmann_json::nlohmann_json CLI11::CLI11)

# Optional TPDE backend — compiled into its own static library so TPDE's -fno-rtti requirement
# (propagated by tpde::tpde_llvm as a usage requirement) does not leak into spicecore, which
# depends on RTTI (dynamic_cast, typeid).
if (SPICE_ENABLE_TPDE)
    # tpde::tpde_llvm publishes -fno-rtti as an INTERFACE_COMPILE_OPTION (when LLVM is built without RTTI).
    # Spice code relies on typeid/dynamic_cast, so we cannot let that option propagate. Wrap the emitter in
    # its own static library and consume TPDE via a shim interface target that carries only the include
    # directories and link libraries — no compile options.
    get_target_property(_tpde_include_dirs tpde_llvm_impl INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(_tpde_system_dirs  tpde_llvm_impl INTERFACE_SYSTEM_INCLUDE_DIRECTORIES)
    add_library(spice_tpde STATIC objectemitter/TPDEObjectEmitter.cpp)
    target_include_directories(spice_tpde SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    # PRIVATE link so TPDE's interface-scoped -fno-rtti does not propagate onwards.
    target_link_libraries(spice_tpde PRIVATE tpde::tpde_llvm spicecore)
    # We compile this TU with -fno-rtti to match TPDE's ABI expectations.
    target_compile_options(spice_tpde PRIVATE -fno-rtti)
    target_compile_definitions(spice_tpde PRIVATE SPICE_ENABLE_TPDE)
endif ()
# Add include directories
target_include_directories(spicecore
    SYSTEM PUBLIC
        ${ANTLR4_INCLUDE_DIRS}
        ${ANTLR_Spice_OUTPUT_DIR}
        ${LLVM_INCLUDE_DIRS}
)
# Add LLVM compile definitions
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
target_compile_definitions(spicecore PRIVATE ${LLVM_DEFINITIONS_LIST})
# Enable pedantic warnings
target_compile_options(spicecore PRIVATE -Wpedantic -Wall -Werror -Wno-unknown-pragmas ${SPICE_EXTRA_COMPILE_OPTIONS})
# Scope volatile build-info defines to the two TUs that actually use them, so ccache isn't busted on every commit.
set(_SPICE_BUILD_INFO_DEFS
    -DSPICE_VERSION="${SPICE_VERSION}"
    -DSPICE_GIT_HASH="${SPICE_GIT_HASH}"
    -DSPICE_BUILT_BY="${SPICE_BUILT_BY}"
)
set_property(SOURCE util/CommonUtil.cpp APPEND PROPERTY COMPILE_OPTIONS ${_SPICE_BUILD_INFO_DEFS})
set_property(SOURCE irgenerator/IRGenerator.cpp APPEND PROPERTY COMPILE_OPTIONS ${_SPICE_BUILD_INFO_DEFS})
# RawStringOStream derives from the polymorphic LLVM type llvm::raw_pwrite_stream. Compiling it with
# RTTI emits a type_info that references "typeinfo for llvm::raw_pwrite_stream", which does not exist
# when LLVM is built without RTTI (the default). The class is never used with dynamic_cast/typeid, so
# compile this single translation unit without RTTI to drop the unused type_info and avoid the
# dangling reference at link time.
set_source_files_properties(util/RawStringOStream.cpp PROPERTIES COMPILE_OPTIONS -fno-rtti)

################## spice executable ##################

add_executable(spice main.cpp)
# Link with spicecore, antlr and llvm
target_link_libraries(spice PRIVATE spicecore ${LLVM_LIBS})
if (SPICE_ENABLE_TPDE)
    target_link_libraries(spice PRIVATE spice_tpde)
endif ()
# Enable pedantic warnings
target_compile_options(spice PRIVATE -Wpedantic -Wall -Werror -Wno-unknown-pragmas ${SPICE_EXTRA_COMPILE_OPTIONS})

################### spice_leakcheck ##################

add_custom_target(spice_leakcheck COMMAND valgrind --leak-check=full --error-exitcode=1 ./spice DEPENDS spice)

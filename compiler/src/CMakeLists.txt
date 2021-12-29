set(BINARY ${CMAKE_PROJECT_NAME})

# Antlr
find_package(ANTLR REQUIRED)
antlr_target(Spice ${CMAKE_CURRENT_SOURCE_DIR}/grammar/Spice.g4 VISITOR)

set(SOURCES
        main.cpp
        CompilerInstance.cpp
        CompilerInstance.h
        analyzer/AnalyzerVisitor.cpp
        analyzer/AnalyzerVisitor.h
        analyzer/SymbolTable.cpp
        analyzer/SymbolTable.h
        analyzer/SymbolTableEntry.cpp
        analyzer/SymbolTableEntry.h
        analyzer/SymbolType.cpp
        analyzer/SymbolType.h
        analyzer/ModuleRegistry.cpp
        analyzer/ModuleRegistry.h
        generator/GeneratorVisitor.cpp
        generator/GeneratorVisitor.h
        exception/SemanticError.cpp
        exception/SemanticError.h
        exception/IRError.cpp
        exception/IRError.h
        type/SymbolType.cpp
        type/SymbolType.h
        type/OpRuleManager.cpp
        type/OpRuleManager.h
        type/OpRuleConversionsManager.cpp
        type/OpRuleConversionsManager.h
        util/ScopeIdUtil.cpp
        util/ScopeIdUtil.h
        util/FunctionSignature.cpp
        util/FunctionSignature.h
        util/FileUtil.cpp
        util/FileUtil.h
        util/IdentifierUtil.cpp
        util/IdentifierUtil.h
        util/CompilerWarning.cpp
        util/CompilerWarning.h)

add_executable(${BINARY}_run ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})

# Include Antlr components
include_directories(${ANTLR4CPP_INCLUDE_DIRS})
include_directories(${ANTLR_Spice_OUTPUT_DIR})
include_directories(../lib/antlr4/runtime/Cpp/runtime/src)

target_link_libraries(${BINARY}_run antlr4_static ${LLVM_LIBS})
add_library(${BINARY}_lib STATIC ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})
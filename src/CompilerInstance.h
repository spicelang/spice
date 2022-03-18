// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <cli/CliInterface.h>
#include <symbol/SymbolTable.h>
#include <analyzer/AnalyzerVisitor.h>
#include <generator/GeneratorVisitor.h>

#include <llvm/IR/IRBuilder.h>

/**
 * Represents the compilation process of a single source file to a LLVM module / object file
 */
class CompilerInstance {
public:
    static SymbolTable* CompileSourceFile(const std::shared_ptr<llvm::LLVMContext>& context,
                                          const std::shared_ptr<llvm::IRBuilder<>>& builder, ModuleRegistry* moduleRegistry,
                                          ThreadFactory* threadFactory, CliOptions* options, const std::string& sourceFile,
                                          bool requiresMainFct, bool stdFile);
};

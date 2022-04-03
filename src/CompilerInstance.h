// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <analyzer/AnalyzerVisitor.h>
#include <cli/CliInterface.h>
#include <generator/GeneratorVisitor.h>
#include <linker/LinkerInterface.h>
#include <symbol/SymbolTable.h>

#include <llvm/IR/IRBuilder.h>

/**
 * Represents the compilation process of a Spice project
 */
class CompilerInstance {
public:
  static SymbolTable *compileSourceFile(const std::shared_ptr<llvm::LLVMContext> &context,
                                        const std::shared_ptr<llvm::IRBuilder<>> &builder, ModuleRegistry *moduleRegistry,
                                        ThreadFactory *threadFactory, CliOptions *options, LinkerInterface *linker,
                                        const std::string &sourceFilePath);
};
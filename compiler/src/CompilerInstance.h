// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <symbol/SymbolTable.h>

#include <llvm/IR/IRBuilder.h>

/**
 * Represents the compilation process of a single source file to a LLVM module / object file
 */
class CompilerInstance {
public:
    static SymbolTable* CompileSourceFile(const std::shared_ptr<llvm::LLVMContext>& context,
                                          const std::shared_ptr<llvm::IRBuilder<>>& builder,
                                          const std::string& sourceFile, const std::string& targetArch,
                                          const std::string& targetVendor, const std::string& targetOs,
                                          const std::string& objectDir, bool debugOutput, int optLevel, bool requiresMainFct,
                                          bool stdFile);
};

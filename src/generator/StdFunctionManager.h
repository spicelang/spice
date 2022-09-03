// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

// Forward declarations
class GeneratorVisitor;

class StdFunctionManager {
public:
  // Constructors
  explicit StdFunctionManager(GeneratorVisitor* generator);

  // Public methods for type retrieval
  llvm::StructType *getStringStructType() const;

  // Public methods for function retrieval
  llvm::Function *getPrintfFct() const;
  llvm::Function *getExitFct() const;
  llvm::Function *getStackSaveFct() const;
  llvm::Function *getStackRestoreFct() const;
  llvm::Function *getStringRawFct() const;
  llvm::Function *getStringLitPlusOpStringLitFct() const;
  llvm::Function *getStringLitEqualsOpStringLitFct() const;

private:
  // Members
  GeneratorVisitor *generator;
  llvm::LLVMContext *context;
  llvm::IRBuilder<> *builder;
  llvm::Module *module;
};
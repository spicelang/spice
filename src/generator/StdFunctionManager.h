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
  llvm::Function *getStackSaveIntrinsic() const;
  llvm::Function *getStackRestoreIntrinsic() const;
  llvm::Function *getStringGetRawFct() const;
  llvm::Function *getStringCtorStringFct() const;
  llvm::Function *getStringCtorStringStringFct() const;
  llvm::Function *getStringIsRawEqualStringStringFct() const;
  llvm::Function *getStringAppendStringFct() const;
  llvm::Function *getStringAppendCharFct() const;

private:
  // Members
  GeneratorVisitor *generator;
  llvm::LLVMContext *context;
  llvm::IRBuilder<> *builder;
  llvm::Module *module;
};
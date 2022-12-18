// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <global/GlobalResourceManager.h>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

class StdFunctionManager {
public:
  // Constructors
  StdFunctionManager(GlobalResourceManager &resourceManager, llvm::Module *module)
      : context(resourceManager.context), builder(resourceManager.builder), module(module) {}

  // Public methods for type retrieval
  static llvm::StructType *getStrobjType(llvm::LLVMContext &context);

  // Public methods for function retrieval
  [[nodiscard]] llvm::Function *getPrintfFct() const;
  [[nodiscard]] llvm::Function *getExitFct() const;
  [[nodiscard]] llvm::Function *getStackSaveIntrinsic() const;
  [[nodiscard]] llvm::Function *getStackRestoreIntrinsic() const;
  [[nodiscard]] llvm::Function *getMemcpyIntrinsic() const;
  [[nodiscard]] llvm::Function *getPthreadCreate() const;
  [[nodiscard]] llvm::Function *getPthreadSelf() const;
  [[nodiscard]] llvm::Function *getPthreadJoin() const;
  [[nodiscard]] llvm::Function *getStringIsRawEqualStringStringFct() const;

private:
  // Members
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  llvm::Module *module;

  // Private methods
  llvm::Function *getFunction(const char *funcName, llvm::Type *returnType, llvm::ArrayRef<llvm::Type *> args,
                              bool varArg = false) const;
  [[nodiscard]] llvm::Function *getProcedure(const char *procName, llvm::ArrayRef<llvm::Type *> args, bool varArg = false) const;
};
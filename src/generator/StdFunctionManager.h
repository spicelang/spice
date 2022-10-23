// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <dependency/GlobalResourceManager.h>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

class StdFunctionManager {
public:
  // Constructors
  explicit StdFunctionManager(GlobalResourceManager &resourceManager, llvm::Module *module)
      : context(resourceManager.context), builder(resourceManager.builder), module(module) {}

  // Public methods for type retrieval
  static llvm::StructType *getStrobjType(llvm::LLVMContext &context);

  // Public methods for function retrieval
  [[nodiscard]] llvm::Function *getPrintfFct() const;
  [[nodiscard]] llvm::Function *getExitFct() const;
  [[nodiscard]] llvm::Function *getStackSaveIntrinsic() const;
  [[nodiscard]] llvm::Function *getStackRestoreIntrinsic() const;
  [[nodiscard]] llvm::Function *getMemcpyIntrinsic() const;
  [[nodiscard]] llvm::Function *getStringGetRawFct() const;
  //[[nodiscard]] llvm::Function *getStringCtorCharFct() const;
  [[nodiscard]] llvm::Function *getStringCtorStringFct() const;
  [[nodiscard]] llvm::Function *getStringCtorFct() const;
  [[nodiscard]] llvm::Function *getStringCtorStringStrobjptrFct() const;
  [[nodiscard]] llvm::Function *getStringCtorStrobjptrStringFct() const;
  [[nodiscard]] llvm::Function *getStringCtorStringStringFct() const;
  //[[nodiscard]] llvm::Function *getStringDtorFct() const;
  [[nodiscard]] llvm::Function *getStringIsRawEqualStringStringFct() const;
  [[nodiscard]] llvm::Function *getStringIsEqualStrobjFct() const;
  [[nodiscard]] llvm::Function *getStringAppendStringFct() const;
  [[nodiscard]] llvm::Function *getStringAppendStrobjptrFct() const;
  [[nodiscard]] llvm::Function *getStringAppendCharFct() const;
  [[nodiscard]] llvm::Function *getStringMulOpIntFct() const;
  [[nodiscard]] llvm::Function *getStringMulOpLongFct() const;
  [[nodiscard]] llvm::Function *getStringMulOpShortFct() const;

private:
  // Members
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  llvm::Module *module;

  // Private methods
  llvm::Function *getFunction(const std::string &funcName, llvm::Type *returnType, llvm::ArrayRef<llvm::Type *> args,
                              bool varArg = false) const;
  [[nodiscard]] llvm::Function *getProcedure(const std::string &procName, llvm::ArrayRef<llvm::Type *> args,
                                             bool varArg = false) const;
};
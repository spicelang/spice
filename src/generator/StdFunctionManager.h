// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

// Forward declarations
class GeneratorVisitor;

class StdFunctionManager {
public:
  // Constructors
  explicit StdFunctionManager(GeneratorVisitor *generator);

  // Public methods for type retrieval
  static llvm::StructType *getStringStructType(llvm::LLVMContext &context);

  // Public methods for function retrieval
  [[nodiscard]] llvm::Function *getPrintfFct() const;
  [[nodiscard]] llvm::Function *getExitFct() const;
  [[nodiscard]] llvm::Function *getStackSaveIntrinsic() const;
  [[nodiscard]] llvm::Function *getStackRestoreIntrinsic() const;
  [[nodiscard]] llvm::Function *getStringGetRawFct() const;
  [[nodiscard]] llvm::Function *getStringCtorCharFct() const;
  [[nodiscard]] llvm::Function *getStringCtorStringFct() const;
  [[nodiscard]] llvm::Function *getStringCtorStringStringFct() const;
  [[nodiscard]] llvm::Function *getStringDtorFct() const;
  [[nodiscard]] llvm::Function *getStringIsRawEqualStringStringFct() const;
  [[nodiscard]] llvm::Function *getStringAppendStringFct() const;
  [[nodiscard]] llvm::Function *getStringAppendCharFct() const;
  [[nodiscard]] llvm::Function *getStringMulOpIntFct() const;
  [[nodiscard]] llvm::Function *getStringMulOpLongFct() const;
  [[nodiscard]] llvm::Function *getStringMulOpShortFct() const;

private:
  // Members
  llvm::LLVMContext *context;
  llvm::IRBuilder<> *builder;
  llvm::Module *module;

  // Private methods
  llvm::Function *getFunction(const std::string &functionName, llvm::Type *returnType, llvm::ArrayRef<llvm::Type *> args,
                              bool varArg = false) const;
  [[nodiscard]] llvm::Function *getProcedure(const std::string &procedureName, llvm::ArrayRef<llvm::Type *> args,
                                             bool varArg = false) const;
};
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
  llvm::Function *getPrintfFct() const;
  llvm::Function *getExitFct() const;
  llvm::Function *getStackSaveIntrinsic() const;
  llvm::Function *getStackRestoreIntrinsic() const;
  llvm::Function *getStringGetRawFct() const;
  llvm::Function *getStringCtorCharFct() const;
  llvm::Function *getStringCtorStringFct() const;
  llvm::Function *getStringCtorStringStringFct() const;
  llvm::Function *getStringDtorFct() const;
  llvm::Function *getStringIsRawEqualStringStringFct() const;
  llvm::Function *getStringAppendStringFct() const;
  llvm::Function *getStringAppendCharFct() const;
  llvm::Function *getStringMulOpIntFct() const;
  llvm::Function *getStringMulOpLongFct() const;
  llvm::Function *getStringMulOpShortFct() const;

private:
  // Members
  llvm::LLVMContext *context;
  llvm::IRBuilder<> *builder;
  llvm::Module *module;

  // Private methods
  llvm::Function *getFunction(const std::string functionName, llvm::Type *returnType, llvm::ArrayRef<llvm::Type *> args,
                              bool varArg = false) const;
  llvm::Function *getProcedure(const std::string procedureName, llvm::ArrayRef<llvm::Type *> args, bool varArg = false) const;
};
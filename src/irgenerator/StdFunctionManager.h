// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <global/GlobalResourceManager.h>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

namespace spice::compiler {

// Forward declarations
class SymbolType;
class Function;

class StdFunctionManager {
public:
  // Constructors
  StdFunctionManager(GlobalResourceManager &resourceManager, llvm::Module *module)
      : context(resourceManager.context), builder(resourceManager.builder), module(module) {}

  // Public methods for function retrieval
  [[nodiscard]] llvm::Function *getPrintfFct() const;
  [[nodiscard]] llvm::Function *getExitFct() const;
  [[nodiscard]] llvm::Function *getMemcpyIntrinsic() const;
  [[nodiscard]] llvm::Function *getStringIsRawEqualStringStringFct() const;
  [[nodiscard]] llvm::Function *getIteratorGetFct(const Function *spiceFunc) const;
  [[nodiscard]] llvm::Function *getIteratorGetIdxFct(const Function *spiceFunc, Scope *accessScope) const;
  [[nodiscard]] llvm::Function *getIteratorIsValidFct(const Function *spiceFunc) const;
  [[nodiscard]] llvm::Function *getIteratorNextFct(const Function *spiceFunc) const;

private:
  // Members
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  llvm::Module *module;

  // Private methods
  llvm::Function *getFunction(const char *funcName, llvm::Type *returnType, llvm::ArrayRef<llvm::Type *> args,
                              bool varArg = false) const;
  [[nodiscard]] llvm::Function *getProcedure(const char *procName, llvm::ArrayRef<llvm::Type *> args) const;
};

} // namespace spice::compiler
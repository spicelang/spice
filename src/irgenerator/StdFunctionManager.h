// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

namespace spice::compiler {

// Forward declarations
class Function;
class GlobalResourceManager;
class SourceFile;

class StdFunctionManager {
public:
  // Constructors
  StdFunctionManager(SourceFile *sourceFile, GlobalResourceManager &resourceManager, llvm::Module *module);

  // Public methods for function retrieval
  [[nodiscard]] llvm::Function *getPrintfFct() const;
  [[nodiscard]] llvm::Function *getFPrintfFct() const;
  [[nodiscard]] llvm::Function *getExitFct() const;
  [[nodiscard]] llvm::Function *getFreeFct() const;
  [[nodiscard]] llvm::Function *getMemcmpFct() const;
  [[nodiscard]] llvm::Function *getMemcpyIntrinsic() const;
  [[nodiscard]] llvm::Function *getStringGetRawLengthStringFct() const;
  [[nodiscard]] llvm::Function *getStringIsRawEqualStringStringFct() const;
  [[nodiscard]] llvm::Function *getAllocUnsafeLongFct() const;
  [[nodiscard]] llvm::Function *getDeallocBytePtrRefFct() const;
  [[nodiscard]] llvm::Function *getIterateFct(const Function *spiceFunc) const;
  [[nodiscard]] llvm::Function *getIteratorFct(const Function *spiceFunc) const;
  [[nodiscard]] llvm::Function *getIteratorGetFct(const Function *spiceFunc) const;
  [[nodiscard]] llvm::Function *getIteratorGetIdxFct(const Function *spiceFunc) const;
  [[nodiscard]] llvm::Function *getIteratorIsValidFct(const Function *spiceFunc) const;
  [[nodiscard]] llvm::Function *getIteratorNextFct(const Function *spiceFunc) const;
  [[nodiscard]] llvm::Function *getAcrtIOFuncFct() const;

private:
  // Members
  SourceFile *sourceFile;
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  llvm::Module *module;

  // Private methods
  llvm::Function *getFunction(const char *funcName, llvm::Type *returnType, llvm::ArrayRef<llvm::Type *> args,
                              bool varArg = false) const;
  [[nodiscard]] llvm::Function *getProcedure(const char *procName, llvm::ArrayRef<llvm::Type *> args) const;
};

} // namespace spice::compiler
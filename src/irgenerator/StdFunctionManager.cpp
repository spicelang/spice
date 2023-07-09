// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "StdFunctionManager.h"

#include <irgenerator/NameMangling.h>
#include <model/Function.h>

namespace spice::compiler {

llvm::Function *StdFunctionManager::getPrintfFct() const {
  llvm::Function *printfFct = getFunction("printf", builder.getInt32Ty(), builder.getInt8PtrTy(), true);
  // Set noundef attribute to template string
  printfFct->addParamAttr(0, llvm::Attribute::NoUndef);
  return printfFct;
}

llvm::Function *StdFunctionManager::getExitFct() const { return getProcedure("exit", builder.getInt32Ty()); }

llvm::Function *StdFunctionManager::getMemcpyIntrinsic() const {
  llvm::Type *ptrTy = builder.getPtrTy();
  return getProcedure("llvm.memcpy.p0.p0.i64", {ptrTy, ptrTy, builder.getInt64Ty(), builder.getInt1Ty()});
}

llvm::Function *StdFunctionManager::getStringIsRawEqualStringStringFct() const {
  return getFunction("_Z10isRawEqualPcPc", builder.getInt1Ty(), {builder.getPtrTy(), builder.getPtrTy()});
}

llvm::Function *StdFunctionManager::getIteratorGetFct(const Function *spiceFunc) const {
  const std::string functionName = NameMangling::mangleFunction(*spiceFunc);
  return getFunction(functionName.c_str(), builder.getPtrTy(), builder.getPtrTy());
}

llvm::Function *StdFunctionManager::getIteratorGetIdxFct(const Function *spiceFunc, Scope *accessScope) const {
  const std::string functionName = NameMangling::mangleFunction(*spiceFunc);
  llvm::Type *pairTy = spiceFunc->returnType.toLLVMType(context, accessScope);
  return getFunction(functionName.c_str(), pairTy, builder.getPtrTy());
}

llvm::Function *StdFunctionManager::getIteratorIsValidFct(const Function *spiceFunc) const {
  const std::string functionName = NameMangling::mangleFunction(*spiceFunc);
  return getFunction(functionName.c_str(), builder.getInt1Ty(), builder.getPtrTy());
}

llvm::Function *StdFunctionManager::getIteratorNextFct(const Function *spiceFunc) const {
  const std::string functionName = NameMangling::mangleFunction(*spiceFunc);
  return getProcedure(functionName.c_str(), builder.getPtrTy());
}

llvm::Function *StdFunctionManager::getFunction(const char *funcName, llvm::Type *returnType, llvm::ArrayRef<llvm::Type *> args,
                                                bool varArg /*=false*/) const {
  llvm::Function *opFct = module->getFunction(funcName);
  if (opFct != nullptr)
    return opFct;
  llvm::FunctionType *opFctTy = llvm::FunctionType::get(returnType, args, varArg);
  module->getOrInsertFunction(funcName, opFctTy);
  return module->getFunction(funcName);
}

llvm::Function *StdFunctionManager::getProcedure(const char *procName, llvm::ArrayRef<llvm::Type *> args) const {
  return getFunction(procName, builder.getVoidTy(), args, false);
}

} // namespace spice::compiler
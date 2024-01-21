// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "StdFunctionManager.h"

#include <irgenerator/NameMangling.h>
#include <model/Function.h>

namespace spice::compiler {

llvm::Function *StdFunctionManager::getPrintfFct() const {
  llvm::Function *printfFct = getFunction("printf", builder.getInt32Ty(), builder.getInt8PtrTy(), true);
  // Set attributes
  printfFct->addFnAttr(llvm::Attribute::NoFree);
  printfFct->addFnAttr(llvm::Attribute::NoUnwind);
  printfFct->addParamAttr(0, llvm::Attribute::NoCapture);
  printfFct->addParamAttr(0, llvm::Attribute::NoUndef);
  printfFct->addParamAttr(0, llvm::Attribute::ReadOnly);
  printfFct->addRetAttr(llvm::Attribute::NoUndef);
  return printfFct;
}

llvm::Function *StdFunctionManager::getExitFct() const {
  llvm::Function *exitFct = getProcedure("exit", builder.getInt32Ty());
  // Set attributes
  exitFct->addFnAttr(llvm::Attribute::Cold);
  exitFct->addFnAttr(llvm::Attribute::NoReturn);
  exitFct->addFnAttr(llvm::Attribute::NoUnwind);
  return exitFct;
}

llvm::Function *StdFunctionManager::getFreeFct() const {
  llvm::Function *freeFct = getProcedure("free", builder.getPtrTy());
  // Set attributes
  freeFct->addFnAttr(llvm::Attribute::NoUnwind);
  freeFct->addParamAttr(0, llvm::Attribute::NoCapture);
  freeFct->addParamAttr(0, llvm::Attribute::NoUndef);
  freeFct->addParamAttr(0, llvm::Attribute::ReadOnly);
  return freeFct;
}

llvm::Function *StdFunctionManager::getMemcmpFct() const {
  llvm::Type *ptrTy = builder.getPtrTy();
  llvm::Function *memcmpFct = getFunction("memcmp", builder.getInt32Ty(), {ptrTy, ptrTy, builder.getInt64Ty()});
  // Set attributes
  memcmpFct->addFnAttr(llvm::Attribute::NoUnwind);
  return memcmpFct;
}

llvm::Function *StdFunctionManager::getMemcpyIntrinsic() const {
  llvm::Type *ptrTy = builder.getPtrTy();
  llvm::Function *memcpyFct = getProcedure("llvm.memcpy.p0.p0.i64", {ptrTy, ptrTy, builder.getInt64Ty(), builder.getInt1Ty()});
  // Set attributes
  memcpyFct->addFnAttr(llvm::Attribute::NoCallback);
  memcpyFct->addFnAttr(llvm::Attribute::NoFree);
  memcpyFct->addFnAttr(llvm::Attribute::NoUnwind);
  memcpyFct->addFnAttr(llvm::Attribute::WillReturn);
  return memcpyFct;
}

llvm::Function *StdFunctionManager::getStringGetRawLengthStringFct() const {
  const ParamList paramLst = {{SymbolType(TY_STRING), false}};
  const Function function("getRawLength", nullptr, SymbolType(TY_DYN), SymbolType(TY_LONG), paramLst, {}, nullptr);
  const std::string mangledName = NameMangling::mangleFunction(function);
  return getFunction(mangledName.c_str(), builder.getInt64Ty(), {builder.getPtrTy()});
}

llvm::Function *StdFunctionManager::getStringIsRawEqualStringStringFct() const {
  const ParamList paramLst = {{SymbolType(TY_STRING), false}, {SymbolType(TY_STRING), false}};
  const Function function("isRawEqual", nullptr, SymbolType(TY_DYN), SymbolType(TY_BOOL), paramLst, {}, nullptr);
  const std::string mangledName = NameMangling::mangleFunction(function);
  return getFunction(mangledName.c_str(), builder.getInt1Ty(), {builder.getPtrTy(), builder.getPtrTy()});
}

llvm::Function *StdFunctionManager::getDeallocBytePtrRefFct() const {
  const ParamList paramLst = {{SymbolType(TY_BYTE).toPointer(nullptr).toReference(nullptr), false}};
  const Function function("sDealloc", nullptr, SymbolType(TY_DYN), SymbolType(TY_DYN), paramLst, {}, nullptr);
  const std::string mangledName = NameMangling::mangleFunction(function);
  return getProcedure(mangledName.c_str(), {builder.getPtrTy()});
}

llvm::Function *StdFunctionManager::getIteratorFct(const Function *spiceFunc) const {
  const std::string functionName = NameMangling::mangleFunction(*spiceFunc);
  llvm::Type *iteratorType = spiceFunc->returnType.toLLVMType(context, nullptr);
  return getFunction(functionName.c_str(), iteratorType, builder.getPtrTy());
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
  // Check if function already exists in the current module
  llvm::Function *opFct = module->getFunction(funcName);
  if (opFct != nullptr)
    return opFct;

  // Add function to the current module
  llvm::FunctionType *opFctTy = llvm::FunctionType::get(returnType, args, varArg);
  module->getOrInsertFunction(funcName, opFctTy);
  return module->getFunction(funcName);
}

llvm::Function *StdFunctionManager::getProcedure(const char *procName, llvm::ArrayRef<llvm::Type *> args) const {
  return getFunction(procName, builder.getVoidTy(), args, false);
}

} // namespace spice::compiler
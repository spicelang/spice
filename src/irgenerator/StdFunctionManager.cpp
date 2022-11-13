// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "StdFunctionManager.h"

#include <model/Function.h>

llvm::StructType *StdFunctionManager::getStrobjType(llvm::LLVMContext &context) {
  std::string structTypeName = "_s__String__charptr_long_long";
  llvm::StructType *structType = llvm::StructType::getTypeByName(context, structTypeName);
  if (structType != nullptr)
    return structType;
  llvm::Type *ptrTy = llvm::PointerType::get(context, 0);
  llvm::Type *int64Ty = llvm::IntegerType::getInt64Ty(context);
  return llvm::StructType::create(context, {ptrTy, int64Ty, int64Ty}, structTypeName);
}

llvm::Function *StdFunctionManager::getPrintfFct() const {
  return getFunction("printf", builder.getInt32Ty(), builder.getInt8PtrTy(), true);
}

llvm::Function *StdFunctionManager::getExitFct() const { return getProcedure("exit", builder.getInt32Ty()); }

llvm::Function *StdFunctionManager::getStackSaveIntrinsic() const {
  return getFunction("llvm.stacksave", builder.getPtrTy(), {});
}

llvm::Function *StdFunctionManager::getStackRestoreIntrinsic() const {
  return getProcedure("llvm.stackrestore", builder.getInt8PtrTy());
}

llvm::Function *StdFunctionManager::getMemcpyIntrinsic() const {
  llvm::Type *ptrTy = builder.getPtrTy();
  return getProcedure("llvm.memcpy.p0.p0.i64", {ptrTy, ptrTy, builder.getInt64Ty(), builder.getInt1Ty()});
}

llvm::Function *StdFunctionManager::getStringGetRawFct() const {
  return getFunction("_mf__String__charptr__getRaw", builder.getPtrTy(), getStrobjType(context)->getPointerTo());
}

llvm::Function *StdFunctionManager::getStringCtorStrobjFct() const {
  return getProcedure("_mp__String__void__ctor__String", {builder.getPtrTy(), getStrobjType(context)});
}

llvm::Function *StdFunctionManager::getStringCtorStrobjStringFct() const {
  return getProcedure("_mp__String__void__ctor__String_string", {builder.getPtrTy(), getStrobjType(context), builder.getPtrTy()});
}

llvm::Function *StdFunctionManager::getStringCtorStrobjStrobjFct() const {
  return getProcedure("_mp__String__void__ctor__String_String",
                      {builder.getPtrTy(), getStrobjType(context), getStrobjType(context)});
}

llvm::Function *StdFunctionManager::getStringIsRawEqualStringStringFct() const {
  return getFunction("_f__void__bool__isRawEqual__string_string", builder.getInt1Ty(), {builder.getPtrTy(), builder.getPtrTy()});
}

llvm::Function *StdFunctionManager::getStringIsEqualStrobjFct() const {
  return getFunction("_mf__String__bool__isEqual__String", builder.getInt1Ty(), {builder.getPtrTy(), getStrobjType(context)});
}

llvm::Function *StdFunctionManager::getStringAppendStringFct() const {
  return getProcedure("_mp__String__void__append__string", {builder.getPtrTy(), builder.getPtrTy()});
}

llvm::Function *StdFunctionManager::getStringAppendCharFct() const {
  return getProcedure("_mp__String__void__append__char", {builder.getPtrTy(), builder.getInt8Ty()});
}

llvm::Function *StdFunctionManager::getStringMulOpIntFct() const {
  return getProcedure("_mp__String__void__opMul__int", {builder.getPtrTy(), builder.getInt32Ty()});
}

llvm::Function *StdFunctionManager::getStringMulOpLongFct() const {
  return getProcedure("_mp__String__void__opMul__long", {builder.getPtrTy(), builder.getInt64Ty()});
}

llvm::Function *StdFunctionManager::getStringMulOpShortFct() const {
  return getProcedure("_mp__String__void__opMul__short", {builder.getPtrTy(), builder.getInt16Ty()});
}

llvm::Function *StdFunctionManager::getFunction(const std::string &funcName, llvm::Type *returnType,
                                                llvm::ArrayRef<llvm::Type *> args, bool varArg) const {
  llvm::Function *opFct = module->getFunction(funcName);
  if (opFct != nullptr)
    return opFct;
  llvm::FunctionType *opFctTy = llvm::FunctionType::get(returnType, args, varArg);
  module->getOrInsertFunction(funcName, opFctTy);
  return module->getFunction(funcName);
}

llvm::Function *StdFunctionManager::getProcedure(const std::string &procName, llvm::ArrayRef<llvm::Type *> args,
                                                 bool varArg) const {
  return getFunction(procName, builder.getVoidTy(), args, varArg);
}
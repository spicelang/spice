// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "StdFunctionManager.h"

#include <generator/GeneratorVisitor.h>

StdFunctionManager::StdFunctionManager(GeneratorVisitor *generator) {
  context = generator->context;
  builder = generator->builder;
  module = generator->module.get();
}

llvm::StructType *StdFunctionManager::getStringStructType(llvm::LLVMContext &context) {
  std::string structTypeName = "_s__String__charptr_long_long";
  llvm::StructType *structType = llvm::StructType::getTypeByName(context, structTypeName);
  if (structType != nullptr)
    return structType;
  llvm::Type *ptrTy = llvm::PointerType::get(context, 0);
  llvm::Type *int64Ty = llvm::IntegerType::getInt64Ty(context);
  return llvm::StructType::create(context, {ptrTy, int64Ty, int64Ty}, structTypeName);
}

llvm::Function *StdFunctionManager::getPrintfFct() const {
  return getFunction("printf", builder->getInt32Ty(), builder->getInt8PtrTy(), true);
}

llvm::Function *StdFunctionManager::getExitFct() const { return getProcedure("exit", builder->getInt32Ty()); }

llvm::Function *StdFunctionManager::getStackSaveIntrinsic() const {
  return getFunction("llvm.stacksave", builder->getPtrTy(), {});
}

llvm::Function *StdFunctionManager::getStackRestoreIntrinsic() const {
  return getProcedure("llvm.stackrestore", builder->getInt8PtrTy());
}

llvm::Function *StdFunctionManager::getStringGetRawFct() const {
  return getFunction("_mf__String__getRaw", builder->getPtrTy(), getStringStructType(*context)->getPointerTo());
}

llvm::Function *StdFunctionManager::getStringCtorCharFct() const {
  return getProcedure("_mp__String__ctor__char", {builder->getPtrTy(), builder->getInt8Ty()});
}

llvm::Function *StdFunctionManager::getStringCtorStringFct() const {
  return getProcedure("_mp__String__ctor__string", {builder->getPtrTy(), builder->getPtrTy()});
}

llvm::Function *StdFunctionManager::getStringCtorStringStringFct() const {
  return getProcedure("_mp__String__ctor__string_string", {builder->getPtrTy(), builder->getPtrTy(), builder->getPtrTy()});
}

llvm::Function *StdFunctionManager::getStringDtorFct() const { return getProcedure("_mp__String__dtor", builder->getPtrTy()); }

llvm::Function *StdFunctionManager::getStringIsRawEqualStringStringFct() const {
  return getFunction("_mf__isRawEqual__string_string", builder->getInt1Ty(), {builder->getPtrTy(), builder->getPtrTy()});
}

llvm::Function *StdFunctionManager::getStringAppendStringFct() const {
  return getProcedure("_mp__String__append__string", {builder->getPtrTy(), builder->getPtrTy()});
}

llvm::Function *StdFunctionManager::getStringAppendCharFct() const {
  return getProcedure("_mp__String__append__char", {builder->getPtrTy(), builder->getInt8Ty()});
}

llvm::Function *StdFunctionManager::getStringMulOpIntFct() const {
  return getProcedure("_mp__String__opMul__int", {builder->getPtrTy(), builder->getInt32Ty()});
}

llvm::Function *StdFunctionManager::getStringMulOpLongFct() const {
  return getProcedure("_mp__String__opMul__long", {builder->getPtrTy(), builder->getInt64Ty()});
}

llvm::Function *StdFunctionManager::getStringMulOpShortFct() const {
  return getProcedure("_mp__String__opMul__short", {builder->getPtrTy(), builder->getInt16Ty()});
}

llvm::Function *StdFunctionManager::getFunction(const std::string functionName, llvm::Type *returnType,
                                                llvm::ArrayRef<llvm::Type *> args, bool varArg) const {
  llvm::Function *opFct = module->getFunction(functionName);
  if (opFct != nullptr)
    return opFct;
  llvm::FunctionType *opFctTy = llvm::FunctionType::get(returnType, args, varArg);
  module->getOrInsertFunction(functionName, opFctTy);
  return module->getFunction(functionName);
}

llvm::Function *StdFunctionManager::getProcedure(const std::string procedureName, llvm::ArrayRef<llvm::Type *> args,
                                                 bool varArg) const {
  return getFunction(procedureName, builder->getVoidTy(), args, varArg);
}
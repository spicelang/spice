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
  llvm::Function *printfFct = getFunction("printf", builder.getInt32Ty(), builder.getInt8PtrTy(), true);
  // Set noundef attribute to template string
  printfFct->addParamAttr(0, llvm::Attribute::NoUndef);
  return printfFct;
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

llvm::Function *StdFunctionManager::getPthreadCreate() const {
  llvm::Type *voidPtrTy = builder.getInt8PtrTy();
  llvm::FunctionType *threadFctTy = llvm::FunctionType::get(voidPtrTy, voidPtrTy, false);
  std::vector<llvm::Type *> argTypes = {voidPtrTy->getPointerTo(), voidPtrTy, threadFctTy->getPointerTo(), voidPtrTy};
  return getFunction("pthread_create", builder.getInt32Ty(), argTypes);
}

llvm::Function *StdFunctionManager::getPthreadSelf() const { return getFunction("pthread_self", builder.getPtrTy(), {}); }

llvm::Function *StdFunctionManager::getPthreadJoin() const {
  return getFunction("pthread_join", builder.getInt32Ty(), {builder.getPtrTy(), builder.getPtrTy()});
}

llvm::Function *StdFunctionManager::getStringIsRawEqualStringStringFct() const {
  return getFunction("_f__void__bool__isRawEqual__string_string", builder.getInt1Ty(), {builder.getPtrTy(), builder.getPtrTy()});
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

llvm::Function *StdFunctionManager::getProcedure(const char *procName, llvm::ArrayRef<llvm::Type *> args,
                                                 bool varArg /*=false*/) const {
  return getFunction(procName, builder.getVoidTy(), args, varArg);
}
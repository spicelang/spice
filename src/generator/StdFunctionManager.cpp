// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "StdFunctionManager.h"

#include <generator/GeneratorVisitor.h>

StdFunctionManager::StdFunctionManager(GeneratorVisitor *generator) : generator(generator) {
  context = generator->context.get();
  builder = generator->builder.get();
  module = generator->module.get();
}

llvm::StructType *StdFunctionManager::getStringStructType() const {
  std::string structName = "_s__String__charptr_long_long";
  llvm::Type *ptrTy = builder->getPtrTy();
  llvm::Type *int64Ty = builder->getInt64Ty();
  return llvm::StructType::create(*context, {ptrTy, int64Ty, int64Ty}, structName);
}

llvm::Function *StdFunctionManager::getPrintfFct() const {
  std::string printfFctName = "printf";
  llvm::Function *printfFct = module->getFunction(printfFctName);
  if (printfFct)
    return printfFct;
  // Not found -> declare it for linkage
  llvm::FunctionType *printfFctTy = llvm::FunctionType::get(builder->getInt32Ty(), builder->getInt8PtrTy(), true);
  module->getOrInsertFunction(printfFctName, printfFctTy);
  return module->getFunction(printfFctName);
}

llvm::Function *StdFunctionManager::getExitFct() const {
  std::string exitFctName = "exit";
  llvm::Function *exitFct = module->getFunction(exitFctName);
  if (exitFct)
    return exitFct;
  // Not found -> declare it for linkage
  llvm::FunctionType *exitFctTy = llvm::FunctionType::get(builder->getVoidTy(), builder->getInt32Ty(), false);
  module->getOrInsertFunction(exitFctName, exitFctTy);
  return module->getFunction(exitFctName);
}

llvm::Function *StdFunctionManager::getStackSaveIntrinsic() const {
  std::string stackSaveFctName = "llvm.stacksave";
  llvm::Function *stackSaveFct = module->getFunction(stackSaveFctName);
  if (stackSaveFct)
    return stackSaveFct;
  // Not found -> declare it for linkage
  llvm::FunctionType *stackSaveFctTy = llvm::FunctionType::get(builder->getInt8PtrTy(), {}, false);
  module->getOrInsertFunction(stackSaveFctName, stackSaveFctTy);
  return module->getFunction(stackSaveFctName);
}

llvm::Function *StdFunctionManager::getStackRestoreIntrinsic() const {
  std::string stackRestoreFctName = "llvm.stackrestore";
  llvm::Function *stackRestoreFct = module->getFunction(stackRestoreFctName);
  if (stackRestoreFct)
    return stackRestoreFct;
  // Not found -> declare it for linkage
  llvm::FunctionType *stackRestoreFctTy = llvm::FunctionType::get(builder->getVoidTy(), builder->getInt8PtrTy(), false);
  module->getOrInsertFunction(stackRestoreFctName, stackRestoreFctTy);
  return module->getFunction(stackRestoreFctName);
}

llvm::Function *StdFunctionManager::getStringGetRawFct() const {
  std::string functionName = "_mf__String__getRaw";
  llvm::Function *opFct = module->getFunction(functionName);
  if (opFct)
    return opFct;
  llvm::Type *structTyPtr = getStringStructType()->getPointerTo();
  llvm::Type *stringTy = builder->getPtrTy();
  llvm::FunctionType *opFctTy = llvm::FunctionType::get(stringTy, structTyPtr, false);
  module->getOrInsertFunction(functionName, opFctTy);
  return module->getFunction(functionName);
}

llvm::Function *StdFunctionManager::getStringCtorStringFct() const {
  std::string functionName = "_mp__String__ctor__string";
  llvm::Function *opFct = module->getFunction(functionName);
  if (opFct != nullptr)
    return opFct;
  llvm::Type *ptrTy = builder->getPtrTy();
  llvm::FunctionType *opFctTy = llvm::FunctionType::get(builder->getVoidTy(), {ptrTy, ptrTy}, false);
  module->getOrInsertFunction(functionName, opFctTy);
  return module->getFunction(functionName);
}

llvm::Function *StdFunctionManager::getStringCtorStringStringFct() const {
  std::string functionName = "_mp__String__ctor__string_string";
  llvm::Function *opFct = module->getFunction(functionName);
  if (opFct != nullptr)
    return opFct;
  llvm::Type *ptrTy = builder->getPtrTy();
  llvm::FunctionType *opFctTy = llvm::FunctionType::get(builder->getVoidTy(), {ptrTy, ptrTy, ptrTy}, false);
  module->getOrInsertFunction(functionName, opFctTy);
  return module->getFunction(functionName);
}

llvm::Function *StdFunctionManager::getStringIsRawEqualStringStringFct() const {
  std::string functionName = "_mf__isRawEqual__string_string";
  llvm::Function *opFct = module->getFunction(functionName);
  if (opFct != nullptr)
    return opFct;
  llvm::Type *ptrTy = builder->getPtrTy();
  llvm::FunctionType *opFctTy = llvm::FunctionType::get(builder->getInt1Ty(), {ptrTy, ptrTy}, false);
  module->getOrInsertFunction(functionName, opFctTy);
  return module->getFunction(functionName);
}

llvm::Function *StdFunctionManager::getStringAppendStringFct() const {
  std::string functionName = "_mp__String__append__string";
  llvm::Function *opFct = module->getFunction(functionName);
  if (opFct != nullptr)
    return opFct;
  llvm::Type *ptrTy = builder->getPtrTy();
  llvm::FunctionType *opFctTy = llvm::FunctionType::get(builder->getVoidTy(), {ptrTy, ptrTy}, false);
  module->getOrInsertFunction(functionName, opFctTy);
  return module->getFunction(functionName);
}

llvm::Function *StdFunctionManager::getStringAppendCharFct() const {
  std::string functionName = "_mp__String__append__char";
  llvm::Function *opFct = module->getFunction(functionName);
  if (opFct != nullptr)
    return opFct;
  llvm::Type *ptrTy = builder->getPtrTy();
  llvm::Type *charTy = builder->getInt8Ty();
  llvm::FunctionType *opFctTy = llvm::FunctionType::get(builder->getVoidTy(), {ptrTy, charTy}, false);
  module->getOrInsertFunction(functionName, opFctTy);
  return module->getFunction(functionName);
}

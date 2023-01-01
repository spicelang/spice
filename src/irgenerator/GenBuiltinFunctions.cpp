// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>

namespace spice::compiler {

std::any IRGenerator::visitPrintfCall(const PrintfCallNode *node) {
  // Retrieve printf function
  llvm::Function *printfFct = stdFunctionManager.getPrintfFct();

  // Push the template string as first argument
  std::vector<llvm::Value *> printfArgs;
  const std::string globalStringName = getUnusedGlobalName("printf.str.");
  printfArgs.push_back(builder.CreateGlobalStringPtr(node->templatedString, globalStringName));

  // Collect replacement arguments
  for (AssignExprNode *arg : node->args()) {
    // Retrieve type of argument
    const SymbolType argSymbolType = arg->getEvaluatedSymbolType(manIdx);
    llvm::Type *argType = argSymbolType.toLLVMType(context, currentScope);

    // Re-map some values
    llvm::Value *argVal;
    if (argSymbolType.isArray()) {
      llvm::Value *argValPtr = resolveAddress(arg);
      llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(0)};
      argVal = builder.CreateInBoundsGEP(argType, argValPtr, indices);
    } else if (argSymbolType.is(TY_STRUCT, STROBJ_NAME)) {
      // ToDo: implement
      // argVal = materializeString(resolveAddress(arg));
    } else {
      argVal = resolveValue(arg);
    }

    // Extend all integer types lower than 32 bit to 32 bit
    if (argVal->getType()->isIntegerTy(8) || argVal->getType()->isIntegerTy(16))
      argVal = builder.CreateSExt(argVal, llvm::Type::getInt32Ty(context));
    else if (argVal->getType()->isIntegerTy(1))
      argVal = builder.CreateZExt(argVal, llvm::Type::getInt32Ty(context));

    printfArgs.push_back(argVal);
  }

  // Call printf function
  llvm::CallInst *returnValue = builder.CreateCall(printfFct, printfArgs);

  // Add noundef attribute to template string
  returnValue->addParamAttr(0, llvm::Attribute::NoUndef);

  return ExprResult{.value = returnValue};
}

std::any IRGenerator::visitSizeofCall(const SizeofCallNode *node) {
  llvm::Type *type;
  if (node->isType) { // Size of type
    type = any_cast<llvm::Type *>(visit(node->dataType()));
  } else { // Size of value
    type = node->assignExpr()->getEvaluatedSymbolType(manIdx).toLLVMType(context, currentScope);
  }
  // Calculate size at compile-time
  const unsigned int size = module->getDataLayout().getTypeSizeInBits(type);

  // Return size value
  llvm::Value *sizeValue = builder.getInt32(size);
  return ExprResult{.value = sizeValue};
}

std::any IRGenerator::visitLenCall(const LenCallNode *node) {
  // Check if the length is fixed and known via the symbol type
  const SymbolType assignExprSymbolType = node->assignExpr()->getEvaluatedSymbolType(manIdx);
  assert(assignExprSymbolType.isArray() && assignExprSymbolType.getArraySize() != ARRAY_SIZE_UNKNOWN);
  llvm::Value *lengthValue = builder.getInt32(assignExprSymbolType.getArraySize());
  return ExprResult{.value = lengthValue};
}

std::any IRGenerator::visitTidCall(const TidCallNode *node) {
  // Retrieve LLVM function
  llvm::Function *psFct = stdFunctionManager.getPthreadSelf();

  // Create call to function
  llvm::Value *threadIdValue = builder.CreateCall(psFct);
  threadIdValue = builder.CreatePtrToInt(threadIdValue, builder.getInt32Ty());

  // Return thread id value
  return ExprResult{.value = threadIdValue};
}

std::any IRGenerator::visitJoinCall(const JoinCallNode *node) {
  // Retrieve LLVM function
  llvm::Function *joinFct = stdFunctionManager.getPthreadJoin();
  llvm::Value *voidPtrPtrNull = llvm::Constant::getNullValue(llvm::Type::getInt8PtrTy(context)->getPointerTo());

  std::vector<llvm::Value *> threadIdPointers;
  for (AssignExprNode *assignExpr : node->assignExpressions()) {
    // Get thread id
    llvm::Value *threadIdPtr = resolveAddress(assignExpr);
    assert(threadIdPtr != nullptr && threadIdPtr->getType()->isPointerTy());

    // Check if we have a single thread to join or an array of thread ids
    const SymbolType assignExprSymbolType = assignExpr->getEvaluatedSymbolType(manIdx);
    if (assignExprSymbolType.isArray()) { // Multiple ids
      assert(assignExprSymbolType.getArraySize() != ARRAY_SIZE_UNKNOWN);
      llvm::Type *threadIdPtrTy = assignExprSymbolType.toLLVMType(context, currentScope);
      for (int i = 0; i < threadIdPtrTy->getArrayNumElements(); i++) {
        // Get thread id that has to be joined
        llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(i)};
        threadIdPtr = builder.CreateGEP(threadIdPtrTy, threadIdPtr, indices);
        threadIdPointers.push_back(threadIdPtr);
      }
    } else { // Single id
      threadIdPointers.push_back(threadIdPtr);
    }
  }

  // Create a call to pthread_join for each thread id pointer
  for (llvm::Value *threadIdPtr : threadIdPointers) {
    // Load thread id that has to be joined
    llvm::Value *threadId = builder.CreateLoad(builder.getInt8PtrTy(), threadIdPtr);
    // Create call to pthread_join
    builder.CreateCall(joinFct, {threadId, voidPtrPtrNull});
  }

  // Return join count value
  llvm::Value *joinCountValue = builder.getInt32(threadIdPointers.size());
  return ExprResult{.value = joinCountValue};
}

} // namespace spice::compiler
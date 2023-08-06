// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>

namespace spice::compiler {

std::any IRGenerator::visitPrintfCall(const PrintfCallNode *node) {
  // Retrieve printf function
  llvm::Function *printfFct = stdFunctionManager.getPrintfFct();

  // Push the template string as first argument
  std::vector<llvm::Value *> printfArgs;
  llvm::Constant *templateString = createGlobalStringConst("printf.str.", node->templatedString, node->codeLoc);
  printfArgs.push_back(templateString);

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
    } else if (argSymbolType.getBaseType().isStringObj()) {
      llvm::Value *argValPtr = resolveAddress(arg);
      llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(0)};
      llvm::Type *argBaseType = argSymbolType.getBaseType().toLLVMType(context, currentScope);
      argValPtr = builder.CreateInBoundsGEP(argBaseType, argValPtr, indices);
      argVal = builder.CreateLoad(builder.getPtrTy(), argValPtr);
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

  return LLVMExprResult{.value = returnValue};
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
  llvm::Value *sizeValue = builder.getInt64(size);
  return LLVMExprResult{.value = sizeValue};
}

std::any IRGenerator::visitAlignofCall(const AlignofCallNode *node) {
  llvm::Type *type;
  if (node->isType) { // Align of type
    type = any_cast<llvm::Type *>(visit(node->dataType()));
  } else { // Align of value
    type = node->assignExpr()->getEvaluatedSymbolType(manIdx).toLLVMType(context, currentScope);
  }
  // Calculate size at compile-time
  const llvm::Align align = module->getDataLayout().getABITypeAlign(type);

  // Return align value
  llvm::Value *sizeValue = builder.getInt64(align.value());
  return LLVMExprResult{.value = sizeValue};
}

std::any IRGenerator::visitLenCall(const LenCallNode *node) {
  // Check if the length is fixed and known via the symbol type
  SymbolType assignExprSymbolType = node->assignExpr()->getEvaluatedSymbolType(manIdx);
  assignExprSymbolType = assignExprSymbolType.removeReferenceWrapper();
  assert(assignExprSymbolType.isArray() && assignExprSymbolType.getArraySize() != ARRAY_SIZE_UNKNOWN);

  // Return length value
  llvm::Value *lengthValue = builder.getInt64(assignExprSymbolType.getArraySize());
  return LLVMExprResult{.value = lengthValue};
}

} // namespace spice::compiler
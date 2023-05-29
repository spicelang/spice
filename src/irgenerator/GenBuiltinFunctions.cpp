// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

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
  llvm::Value *sizeValue = builder.getInt64(size);
  return ExprResult{.value = sizeValue};
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
  return ExprResult{.value = sizeValue};
}

std::any IRGenerator::visitLenCall(const LenCallNode *node) {
  // Check if the length is fixed and known via the symbol type
  const SymbolType assignExprSymbolType = node->assignExpr()->getEvaluatedSymbolType(manIdx);
  assert(assignExprSymbolType.isArray() && assignExprSymbolType.getArraySize() != ARRAY_SIZE_UNKNOWN);

  // Return length value
  llvm::Value *lengthValue = builder.getInt64(assignExprSymbolType.getArraySize());
  return ExprResult{.value = lengthValue};
}

} // namespace spice::compiler
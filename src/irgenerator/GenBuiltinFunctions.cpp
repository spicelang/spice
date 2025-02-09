// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>
#include <llvm/IR/InlineAsm.h>

#include <llvm/IR/Module.h>

namespace spice::compiler {

std::any IRGenerator::visitBuiltinCall(const BuiltinCallNode *node) {
  if (node->printfCall)
    return visit(node->printfCall);
  if (node->sizeofCall)
    return visit(node->sizeofCall);
  if (node->alignofCall)
    return visit(node->alignofCall);
  if (node->lenCall)
    return visit(node->lenCall);
  if (node->panicCall)
    return visit(node->panicCall);
  if (node->sysCall)
    return visit(node->sysCall);
  assert_fail("Unknown builtin call"); // LCOV_EXCL_LINE
  return nullptr;                      // LCOV_EXCL_LINE
}

std::any IRGenerator::visitPrintfCall(const PrintfCallNode *node) {
  // Retrieve printf function
  llvm::Function *printfFct = stdFunctionManager.getPrintfFct();

  // Push the template string as first argument
  std::vector<llvm::Value *> printfArgs;
  llvm::Constant *templateString = createGlobalStringConst("printf.str.", node->templatedString, node->codeLoc);
  printfArgs.push_back(templateString);

  // Collect replacement arguments
  for (const AssignExprNode *arg : node->args) {
    // Retrieve type of argument
    const QualType argSymbolType = arg->getEvaluatedSymbolType(manIdx);

    // Re-map some values
    llvm::Value *argVal;
    if (argSymbolType.isArray()) {
      llvm::Value *argValPtr = resolveAddress(arg);
      llvm::Value *indices[2] = {builder.getInt64(0), builder.getInt32(0)};
      llvm::Type *argType = argSymbolType.toLLVMType(sourceFile);
      argVal = insertInBoundsGEP(argType, argValPtr, indices);
    } else if (argSymbolType.getBase().isStringObj()) {
      llvm::Value *argValPtr = resolveAddress(arg);
      llvm::Type *argBaseType = argSymbolType.getBase().toLLVMType(sourceFile);
      argValPtr = insertStructGEP(argBaseType, argValPtr, 0);
      argVal = insertLoad(builder.getPtrTy(), argValPtr);
    } else {
      argVal = resolveValue(arg);
    }

    // Extend all integer types lower than 32 bit to 32 bit
    if (argSymbolType.removeReferenceWrapper().isOneOf({TY_SHORT, TY_BYTE, TY_CHAR, TY_BOOL}))
      argVal = builder.CreateIntCast(argVal, builder.getInt32Ty(), argSymbolType.removeReferenceWrapper().isSigned());

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
    type = any_cast<llvm::Type *>(visit(node->dataType));
  } else { // Size of value
    type = node->assignExpr->getEvaluatedSymbolType(manIdx).toLLVMType(sourceFile);
  }
  // Calculate size at compile-time
  const llvm::TypeSize sizeInBits = module->getDataLayout().getTypeSizeInBits(type);

  // Return size value
  llvm::Value *sizeValue = builder.getInt64(sizeInBits);
  return LLVMExprResult{.value = sizeValue};
}

std::any IRGenerator::visitAlignofCall(const AlignofCallNode *node) {
  llvm::Type *type;
  if (node->isType) { // Align of type
    type = any_cast<llvm::Type *>(visit(node->dataType));
  } else { // Align of value
    type = node->assignExpr->getEvaluatedSymbolType(manIdx).toLLVMType(sourceFile);
  }
  // Calculate size at compile-time
  const llvm::Align align = module->getDataLayout().getABITypeAlign(type);

  // Return align value
  llvm::Value *sizeValue = builder.getInt64(align.value());
  return LLVMExprResult{.value = sizeValue};
}

std::any IRGenerator::visitLenCall(const LenCallNode *node) {
  // Check if the length is fixed and known via the symbol type
  QualType symbolType = node->assignExpr->getEvaluatedSymbolType(manIdx);
  symbolType = symbolType.removeReferenceWrapper();

  llvm::Value *lengthValue;
  if (symbolType.is(TY_STRING)) {
    llvm::Function *getRawLengthFct = stdFunctionManager.getStringGetRawLengthStringFct();
    lengthValue = builder.CreateCall(getRawLengthFct, resolveValue(node->assignExpr));
  } else {
    assert(symbolType.isArray() && symbolType.getArraySize() != ARRAY_SIZE_UNKNOWN);
    // Return length value
    lengthValue = builder.getInt64(symbolType.getArraySize());
  }
  return LLVMExprResult{.value = lengthValue};
}

std::any IRGenerator::visitPanicCall(const PanicCallNode *node) {
  // Get value for stderr
  llvm::PointerType *ptrTy = builder.getPtrTy();
  constexpr auto globalName = "stderr";
  module->getOrInsertGlobal(globalName, ptrTy);
  llvm::GlobalVariable *stdErrPtr = module->getNamedGlobal(globalName);
  stdErrPtr->setLinkage(llvm::GlobalVariable::ExternalLinkage);
  stdErrPtr->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Local);
  stdErrPtr->setAlignment(llvm::MaybeAlign(8));
  llvm::Value *stdErr = insertLoad(ptrTy, stdErrPtr);

  // Create constant for error message
  const std::string codeLoc = node->codeLoc.toPrettyString();
  const std::string templateStr = "Program panicked at " + codeLoc + ": %s\n" + node->getErrorMessage() + "\n";
  llvm::Constant *globalString = builder.CreateGlobalString(templateStr, getUnusedGlobalName(ANON_GLOBAL_STRING_NAME));

  // Get actual error message
  llvm::Value *errorObjPtr = resolveAddress(node->assignExpr);
  llvm::Type *errorObjTy = node->assignExpr->getEvaluatedSymbolType(manIdx).toLLVMType(sourceFile);
  llvm::Value *errorMessagePtr = insertStructGEP(errorObjTy, errorObjPtr, 1);
  llvm::Value *errorMessage = insertLoad(ptrTy, errorMessagePtr);

  // Print the error message to stderr
  llvm::Function *fprintfFct = stdFunctionManager.getFPrintfFct();
  builder.CreateCall(fprintfFct, {stdErr, globalString, errorMessage});

  // Generate call to exit()
  llvm::Function *exitFct = stdFunctionManager.getExitFct();
  builder.CreateCall(exitFct, builder.getInt32(EXIT_FAILURE));
  // Create unreachable instruction
  builder.CreateUnreachable();
  // Unreachable counts as terminator
  terminateBlock(node->getNextOuterStmtLst());

  return nullptr;
}

std::any IRGenerator::visitSysCall(const SysCallNode *node) {
  // Create assembly string
  static constexpr uint8_t NUM_REGS = 7;
  const char *asmString = getSysCallAsmString();
  const char *constraints = getSysCallConstraintString();

  // Create inline assembly
  llvm::Type *int64Ty = builder.getInt64Ty();
  llvm::Type *argTypes[NUM_REGS] = {int64Ty, int64Ty, int64Ty, int64Ty, int64Ty, int64Ty, int64Ty};
  llvm::FunctionType *fctType = llvm::FunctionType::get(builder.getVoidTy(), argTypes, false);
  llvm::InlineAsm *inlineAsm = llvm::InlineAsm::get(fctType, asmString, constraints, true);

  // Fill arguments array (first argument is syscall number)
  llvm::Value *argValues[NUM_REGS];
  for (unsigned short i = 0; i < NUM_REGS; i++) {
    if (i < node->args.size()) {
      const AssignExprNode *argNode = node->args.at(i);
      const QualType &argType = argNode->getEvaluatedSymbolType(manIdx);
      assert(argType.isOneOf({TY_INT, TY_LONG, TY_SHORT, TY_BOOL, TY_BYTE, TY_PTR, TY_STRING}));
      if (argType.isOneOf({TY_PTR, TY_STRING}))
        argValues[i] = builder.CreatePtrToInt(resolveValue(argNode), builder.getInt64Ty());
      else
        argValues[i] = builder.CreateZExt(resolveValue(argNode), builder.getInt64Ty());
    } else {
      argValues[i] = builder.getInt64(0);
    }
  }

  // Generate call
  llvm::Value *result = builder.CreateCall(inlineAsm, argValues);

  return LLVMExprResult{.value = result};
}

} // namespace spice::compiler
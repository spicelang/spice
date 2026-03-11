// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "IRGenerator.h"
#include "typechecker/TypeChecker.h"

#include <ast/ASTNodes.h>
#include <driver/Driver.h>
#include <global/TypeRegistry.h>
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
  if (node->typeidCall)
    return visit(node->typeidCall);
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
  llvm::CallInst *callInst = builder.CreateCall(printfFct, printfArgs);

  // Add noundef attribute to return value and all arguments
  callInst->addRetAttr(llvm::Attribute::NoUndef);
  for (size_t i = 0; i < printfArgs.size(); i++)
    callInst->addParamAttr(i, llvm::Attribute::NoUndef);

  return LLVMExprResult{.value = callInst};
}

std::any IRGenerator::visitSizeofCall(const SizeofCallNode *node) {
  llvm::Type *type;
  if (node->isType) { // Size of type
    type = any_cast<llvm::Type *>(visit(node->dataType));
  } else { // Size of value
    type = node->assignExpr->getEvaluatedSymbolType(manIdx).toLLVMType(sourceFile);
  }
  // Calculate size at compile-time
  const llvm::TypeSize sizeInBytes = module->getDataLayout().getTypeAllocSize(type);

  // Return size value
  llvm::Value *sizeValue = builder.getInt64(sizeInBytes);
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
  const llvm::Align alignmentInBytes = module->getDataLayout().getABITypeAlign(type);

  // Return align value
  llvm::Value *alignValue = builder.getInt64(alignmentInBytes.value());
  return LLVMExprResult{.value = alignValue};
}

std::any IRGenerator::visitTypeidCall(const TypeidCallNode *node) {
  // Return type id value
  const QualType qualType = node->assignExpr->getEvaluatedSymbolType(manIdx);
  const uint64_t typeId = TypeRegistry::getTypeHash(*qualType.getType());
  llvm::Value *typeIdValue = builder.getInt64(typeId);
  return LLVMExprResult{.value = typeIdValue};
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
  llvm::PointerType *ptrTy = builder.getPtrTy();

  // Get value for stderr
  llvm::Value *stdErr;
  if (cliOptions.targetTriple.isOSWindows()) {
    llvm::Function *getAcrtIOFuncFct = stdFunctionManager.getAcrtIOFuncFct();
    stdErr = builder.CreateCall(getAcrtIOFuncFct, {builder.getInt32(/*constant for stderr*/ 2)});
  } else {
    const char *globalName = cliOptions.targetTriple.isOSDarwin() ? "__stderrp" : "stderr";
    module->getOrInsertGlobal(globalName, ptrTy);
    llvm::GlobalVariable *stdErrPtr = module->getNamedGlobal(globalName);
    stdErrPtr->setLinkage(llvm::GlobalVariable::ExternalLinkage);
    stdErrPtr->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Local);
    stdErrPtr->setAlignment(llvm::MaybeAlign(8));
    stdErr = insertLoad(ptrTy, stdErrPtr);
  }

  // Create constant for error message
  const std::string codeLoc = node->codeLoc.toPrettyString();
  const std::string templateStr = "Program panicked at " + codeLoc + ": %s\n" + node->getErrorMessage() + "\n";
  llvm::GlobalVariable *globalString = builder.CreateGlobalString(templateStr, getUnusedGlobalName(ANON_GLOBAL_STRING_NAME));

  // If the output should be comparable, fix alignment to 4 bytes
  if (cliOptions.comparableOutput)
    globalString->setAlignment(llvm::Align(4));

  // Get actual error message
  llvm::Value *errorObjPtr = resolveAddress(node->assignExpr);
  llvm::Type *errorObjTy = node->assignExpr->getEvaluatedSymbolType(manIdx).toLLVMType(sourceFile);
  llvm::Value *errorMessagePtr = insertStructGEP(errorObjTy, errorObjPtr, 1);
  llvm::Value *errorMessage = insertLoad(ptrTy, errorMessagePtr);

  // Print the error message to stderr
  llvm::Function *fprintfFct = stdFunctionManager.getFPrintfFct();
  builder.CreateCall(fprintfFct, {stdErr, globalString, errorMessage});

  // Cleanup the scope before calling exit()
  // Unreachable below counts as terminator
  terminateBlock(node->getNextOuterStmtLst());

  // Generate call to exit()
  llvm::Function *exitFct = stdFunctionManager.getExitFct();
  builder.CreateCall(exitFct, builder.getInt32(EXIT_FAILURE));
  // Create unreachable instruction
  builder.CreateUnreachable();

  return nullptr;
}

std::any IRGenerator::visitSysCall(const SysCallNode *node) {
  // Determine the required number of operands.
  // (We assume at least one argument is provided: the syscall number.)
  const auto requiredRegs = static_cast<uint8_t>(node->args.size());
  assert(requiredRegs >= 1 && requiredRegs <= 6);

  // Create the asm and constraint strings based on the required number of registers.
  const std::string asmString = getSysCallAsmString(requiredRegs);
  const std::string constraints = getSysCallConstraintString(requiredRegs);

  // Create the LLVM function type for the inline asm with only the needed operands.
  llvm::Type *int64Ty = builder.getInt64Ty();
  const std::vector argTypes(requiredRegs, int64Ty);
  llvm::FunctionType *fctType = llvm::FunctionType::get(builder.getVoidTy(), argTypes, false);
  llvm::InlineAsm *inlineAsm = llvm::InlineAsm::get(fctType, asmString, constraints, true);

  // Build the argument list (each provided argument is converted to i64).
  std::vector<llvm::Value *> argValues;
  argValues.reserve(requiredRegs);
  for (uint8_t i = 0; i < requiredRegs; i++) {
    const AssignExprNode *argNode = node->args.at(i);
    const QualType &argType = argNode->getEvaluatedSymbolType(manIdx);
    assert(argType.isOneOf({TY_INT, TY_LONG, TY_SHORT, TY_BOOL, TY_BYTE, TY_PTR, TY_STRING}));
    if (argType.isOneOf({TY_PTR, TY_STRING}))
      argValues.push_back(builder.CreatePtrToInt(resolveValue(argNode), builder.getInt64Ty()));
    else
      argValues.push_back(builder.CreateZExt(resolveValue(argNode), builder.getInt64Ty()));
  }

  // Generate the call using only the required number of arguments.
  llvm::Value *result = builder.CreateCall(inlineAsm, argValues);

  return LLVMExprResult{.value = result};
}

std::any IRGenerator::visitNewBuiltinCall(const FctCallNode *node) const {
  if (node->fqFunctionName == BUILTIN_FCT_NAME_IS_SAME)
    return visitBuiltinCallIsSame(node);
  if (node->fqFunctionName == BUILTIN_FCT_NAME_IMPLEMENTS_INTERFACE)
    return visitBuiltinCallImplementsInterface(node);

  assert_fail("This builtin call is not implemented yet"); // LCOV_EXCL_LINE
  return nullptr;                                          // LCOV_EXCL_LINE
}

std::any IRGenerator::visitBuiltinCallIsSame(const FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_IS_SAME);

  // Get the types to compare
  bool isSame = true;
  assert(node->hasTemplateTypes && node->templateTypeLst->dataTypes.size() >= 2);
  const std::vector<DataTypeNode *> &dataTypeNodes = node->templateTypeLst->dataTypes;
  const QualType firstType = dataTypeNodes.front()->getEvaluatedSymbolType(manIdx);
  for (size_t i = 1; i < dataTypeNodes.size(); i++) {
    const QualType qualType = dataTypeNodes.at(i)->getEvaluatedSymbolType(manIdx);
    if (!qualType.matches(firstType, false, true, false)) {
      isSame = false;
      break;
    }
  }

  return LLVMExprResult{.value = builder.getInt1(isSame)};
}

std::any IRGenerator::visitBuiltinCallImplementsInterface(const FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_IMPLEMENTS_INTERFACE);

  // Get the types to compare
  const QualType interfaceType = node->templateTypeLst->dataTypes.at(0)->getEvaluatedSymbolType(manIdx);
  const QualType structType = node->templateTypeLst->dataTypes.at(1)->getEvaluatedSymbolType(manIdx);

  // The first type must be an interface and the second type must be a struct that implements this interface
  const bool implementsInterface =
      interfaceType.is(TY_INTERFACE) && structType.is(TY_STRUCT) && structType.doesImplement(interfaceType, node);
  return LLVMExprResult{.value = builder.getInt1(implementsInterface)};
}

} // namespace spice::compiler
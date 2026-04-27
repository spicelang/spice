// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>
#include <driver/Driver.h>
#include <global/GlobalResourceManager.h>
#include <global/TypeRegistry.h>
#include <typechecker/Builtins.h>
#include <typechecker/TypeChecker.h>

#include <llvm/IR/InlineAsm.h>
#include <llvm/IR/Module.h>

namespace spice::compiler {

std::any IRGenerator::visitBuiltinCall(const FctCallNode *node) {
  // If we have a compile time value, but the computation is still there, we can simply use this constant value
  if (node->hasCompileTimeValue(manIdx)) {
    llvm::Constant *value = getConst(node->getCompileTimeValue(manIdx), node->getEvaluatedSymbolType(manIdx), node);
    return LLVMExprResult{.constant = value};
  }

  // If we need to perform runtime actions, call the specified IRGenerator delegate
  assert(BUILTIN_FUNCTIONS_MAP.contains(node->fqFunctionName) && "Builtin function not implemented!");
  const BuiltinFunctionInfo &info = BUILTIN_FUNCTIONS_MAP.find(node->fqFunctionName)->second;
  assert(info.irGeneratorVisitMethod != nullptr);
  return (this->*info.irGeneratorVisitMethod)(node);
}

std::any IRGenerator::visitBuiltinPrintfCall(const FctCallNode *node) {
  // Retrieve templated string
  assert(node->hasArgs);
  const ExprNode *firstArg = node->argLst->args.front();
  assert(firstArg->getEvaluatedSymbolType(manIdx).is(TY_STRING) && firstArg->hasCompileTimeValue(manIdx));
  const size_t stringOffset = firstArg->getCompileTimeValue(manIdx).stringValueOffset;
  const std::string templatedString = resourceManager.compileTimeStringValues.at(stringOffset);

  // Push the template string as first argument
  std::vector<llvm::Value *> printfArgs;
  llvm::Constant *templateString = createGlobalStringConst("printf.str.", templatedString, node->codeLoc);
  printfArgs.push_back(templateString);

  // Collect replacement arguments
  for (size_t argIdx = 1; argIdx < node->argLst->args.size(); argIdx++) {
    const ExprNode *arg = node->argLst->args.at(argIdx);
    // Retrieve type of argument
    const QualType argSymbolType = arg->getEvaluatedSymbolType(manIdx);

    // Re-map some values
    llvm::Value *argVal;
    if (argSymbolType.isArray()) {
      // ToDo: Check if GEP can be removed
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
  llvm::Function *printfFct = stdFunctionManager.getPrintfFct();
  llvm::CallInst *callInst = builder.CreateCall(printfFct, printfArgs);

  // Add noundef attribute to return value and all arguments
  callInst->addRetAttr(llvm::Attribute::NoUndef);
  for (size_t i = 0; i < printfArgs.size(); i++)
    callInst->addParamAttr(i, llvm::Attribute::NoUndef);

  return LLVMExprResult{.value = callInst};
}

std::any IRGenerator::visitBuiltinLenCall(const FctCallNode *node) {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_LEN);

  // Check if the length is fixed and known via the symbol type
  const ExprNode *argNode = node->argLst->args.front();
  QualType symbolType = argNode->getEvaluatedSymbolType(manIdx);
  symbolType = symbolType.removeReferenceWrapper();
  assert(symbolType.is(TY_STRING));

  llvm::Function *getRawLengthFct = stdFunctionManager.getStringGetRawLengthStringFct();
  llvm::Value *lengthValue = builder.CreateCall(getRawLengthFct, resolveValue(argNode));
  return LLVMExprResult{.value = lengthValue};
}

std::any IRGenerator::visitBuiltinPanicCall(const FctCallNode *node) {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_PANIC);

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
  assert(node->hasArgs);
  const ExprNode *assignExpr = node->argLst->args.front();
  llvm::Value *errorObjPtr = resolveAddress(assignExpr);
  llvm::Type *errorObjTy = assignExpr->getEvaluatedSymbolType(manIdx).toLLVMType(sourceFile);
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

std::any IRGenerator::visitBuiltinSyscallCall(const FctCallNode *node) {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_SYSCALL);

  // Determine the required number of operands.
  // (We assume at least one argument is provided: the syscall number)
  assert(node->hasArgs);
  const auto requiredRegs = static_cast<uint8_t>(node->argLst->args.size());
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
    const ExprNode *argNode = node->argLst->args.at(i);
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

std::any IRGenerator::visitBuiltinNewCall(const FctCallNode *node) {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_NEW);

  const FctCallNode::FctCallData &data = node->data.at(manIdx);
  const QualType &templateType = data.templateTypes.front();

  // Allocate sizeof(T) bytes on the heap
  llvm::Type *llvmType = templateType.toLLVMType(sourceFile);
  const uint64_t typeSize = module->getDataLayout().getTypeAllocSize(llvmType);
  llvm::Function *allocFct = stdFunctionManager.getAllocUnsafeLongFct();
  llvm::Value *targetPtr = builder.CreateCall(allocFct, {builder.getInt64(typeSize)});

  // Call the constructor if one was resolved during type checking
  if (const Function *ctor = data.callee) {
    std::vector<llvm::Value *> ctorArgValues;
    if (node->hasArgs) {
      const QualTypeList paramTypes = ctor->getParamTypes();
      for (size_t i = 0; i < node->argLst->args.size(); i++) {
        const ExprNode *argNode = node->argLst->args.at(i);
        if (paramTypes.at(i).isRef())
          ctorArgValues.push_back(resolveAddress(argNode));
        else
          ctorArgValues.push_back(resolveValue(argNode));
      }
    }
    generateCtorOrDtorCall(targetPtr, ctor, ctorArgValues);
  } else {
    const std::vector<ExprNode *> &args = node->argLst->args;
    llvm::Value *initVal = node->hasArgs ? resolveValue(args.front()) : getDefaultValueForSymbolType(templateType);
    insertStore(initVal, targetPtr);
  }

  return LLVMExprResult{.value = targetPtr};
}

std::any IRGenerator::visitBuiltinPlacementNewCall(const FctCallNode *node) {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_PLACEMENT_NEW);

  const FctCallNode::FctCallData &data = node->data.at(manIdx);

  // Resolve the target address from the first argument (the byte* pointer)
  llvm::Value *targetPtr = resolveValue(node->argLst->args.front());

  // Call the constructor if one was resolved during type checking
  if (const Function *ctor = data.callee) {
    std::vector<llvm::Value *> ctorArgValues;
    const QualTypeList paramTypes = ctor->getParamTypes();
    for (size_t i = 1; i < node->argLst->args.size(); i++) {
      const ExprNode *argNode = node->argLst->args.at(i);
      if (paramTypes.at(i - 1).isRef())
        ctorArgValues.push_back(resolveAddress(argNode));
      else
        ctorArgValues.push_back(resolveValue(argNode));
    }
    generateCtorOrDtorCall(targetPtr, ctor, ctorArgValues);
  } else {
    const QualType &templateType = data.templateTypes.front();
    const std::vector<ExprNode *> &args = node->argLst->args;
    llvm::Value *initVal = args.size() > 1 ? resolveValue(args.at(1)) : getDefaultValueForSymbolType(templateType);
    insertStore(initVal, targetPtr);
  }

  return LLVMExprResult{.value = targetPtr};
}

} // namespace spice::compiler
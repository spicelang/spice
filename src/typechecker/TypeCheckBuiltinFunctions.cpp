// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <global/GlobalResourceManager.h>
#include <global/TypeRegistry.h>
#include <typechecker/MacroDefs.h>

namespace spice::compiler {

std::any TypeChecker::visitBuiltinCall(BuiltinCallNode *node) {
  if (node->sysCall)
    return visitSysCall(node->sysCall);
  assert_fail("Unknown builtin call"); // LCOV_EXCL_LINE
  return nullptr;                      // LCOV_EXCL_LINE
}

std::any TypeChecker::visitSysCall(SysCallNode *node) {
  // Check if the syscall number if of type short
  const QualType sysCallNumberType = std::any_cast<ExprResult>(visit(node->args.front())).type;
  if (!sysCallNumberType.is(TY_SHORT))
    SOFT_ERROR_ER(node->args.front(), INVALID_SYSCALL_NUMBER_TYPE, "Syscall number must be of type short")

  // Check if the syscall number is out of range
  // According to https://www.chromium.org/chromium-os/developer-library/reference/linux-constants/syscalls/
  if (node->hasCompileTimeValue(manIdx)) {
    const unsigned short sysCallNumber = node->getCompileTimeValue(manIdx).shortValue;
    if (sysCallNumber < 0 || sysCallNumber > 439)
      SOFT_ERROR_ER(node, SYSCALL_NUMBER_OUT_OF_RANGE, "Only syscall numbers between 0 and 439 are supported")
  }

  // Check if too many syscall args are given
  // According to https://www.chromium.org/chromium-os/developer-library/reference/linux-constants/syscalls/
  if (node->args.size() > 6)
    SOFT_ERROR_ER(node->args.front(), TOO_MANY_SYSCALL_ARGS, "There are no syscalls that support more than 6 arguments")

  // Visit children
  for (size_t i = 1; i < node->args.size(); i++)
    visit(node->args.at(i));

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitNewBuiltinCall(FctCallNode *node) const {
  if (node->fqFunctionName == BUILTIN_FCT_NAME_PRINTF)
    return visitBuiltinPrintfCall(node);
  if (node->fqFunctionName == BUILTIN_FCT_NAME_SIZEOF)
    return visitBuiltinSizeOfCall(node);
  if (node->fqFunctionName == BUILTIN_FCT_NAME_ALIGNOF)
    return visitBuiltinAlignOfCall(node);
  if (node->fqFunctionName == BUILTIN_FCT_NAME_TYPEID)
    return visitBuiltinTypeIdCall(node);
  if (node->fqFunctionName == BUILTIN_FCT_NAME_LEN)
    return visitBuiltinLenCall(node);
  if (node->fqFunctionName == BUILTIN_FCT_NAME_PANIC)
    return visitBuiltinPanicCall(node);
  if (node->fqFunctionName == BUILTIN_FCT_NAME_IS_SAME)
    return visitBuiltinIsSameCall(node);
  if (node->fqFunctionName == BUILTIN_FCT_NAME_IMPLEMENTS_INTERFACE)
    return visitBuiltinImplementsInterfaceCall(node);

  assert_fail("This builtin call is not implemented yet"); // LCOV_EXCL_LINE
  return nullptr;                                          // LCOV_EXCL_LINE
}

std::any TypeChecker::visitBuiltinPrintfCall(FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_PRINTF);

  // Retrieve templated string
  assert(node->hasArgs);
  const AssignExprNode *firstArg = node->argLst->args.front();
  assert(firstArg->getEvaluatedSymbolType(manIdx).is(TY_STRING) && firstArg->hasCompileTimeValue(manIdx));
  const size_t stringOffset = firstArg->getCompileTimeValue(manIdx).stringValueOffset;
  const std::string templatedString = resourceManager.compileTimeStringValues.at(stringOffset);

  // Check if assignment types match placeholder types
  size_t placeholderCount = 0;
  size_t index = templatedString.find_first_of('%');
  while (index != std::string::npos && index != templatedString.size() - 1) {
    // Check if there is another assignExpr
    if (node->argLst->args.size() - 1 <= placeholderCount)
      SOFT_ERROR_ER(node, PRINTF_ARG_COUNT_ERROR, "The placeholder string contains more placeholders than arguments")

    // Get next assignment
    const AssignExprNode *assignment = node->argLst->args.at(placeholderCount + 1);
    // Visit assignment
    QualType argType = assignment->getEvaluatedSymbolType(manIdx);
    HANDLE_UNRESOLVED_TYPE_ER(argType)
    argType = argType.removeReferenceWrapper();

    switch (templatedString.at(index + 1)) {
    case 'c': {
      if (!argType.is(TY_CHAR))
        SOFT_ERROR_ER(assignment, PRINTF_TYPE_ERROR, "The placeholder string expects char, but got " + argType.getName(false))
      placeholderCount++;
      break;
    }
    case 'd':
    case 'i':
    case 'l':
    case 'o':
    case 'u':
    case 'x':
    case 'X': {
      if (!argType.isOneOf({TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_BOOL}))
        SOFT_ERROR_ER(assignment, PRINTF_TYPE_ERROR,
                      "The placeholder string expects int, short, long, byte or bool, but got " + argType.getName(false))
      placeholderCount++;
      break;
    }
    case 'a':
    case 'A':
    case 'f':
    case 'F':
    case 'e':
    case 'E':
    case 'g':
    case 'G': {
      if (!argType.is(TY_DOUBLE))
        SOFT_ERROR_ER(assignment, PRINTF_TYPE_ERROR, "The placeholder string expects double, but got " + argType.getName(false))
      placeholderCount++;
      break;
    }
    case 's': {
      if (!argType.is(TY_STRING) && !argType.isStringObj() && !argType.isPtrTo(TY_CHAR) && !argType.isArrayOf(TY_CHAR))
        SOFT_ERROR_ER(assignment, PRINTF_TYPE_ERROR,
                      "The placeholder string expects string, String, char* or char[], but got " + argType.getName(false))
      placeholderCount++;
      break;
    }
    case 'p': {
      if (!argType.isPtr() && !argType.isArray() && !argType.is(TY_STRING))
        SOFT_ERROR_ER(assignment, PRINTF_TYPE_ERROR,
                      "The placeholder string expects pointer, array or string, but got " + argType.getName(false))
      placeholderCount++;
      break;
    }
    default:
      SOFT_ERROR_ER(node, PRINTF_TYPE_ERROR, "The placeholder string contains an invalid placeholder")
    }
    index = templatedString.find_first_of('%', index + 2); // We can also skip the following char
  }

  // Check if the number of placeholders matches the number of args
  if (placeholderCount < node->argLst->args.size() - 1)
    SOFT_ERROR_ER(node, PRINTF_ARG_COUNT_ERROR, "The placeholder string contains less placeholders than arguments")

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_INT), manIdx)};
}

std::any TypeChecker::visitBuiltinSizeOfCall(FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_SIZEOF);

  // Check if arguments are given
  const bool hasExactlyOneTemplateType = node->hasTemplateTypes && node->templateTypeLst->dataTypes.size() == 1;
  const bool hasExactlyOneArg = node->hasArgs && node->argLst->args.size() == 1;
  if (!hasExactlyOneTemplateType && !hasExactlyOneArg)
    SOFT_ERROR_ER(node, BUILTIN_ARG_COUNT_MISMATCH, "This builtin does either take exactly one template type or argument");

  // Directly set compile time value here, so that compile time ifs can be evaluated.
  llvm::Type *type;
  if (hasExactlyOneTemplateType) { // Size of type
    type = node->templateTypeLst->dataTypes.front()->getEvaluatedSymbolType(manIdx).toLLVMType(sourceFile);
  } else { // Size of value
    type = node->argLst->args.front()->getEvaluatedSymbolType(manIdx).toLLVMType(sourceFile);
  }
  const int64_t typeSize = sourceFile->targetMachine->createDataLayout().getTypeAllocSize(type);
  node->data.at(manIdx).setCompileTimeValue({.longValue = typeSize});

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitBuiltinAlignOfCall(FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_ALIGNOF);

  // Check if arguments are given
  const bool hasExactlyOneTemplateType = node->hasTemplateTypes && node->templateTypeLst->dataTypes.size() == 1;
  const bool hasExactlyOneArg = node->hasArgs && node->argLst->args.size() == 1;
  if (!hasExactlyOneTemplateType && !hasExactlyOneArg)
    SOFT_ERROR_ER(node, BUILTIN_ARG_COUNT_MISMATCH, "This builtin does either take exactly one template type or argument");

  // Directly set compile time value here, so that compile time ifs can be evaluated.
  llvm::Type *type;
  if (hasExactlyOneTemplateType) { // Align of type
    type = node->templateTypeLst->dataTypes.front()->getEvaluatedSymbolType(manIdx).toLLVMType(sourceFile);
  } else { // Align of value
    type = node->argLst->args.front()->getEvaluatedSymbolType(manIdx).toLLVMType(sourceFile);
  }
  const int64_t typeAlignment = sourceFile->targetMachine->createDataLayout().getABITypeAlign(type).value();
  node->data.at(manIdx).setCompileTimeValue({.longValue = typeAlignment});

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitBuiltinTypeIdCall(FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_TYPEID);

  // Check if arguments are given
  const bool hasExactlyOneTemplateType = node->hasTemplateTypes && node->templateTypeLst->dataTypes.size() == 1;
  const bool hasExactlyOneArg = node->hasArgs && node->argLst->args.size() == 1;
  if (!hasExactlyOneTemplateType && !hasExactlyOneArg)
    SOFT_ERROR_ER(node, BUILTIN_ARG_COUNT_MISMATCH, "This builtin does either take exactly one template type or argument");

  // Directly set compile time value here, so that compile time ifs can be evaluated.
  QualType qualType;
  if (hasExactlyOneTemplateType) { // typeid of type
    qualType = node->templateTypeLst->dataTypes.front()->getEvaluatedSymbolType(manIdx);
  } else { // typeid of value
    qualType = node->argLst->args.front()->getEvaluatedSymbolType(manIdx);
  }
  const uint64_t typeId = TypeRegistry::getTypeHash(*qualType.getType());
  node->data.at(manIdx).setCompileTimeValue({.longValue = std::bit_cast<int64_t>(typeId)});

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitBuiltinLenCall(FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_LEN);

  // Check if arguments are given
  if (node->hasTemplateTypes)
    SOFT_ERROR_ER(node, BUILTIN_ARG_COUNT_MISMATCH, "This builtin does not take template types");
  if (!node->hasArgs || node->argLst->args.size() != 1)
    SOFT_ERROR_ER(node, BUILTIN_ARG_COUNT_MISMATCH, "This builtin does take exactly one argument");

  // Directly set compile time value here, so that compile time ifs can be evaluated.
  QualType argType = node->argLst->args.front()->getEvaluatedSymbolType(manIdx);
  argType = argType.removeReferenceWrapper();

  // Check if arg is of type array
  if (!argType.isArray() && !argType.is(TY_STRING))
    SOFT_ERROR_ER(node->argLst->args.front(), EXPECTED_ARRAY_TYPE, "The len builtin can only work on arrays or strings")

  if (argType.is(TY_ARRAY)) {
    node->data.at(manIdx).setCompileTimeValue({.longValue = argType.getArraySize()});
  } else {
    // If we want to use the len builtin on a string, we need to import the string runtime module
    if (!sourceFile->isStringRT())
      sourceFile->requestRuntimeModule(STRING_RT);
  }

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitBuiltinPanicCall(FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_PANIC);

  // Check if arguments are given
  if (!node->hasArgs || node->argLst->args.size() != 1)
    SOFT_ERROR_ER(node, BUILTIN_ARG_COUNT_MISMATCH, "This builtin takes exactly one argument");

  const AssignExprNode *assignExpr = node->argLst->args.front();
  QualType argType = assignExpr->getEvaluatedSymbolType(manIdx);
  HANDLE_UNRESOLVED_TYPE_ER(argType)
  argType = argType.removeReferenceWrapper();

  // Check if arg is of type array
  if (!argType.isErrorObj())
    SOFT_ERROR_ER(assignExpr, EXPECTED_ERROR_TYPE, "The panic builtin can only work with errors")

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_DYN), manIdx)};
}

std::any TypeChecker::visitBuiltinIsSameCall(FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_IS_SAME);

  // Check if arguments are given
  if (node->hasArgs)
    SOFT_ERROR_ER(node, BUILTIN_ARG_COUNT_MISMATCH, "This builtin does not take any arguments");

  // Check that the function is called with two or more template types
  if (!node->hasTemplateTypes || node->templateTypeLst->dataTypes.size() < 2)
    SOFT_ERROR_ER(node, BUILTIN_ARG_COUNT_MISMATCH, "This builtin needs to be called with at least two template types");

  // Directly set compile time value here, so that compile time ifs can be evaluated.
  node->setCompileTimeValue({.boolValue = true}, manIdx);
  const std::vector<DataTypeNode *> &dataTypeNodes = node->templateTypeLst->dataTypes;
  const QualType firstType = dataTypeNodes.front()->getEvaluatedSymbolType(manIdx);
  for (size_t i = 1; i < dataTypeNodes.size(); i++) {
    const QualType qualType = dataTypeNodes.at(i)->getEvaluatedSymbolType(manIdx);
    if (!qualType.matches(firstType, false, true, false)) {
      node->setCompileTimeValue({.boolValue = false}, manIdx);
      break;
    }
  }

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_BOOL), manIdx)};
}

std::any TypeChecker::visitBuiltinImplementsInterfaceCall(FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_IMPLEMENTS_INTERFACE);

  // Check if arguments are given
  if (node->hasArgs)
    SOFT_ERROR_ER(node, BUILTIN_ARG_COUNT_MISMATCH, "This builtin does not take any arguments");

  // Check that the function is called with exactly two or more template types
  if (!node->hasTemplateTypes || node->templateTypeLst->dataTypes.size() != 2)
    SOFT_ERROR_ER(node, BUILTIN_ARG_COUNT_MISMATCH, "This builtin needs to be called with exactly two template types");

  const QualType interfaceType = node->templateTypeLst->dataTypes.front()->getEvaluatedSymbolType(manIdx);
  const QualType structType = node->templateTypeLst->dataTypes.back()->getEvaluatedSymbolType(manIdx);
  const bool value = interfaceType.is(TY_INTERFACE) && structType.is(TY_STRUCT) && structType.doesImplement(interfaceType, node);
  node->setCompileTimeValue({.boolValue = value}, manIdx);

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_BOOL), manIdx)};
}

} // namespace spice::compiler

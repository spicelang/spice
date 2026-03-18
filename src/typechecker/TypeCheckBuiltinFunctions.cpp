// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <driver/Driver.h>
#include <global/GlobalResourceManager.h>
#include <global/TypeRegistry.h>
#include <typechecker/Builtins.h>
#include <typechecker/MacroDefs.h>

namespace spice::compiler {

std::any TypeChecker::visitBuiltinCall(FctCallNode *node) const {
  assert(BUILTIN_FUNCTIONS_MAP.contains(node->fqFunctionName) && "Builtin function not implemented!");
  const auto &info = BUILTIN_FUNCTIONS_MAP.find(node->fqFunctionName)->second;

  const auto buildErrorMessage = [](unsigned int min, unsigned int max, unsigned int actual, const char *suffix) {
    std::string expectedStr;
    if (min == max)
      expectedStr = min == 0 ? "no" : std::to_string(min);
    else
      expectedStr = "between " + std::to_string(min) + " and " + std::to_string(max) + " " + suffix;
    return "This builtin expects " + expectedStr + " " + suffix + ", but got " + std::to_string(actual);
  };

  // Do basic checks of template types and args, based on the builtin function info
  const size_t numTemplateTypes = node->hasTemplateTypes ? node->templateTypeLst->dataTypes.size() : 0;
  if (numTemplateTypes < info.minTemplateTypes || numTemplateTypes > info.maxTemplateTypes) {
    const auto msg = buildErrorMessage(info.minTemplateTypes, info.maxTemplateTypes, numTemplateTypes, "template type(s)");
    SOFT_ERROR_ER(node, BUILTIN_TEMPLATE_TYPE_COUNT_MISMATCH, msg);
  }

  const size_t numArgs = node->hasArgs ? node->argLst->args.size() : 0;
  if (numArgs < info.minArgTypes || numArgs > info.maxArgTypes) {
    const auto msg = buildErrorMessage(info.minArgTypes, info.maxArgTypes, numArgs, "argument(s)");
    SOFT_ERROR_ER(node, BUILTIN_ARG_COUNT_MISMATCH, msg);
  }

  if (info.allTemplateTypesOrAllArgTypes) {
    if (numTemplateTypes > 0 && numArgs > 0)
      SOFT_ERROR_ER(node, BUILTIN_SIGNATURE_MISMATCH, "This builtin expects either template types or arguments, but got both");
    if (numTemplateTypes == 0 && numArgs == 0)
      SOFT_ERROR_ER(node, BUILTIN_SIGNATURE_MISMATCH, "This builtin expects either template types or arguments, but got none");
  }

  // If specified, call to TypeChecker delegate to execute further checks
  return info.typeCheckerVisitMethod != nullptr ? (this->*info.typeCheckerVisitMethod)(node) : nullptr;
}

std::any TypeChecker::visitBuiltinPrintfCall(FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_PRINTF);

  // Retrieve templated string
  assert(node->hasArgs);
  const ExprNode *firstArg = node->argLst->args.front();
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
    const ExprNode *assignment = node->argLst->args.at(placeholderCount + 1);
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

  // Directly set compile time value here, so that compile time ifs can be evaluated.
  QualType qualType;
  if (node->hasTemplateTypes) { // Align of type
    qualType = node->templateTypeLst->dataTypes.front()->getEvaluatedSymbolType(manIdx);
  } else { // Align of value
    qualType = node->argLst->args.front()->getEvaluatedSymbolType(manIdx);
  }

  if (qualType.isOneOf({TY_UNRESOLVED, TY_DYN}))
    SOFT_ERROR_ER(node, UNEXPECTED_DYN_TYPE, "Cannot use sizeof on a dyn or unresolved type");

  llvm::Type *type = qualType.toLLVMType(sourceFile);
  const int64_t typeSize = sourceFile->targetMachine->createDataLayout().getTypeAllocSize(type);
  node->data.at(manIdx).setCompileTimeValue({.longValue = typeSize});

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitBuiltinAlignOfCall(FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_ALIGNOF);

  // Directly set compile time value here, so that compile time ifs can be evaluated.
  QualType qualType;
  if (node->hasTemplateTypes) { // Align of type
    qualType = node->templateTypeLst->dataTypes.front()->getEvaluatedSymbolType(manIdx);
  } else { // Align of value
    qualType = node->argLst->args.front()->getEvaluatedSymbolType(manIdx);
  }

  if (qualType.isOneOf({TY_UNRESOLVED, TY_DYN}))
    SOFT_ERROR_ER(node, UNEXPECTED_DYN_TYPE, "Cannot use alignof on a dyn or unresolved type");

  llvm::Type *type = qualType.toLLVMType(sourceFile);
  const int64_t typeAlignment = sourceFile->targetMachine->createDataLayout().getABITypeAlign(type).value();
  node->data.at(manIdx).setCompileTimeValue({.longValue = typeAlignment});

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitBuiltinTypeIdCall(FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_TYPEID);

  // Directly set compile time value here, so that compile time ifs can be evaluated.
  QualType qualType;
  if (node->hasTemplateTypes) { // typeid of type
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

  assert(node->hasArgs);
  const ExprNode *assignExpr = node->argLst->args.front();
  QualType argType = assignExpr->getEvaluatedSymbolType(manIdx);
  HANDLE_UNRESOLVED_TYPE_ER(argType)
  argType = argType.removeReferenceWrapper();

  // Check if arg is of type array
  if (!argType.isErrorObj())
    SOFT_ERROR_ER(assignExpr, EXPECTED_ERROR_TYPE, "The panic builtin can only work with errors")

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_DYN), manIdx)};
}

std::any TypeChecker::visitBuiltinSyscallCall(FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_SYSCALL);

  // Check if the syscall number if of type short
  const ExprNode *sysCallNumberExpr = node->argLst->args.front();
  const QualType sysCallNumberType = sysCallNumberExpr->getEvaluatedSymbolType(manIdx);
  if (!sysCallNumberType.is(TY_SHORT))
    SOFT_ERROR_ER(sysCallNumberExpr, INVALID_SYSCALL_NUMBER_TYPE, "Syscall number must be of type short")

  // Check if the syscall number is out of range
  // According to https://www.chromium.org/chromium-os/developer-library/reference/linux-constants/syscalls/
  if (node->hasCompileTimeValue(manIdx)) {
    const unsigned short sysCallNumber = node->getCompileTimeValue(manIdx).shortValue;
    if (sysCallNumber < 0 || sysCallNumber > 439)
      SOFT_ERROR_ER(node, SYSCALL_NUMBER_OUT_OF_RANGE, "Only syscall numbers between 0 and 439 are supported")
  }

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitBuiltinIsSameCall(FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_IS_SAME);

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

  const QualType interfaceType = node->templateTypeLst->dataTypes.front()->getEvaluatedSymbolType(manIdx);
  const QualType structType = node->templateTypeLst->dataTypes.back()->getEvaluatedSymbolType(manIdx);
  const bool value = interfaceType.is(TY_INTERFACE) && structType.is(TY_STRUCT) && structType.doesImplement(interfaceType, node);
  node->setCompileTimeValue({.boolValue = value}, manIdx);

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_BOOL), manIdx)};
}

std::any TypeChecker::visitBuiltinGetBuildVarCall(FctCallNode *node) const {
  assert(node->fqFunctionName == BUILTIN_FCT_NAME_GET_BUILD_VAR);

  const DataTypeNode *requestedTypeNode = node->templateTypeLst->dataTypes.front();
  const QualType requestedType = requestedTypeNode->getEvaluatedSymbolType(manIdx);
  if (!requestedType.isOneOf({TY_STRING, TY_INT, TY_BOOL}))
    SOFT_ERROR_ER(requestedTypeNode, BUILTIN_SIGNATURE_MISMATCH, "Build var type must be string, int or bool")

  const ExprNode *varNameNode = node->argLst->args.at(0);
  const QualType varNameType = varNameNode->getEvaluatedSymbolType(manIdx);
  if (!varNameType.is(TY_STRING))
    SOFT_ERROR_ER(varNameNode, BUILTIN_ARG_TYPE_MISMATCH, "Build var name must be a string")
  if (!varNameNode->hasCompileTimeValue(manIdx))
    SOFT_ERROR_ER(varNameNode, EXPECTED_COMPILE_TIME_VALUE, "Var name must be known at compile time")

  const bool hasDefaultValue = node->argLst->args.size() == 2;
  CompileTimeValue defaultValue;
  if (hasDefaultValue) {
    const ExprNode *defaultValueNode = node->argLst->args.at(1);
    const QualType defaultValueType = defaultValueNode->getEvaluatedSymbolType(manIdx);
    if (!defaultValueType.matches(requestedType, false, true, true))
      SOFT_ERROR_ER(defaultValueNode, BUILTIN_ARG_TYPE_MISMATCH, "Default value type must be the same as the requested type")
    if (!defaultValueNode->hasCompileTimeValue(manIdx))
      SOFT_ERROR_ER(defaultValueNode, EXPECTED_COMPILE_TIME_VALUE, "Default value must be known at compile time")
    defaultValue = defaultValueNode->getCompileTimeValue(manIdx);
  }

  const size_t stringValueOffset = varNameNode->getCompileTimeValue(manIdx).stringValueOffset;
  const std::string &varName = resourceManager.compileTimeStringValues.at(stringValueOffset);
  const auto it = cliOptions.buildVars.find(varName);
  if (it != cliOptions.buildVars.end()) {
    try {
      if (requestedType.is(TY_STRING)) {
        const size_t value = resourceManager.compileTimeStringValues.size();
        resourceManager.compileTimeStringValues.push_back(it->second);
        node->setCompileTimeValue({.stringValueOffset = value}, manIdx);
      } else if (requestedType.is(TY_INT)) {
        const int value = std::stoi(it->second);
        node->setCompileTimeValue({.intValue = value}, manIdx);
      } else if (requestedType.is(TY_BOOL)) {
        const bool value = it->second == "true";
        node->setCompileTimeValue({.boolValue = value}, manIdx);
      }
    } catch (const std::invalid_argument &) {
      SOFT_ERROR_ER(node, BUILTIN_ARG_TYPE_MISMATCH, "Error while parsing the provided value to the requested type")
    } catch (const std::out_of_range &) {
      SOFT_ERROR_ER(node, BUILTIN_ARG_TYPE_MISMATCH, "Error while parsing the provided value to the requested type")
    }
  } else {
    if (!hasDefaultValue)
      SOFT_ERROR_ER(varNameNode, BUILTIN_ARG_TYPE_MISMATCH, "Build var with this name was not provided");
    node->setCompileTimeValue(defaultValue, manIdx);
  }

  return ExprResult{node->setEvaluatedSymbolType(requestedType, manIdx)};
}

} // namespace spice::compiler

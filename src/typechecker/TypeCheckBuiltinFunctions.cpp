// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <global/GlobalResourceManager.h>
#include <typechecker/MacroDefs.h>

namespace spice::compiler {

std::any TypeChecker::visitBuiltinCall(BuiltinCallNode *node) {
  if (node->printfCall)
    return visitPrintfCall(node->printfCall);
  if (node->sizeofCall)
    return visitSizeofCall(node->sizeofCall);
  if (node->alignofCall)
    return visitAlignofCall(node->alignofCall);
  if (node->typeidCall)
    return visitTypeidCall(node->typeidCall);
  if (node->lenCall)
    return visitLenCall(node->lenCall);
  if (node->panicCall)
    return visitPanicCall(node->panicCall);
  if (node->sysCall)
    return visitSysCall(node->sysCall);
  assert_fail("Unknown builtin call"); // LCOV_EXCL_LINE
  return nullptr;                      // LCOV_EXCL_LINE
}

std::any TypeChecker::visitPrintfCall(PrintfCallNode *node) {
  // Check if assignment types match placeholder types
  size_t placeholderCount = 0;
  size_t index = node->templatedString.find_first_of('%');
  while (index != std::string::npos && index != node->templatedString.size() - 1) {
    // Check if there is another assignExpr
    if (node->args.size() <= placeholderCount)
      SOFT_ERROR_ER(node, PRINTF_ARG_COUNT_ERROR, "The placeholder string contains more placeholders than arguments")

    // Get next assignment
    AssignExprNode *assignment = node->args.at(placeholderCount);
    // Visit assignment
    QualType argType = std::any_cast<ExprResult>(visit(assignment)).type;
    HANDLE_UNRESOLVED_TYPE_ER(argType)
    argType = argType.removeReferenceWrapper();

    switch (node->templatedString.at(index + 1)) {
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
    index = node->templatedString.find_first_of('%', index + 2); // We can also skip the following char
  }

  // Check if the number of placeholders matches the number of args
  if (placeholderCount < node->args.size())
    SOFT_ERROR_ER(node, PRINTF_ARG_COUNT_ERROR, "The placeholder string contains less placeholders than arguments")

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_INT), manIdx)};
}

std::any TypeChecker::visitSizeofCall(SizeofCallNode *node) {
  if (node->isType) { // Size of type
    visit(node->dataType);
  } else { // Size of value
    visit(node->assignExpr);
  }

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitAlignofCall(AlignofCallNode *node) {
  if (node->isType) { // Align of type
    visit(node->dataType);
  } else { // Align of value
    visit(node->assignExpr);
  }

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitTypeidCall(TypeidCallNode *node) {
  if (node->isType) { // Align of type
    visit(node->dataType);
  } else { // Align of value
    visit(node->assignExpr);
  }

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitLenCall(LenCallNode *node) {
  QualType argType = std::any_cast<ExprResult>(visit(node->assignExpr)).type;
  HANDLE_UNRESOLVED_TYPE_ER(argType)
  argType = argType.removeReferenceWrapper();

  // Check if arg is of type array
  if (!argType.isArray() && !argType.is(TY_STRING))
    SOFT_ERROR_ER(node->assignExpr, EXPECTED_ARRAY_TYPE, "The len builtin can only work on arrays or strings")

  // If we want to use the len builtin on a string, we need to import the string runtime module
  if (argType.is(TY_STRING) && !sourceFile->isStringRT())
    sourceFile->requestRuntimeModule(STRING_RT);

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_LONG), manIdx)};
}

std::any TypeChecker::visitPanicCall(PanicCallNode *node) {
  QualType argType = std::any_cast<ExprResult>(visit(node->assignExpr)).type;
  HANDLE_UNRESOLVED_TYPE_ER(argType)
  argType = argType.removeReferenceWrapper();

  // Check if arg is of type array
  if (!argType.isErrorObj())
    SOFT_ERROR_ER(node->assignExpr, EXPECTED_ERROR_TYPE, "The panic builtin can only work with errors")

  return ExprResult{node->setEvaluatedSymbolType(QualType(TY_DYN), manIdx)};
}

std::any TypeChecker::visitSysCall(SysCallNode *node) {
  // Check if the syscall number if of type short
  const QualType sysCallNumberType = std::any_cast<ExprResult>(visit(node->args.front())).type;
  if (!sysCallNumberType.is(TY_SHORT))
    SOFT_ERROR_ER(node->args.front(), INVALID_SYSCALL_NUMBER_TYPE, "Syscall number must be of type short")

  // Check if the syscall number is out of range
  // According to https://www.chromium.org/chromium-os/developer-library/reference/linux-constants/syscalls/
  if (node->hasCompileTimeValue()) {
    const unsigned short sysCallNumber = node->getCompileTimeValue().shortValue;
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

} // namespace spice::compiler

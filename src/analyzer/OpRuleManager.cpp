// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "OpRuleManager.h"

SymbolType OpRuleManager::getAssignResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  // Skip type compatibility check if the lhs is of type dyn -> perform type inference
  if (lhs.is(TY_DYN))
    return rhs;
  // Allow pointers, arrays and structs of the same type straight away
  if (lhs.isOneOf({TY_PTR, TY_ARRAY, TY_STRUCT}) && lhs == rhs)
    return rhs;
  // Allow array to pointer
  if (lhs.is(TY_PTR) && rhs.is(TY_ARRAY) && lhs.getContainedTy() == rhs.getContainedTy())
    return lhs;
  // Allow char* = string
  if (lhs.isPointerOf(TY_CHAR) && rhs.is(TY_STRING))
    return lhs;
  // Check primitive type combinations
  return validateBinaryOperation(codeLoc, ASSIGN_OP_RULES, "=", lhs, rhs);
}

SymbolType OpRuleManager::getPlusEqualResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  if (lhs.isPointer() && rhs.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    if (withinUnsafeBlock)
      return lhs;
    else
      throw printErrorMessageUnsafe(codeLoc, "+=", lhs, rhs);
  }

  return validateBinaryOperation(codeLoc, PLUS_EQUAL_OP_RULES, "+=", lhs, rhs);
}

SymbolType OpRuleManager::getMinusEqualResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  if (lhs.isPointer() && rhs.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    if (withinUnsafeBlock)
      return lhs;
    else
      throw printErrorMessageUnsafe(codeLoc, "-=", lhs, rhs);
  }

  return validateBinaryOperation(codeLoc, MINUS_EQUAL_OP_RULES, "-=", lhs, rhs);
}

SymbolType OpRuleManager::getMulEqualResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, MUL_EQUAL_OP_RULES, "*=", lhs, rhs);
}

SymbolType OpRuleManager::getDivEqualResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, DIV_EQUAL_OP_RULES, "/=", lhs, rhs);
}

SymbolType OpRuleManager::getRemEqualResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, REM_EQUAL_OP_RULES, "%=", lhs, rhs);
}

SymbolType OpRuleManager::getSHLEqualResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, SHL_EQUAL_OP_RULES, "<<=", lhs, rhs);
}

SymbolType OpRuleManager::getSHREqualResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, SHR_EQUAL_OP_RULES, ">>=", lhs, rhs);
}

SymbolType OpRuleManager::getAndEqualResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, AND_EQUAL_OP_RULES, "&=", lhs, rhs);
}

SymbolType OpRuleManager::getOrEqualResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, OR_EQUAL_OP_RULES, "|=", lhs, rhs);
}

SymbolType OpRuleManager::getXorEqualResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, XOR_EQUAL_OP_RULES, "^=", lhs, rhs);
}

SymbolType OpRuleManager::getLogicalAndResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, LOGICAL_AND_OP_RULES, "&&", lhs, rhs);
}

SymbolType OpRuleManager::getLogicalOrResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, LOGICAL_OR_OP_RULES, "||", lhs, rhs);
}

SymbolType OpRuleManager::getBitwiseAndResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, BITWISE_AND_OP_RULES, "&", lhs, rhs);
}

SymbolType OpRuleManager::getBitwiseOrResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, BITWISE_OR_OP_RULES, "|", lhs, rhs);
}

SymbolType OpRuleManager::getBitwiseXorResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, BITWISE_XOR_OP_RULES, "^", lhs, rhs);
}

SymbolType OpRuleManager::getEqualResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  // Allow 'pointer == pointer' straight away
  if (lhs.isPointer() && rhs.isPointer())
    return SymbolType(TY_BOOL);
  // Allow 'pointer == byte' straight away
  if (lhs.isPointer() && rhs.is(TY_INT))
    return SymbolType(TY_BOOL);
  // Check primitive type combinations
  return validateBinaryOperation(codeLoc, EQUAL_OP_RULES, "==", lhs, rhs);
}

SymbolType OpRuleManager::getNotEqualResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  // Allow 'pointer != pointer' straight away
  if (lhs.isPointer() && rhs.isPointer())
    return SymbolType(TY_BOOL);
  // Allow 'pointer != byte' straight away
  if (lhs.isPointer() && rhs.is(TY_INT))
    return SymbolType(TY_BOOL);
  // Check primitive type combinations
  return validateBinaryOperation(codeLoc, NOT_EQUAL_OP_RULES, "!=", lhs, rhs);
}

SymbolType OpRuleManager::getLessResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, LESS_OP_RULES, "<", lhs, rhs);
}

SymbolType OpRuleManager::getGreaterResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, GREATER_OP_RULES, ">", lhs, rhs);
}

SymbolType OpRuleManager::getLessEqualResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, LESS_EQUAL_OP_RULES, "<=", lhs, rhs);
}

SymbolType OpRuleManager::getGreaterEqualResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, GREATER_EQUAL_OP_RULES, ">=", lhs, rhs);
}

SymbolType OpRuleManager::getShiftLeftResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, SHIFT_LEFT_OP_RULES, "<<", lhs, rhs);
}

SymbolType OpRuleManager::getShiftRightResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, SHIFT_RIGHT_OP_RULES, ">>", lhs, rhs);
}

SymbolType OpRuleManager::getPlusResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  // Allow any* + <int/long/short>
  if (lhs.isPointer() && rhs.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    if (withinUnsafeBlock)
      return lhs;
    else
      throw printErrorMessageUnsafe(codeLoc, "+", lhs, rhs);
  }
  // Allow <int/long/short> + any*
  if (lhs.isOneOf({TY_INT, TY_LONG, TY_SHORT}) && rhs.isPointer()) {
    if (withinUnsafeBlock)
      return rhs;
    else
      throw printErrorMessageUnsafe(codeLoc, "+", lhs, rhs);
  }

  return validateBinaryOperation(codeLoc, PLUS_OP_RULES, "+", lhs, rhs);
}

SymbolType OpRuleManager::getMinusResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  // Allow any* - <int/long/short>
  if (lhs.isPointer() && rhs.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    if (withinUnsafeBlock)
      return lhs;
    else
      throw printErrorMessageUnsafe(codeLoc, "-", lhs, rhs);
  }
  // Allow <int/long/short> - any*
  if (lhs.isOneOf({TY_INT, TY_LONG, TY_SHORT}) && rhs.isPointer()) {
    if (withinUnsafeBlock)
      return rhs;
    else
      throw printErrorMessageUnsafe(codeLoc, "-", lhs, rhs);
  }

  return validateBinaryOperation(codeLoc, MINUS_OP_RULES, "-", lhs, rhs);
}

SymbolType OpRuleManager::getMulResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, MUL_OP_RULES, "*", lhs, rhs);
}

SymbolType OpRuleManager::getDivResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, DIV_OP_RULES, "/", lhs, rhs);
}

SymbolType OpRuleManager::getRemResultType(const CodeLoc &codeLoc, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(codeLoc, REM_OP_RULES, "%", lhs, rhs);
}

SymbolType OpRuleManager::getPrefixMinusResultType(const CodeLoc &codeLoc, const SymbolType &lhs) {
  return validateUnaryOperation(codeLoc, PREFIX_MINUS_OP_RULES, "-", lhs);
}

SymbolType OpRuleManager::getPrefixPlusPlusResultType(const CodeLoc &codeLoc, const SymbolType &lhs) {
  return validateUnaryOperation(codeLoc, PREFIX_PLUS_PLUS_OP_RULES, "++", lhs);
}

SymbolType OpRuleManager::getPrefixMinusMinusResultType(const CodeLoc &codeLoc, const SymbolType &lhs) {
  return validateUnaryOperation(codeLoc, PREFIX_MINUS_MINUS_OP_RULES, "--", lhs);
}

SymbolType OpRuleManager::getPrefixNotResultType(const CodeLoc &codeLoc, const SymbolType &lhs) {
  return validateUnaryOperation(codeLoc, PREFIX_NOT_OP_RULES, "!", lhs);
}

SymbolType OpRuleManager::getPrefixBitwiseNotResultType(const CodeLoc &codeLoc, const SymbolType &lhs) {
  return validateUnaryOperation(codeLoc, PREFIX_BITWISE_NOT_OP_RULES, "~", lhs);
}

SymbolType OpRuleManager::getPrefixMulResultType(const CodeLoc &codeLoc, const SymbolType &lhs) {
  if (!lhs.isPointer())
    throw SemanticError(codeLoc, OPERATOR_WRONG_DATA_TYPE, "Cannot apply de-referencing operator on type " + lhs.getName(true));
  return lhs.getContainedTy();
}

SymbolType OpRuleManager::getPrefixBitwiseAndResultType(const CodeLoc &codeLoc, const SymbolType &lhs) {
  return lhs.toPointer(codeLoc);
}

SymbolType OpRuleManager::getPostfixPlusPlusResultType(const CodeLoc &codeLoc, const SymbolType &lhs) {
  return validateUnaryOperation(codeLoc, POSTFIX_PLUS_PLUS_OP_RULES, "++", lhs);
}

SymbolType OpRuleManager::getPostfixMinusMinusResultType(const CodeLoc &codeLoc, const SymbolType &lhs) {
  return validateUnaryOperation(codeLoc, POSTFIX_MINUS_MINUS_OP_RULES, "--", lhs);
}

SymbolType OpRuleManager::getCastResultType(const CodeLoc &codeLoc, const SymbolType &lhs,
                                            const SymbolType &rhs) { // double dbl = (double) 10;
  // Allow casts string -> char*  and string -> char[]
  if (lhs.isOneOf({TY_PTR, TY_ARRAY}) && lhs.getContainedTy().is(TY_CHAR) && rhs.is(TY_STRING))
    return lhs;
  // Allow casts char* -> string and char[] -> string
  if (lhs.is(TY_STRING) && rhs.isOneOf({TY_PTR, TY_ARRAY}) && rhs.getContainedTy().is(TY_CHAR))
    return lhs;
  // Allow casts any* -> any*
  if (lhs.isPointer() && rhs.isPointer()) {
    if (withinUnsafeBlock)
      return lhs;
    else
      throw printErrorMessageUnsafe(codeLoc, "(cast)", lhs, rhs);
  }
  // Check primitive type combinations
  return validateBinaryOperation(codeLoc, CAST_OP_RULES, "(cast)", lhs, rhs);
}

SymbolType OpRuleManager::validateBinaryOperation(const CodeLoc &codeLoc, const std::vector<BinaryOpRule> &opRules,
                                                  const std::string &opName, const SymbolType &lhs, const SymbolType &rhs) {
  for (const auto &rule : opRules) {
    if (std::get<0>(rule) == lhs && std::get<1>(rule) == rhs)
      return std::get<2>(rule);
  }
  throw printErrorMessageBinary(codeLoc, opName, lhs, rhs);
}

SymbolType OpRuleManager::validateUnaryOperation(const CodeLoc &codeLoc, const std::vector<UnaryOpRule> &opRules,
                                                 const std::string &opName, const SymbolType &lhs) {
  for (const auto &rule : opRules) {
    if (std::get<0>(rule) == lhs)
      return std::get<1>(rule);
  }
  throw printErrorMessageUnary(codeLoc, opName, lhs);
}

SemanticError OpRuleManager::printErrorMessageBinary(const CodeLoc &codeLoc, const std::string &operatorName,
                                                     const SymbolType &lhs, const SymbolType &rhs) {
  return SemanticError(codeLoc, OPERATOR_WRONG_DATA_TYPE,
                       "Cannot apply '" + operatorName + "' operator on types " + lhs.getName(true) + " and " +
                           rhs.getName(true));
}

SemanticError OpRuleManager::printErrorMessageUnary(const CodeLoc &codeLoc, const std::string &operatorName,
                                                    const SymbolType &lhs) {
  return SemanticError(codeLoc, OPERATOR_WRONG_DATA_TYPE,
                       "Cannot apply '" + operatorName + "' operator on type " + lhs.getName(true));
}

SemanticError OpRuleManager::printErrorMessageUnsafe(const CodeLoc &codeLoc, const std::string &operatorName,
                                                     const SymbolType &lhs, const SymbolType &rhs) {
  return SemanticError(codeLoc, UNSAFE_OPERATION_IN_SAFE_CONTEXT,
                       "Cannot apply '" + operatorName + "' operator on types " + lhs.getName(true) + " and " +
                           rhs.getName(true) +
                           " as this is an unsafe operation. Please use unsafe blocks if you know what you are doing.");
}
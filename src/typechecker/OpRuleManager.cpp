// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "OpRuleManager.h"

#include <analyzer/AnalyzerVisitor.h>
#include <ast/AstNodes.h>
#include <global/RuntimeModuleManager.h>

SymbolType OpRuleManager::getAssignResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
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
  // Allow String (strobj) = String (struct)
  if (lhs.is(TY_STROBJ) && rhs.is(TY_STRUCT, STROBJ_NAME))
    return lhs;
  // Check primitive type combinations
  return validateBinaryOperation(node, ASSIGN_OP_RULES, "=", lhs, rhs);
}

SymbolType OpRuleManager::getPlusEqualResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  if (lhs.isPointer() && rhs.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    if (analyzer->allowUnsafeOperations)
      return lhs;
    else
      throw printErrorMessageUnsafe(node, "+=", lhs, rhs);
  }

  return validateBinaryOperation(node, PLUS_EQUAL_OP_RULES, "+=", lhs, rhs);
}

SymbolType OpRuleManager::getMinusEqualResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  if (lhs.isPointer() && rhs.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    if (analyzer->allowUnsafeOperations)
      return lhs;
    else
      throw printErrorMessageUnsafe(node, "-=", lhs, rhs);
  }

  return validateBinaryOperation(node, MINUS_EQUAL_OP_RULES, "-=", lhs, rhs);
}

SymbolType OpRuleManager::getMulEqualResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, MUL_EQUAL_OP_RULES, "*=", lhs, rhs);
}

SymbolType OpRuleManager::getDivEqualResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, DIV_EQUAL_OP_RULES, "/=", lhs, rhs);
}

SymbolType OpRuleManager::getRemEqualResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, REM_EQUAL_OP_RULES, "%=", lhs, rhs);
}

SymbolType OpRuleManager::getSHLEqualResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, SHL_EQUAL_OP_RULES, "<<=", lhs, rhs);
}

SymbolType OpRuleManager::getSHREqualResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, SHR_EQUAL_OP_RULES, ">>=", lhs, rhs);
}

SymbolType OpRuleManager::getAndEqualResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, AND_EQUAL_OP_RULES, "&=", lhs, rhs);
}

SymbolType OpRuleManager::getOrEqualResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, OR_EQUAL_OP_RULES, "|=", lhs, rhs);
}

SymbolType OpRuleManager::getXorEqualResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, XOR_EQUAL_OP_RULES, "^=", lhs, rhs);
}

SymbolType OpRuleManager::getLogicalAndResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, LOGICAL_AND_OP_RULES, "&&", lhs, rhs);
}

SymbolType OpRuleManager::getLogicalOrResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, LOGICAL_OR_OP_RULES, "||", lhs, rhs);
}

SymbolType OpRuleManager::getBitwiseAndResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, BITWISE_AND_OP_RULES, "&", lhs, rhs);
}

SymbolType OpRuleManager::getBitwiseOrResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, BITWISE_OR_OP_RULES, "|", lhs, rhs);
}

SymbolType OpRuleManager::getBitwiseXorResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, BITWISE_XOR_OP_RULES, "^", lhs, rhs);
}

SymbolType OpRuleManager::getEqualResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  // Allow 'pointer == pointer' straight away
  if (lhs.isPointer() && rhs.isPointer())
    return SymbolType(TY_BOOL);
  // Allow 'pointer == byte' straight away
  if (lhs.isPointer() && rhs.is(TY_INT))
    return SymbolType(TY_BOOL);
  // Check primitive type combinations
  return validateBinaryOperation(node, EQUAL_OP_RULES, "==", lhs, rhs);
}

SymbolType OpRuleManager::getNotEqualResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  // Allow 'pointer != pointer' straight away
  if (lhs.isPointer() && rhs.isPointer())
    return SymbolType(TY_BOOL);
  // Allow 'pointer != byte' straight away
  if (lhs.isPointer() && rhs.is(TY_INT))
    return SymbolType(TY_BOOL);
  // Check primitive type combinations
  return validateBinaryOperation(node, NOT_EQUAL_OP_RULES, "!=", lhs, rhs);
}

SymbolType OpRuleManager::getLessResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, LESS_OP_RULES, "<", lhs, rhs);
}

SymbolType OpRuleManager::getGreaterResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, GREATER_OP_RULES, ">", lhs, rhs);
}

SymbolType OpRuleManager::getLessEqualResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, LESS_EQUAL_OP_RULES, "<=", lhs, rhs);
}

SymbolType OpRuleManager::getGreaterEqualResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, GREATER_EQUAL_OP_RULES, ">=", lhs, rhs);
}

SymbolType OpRuleManager::getShiftLeftResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, SHIFT_LEFT_OP_RULES, "<<", lhs, rhs);
}

SymbolType OpRuleManager::getShiftRightResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, SHIFT_RIGHT_OP_RULES, ">>", lhs, rhs);
}

SymbolType OpRuleManager::getPlusResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  // Allow any* + <int/long/short>
  if (lhs.isPointer() && rhs.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    if (analyzer->allowUnsafeOperations)
      return lhs;
    else
      throw printErrorMessageUnsafe(node, "+", lhs, rhs);
  }
  // Allow <int/long/short> + any*
  if (lhs.isOneOf({TY_INT, TY_LONG, TY_SHORT}) && rhs.isPointer()) {
    if (analyzer->allowUnsafeOperations)
      return rhs;
    else
      throw printErrorMessageUnsafe(node, "+", lhs, rhs);
  }

  // Allow strobj + string
  if (lhs.is(TY_STROBJ) && rhs.is(TY_STRING))
    analyzer->insertAnonStringStructSymbol(node);
  // Allow strobj + strobj
  if (lhs.is(TY_STROBJ) && rhs.is(TY_STROBJ))
    analyzer->insertAnonStringStructSymbol(node);

  return validateBinaryOperation(node, PLUS_OP_RULES, "+", lhs, rhs);
}

SymbolType OpRuleManager::getMinusResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  // Allow any* - <int/long/short>
  if (lhs.isPointer() && rhs.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    if (analyzer->allowUnsafeOperations)
      return lhs;
    else
      throw printErrorMessageUnsafe(node, "-", lhs, rhs);
  }
  // Allow <int/long/short> - any*
  if (lhs.isOneOf({TY_INT, TY_LONG, TY_SHORT}) && rhs.isPointer()) {
    if (analyzer->allowUnsafeOperations)
      return rhs;
    else
      throw printErrorMessageUnsafe(node, "-", lhs, rhs);
  }

  return validateBinaryOperation(node, MINUS_OP_RULES, "-", lhs, rhs);
}

SymbolType OpRuleManager::getMulResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  // Allow strobj * <int|short|long>
  if (lhs.is(TY_STROBJ) && rhs.isOneOf({TY_INT, TY_SHORT, TY_LONG}))
    analyzer->insertAnonStringStructSymbol(node);

  // Allow <int|short|long> * strobj
  if (lhs.isOneOf({TY_INT, TY_SHORT, TY_LONG}) && rhs.is(TY_STROBJ))
    analyzer->insertAnonStringStructSymbol(node);

  return validateBinaryOperation(node, MUL_OP_RULES, "*", lhs, rhs);
}

SymbolType OpRuleManager::getDivResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, DIV_OP_RULES, "/", lhs, rhs);
}

SymbolType OpRuleManager::getRemResultType(const AstNode *node, const SymbolType &lhs, const SymbolType &rhs) {
  return validateBinaryOperation(node, REM_OP_RULES, "%", lhs, rhs);
}

SymbolType OpRuleManager::getPrefixMinusResultType(const AstNode *node, const SymbolType &lhs) {
  return validateUnaryOperation(node, PREFIX_MINUS_OP_RULES, "-", lhs);
}

SymbolType OpRuleManager::getPrefixPlusPlusResultType(const AstNode *node, const SymbolType &lhs) {
  return validateUnaryOperation(node, PREFIX_PLUS_PLUS_OP_RULES, "++", lhs);
}

SymbolType OpRuleManager::getPrefixMinusMinusResultType(const AstNode *node, const SymbolType &lhs) {
  return validateUnaryOperation(node, PREFIX_MINUS_MINUS_OP_RULES, "--", lhs);
}

SymbolType OpRuleManager::getPrefixNotResultType(const AstNode *node, const SymbolType &lhs) {
  return validateUnaryOperation(node, PREFIX_NOT_OP_RULES, "!", lhs);
}

SymbolType OpRuleManager::getPrefixBitwiseNotResultType(const AstNode *node, const SymbolType &lhs) {
  return validateUnaryOperation(node, PREFIX_BITWISE_NOT_OP_RULES, "~", lhs);
}

SymbolType OpRuleManager::getPrefixMulResultType(const AstNode *node, const SymbolType &lhs) {
  if (!lhs.isPointer())
    throw SemanticError(node, OPERATOR_WRONG_DATA_TYPE, "Cannot apply de-referencing operator on type " + lhs.getName(true));
  return lhs.getContainedTy();
}

SymbolType OpRuleManager::getPrefixBitwiseAndResultType(const AstNode *node, const SymbolType &lhs) {
  return lhs.toPointer(node);
}

SymbolType OpRuleManager::getPostfixPlusPlusResultType(const AstNode *node, const SymbolType &lhs) {
  return validateUnaryOperation(node, POSTFIX_PLUS_PLUS_OP_RULES, "++", lhs);
}

SymbolType OpRuleManager::getPostfixMinusMinusResultType(const AstNode *node, const SymbolType &lhs) {
  return validateUnaryOperation(node, POSTFIX_MINUS_MINUS_OP_RULES, "--", lhs);
}

SymbolType OpRuleManager::getCastResultType(const AstNode *node, const SymbolType &lhs,
                                            const SymbolType &rhs) { // double dbl = (double) 10;
  // Allow casts string -> char*  and string -> char[]
  if (lhs.isOneOf({TY_PTR, TY_ARRAY}) && lhs.getContainedTy().is(TY_CHAR) && rhs.is(TY_STRING))
    return lhs;
  // Allow casts char* -> string and char[] -> string
  if (lhs.is(TY_STRING) && rhs.isOneOf({TY_PTR, TY_ARRAY}) && rhs.getContainedTy().is(TY_CHAR))
    return lhs;
  // Allow casts any* -> any*
  if (lhs.isPointer() && rhs.isPointer()) {
    if (analyzer->allowUnsafeOperations)
      return lhs;
    else
      throw printErrorMessageUnsafe(node, "(cast)", lhs, rhs);
  }
  // Check primitive type combinations
  return validateBinaryOperation(node, CAST_OP_RULES, "(cast)", lhs, rhs);
}

SymbolType OpRuleManager::validateBinaryOperation(const AstNode *node, const std::vector<BinaryOpRule> &opRules,
                                                  const std::string &name, const SymbolType &lhs, const SymbolType &rhs) {
  for (const auto &rule : opRules) {
    if (std::get<0>(rule) == lhs && std::get<1>(rule) == rhs)
      return std::get<2>(rule);
  }
  throw printErrorMessageBinary(node, name, lhs, rhs);
}

SymbolType OpRuleManager::validateUnaryOperation(const AstNode *node, const std::vector<UnaryOpRule> &opRules,
                                                 const std::string &name, const SymbolType &lhs) {
  for (const auto &rule : opRules) {
    if (std::get<0>(rule) == lhs)
      return std::get<1>(rule);
  }
  throw printErrorMessageUnary(node, name, lhs);
}

SemanticError OpRuleManager::printErrorMessageBinary(const AstNode *node, const std::string &name, const SymbolType &lhs,
                                                     const SymbolType &rhs) {
  return SemanticError(node, OPERATOR_WRONG_DATA_TYPE,
                       "Cannot apply '" + name + "' operator on types " + lhs.getName(true) + " and " + rhs.getName(true));
}

SemanticError OpRuleManager::printErrorMessageUnary(const AstNode *node, const std::string &name, const SymbolType &lhs) {
  return SemanticError(node, OPERATOR_WRONG_DATA_TYPE, "Cannot apply '" + name + "' operator on type " + lhs.getName(true));
}

SemanticError OpRuleManager::printErrorMessageUnsafe(const AstNode *node, const std::string &name, const SymbolType &lhs,
                                                     const SymbolType &rhs) {
  return SemanticError(node, UNSAFE_OPERATION_IN_SAFE_CONTEXT,
                       "Cannot apply '" + name + "' operator on types " + lhs.getName(true) + " and " + rhs.getName(true) +
                           " as this is an unsafe operation. Please use unsafe blocks if you know what you are doing.");
}
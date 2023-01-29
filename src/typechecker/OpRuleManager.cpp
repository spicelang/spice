// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "OpRuleManager.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <global/RuntimeModuleManager.h>
#include <typechecker/TypeChecker.h>

namespace spice::compiler {

SymbolType OpRuleManager::getAssignResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  // Skip type compatibility check if the lhs is of type dyn -> perform type inference
  if (lhs.is(TY_DYN))
    return rhs;
  // Allow pointers, arrays and structs of the same type straight away
  if (lhs.isOneOf({TY_PTR, TY_ARRAY, TY_STRUCT}) && lhs == rhs)
    return rhs;
  // Allow type to ref type of the same contained type straight away
  if (lhs.is(TY_REF) && lhs.getContainedTy() == rhs)
    return lhs;
  // Allow array to pointer
  if (lhs.is(TY_PTR) && rhs.is(TY_ARRAY) && lhs.getContainedTy() == rhs.getContainedTy())
    return lhs;
  // Allow char* = string
  if (lhs.isPtrOf(TY_CHAR) && rhs.is(TY_STRING))
    return lhs;
  // Check primitive type combinations
  return validateBinaryOperation(node, ASSIGN_OP_RULES, ARRAY_LENGTH(ASSIGN_OP_RULES), "=", lhs, rhs);
}

SymbolType OpRuleManager::getPlusEqualResultType(ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  // Check if this is an unsafe operation
  if (lhs.isPtr() && rhs.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    ensureUnsafeAllowed(node, "+=", lhs, rhs);
    return lhs;
  }

  // Check is there is an overloaded operator function available
  if (lhs.is(TY_STRUCT) || rhs.is(TY_STRUCT)) {
    SymbolType resultType = isBinaryOperatorOverloadingFctAvailable(OP_FCT_PLUS_EQUAL, lhs, rhs, node, opIdx);
    if (!resultType.is(TY_INVALID))
      return resultType;
  }

  return validateBinaryOperation(node, PLUS_EQUAL_OP_RULES, ARRAY_LENGTH(PLUS_EQUAL_OP_RULES), "+=", lhs, rhs);
}

SymbolType OpRuleManager::getMinusEqualResultType(ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  if (lhs.isPtr() && rhs.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    ensureUnsafeAllowed(node, "-=", lhs, rhs);
    return lhs;
  }

  // Check is there is an overloaded operator function available
  if (lhs.is(TY_STRUCT) || rhs.is(TY_STRUCT)) {
    SymbolType resultType = isBinaryOperatorOverloadingFctAvailable(OP_FCT_MINUS_EQUAL, lhs, rhs, node, opIdx);
    if (!resultType.is(TY_INVALID))
      return resultType;
  }

  return validateBinaryOperation(node, MINUS_EQUAL_OP_RULES, ARRAY_LENGTH(MINUS_EQUAL_OP_RULES), "-=", lhs, rhs);
}

SymbolType OpRuleManager::getMulEqualResultType(ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  // Check is there is an overloaded operator function available
  if (lhs.is(TY_STRUCT) || rhs.is(TY_STRUCT)) {
    SymbolType resultType = isBinaryOperatorOverloadingFctAvailable(OP_FCT_MUL_EQUAL, lhs, rhs, node, opIdx);
    if (!resultType.is(TY_INVALID))
      return resultType;
  }

  return validateBinaryOperation(node, MUL_EQUAL_OP_RULES, ARRAY_LENGTH(MUL_EQUAL_OP_RULES), "*=", lhs, rhs);
}

SymbolType OpRuleManager::getDivEqualResultType(ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  // Check is there is an overloaded operator function available
  if (lhs.is(TY_STRUCT) || rhs.is(TY_STRUCT)) {
    SymbolType resultType = isBinaryOperatorOverloadingFctAvailable(OP_FCT_DIV_EQUAL, lhs, rhs, node, opIdx);
    if (!resultType.is(TY_INVALID))
      return resultType;
  }

  return validateBinaryOperation(node, DIV_EQUAL_OP_RULES, ARRAY_LENGTH(DIV_EQUAL_OP_RULES), "/=", lhs, rhs);
}

SymbolType OpRuleManager::getRemEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, REM_EQUAL_OP_RULES, ARRAY_LENGTH(REM_EQUAL_OP_RULES), "%=", lhs, rhs);
}

SymbolType OpRuleManager::getSHLEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, SHL_EQUAL_OP_RULES, ARRAY_LENGTH(SHL_EQUAL_OP_RULES), "<<=", lhs, rhs);
}

SymbolType OpRuleManager::getSHREqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, SHR_EQUAL_OP_RULES, ARRAY_LENGTH(SHR_EQUAL_OP_RULES), ">>=", lhs, rhs);
}

SymbolType OpRuleManager::getAndEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, AND_EQUAL_OP_RULES, ARRAY_LENGTH(AND_EQUAL_OP_RULES), "&=", lhs, rhs);
}

SymbolType OpRuleManager::getOrEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, OR_EQUAL_OP_RULES, ARRAY_LENGTH(OR_EQUAL_OP_RULES), "|=", lhs, rhs);
}

SymbolType OpRuleManager::getXorEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, XOR_EQUAL_OP_RULES, ARRAY_LENGTH(XOR_EQUAL_OP_RULES), "^=", lhs, rhs);
}

SymbolType OpRuleManager::getLogicalAndResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, LOGICAL_AND_OP_RULES, ARRAY_LENGTH(LOGICAL_AND_OP_RULES), "&&", lhs, rhs);
}

SymbolType OpRuleManager::getLogicalOrResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, LOGICAL_OR_OP_RULES, ARRAY_LENGTH(LOGICAL_OR_OP_RULES), "||", lhs, rhs);
}

SymbolType OpRuleManager::getBitwiseAndResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, BITWISE_AND_OP_RULES, ARRAY_LENGTH(BITWISE_AND_OP_RULES), "&", lhs, rhs);
}

SymbolType OpRuleManager::getBitwiseOrResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, BITWISE_OR_OP_RULES, ARRAY_LENGTH(BITWISE_OR_OP_RULES), "|", lhs, rhs);
}

SymbolType OpRuleManager::getBitwiseXorResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, BITWISE_XOR_OP_RULES, ARRAY_LENGTH(BITWISE_XOR_OP_RULES), "^", lhs, rhs);
}

SymbolType OpRuleManager::getEqualResultType(ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  // Allow 'pointer == pointer' straight away
  if (lhs.isPtr() && rhs.isPtr())
    return SymbolType(TY_BOOL);

  // Allow 'pointer == int' straight away
  if (lhs.isPtr() && rhs.is(TY_INT))
    return SymbolType(TY_BOOL);

  // Check is there is an overloaded operator function available
  if (lhs.is(TY_STRUCT) || rhs.is(TY_STRUCT)) {
    SymbolType resultType = isBinaryOperatorOverloadingFctAvailable(OP_FCT_EQUAL, lhs, rhs, node, opIdx);
    if (!resultType.is(TY_INVALID))
      return resultType;
  }

  // Check primitive type combinations
  return validateBinaryOperation(node, EQUAL_OP_RULES, ARRAY_LENGTH(EQUAL_OP_RULES), "==", lhs, rhs);
}

SymbolType OpRuleManager::getNotEqualResultType(ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  // Allow 'pointer != pointer' straight away
  if (lhs.isPtr() && rhs.isPtr())
    return SymbolType(TY_BOOL);

  // Allow 'pointer != int' straight away
  if (lhs.isPtr() && rhs.is(TY_INT))
    return SymbolType(TY_BOOL);

  // Check is there is an overloaded operator function available
  if (lhs.is(TY_STRUCT) || rhs.is(TY_STRUCT)) {
    SymbolType resultType = isBinaryOperatorOverloadingFctAvailable(OP_FCT_NOT_EQUAL, lhs, rhs, node, opIdx);
    if (!resultType.is(TY_INVALID))
      return resultType;
  }

  // Check primitive type combinations
  return validateBinaryOperation(node, NOT_EQUAL_OP_RULES, ARRAY_LENGTH(NOT_EQUAL_OP_RULES), "!=", lhs, rhs);
}

SymbolType OpRuleManager::getLessResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, LESS_OP_RULES, ARRAY_LENGTH(LESS_OP_RULES), "<", lhs, rhs);
}

SymbolType OpRuleManager::getGreaterResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, GREATER_OP_RULES, ARRAY_LENGTH(GREATER_OP_RULES), ">", lhs, rhs);
}

SymbolType OpRuleManager::getLessEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, LESS_EQUAL_OP_RULES, ARRAY_LENGTH(LESS_EQUAL_OP_RULES), "<=", lhs, rhs);
}

SymbolType OpRuleManager::getGreaterEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, GREATER_EQUAL_OP_RULES, ARRAY_LENGTH(GREATER_EQUAL_OP_RULES), ">=", lhs, rhs);
}

SymbolType OpRuleManager::getShiftLeftResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, SHIFT_LEFT_OP_RULES, ARRAY_LENGTH(SHIFT_LEFT_OP_RULES), "<<", lhs, rhs);
}

SymbolType OpRuleManager::getShiftRightResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, SHIFT_RIGHT_OP_RULES, ARRAY_LENGTH(SHIFT_RIGHT_OP_RULES), ">>", lhs, rhs);
}

SymbolType OpRuleManager::getPlusResultType(ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  // Allow any* + <int/long/short>
  if (lhs.isPtr() && rhs.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    ensureUnsafeAllowed(node, "+", lhs, rhs);
    return lhs;
  }
  // Allow <int/long/short> + any*
  if (lhs.isOneOf({TY_INT, TY_LONG, TY_SHORT}) && rhs.isPtr()) {
    ensureUnsafeAllowed(node, "+", lhs, rhs);
    return rhs;
  }

  // Check is there is an overloaded operator function available
  if (lhs.is(TY_STRUCT) || rhs.is(TY_STRUCT)) {
    SymbolType resultType = isBinaryOperatorOverloadingFctAvailable(OP_FCT_PLUS, lhs, rhs, node, opIdx);
    if (!resultType.is(TY_INVALID))
      return resultType;
  }

  return validateBinaryOperation(node, PLUS_OP_RULES, ARRAY_LENGTH(PLUS_OP_RULES), "+", lhs, rhs);
}

SymbolType OpRuleManager::getMinusResultType(ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  // Allow any* - <int/long/short>
  if (lhs.isPtr() && rhs.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    ensureUnsafeAllowed(node, "-", lhs, rhs);
    return lhs;
  }
  // Allow <int/long/short> - any*
  if (lhs.isOneOf({TY_INT, TY_LONG, TY_SHORT}) && rhs.isPtr()) {
    ensureUnsafeAllowed(node, "-", lhs, rhs);
    return rhs;
  }

  // Check is there is an overloaded operator function available
  if (lhs.is(TY_STRUCT) || rhs.is(TY_STRUCT)) {
    SymbolType resultType = isBinaryOperatorOverloadingFctAvailable(OP_FCT_MINUS, lhs, rhs, node, opIdx);
    if (!resultType.is(TY_INVALID))
      return resultType;
  }

  return validateBinaryOperation(node, MINUS_OP_RULES, ARRAY_LENGTH(MINUS_OP_RULES), "-", lhs, rhs);
}

SymbolType OpRuleManager::getMulResultType(ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  // Check is there is an overloaded operator function available
  if (lhs.is(TY_STRUCT) || rhs.is(TY_STRUCT)) {
    SymbolType resultType = isBinaryOperatorOverloadingFctAvailable(OP_FCT_MUL, lhs, rhs, node, opIdx);
    if (!resultType.is(TY_INVALID))
      return resultType;
  }

  return validateBinaryOperation(node, MUL_OP_RULES, ARRAY_LENGTH(MUL_OP_RULES), "*", lhs, rhs);
}

SymbolType OpRuleManager::getDivResultType(ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  // Check is there is an overloaded operator function available
  if (lhs.is(TY_STRUCT) || rhs.is(TY_STRUCT)) {
    SymbolType resultType = isBinaryOperatorOverloadingFctAvailable(OP_FCT_DIV, lhs, rhs, node, opIdx);
    if (!resultType.is(TY_INVALID))
      return resultType;
  }

  return validateBinaryOperation(node, DIV_OP_RULES, ARRAY_LENGTH(DIV_OP_RULES), "/", lhs, rhs);
}

SymbolType OpRuleManager::getRemResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  return validateBinaryOperation(node, REM_OP_RULES, ARRAY_LENGTH(REM_OP_RULES), "%", lhs, rhs);
}

SymbolType OpRuleManager::getPrefixMinusResultType(const ASTNode *node, SymbolType lhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();

  return validateUnaryOperation(node, PREFIX_MINUS_OP_RULES, ARRAY_LENGTH(PREFIX_MINUS_OP_RULES), "-", lhs);
}

SymbolType OpRuleManager::getPrefixPlusPlusResultType(const ASTNode *node, SymbolType lhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();

  return validateUnaryOperation(node, PREFIX_PLUS_PLUS_OP_RULES, ARRAY_LENGTH(PREFIX_PLUS_PLUS_OP_RULES), "++", lhs);
}

SymbolType OpRuleManager::getPrefixMinusMinusResultType(const ASTNode *node, SymbolType lhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();

  return validateUnaryOperation(node, PREFIX_MINUS_MINUS_OP_RULES, ARRAY_LENGTH(PREFIX_MINUS_MINUS_OP_RULES), "--", lhs);
}

SymbolType OpRuleManager::getPrefixNotResultType(const ASTNode *node, SymbolType lhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();

  return validateUnaryOperation(node, PREFIX_NOT_OP_RULES, ARRAY_LENGTH(PREFIX_NOT_OP_RULES), "!", lhs);
}

SymbolType OpRuleManager::getPrefixBitwiseNotResultType(const ASTNode *node, SymbolType lhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();

  return validateUnaryOperation(node, PREFIX_BITWISE_NOT_OP_RULES, ARRAY_LENGTH(PREFIX_BITWISE_NOT_OP_RULES), "~", lhs);
}

SymbolType OpRuleManager::getPrefixMulResultType(const ASTNode *node, SymbolType lhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();

  if (!lhs.isPtr())
    throw SemanticError(node, OPERATOR_WRONG_DATA_TYPE, "Cannot apply de-referencing operator on type " + lhs.getName(true));
  return lhs.getContainedTy();
}

SymbolType OpRuleManager::getPrefixBitwiseAndResultType(const ASTNode *node, SymbolType lhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();

  return lhs.toPointer(node);
}

SymbolType OpRuleManager::getPostfixPlusPlusResultType(const ASTNode *node, SymbolType lhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();

  return validateUnaryOperation(node, POSTFIX_PLUS_PLUS_OP_RULES, ARRAY_LENGTH(POSTFIX_PLUS_PLUS_OP_RULES), "++", lhs);
}

SymbolType OpRuleManager::getPostfixMinusMinusResultType(const ASTNode *node, SymbolType lhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();

  return validateUnaryOperation(node, POSTFIX_MINUS_MINUS_OP_RULES, ARRAY_LENGTH(POSTFIX_MINUS_MINUS_OP_RULES), "--", lhs);
}

SymbolType OpRuleManager::getCastResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs, size_t opIdx) {
  lhs = lhs.removeReferenceWrappers();
  rhs = rhs.removeReferenceWrappers();

  // Allow casts string -> char*  and string -> char[]
  if (lhs.isOneOf({TY_PTR, TY_ARRAY}) && lhs.getContainedTy().is(TY_CHAR) && rhs.is(TY_STRING))
    return lhs;
  // Allow casts char* -> string and char[] -> string
  if (lhs.is(TY_STRING) && rhs.isOneOf({TY_PTR, TY_ARRAY}) && rhs.getContainedTy().is(TY_CHAR))
    return lhs;
  // Allow casts any* -> any*
  if (lhs.isPtr() && rhs.isPtr()) {
    ensureUnsafeAllowed(node, "(cast)", lhs, rhs);
    return lhs;
  }
  // Check primitive type combinations
  return validateBinaryOperation(node, CAST_OP_RULES, ARRAY_LENGTH(CAST_OP_RULES), "(cast)", lhs, rhs);
}

SymbolType OpRuleManager::isBinaryOperatorOverloadingFctAvailable(const char *const fctName, SymbolType &lhs, SymbolType &rhs,
                                                                  ASTNode *node, size_t opIdx) {
  const size_t manIdx = typeChecker->manIdx;

  const NameRegistryEntry *registryEntry = typeChecker->sourceFile->getNameRegistryEntry(fctName);
  if (!registryEntry)
    return SymbolType(TY_INVALID);
  Scope *calleeParentScope = registryEntry->targetScope;

  // Get callee
  const SymbolType localLhs = typeChecker->mapLocalTypeToImportedScopeType(calleeParentScope, lhs);
  const SymbolType localRhs = typeChecker->mapLocalTypeToImportedScopeType(calleeParentScope, rhs);
  Function *callee = FunctionManager::matchFunction(calleeParentScope, fctName, SymbolType(TY_DYN), {localLhs, localRhs}, node);

  // Return invalid type if the callee was not found
  if (!callee)
    return SymbolType(TY_INVALID);

  // Save the pointer to the operator function in the AST node
  assert(node->opFct.size() > manIdx);
  if (node->opFct.at(manIdx).size() <= opIdx)
    node->opFct.at(manIdx).resize(opIdx + 1);
  node->opFct.at(manIdx).at(opIdx) = callee;

  // Check if we need to request a re-visit, because the function body was not type-checked yet
  if (!callee->alreadyTypeChecked)
    typeChecker->reVisitRequested = true;

  // Check if the called function has sufficient visibility
  const bool isImported = calleeParentScope->isImportedBy(typeChecker->rootScope);
  SymbolTableEntry *calleeEntry = callee->entry;
  if (isImported && !calleeEntry->specifiers.isPublic())
    throw SemanticError(node, INSUFFICIENT_VISIBILITY,
                        "Overloaded operator '" + callee->getSignature() + "' has insufficient visibility");

  // Procedures always have the return type 'bool'
  if (callee->isProcedure() || callee->returnType.is(TY_DYN))
    return SymbolType(TY_BOOL);

  SymbolType localReturnType = typeChecker->mapImportedScopeTypeToLocalType(calleeParentScope, callee->returnType);
  return localReturnType;
}

SymbolType OpRuleManager::validateBinaryOperation(const ASTNode *node, const BinaryOpRule opRules[], size_t opRulesSize,
                                                  const char *name, const SymbolType &lhs, const SymbolType &rhs) {
  for (size_t i = 0; i < opRulesSize; i++) {
    const BinaryOpRule &rule = opRules[i];
    if (std::get<0>(rule) == lhs.getSuperType() && std::get<1>(rule) == rhs.getSuperType())
      return SymbolType(SymbolSuperType(std::get<2>(rule)));
  }
  throw printErrorMessageBinary(node, name, lhs, rhs);
}

SymbolType OpRuleManager::validateUnaryOperation(const ASTNode *node, const UnaryOpRule opRules[], size_t opRulesSize,
                                                 const char *name, const SymbolType &lhs) {
  for (size_t i = 0; i < opRulesSize; i++) {
    const UnaryOpRule &rule = opRules[i];
    if (std::get<0>(rule) == lhs.getSuperType())
      return SymbolType(SymbolSuperType(std::get<1>(rule)));
  }
  throw printErrorMessageUnary(node, name, lhs);
}

void OpRuleManager::ensureUnsafeAllowed(const ASTNode *node, const char *name, const SymbolType &lhs,
                                        const SymbolType &rhs) const {
  if (typeChecker->currentScope->doesAllowUnsafeOperations())
    return;
  // Print error message
  const std::string lhsName = lhs.getName(true);
  const std::string rhsName = rhs.getName(true);
  const std::string errorMsg = "Cannot apply '" + std::string(name) + "' operator on types " + lhsName + " and " + rhsName +
                               " as this is an unsafe operation. Please use unsafe blocks if you know what you are doing.";
  throw SemanticError(node, UNSAFE_OPERATION_IN_SAFE_CONTEXT, errorMsg);
}

SemanticError OpRuleManager::printErrorMessageBinary(const ASTNode *node, const char *name, const SymbolType &lhs,
                                                     const SymbolType &rhs) {
  return {node, OPERATOR_WRONG_DATA_TYPE,
          "Cannot apply '" + std::string(name) + "' operator on types " + lhs.getName(true) + " and " + rhs.getName(true)};
}

SemanticError OpRuleManager::printErrorMessageUnary(const ASTNode *node, const char *name, const SymbolType &lhs) {
  return {node, OPERATOR_WRONG_DATA_TYPE, "Cannot apply '" + std::string(name) + "' operator on type " + lhs.getName(true)};
}

} // namespace spice::compiler
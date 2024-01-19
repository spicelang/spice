// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "OpRuleManager.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <global/RuntimeModuleManager.h>
#include <typechecker/TypeChecker.h>

namespace spice::compiler {

OpRuleManager::OpRuleManager(TypeChecker *typeChecker)
    : typeChecker(typeChecker), resourceManager(typeChecker->resourceManager) {}

SymbolType OpRuleManager::getAssignResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, bool isDecl,
                                              const char *errMsgPrefix) {
  // Check if we try to assign a constant value
  if (!isDecl)
    ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  SymbolType lhsType = lhs.type;
  SymbolType rhsType = rhs.type;

  // Skip type compatibility check if the lhs is of type dyn -> perform type inference
  if (lhsType.is(TY_DYN))
    return rhsType;
  // Allow pointers and arrays of the same type straight away
  if (lhsType.isOneOf({TY_PTR, TY_REF}) && lhsType.matches(rhsType, false, false, true))
    return rhsType;
  // Allow type to ref type of the same contained type straight away
  if (lhsType.isRef() && lhsType.getContainedTy().matches(rhsType, false, false, true)) {
    if (isDecl && !lhsType.canBind(rhsType, rhs.isTemporary()))
      throw SemanticError(node, TEMP_TO_NON_CONST_REF, "Temporary values can only be bound to const reference parameters");
    return lhsType;
  }
  // Allow ref type to type of the same contained type straight away
  if (rhsType.isRef()) {
    // If this is const ref, remove both: the reference and the constness
    SymbolType rhsModified = rhsType.getContainedTy();
    rhsModified.specifiers.isConst = false;

    if (lhsType.matches(rhsModified, false, !lhsType.isRef(), true))
      return lhsType;
  }
  // Allow arrays, structs, interfaces, functions, procedures of the same type straight away
  if (lhsType.isOneOf({TY_ARRAY, TY_STRUCT, TY_INTERFACE, TY_FUNCTION, TY_PROCEDURE}) &&
      lhsType.matches(rhsType, false, true, true))
    return rhsType;
  // Allow array to pointer
  if (lhsType.isPtr() && rhsType.isArray() && lhsType.getContainedTy().matches(rhsType.getContainedTy(), false, false, true))
    return lhsType;
  // Allow dyn[] (empty array literal) to any array
  if (lhsType.isArray() && rhsType.isArrayOf(TY_DYN))
    return lhsType;
  // Allow char* = string
  if (lhsType.isPtrOf(TY_CHAR) && rhsType.is(TY_STRING) && lhsType.specifiers == rhsType.specifiers)
    return lhsType;
  // Allow interface* = struct* that implements this interface
  const bool sameChainDepth = lhsType.typeChain.size() == rhsType.typeChain.size();
  if (lhsType.isPtr() && rhsType.isPtr() && sameChainDepth && lhsType.isBaseType(TY_INTERFACE) && rhsType.isBaseType(TY_STRUCT)) {
    SymbolType lhsTypeCopy = lhsType;
    SymbolType rhsTypeCopy = rhsType;
    SymbolType::unwrapBoth(lhsTypeCopy, rhsTypeCopy);

    Struct *spiceStruct = rhsTypeCopy.getStruct(node);
    assert(spiceStruct != nullptr);
    for (const SymbolType &interfaceType : spiceStruct->interfaceTypes) {
      assert(interfaceType.is(TY_INTERFACE));
      if (lhsTypeCopy.matches(interfaceType, false, false, true))
        return lhsType;
    }
  }
  // Check primitive type combinations
  return validateBinaryOperation(node, ASSIGN_OP_RULES, ARRAY_LENGTH(ASSIGN_OP_RULES), "=", lhsType, rhsType, true, errMsgPrefix);
}

SymbolType OpRuleManager::getFieldAssignResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, bool imm,
                                                   bool isDecl) {
  // Check if we try to assign a constant value
  if (!isDecl)
    ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  SymbolType lhsType = lhs.type;
  SymbolType rhsType = rhs.type;

  // Allow pointers and arrays of the same type straight away
  if (lhsType.isOneOf({TY_PTR, TY_ARRAY, TY_STRUCT}) && lhsType == rhsType)
    return rhsType;
  // Allow struct of the same type straight away
  if (lhsType.is(TY_STRUCT) && lhsType.matches(rhsType, false, true, true))
    return rhsType;
  // Allow type to ref type of the same contained type straight away
  if (lhsType.isRef() && lhsType.getContainedTy().matches(rhsType, false, false, true)) {
    if (isDecl && !lhsType.canBind(rhsType, rhs.isTemporary()))
      throw SemanticError(node, TEMP_TO_NON_CONST_REF, "Temporary values can only be bound to const reference parameters");
    return lhsType;
  }
  // Allow ref type to type of the same contained type straight away
  if (rhsType.isRef() && lhsType.matches(rhsType.getContainedTy(), false, false, true))
    return lhsType;
  // Allow immediate value to const ref of the same contained type straight away
  if (lhsType.isRef() && lhsType.getContainedTy().isConst() && imm)
    return rhsType;
  // Allow array to pointer
  if (lhsType.isPtr() && rhsType.isArray() && lhsType.getContainedTy().matches(rhsType.getContainedTy(), false, false, true))
    return lhsType;
  // Allow dyn[] (empty array literal) to any array
  if (lhsType.isArray() && rhsType.isArrayOf(TY_DYN))
    return lhsType;
  // Allow char* = string
  if (lhsType.isPtrOf(TY_CHAR) && rhsType.is(TY_STRING) && lhsType.specifiers == rhsType.specifiers)
    return lhsType;
  // Check primitive type combinations
  return validateBinaryOperation(node, ASSIGN_OP_RULES, ARRAY_LENGTH(ASSIGN_OP_RULES), "=", lhsType, rhsType, true,
                                 ERROR_FIELD_ASSIGN);
}

ExprResult OpRuleManager::getPlusEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_PLUS_EQUAL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  // Check if this is an unsafe operation
  if (lhsType.isPtr() && rhsType.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    ensureUnsafeAllowed(node, "+=", lhsType, rhsType);
    return {lhs};
  }

  return {validateBinaryOperation(node, PLUS_EQUAL_OP_RULES, ARRAY_LENGTH(PLUS_EQUAL_OP_RULES), "+=", lhsType, rhsType)};
}

ExprResult OpRuleManager::getMinusEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_MINUS_EQUAL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  // Check if this is an unsafe operation
  if (lhsType.isPtr() && rhsType.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    ensureUnsafeAllowed(node, "-=", lhsType, rhsType);
    return {lhs};
  }

  return {validateBinaryOperation(node, MINUS_EQUAL_OP_RULES, ARRAY_LENGTH(MINUS_EQUAL_OP_RULES), "-=", lhsType, rhsType)};
}

ExprResult OpRuleManager::getMulEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_MUL_EQUAL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return {validateBinaryOperation(node, MUL_EQUAL_OP_RULES, ARRAY_LENGTH(MUL_EQUAL_OP_RULES), "*=", lhsType, rhsType)};
}

ExprResult OpRuleManager::getDivEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_DIV_EQUAL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return {validateBinaryOperation(node, DIV_EQUAL_OP_RULES, ARRAY_LENGTH(DIV_EQUAL_OP_RULES), "/=", lhsType, rhsType)};
}

SymbolType OpRuleManager::getRemEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, REM_EQUAL_OP_RULES, ARRAY_LENGTH(REM_EQUAL_OP_RULES), "%=", lhsType, rhsType);
}

SymbolType OpRuleManager::getSHLEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, SHL_EQUAL_OP_RULES, ARRAY_LENGTH(SHL_EQUAL_OP_RULES), "<<=", lhsType, rhsType);
}

SymbolType OpRuleManager::getSHREqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, SHR_EQUAL_OP_RULES, ARRAY_LENGTH(SHR_EQUAL_OP_RULES), ">>=", lhsType, rhsType);
}

SymbolType OpRuleManager::getAndEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, AND_EQUAL_OP_RULES, ARRAY_LENGTH(AND_EQUAL_OP_RULES), "&=", lhsType, rhsType);
}

SymbolType OpRuleManager::getOrEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, OR_EQUAL_OP_RULES, ARRAY_LENGTH(OR_EQUAL_OP_RULES), "|=", lhsType, rhsType);
}

SymbolType OpRuleManager::getXorEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, XOR_EQUAL_OP_RULES, ARRAY_LENGTH(XOR_EQUAL_OP_RULES), "^=", lhsType, rhsType);
}

SymbolType OpRuleManager::getLogicalOrResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, LOGICAL_OR_OP_RULES, ARRAY_LENGTH(LOGICAL_OR_OP_RULES), "||", lhsType, rhsType);
}

SymbolType OpRuleManager::getLogicalAndResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, LOGICAL_AND_OP_RULES, ARRAY_LENGTH(LOGICAL_AND_OP_RULES), "&&", lhsType, rhsType);
}

SymbolType OpRuleManager::getBitwiseOrResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, BITWISE_OR_OP_RULES, ARRAY_LENGTH(BITWISE_OR_OP_RULES), "|", lhsType, rhsType);
}

SymbolType OpRuleManager::getBitwiseXorResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, BITWISE_XOR_OP_RULES, ARRAY_LENGTH(BITWISE_XOR_OP_RULES), "^", lhsType, rhsType);
}

SymbolType OpRuleManager::getBitwiseAndResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, BITWISE_AND_OP_RULES, ARRAY_LENGTH(BITWISE_AND_OP_RULES), "&", lhsType, rhsType);
}

ExprResult OpRuleManager::getEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_EQUAL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  // Allow 'pointer == pointer' straight away
  if (lhsType.isPtr() && rhsType.isPtr())
    return ExprResult(SymbolType(TY_BOOL));

  // Allow 'pointer == int' straight away
  if (lhsType.isPtr() && rhsType.is(TY_INT))
    return ExprResult(SymbolType(TY_BOOL));

  // Allow 'string == char*' and vice versa straight away
  if ((lhsType.is(TY_STRING) && rhsType.isPtrOf(TY_CHAR)) || (lhsType.isPtrOf(TY_CHAR) && rhsType.is(TY_STRING)))
    return ExprResult(SymbolType(TY_BOOL));

  // Check primitive type combinations
  return ExprResult(validateBinaryOperation(node, EQUAL_OP_RULES, ARRAY_LENGTH(EQUAL_OP_RULES), "==", lhsType, rhsType));
}

ExprResult OpRuleManager::getNotEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_NOT_EQUAL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  // Allow 'pointer != pointer' straight away
  if (lhsType.isPtr() && rhsType.isPtr())
    return ExprResult(SymbolType(TY_BOOL));

  // Allow 'pointer != int' straight away
  if (lhsType.isPtr() && rhsType.is(TY_INT))
    return ExprResult(SymbolType(TY_BOOL));

  // Allow 'string != char*' and vice versa straight away
  if ((lhsType.is(TY_STRING) && rhsType.isPtrOf(TY_CHAR)) || (lhsType.isPtrOf(TY_CHAR) && rhsType.is(TY_STRING)))
    return ExprResult(SymbolType(TY_BOOL));

  // Check primitive type combinations
  return ExprResult(validateBinaryOperation(node, NOT_EQUAL_OP_RULES, ARRAY_LENGTH(NOT_EQUAL_OP_RULES), "!=", lhsType, rhsType));
}

SymbolType OpRuleManager::getLessResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, LESS_OP_RULES, ARRAY_LENGTH(LESS_OP_RULES), "<", lhsType, rhsType);
}

SymbolType OpRuleManager::getGreaterResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, GREATER_OP_RULES, ARRAY_LENGTH(GREATER_OP_RULES), ">", lhsType, rhsType);
}

SymbolType OpRuleManager::getLessEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, LESS_EQUAL_OP_RULES, ARRAY_LENGTH(LESS_EQUAL_OP_RULES), "<=", lhsType, rhsType);
}

SymbolType OpRuleManager::getGreaterEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, GREATER_EQUAL_OP_RULES, ARRAY_LENGTH(GREATER_EQUAL_OP_RULES), ">=", lhsType, rhsType);
}

ExprResult OpRuleManager::getShiftLeftResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_SHL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return {validateBinaryOperation(node, SHIFT_LEFT_OP_RULES, ARRAY_LENGTH(SHIFT_LEFT_OP_RULES), "<<", lhsType, rhsType)};
}

ExprResult OpRuleManager::getShiftRightResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_SHR, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return {validateBinaryOperation(node, SHIFT_RIGHT_OP_RULES, ARRAY_LENGTH(SHIFT_RIGHT_OP_RULES), ">>", lhsType, rhsType)};
}

ExprResult OpRuleManager::getPlusResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  ExprResult result = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_PLUS, {lhs, rhs}, opIdx);
  if (!result.type.is(TY_INVALID))
    return result;

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  // Allow any* + <int/long/short>
  if (lhsType.isPtr() && rhsType.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    ensureUnsafeAllowed(node, "+", lhsType, rhsType);
    return {lhsType};
  }
  // Allow <int/long/short> + any*
  if (lhsType.isOneOf({TY_INT, TY_LONG, TY_SHORT}) && rhsType.isPtr()) {
    ensureUnsafeAllowed(node, "+", lhsType, rhsType);
    return {rhsType};
  }

  return {validateBinaryOperation(node, PLUS_OP_RULES, ARRAY_LENGTH(PLUS_OP_RULES), "+", lhsType, rhsType)};
}

ExprResult OpRuleManager::getMinusResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_MINUS, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  // Allow any* - <int/long/short>
  if (lhsType.isPtr() && rhsType.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    ensureUnsafeAllowed(node, "-", lhsType, rhsType);
    return {lhs};
  }
  // Allow <int/long/short> - any*
  if (lhsType.isOneOf({TY_INT, TY_LONG, TY_SHORT}) && rhsType.isPtr()) {
    ensureUnsafeAllowed(node, "-", lhsType, rhsType);
    return {rhs};
  }

  return {validateBinaryOperation(node, MINUS_OP_RULES, ARRAY_LENGTH(MINUS_OP_RULES), "-", lhsType, rhsType)};
}

ExprResult OpRuleManager::getMulResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_MUL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return {validateBinaryOperation(node, MUL_OP_RULES, ARRAY_LENGTH(MUL_OP_RULES), "*", lhsType, rhsType)};
}

ExprResult OpRuleManager::getDivResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_DIV, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return {validateBinaryOperation(node, DIV_OP_RULES, ARRAY_LENGTH(DIV_OP_RULES), "/", lhsType, rhsType)};
}

ExprResult OpRuleManager::getRemResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const SymbolType lhsType = lhs.type.removeReferenceWrapper();
  const SymbolType rhsType = rhs.type.removeReferenceWrapper();

  return {validateBinaryOperation(node, REM_OP_RULES, ARRAY_LENGTH(REM_OP_RULES), "%", lhsType, rhsType)};
}

SymbolType OpRuleManager::getPrefixMinusResultType(const ASTNode *node, const ExprResult &lhs) {
  // Remove reference wrappers
  SymbolType lhsType = lhs.type.removeReferenceWrapper();

  return validateUnaryOperation(node, PREFIX_MINUS_OP_RULES, ARRAY_LENGTH(PREFIX_MINUS_OP_RULES), "-", lhsType);
}

SymbolType OpRuleManager::getPrefixPlusPlusResultType(const ASTNode *node, const ExprResult &lhs) {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  SymbolType lhsType = lhs.type.removeReferenceWrapper();

  return validateUnaryOperation(node, PREFIX_PLUS_PLUS_OP_RULES, ARRAY_LENGTH(PREFIX_PLUS_PLUS_OP_RULES), "++", lhsType);
}

SymbolType OpRuleManager::getPrefixMinusMinusResultType(const ASTNode *node, const ExprResult &lhs) {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  SymbolType lhsType = lhs.type.removeReferenceWrapper();

  return validateUnaryOperation(node, PREFIX_MINUS_MINUS_OP_RULES, ARRAY_LENGTH(PREFIX_MINUS_MINUS_OP_RULES), "--", lhsType);
}

SymbolType OpRuleManager::getPrefixNotResultType(const ASTNode *node, const ExprResult &lhs) {
  // Remove reference wrappers
  SymbolType lhsType = lhs.type.removeReferenceWrapper();

  return validateUnaryOperation(node, PREFIX_NOT_OP_RULES, ARRAY_LENGTH(PREFIX_NOT_OP_RULES), "!", lhsType);
}

SymbolType OpRuleManager::getPrefixBitwiseNotResultType(const ASTNode *node, const ExprResult &lhs) {
  // Remove reference wrappers
  SymbolType lhsType = lhs.type.removeReferenceWrapper();

  return validateUnaryOperation(node, PREFIX_BITWISE_NOT_OP_RULES, ARRAY_LENGTH(PREFIX_BITWISE_NOT_OP_RULES), "~", lhsType);
}

SymbolType OpRuleManager::getPrefixMulResultType(const ASTNode *node, const ExprResult &lhs) {
  // Remove reference wrappers
  SymbolType lhsType = lhs.type.removeReferenceWrapper();

  if (!lhsType.isPtr())
    throw SemanticError(node, OPERATOR_WRONG_DATA_TYPE, "Cannot apply de-referencing operator on type " + lhsType.getName(true));
  return lhsType.getContainedTy();
}

SymbolType OpRuleManager::getPrefixBitwiseAndResultType(const ASTNode *node, const ExprResult &lhs) {
  // Remove reference wrappers
  SymbolType lhsType = lhs.type.removeReferenceWrapper();

  return lhsType.toPointer(node);
}

ExprResult OpRuleManager::getPostfixPlusPlusResultType(ASTNode *node, const ExprResult &lhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  ExprResult resultType = isOperatorOverloadingFctAvailable<1>(node, OP_FCT_POSTFIX_PLUS_PLUS, {lhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  SymbolType lhsType = lhs.type.removeReferenceWrapper();

  return ExprResult(
      validateUnaryOperation(node, POSTFIX_PLUS_PLUS_OP_RULES, ARRAY_LENGTH(POSTFIX_PLUS_PLUS_OP_RULES), "++", lhsType));
}

ExprResult OpRuleManager::getPostfixMinusMinusResultType(ASTNode *node, const ExprResult &lhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  ExprResult resultType = isOperatorOverloadingFctAvailable<1>(node, OP_FCT_POSTFIX_MINUS_MINUS, {lhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  SymbolType lhsType = lhs.type.removeReferenceWrapper();

  return {validateUnaryOperation(node, POSTFIX_MINUS_MINUS_OP_RULES, ARRAY_LENGTH(POSTFIX_MINUS_MINUS_OP_RULES), "--", lhsType)};
}

SymbolType OpRuleManager::getCastResultType(const ASTNode *node, SymbolType lhsType, const ExprResult &rhs) {
  // Remove reference wrappers
  lhsType = lhsType.removeReferenceWrapper();
  SymbolType rhsType = rhs.type.removeReferenceWrapper();

  // Only allow to cast the 'heap' specifier away, if we are in unsafe mode
  if (lhsType.specifiers.isHeap != rhsType.specifiers.isHeap)
    ensureUnsafeAllowed(node, "(cast)", lhsType, rhsType);

  // Allow casts string -> char* and string -> char[]
  if (lhsType.isOneOf({TY_PTR, TY_ARRAY}) && lhsType.getContainedTy().is(TY_CHAR) && rhsType.is(TY_STRING))
    return lhsType;
  // Allow casts char* -> string and char[] -> string
  if (lhsType.is(TY_STRING) && rhsType.isOneOf({TY_PTR, TY_ARRAY}) && rhsType.getContainedTy().is(TY_CHAR))
    return lhsType;
  // Allow casts any* -> any*
  if (lhsType.isOneOf({TY_PTR, TY_STRING}) && rhsType.isOneOf({TY_PTR, TY_STRING})) {
    if (lhsType != rhsType)
      ensureUnsafeAllowed(node, "(cast)", lhsType, rhsType);
    return lhsType;
  }
  // Check primitive type combinations
  return validateBinaryOperation(node, CAST_OP_RULES, ARRAY_LENGTH(CAST_OP_RULES), "(cast)", lhsType, rhsType, true);
}

template <size_t N>
ExprResult OpRuleManager::isOperatorOverloadingFctAvailable(ASTNode *node, const char *const fctName,
                                                            const std::array<ExprResult, N> &op, size_t opIdx) {
  Scope *calleeParentScope = nullptr;
  Function *callee = nullptr;
  for (const auto &[_, sourceFile] : typeChecker->resourceManager.sourceFiles) {
    // Check if there is a registered operator function
    if (!sourceFile->getNameRegistryEntry(fctName))
      continue;

    // Match callees in the global scope of this source file
    calleeParentScope = sourceFile->globalScope.get();
    const SymbolType thisType(TY_DYN);
    ArgList args(N);
    args[0] = {typeChecker->mapLocalTypeToImportedScopeType(calleeParentScope, op[0].type), op[0].isTemporary()};
    if constexpr (N == 2)
      args[1] = {typeChecker->mapLocalTypeToImportedScopeType(calleeParentScope, op[1].type), op[1].isTemporary()};
    callee = FunctionManager::matchFunction(calleeParentScope, fctName, thisType, args, {}, false, node);
    if (callee)
      break;
  }

  // Return invalid type if the callee was not found
  if (!callee)
    return ExprResult(SymbolType(TY_INVALID));
  assert(calleeParentScope != nullptr);

  // Save the pointer to the operator function in the AST node
  std::vector<const Function *> &opFctPointers = typeChecker->getOpFctPointers(node);
  if (opFctPointers.size() <= opIdx)
    opFctPointers.resize(opIdx + 1, nullptr);
  opFctPointers.at(opIdx) = callee;

  // Check if we need to request a re-visit, because the function body was not type-checked yet
  const bool isCallToImportedSourceFile = callee->entry->scope->isImportedBy(typeChecker->rootScope);
  if (!callee->alreadyTypeChecked && !isCallToImportedSourceFile)
    typeChecker->reVisitRequested = true;

  // Check if the called function has sufficient visibility
  const bool isImported = calleeParentScope->isImportedBy(typeChecker->rootScope);
  SymbolTableEntry *calleeEntry = callee->entry;
  if (isImported && !calleeEntry->getType().isPublic())
    throw SemanticError(node, INSUFFICIENT_VISIBILITY,
                        "Overloaded operator '" + callee->getSignature() + "' has insufficient visibility");

  // Procedures always have the return type 'bool'
  if (callee->isProcedure())
    return ExprResult(SymbolType(TY_BOOL));

  // Add anonymous symbol to keep track of deallocation
  SymbolTableEntry *anonymousSymbol = nullptr;
  if (callee->returnType.is(TY_STRUCT))
    anonymousSymbol = typeChecker->currentScope->symbolTable.insertAnonymous(callee->returnType, node, opIdx);

  return {typeChecker->mapImportedScopeTypeToLocalType(calleeParentScope, callee->returnType), anonymousSymbol};
}

SymbolType OpRuleManager::validateUnaryOperation(const ASTNode *node, const UnaryOpRule opRules[], size_t opRulesSize,
                                                 const char *name, const SymbolType &lhs) {
  for (size_t i = 0; i < opRulesSize; i++) {
    const UnaryOpRule &rule = opRules[i];
    if (std::get<0>(rule) == lhs.getSuperType())
      return SymbolType(SymbolSuperType(std::get<1>(rule)));
  }
  throw getExceptionUnary(node, name, lhs);
}

SymbolType OpRuleManager::validateBinaryOperation(const ASTNode *node, const BinaryOpRule opRules[], size_t opRulesSize,
                                                  const char *name, const SymbolType &lhs, const SymbolType &rhs,
                                                  bool preserveSpecifiersFromLhs, const char *customMessagePrefix) {
  for (size_t i = 0; i < opRulesSize; i++) {
    const BinaryOpRule &rule = opRules[i];
    if (std::get<0>(rule) == lhs.getSuperType() && std::get<1>(rule) == rhs.getSuperType()) {
      SymbolType resultType = SymbolType(SymbolSuperType(std::get<2>(rule)));
      if (preserveSpecifiersFromLhs)
        resultType.specifiers = lhs.specifiers;
      return resultType;
    }
  }
  throw getExceptionBinary(node, name, lhs, rhs, customMessagePrefix);
}

SemanticError OpRuleManager::getExceptionUnary(const ASTNode *node, const char *name, const SymbolType &lhs) {
  return {node, OPERATOR_WRONG_DATA_TYPE, "Cannot apply '" + std::string(name) + "' operator on type " + lhs.getName(true)};
}

SemanticError OpRuleManager::getExceptionBinary(const ASTNode *node, const char *name, const SymbolType &lhs,
                                                const SymbolType &rhs, const char *messagePrefix) {
  // Build error message
  std::stringstream errorMsg;
  if (strlen(messagePrefix) != 0)
    errorMsg << messagePrefix << ". Expected " << lhs.getName(true) << " but got " << rhs.getName(true);
  else
    errorMsg << "Cannot apply '" << name << "' operator on types " << lhs.getName(true) << " and " << rhs.getName(true);

  // Return the exception
  return {node, OPERATOR_WRONG_DATA_TYPE, errorMsg.str()};
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
  SOFT_ERROR_VOID(node, UNSAFE_OPERATION_IN_SAFE_CONTEXT, errorMsg)
}

void OpRuleManager::ensureNoConstAssign(const ASTNode *node, const SymbolType &lhs) {
  // Check if we try to assign a constant value
  if (lhs.removeReferenceWrapper().isConst()) {
    const std::string errorMessage = "Trying to assign value to an immutable variable of type " + lhs.getName(true);
    throw SemanticError(node, REASSIGN_CONST_VARIABLE, errorMessage);
  }
}

} // namespace spice::compiler
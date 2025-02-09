// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "OpRuleManager.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <global/GlobalResourceManager.h>
#include <global/RuntimeModuleManager.h>
#include <typechecker/MacroDefs.h>
#include <typechecker/TypeChecker.h>

namespace spice::compiler {

OpRuleManager::OpRuleManager(TypeChecker *typeChecker)
    : typeChecker(typeChecker), resourceManager(typeChecker->resourceManager) {}

std::pair<QualType, Function *> OpRuleManager::getAssignResultType(const ASTNode *node, const ExprResult &lhs,
                                                                   const ExprResult &rhs, bool isDecl, bool isReturn,
                                                                   const char *errMsgPrefix) const {
  // Retrieve types
  const QualType lhsType = lhs.type;
  const QualType rhsType = rhs.type;

  // Check if lhs is a temporary
  if (lhs.isTemporary())
    throw SemanticError(node, OPERATOR_WRONG_DATA_TYPE, "Cannot assign to a temporary value");

  // Skip type compatibility check if the lhs is of type dyn -> perform type inference
  if (lhsType.is(TY_DYN))
    return {rhsType, nullptr};

  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhsType, isDecl, isReturn);

  // Allow pointers and references of the same type straight away
  if (lhsType.isOneOf({TY_PTR, TY_REF}) && lhsType.matches(rhsType, false, false, true)) {
    // If we perform a heap x* = heap x* assignment, we need set the right hand side to MOVED
    if (rhs.entry && lhsType.isPtr() && lhsType.isHeap() && rhsType.removeReferenceWrapper().isPtr() && rhsType.isHeap())
      rhs.entry->updateState(MOVED, node);
    return {rhsType, nullptr};
  }
  // Allow ref type to type of the same contained type straight away
  if (rhsType.isRef()) {
    // If this is const ref, remove both: the reference and the constness
    const QualType rhsModified = rhsType.getContained().toNonConst();
    if (lhsType.matches(rhsModified, false, false, true)) {
      // Check if we support nrvo. If yes, skip the implicit copy ctor call
      const bool supportsNRVO = isReturn && !rhs.isTemporary();
      Function *copyCtor = nullptr;
      if (rhsModified.is(TY_STRUCT) && !supportsNRVO)
        copyCtor = typeChecker->implicitlyCallStructCopyCtor(rhsModified, node);
      return {lhsType, copyCtor};
    }
  }
  // Allow arrays, structs, interfaces, functions, procedures of the same type straight away
  if (lhsType.isOneOf({TY_ARRAY, TY_INTERFACE, TY_FUNCTION, TY_PROCEDURE}) && lhsType.matches(rhsType, false, true, true))
    return {rhsType, nullptr};
  // Allow struct of the same type straight away
  if (lhsType.is(TY_STRUCT) && lhsType.matches(rhsType, false, true, true)) {
    // Check if we support nrvo. If yes, skip the implicit copy ctor call
    const bool supportsNRVO = isReturn && !rhs.isTemporary();
    Function *copyCtor = nullptr;
    if (rhs.entry != nullptr && !supportsNRVO && !isReturn && !rhs.isTemporary())
      copyCtor = typeChecker->implicitlyCallStructCopyCtor(rhsType, rhs.entry->declNode);
    return {rhsType, copyCtor};
  }

  // Check common type combinations
  const QualType resultType = getAssignResultTypeCommon(node, lhs, rhs, isDecl, isReturn);
  if (!resultType.is(TY_INVALID))
    return {resultType, nullptr};

  // Check primitive type combinations
  const QualType binOpType =
      validateBinaryOperation(node, ASSIGN_OP_RULES, std::size(ASSIGN_OP_RULES), "=", lhsType, rhsType, true, errMsgPrefix);
  return {binOpType, nullptr};
}

QualType OpRuleManager::getFieldAssignResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, bool imm,
                                                 bool isDecl) const {
  // Retrieve types
  const QualType lhsType = lhs.type;
  const QualType rhsType = rhs.type;
  assert(!lhsType.is(TY_DYN));

  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhsType, isDecl);

  // Allow pointers, arrays and structs of the same type straight away
  if (lhsType.isOneOf({TY_PTR, TY_ARRAY}) && lhsType == rhsType) {
    // If we perform a heap x* = heap x* assignment, we need set the right hand side to MOVED
    if (rhs.entry && lhsType.isPtr() && lhsType.isHeap() && rhsType.removeReferenceWrapper().isPtr() && rhsType.isHeap())
      rhs.entry->updateState(MOVED, node);
    return rhsType;
  }
  // Allow struct of the same type straight away
  if (lhsType.is(TY_STRUCT) && lhsType.matches(rhsType, false, true, true)) {
    if (!rhs.isTemporary())
      typeChecker->implicitlyCallStructCopyCtor(rhs.entry, rhs.entry->declNode);
    return rhsType;
  }
  // Allow ref type to type of the same contained type straight away
  if (rhsType.isRef() && lhsType.matches(rhsType.getContained(), false, false, true)) {
    // In case of a return expression, we perform temp stealing
    if (rhsType.getContained().is(TY_STRUCT) && !rhs.isTemporary())
      typeChecker->implicitlyCallStructCopyCtor(rhs.entry, rhs.entry->declNode);
    return lhsType;
  }
  // Allow const ref type to type of the same contained type straight away
  if (rhsType.isConstRef() && lhsType.matches(rhsType.getContained().toNonConst(), false, false, true))
    return lhsType;
  // Allow immediate value to const ref of the same contained type straight away
  if (lhsType.isConstRef() && imm)
    return rhsType;

  // Check common type combinations
  const QualType resultType = getAssignResultTypeCommon(node, lhs, rhs, isDecl, false);
  if (!resultType.is(TY_INVALID))
    return resultType;

  // Check primitive type combinations
  return validateBinaryOperation(node, ASSIGN_OP_RULES, std::size(ASSIGN_OP_RULES), "=", lhsType, rhsType, true,
                                 ERROR_FIELD_ASSIGN);
}

QualType OpRuleManager::getAssignResultTypeCommon(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, bool isDecl,
                                                  bool isReturn) {
  // Retrieve types
  const QualType lhsType = lhs.type;
  const QualType rhsType = rhs.type;

  // Allow type to ref type of the same contained type straight away
  if (lhsType.isRef() && lhsType.getContained().matches(rhsType, false, false, true)) {
    const bool isDeclOrReturn = isDecl || isReturn;
    if (isDeclOrReturn && !lhsType.canBind(rhsType, rhs.isTemporary()))
      throw SemanticError(node, TEMP_TO_NON_CONST_REF, "Temporary values can only be bound to const reference variables/fields");
    if (isReturn && rhs.isTemporary())
      throw SemanticError(node, RETURN_OF_TEMPORARY_VALUE, "Cannot return reference to temporary value");
    return lhsType;
  }
  // Allow char* = string
  if (lhsType.isPtrTo(TY_CHAR) && rhsType.is(TY_STRING) && lhsType.getQualifiers() == rhsType.getQualifiers())
    return lhsType;
  // Allow array to pointer
  if (lhsType.isPtr() && rhsType.isArray() && lhsType.getContained().matches(rhsType.getContained(), false, false, true))
    return lhsType;
  // Allow interface* = struct* or interface& = struct that implements this interface
  const bool sameChainDepth = Type::hasSameTypeChainDepth(lhsType.getType(), rhsType.getType());
  const bool typesCompatible = (lhsType.isPtr() && rhsType.isPtr() && sameChainDepth) || lhsType.isRef();
  if (typesCompatible && lhsType.isBase(TY_INTERFACE) && rhsType.isBase(TY_STRUCT)) {
    QualType lhsTypeCopy = lhsType;
    QualType rhsTypeCopy = rhsType;
    QualType::unwrapBoth(lhsTypeCopy, rhsTypeCopy);
    if (lhsTypeCopy.matchesInterfaceImplementedByStruct(rhsTypeCopy))
      return lhsType;
  }
  // Allow type* = heap type* straight away. This is used for initializing non-owning pointers to heap allocations
  if (lhsType.isPtr() && rhsType.isHeap() && lhsType.matches(rhsType, false, true, true)) {
    TypeQualifiers rhsQualifiers = rhsType.getQualifiers();
    rhsQualifiers.isHeap = false;
    if (lhsType.getQualifiers() == rhsQualifiers)
      return lhsType;
  }

  // Nothing matched
  return QualType(TY_INVALID);
}

ExprResult OpRuleManager::getPlusEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  const ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_PLUS_EQUAL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  // Check if this is an unsafe operation
  if (lhsType.isPtr() && rhsType.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    ensureUnsafeAllowed(node, "+=", lhsType, rhsType);
    return {lhs};
  }

  return {validateBinaryOperation(node, PLUS_EQUAL_OP_RULES, std::size(PLUS_EQUAL_OP_RULES), "+=", lhsType, rhsType)};
}

ExprResult OpRuleManager::getMinusEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  const ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_MINUS_EQUAL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  // Check if this is an unsafe operation
  if (lhsType.isPtr() && rhsType.isOneOf({TY_INT, TY_LONG, TY_SHORT})) {
    ensureUnsafeAllowed(node, "-=", lhsType, rhsType);
    return {lhs};
  }

  return {validateBinaryOperation(node, MINUS_EQUAL_OP_RULES, std::size(MINUS_EQUAL_OP_RULES), "-=", lhsType, rhsType)};
}

ExprResult OpRuleManager::getMulEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  const ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_MUL_EQUAL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return {validateBinaryOperation(node, MUL_EQUAL_OP_RULES, std::size(MUL_EQUAL_OP_RULES), "*=", lhsType, rhsType)};
}

ExprResult OpRuleManager::getDivEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  const ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_DIV_EQUAL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return {validateBinaryOperation(node, DIV_EQUAL_OP_RULES, std::size(DIV_EQUAL_OP_RULES), "/=", lhsType, rhsType)};
}

QualType OpRuleManager::getRemEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) const {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, REM_EQUAL_OP_RULES, std::size(REM_EQUAL_OP_RULES), "%=", lhsType, rhsType);
}

QualType OpRuleManager::getSHLEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) const {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, SHL_EQUAL_OP_RULES, std::size(SHL_EQUAL_OP_RULES), "<<=", lhsType, rhsType);
}

QualType OpRuleManager::getSHREqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) const {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, SHR_EQUAL_OP_RULES, std::size(SHR_EQUAL_OP_RULES), ">>=", lhsType, rhsType);
}

QualType OpRuleManager::getAndEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) const {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, AND_EQUAL_OP_RULES, std::size(AND_EQUAL_OP_RULES), "&=", lhsType, rhsType);
}

QualType OpRuleManager::getOrEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) const {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, OR_EQUAL_OP_RULES, std::size(OR_EQUAL_OP_RULES), "|=", lhsType, rhsType);
}

QualType OpRuleManager::getXorEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) const {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, XOR_EQUAL_OP_RULES, std::size(XOR_EQUAL_OP_RULES), "^=", lhsType, rhsType);
}

QualType OpRuleManager::getLogicalOrResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, LOGICAL_OR_OP_RULES, std::size(LOGICAL_OR_OP_RULES), "||", lhsType, rhsType);
}

QualType OpRuleManager::getLogicalAndResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, LOGICAL_AND_OP_RULES, std::size(LOGICAL_AND_OP_RULES), "&&", lhsType, rhsType);
}

QualType OpRuleManager::getBitwiseOrResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, BITWISE_OR_OP_RULES, std::size(BITWISE_OR_OP_RULES), "|", lhsType, rhsType);
}

QualType OpRuleManager::getBitwiseXorResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, BITWISE_XOR_OP_RULES, std::size(BITWISE_XOR_OP_RULES), "^", lhsType, rhsType);
}

QualType OpRuleManager::getBitwiseAndResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, BITWISE_AND_OP_RULES, std::size(BITWISE_AND_OP_RULES), "&", lhsType, rhsType);
}

ExprResult OpRuleManager::getEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  const ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_EQUAL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  // Allow 'pointer == pointer' straight away
  if (lhsType.isPtr() && rhsType.isPtr())
    return ExprResult(QualType(TY_BOOL));

  // Allow 'pointer == int' straight away
  if (lhsType.isPtr() && rhsType.is(TY_INT))
    return ExprResult(QualType(TY_BOOL));

  // Allow 'string == char*' and vice versa straight away
  if ((lhsType.is(TY_STRING) && rhsType.isPtrTo(TY_CHAR)) || (lhsType.isPtrTo(TY_CHAR) && rhsType.is(TY_STRING)))
    return ExprResult(QualType(TY_BOOL));

  // Check primitive type combinations
  return ExprResult(validateBinaryOperation(node, EQUAL_OP_RULES, std::size(EQUAL_OP_RULES), "==", lhsType, rhsType));
}

ExprResult OpRuleManager::getNotEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  const ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_NOT_EQUAL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  // Allow 'pointer != pointer' straight away
  if (lhsType.isPtr() && rhsType.isPtr())
    return ExprResult(QualType(TY_BOOL));

  // Allow 'pointer != int' straight away
  if (lhsType.isPtr() && rhsType.is(TY_INT))
    return ExprResult(QualType(TY_BOOL));

  // Allow 'string != char*' and vice versa straight away
  if ((lhsType.is(TY_STRING) && rhsType.isPtrTo(TY_CHAR)) || (lhsType.isPtrTo(TY_CHAR) && rhsType.is(TY_STRING)))
    return ExprResult(QualType(TY_BOOL));

  // Check primitive type combinations
  return ExprResult(validateBinaryOperation(node, NOT_EQUAL_OP_RULES, std::size(NOT_EQUAL_OP_RULES), "!=", lhsType, rhsType));
}

QualType OpRuleManager::getLessResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, LESS_OP_RULES, std::size(LESS_OP_RULES), "<", lhsType, rhsType);
}

QualType OpRuleManager::getGreaterResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, GREATER_OP_RULES, std::size(GREATER_OP_RULES), ">", lhsType, rhsType);
}

QualType OpRuleManager::getLessEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, LESS_EQUAL_OP_RULES, std::size(LESS_EQUAL_OP_RULES), "<=", lhsType, rhsType);
}

QualType OpRuleManager::getGreaterEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return validateBinaryOperation(node, GREATER_EQUAL_OP_RULES, std::size(GREATER_EQUAL_OP_RULES), ">=", lhsType, rhsType);
}

ExprResult OpRuleManager::getShiftLeftResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  const ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_SHL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return {validateBinaryOperation(node, SHIFT_LEFT_OP_RULES, std::size(SHIFT_LEFT_OP_RULES), "<<", lhsType, rhsType)};
}

ExprResult OpRuleManager::getShiftRightResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  const ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_SHR, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return {validateBinaryOperation(node, SHIFT_RIGHT_OP_RULES, std::size(SHIFT_RIGHT_OP_RULES), ">>", lhsType, rhsType)};
}

ExprResult OpRuleManager::getPlusResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  const ExprResult result = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_PLUS, {lhs, rhs}, opIdx);
  if (!result.type.is(TY_INVALID))
    return result;

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

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

  return {validateBinaryOperation(node, PLUS_OP_RULES, std::size(PLUS_OP_RULES), "+", lhsType, rhsType)};
}

ExprResult OpRuleManager::getMinusResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  const ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_MINUS, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

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

  return {validateBinaryOperation(node, MINUS_OP_RULES, std::size(MINUS_OP_RULES), "-", lhsType, rhsType)};
}

ExprResult OpRuleManager::getMulResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  const ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_MUL, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return {validateBinaryOperation(node, MUL_OP_RULES, std::size(MUL_OP_RULES), "*", lhsType, rhsType)};
}

ExprResult OpRuleManager::getDivResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  const ExprResult resultType = isOperatorOverloadingFctAvailable<2>(node, OP_FCT_DIV, {lhs, rhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return {validateBinaryOperation(node, DIV_OP_RULES, std::size(DIV_OP_RULES), "/", lhsType, rhsType)};
}

ExprResult OpRuleManager::getRemResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) {
  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();
  const QualType rhsType = rhs.type.removeReferenceWrapper();

  return {validateBinaryOperation(node, REM_OP_RULES, std::size(REM_OP_RULES), "%", lhsType, rhsType)};
}

QualType OpRuleManager::getPrefixMinusResultType(const ASTNode *node, const ExprResult &lhs) {
  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();

  return validateUnaryOperation(node, PREFIX_MINUS_OP_RULES, std::size(PREFIX_MINUS_OP_RULES), "-", lhsType);
}

QualType OpRuleManager::getPrefixPlusPlusResultType(const ASTNode *node, const ExprResult &lhs) const {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();

  // Check if this is an unsafe operation
  if (lhsType.isPtr()) {
    ensureUnsafeAllowed(node, "++", lhsType);
    return lhsType;
  }

  return validateUnaryOperation(node, PREFIX_PLUS_PLUS_OP_RULES, std::size(PREFIX_PLUS_PLUS_OP_RULES), "++", lhsType);
}

QualType OpRuleManager::getPrefixMinusMinusResultType(const ASTNode *node, const ExprResult &lhs) const {
  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();

  // Check if this is an unsafe operation
  if (lhsType.isPtr()) {
    ensureUnsafeAllowed(node, "--", lhsType);
    return lhsType;
  }

  return validateUnaryOperation(node, PREFIX_MINUS_MINUS_OP_RULES, std::size(PREFIX_MINUS_MINUS_OP_RULES), "--", lhsType);
}

QualType OpRuleManager::getPrefixNotResultType(const ASTNode *node, const ExprResult &lhs) {
  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();

  return validateUnaryOperation(node, PREFIX_NOT_OP_RULES, std::size(PREFIX_NOT_OP_RULES), "!", lhsType);
}

QualType OpRuleManager::getPrefixBitwiseNotResultType(const ASTNode *node, const ExprResult &lhs) {
  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();

  return validateUnaryOperation(node, PREFIX_BITWISE_NOT_OP_RULES, std::size(PREFIX_BITWISE_NOT_OP_RULES), "~", lhsType);
}

QualType OpRuleManager::getPrefixMulResultType(const ASTNode *node, const ExprResult &lhs) {
  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();

  if (!lhsType.isPtr())
    throw SemanticError(node, OPERATOR_WRONG_DATA_TYPE, "Cannot apply de-referencing operator on type " + lhsType.getName(true));
  return lhsType.getContained();
}

QualType OpRuleManager::getPrefixBitwiseAndResultType(const ASTNode *node, const ExprResult &lhs) {
  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();

  return lhsType.toPtr(node);
}

ExprResult OpRuleManager::getPostfixPlusPlusResultType(ASTNode *node, const ExprResult &lhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  const ExprResult resultType = isOperatorOverloadingFctAvailable<1>(node, OP_FCT_POSTFIX_PLUS_PLUS, {lhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();

  // Check if this is an unsafe operation
  if (lhsType.isPtr()) {
    ensureUnsafeAllowed(node, "++", lhsType);
    return {lhs};
  }

  return {validateUnaryOperation(node, POSTFIX_PLUS_PLUS_OP_RULES, std::size(POSTFIX_PLUS_PLUS_OP_RULES), "++", lhsType)};
}

ExprResult OpRuleManager::getPostfixMinusMinusResultType(ASTNode *node, const ExprResult &lhs, size_t opIdx) {
  // Check is there is an overloaded operator function available
  const ExprResult resultType = isOperatorOverloadingFctAvailable<1>(node, OP_FCT_POSTFIX_MINUS_MINUS, {lhs}, opIdx);
  if (!resultType.type.is(TY_INVALID))
    return resultType;

  // Check if we try to assign a constant value
  ensureNoConstAssign(node, lhs.type);

  // Remove reference wrappers
  const QualType lhsType = lhs.type.removeReferenceWrapper();

  // Check if this is an unsafe operation
  if (lhsType.isPtr()) {
    ensureUnsafeAllowed(node, "--", lhsType);
    return {lhs};
  }

  return {validateUnaryOperation(node, POSTFIX_MINUS_MINUS_OP_RULES, std::size(POSTFIX_MINUS_MINUS_OP_RULES), "--", lhsType)};
}

QualType OpRuleManager::getCastResultType(const ASTNode *node, QualType lhsType, const ExprResult &rhs) const {
  // Remove reference wrappers
  lhsType = lhsType.removeReferenceWrapper();
  QualType rhsType = rhs.type.removeReferenceWrapper();

  // Only allow to cast the 'heap' qualifier away, if we are in unsafe mode
  if (lhsType.getQualifiers().isHeap != rhsType.getQualifiers().isHeap)
    ensureUnsafeAllowed(node, "(cast)", lhsType, rhsType);

  // Allow identity casts
  if (lhsType.matches(rhsType, false, true, true))
    return lhsType;
  // Allow casts string -> char* and string -> char[]
  if (lhsType.isOneOf({TY_PTR, TY_ARRAY}) && lhsType.getContained().is(TY_CHAR) && rhsType.is(TY_STRING))
    return lhsType;
  // Allow casts char* -> string and char[] -> string
  if (lhsType.is(TY_STRING) && rhsType.isOneOf({TY_PTR, TY_ARRAY}) && rhsType.getContained().is(TY_CHAR))
    return lhsType;
  // Allow casts any* -> any*
  if (lhsType.isOneOf({TY_PTR, TY_STRING}) && rhsType.isOneOf({TY_PTR, TY_STRING})) {
    if (lhsType != rhsType)
      ensureUnsafeAllowed(node, "(cast)", lhsType, rhsType);
    return lhsType;
  }
  // Check primitive type combinations
  return validateBinaryOperation(node, CAST_OP_RULES, std::size(CAST_OP_RULES), "(cast)", lhsType, rhsType, true);
}

template <size_t N>
ExprResult OpRuleManager::isOperatorOverloadingFctAvailable(ASTNode *node, const char *const fctName,
                                                            const std::array<ExprResult, N> &op, size_t opIdx) {
  static_assert(N == 1 || N == 2, "Only unary and binary operators are overloadable");
  Scope *calleeParentScope = nullptr;
  const Function *callee = nullptr;
  for (const auto &sourceFile : typeChecker->resourceManager.sourceFiles | std::views::values) {
    // Check if there is a registered operator function
    if (!sourceFile->getNameRegistryEntry(fctName))
      continue;

    // Match callees in the global scope of this source file
    calleeParentScope = sourceFile->globalScope.get();
    const QualType thisType(TY_DYN);
    ArgList args(N);
    args[0] = {typeChecker->mapLocalTypeToImportedScopeType(calleeParentScope, op[0].type), op[0].isTemporary()};
    if constexpr (N == 2)
      args[1] = {typeChecker->mapLocalTypeToImportedScopeType(calleeParentScope, op[1].type), op[1].isTemporary()};
    callee = FunctionManager::match(typeChecker, calleeParentScope, fctName, thisType, args, {}, false, node);
    if (callee)
      break;
  }

  // Return invalid type if the callee was not found
  if (!callee)
    return ExprResult(QualType(TY_INVALID));
  assert(calleeParentScope != nullptr);

  // Save the pointer to the operator function in the AST node
  std::vector<const Function *> &opFctPointers = typeChecker->getOpFctPointers(node);
  if (opFctPointers.size() <= opIdx)
    opFctPointers.resize(opIdx + 1, nullptr);
  opFctPointers.at(opIdx) = callee;

  // Check if we need to request a re-visit, because the function body was not type-checked yet
  typeChecker->requestRevisitIfRequired(callee);

  // Check if the called function has sufficient visibility
  const bool isImported = calleeParentScope != nullptr && calleeParentScope->isImportedBy(typeChecker->rootScope);
  const SymbolTableEntry *calleeEntry = callee->entry;
  if (isImported && !calleeEntry->getQualType().isPublic())
    throw SemanticError(node, INSUFFICIENT_VISIBILITY,
                        "Overloaded operator '" + callee->getSignature() + "' has insufficient visibility");

  // Procedures always have the return type 'bool'
  if (callee->isProcedure())
    return ExprResult(QualType(TY_BOOL));
  const QualType &returnType = callee->returnType;

  // Add anonymous symbol to keep track of dtor call, if non-trivially destructible
  SymbolTableEntry *anonymousSymbol = nullptr;
  if (returnType.is(TY_STRUCT) && !returnType.isTriviallyDestructible(node))
    anonymousSymbol = typeChecker->currentScope->symbolTable.insertAnonymous(returnType, node, opIdx);

  return {typeChecker->mapImportedScopeTypeToLocalType(calleeParentScope, returnType), anonymousSymbol};
}

QualType OpRuleManager::validateUnaryOperation(const ASTNode *node, const UnaryOpRule opRules[], size_t opRulesSize,
                                               const char *name, const QualType &lhs) {
  for (size_t i = 0; i < opRulesSize; i++) {
    const UnaryOpRule &rule = opRules[i];
    if (std::get<0>(rule) == lhs.getSuperType())
      return QualType(std::get<1>(rule));
  }
  throw getExceptionUnary(node, name, lhs);
}

QualType OpRuleManager::validateBinaryOperation(const ASTNode *node, const BinaryOpRule opRules[], size_t opRulesSize,
                                                const char *name, const QualType &lhs, const QualType &rhs,
                                                bool preserveQualifiersFromLhs, const char *customMessagePrefix) {
  for (size_t i = 0; i < opRulesSize; i++) {
    const BinaryOpRule &rule = opRules[i];
    if (std::get<0>(rule) == lhs.getSuperType() && std::get<1>(rule) == rhs.getSuperType()) {
      QualType resultType((std::get<2>(rule)));
      if (preserveQualifiersFromLhs)
        resultType.setQualifiers(lhs.getQualifiers());
      return resultType;
    }
  }
  throw getExceptionBinary(node, name, lhs, rhs, customMessagePrefix);
}

SemanticError OpRuleManager::getExceptionUnary(const ASTNode *node, const char *name, const QualType &lhs) {
  return {node, OPERATOR_WRONG_DATA_TYPE, "Cannot apply '" + std::string(name) + "' operator on type " + lhs.getName(true)};
}

SemanticError OpRuleManager::getExceptionBinary(const ASTNode *node, const char *name, const QualType &lhs, const QualType &rhs,
                                                const char *messagePrefix) {
  // Build error message
  std::stringstream errorMsg;
  if (strlen(messagePrefix) != 0)
    errorMsg << messagePrefix << ". Expected " << lhs.getName(true) << " but got " << rhs.getName(true);
  else
    errorMsg << "Cannot apply '" << name << "' operator on types " << lhs.getName(true) << " and " << rhs.getName(true);

  // Return the exception
  return {node, OPERATOR_WRONG_DATA_TYPE, errorMsg.str()};
}

void OpRuleManager::ensureUnsafeAllowed(const ASTNode *node, const char *name, const QualType &lhs) const {
  if (typeChecker->currentScope->doesAllowUnsafeOperations())
    return;
  // Print error message
  const std::string lhsName = lhs.getName(true);
  const std::string errorMsg = "Cannot apply '" + std::string(name) + "' operator on type " + lhsName +
                               " as this is an unsafe operation. Please use unsafe blocks if you know what you are doing.";
  SOFT_ERROR_VOID(node, UNSAFE_OPERATION_IN_SAFE_CONTEXT, errorMsg)
}

void OpRuleManager::ensureUnsafeAllowed(const ASTNode *node, const char *name, const QualType &lhs, const QualType &rhs) const {
  if (typeChecker->currentScope->doesAllowUnsafeOperations())
    return;
  // Print error message
  const std::string lhsName = lhs.getName(true);
  const std::string rhsName = rhs.getName(true);
  const std::string errorMsg = "Cannot apply '" + std::string(name) + "' operator on types " + lhsName + " and " + rhsName +
                               " as this is an unsafe operation. Please use unsafe blocks if you know what you are doing.";
  SOFT_ERROR_VOID(node, UNSAFE_OPERATION_IN_SAFE_CONTEXT, errorMsg)
}

void OpRuleManager::ensureNoConstAssign(const ASTNode *node, const QualType &lhs, bool isDecl, bool isReturn) const {
  // Check if we try to assign a constant value
  if (lhs.removeReferenceWrapper().isConst() && !isDecl && !isReturn) {
    const std::string errorMessage = "Trying to assign value to an immutable variable of type " + lhs.getName(true);
    SOFT_ERROR_VOID(node, REASSIGN_CONST_VARIABLE, errorMessage);
  }
}

} // namespace spice::compiler
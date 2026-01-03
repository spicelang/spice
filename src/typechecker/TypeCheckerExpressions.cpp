// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <global/GlobalResourceManager.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/MacroDefs.h>

namespace spice::compiler {

std::any TypeChecker::visitAssignExpr(AssignExprNode *node) {
  // Check if ternary
  if (node->ternaryExpr) {
    auto result = std::any_cast<ExprResult>(visit(node->ternaryExpr));
    node->setEvaluatedSymbolType(result.type, manIdx);
    return result;
  }

  // Check if assignment
  if (node->op != AssignExprNode::AssignOp::OP_NONE) {
    // Visit the right side first
    auto rhs = std::any_cast<ExprResult>(visit(node->rhs));
    auto [rhsType, rhsEntry] = rhs;
    HANDLE_UNRESOLVED_TYPE_ER(rhsType)
    // Then visit the left side
    auto lhs = std::any_cast<ExprResult>(visit(node->lhs));
    auto [lhsType, lhsVar] = lhs;
    HANDLE_UNRESOLVED_TYPE_ER(lhsType)

    // Take a look at the operator
    if (node->op == AssignExprNode::AssignOp::OP_ASSIGN) {
      const bool isDecl = lhs.entry != nullptr && lhs.entry->isField() && !lhs.entry->getLifecycle().isInitialized();
      rhsType = opRuleManager.getAssignResultType(node, lhs, rhs, isDecl).first;

      // If there is an anonymous entry attached (e.g. for struct instantiation), delete it
      if (rhsEntry != nullptr && rhsEntry->anonymous)
        currentScope->symbolTable.deleteAnonymous(rhsEntry->name);
    } else if (node->op == AssignExprNode::AssignOp::OP_PLUS_EQUAL) {
      rhsType = opRuleManager.getPlusEqualResultType(node, lhs, rhs, 0).type;
    } else if (node->op == AssignExprNode::AssignOp::OP_MINUS_EQUAL) {
      rhsType = opRuleManager.getMinusEqualResultType(node, lhs, rhs, 0).type;
    } else if (node->op == AssignExprNode::AssignOp::OP_MUL_EQUAL) {
      rhsType = opRuleManager.getMulEqualResultType(node, lhs, rhs, 0).type;
    } else if (node->op == AssignExprNode::AssignOp::OP_DIV_EQUAL) {
      rhsType = opRuleManager.getDivEqualResultType(node, lhs, rhs, 0).type;
    } else if (node->op == AssignExprNode::AssignOp::OP_REM_EQUAL) {
      rhsType = opRuleManager.getRemEqualResultType(node, lhs, rhs);
    } else if (node->op == AssignExprNode::AssignOp::OP_SHL_EQUAL) {
      rhsType = opRuleManager.getSHLEqualResultType(node, lhs, rhs);
    } else if (node->op == AssignExprNode::AssignOp::OP_SHR_EQUAL) {
      rhsType = opRuleManager.getSHREqualResultType(node, lhs, rhs);
    } else if (node->op == AssignExprNode::AssignOp::OP_AND_EQUAL) {
      rhsType = opRuleManager.getAndEqualResultType(node, lhs, rhs);
    } else if (node->op == AssignExprNode::AssignOp::OP_OR_EQUAL) {
      rhsType = opRuleManager.getOrEqualResultType(node, lhs, rhs);
    } else if (node->op == AssignExprNode::AssignOp::OP_XOR_EQUAL) {
      rhsType = opRuleManager.getXorEqualResultType(node, lhs, rhs);
    }

    if (lhsVar) { // Variable is involved on the left side
      // Perform type inference
      if (lhsType.is(TY_DYN))
        lhsVar->updateType(rhsType, false);

      // In case the lhs variable is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(lhsVar->name); lhsCapture)
        lhsCapture->setAccessType(READ_WRITE);

      // Update the state of the variable
      lhsVar->updateState(INITIALIZED, node);
    }

    return ExprResult{node->setEvaluatedSymbolType(rhsType, manIdx)};
  }

  throw CompilerError(UNHANDLED_BRANCH, "AssignExpr fall-through"); // GCOV_EXCL_LINE
}

std::any TypeChecker::visitTernaryExpr(TernaryExprNode *node) {
  // Check if there is a ternary operator applied
  if (!node->falseExpr)
    return visit(node->condition);

  // Visit condition
  const auto condition = std::any_cast<ExprResult>(visit(node->condition));
  HANDLE_UNRESOLVED_TYPE_ER(condition.type)
  const auto trueExpr = node->isShortened ? condition : std::any_cast<ExprResult>(visit(node->trueExpr));
  const auto [trueType, trueEntry] = trueExpr;
  HANDLE_UNRESOLVED_TYPE_ER(trueType)
  const auto falseExpr = std::any_cast<ExprResult>(visit(node->falseExpr));
  const auto [falseType, falseEntry] = falseExpr;
  HANDLE_UNRESOLVED_TYPE_ER(falseType)

  // Check if the condition evaluates to bool
  if (!condition.type.is(TY_BOOL))
    SOFT_ERROR_ER(node->condition, OPERATOR_WRONG_DATA_TYPE, "Condition operand in ternary must be a bool")

  // Check if trueType and falseType are matching
  const QualType trueTypeModified = trueType.removeReferenceWrapper();
  const QualType falseTypeModified = falseType.removeReferenceWrapper();
  if (!trueTypeModified.matches(falseTypeModified, false, true, false))
    SOFT_ERROR_ER(node, OPERATOR_WRONG_DATA_TYPE,
                  "True and false operands in ternary must be of same data type. Got " + trueType.getName(true) + " and " +
                      falseType.getName(true))

  // If there is an anonymous symbol attached to left or right, remove it,
  // since the result takes over the ownership of any destructible object.
  bool removedAnonymousSymbols = false;
  if (trueEntry) {
    if (trueEntry->anonymous) {
      currentScope->symbolTable.deleteAnonymous(trueEntry->name);
      removedAnonymousSymbols = true;
    } else if (!trueType.isRef() && !trueType.isTriviallyCopyable(node)) {
      node->trueSideCallsCopyCtor = true;
    }
  }
  if (falseEntry) {
    if (falseEntry->anonymous) {
      currentScope->symbolTable.deleteAnonymous(falseEntry->name);
      removedAnonymousSymbols = true;
    } else if (!falseType.isRef() && !falseType.isTriviallyCopyable(node)) {
      node->falseSideCallsCopyCtor = true;
    }
  }

  // Create a new anonymous symbol for the result if required
  const QualType &resultType = trueType;
  SymbolTableEntry *anonymousSymbol = nullptr;
  const bool calledCopyCtor = node->trueSideCallsCopyCtor || node->falseSideCallsCopyCtor;
  if (removedAnonymousSymbols || calledCopyCtor || resultType.isRef())
    anonymousSymbol = currentScope->symbolTable.insertAnonymous(resultType, node);

  // Look up the copy ctor if at least one side needs it
  if (node->trueSideCallsCopyCtor || node->falseSideCallsCopyCtor)
    node->calledCopyCtor = matchCopyCtor(trueTypeModified, node);

  return ExprResult{node->setEvaluatedSymbolType(resultType, manIdx), anonymousSymbol};
}

std::any TypeChecker::visitLogicalOrExpr(LogicalOrExprNode *node) {
  // Check if a logical or operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentOperand = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentOperand.type)

  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands.size(); i++) {
    auto rhsOperand = std::any_cast<ExprResult>(visit(node->operands[i]));
    HANDLE_UNRESOLVED_TYPE_ER(rhsOperand.type)
    currentOperand = {OpRuleManager::getLogicalOrResultType(node, currentOperand, rhsOperand)};
  }

  node->setEvaluatedSymbolType(currentOperand.type, manIdx);
  return currentOperand;
}

std::any TypeChecker::visitLogicalAndExpr(LogicalAndExprNode *node) {
  // Check if a logical and operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentOperand = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentOperand.type)

  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands.size(); i++) {
    auto rhsOperand = std::any_cast<ExprResult>(visit(node->operands[i]));
    HANDLE_UNRESOLVED_TYPE_ER(rhsOperand.type)
    currentOperand = {OpRuleManager::getLogicalAndResultType(node, currentOperand, rhsOperand)};
  }

  node->setEvaluatedSymbolType(currentOperand.type, manIdx);
  return currentOperand;
}

std::any TypeChecker::visitBitwiseOrExpr(BitwiseOrExprNode *node) {
  // Check if a bitwise or operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentOperand = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentOperand.type)

  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands.size(); i++) {
    auto rhsOperand = std::any_cast<ExprResult>(visit(node->operands[i]));
    HANDLE_UNRESOLVED_TYPE_ER(rhsOperand.type)
    currentOperand = {OpRuleManager::getBitwiseOrResultType(node, currentOperand, rhsOperand)};
  }

  node->setEvaluatedSymbolType(currentOperand.type, manIdx);
  return currentOperand;
}

std::any TypeChecker::visitBitwiseXorExpr(BitwiseXorExprNode *node) {
  // Check if a bitwise xor operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentOperand = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentOperand.type)

  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands.size(); i++) {
    auto rhsOperand = std::any_cast<ExprResult>(visit(node->operands[i]));
    HANDLE_UNRESOLVED_TYPE_ER(rhsOperand.type)
    currentOperand = {OpRuleManager::getBitwiseXorResultType(node, currentOperand, rhsOperand)};
  }

  node->setEvaluatedSymbolType(currentOperand.type, manIdx);
  return currentOperand;
}

std::any TypeChecker::visitBitwiseAndExpr(BitwiseAndExprNode *node) {
  // Check if a bitwise and operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentOperand = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentOperand.type)

  // Loop through all remaining operands
  for (size_t i = 1; i < node->operands.size(); i++) {
    auto rhsOperand = std::any_cast<ExprResult>(visit(node->operands[i]));
    HANDLE_UNRESOLVED_TYPE_ER(rhsOperand.type)
    currentOperand = {OpRuleManager::getBitwiseAndResultType(node, currentOperand, rhsOperand)};
  }

  node->setEvaluatedSymbolType(currentOperand.type, manIdx);
  return currentOperand;
}

std::any TypeChecker::visitEqualityExpr(EqualityExprNode *node) {
  // Check if at least one equality operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit right side first, then left side
  const auto rhs = std::any_cast<ExprResult>(visit(node->operands[1]));
  HANDLE_UNRESOLVED_TYPE_ER(rhs.type)
  const auto lhs = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(lhs.type)

  // Check if we need the string runtime to perform a string comparison
  if (lhs.type.is(TY_STRING) && rhs.type.is(TY_STRING) && !sourceFile->isStringRT())
    sourceFile->requestRuntimeModule(STRING_RT);

  // Check operator
  ExprResult result;
  if (node->op == EqualityExprNode::EqualityOp::OP_EQUAL) // Operator was equal
    result = opRuleManager.getEqualResultType(node, lhs, rhs, 0);
  else if (node->op == EqualityExprNode::EqualityOp::OP_NOT_EQUAL) // Operator was not equal
    result = opRuleManager.getNotEqualResultType(node, lhs, rhs, 0);
  else
    throw CompilerError(UNHANDLED_BRANCH, "EqualityExpr fall-through"); // GCOV_EXCL_LINE

  node->setEvaluatedSymbolType(result.type, manIdx);
  return result;
}

std::any TypeChecker::visitRelationalExpr(RelationalExprNode *node) {
  // Check if a relational operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit right side first, then left side
  const auto rhs = std::any_cast<ExprResult>(visit(node->operands[1]));
  HANDLE_UNRESOLVED_TYPE_ER(rhs.type)
  const auto lhs = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(lhs.type)

  // Check operator
  QualType resultType;
  if (node->op == RelationalExprNode::RelationalOp::OP_LESS) // Operator was less
    resultType = OpRuleManager::getLessResultType(node, lhs, rhs);
  else if (node->op == RelationalExprNode::RelationalOp::OP_GREATER) // Operator was greater
    resultType = OpRuleManager::getGreaterResultType(node, lhs, rhs);
  else if (node->op == RelationalExprNode::RelationalOp::OP_LESS_EQUAL) // Operator was less equal
    resultType = OpRuleManager::getLessEqualResultType(node, lhs, rhs);
  else if (node->op == RelationalExprNode::RelationalOp::OP_GREATER_EQUAL) // Operator was greater equal
    resultType = OpRuleManager::getGreaterEqualResultType(node, lhs, rhs);
  else
    throw CompilerError(UNHANDLED_BRANCH, "RelationalExpr fall-through"); // GCOV_EXCL_LINE

  return ExprResult{node->setEvaluatedSymbolType(resultType, manIdx)};
}

std::any TypeChecker::visitShiftExpr(ShiftExprNode *node) {
  // Check if at least one shift operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentResult = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentResult.type)

  // Loop through remaining operands
  for (size_t i = 0; i < node->opQueue.size(); i++) {
    auto operandResult = std::any_cast<ExprResult>(visit(node->operands[i + 1]));
    HANDLE_UNRESOLVED_TYPE_ER(operandResult.type)

    // Check operator
    const ShiftExprNode::ShiftOp &op = node->opQueue.front().first;
    if (op == ShiftExprNode::ShiftOp::OP_SHIFT_LEFT)
      currentResult = opRuleManager.getShiftLeftResultType(node, currentResult, operandResult, i);
    else if (op == ShiftExprNode::ShiftOp::OP_SHIFT_RIGHT)
      currentResult = opRuleManager.getShiftRightResultType(node, currentResult, operandResult, i);
    else
      throw CompilerError(UNHANDLED_BRANCH, "ShiftExpr fall-through"); // GCOV_EXCL_LINE

    // Push the new item and pop the old one on the other side of the queue
    node->opQueue.emplace(op, currentResult.type);
    node->opQueue.pop();
  }

  node->setEvaluatedSymbolType(currentResult.type, manIdx);
  return currentResult;
}

std::any TypeChecker::visitAdditiveExpr(AdditiveExprNode *node) {
  // Check if at least one additive operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentResult = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentResult.type)

  // Loop through remaining operands
  for (size_t i = 0; i < node->opQueue.size(); i++) {
    auto operandResult = std::any_cast<ExprResult>(visit(node->operands[i + 1]));
    HANDLE_UNRESOLVED_TYPE_ER(operandResult.type)

    // Check operator
    const AdditiveExprNode::AdditiveOp &op = node->opQueue.front().first;
    if (op == AdditiveExprNode::AdditiveOp::OP_PLUS)
      currentResult = opRuleManager.getPlusResultType(node, currentResult, operandResult, i);
    else if (op == AdditiveExprNode::AdditiveOp::OP_MINUS)
      currentResult = opRuleManager.getMinusResultType(node, currentResult, operandResult, i);
    else
      throw CompilerError(UNHANDLED_BRANCH, "AdditiveExpr fall-through"); // GCOV_EXCL_LINE

    // Push the new item and pop the old one on the other side of the queue
    node->opQueue.emplace(op, currentResult.type);
    node->opQueue.pop();
  }

  node->setEvaluatedSymbolType(currentResult.type, manIdx);
  return currentResult;
}

std::any TypeChecker::visitMultiplicativeExpr(MultiplicativeExprNode *node) {
  // Check if at least one multiplicative operator is applied
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // Visit leftmost operand
  auto currentResult = std::any_cast<ExprResult>(visit(node->operands[0]));
  HANDLE_UNRESOLVED_TYPE_ER(currentResult.type)
  // Loop through remaining operands
  for (size_t i = 0; i < node->opQueue.size(); i++) {
    auto operandResult = std::any_cast<ExprResult>(visit(node->operands[i + 1]));
    HANDLE_UNRESOLVED_TYPE_ER(operandResult.type)

    // Check operator
    const MultiplicativeExprNode::MultiplicativeOp &op = node->opQueue.front().first;
    if (op == MultiplicativeExprNode::MultiplicativeOp::OP_MUL)
      currentResult = opRuleManager.getMulResultType(node, currentResult, operandResult, i);
    else if (op == MultiplicativeExprNode::MultiplicativeOp::OP_DIV)
      currentResult = opRuleManager.getDivResultType(node, currentResult, operandResult, i);
    else if (op == MultiplicativeExprNode::MultiplicativeOp::OP_REM)
      currentResult = OpRuleManager::getRemResultType(node, currentResult, operandResult);
    else
      throw CompilerError(UNHANDLED_BRANCH, "Multiplicative fall-through"); // GCOV_EXCL_LINE

    // Push the new item and pop the old one on the other side of the queue
    node->opQueue.emplace(op, currentResult.type);
    node->opQueue.pop();
  }

  node->setEvaluatedSymbolType(currentResult.type, manIdx);
  return currentResult;
}

std::any TypeChecker::visitCastExpr(CastExprNode *node) {
  // Check if cast is applied
  if (!node->isCast)
    return visit(node->prefixUnaryExpr);

  // Visit destination type
  const auto dstType = std::any_cast<QualType>(visit(node->dataType));
  HANDLE_UNRESOLVED_TYPE_ER(dstType)
  // Visit source type
  const auto src = std::any_cast<ExprResult>(visit(node->assignExpr));
  HANDLE_UNRESOLVED_TYPE_ER(src.type)

  // Check for identity cast
  if (src.type == dstType) {
    const CompilerWarning warning(node->codeLoc, IDENTITY_CAST, "You cast from a type to itself. Thus, this can be simplified.");
    sourceFile->compilerOutput.warnings.push_back(warning);
  }

  // Get result type
  const QualType resultType = opRuleManager.getCastResultType(node, dstType, src);

  const bool typesMatch = dstType.matches(src.type, false, true, true);
  const bool sameContainerType = src.type.isSameContainerTypeAs(dstType);
  SymbolTableEntry *entry = typesMatch || sameContainerType ? src.entry : nullptr;
  return ExprResult{node->setEvaluatedSymbolType(resultType, manIdx), entry};
}

std::any TypeChecker::visitPrefixUnaryExpr(PrefixUnaryExprNode *node) {
  // If no operator is applied, simply visit the postfix unary expression
  if (node->op == PrefixUnaryExprNode::PrefixUnaryOp::OP_NONE)
    return visit(node->postfixUnaryExpr);

  // Visit the right side
  PrefixUnaryExprNode *rhsNode = node->prefixUnaryExpr;
  auto operand = std::any_cast<ExprResult>(visit(rhsNode));
  auto [operandType, operandEntry] = operand;
  HANDLE_UNRESOLVED_TYPE_ER(operandType)
  // Determine action, based on the given operator
  switch (node->op) {
  case PrefixUnaryExprNode::PrefixUnaryOp::OP_MINUS:
    operandType = OpRuleManager::getPrefixMinusResultType(node, operand);
    break;
  case PrefixUnaryExprNode::PrefixUnaryOp::OP_PLUS_PLUS:
    operandType = opRuleManager.getPrefixPlusPlusResultType(node, operand);

    if (operandEntry) {
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(operandEntry->name); lhsCapture)
        lhsCapture->setAccessType(READ_WRITE);

      // Update the state of the variable
      operandEntry->updateState(INITIALIZED, node);
    }

    break;
  case PrefixUnaryExprNode::PrefixUnaryOp::OP_MINUS_MINUS:
    operandType = opRuleManager.getPrefixMinusMinusResultType(node, operand);

    if (operandEntry) {
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(operandEntry->name); lhsCapture)
        lhsCapture->setAccessType(READ_WRITE);

      // Update the state of the variable
      operandEntry->updateState(INITIALIZED, node);
    }

    break;
  case PrefixUnaryExprNode::PrefixUnaryOp::OP_NOT:
    operandType = OpRuleManager::getPrefixNotResultType(node, operand);
    break;
  case PrefixUnaryExprNode::PrefixUnaryOp::OP_BITWISE_NOT:
    operandType = OpRuleManager::getPrefixBitwiseNotResultType(node, operand);
    break;
  case PrefixUnaryExprNode::PrefixUnaryOp::OP_DEREFERENCE:
    operandType = OpRuleManager::getPrefixMulResultType(node, operand);
    break;
  case PrefixUnaryExprNode::PrefixUnaryOp::OP_ADDRESS_OF:
    operandType = OpRuleManager::getPrefixBitwiseAndResultType(node, operand);
    break;
  default:                                                                 // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "PrefixUnaryExpr fall-through"); // GCOV_EXCL_LINE
  }

  return ExprResult{node->setEvaluatedSymbolType(operandType, manIdx), operandEntry};
}

std::any TypeChecker::visitPostfixUnaryExpr(PostfixUnaryExprNode *node) {
  // If no operator is applied, simply visit the atomic expression
  if (node->op == PostfixUnaryExprNode::PostfixUnaryOp::OP_NONE)
    return visit(node->atomicExpr);

  // Visit left side
  PostfixUnaryExprNode *lhsNode = node->postfixUnaryExpr;
  auto operand = std::any_cast<ExprResult>(visit(lhsNode));
  auto [operandType, operandEntry] = operand;
  HANDLE_UNRESOLVED_TYPE_ER(operandType)

  switch (node->op) {
  case PostfixUnaryExprNode::PostfixUnaryOp::OP_SUBSCRIPT: {
    // Visit index assignment
    AssignExprNode *indexAssignExpr = node->subscriptIndexExpr;
    const auto index = std::any_cast<ExprResult>(visit(indexAssignExpr));
    HANDLE_UNRESOLVED_TYPE_ER(index.type)

    // Check is there is an overloaded operator function available, if yes accept it
    const auto [type, _] = opRuleManager.isOperatorOverloadingFctAvailable<2>(node, OP_FCT_SUBSCRIPT, {operand, index}, 0);
    if (!type.is(TY_INVALID)) {
      operandType = type;
      break;
    }

    operandType = operandType.removeReferenceWrapper();

    // Check if the index is of the right type
    if (!index.type.isOneOf({TY_INT, TY_LONG}))
      SOFT_ERROR_ER(node, ARRAY_INDEX_NOT_INT_OR_LONG, "Array index must be of type int or long")

    // Check if we can apply the subscript operator on the lhs type
    if (!operandType.isOneOf({TY_ARRAY, TY_PTR, TY_STRING}))
      SOFT_ERROR_ER(node, OPERATOR_WRONG_DATA_TYPE,
                    "Can only apply subscript operator on array type, got " + operandType.getName(true))

    // Check if we have an unsafe operation
    if (operandType.isPtr() && !currentScope->doesAllowUnsafeOperations())
      SOFT_ERROR_ER(
          node, UNSAFE_OPERATION_IN_SAFE_CONTEXT,
          "The subscript operator on pointers is an unsafe operation. Use unsafe blocks if you know what you are doing.")

    // In case of compile time index value and known array size, perform a compile time out-of-bounds check
    if (operandType.isArray() && operandType.getArraySize() != ARRAY_SIZE_UNKNOWN && indexAssignExpr->hasCompileTimeValue()) {
      const int32_t constIndex = indexAssignExpr->getCompileTimeValue().intValue;
      const unsigned int constSize = operandType.getArraySize();
      // Check if we are accessing out-of-bounds memory
      if (constIndex >= static_cast<int32_t>(constSize)) {
        const std::string idxStr = std::to_string(constIndex);
        const std::string sizeStr = std::to_string(constSize);
        SOFT_ERROR_ER(node, ARRAY_INDEX_OUT_OF_BOUNDS,
                      "You are trying to access element with index " + idxStr + " of an array with size " + sizeStr)
      }
    }

    // Get item type
    operandType = operandType.getContained();

    // Remove heap qualifier
    operandType.getQualifiers().isHeap = false;

    break;
  }
  case PostfixUnaryExprNode::PostfixUnaryOp::OP_MEMBER_ACCESS: {
    const std::string &fieldName = node->identifier;

    // Check if lhs is enum or strobj
    const QualType lhsBaseTy = operandType.autoDeReference();
    if (!lhsBaseTy.is(TY_STRUCT))
      SOFT_ERROR_ER(node, INVALID_MEMBER_ACCESS, "Cannot apply member access operator on " + operandType.getName(false))

    // Retrieve registry entry
    const std::string &structName = lhsBaseTy.getSubType();
    Scope *structScope = lhsBaseTy.getBodyScope();

    // If we only have the generic struct scope, lookup the concrete manifestation scope
    if (structScope->isGenericScope) {
      const Struct *spiceStruct = lhsBaseTy.getStruct(node);
      assert(spiceStruct != nullptr);
      structScope = spiceStruct->scope;
    }
    assert(!structScope->isGenericScope); // At this point we always expect a substantiation scope

    // Get accessed field
    std::vector<size_t> indexPath;
    SymbolTableEntry *memberEntry = structScope->symbolTable.lookupInComposedFields(fieldName, indexPath);
    if (!memberEntry)
      SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_VARIABLE, "Field '" + node->identifier + "' not found in struct " + structName)
    const QualType memberType = memberEntry->getQualType();

    // Check for insufficient visibility
    if (structScope->isImportedBy(rootScope) && !memberEntry->getQualType().getBase().isPublic())
      SOFT_ERROR_ER(node, INSUFFICIENT_VISIBILITY, "Cannot access field '" + fieldName + "' due to its private visibility")

    // Set field to used
    memberEntry->used = true;

    // Overwrite type and entry of left side with member type and entry
    operandType = memberType;
    operandEntry = memberEntry;
    break;
  }
  case PostfixUnaryExprNode::PostfixUnaryOp::OP_PLUS_PLUS: {
    operandType = opRuleManager.getPostfixPlusPlusResultType(node, operand, 0).type;

    if (operandEntry) {
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(operandEntry->name); lhsCapture)
        lhsCapture->setAccessType(READ_WRITE);

      // Update the state of the variable
      operandEntry->updateState(INITIALIZED, node);
    }

    break;
  }
  case PostfixUnaryExprNode::PostfixUnaryOp::OP_MINUS_MINUS: {
    operandType = opRuleManager.getPostfixMinusMinusResultType(node, operand, 0).type;

    if (operandEntry) {
      // In case the lhs is captured, notify the capture about the write access
      if (Capture *lhsCapture = currentScope->symbolTable.lookupCapture(operandEntry->name); lhsCapture)
        lhsCapture->setAccessType(READ_WRITE);

      // Update the state of the variable
      operandEntry->updateState(INITIALIZED, node);
    }

    break;
  }
  default:                                                                  // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "PostfixUnaryExpr fall-through"); // GCOV_EXCL_LINE
  }

  if (operandType.is(TY_INVALID)) {
    const std::string &varName = operandEntry ? operandEntry->name : "";
    SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_VARIABLE, "Variable '" + varName + "' was referenced before declared")
  }

  return ExprResult{node->setEvaluatedSymbolType(operandType, manIdx), operandEntry};
}

std::any TypeChecker::visitAtomicExpr(AtomicExprNode *node) {
  // Check if constant
  if (node->constant)
    return visit(node->constant);

  // Check if value
  if (node->value)
    return visit(node->value);

  // Check for builtin calls
  if (node->builtinCall)
    return visit(node->builtinCall);

  // Check for assign expression within parentheses
  if (node->assignExpr)
    return visit(node->assignExpr);

  // Identifier (local or global variable access)
  assert(!node->fqIdentifier.empty());

  auto &[entry, accessScope, capture] = node->data.at(manIdx);
  accessScope = currentScope;

  // Check if a local or global variable can be found by searching for the name
  if (node->identifierFragments.size() == 1)
    entry = accessScope->lookup(node->identifierFragments.back());

  // If no local or global was found, search in the name registry
  if (!entry) {
    const NameRegistryEntry *registryEntry = sourceFile->getNameRegistryEntry(node->fqIdentifier);
    if (!registryEntry)
      SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_VARIABLE, "The variable '" + node->fqIdentifier + "' could not be found")
    entry = registryEntry->targetEntry;
    accessScope = registryEntry->targetScope;
  }
  assert(entry != nullptr);
  entry->used = true;
  capture = accessScope->symbolTable.lookupCapture(entry->name);

  const QualType varType = entry->getQualType();
  HANDLE_UNRESOLVED_TYPE_ER(varType)
  if (varType.is(TY_INVALID))
    SOFT_ERROR_ER(node, USED_BEFORE_DECLARED, "Symbol '" + entry->name + "' was used before declared.")

  if (varType.isOneOf({TY_FUNCTION, TY_PROCEDURE}) && entry->global) {
    // Check if overloaded function was referenced
    const std::vector<Function *> *manifestations = entry->declNode->getFctManifestations(entry->name);
    if (manifestations->size() > 1)
      SOFT_ERROR_ER(node, REFERENCED_OVERLOADED_FCT, "Overloaded functions / functions with optional params cannot be referenced")
    if (!manifestations->front()->templateTypes.empty())
      SOFT_ERROR_ER(node, REFERENCED_OVERLOADED_FCT, "Generic functions cannot be referenced")
    // Set referenced function to used
    Function *referencedFunction = manifestations->front();
    referencedFunction->used = true;
    referencedFunction->entry->used = true;
  }

  // The base type should be an extended primitive
  const QualType baseType = varType.getBase();
  if (!baseType.isExtendedPrimitive() && !baseType.is(TY_DYN))
    SOFT_ERROR_ER(node, INVALID_SYMBOL_ACCESS, "A symbol of type " + varType.getName(false) + " cannot be accessed here")

  // Check if we have seen a 'this.' prefix, because the generator needs that
  if (entry->scope->type == ScopeType::STRUCT && node->identifierFragments.front() != THIS_VARIABLE_NAME)
    SOFT_ERROR_ER(node, REFERENCED_UNDEFINED_VARIABLE,
                  "The symbol '" + node->fqIdentifier + "' could not be found. Missing 'this.' prefix?")

  // Ensure that the entry is public, if the symbol is imported.
  // An exception are enum items. There it is sufficient, that the enum itself is public.
  if (accessScope->isImportedBy(rootScope) && accessScope->type != ScopeType::ENUM)
    if (!entry->getQualType().getBase().isPublic())
      SOFT_ERROR_ER(node, INSUFFICIENT_VISIBILITY, "Cannot access '" + entry->name + "' due to its private visibility")

  // For enum item access, use access scope of the enum
  if (entry->scope->type == ScopeType::ENUM)
    accessScope = entry->scope;

  // For struct access, use access scope of the struct
  if (baseType.is(TY_STRUCT)) {
    const std::string &structName = baseType.getSubType();
    const NameRegistryEntry *nameRegistryEntry = sourceFile->getNameRegistryEntry(structName);
    assert(nameRegistryEntry != nullptr);
    accessScope = nameRegistryEntry->targetScope;
    assert(accessScope != nullptr);
  }

  return ExprResult{node->setEvaluatedSymbolType(varType, manIdx), entry};
}

} // namespace spice::compiler

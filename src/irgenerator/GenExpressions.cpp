// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>

namespace spice::compiler {

std::any IRGenerator::visitAssignExpr(const AssignExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Visit ternary expression
  if (node->ternaryExpr())
    return visit(node->ternaryExpr());

  // Visit thread definition
  if (node->threadDef())
    return visit(node->threadDef());

  // Assign or compound assign operation
  if (node->hasOperator) {
    const PrefixUnaryExprNode *lhsNode = node->lhs();
    const AssignExprNode *rhsNode = node->rhs();

    if (node->op == AssignExprNode::OP_ASSIGN) { // Normal assignment
      return doAssignment(lhsNode, rhsNode);
    } else { // Compound assignment
      // Get symbol types of left and right side
      const SymbolType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
      const SymbolType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);

      // Retrieve rhs
      auto rhs = std::any_cast<ExprResult>(visit(rhsNode));
      // Retrieve lhs
      auto lhs = std::any_cast<ExprResult>(visit(lhsNode));

      ExprResult result;
      switch (node->op) {
      case AssignExprNode::OP_PLUS_EQUAL:
        result = conversionManager.getPlusEqualInst(node, lhs, lhsSTy, rhs, rhsSTy, currentScope, 0);
        break;
      case AssignExprNode::OP_MINUS_EQUAL:
        result = conversionManager.getMinusEqualInst(node, lhs, lhsSTy, rhs, rhsSTy, currentScope, 0);
        break;
      case AssignExprNode::OP_MUL_EQUAL:
        result = conversionManager.getMulEqualInst(node, lhs, lhsSTy, rhs, rhsSTy, currentScope, 0);
        break;
      case AssignExprNode::OP_DIV_EQUAL:
        result = conversionManager.getDivEqualInst(node, lhs, lhsSTy, rhs, rhsSTy, currentScope, 0);
        break;
      case AssignExprNode::OP_REM_EQUAL:
        result = conversionManager.getRemEqualInst(node, lhs, lhsSTy, rhs, rhsSTy, currentScope, 0);
        break;
      case AssignExprNode::OP_SHL_EQUAL:
        result = conversionManager.getSHLEqualInst(node, lhs, lhsSTy, rhs, rhsSTy, currentScope, 0);
        break;
      case AssignExprNode::OP_SHR_EQUAL:
        result = conversionManager.getSHREqualInst(node, lhs, lhsSTy, rhs, rhsSTy, currentScope, 0);
        break;
      case AssignExprNode::OP_AND_EQUAL:
        result = conversionManager.getAndEqualInst(node, lhs, lhsSTy, rhs, rhsSTy, currentScope, 0);
        break;
      case AssignExprNode::OP_OR_EQUAL:
        result = conversionManager.getOrEqualInst(node, lhs, lhsSTy, rhs, rhsSTy, currentScope, 0);
        break;
      case AssignExprNode::OP_XOR_EQUAL:
        result = conversionManager.getXorEqualInst(node, lhs, lhsSTy, rhs, rhsSTy, currentScope, 0);
        break;
      default:
        throw CompilerError(UNHANDLED_BRANCH, "Assign op fall-through"); // GCOV_EXCL_LINE
      }

      if (result.ptr) { // The operation allocated more memory
        if (lhs.entry)
          lhs.entry->updateAddress(result.ptr);
      } else if (result.value) { // The operation only updated the value
        // Store the result
        lhs.value = result.value;
        builder.CreateStore(lhs.value, lhs.ptr, lhs.entry && lhs.entry->isVolatile);
      }
      return ExprResult{.value = lhs.value, .ptr = lhs.ptr, .refPtr = lhs.refPtr, .entry = lhs.entry};
    }
  }

  // This is a fallthrough case -> throw an error
  throw CompilerError(UNHANDLED_BRANCH, "AssignStmt fall-through"); // GCOV_EXCL_LINE
}

std::any IRGenerator::visitTernaryExpr(const TernaryExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands().size() == 1)
    return visit(node->operands().front());

  // It is a ternary
  // Retrieve the condition value
  llvm::Value *condValue = resolveValue(node->operands()[0]);

  // Get the values of true and false
  llvm::Value *trueValue;
  llvm::Value *falseValue;
  if (node->isShortened) {
    trueValue = condValue;
    falseValue = resolveValue(node->operands()[1]);
  } else {
    trueValue = resolveValue(node->operands()[1]);
    falseValue = resolveValue(node->operands()[2]);
  }

  llvm::Value *resultValue = builder.CreateSelect(condValue, trueValue, falseValue);
  return ExprResult{.value = resultValue};
}

std::any IRGenerator::visitLogicalOrExpr(const LogicalOrExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands().size() == 1)
    return visit(node->operands().front());

  // It is a logical or expression
  // Create exit block for short-circuiting
  const std::string codeLoc = node->codeLoc.toPrettyLineAndColumn();
  llvm::BasicBlock *bExit = createBlock("lor.exit." + codeLoc);

  // Visit the first operand
  llvm::Value *firstOperandValue = resolveValue(node->operands().front());

  // Prepare an array for value-to-block-mapping
  std::pair<llvm::BasicBlock *, llvm::Value *> shortCircuitBlocks[node->operands().size()];
  // The first element is the first operand value with the original block
  shortCircuitBlocks[0] = {builder.GetInsertBlock(), firstOperandValue};
  // Create a block for each additional operand and save it to the mapping
  for (size_t i = 1; i < node->operands().size(); i++) {
    llvm::BasicBlock *nextBlock = createBlock("lor." + std::to_string(i) + "." + codeLoc);
    shortCircuitBlocks[i] = {nextBlock, nullptr};
  }
  // Create conditional jump to the exit block if the first operand was true, otherwise to the next block
  insertCondJump(firstOperandValue, bExit, shortCircuitBlocks[1].first);

  // Create block for each operand
  for (int i = 1; i < node->operands().size(); i++) {
    // Switch to the next block
    switchToBlock(shortCircuitBlocks[i].first);
    // Evaluate operand and save the result in the mapping
    shortCircuitBlocks[i].second = resolveValue(node->operands()[i]);
    // Replace the array entry with the current insert block, since the insert block could have changed in the meantime
    shortCircuitBlocks[i].first = builder.GetInsertBlock();
    // Check if there are more blocks to process
    if (i == node->operands().size() - 1) {
      // Insert a simple jump to the exit block for the last block
      insertJump(bExit);
    } else {
      // Create conditional jump to the exit block if the first operand was true, otherwise to the next block
      insertCondJump(shortCircuitBlocks[i].second, bExit, shortCircuitBlocks[i + 1].first);
    }
  }

  // Get the result with the phi node
  switchToBlock(bExit);
  llvm::PHINode *result = builder.CreatePHI(firstOperandValue->getType(), node->operands().size(), "lor_phi");
  for (const auto &[incomingBlock, value] : shortCircuitBlocks)
    result->addIncoming(value, incomingBlock);

  // Return the result
  return ExprResult{.value = result};
}

std::any IRGenerator::visitLogicalAndExpr(const LogicalAndExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands().size() == 1)
    return visit(node->operands().front());

  // It is a logical and expression
  // Create exit block for short-circuiting
  const std::string codeLoc = node->codeLoc.toPrettyLineAndColumn();
  llvm::BasicBlock *bExit = createBlock("land.exit." + codeLoc);

  // Visit the first operand
  llvm::Value *firstOperandValue = resolveValue(node->operands().front());

  // Prepare an array for value-to-block-mapping
  std::pair<llvm::BasicBlock *, llvm::Value *> shortCircuitBlocks[node->operands().size()];
  // The first element is the first operand value with the original block
  shortCircuitBlocks[0] = {builder.GetInsertBlock(), firstOperandValue};
  // Create a block for each additional operand and save it to the mapping
  for (size_t i = 1; i < node->operands().size(); i++) {
    llvm::BasicBlock *nextBlock = createBlock("land." + std::to_string(i) + "." + codeLoc);
    shortCircuitBlocks[i] = {nextBlock, nullptr};
  }
  // Create conditional jump to the exit block if the first operand was true, otherwise to the next block
  insertCondJump(firstOperandValue, shortCircuitBlocks[1].first, bExit);

  // Create block for each operand
  for (int i = 1; i < node->operands().size(); i++) {
    // Switch to the next block
    switchToBlock(shortCircuitBlocks[i].first);
    // Evaluate operand and save the result in the mapping
    shortCircuitBlocks[i].second = resolveValue(node->operands()[i]);
    // Replace the array entry with the current insert block, since the insert block could have changed in the meantime
    shortCircuitBlocks[i].first = builder.GetInsertBlock();
    // Check if there are more blocks to process
    if (i == node->operands().size() - 1) {
      // Insert a simple jump to the exit block for the last block
      insertJump(bExit);
    } else {
      // Create conditional jump to the exit block if the operand was true, otherwise to the next block
      insertCondJump(shortCircuitBlocks[i].second, shortCircuitBlocks[i + 1].first, bExit);
    }
  }

  // Get the result with the phi node
  switchToBlock(bExit);
  llvm::PHINode *result = builder.CreatePHI(firstOperandValue->getType(), node->operands().size(), "land_phi");
  for (const auto &[incomingBlock, value] : shortCircuitBlocks)
    result->addIncoming(value, incomingBlock);

  // Return the result
  return ExprResult{.value = result};
}

std::any IRGenerator::visitBitwiseOrExpr(const BitwiseOrExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands().size() == 1)
    return visit(node->operands().front());

  // It is a bitwise or expression
  // Evaluate first operand
  BitwiseXorExprNode *lhsNode = node->operands().front();
  const SymbolType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto result = std::any_cast<ExprResult>(visit(lhsNode));

  // Evaluate all additional operands
  for (size_t i = 1; i < node->operands().size(); i++) {
    // Evaluate the operand
    BitwiseXorExprNode *rhsNode = node->operands()[i];
    const SymbolType rhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
    auto rhs = std::any_cast<ExprResult>(visit(rhsNode));
    result = conversionManager.getBitwiseOrInst(node, result, lhsSTy, rhs, rhsSTy, currentScope, i - 1);
  }

  // Return result
  return result;
}

std::any IRGenerator::visitBitwiseXorExpr(const BitwiseXorExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands().size() == 1)
    return visit(node->operands().front());

  // It is a bitwise xor expression
  // Evaluate first operand
  BitwiseAndExprNode *lhsNode = node->operands().front();
  const SymbolType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto result = std::any_cast<ExprResult>(visit(lhsNode));

  // Evaluate all additional operands
  for (size_t i = 1; i < node->operands().size(); i++) {
    // Evaluate the operand
    BitwiseAndExprNode *rhsNode = node->operands()[i];
    const SymbolType rhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
    auto rhs = std::any_cast<ExprResult>(visit(rhsNode));
    result = conversionManager.getBitwiseXorInst(node, result, lhsSTy, rhs, rhsSTy, currentScope, i - 1);
  }

  // Return result
  return result;
}

std::any IRGenerator::visitBitwiseAndExpr(const BitwiseAndExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands().size() == 1)
    return visit(node->operands().front());

  // It is a bitwise and expression
  // Evaluate first operand
  EqualityExprNode *lhsNode = node->operands().front();
  const SymbolType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto result = std::any_cast<ExprResult>(visit(lhsNode));

  // Evaluate all additional operands
  for (size_t i = 1; i < node->operands().size(); i++) {
    // Evaluate the operand
    EqualityExprNode *rhsNode = node->operands()[i];
    const SymbolType rhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
    auto rhs = std::any_cast<ExprResult>(visit(rhsNode));
    result = conversionManager.getBitwiseAndInst(rhsNode, result, lhsSTy, rhs, rhsSTy, currentScope, i - 1);
  }

  // Return result
  return result;
}

std::any IRGenerator::visitEqualityExpr(const EqualityExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands().size() == 1)
    return visit(node->operands().front());

  // It is an equality expression
  // Evaluate lhs
  RelationalExprNode *lhsNode = node->operands()[0];
  const SymbolType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto result = std::any_cast<ExprResult>(visit(lhsNode));

  // Evaluate rhs
  RelationalExprNode *rhsNode = node->operands()[1];
  const SymbolType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
  auto rhs = std::any_cast<ExprResult>(visit(rhsNode));

  // Retrieve the result value, based on the exact operator
  switch (node->op) {
  case EqualityExprNode::OP_EQUAL:
    result = conversionManager.getEqualInst(node, result, lhsSTy, rhs, rhsSTy, currentScope, 0);
    break;
  case EqualityExprNode::OP_NOT_EQUAL:
    result = conversionManager.getNotEqualInst(node, result, lhsSTy, rhs, rhsSTy, currentScope, 0);
    break;
  default:
    throw CompilerError(UNHANDLED_BRANCH, "EqualityExpr fall-through"); // GCOV_EXCL_LINE
  }

  // Return the result
  return result;
}

std::any IRGenerator::visitRelationalExpr(const RelationalExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands().size() == 1)
    return visit(node->operands().front());

  // It is a relational expression
  // Evaluate lhs
  ShiftExprNode *lhsNode = node->operands()[0];
  const SymbolType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto result = std::any_cast<ExprResult>(visit(lhsNode));

  // Evaluate rhs
  ShiftExprNode *rhsNode = node->operands()[1];
  const SymbolType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
  auto rhs = std::any_cast<ExprResult>(visit(rhsNode));

  // Retrieve the result value, based on the exact operator
  switch (node->op) {
  case RelationalExprNode::OP_LESS:
    result = conversionManager.getLessInst(node, result, lhsSTy, rhs, rhsSTy, currentScope, 0);
    break;
  case RelationalExprNode::OP_GREATER:
    result = conversionManager.getGreaterInst(node, result, lhsSTy, rhs, rhsSTy, currentScope, 0);
    break;
  case RelationalExprNode::OP_LESS_EQUAL:
    result = conversionManager.getLessEqualInst(node, result, lhsSTy, rhs, rhsSTy, currentScope, 0);
    break;
  case RelationalExprNode::OP_GREATER_EQUAL:
    result = conversionManager.getGreaterEqualInst(node, result, lhsSTy, rhs, rhsSTy, currentScope, 0);
    break;
  default:
    throw CompilerError(UNHANDLED_BRANCH, "EqualityExpr fall-through"); // GCOV_EXCL_LINE
  }

  // Return the result
  return result;
}

std::any IRGenerator::visitShiftExpr(const ShiftExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands().size() == 1)
    return visit(node->operands().front());

  // It is a shift expression
  // Evaluate lhs
  AdditiveExprNode *lhsNode = node->operands()[0];
  const SymbolType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto result = std::any_cast<ExprResult>(visit(lhsNode));

  // Evaluate rhs
  AdditiveExprNode *rhsNode = node->operands()[1];
  const SymbolType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
  auto rhs = std::any_cast<ExprResult>(visit(rhsNode));

  // Retrieve the result value, based on the exact operator
  switch (node->op) {
  case ShiftExprNode::OP_SHIFT_LEFT:
    result = conversionManager.getShiftLeftInst(node, result, lhsSTy, rhs, rhsSTy, currentScope, 0);
    break;
  case ShiftExprNode::OP_SHIFT_RIGHT:
    result = conversionManager.getShiftRightInst(node, result, lhsSTy, rhs, rhsSTy, currentScope, 0);
    break;
  default:
    throw CompilerError(UNHANDLED_BRANCH, "ShiftExpr fall-through"); // GCOV_EXCL_LINE
  }

  // Return the result
  return result;
}

std::any IRGenerator::visitAdditiveExpr(const AdditiveExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands().size() == 1)
    return visit(node->operands().front());

  // It is an additive expression
  // Evaluate first operand
  MultiplicativeExprNode *lhsNode = node->operands()[0];
  SymbolType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto lhs = std::any_cast<ExprResult>(visit(lhsNode));

  auto opQueue = node->opQueue;
  size_t operandIndex = 1;
  while (!opQueue.empty()) {
    const size_t operatorIndex = operandIndex - 1;
    // Evaluate next operand
    MultiplicativeExprNode *rhsNode = node->operands()[operandIndex++];
    assert(rhsNode != nullptr);
    const SymbolType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
    auto rhs = std::any_cast<ExprResult>(visit(rhsNode));

    // Retrieve the result, based on the exact operator
    switch (opQueue.front().first) {
    case AdditiveExprNode::OP_PLUS:
      lhs = conversionManager.getPlusInst(node, lhs, lhsSTy, rhs, rhsSTy, currentScope, operatorIndex);
      break;
    case AdditiveExprNode::OP_MINUS:
      lhs = conversionManager.getMinusInst(node, lhs, lhsSTy, rhs, rhsSTy, currentScope, operatorIndex);
      break;
    default:
      throw CompilerError(UNHANDLED_BRANCH, "AdditiveExpr fall-through"); // GCOV_EXCL_LINE
    }

    // Retrieve the new lhs symbol type
    lhsSTy = opQueue.front().second;

    opQueue.pop();
  }

  // Return the result
  return lhs;
}

std::any IRGenerator::visitMultiplicativeExpr(const MultiplicativeExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands().size() == 1)
    return visit(node->operands().front());

  // It is an additive expression
  // Evaluate first operand
  CastExprNode *lhsNode = node->operands()[0];
  SymbolType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto result = std::any_cast<ExprResult>(visit(lhsNode));

  auto opQueue = node->opQueue;
  size_t operandIndex = 1;
  while (!opQueue.empty()) {
    const size_t operatorIndex = operandIndex - 1;
    // Evaluate next operand
    CastExprNode *rhsNode = node->operands()[operandIndex++];
    assert(rhsNode != nullptr);
    const SymbolType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
    auto rhs = std::any_cast<ExprResult>(visit(rhsNode));

    // Retrieve the result, based on the exact operator
    switch (opQueue.front().first) {
    case MultiplicativeExprNode::OP_MUL:
      result = conversionManager.getMulInst(node, result, lhsSTy, rhs, rhsSTy, currentScope, operatorIndex);
      break;
    case MultiplicativeExprNode::OP_DIV:
      result = conversionManager.getDivInst(node, result, lhsSTy, rhs, rhsSTy, currentScope, operatorIndex);
      break;
    case MultiplicativeExprNode::OP_REM:
      result = conversionManager.getRemInst(node, result, lhsSTy, rhs, rhsSTy, currentScope, operatorIndex);
      break;
    default:
      throw CompilerError(UNHANDLED_BRANCH, "MultiplicativeExpr fall-through"); // GCOV_EXCL_LINE
    }

    // Retrieve the new lhs symbol type
    lhsSTy = opQueue.front().second;
    opQueue.pop();
  }

  // Return the result
  return result;
}

std::any IRGenerator::visitCastExpr(const CastExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (!node->isCasted)
    return visit(node->prefixUnaryExpr());

  // It is a cast expression
  // Retrieve target symbol type
  const SymbolType targetSTy = node->getEvaluatedSymbolType(manIdx);

  // Evaluate rhs
  PrefixUnaryExprNode *rhsNode = node->prefixUnaryExpr();
  const SymbolType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
  auto rhs = std::any_cast<ExprResult>(visit(rhsNode));

  // Retrieve the result value
  const ExprResult result = conversionManager.getCastInst(node, targetSTy, rhs, rhsSTy, currentScope, 0);

  // Return the result
  return result;
}

std::any IRGenerator::visitPrefixUnaryExpr(const PrefixUnaryExprNode *node) {
  diGenerator.setSourceLocation(node);

  // If no operator is applied, simply visit the atomic expression
  if (node->op == PrefixUnaryExprNode::OP_NONE)
    return visit(node->postfixUnaryExpr());

  // Evaluate lhs
  PrefixUnaryExprNode *lhsNode = node->prefixUnary();
  SymbolType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto lhs = std::any_cast<ExprResult>(visit(lhsNode));

  switch (node->op) {
  case PrefixUnaryExprNode::OP_MINUS: {
    // Execute operation
    lhs = conversionManager.getPrefixMinusInst(node, lhs, lhsSTy, currentScope, 0);

    // This operator can not work in-place, so we need additional memory
    lhs.ptr = insertAlloca(lhs.value->getType());

    // Store the new value
    builder.CreateStore(lhs.value, lhs.ptr);

    break;
  }
  case PrefixUnaryExprNode::OP_PLUS_PLUS: {
    // Make sure the value is present
    resolveValue(lhsNode, lhs);

    // Execute operation
    lhs.value = conversionManager.getPrefixPlusPlusInst(node, lhs, lhsSTy, currentScope, 0).value;

    // If this operation happens on a volatile variable, store the value directly
    if (lhs.entry && lhs.entry->isVolatile)
      builder.CreateStore(lhs.value, lhs.ptr, true);

    // Save to the existing address if possible, otherwise (e.g. for literals) allocate new space
    if (!lhs.ptr)
      lhs.ptr = insertAlloca(lhs.value->getType());

    // Store the new value
    builder.CreateStore(lhs.value, lhs.ptr);

    break;
  }
  case PrefixUnaryExprNode::OP_MINUS_MINUS: {
    // Make sure the value is present
    resolveValue(lhsNode, lhs);

    // Execute operation
    lhs.value = conversionManager.getPrefixMinusMinusInst(node, lhs, lhsSTy, currentScope, 0).value;

    // If this operation happens on a volatile variable, store the value directly
    if (lhs.entry && lhs.entry->isVolatile)
      builder.CreateStore(lhs.value, lhs.ptr, true);

    // Save to the existing address if possible, otherwise (e.g. for literals) allocate new space
    if (!lhs.ptr)
      lhs.ptr = insertAlloca(lhs.value->getType());

    // Store the new value
    builder.CreateStore(lhs.value, lhs.ptr);

    break;
  }
  case PrefixUnaryExprNode::OP_NOT: {
    // Make sure the value is present
    resolveValue(lhsNode, lhs);

    // Execute operation
    lhs = conversionManager.getPrefixNotInst(node, lhs, lhsSTy, currentScope, 0);

    // This operator can not work in-place, so we need additional memory
    lhs.ptr = insertAlloca(lhs.value->getType());

    // Store the new value
    builder.CreateStore(lhs.value, lhs.ptr, lhs.entry && lhs.entry->isVolatile);

    break;
  }
  case PrefixUnaryExprNode::OP_BITWISE_NOT: {
    // Make sure the value is present
    resolveValue(lhsNode, lhs);

    // Execute operation
    lhs = conversionManager.getPrefixBitwiseNotInst(node, lhs, lhsSTy, currentScope, 0);

    // This operator can not work in-place, so we need additional memory
    lhs.ptr = insertAlloca(lhs.value->getType());

    // Store the new value
    builder.CreateStore(lhs.value, lhs.ptr, lhs.entry && lhs.entry->isVolatile);

    break;
  }
  case PrefixUnaryExprNode::OP_INDIRECTION: {
    // Make sure the value is present
    resolveValue(lhsNode, lhs);

    // Execute operation
    lhs.ptr = lhs.value;

    // Reset the value
    lhs.value = nullptr;

    break;
  }
  case PrefixUnaryExprNode::OP_ADDRESS_OF: {
    // Make sure the address is present
    resolveAddress(lhs);

    // Execute operation
    lhs.value = lhs.ptr;

    // Reset the address
    lhs.ptr = nullptr;

    break;
  }
  default:
    throw CompilerError(UNHANDLED_BRANCH, "PrefixUnaryExpr fall-through"); // GCOV_EXCL_LINE
  }

  return lhs;
}

std::any IRGenerator::visitPostfixUnaryExpr(const PostfixUnaryExprNode *node) {
  diGenerator.setSourceLocation(node);

  // If no operator is applied, simply visit the atomic expression
  if (node->op == PostfixUnaryExprNode::OP_NONE)
    return visit(node->atomicExpr());

  // Evaluate lhs
  PostfixUnaryExprNode *lhsNode = node->postfixUnaryExpr();
  SymbolType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto lhs = std::any_cast<ExprResult>(visit(lhsNode));

  switch (node->op) {
  case PostfixUnaryExprNode::OP_SUBSCRIPT: {
    // Get the LLVM type of the operand
    llvm::Type *lhsTy = lhsSTy.toLLVMType(context, currentScope);

    // Get the index value
    AssignExprNode *indexExpr = node->assignExpr();
    llvm::Value *indexValue = resolveValue(indexExpr);
    // Come up with the address
    if (lhsSTy.isArray() && lhsSTy.getArraySize() != ARRAY_SIZE_UNKNOWN) { // Array
      // Make sure the address is present
      resolveAddress(lhs);

      // Calculate address of array item
      llvm::Value *indices[2] = {builder.getInt32(0), indexValue};
      lhs.ptr = builder.CreateInBoundsGEP(lhsTy, lhs.ptr, indices);
    } else { // Pointer
      // Make sure the value is present
      resolveValue(lhsNode, lhs);
      assert(lhs.value != nullptr);

      // Now the pointer is the value
      lhs.ptr = lhs.value;

      lhsTy = lhsSTy.getContainedTy().toLLVMType(context, currentScope);
      // Calculate address of pointer item
      lhs.ptr = builder.CreateInBoundsGEP(lhsTy, lhs.ptr, indexValue);
    }

    // Reset value and entry
    lhs.value = nullptr;
    lhs.entry = nullptr;
    break;
  }
  case PostfixUnaryExprNode::OP_MEMBER_ACCESS: {
    // Get the address of the struct instance
    resolveAddress(lhs);
    lhsSTy = lhsSTy.removeReferenceWrapper();

    // Auto de-reference pointer
    autoDeReferencePtr(lhs.ptr, lhsSTy, currentScope);
    assert(lhsSTy.is(TY_STRUCT));

    // Retrieve struct scope
    const std::string &fieldName = node->identifier;
    Scope *structScope = lhsSTy.getBodyScope();

    // Retrieve field entry
    lhs.entry = structScope->lookupStrict(fieldName);
    assert(lhs.entry != nullptr);
    SymbolType fieldSymbolType = lhs.entry->getType();

    // Get address of the field in the struct instance
    llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(lhs.entry->orderIndex)};
    llvm::Value *memberAddress = builder.CreateInBoundsGEP(lhsSTy.toLLVMType(context, structScope->parent), lhs.ptr, indices);
    memberAddress->setName(fieldName + "_addr");

    // Set as ptr or refPtr, depending on the type
    if (fieldSymbolType.isRef()) {
      lhs.ptr = nullptr;
      lhs.refPtr = memberAddress;
    } else {
      lhs.ptr = memberAddress;
      lhs.refPtr = nullptr;
    }

    // Reset the value
    lhs.value = nullptr;
    break;
  }
  case PostfixUnaryExprNode::OP_PLUS_PLUS: {
    // Make sure a value is present
    resolveValue(lhsNode, lhs);

    // Allocate new local variable if required
    if (!lhs.ptr) {
      assert(lhs.value != nullptr);
      lhs.ptr = insertAlloca(lhs.value->getType());
    }

    // Execute operation
    ExprResult result = conversionManager.getPostfixPlusPlusInst(node, lhs, lhsSTy, currentScope, 0);

    // Save the new value to the old address
    if (conversionManager.callsOverloadedOpFct(node, 0)) {
      lhs.value = result.value;
      lhs.ptr = result.ptr;
    } else {
      builder.CreateStore(result.value, lhs.ptr, lhs.entry && lhs.entry->isVolatile);
      lhs.ptr = nullptr;
    }
    break;
  }
  case PostfixUnaryExprNode::OP_MINUS_MINUS: {
    // Make sure a value is present
    resolveValue(lhsNode, lhs);

    // Allocate new local variable if required
    if (!lhs.ptr) {
      assert(lhs.value != nullptr);
      lhs.ptr = insertAlloca(lhs.value->getType());
    }

    // Execute operation
    ExprResult result = conversionManager.getPostfixMinusMinusInst(node, lhs, lhsSTy, currentScope, 0);

    // Save the new value to the old address
    if (conversionManager.callsOverloadedOpFct(node, 0)) {
      lhs.value = result.value;
      lhs.ptr = result.ptr;
    } else {
      builder.CreateStore(result.value, lhs.ptr, lhs.entry && lhs.entry->isVolatile);
      lhs.ptr = nullptr;
    }
    break;
  }
  default:
    throw CompilerError(UNHANDLED_BRANCH, "PostfixUnaryExpr fall-through"); // GCOV_EXCL_LINE
  }

  return lhs;
}

std::any IRGenerator::visitAtomicExpr(const AtomicExprNode *node) {
  diGenerator.setSourceLocation(node);

  // If constant
  if (node->constant()) {
    auto constantValue = std::any_cast<llvm::Constant *>(visit(node->constant()));
    return ExprResult{.constant = constantValue};
  }

  // If value
  if (node->value())
    return visit(node->value());

  // Is assign expression
  if (node->assignExpr())
    return visit(node->assignExpr());

  // Is call to printf builtin
  if (node->printfCall())
    return visit(node->printfCall());

  // Is call to sizeof builtin
  if (node->sizeofCall())
    return visit(node->sizeofCall());

  // Is call to alignof builtin
  if (node->alignofCall())
    return visit(node->alignofCall());

  // Is call to len builtin
  if (node->lenCall())
    return visit(node->lenCall());

  // Is call to tid builtin
  if (node->tidCall())
    return visit(node->tidCall());

  // Is call to join builtin
  if (node->joinCall())
    return visit(node->joinCall());

  // Identifier (local or global variable access)
  assert(!node->identifierFragments.empty());

  // Get symbol table entry
  SymbolTableEntry *varEntry = node->entries.at(manIdx);
  assert(varEntry != nullptr);
  Scope *accessScope = node->accessScopes.at(manIdx);
  assert(accessScope != nullptr);
  SymbolType varSymbolType = varEntry->getType();
  llvm::Type *varType = varSymbolType.toLLVMType(context, accessScope);

  // Check if external global variable
  if (varEntry->global && accessScope->isImportedBy(rootScope)) {
    // External global variables need to be declared and allocated in the current module
    llvm::Value *varAddress = module->getOrInsertGlobal(varEntry->name, varType);
    varEntry->updateAddress(varAddress);
  }

  // Check if enum item
  if (accessScope->type == SCOPE_ENUM) {
    auto itemNode = dynamic_cast<const EnumItemNode *>(varEntry->declNode);
    llvm::Constant *constantItemValue = llvm::ConstantInt::get(varType, itemNode->itemValue);
    return ExprResult{.constant = constantItemValue, .entry = varEntry};
  }

  llvm::Value *address = varEntry->getAddress();
  assert(address != nullptr);

  // Load the address of the referenced variable
  if (varSymbolType.isRef())
    return ExprResult{.refPtr = address, .entry = varEntry};

  return ExprResult{.ptr = address, .entry = varEntry};
}

} // namespace spice::compiler
// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>

#include <llvm/IR/Module.h>

namespace spice::compiler {

std::any IRGenerator::visitAssignExpr(const AssignExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Visit ternary expression
  if (node->ternaryExpr)
    return visit(node->ternaryExpr);

  // Assign or compound assign operation
  if (node->op != AssignExprNode::OP_NONE) {
    const PrefixUnaryExprNode *lhsNode = node->lhs;
    const AssignExprNode *rhsNode = node->rhs;

    // Normal assignment
    if (node->op == AssignExprNode::OP_ASSIGN)
      return doAssignment(lhsNode, rhsNode, node);

    // Compound assignment
    // Get symbol types of left and right side
    const QualType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
    const QualType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);

    // Retrieve rhs
    auto rhs = std::any_cast<LLVMExprResult>(visit(rhsNode));
    // Retrieve lhs
    auto lhs = std::any_cast<LLVMExprResult>(visit(lhsNode));

    LLVMExprResult result;
    switch (node->op) {
    case AssignExprNode::OP_PLUS_EQUAL:
      result = conversionManager.getPlusEqualInst(node, lhs, lhsSTy, rhs, rhsSTy, 0);
      break;
    case AssignExprNode::OP_MINUS_EQUAL:
      result = conversionManager.getMinusEqualInst(node, lhs, lhsSTy, rhs, rhsSTy, 0);
      break;
    case AssignExprNode::OP_MUL_EQUAL:
      result = conversionManager.getMulEqualInst(node, lhs, lhsSTy, rhs, rhsSTy, 0);
      break;
    case AssignExprNode::OP_DIV_EQUAL:
      result = conversionManager.getDivEqualInst(node, lhs, lhsSTy, rhs, rhsSTy, 0);
      break;
    case AssignExprNode::OP_REM_EQUAL:
      result = conversionManager.getRemEqualInst(node, lhs, lhsSTy, rhs, rhsSTy);
      break;
    case AssignExprNode::OP_SHL_EQUAL:
      result = conversionManager.getSHLEqualInst(node, lhs, lhsSTy, rhs, rhsSTy);
      break;
    case AssignExprNode::OP_SHR_EQUAL:
      result = conversionManager.getSHREqualInst(node, lhs, lhsSTy, rhs, rhsSTy);
      break;
    case AssignExprNode::OP_AND_EQUAL:
      result = conversionManager.getAndEqualInst(node, lhs, lhsSTy, rhs, rhsSTy);
      break;
    case AssignExprNode::OP_OR_EQUAL:
      result = conversionManager.getOrEqualInst(node, lhs, lhsSTy, rhs, rhsSTy);
      break;
    case AssignExprNode::OP_XOR_EQUAL:
      result = conversionManager.getXorEqualInst(node, lhs, lhsSTy, rhs, rhsSTy);
      break;
    default:                                                           // GCOV_EXCL_LINE
      throw CompilerError(UNHANDLED_BRANCH, "Assign op fall-through"); // GCOV_EXCL_LINE
    }

    if (result.ptr) { // The operation allocated more memory
      if (lhs.entry)
        lhs.entry->updateAddress(result.ptr);
    } else if (result.value) { // The operation only updated the value
      // Store the result
      lhs.value = result.value;
      insertStore(lhs.value, lhs.ptr, lhs.entry && lhs.entry->isVolatile);
    }
    return LLVMExprResult{.value = lhs.value, .ptr = lhs.ptr, .refPtr = lhs.refPtr, .entry = lhs.entry};
  }

  // This is a fallthrough case -> throw an error
  throw CompilerError(UNHANDLED_BRANCH, "AssignStmt fall-through"); // GCOV_EXCL_LINE
}

std::any IRGenerator::visitTernaryExpr(const TernaryExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (!node->falseExpr)
    return visit(node->condition);

  // It is a ternary
  // Retrieve the condition value
  llvm::Value *condValue = resolveValue(node->condition);

  // Get the values of true and false
  llvm::Value *trueValue;
  llvm::Value *falseValue;
  if (node->isShortened) {
    trueValue = condValue;
    falseValue = resolveValue(node->falseExpr);
  } else {
    trueValue = resolveValue(node->trueExpr);
    falseValue = resolveValue(node->falseExpr);
  }

  llvm::Value *resultValue = builder.CreateSelect(condValue, trueValue, falseValue);
  return LLVMExprResult{.value = resultValue};
}

std::any IRGenerator::visitLogicalOrExpr(const LogicalOrExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // It is a logical or expression
  // Create exit block for short-circuiting
  const std::string codeLoc = node->codeLoc.toPrettyLineAndColumn();
  llvm::BasicBlock *bExit = createBlock("lor.exit." + codeLoc);

  // Visit the first operand
  llvm::Value *firstOperandValue = resolveValue(node->operands.front());

  // Prepare an array for value-to-block-mapping
  std::vector<std::pair<llvm::BasicBlock *, llvm::Value *>> shortCircuitBlocks;
  shortCircuitBlocks.reserve(node->operands.size());
  // The first element is the first operand value with the original block
  shortCircuitBlocks.emplace_back(builder.GetInsertBlock(), firstOperandValue);
  // Create a block for each additional operand and save it to the mapping
  for (size_t i = 1; i < node->operands.size(); i++)
    shortCircuitBlocks.emplace_back(createBlock("lor." + std::to_string(i) + "." + codeLoc), nullptr);
  // Create conditional jump to the exit block if the first operand was true, otherwise to the next block
  insertCondJump(firstOperandValue, bExit, shortCircuitBlocks.at(1).first);

  // Create block for each operand
  for (size_t i = 1; i < node->operands.size(); i++) {
    // Switch to the next block
    switchToBlock(shortCircuitBlocks.at(i).first);
    // Evaluate operand and save the result in the mapping
    shortCircuitBlocks.at(i).second = resolveValue(node->operands[i]);
    // Replace the array entry with the current insert block, since the insert block could have changed in the meantime
    shortCircuitBlocks.at(i).first = builder.GetInsertBlock();
    // Check if there are more blocks to process
    if (i == node->operands.size() - 1) {
      // Insert a simple jump to the exit block for the last block
      insertJump(bExit);
    } else {
      // Create conditional jump to the exit block if the first operand was true, otherwise to the next block
      insertCondJump(shortCircuitBlocks.at(i).second, bExit, shortCircuitBlocks.at(i + 1).first);
    }
  }

  // Get the result with the phi node
  switchToBlock(bExit);
  llvm::PHINode *result = builder.CreatePHI(firstOperandValue->getType(), node->operands.size(), "lor_phi");
  for (const auto &[incomingBlock, value] : shortCircuitBlocks)
    result->addIncoming(value, incomingBlock);

  // Return the result
  return LLVMExprResult{.value = result};
}

std::any IRGenerator::visitLogicalAndExpr(const LogicalAndExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // It is a logical and expression
  // Create exit block for short-circuiting
  const std::string codeLoc = node->codeLoc.toPrettyLineAndColumn();
  llvm::BasicBlock *bExit = createBlock("land.exit." + codeLoc);

  // Visit the first operand
  llvm::Value *firstOperandValue = resolveValue(node->operands.front());

  // Prepare an array for value-to-block-mapping
  std::vector<std::pair<llvm::BasicBlock *, llvm::Value *>> shortCircuitBlocks;
  shortCircuitBlocks.reserve(node->operands.size());
  // The first element is the first operand value with the original block
  shortCircuitBlocks.emplace_back(builder.GetInsertBlock(), firstOperandValue);
  // Create a block for each additional operand and save it to the mapping
  for (size_t i = 1; i < node->operands.size(); i++)
    shortCircuitBlocks.emplace_back(createBlock("land." + std::to_string(i) + "." + codeLoc), nullptr);
  // Create conditional jump to the exit block if the first operand was true, otherwise to the next block
  insertCondJump(firstOperandValue, shortCircuitBlocks.at(1).first, bExit);

  // Create block for each operand
  for (size_t i = 1; i < node->operands.size(); i++) {
    // Switch to the next block
    switchToBlock(shortCircuitBlocks.at(i).first);
    // Evaluate operand and save the result in the mapping
    shortCircuitBlocks.at(i).second = resolveValue(node->operands[i]);
    // Replace the array entry with the current insert block, since the insert block could have changed in the meantime
    shortCircuitBlocks.at(i).first = builder.GetInsertBlock();
    // Check if there are more blocks to process
    if (i == node->operands.size() - 1) {
      // Insert a simple jump to the exit block for the last block
      insertJump(bExit);
    } else {
      // Create conditional jump to the exit block if the operand was true, otherwise to the next block
      insertCondJump(shortCircuitBlocks.at(i).second, shortCircuitBlocks.at(i + 1).first, bExit);
    }
  }

  // Get the result with the phi node
  switchToBlock(bExit);
  llvm::PHINode *result = builder.CreatePHI(firstOperandValue->getType(), node->operands.size(), "land_phi");
  for (const auto &[incomingBlock, value] : shortCircuitBlocks)
    result->addIncoming(value, incomingBlock);

  // Return the result
  return LLVMExprResult{.value = result};
}

std::any IRGenerator::visitBitwiseOrExpr(const BitwiseOrExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // It is a bitwise or expression
  // Evaluate first operand
  const BitwiseXorExprNode *lhsNode = node->operands.front();
  const QualType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto result = std::any_cast<LLVMExprResult>(visit(lhsNode));

  // Evaluate all additional operands
  for (size_t i = 1; i < node->operands.size(); i++) {
    // Evaluate the operand
    const BitwiseXorExprNode *rhsNode = node->operands[i];
    const QualType rhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
    auto rhs = std::any_cast<LLVMExprResult>(visit(rhsNode));
    result = conversionManager.getBitwiseOrInst(node, result, lhsSTy, rhs, rhsSTy, i - 1);
  }

  // Return result
  return result;
}

std::any IRGenerator::visitBitwiseXorExpr(const BitwiseXorExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // It is a bitwise xor expression
  // Evaluate first operand
  const BitwiseAndExprNode *lhsNode = node->operands.front();
  const QualType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto result = std::any_cast<LLVMExprResult>(visit(lhsNode));

  // Evaluate all additional operands
  for (size_t i = 1; i < node->operands.size(); i++) {
    // Evaluate the operand
    const BitwiseAndExprNode *rhsNode = node->operands[i];
    const QualType rhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
    auto rhs = std::any_cast<LLVMExprResult>(visit(rhsNode));
    result = conversionManager.getBitwiseXorInst(node, result, lhsSTy, rhs, rhsSTy);
  }

  // Return result
  return result;
}

std::any IRGenerator::visitBitwiseAndExpr(const BitwiseAndExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // It is a bitwise and expression
  // Evaluate first operand
  const EqualityExprNode *lhsNode = node->operands.front();
  const QualType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto result = std::any_cast<LLVMExprResult>(visit(lhsNode));

  // Evaluate all additional operands
  for (size_t i = 1; i < node->operands.size(); i++) {
    // Evaluate the operand
    const EqualityExprNode *rhsNode = node->operands[i];
    const QualType rhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
    auto rhs = std::any_cast<LLVMExprResult>(visit(rhsNode));
    result = conversionManager.getBitwiseAndInst(rhsNode, result, lhsSTy, rhs, rhsSTy, i - 1);
  }

  // Return result
  return result;
}

std::any IRGenerator::visitEqualityExpr(const EqualityExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // It is an equality expression
  // Evaluate lhs
  const RelationalExprNode *lhsNode = node->operands[0];
  const QualType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto result = std::any_cast<LLVMExprResult>(visit(lhsNode));

  // Evaluate rhs
  const RelationalExprNode *rhsNode = node->operands[1];
  const QualType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
  auto rhs = std::any_cast<LLVMExprResult>(visit(rhsNode));

  // Retrieve the result value, based on the exact operator
  switch (node->op) {
  case EqualityExprNode::OP_EQUAL:
    result = conversionManager.getEqualInst(node, result, lhsSTy, rhs, rhsSTy, 0);
    break;
  case EqualityExprNode::OP_NOT_EQUAL:
    result = conversionManager.getNotEqualInst(node, result, lhsSTy, rhs, rhsSTy, 0);
    break;
  default:                                                              // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "EqualityExpr fall-through"); // GCOV_EXCL_LINE
  }

  // Return the result
  return result;
}

std::any IRGenerator::visitRelationalExpr(const RelationalExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // It is a relational expression
  // Evaluate lhs
  const ShiftExprNode *lhsNode = node->operands[0];
  const QualType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto result = std::any_cast<LLVMExprResult>(visit(lhsNode));

  // Evaluate rhs
  const ShiftExprNode *rhsNode = node->operands[1];
  const QualType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
  auto rhs = std::any_cast<LLVMExprResult>(visit(rhsNode));

  // Retrieve the result value, based on the exact operator
  switch (node->op) {
  case RelationalExprNode::OP_LESS:
    result = conversionManager.getLessInst(node, result, lhsSTy, rhs, rhsSTy);
    break;
  case RelationalExprNode::OP_GREATER:
    result = conversionManager.getGreaterInst(node, result, lhsSTy, rhs, rhsSTy);
    break;
  case RelationalExprNode::OP_LESS_EQUAL:
    result = conversionManager.getLessEqualInst(node, result, lhsSTy, rhs, rhsSTy);
    break;
  case RelationalExprNode::OP_GREATER_EQUAL:
    result = conversionManager.getGreaterEqualInst(node, result, lhsSTy, rhs, rhsSTy);
    break;
  default:                                                              // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "EqualityExpr fall-through"); // GCOV_EXCL_LINE
  }

  // Return the result
  return result;
}

std::any IRGenerator::visitShiftExpr(const ShiftExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // It is a shift expression
  // Evaluate lhs
  const AdditiveExprNode *lhsNode = node->operands[0];
  const QualType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto result = std::any_cast<LLVMExprResult>(visit(lhsNode));

  // Evaluate rhs
  const AdditiveExprNode *rhsNode = node->operands[1];
  const QualType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
  auto rhs = std::any_cast<LLVMExprResult>(visit(rhsNode));

  // Retrieve the result value, based on the exact operator
  switch (node->op) {
  case ShiftExprNode::OP_SHIFT_LEFT:
    result = conversionManager.getShiftLeftInst(node, result, lhsSTy, rhs, rhsSTy, 0);
    break;
  case ShiftExprNode::OP_SHIFT_RIGHT:
    result = conversionManager.getShiftRightInst(node, result, lhsSTy, rhs, rhsSTy, 0);
    break;
  default:                                                           // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "ShiftExpr fall-through"); // GCOV_EXCL_LINE
  }

  // Return the result
  return result;
}

std::any IRGenerator::visitAdditiveExpr(const AdditiveExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // It is an additive expression
  // Evaluate first operand
  const MultiplicativeExprNode *lhsNode = node->operands[0];
  QualType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto lhs = std::any_cast<LLVMExprResult>(visit(lhsNode));

  auto opQueue = node->opQueue;
  size_t operandIndex = 1;
  while (!opQueue.empty()) {
    const size_t operatorIndex = operandIndex - 1;
    // Evaluate next operand
    const MultiplicativeExprNode *rhsNode = node->operands[operandIndex++];
    assert(rhsNode != nullptr);
    const QualType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
    auto rhs = std::any_cast<LLVMExprResult>(visit(rhsNode));

    // Retrieve the result, based on the exact operator
    switch (opQueue.front().first) {
    case AdditiveExprNode::OP_PLUS:
      lhs = conversionManager.getPlusInst(node, lhs, lhsSTy, rhs, rhsSTy, operatorIndex);
      break;
    case AdditiveExprNode::OP_MINUS:
      lhs = conversionManager.getMinusInst(node, lhs, lhsSTy, rhs, rhsSTy, operatorIndex);
      break;
    default:                                                              // GCOV_EXCL_LINE
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
  if (node->operands.size() == 1)
    return visit(node->operands.front());

  // It is an additive expression
  // Evaluate first operand
  const CastExprNode *lhsNode = node->operands[0];
  QualType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto result = std::any_cast<LLVMExprResult>(visit(lhsNode));

  auto opQueue = node->opQueue;
  size_t operandIndex = 1;
  while (!opQueue.empty()) {
    const size_t operatorIndex = operandIndex - 1;
    // Evaluate next operand
    const CastExprNode *rhsNode = node->operands[operandIndex++];
    assert(rhsNode != nullptr);
    const QualType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
    auto rhs = std::any_cast<LLVMExprResult>(visit(rhsNode));

    // Retrieve the result, based on the exact operator
    switch (opQueue.front().first) {
    case MultiplicativeExprNode::OP_MUL:
      result = conversionManager.getMulInst(node, result, lhsSTy, rhs, rhsSTy, operatorIndex);
      break;
    case MultiplicativeExprNode::OP_DIV:
      result = conversionManager.getDivInst(node, result, lhsSTy, rhs, rhsSTy, operatorIndex);
      break;
    case MultiplicativeExprNode::OP_REM:
      result = conversionManager.getRemInst(node, result, lhsSTy, rhs, rhsSTy);
      break;
    default:                                                                    // GCOV_EXCL_LINE
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
  if (!node->isCast)
    return visit(node->prefixUnaryExpr);

  // It is a cast expression
  // Retrieve target symbol type
  const QualType targetSTy = node->getEvaluatedSymbolType(manIdx);

  // Evaluate rhs
  const PrefixUnaryExprNode *rhsNode = node->prefixUnaryExpr;
  const QualType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
  auto rhs = std::any_cast<LLVMExprResult>(visit(rhsNode));

  // Retrieve the result value
  const LLVMExprResult result = conversionManager.getCastInst(node, targetSTy, rhs, rhsSTy);

  // Return the result
  return result;
}

std::any IRGenerator::visitPrefixUnaryExpr(const PrefixUnaryExprNode *node) {
  diGenerator.setSourceLocation(node);

  // If no operator is applied, simply visit the atomic expression
  if (node->op == PrefixUnaryExprNode::OP_NONE)
    return visit(node->postfixUnaryExpr);

  // Evaluate lhs
  const PrefixUnaryExprNode *lhsNode = node->prefixUnaryExpr;
  const QualType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto lhs = std::any_cast<LLVMExprResult>(visit(lhsNode));

  switch (node->op) {
  case PrefixUnaryExprNode::OP_MINUS: {
    // Execute operation
    lhs = conversionManager.getPrefixMinusInst(node, lhs, lhsSTy);

    // This operator can not work in-place, so we need additional memory
    lhs.ptr = insertAlloca(lhs.value->getType());

    // Store the new value
    insertStore(lhs.value, lhs.ptr);

    break;
  }
  case PrefixUnaryExprNode::OP_PLUS_PLUS: {
    // Make sure the value is present
    resolveValue(lhsNode, lhs);

    // Execute operation
    lhs.value = conversionManager.getPrefixPlusPlusInst(node, lhs, lhsSTy).value;

    // If this operation happens on a volatile variable, store the value directly
    if (lhs.entry && lhs.entry->isVolatile)
      insertStore(lhs.value, lhs.ptr, true);

    // Save to the existing address if possible, otherwise (e.g. for literals) allocate new space
    if (!lhs.ptr)
      lhs.ptr = insertAlloca(lhs.value->getType());

    // Store the new value
    insertStore(lhs.value, lhs.ptr);

    break;
  }
  case PrefixUnaryExprNode::OP_MINUS_MINUS: {
    // Make sure the value is present
    resolveValue(lhsNode, lhs);

    // Execute operation
    lhs.value = conversionManager.getPrefixMinusMinusInst(node, lhs, lhsSTy).value;

    // If this operation happens on a volatile variable, store the value directly
    if (lhs.entry && lhs.entry->isVolatile)
      insertStore(lhs.value, lhs.ptr, true);

    // Save to the existing address if possible, otherwise (e.g. for literals) allocate new space
    if (!lhs.ptr)
      lhs.ptr = insertAlloca(lhs.value->getType());

    // Store the new value
    insertStore(lhs.value, lhs.ptr);

    break;
  }
  case PrefixUnaryExprNode::OP_NOT: {
    // Make sure the value is present
    resolveValue(lhsNode, lhs);

    // Execute operation
    lhs = conversionManager.getPrefixNotInst(node, lhs, lhsSTy);

    // This operator can not work in-place, so we need additional memory
    lhs.ptr = insertAlloca(lhs.value->getType());

    // Store the new value
    insertStore(lhs.value, lhs.ptr, lhs.entry && lhs.entry->isVolatile);

    break;
  }
  case PrefixUnaryExprNode::OP_BITWISE_NOT: {
    // Make sure the value is present
    resolveValue(lhsNode, lhs);

    // Execute operation
    lhs = conversionManager.getPrefixBitwiseNotInst(node, lhs, lhsSTy);

    // This operator can not work in-place, so we need additional memory
    lhs.ptr = insertAlloca(lhs.value->getType());

    // Store the new value
    insertStore(lhs.value, lhs.ptr, lhs.entry && lhs.entry->isVolatile);

    break;
  }
  case PrefixUnaryExprNode::OP_DEREFERENCE: {
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
  default:                                                                 // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "PrefixUnaryExpr fall-through"); // GCOV_EXCL_LINE
  }

  return lhs;
}

std::any IRGenerator::visitPostfixUnaryExpr(const PostfixUnaryExprNode *node) {
  diGenerator.setSourceLocation(node);

  // If no operator is applied, simply visit the atomic expression
  if (node->op == PostfixUnaryExprNode::OP_NONE)
    return visit(node->atomicExpr);

  // Evaluate lhs
  const PostfixUnaryExprNode *lhsNode = node->postfixUnaryExpr;
  QualType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  auto lhs = std::any_cast<LLVMExprResult>(visit(lhsNode));

  switch (node->op) {
  case PostfixUnaryExprNode::OP_SUBSCRIPT: {
    lhsSTy = lhsSTy.removeReferenceWrapper();

    // Get the index value
    const AssignExprNode *indexExpr = node->subscriptIndexExpr;
    llvm::Value *indexValue = resolveValue(indexExpr);
    // Come up with the address
    if (lhsSTy.isArray() && lhsSTy.getArraySize() != ARRAY_SIZE_UNKNOWN) { // Array
      // Make sure the address is present
      resolveAddress(lhs);

      // Calculate address of array item
      llvm::Type *lhsTy = lhsSTy.toLLVMType(sourceFile);
      llvm::Value *indices[2] = {builder.getInt64(0), indexValue};
      lhs.ptr = insertInBoundsGEP(lhsTy, lhs.ptr, indices);
    } else { // Pointer
      // Make sure the value is present
      resolveValue(lhsNode, lhs);
      assert(lhs.value != nullptr);

      // Now the pointer is the value
      lhs.ptr = lhs.value;

      llvm::Type *lhsTy = lhsSTy.getContained().toLLVMType(sourceFile);
      // Calculate address of pointer item
      lhs.ptr = insertInBoundsGEP(lhsTy, lhs.ptr, indexValue);
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
    autoDeReferencePtr(lhs.ptr, lhsSTy);
    assert(lhsSTy.is(TY_STRUCT));

    // Retrieve struct scope
    const std::string &fieldName = node->identifier;
    Scope *structScope = lhsSTy.getBodyScope();

    // Retrieve field entry
    std::vector<size_t> indexPath;
    lhs.entry = structScope->symbolTable.lookupInComposedFields(fieldName, indexPath);
    assert(lhs.entry != nullptr);
    const QualType fieldSymbolType = lhs.entry->getQualType();

    // Get address of the field in the struct instance
    std::vector<llvm::Value *> indices = {builder.getInt64(0)};
    for (const size_t index : indexPath)
      indices.push_back(builder.getInt32(index));
    const std::string name = fieldName + "_addr";
    llvm::Value *memberAddr = insertInBoundsGEP(lhsSTy.toLLVMType(sourceFile), lhs.ptr, indices, name);

    // Set as ptr or refPtr, depending on the type
    if (fieldSymbolType.isRef()) {
      lhs.ptr = nullptr;
      lhs.refPtr = memberAddr;
    } else {
      lhs.ptr = memberAddr;
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
    const LLVMExprResult result = conversionManager.getPostfixPlusPlusInst(node, lhs, lhsSTy, 0);

    // Save the new value to the old address
    if (conversionManager.callsOverloadedOpFct(node, 0)) {
      lhs.value = result.value;
      lhs.ptr = result.ptr;
    } else {
      insertStore(result.value, lhs.ptr, lhs.entry && lhs.entry->isVolatile);
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
    const LLVMExprResult result = conversionManager.getPostfixMinusMinusInst(node, lhs, lhsSTy, 0);

    // Save the new value to the old address
    if (conversionManager.callsOverloadedOpFct(node, 0)) {
      lhs.value = result.value;
      lhs.ptr = result.ptr;
    } else {
      insertStore(result.value, lhs.ptr, lhs.entry && lhs.entry->isVolatile);
      lhs.ptr = nullptr;
    }
    break;
  }
  default:                                                                  // GCOV_EXCL_LINE
    throw CompilerError(UNHANDLED_BRANCH, "PostfixUnaryExpr fall-through"); // GCOV_EXCL_LINE
  }

  return lhs;
}

std::any IRGenerator::visitAtomicExpr(const AtomicExprNode *node) {
  diGenerator.setSourceLocation(node);

  // If constant
  if (node->constant()) {
    const auto constantValue = std::any_cast<llvm::Constant *>(visit(node->constant()));
    return LLVMExprResult{.constant = constantValue};
  }

  // If value
  if (node->value())
    return visit(node->value());

  // Is assign expression
  if (node->assignExpr())
    return visit(node->assignExpr());

  // Check for builtin calls
  if (node->printfCall())
    return visit(node->printfCall());
  if (node->sizeofCall())
    return visit(node->sizeofCall());
  if (node->alignofCall())
    return visit(node->alignofCall());
  if (node->lenCall())
    return visit(node->lenCall());
  if (node->panicCall())
    return visit(node->panicCall());
  if (node->sysCall())
    return visit(node->sysCall());

  // Identifier (local or global variable access)
  assert(!node->identifierFragments.empty());

  // Get symbol table entry
  const auto &[varEntry, accessScope, capture] = node->data.at(manIdx);
  assert(varEntry != nullptr);
  assert(accessScope != nullptr);
  const QualType varSymbolType = varEntry->getQualType();
  llvm::Type *varType = varSymbolType.toLLVMType(sourceFile);

  // Check if external global variable
  if (varEntry->global && accessScope->isImportedBy(rootScope)) {
    // External global variables need to be declared and allocated in the current module
    llvm::Value *varAddress = module->getOrInsertGlobal(varEntry->name, varType);
    varEntry->updateAddress(varAddress);
  }

  // Check if enum item
  if (accessScope->type == ScopeType::ENUM) {
    const auto itemNode = spice_pointer_cast<const EnumItemNode *>(varEntry->declNode);
    llvm::Constant *constantItemValue = llvm::ConstantInt::get(varType, itemNode->itemValue);
    return LLVMExprResult{.constant = constantItemValue, .entry = varEntry};
  }

  llvm::Value *address = varEntry->getAddress();
  assert(address != nullptr);

  // If this is a function/procedure reference, return it as value
  if (varEntry->global && varSymbolType.isOneOf({TY_FUNCTION, TY_PROCEDURE})) {
    llvm::Value *fatPtr = buildFatFctPtr(nullptr, nullptr, address);
    return LLVMExprResult{.ptr = fatPtr, .entry = varEntry};
  }

  // Load the address of the referenced variable
  if (varSymbolType.isRef() || (capture && capture->getMode() == BY_REFERENCE))
    return LLVMExprResult{.refPtr = address, .entry = varEntry};

  return LLVMExprResult{.ptr = address, .entry = varEntry};
}

} // namespace spice::compiler
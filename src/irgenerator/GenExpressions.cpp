// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>

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
      const SymbolType lhsSType = lhsNode->getEvaluatedSymbolType(manIdx);
      const SymbolType rhsSType = rhsNode->getEvaluatedSymbolType(manIdx);

      // Retrieve rhs
      auto rhs = std::any_cast<ExprResult>(visit(rhsNode));
      // Retrieve lhs
      auto lhs = std::any_cast<ExprResult>(visit(lhsNode));
      assert(lhs.entry != nullptr);

      PtrAndValue result;
      SymbolTableEntry *lhsEntry = nullptr;
      switch (node->op) {
      case AssignExprNode::OP_PLUS_EQUAL:
        result = conversionManager.getPlusEqualInst(lhs, lhsNode, rhs, rhsNode, currentScope);
        break;
      case AssignExprNode::OP_MINUS_EQUAL:
        result.value = conversionManager.getMinusEqualInst(lhs, lhsNode, rhs, rhsNode, currentScope);
        break;
      case AssignExprNode::OP_MUL_EQUAL:
        result = conversionManager.getMulEqualInst(lhs, lhsNode, rhs, rhsNode, currentScope);
        break;
      case AssignExprNode::OP_DIV_EQUAL:
        result.value = conversionManager.getDivEqualInst(lhs, lhsNode, rhs, rhsNode, currentScope);
        break;
      case AssignExprNode::OP_REM_EQUAL:
        result.value = conversionManager.getRemEqualInst(lhs, lhsNode, rhs, rhsNode, currentScope);
        break;
      case AssignExprNode::OP_SHL_EQUAL:
        result.value = conversionManager.getSHLEqualInst(lhs, lhsNode, rhs, rhsNode, currentScope);
        break;
      case AssignExprNode::OP_SHR_EQUAL:
        result.value = conversionManager.getSHREqualInst(lhs, lhsNode, rhs, rhsNode, currentScope);
        break;
      case AssignExprNode::OP_AND_EQUAL:
        result.value = conversionManager.getAndEqualInst(lhs, lhsNode, rhs, rhsNode, currentScope);
        break;
      case AssignExprNode::OP_OR_EQUAL:
        result.value = conversionManager.getOrEqualInst(lhs, lhsNode, rhs, rhsNode, currentScope);
        break;
      case AssignExprNode::OP_XOR_EQUAL:
        result.value = conversionManager.getXorEqualInst(lhs, lhsNode, rhs, rhsNode, currentScope);
        break;
      default:
        throw std::runtime_error("Assign op fall-through");
      }
      assert(lhsEntry != nullptr);

      if (result.ptr) {
        // Check if the operation updated the address
        lhsEntry->updateAddress(result.ptr);
      } else {
        // Store the result
        result.ptr = lhsEntry->getAddress();
        builder.CreateStore(result.value, result.ptr, lhsEntry->isVolatile);
      }
      return ExprResult{.ptr = result.ptr, .value = result.value, .entry = lhsEntry};
    }
  }

  // This is a fallthrough case -> throw an error
  throw std::runtime_error("Internal compiler error: Assign stmt fall-through"); // GCOV_EXCL_LINE
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
  const std::string codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bExit = createBlock("lor.exit." + codeLine);

  // Visit the first operand
  llvm::Value *firstOperandValue = resolveValue(node->operands().front());

  // Prepare an array for value-to-block-mapping
  std::pair<llvm::Value *, llvm::BasicBlock *> shortCircuitBlocks[node->operands().size()];
  // The first element is the first operand value with the original block
  shortCircuitBlocks[0] = {firstOperandValue, builder.GetInsertBlock()};
  // Create a block for each additional operand and save it to the mapping
  for (size_t i = 1; i < node->operands().size(); i++) {
    llvm::BasicBlock *nextBlock = createBlock("lor." + std::to_string(i) + "." + codeLine);
    shortCircuitBlocks[i] = {nullptr, nextBlock};
  }
  // Create conditional jump to the exit block if the first operand was true, otherwise to the next block
  insertCondJump(firstOperandValue, bExit, shortCircuitBlocks[1].second);

  // Create block for each operand
  for (int i = 1; i < node->operands().size(); i++) {
    // Switch to the next block
    switchToBlock(shortCircuitBlocks[i].second);
    // Evaluate operand and save the result in the mapping
    shortCircuitBlocks[i].first = resolveValue(node->operands()[i]);
    // Check if there are more blocks to process
    if (i < node->operands().size() - 1) {
      // Create conditional jump to the exit block if the first operand was true, otherwise to the next block
      insertCondJump(shortCircuitBlocks[i].first, bExit, shortCircuitBlocks[i + 1].second);
      continue;
    }
    // Insert a simple jump to the exit block for the last block
    insertJump(bExit);
  }

  // Get the result with the phi node
  switchToBlock(bExit);
  llvm::PHINode *result = builder.CreatePHI(firstOperandValue->getType(), node->operands().size(), "lor_phi");
  for (const auto &[value, incomingBlock] : shortCircuitBlocks)
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
  const std::string codeLine = node->codeLoc.toPrettyLine();
  llvm::BasicBlock *bExit = createBlock("land.exit." + codeLine);

  // Visit the first operand
  llvm::Value *firstOperandValue = resolveValue(node->operands().front());

  // Prepare an array for value-to-block-mapping
  std::pair<llvm::Value *, llvm::BasicBlock *> shortCircuitBlocks[node->operands().size()];
  // The first element is the first operand value with the original block
  shortCircuitBlocks[0] = {firstOperandValue, builder.GetInsertBlock()};
  // Create a block for each additional operand and save it to the mapping
  for (size_t i = 1; i < node->operands().size(); i++) {
    llvm::BasicBlock *nextBlock = createBlock("land." + std::to_string(i) + "." + codeLine);
    shortCircuitBlocks[i] = {nullptr, nextBlock};
  }
  // Create conditional jump to the exit block if the first operand was true, otherwise to the next block
  insertCondJump(firstOperandValue, shortCircuitBlocks[1].second, bExit);

  // Create block for each operand
  for (int i = 1; i < node->operands().size(); i++) {
    // Switch to the next block
    switchToBlock(shortCircuitBlocks[i].second);
    // Evaluate operand and save the result in the mapping
    shortCircuitBlocks[i].first = resolveValue(node->operands()[i]);
    // Check if there are more blocks to process
    if (i < node->operands().size() - 1) {
      // Create conditional jump to the exit block if the first operand was true, otherwise to the next block
      insertCondJump(shortCircuitBlocks[i].first, shortCircuitBlocks[i + 1].second, bExit);
      continue;
    }
    // Insert a simple jump to the exit block for the last block
    insertJump(bExit);
  }

  // Get the result with the phi node
  switchToBlock(bExit);
  llvm::PHINode *result = builder.CreatePHI(firstOperandValue->getType(), node->operands().size(), "land_phi");
  for (const auto &[value, incomingBlock] : shortCircuitBlocks)
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
  auto lhs = std::any_cast<ExprResult>(visit(lhsNode));

  // Evaluate all additional operands
  for (int i = 1; i < node->operands().size(); i++) {
    // Evaluate the operand
    BitwiseXorExprNode *rhsNode = node->operands()[i];
    const SymbolType rhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
    auto rhs = std::any_cast<ExprResult>(visit(rhsNode));
    lhs.value = conversionManager.getBitwiseOrInst(lhs, lhsNode, rhs, rhsNode, currentScope);
  }

  // Return result
  return ExprResult{.value = lhs.value};
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
  auto lhs = std::any_cast<ExprResult>(visit(lhsNode));

  // Evaluate all additional operands
  for (int i = 1; i < node->operands().size(); i++) {
    // Evaluate the operand
    BitwiseAndExprNode *rhsNode = node->operands()[i];
    const SymbolType rhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
    auto rhs = std::any_cast<ExprResult>(visit(rhsNode));
    lhs.value = conversionManager.getBitwiseXorInst(lhs, lhsNode, rhs, rhsNode, currentScope);
  }

  // Return result
  return ExprResult{.value = lhs.value};
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
  auto lhs = std::any_cast<ExprResult>(visit(lhsNode));

  // Evaluate all additional operands
  for (int i = 1; i < node->operands().size(); i++) {
    // Evaluate the operand
    EqualityExprNode *rhsNode = node->operands()[i];
    const SymbolType rhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
    auto rhs = std::any_cast<ExprResult>(visit(rhsNode));
    lhs.value = conversionManager.getBitwiseAndInst(lhs, lhsNode, rhs, rhsNode, currentScope);
  }

  // Return result
  return ExprResult{.value = lhs.value};
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
  auto lhs = std::any_cast<ExprResult>(visit(lhsNode));

  // Evaluate rhs
  RelationalExprNode *rhsNode = node->operands()[1];
  const SymbolType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
  auto rhs = std::any_cast<ExprResult>(visit(rhsNode));

  // Retrieve the result value, based on the exact operator
  llvm::Value *result;
  switch (node->op) {
  case EqualityExprNode::OP_EQUAL:
    result = conversionManager.getEqualInst(lhs, lhsNode, rhs, rhsNode, currentScope);
    break;
  case EqualityExprNode::OP_NOT_EQUAL:
    result = conversionManager.getNotEqualInst(lhs, lhsNode, rhs, rhsNode, currentScope);
    break;
  default:
    throw std::runtime_error("Equality expr fall-through");
  }

  // Return the result
  return ExprResult{.value = result};
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
  auto lhs = std::any_cast<ExprResult>(visit(lhsNode));

  // Evaluate rhs
  ShiftExprNode *rhsNode = node->operands()[1];
  const SymbolType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
  auto rhs = std::any_cast<ExprResult>(visit(rhsNode));

  // Retrieve the result value, based on the exact operator
  llvm::Value *result;
  switch (node->op) {
  case RelationalExprNode::OP_LESS:
    result = conversionManager.getLessInst(lhs, lhsNode, rhs, rhsNode, currentScope);
    break;
  case RelationalExprNode::OP_GREATER:
    result = conversionManager.getGreaterInst(lhs, lhsNode, rhs, rhsNode, currentScope);
    break;
  case RelationalExprNode::OP_LESS_EQUAL:
    result = conversionManager.getLessEqualInst(lhs, lhsNode, rhs, rhsNode, currentScope);
    break;
  case RelationalExprNode::OP_GREATER_EQUAL:
    result = conversionManager.getGreaterEqualInst(lhs, lhsNode, rhs, rhsNode, currentScope);
    break;
  default:
    throw std::runtime_error("Relational expr fall-through");
  }

  // Return the result
  return ExprResult{.value = result};
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
  auto lhs = std::any_cast<ExprResult>(visit(lhsNode));

  // Evaluate rhs
  AdditiveExprNode *rhsNode = node->operands()[1];
  const SymbolType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
  auto rhs = std::any_cast<ExprResult>(visit(rhsNode));

  // Retrieve the result value, based on the exact operator
  llvm::Value *result;
  switch (node->op) {
  case ShiftExprNode::OP_SHIFT_LEFT:
    result = conversionManager.getShiftLeftInst(lhs, lhsNode, rhs, rhsNode, currentScope);
    break;
  case ShiftExprNode::OP_SHIFT_RIGHT:
    result = conversionManager.getShiftRightInst(lhs, lhsNode, rhs, rhsNode, currentScope);
    break;
  default:
    throw std::runtime_error("Shift expr fall-through");
  }

  // Return the result
  return ExprResult{.value = result};
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
    // Evaluate next operand
    MultiplicativeExprNode *rhsNode = node->operands()[operandIndex++];
    assert(rhsNode != nullptr);
    const SymbolType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
    auto rhs = std::any_cast<ExprResult>(rhsNode);

    // Retrieve the result, based on the exact operator
    PtrAndValue result = {nullptr, nullptr};
    switch (opQueue.front().first) {
    case AdditiveExprNode::OP_PLUS:
      result = conversionManager.getPlusInst(lhs, lhsNode, rhs, rhsNode, currentScope);
      break;
    case AdditiveExprNode::OP_MINUS:
      result.value = conversionManager.getMinusInst(lhs, lhsNode, rhs, rhsNode, currentScope);
      break;
    default:
      throw std::runtime_error("Additive expr fall-through");
    }

    // Retrieve the new lhs symbol type
    lhsSTy = opQueue.front().second;

    // Load value from pointer if it is not present already
    if (result.value) {
      lhs.value = result.value;
    } else if (result.ptr) {
      lhs.value = builder.CreateLoad(lhsSTy.toLLVMType(context, currentScope), result.ptr);
    }

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
  auto lhs = std::any_cast<ExprResult>(visit(lhsNode));

  auto opQueue = node->opQueue;
  size_t operandIndex = 1;
  while (!opQueue.empty()) {
    // Evaluate next operand
    CastExprNode *rhsNode = node->operands()[operandIndex++];
    assert(rhsNode != nullptr);
    const SymbolType rhsSTy = rhsNode->getEvaluatedSymbolType(manIdx);
    auto rhs = std::any_cast<ExprResult>(rhsNode);

    // Retrieve the result, based on the exact operator
    PtrAndValue result = {nullptr, nullptr};
    switch (opQueue.front().first) {
    case MultiplicativeExprNode::OP_MUL:
      result = conversionManager.getMulInst(lhs, lhsNode, rhs, rhsNode, currentScope);
      break;
    case MultiplicativeExprNode::OP_DIV:
      result.value = conversionManager.getDivInst(lhs, lhsNode, rhs, rhsNode, currentScope);
      break;
    case MultiplicativeExprNode::OP_REM:
      result.value = conversionManager.getRemInst(lhs, lhsNode, rhs, rhsNode, currentScope);
      break;
    default:
      throw std::runtime_error("Additive expr fall-through");
    }

    // Retrieve the new lhs symbol type
    lhsSTy = opQueue.front().second;

    // Load value from pointer if it is not present already
    if (result.value) {
      lhs.value = result.value;
    } else if (result.ptr) {
      lhs.value = builder.CreateLoad(lhsSTy.toLLVMType(context, currentScope), result.ptr);
    }

    opQueue.pop();
  }

  // Return the result
  return lhs;
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
  llvm::Value *result = conversionManager.getCastInst(targetSTy, rhs, rhsNode, currentScope);

  // Return the result
  return ExprResult{.value = result};
}

std::any IRGenerator::visitPrefixUnaryExpr(const PrefixUnaryExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->opQueue.empty())
    return visit(node->postfixUnaryExpr());

  // It is a prefix unary expression
  // Evaluate lhs
  PostfixUnaryExprNode *lhsNode = node->postfixUnaryExpr();
  SymbolType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  llvm::Type *lhsTy = lhsSTy.toLLVMType(context, currentScope);
  auto lhs = std::any_cast<ExprResult>(visit(lhsNode));

  bool isVolatile = false;
  bool storeValue = false;

  // Loop over additional operators and operands
  auto opQueue = node->opQueue;
  while (!opQueue.empty()) {
    switch (opQueue.front().first) {
    case PrefixUnaryExprNode::OP_MINUS: {
      // Execute operation
      lhs.value = conversionManager.getPrefixMinusInst(lhs, lhsNode, currentScope);
      storeValue = true;
      break;
    }
    case PrefixUnaryExprNode::OP_PLUS_PLUS: {
      // Load value of lhs
      if (!lhs.value)
        lhs.value = builder.CreateLoad(lhsTy, lhs.ptr, isVolatile);

      // Execute operation
      lhs.value = conversionManager.getPrefixPlusPlusInst(lhs, lhsNode, currentScope);

      // If this operation happens on a volatile variable, store the value directly
      if (lhs.entry && lhs.entry->isVolatile) {
        isVolatile = true;
        builder.CreateStore(lhs.value, lhs.ptr, isVolatile);
      }

      storeValue = true;
      break;
    }
    case PrefixUnaryExprNode::OP_MINUS_MINUS: {
      // Load value of lhs
      if (!lhs.value)
        lhs.value = builder.CreateLoad(lhsTy, lhs.ptr, isVolatile);

      // Execute operation
      lhs.value = conversionManager.getPostfixMinusMinusInst(lhs, lhsNode, currentScope);

      // If this operation happens on a volatile variable, store the value directly
      if (lhs.entry && lhs.entry->isVolatile) {
        isVolatile = true;
        builder.CreateStore(lhs.value, lhs.ptr, isVolatile);
      }

      storeValue = true;
      break;
    }
    case PrefixUnaryExprNode::OP_NOT: {
      // Load value of lhs
      if (!lhs.value)
        lhs.value = builder.CreateLoad(lhsTy, lhs.ptr, isVolatile);

      // Execute operation
      lhs.value = conversionManager.getPrefixNotInst(lhs, lhsNode, currentScope);

      // This operator can not work in-place, so we need additional memory
      lhs.ptr = insertAlloca(lhs.value->getType());

      storeValue = true;
      break;
    }
    case PrefixUnaryExprNode::OP_BITWISE_NOT: {
      // Load value of lhs
      if (!lhs.value)
        lhs.value = builder.CreateLoad(lhsTy, lhs.ptr, isVolatile);

      // Execute operation
      lhs.value = conversionManager.getPrefixBitwiseNotInst(lhs, lhsNode, currentScope);

      // This operator can not work in-place, so we need additional memory
      lhs.ptr = insertAlloca(lhs.value->getType());

      storeValue = true;
      break;
    }
    case PrefixUnaryExprNode::OP_INDIRECTION: {
      if (!lhs.value) {
        lhs.value = builder.CreateLoad(lhsTy, lhs.ptr);
        lhsSTy = lhsSTy.getContainedTy();
        lhsTy = lhsSTy.toLLVMType(context, currentScope);
      }

      // Execute operation
      lhs.ptr = lhs.value;
      lhs.value = builder.CreateLoad(lhsTy, lhs.value);

      // If this is the last operation, we can skip the final store, as we just stored
      storeValue = false;
      break;
    }
    case PrefixUnaryExprNode::OP_ADDRESS_OF: {
      // If the address is unknown, allocate memory to save the value or constant
      if (!lhs.ptr) {
        llvm::Value *value = lhs.value ?: lhs.constant;
        lhs.ptr = insertAlloca(value->getType());
        builder.CreateStore(value, lhs.ptr);
      }

      // Execute operation
      lhs.value = lhs.ptr;
      lhs.ptr = insertAlloca(lhs.value->getType());
      builder.CreateStore(lhs.value, lhs.ptr);

      // If this is the last operation, we can skip the final store, as we just stored
      storeValue = false;
      break;
    }
    default:
      throw std::runtime_error("PrefixUnary fall-through");
    }

    lhsSTy = opQueue.front().second;
    lhsTy = lhsSTy.toLLVMType(context, currentScope);
    opQueue.pop();
  }

  // Store the value to the address if necessary
  if (storeValue) {
    if (!lhs.ptr)
      lhs.ptr = insertAlloca(lhs.value->getType());
    builder.CreateStore(lhs.value, lhs.ptr, isVolatile);
  }

  return lhs;
}

std::any IRGenerator::visitPostfixUnaryExpr(const PostfixUnaryExprNode *node) {
  diGenerator.setSourceLocation(node);

  // Check if only one operand is present -> loop through
  if (node->opQueue.empty())
    return visit(node->atomicExpr());

  // It is a postfix unary expression
  // Evaluate lhs
  AtomicExprNode *lhsNode = node->atomicExpr();
  SymbolType lhsSTy = lhsNode->getEvaluatedSymbolType(manIdx);
  llvm::Type *lhsTy = lhsSTy.is(TY_IMPORT) ? nullptr : lhsSTy.toLLVMType(context, currentScope);
  auto lhs = std::any_cast<ExprResult>(visit(lhsNode));

  size_t subscriptCounter = 0;
  size_t memberAccessCounter = 0;
  bool isVolatile = false;
  bool storeValue = false;

  // Loop over additional operators and operands
  auto opQueue = node->opQueue;
  while (!opQueue.empty()) {
    switch (opQueue.front().first) {
    case PostfixUnaryExprNode::OP_SUBSCRIPT: {
      // Get the index value
      AssignExprNode *indexExpr = node->assignExpr()[subscriptCounter++];
      llvm::Value *indexValue = resolveValue(indexExpr);

      if (lhsSTy.isArray() && lhsSTy.getArraySize() > 0) { // Array
        // Calculate address of array item
        llvm::Value *indices[2] = {builder.getInt32(0), indexValue};
        lhs.ptr = builder.CreateInBoundsGEP(lhsTy, lhs.ptr, indices);
      } else { // Pointer
        lhs.ptr = builder.CreateLoad(lhsTy, lhs.ptr);
        lhsTy = lhsSTy.getContainedTy().toLLVMType(context, currentScope);
        // Calculate address of pointer item
        lhs.ptr = builder.CreateInBoundsGEP(lhsTy, lhs.ptr, indexValue);
      }

      lhs.value = nullptr;
      break;
    }
    case PostfixUnaryExprNode::OP_MEMBER_ACCESS: {
      // Auto de-reference pointer
      autoDeReferencePtr(lhs.ptr, lhsSTy, currentScope);
      assert(lhsSTy.is(TY_STRUCT));

      // Retrieve struct scope
      const std::string &fieldName = node->identifier.at(memberAccessCounter++);
      Scope *structScope = lhsSTy.getStructBodyScope();
      SymbolTableEntry *fieldEntry = structScope->lookupStrict(fieldName);

      // Get address of the field in the struct instance
      llvm::Value *indices[2] = {builder.getInt32(0), builder.getInt32(fieldEntry->orderIndex)};
      lhs.ptr = builder.CreateInBoundsGEP(lhsSTy.toLLVMType(context, currentScope), lhs.ptr, indices);

      // Set the new symbol type and the value
      lhsSTy = fieldEntry->getType();
      lhs.value = nullptr;
      break;
    }
    case PostfixUnaryExprNode::OP_PLUS_PLUS: {
      // Load value of lhs
      if (!lhs.value)
        lhs.value = builder.CreateLoad(lhsTy, lhs.ptr, isVolatile);

      // Execute operation
      llvm::Value *newLhsValue = conversionManager.getPostfixPlusPlusInst(lhs, lhsNode, currentScope);

      // Save the new value to the old address
      assert(lhs.entry != nullptr);
      isVolatile |= lhs.entry->isVolatile;
      builder.CreateStore(newLhsValue, lhs.ptr, isVolatile);

      // Allocate new space and continue working with the new memory slot
      lhs.ptr = insertAlloca(lhsTy);

      storeValue = true;
      break;
    }
    case PostfixUnaryExprNode::OP_MINUS_MINUS: {
      // Load value of lhs
      if (!lhs.value)
        lhs.value = builder.CreateLoad(lhsTy, lhs.ptr, isVolatile);

      // Execute operation
      llvm::Value *newLhsValue = conversionManager.getPostfixMinusMinusInst(lhs, lhsNode, currentScope);

      // Save the new value to the old address
      assert(lhs.entry != nullptr);
      isVolatile |= lhs.entry->isVolatile;
      builder.CreateStore(newLhsValue, lhs.ptr, isVolatile);

      // Allocate new space and continue working with the new memory slot
      lhs.ptr = insertAlloca(lhsTy);

      storeValue = true;
      break;
    }
    default:
      throw std::runtime_error("PostfixUnary fall-through");
    }

    lhsSTy = opQueue.front().second;
    lhsTy = lhsSTy.toLLVMType(context, currentScope);
    opQueue.pop();
  }

  // Store the value to the address if necessary
  if (storeValue) {
    if (!lhs.ptr)
      lhs.ptr = insertAlloca(lhs.value->getType());
    builder.CreateStore(lhs.value, lhs.ptr, isVolatile);
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
  const SymbolType varSymbolType = varEntry->getType();
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

  assert(varEntry->getAddress() != nullptr);
  return ExprResult{.ptr = varEntry->getAddress(), .entry = varEntry};
}
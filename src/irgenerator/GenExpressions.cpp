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
    // Get symbol types of left and right side
    const SymbolType lhsSType = node->lhs()->getEvaluatedSymbolType();
    const SymbolType rhsSType = node->rhs()->getEvaluatedSymbolType();

    if (node->op == AssignExprNode::OP_ASSIGN) { // Normal assignment
      // Deduce some information about the assignment
      const bool isRefAssign = lhsSType.isReference();
      const bool needsShallowCopy = !isRefAssign && lhsSType.is(TY_STRUCT);

      if (isRefAssign) { // We simply set the address of lhs to the address of rhs
        // Get address of right side
        llvm::Value *rhsAddress = resolveAddress(node->rhs());
        assert(rhsAddress != nullptr);
        // Get entry of left side
        auto lhs = std::any_cast<ExprResult>(visit(node->lhs()));
        assert(lhs.entry != nullptr);
        // Set address of rhs to lhs entry
        lhs.entry->updateAddress(rhsAddress);
        return ExprResult{.ptr = rhsAddress, .entry = lhs.entry};
      }

      // Check if we need to copy the rhs to the lhs. This happens for structs
      if (needsShallowCopy) {
        // Get address of right side
        llvm::Value *rhsAddress = resolveAddress(node->rhs());
        assert(rhsAddress != nullptr);
        // Get entry of left side
        auto lhs = std::any_cast<ExprResult>(visit(node->lhs()));
        assert(lhs.entry != nullptr);
        // Create shallow copy
        llvm::Value *newAddress = copyMemoryShallow(rhsAddress, rhsAddress->getType(), lhs.entry->name, lhs.entry->isVolatile);
        // Set address of lhs to the copy
        lhs.entry->updateAddress(newAddress);
        return ExprResult{.ptr = newAddress, .entry = lhs.entry};
      }

      // We can load the value from the right side and store it to the left side
      // Retrieve value of the right side
      llvm::Value *rhsValue = resolveValue(node->rhs());
      // Retrieve address of the lhs side
      llvm::Value *lhsAddress = resolveAddress(node->lhs());
      // Store the value to the address
      builder.CreateStore(rhsValue, lhsAddress);
      return ExprResult{.ptr = lhsAddress, .value = rhsValue};
    } else { // Compound assignment
      // Retrieve rhs
      auto rhs = std::any_cast<ExprResult>(visit(node->rhs()));
      // Retrieve lhs
      auto lhs = std::any_cast<ExprResult>(visit(node->lhs()));
      assert(lhs.entry != nullptr);

      PtrAndValue result;
      switch (node->op) {
      case AssignExprNode::OP_PLUS_EQUAL:
        result = conversionManager.getPlusEqualInst(lhs, rhs, lhsSType, rhsSType, currentScope);
        break;
      case AssignExprNode::OP_MINUS_EQUAL:
        result.value = conversionManager.getMinusEqualInst(lhs, rhs, lhsSType, rhsSType, currentScope);
        break;
      case AssignExprNode::OP_MUL_EQUAL:
        result = conversionManager.getMulEqualInst(lhs, rhs, lhsSType, rhsSType);
        break;
      case AssignExprNode::OP_DIV_EQUAL:
        result.value = conversionManager.getDivEqualInst(lhs, rhs, lhsSType, rhsSType);
        break;
      case AssignExprNode::OP_REM_EQUAL:
        result.value = conversionManager.getRemEqualInst(lhs, rhs, lhsSType, rhsSType);
        break;
      case AssignExprNode::OP_SHL_EQUAL:
        result.value = conversionManager.getSHLEqualInst(lhs, rhs, lhsSType, rhsSType);
        break;
      case AssignExprNode::OP_SHR_EQUAL:
        result.value = conversionManager.getSHREqualInst(lhs, rhs, lhsSType, rhsSType);
        break;
      case AssignExprNode::OP_AND_EQUAL:
        result.value = conversionManager.getAndEqualInst(lhs, rhs, lhsSType, rhsSType);
        break;
      case AssignExprNode::OP_OR_EQUAL:
        result.value = conversionManager.getOrEqualInst(lhs, rhs, lhsSType, rhsSType);
        break;
      case AssignExprNode::OP_XOR_EQUAL:
        result.value = conversionManager.getXorEqualInst(lhs, rhs, lhsSType, rhsSType);
        break;
      default:
        throw std::runtime_error("Assign op fall-through");
      }

      if (result.ptr) {
        // Check if the operation updated the address
        lhs.entry->updateAddress(result.ptr);
      } else {
        // Store the result
        result.ptr = lhs.entry->getAddress();
        builder.CreateStore(result.value, result.ptr, lhs.entry->isVolatile);
      }
      return ExprResult{.ptr = result.ptr, .value = result.value, .entry = lhs.entry};
    }
  }

  // This is a fallthrough case -> throw an error
  throw std::runtime_error("Internal compiler error: Assign stmt fall-through"); // GCOV_EXCL_LINE
}

std::any IRGenerator::visitTernaryExpr(const TernaryExprNode *node) { return ParallelizableASTVisitor::visitTernaryExpr(node); }

std::any IRGenerator::visitLogicalOrExpr(const LogicalOrExprNode *node) {
  return ParallelizableASTVisitor::visitLogicalOrExpr(node);
}

std::any IRGenerator::visitLogicalAndExpr(const LogicalAndExprNode *node) {
  return ParallelizableASTVisitor::visitLogicalAndExpr(node);
}

std::any IRGenerator::visitBitwiseOrExpr(const BitwiseOrExprNode *node) {
  return ParallelizableASTVisitor::visitBitwiseOrExpr(node);
}

std::any IRGenerator::visitBitwiseXorExpr(const BitwiseXorExprNode *node) {
  return ParallelizableASTVisitor::visitBitwiseXorExpr(node);
}

std::any IRGenerator::visitBitwiseAndExpr(const BitwiseAndExprNode *node) {
  return ParallelizableASTVisitor::visitBitwiseAndExpr(node);
}

std::any IRGenerator::visitEqualityExpr(const EqualityExprNode *node) {
  return ParallelizableASTVisitor::visitEqualityExpr(node);
}

std::any IRGenerator::visitRelationalExpr(const RelationalExprNode *node) {
  return ParallelizableASTVisitor::visitRelationalExpr(node);
}

std::any IRGenerator::visitShiftExpr(const ShiftExprNode *node) { return ParallelizableASTVisitor::visitShiftExpr(node); }

std::any IRGenerator::visitAdditiveExpr(const AdditiveExprNode *node) {
  return ParallelizableASTVisitor::visitAdditiveExpr(node);
}

std::any IRGenerator::visitMultiplicativeExpr(const MultiplicativeExprNode *node) {
  return ParallelizableASTVisitor::visitMultiplicativeExpr(node);
}

std::any IRGenerator::visitCastExpr(const CastExprNode *node) { return ParallelizableASTVisitor::visitCastExpr(node); }

std::any IRGenerator::visitPrefixUnaryExpr(const PrefixUnaryExprNode *node) {
  return ParallelizableASTVisitor::visitPrefixUnaryExpr(node);
}

std::any IRGenerator::visitPostfixUnaryExpr(const PostfixUnaryExprNode *node) {
  return ParallelizableASTVisitor::visitPostfixUnaryExpr(node);
}

std::any IRGenerator::visitAtomicExpr(const AtomicExprNode *node) { return ParallelizableASTVisitor::visitAtomicExpr(node); }
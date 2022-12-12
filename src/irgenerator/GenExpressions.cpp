// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "IRGenerator.h"

std::any IRGenerator::visitAssignExpr(const AssignExprNode *node) { return ParallelizableASTVisitor::visitAssignExpr(node); }

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
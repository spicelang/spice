// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "IRGenerator.h"

std::any IRGenerator::visitThreadDef(const ThreadDefNode *node) { return ParallelizableASTVisitor::visitThreadDef(node); }

std::any IRGenerator::visitUnsafeBlockDef(const UnsafeBlockDefNode *node) {
  return ParallelizableASTVisitor::visitUnsafeBlockDef(node);
}

std::any IRGenerator::visitForLoop(const ForLoopNode *node) { return ParallelizableASTVisitor::visitForLoop(node); }

std::any IRGenerator::visitForeachLoop(const ForeachLoopNode *node) { return ParallelizableASTVisitor::visitForeachLoop(node); }

std::any IRGenerator::visitWhileLoop(const WhileLoopNode *node) { return ParallelizableASTVisitor::visitWhileLoop(node); }

std::any IRGenerator::visitIfStmt(const IfStmtNode *node) { return ParallelizableASTVisitor::visitIfStmt(node); }

std::any IRGenerator::visitElseStmt(const ElseStmtNode *node) { return ParallelizableASTVisitor::visitElseStmt(node); }

std::any IRGenerator::visitAssertStmt(const AssertStmtNode *node) { return ParallelizableASTVisitor::visitAssertStmt(node); }

std::any IRGenerator::visitAnonymousBlockStmt(const AnonymousBlockStmtNode *node) {
  return ParallelizableASTVisitor::visitAnonymousBlockStmt(node);
}
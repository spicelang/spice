// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>

std::any IRGenerator::visitStmtLst(const StmtLstNode *node) {
  for (const ASTNode *child : node->children) {
    // Check if we can cancel generating instructions for this code branch
    if (blockAlreadyTerminated || node->unreachable)
      return nullptr;
    // Visit child
    visit(child);
  }
  return nullptr;
}

std::any IRGenerator::visitTypeAltsLst(const TypeAltsLstNode *node) {
  return nullptr; // Noop
}

std::any IRGenerator::visitDeclStmt(const DeclStmtNode *node) { return ParallelizableASTVisitor::visitDeclStmt(node); }

std::any IRGenerator::visitSpecifierLst(const SpecifierLstNode *node) {
  return nullptr; // Noop
}

std::any IRGenerator::visitReturnStmt(const ReturnStmtNode *node) { return ParallelizableASTVisitor::visitReturnStmt(node); }

std::any IRGenerator::visitBreakStmt(const BreakStmtNode *node) { return ParallelizableASTVisitor::visitBreakStmt(node); }

std::any IRGenerator::visitContinueStmt(const ContinueStmtNode *node) {
  return ParallelizableASTVisitor::visitContinueStmt(node);
}
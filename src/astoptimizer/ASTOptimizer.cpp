// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "ASTOptimizer.h"

#include <ast/ASTNodes.h>

namespace spice::compiler {

std::any ASTOptimizer::visitUnsafeBlockDef(UnsafeBlockDefNode *node) {
  // Optimize all children first
  visitChildren(node);

  const bool isBodyEmpty = node->body()->children.empty();

  // The body is empty -> remove the whole node
  if (isBodyEmpty) {
    node->removeFromParent();
    return true;
  }

  return false;
}

std::any ASTOptimizer::visitForLoop(ForLoopNode *node) {
  // Optimize all children first
  visitChildren(node);

  const bool isBodyEmpty = node->body()->children.empty();

  // The body is empty -> remove the whole node
  if (isBodyEmpty) {
    node->removeFromParent();
    return true;
  }

  return false;
}

std::any ASTOptimizer::visitForeachLoop(ForeachLoopNode *node) {
  // Optimize all children first
  visitChildren(node);

  const bool isBodyEmpty = node->body()->children.empty();

  // The body is empty -> remove the whole node
  if (isBodyEmpty) {
    node->removeFromParent();
    return true;
  }

  return false;
}

std::any ASTOptimizer::visitWhileLoop(WhileLoopNode *node) {
  // Optimize all children first
  visitChildren(node);

  const bool isBodyEmpty = node->body()->children.empty();

  // The body is empty -> remove the whole node
  if (isBodyEmpty) {
    node->removeFromParent();
    return true;
  }

  return false;
}

std::any ASTOptimizer::visitDoWhileLoop(DoWhileLoopNode *node) {
  // Optimize all children first
  visitChildren(node);

  const bool isBodyEmpty = node->body()->children.empty();

  // The body is empty -> remove the whole node
  if (isBodyEmpty) {
    node->removeFromParent();
    return true;
  }

  return false;
}

std::any ASTOptimizer::visitIfStmt(IfStmtNode *node) {
  // Optimize all children first
  visitChildren(node);

  const bool thenBranchEmpty = node->thenBody()->children.empty();
  const bool elseStmtEmpty = !node->elseStmt();

  // Both are empty -> remove the whole node
  if (thenBranchEmpty && elseStmtEmpty) {
    node->removeFromParent();
    return true;
  }

  // Only the 'then' branch is empty
  if (thenBranchEmpty) {
    // Check if the else branch is an else if
    if (node->elseStmt()->isElseIf) {
      // If it is an else if -> replace this node in its parent with the ifStmt of the else if
      node->replaceInParent(node->elseStmt()->ifStmt());
      return true;
    }
    // Otherwise, do nothing for now
  }

  return false;
}

std::any ASTOptimizer::visitElseStmt(ElseStmtNode *node) {
  // Optimize all children first
  visitChildren(node);

  const bool ifStmtEmpty = node->isElseIf && !node->ifStmt();
  const bool elseBranchEmpty = !node->isElseIf && node->body()->children.empty();

  // This is an else-if and the ifStmt was optimized away -> remove the whole node
  if (ifStmtEmpty) {
    node->removeFromParent();
    return true;
  }

  // The else branch is empty -> remove the whole node
  if (elseBranchEmpty) {
    node->removeFromParent();
    return true;
  }

  return false;
}

std::any ASTOptimizer::visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) {
  // Optimize all children first
  visitChildren(node);

  const bool isBodyEmpty = node->body()->children.empty();

  // The body is empty -> remove the whole node
  if (isBodyEmpty) {
    node->removeFromParent();
    return true;
  }

  return false;
}

} // namespace spice::compiler
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AbstractASTVisitor.h"

#include <ast/ASTNodes.h>

std::any AbstractASTVisitor::visit(ASTNode *node) { return node->accept(this); }

std::any AbstractASTVisitor::visitChildren(ASTNode *node) {
  for (const auto &child : node->children)
    child->accept(this);
  return {};
}
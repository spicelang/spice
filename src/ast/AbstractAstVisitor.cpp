// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AbstractAstVisitor.h"

#include <ast/AstNodes.h>

std::any AbstractAstVisitor::visit(AstNode *node) { return node->accept(this); }

std::any AbstractAstVisitor::visitChildren(AstNode *node) {
  std::any result = std::any();
  for (const auto &child : node->children)
    result = child->accept(this);
  return result;
}
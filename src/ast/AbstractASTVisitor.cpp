// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "AbstractASTVisitor.h"

#include <ast/ASTNodes.h>

namespace spice::compiler {

std::any AbstractASTVisitor::visit(ASTNode *node) { return node->accept(this); }

std::any AbstractASTVisitor::visitChildren(ASTNode *node) {
  for (ASTNode *child : node->children)
    if (child != nullptr)
      child->accept(this);
  return nullptr;
}

} // namespace spice::compiler
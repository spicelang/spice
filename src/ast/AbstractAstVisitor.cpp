// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AbstractAstVisitor.h"

#include <ast/AstNodes.h>

std::any AbstractAstVisitor::visit(AstNode *ctx) {
  return ctx->accept(this);
}

std::any AbstractAstVisitor::visitChildren(const AstNode *ctx) const {
  std::any result = std::any();
  for (const auto child : ctx->children)
    result = child->accept(const_cast<AbstractAstVisitor *>(this));
  return result;
}

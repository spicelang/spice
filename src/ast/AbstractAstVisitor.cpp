// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AbstractAstVisitor.h"

#include <ast/AstNodes.h>

std::any AbstractAstVisitor::visit(AstNode *ctx) {
  return ctx->accept(this);
}

std::any AbstractAstVisitor::visitChildren(AstNode *ctx) {
  std::any result = std::any();
  for (const auto child : ctx->children)
    result = child->accept(this);
  return result;
}

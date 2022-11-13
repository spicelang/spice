// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AbstractASTVisitor.h"

#include <ast/ASTNodes.h>

template <typename T> T AbstractASTVisitor<T>::visit(ASTNode *node) { return node->accept(this); }

template <typename T> T AbstractASTVisitor<T>::visitChildren(ASTNode *node) {
  for (const auto &child : node->children)
    child->accept(this);
  return {};
}
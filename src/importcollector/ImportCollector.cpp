// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ImportCollector.h"

#include <ast/ASTNodes.h>

std::any ImportCollector::visitEntry(EntryNode *node) {
  // Visit all import statements
  for (ImportStmtNode *importStmt : node->importStmts())
    visit(importStmt);

  // Reset the AST
  node->reset();

  return nullptr;
}

std::any ImportCollector::visitImportStmt(ImportStmtNode *node) {

}

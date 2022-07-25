// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AstVisitor.h"

#include <ast/AstNodes.h>

std::any AstVisitor::visitEntry(const EntryNode *ctx) const { return visitChildren(ctx); }

std::any AstVisitor::visitMainFctDef(const MainFctDefNode *ctx) const { return visitChildren(ctx); }

std::any AstVisitor::visitFctDef(const FctDefNode *ctx) const { return visitChildren(ctx); }

std::any AstVisitor::visitProcDef(const ProcDefNode *ctx) const { return visitChildren(ctx); }

std::any AstVisitor::visitStructDef(const StructDefNode *ctx) const { return visitChildren(ctx); }

std::any AstVisitor::visitGenericTypeDef(const GenericTypeDefNode *ctx) const { return visitChildren(ctx); }

std::any AstVisitor::visitGlobalVarDef(const GlobalVarDefNode *ctx) const { return visitChildren(ctx); }

std::any AstVisitor::visitImportStmt(const ImportStmtNode *ctx) const { return visitChildren(ctx); }

std::any AstVisitor::visitExtDecl(const ExtDeclNode *ctx) const { return visitChildren(ctx); }



std::any AstVisitor::visitChildren(const AstNode *ctx) const {
  std::any result = std::any();
  for (const auto child : ctx->children)
    result = child->accept(this);
  return result;
}

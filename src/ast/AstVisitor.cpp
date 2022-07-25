// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AstVisitor.h"

#include <ast/AstNodes.h>

std::any AstVisitor::visitEntry(const EntryNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitMainFctDef(const MainFctDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitFctDef(const FctDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitProcDef(const ProcDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitStructDef(const StructDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitGenericTypeDef(const GenericTypeDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitGlobalVarDef(const GlobalVarDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitImportStmt(const ImportStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitExtDecl(const ExtDeclNode *ctx) { return visitChildren(ctx); }

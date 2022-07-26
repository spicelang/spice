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

std::any AstVisitor::visitExtDecl(const ExtDeclNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitThreadDef(const ThreadDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitUnsafeBlockDef(const UnsafeBlockDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitForLoop(const ForLoopNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitForeachLoop(const ForeachLoopNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitWhileLoop(const WhileLoopNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitIfStmt(const IfStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitAssertStmt(const AssertStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitStmtLst(const StmtLstNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitTypeLst(const TypeLstNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitTypeAltsLst(const TypeAltsLstNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitArgLstDef(const ArgLstDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitArgLst(const ArgLstNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitField(const FieldNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitStmt(const StmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitDeclStmt(const DeclStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitDeclSpecifiers(const DeclSpecifiersNode *ctx) { return visitChildren(ctx); }



std::any AstVisitor::visitImportStmt(const ImportStmtNode *ctx) { return visitChildren(ctx); }
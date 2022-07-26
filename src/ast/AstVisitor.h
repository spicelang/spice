// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <ast/AbstractAstVisitor.h>

class AstVisitor : public AbstractAstVisitor {
public:
  // Visitor methods
  virtual std::any visitEntry(const EntryNode *ctx) override;
  virtual std::any visitMainFctDef(const MainFctDefNode *ctx) override;
  virtual std::any visitFctDef(const FctDefNode *ctx) override;
  virtual std::any visitProcDef(const ProcDefNode *ctx) override;
  virtual std::any visitStructDef(const StructDefNode *ctx) override;
  virtual std::any visitGenericTypeDef(const GenericTypeDefNode *ctx) override;
  virtual std::any visitGlobalVarDef(const GlobalVarDefNode *ctx) override;
  virtual std::any visitExtDecl(const ExtDeclNode *ctx) override;
  virtual std::any visitThreadDef(const ThreadDefNode *ctx) override;
  virtual std::any visitUnsafeBlockDef(const UnsafeBlockDefNode *ctx) override;
  virtual std::any visitForLoop(const ForLoopNode *ctx) override;
  virtual std::any visitForeachLoop(const ForeachLoopNode *ctx) override;
  virtual std::any visitWhileLoop(const WhileLoopNode *ctx) override;
  virtual std::any visitIfStmt(const IfStmtNode *ctx) override;
  virtual std::any visitAssertStmt(const AssertStmtNode *ctx) override;
  virtual std::any visitStmtLst(const StmtLstNode *ctx) override;
  virtual std::any visitTypeLst(const TypeLstNode *ctx) override;
  virtual std::any visitTypeAltsLst(const TypeAltsLstNode *ctx) override;
  virtual std::any visitArgLstDef(const ArgLstDefNode *ctx) override;
  virtual std::any visitArgLst(const ArgLstNode *ctx) override;
  virtual std::any visitField(const FieldNode *ctx) override;
  virtual std::any visitStmt(const StmtNode *ctx) override;
  virtual std::any visitDeclStmt(const DeclStmtNode *ctx) override;
  virtual std::any visitDeclSpecifiers(const DeclSpecifiersNode *ctx) override;

  virtual std::any visitImportStmt(const ImportStmtNode *ctx) override;
};
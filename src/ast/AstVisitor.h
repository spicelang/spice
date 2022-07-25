// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <ast/AbstractAstVisitor.h>

class AstVisitor : public AbstractAstVisitor {
public:
  // Visitor methods
  virtual std::any visitEntry(const EntryNode *ctx) const;
  virtual std::any visitMainFctDef(const MainFctDefNode *ctx) const;
  virtual std::any visitFctDef(const FctDefNode *ctx) const;
  virtual std::any visitProcDef(const ProcDefNode *ctx) const;
  virtual std::any visitStructDef(const StructDefNode *ctx) const;
  virtual std::any visitGenericTypeDef(const GenericTypeDefNode *ctx) const;
  virtual std::any visitGlobalVarDef(const GlobalVarDefNode *ctx) const;
  virtual std::any visitImportStmt(const ImportStmtNode *ctx) const;
  virtual std::any visitExtDecl(const ExtDeclNode *ctx) const;

  // Public methods
  virtual std::any visitChildren(const AstNode *ctx) const;

private:
  // Members
};
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
  virtual std::any visitImportStmt(const ImportStmtNode *ctx) override;
  virtual std::any visitExtDecl(const ExtDeclNode *ctx) override;

};
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <any>

// Forward declarations
class AstNode;
class EntryNode;
class MainFctDefNode;
class FctDefNode;
class ProcDefNode;
class StructDefNode;
class GenericTypeDefNode;
class GlobalVarDefNode;
class ImportStmtNode;
class ExtDeclNode;

class AbstractAstVisitor {
public:
  // Visitor methods
  virtual std::any visitEntry(const EntryNode *ctx) const = 0;
  virtual std::any visitMainFctDef(const MainFctDefNode *ctx) const = 0;
  virtual std::any visitFctDef(const FctDefNode *ctx) const = 0;
  virtual std::any visitProcDef(const ProcDefNode *ctx) const = 0;
  virtual std::any visitStructDef(const StructDefNode *ctx) const = 0;
  virtual std::any visitGenericTypeDef(const GenericTypeDefNode *ctx) const = 0;
  virtual std::any visitGlobalVarDef(const GlobalVarDefNode *ctx) const = 0;
  virtual std::any visitImportStmt(const ImportStmtNode *ctx) const = 0;
  virtual std::any visitExtDecl(const ExtDeclNode *ctx) const = 0;

  // Public methods
  virtual std::any visitChildren(const AstNode *ctx) const = 0;
};
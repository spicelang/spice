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
class ExtDeclNode;
class ThreadDefNode;
class UnsafeBlockDefNode;
class ForLoopNode;
class ForeachLoopNode;
class WhileLoopNode;
class IfStmtNode;
class AssertStmtNode;
class StmtLstNode;
class TypeLstNode;
class TypeAltsLstNode;
class ArgLstDefNode;
class ArgLstNode;
class FieldNode;
class StmtNode;
class DeclStmtNode;
class DeclSpecifiersNode;

class ImportStmtNode;

class AbstractAstVisitor {
public:
  // General visitor method
  virtual std::any visit(AstNode *ctx);

  // Visitor methods
  virtual std::any visitEntry(const EntryNode *ctx) = 0;
  virtual std::any visitMainFctDef(const MainFctDefNode *ctx) = 0;
  virtual std::any visitFctDef(const FctDefNode *ctx) = 0;
  virtual std::any visitProcDef(const ProcDefNode *ctx) = 0;
  virtual std::any visitStructDef(const StructDefNode *ctx) = 0;
  virtual std::any visitGenericTypeDef(const GenericTypeDefNode *ctx) = 0;
  virtual std::any visitGlobalVarDef(const GlobalVarDefNode *ctx) = 0;
  virtual std::any visitExtDecl(const ExtDeclNode *ctx) = 0;
  virtual std::any visitThreadDef(const ThreadDefNode *ctx) = 0;
  virtual std::any visitUnsafeBlockDef(const UnsafeBlockDefNode *ctx) = 0;
  virtual std::any visitForLoop(const ForLoopNode *ctx) = 0;
  virtual std::any visitForeachLoop(const ForeachLoopNode *ctx) = 0;
  virtual std::any visitWhileLoop(const WhileLoopNode *ctx) = 0;
  virtual std::any visitIfStmt(const IfStmtNode *ctx) = 0;
  virtual std::any visitAssertStmt(const AssertStmtNode *ctx) = 0;
  virtual std::any visitStmtLst(const StmtLstNode *ctx) = 0;
  virtual std::any visitTypeLst(const TypeLstNode *ctx) = 0;
  virtual std::any visitTypeAltsLst(const TypeAltsLstNode *ctx) = 0;
  virtual std::any visitArgLstDef(const ArgLstDefNode *ctx) = 0;
  virtual std::any visitArgLst(const ArgLstNode *ctx) = 0;
  virtual std::any visitField(const FieldNode *ctx) = 0;
  virtual std::any visitStmt(const StmtNode *ctx) = 0;
  virtual std::any visitDeclStmt(const DeclStmtNode *ctx) = 0;
  virtual std::any visitDeclSpecifiers(const DeclSpecifiersNode *ctx) = 0;

  virtual std::any visitImportStmt(const ImportStmtNode *ctx) = 0;

  // Public methods
  virtual std::any visitChildren(const AstNode *ctx) const;
};
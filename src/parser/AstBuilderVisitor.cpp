// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AstBuilderVisitor.h"

#include <ast/AstNodes.h>

std::any AstBuilderVisitor::visitEntry(SpiceParser::EntryContext *ctx) {
  auto entryNode = static_cast<EntryNode *>(currentNode);

  // Main function definitions
  for (auto &child : ctx->mainFunctionDef()) {
    visit(child);
    entryNode->createChild<MainFctDefNode>(CodeLoc(child->start));
  }
  // Function definitions
  for (auto &child : ctx->functionDef()) {
    visit(child);
    entryNode->createChild<FctDefNode>(CodeLoc(child->start));
  }
  // Procedure definitions
  for (auto &child : ctx->procedureDef()) {
    visit(child);
    entryNode->createChild<ProcDefNode>(CodeLoc(child->start));
  }
  // Struct definitions
  for (auto &child : ctx->structDef()) {
    visit(child);
    entryNode->createChild<StructDefNode>(CodeLoc(child->start));
  }
  // Generic type definitions
  for (auto &child : ctx->genericTypeDef()) {
    visit(child);
    entryNode->createChild<GenericTypeDefNode>(CodeLoc(child->start));
  }
  // Global var definition
  for (auto &child : ctx->globalVarDef()) {
    visit(child);
    entryNode->createChild<GlobalVarDefNode>(CodeLoc(child->start));
  }
  // Import statements
  for (auto &child : ctx->importStmt()) {
    visit(child);
    entryNode->createChild<ImportStmtNode>(CodeLoc(child->start));
  }
  // External declarations
  for (auto &child : ctx->extDecl()) {
    visit(child);
    entryNode->createChild<ExtDeclNode>(CodeLoc(child->start));
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitExtDecl(SpiceParser::ExtDeclContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitStructDef(SpiceParser::StructDefContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitThreadDef(SpiceParser::ThreadDefContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitUnsafeBlockDef(SpiceParser::UnsafeBlockDefContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitForLoop(SpiceParser::ForLoopContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitForHead(SpiceParser::ForHeadContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitForeachHead(SpiceParser::ForeachHeadContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitIfStmt(SpiceParser::IfStmtContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitElseStmt(SpiceParser::ElseStmtContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitAssertStmt(SpiceParser::AssertStmtContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitStmtLst(SpiceParser::StmtLstContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitField(SpiceParser::FieldContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitTypeLst(SpiceParser::TypeLstContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitTypeLstEllipsis(SpiceParser::TypeLstEllipsisContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitTypeAlts(SpiceParser::TypeAltsContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitArgLstDef(SpiceParser::ArgLstDefContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitArgLst(SpiceParser::ArgLstContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitStmt(SpiceParser::StmtContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitDeclSpecifiers(SpiceParser::DeclSpecifiersContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitDeclSpecifier(SpiceParser::DeclSpecifierContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitBreakStmt(SpiceParser::BreakStmtContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitPrintfCall(SpiceParser::PrintfCallContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitLenCall(SpiceParser::LenCallContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitTidCall(SpiceParser::TidCallContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitJoinCall(SpiceParser::JoinCallContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitAssignExpr(SpiceParser::AssignExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitShiftExpr(SpiceParser::ShiftExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitCastExpr(SpiceParser::CastExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitValue(SpiceParser::ValueContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitFunctionCall(SpiceParser::FunctionCallContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitPrimitiveValue(SpiceParser::PrimitiveValueContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitDataType(SpiceParser::DataTypeContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitAssignOp(SpiceParser::AssignOpContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitPrefixUnaryOp(SpiceParser::PrefixUnaryOpContext *ctx) { return nullptr; }

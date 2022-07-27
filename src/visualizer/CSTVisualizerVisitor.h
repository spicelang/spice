// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <SpiceLexer.h>
#include <SpiceVisitor.h>

/**
 * Visitor for debug purposes (is only executed in the compiler debug mode)
 *
 * Jobs:
 * - Visualize CST
 */
class CSTVisualizerVisitor : public SpiceVisitor {
public:
  // Constructors
  explicit CSTVisualizerVisitor(const std::shared_ptr<SpiceLexer> &lexer, const std::shared_ptr<SpiceParser> &parser)
      : vocabulary(lexer->getVocabulary()), ruleNames(parser->getRuleNames()){};

  // Visitor methods
  std::any visitEntry(SpiceParser::EntryContext *ctx) override { return buildRule(ctx); };
  std::any visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) override { return buildRule(ctx); };
  std::any visitFunctionDef(SpiceParser::FunctionDefContext *ctx) override { return buildRule(ctx); };
  std::any visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) override { return buildRule(ctx); };
  std::any visitExtDecl(SpiceParser::ExtDeclContext *ctx) override { return buildRule(ctx); };
  std::any visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) override { return buildRule(ctx); };
  std::any visitStructDef(SpiceParser::StructDefContext *ctx) override { return buildRule(ctx); };
  std::any visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) override { return buildRule(ctx); };
  std::any visitThreadDef(SpiceParser::ThreadDefContext *ctx) override { return buildRule(ctx); };
  std::any visitUnsafeBlockDef(SpiceParser::UnsafeBlockDefContext *ctx) override { return buildRule(ctx); };
  std::any visitForLoop(SpiceParser::ForLoopContext *ctx) override { return buildRule(ctx); };
  std::any visitForHead(SpiceParser::ForHeadContext *ctx) override { return buildRule(ctx); };
  std::any visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) override { return buildRule(ctx); };
  std::any visitForeachHead(SpiceParser::ForeachHeadContext *ctx) override { return buildRule(ctx); };
  std::any visitWhileLoop(SpiceParser::WhileLoopContext *ctx) override { return buildRule(ctx); };
  std::any visitIfStmt(SpiceParser::IfStmtContext *ctx) override { return buildRule(ctx); };
  std::any visitElseStmt(SpiceParser::ElseStmtContext *ctx) override { return buildRule(ctx); };
  std::any visitAssertStmt(SpiceParser::AssertStmtContext *ctx) override { return buildRule(ctx); };
  std::any visitStmtLst(SpiceParser::StmtLstContext *ctx) override { return buildRule(ctx); };
  std::any visitField(SpiceParser::FieldContext *ctx) override { return buildRule(ctx); };
  std::any visitTypeLst(SpiceParser::TypeLstContext *ctx) override { return buildRule(ctx); };
  std::any visitTypeAltsLst(SpiceParser::TypeAltsLstContext *ctx) override { return buildRule(ctx); };
  std::any visitArgLstDef(SpiceParser::ArgLstDefContext *ctx) override { return buildRule(ctx); };
  std::any visitArgLst(SpiceParser::ArgLstContext *ctx) override { return buildRule(ctx); };
  std::any visitStmt(SpiceParser::StmtContext *ctx) override { return buildRule(ctx); };
  std::any visitDeclStmt(SpiceParser::DeclStmtContext *ctx) override { return buildRule(ctx); };
  std::any visitDeclSpecifiers(SpiceParser::DeclSpecifiersContext *ctx) override { return buildRule(ctx); };
  std::any visitDeclSpecifier(SpiceParser::DeclSpecifierContext *ctx) override { return buildRule(ctx); };
  std::any visitImportStmt(SpiceParser::ImportStmtContext *ctx) override { return buildRule(ctx); };
  std::any visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) override { return buildRule(ctx); };
  std::any visitBreakStmt(SpiceParser::BreakStmtContext *ctx) override { return buildRule(ctx); };
  std::any visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) override { return buildRule(ctx); };
  std::any visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) override { return buildRule(ctx); };
  std::any visitPrintfCall(SpiceParser::PrintfCallContext *ctx) override { return buildRule(ctx); };
  std::any visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) override { return buildRule(ctx); };
  std::any visitLenCall(SpiceParser::LenCallContext *ctx) override { return buildRule(ctx); };
  std::any visitTidCall(SpiceParser::TidCallContext *ctx) override { return buildRule(ctx); };
  std::any visitJoinCall(SpiceParser::JoinCallContext *ctx) override { return buildRule(ctx); };
  std::any visitAssignExpr(SpiceParser::AssignExprContext *ctx) override { return buildRule(ctx); };
  std::any visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) override { return buildRule(ctx); };
  std::any visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) override { return buildRule(ctx); };
  std::any visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) override { return buildRule(ctx); };
  std::any visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) override { return buildRule(ctx); };
  std::any visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) override { return buildRule(ctx); };
  std::any visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) override { return buildRule(ctx); };
  std::any visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) override { return buildRule(ctx); };
  std::any visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) override { return buildRule(ctx); };
  std::any visitShiftExpr(SpiceParser::ShiftExprContext *ctx) override { return buildRule(ctx); };
  std::any visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) override { return buildRule(ctx); };
  std::any visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) override { return buildRule(ctx); };
  std::any visitCastExpr(SpiceParser::CastExprContext *ctx) override { return buildRule(ctx); };
  std::any visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) override { return buildRule(ctx); };
  std::any visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) override { return buildRule(ctx); };
  std::any visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) override { return buildRule(ctx); };
  std::any visitValue(SpiceParser::ValueContext *ctx) override { return buildRule(ctx); };
  std::any visitFunctionCall(SpiceParser::FunctionCallContext *ctx) override { return buildRule(ctx); };
  std::any visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) override { return buildRule(ctx); };
  std::any visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) override { return buildRule(ctx); };
  std::any visitPrimitiveValue(SpiceParser::PrimitiveValueContext *ctx) override { return buildRule(ctx); };
  std::any visitDataType(SpiceParser::DataTypeContext *ctx) override { return buildRule(ctx); };
  std::any visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) override { return buildRule(ctx); };
  std::any visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) override { return buildRule(ctx); };
  std::any visitAssignOp(SpiceParser::AssignOpContext *ctx) override { return buildRule(ctx); };
  std::any visitPrefixUnaryOp(SpiceParser::PrefixUnaryOpContext *ctx) override { return buildRule(ctx); };

private:
  // Members
  const antlr4::dfa::Vocabulary &vocabulary;
  const std::vector<std::string> ruleNames;
  int currentTabs = 1;
  std::string parentNodeId;

  // Private methods
  std::string buildRule(antlr4::ParserRuleContext *ctx);
  [[nodiscard]] std::string getSpaces() const;
};
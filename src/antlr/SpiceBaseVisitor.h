
// Generated from D:/Dokumente/JustForFun GitHub Clones/spice/src\Spice.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"
#include "SpiceVisitor.h"


/**
 * This class provides an empty implementation of SpiceVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  SpiceBaseVisitor : public SpiceVisitor {
public:

  virtual antlrcpp::Any visitEntry(SpiceParser::EntryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunctionDef(SpiceParser::FunctionDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitForLoop(SpiceParser::ForLoopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitWhileLoop(SpiceParser::WhileLoopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIfStmt(SpiceParser::IfStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStmtLst(SpiceParser::StmtLstContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParamLst(SpiceParser::ParamLstContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStmt(SpiceParser::StmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDeclStmt(SpiceParser::DeclStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunctionCall(SpiceParser::FunctionCallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitImportStmt(SpiceParser::ImportStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTopLvlExpr(SpiceParser::TopLvlExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignment(SpiceParser::AssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTernary(SpiceParser::TernaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEquality(SpiceParser::EqualityContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPrefixUnary(SpiceParser::PrefixUnaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPostfixUnary(SpiceParser::PostfixUnaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValue(SpiceParser::ValueContext *ctx) override {
    return visitChildren(ctx);
  }


};


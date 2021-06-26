
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

  virtual antlrcpp::Any visitFunction_def(SpiceParser::Function_defContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitProcedure_def(SpiceParser::Procedure_defContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFor_loop(SpiceParser::For_loopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFor_each_loop(SpiceParser::For_each_loopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitWhile_loop(SpiceParser::While_loopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIf_stmt(SpiceParser::If_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStmt_lst(SpiceParser::Stmt_lstContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParam_lst(SpiceParser::Param_lstContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStmt(SpiceParser::StmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDecl_stmt(SpiceParser::Decl_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunction_call(SpiceParser::Function_callContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitImport_stmt(SpiceParser::Import_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturn_stmt(SpiceParser::Return_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTop_lvl_expr(SpiceParser::Top_lvl_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignment(SpiceParser::AssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTernary(SpiceParser::TernaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLogical_or_expr(SpiceParser::Logical_or_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLogical_and_expr(SpiceParser::Logical_and_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBitwise_or_expr(SpiceParser::Bitwise_or_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBitwise_and_expr(SpiceParser::Bitwise_and_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEquality(SpiceParser::EqualityContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRelational_expr(SpiceParser::Relational_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAdditive_expr(SpiceParser::Additive_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMultiplicative_expr(SpiceParser::Multiplicative_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPrefix_unary(SpiceParser::Prefix_unaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPostfix_unary(SpiceParser::Postfix_unaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAtomic_expr(SpiceParser::Atomic_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValue(SpiceParser::ValueContext *ctx) override {
    return visitChildren(ctx);
  }


};


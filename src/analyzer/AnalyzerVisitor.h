// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include "../antlr/SpiceBaseVisitor.h"
#include "SymbolTable.h"

class AnalyzerVisitor : public SpiceBaseVisitor {
public:
    // Constructor
    AnalyzerVisitor();

    // Public methods
    antlrcpp::Any visitFunctionDef(SpiceParser::FunctionDefContext *ctx) override;
    antlrcpp::Any visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) override;
    antlrcpp::Any visitForLoop(SpiceParser::ForLoopContext *ctx) override;
    antlrcpp::Any visitWhileLoop(SpiceParser::WhileLoopContext *ctx) override;
    antlrcpp::Any visitIfStmt(SpiceParser::IfStmtContext *ctx) override;
    antlrcpp::Any visitParamLst(SpiceParser::ParamLstContext *ctx) override;
    antlrcpp::Any visitDeclStmt(SpiceParser::DeclStmtContext *ctx) override;
    antlrcpp::Any visitFunctionCall(SpiceParser::FunctionCallContext *ctx) override;
    antlrcpp::Any visitImportStmt(SpiceParser::ImportStmtContext *ctx) override;
    antlrcpp::Any visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) override;
    antlrcpp::Any visitAssignment(SpiceParser::AssignmentContext *ctx) override;
    antlrcpp::Any visitTernary(SpiceParser::TernaryContext *ctx) override;
    antlrcpp::Any visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) override;
    antlrcpp::Any visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) override;
    antlrcpp::Any visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) override;
    antlrcpp::Any visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) override;
    antlrcpp::Any visitEquality(SpiceParser::EqualityContext *ctx) override;
    antlrcpp::Any visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) override;
    antlrcpp::Any visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) override;
    antlrcpp::Any visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) override;
    antlrcpp::Any visitPrefixUnary(SpiceParser::PrefixUnaryContext *ctx) override;
    antlrcpp::Any visitPostfixUnary(SpiceParser::PostfixUnaryContext *ctx) override;
    antlrcpp::Any visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) override;
private:
    // Members
    SymbolTable symbolTable;
};
// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "AnalyzerVisitor.h"

AnalyzerVisitor::AnalyzerVisitor() {
    //symbolTable = SymbolTable();
}

antlrcpp::Any AnalyzerVisitor::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
    std::cout << ctx->IDENTIFIER()->toString() << std::endl;
    symbolTable.insertSymbol(ctx->IDENTIFIER()->toString(), "global", SymbolType::FUNCTION);
    return SpiceBaseVisitor::visitFunctionDef(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
    return SpiceBaseVisitor::visitProcedureDef(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitForLoop(SpiceParser::ForLoopContext *ctx) {
    return SpiceBaseVisitor::visitForLoop(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
    return SpiceBaseVisitor::visitWhileLoop(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
    return SpiceBaseVisitor::visitIfStmt(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitParamLst(SpiceParser::ParamLstContext *ctx) {
    return SpiceBaseVisitor::visitParamLst(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
    return SpiceBaseVisitor::visitDeclStmt(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitFunctionCall(SpiceParser::FunctionCallContext *ctx) {
    return SpiceBaseVisitor::visitFunctionCall(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {
    return SpiceBaseVisitor::visitImportStmt(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
    return SpiceBaseVisitor::visitReturnStmt(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitAssignment(SpiceParser::AssignmentContext *ctx) {
    return SpiceBaseVisitor::visitAssignment(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitTernary(SpiceParser::TernaryContext *ctx) {
    return SpiceBaseVisitor::visitTernary(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
    return SpiceBaseVisitor::visitLogicalOrExpr(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
    return SpiceBaseVisitor::visitLogicalAndExpr(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
    return SpiceBaseVisitor::visitBitwiseOrExpr(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
    return SpiceBaseVisitor::visitBitwiseAndExpr(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitEquality(SpiceParser::EqualityContext *ctx) {
    return SpiceBaseVisitor::visitEquality(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {
    return SpiceBaseVisitor::visitRelationalExpr(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {
    return SpiceBaseVisitor::visitAdditiveExpr(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
    return SpiceBaseVisitor::visitMultiplicativeExpr(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitPrefixUnary(SpiceParser::PrefixUnaryContext *ctx) {
    return SpiceBaseVisitor::visitPrefixUnary(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitPostfixUnary(SpiceParser::PostfixUnaryContext *ctx) {
    return SpiceBaseVisitor::visitPostfixUnary(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
    return SpiceBaseVisitor::visitAtomicExpr(ctx);
}

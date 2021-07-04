// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "GeneratorVisitor.h"

antlrcpp::Any GeneratorVisitor::visitEntry(SpiceParser::EntryContext *ctx) {
    return SpiceBaseVisitor::visitEntry(ctx);
}

antlrcpp::Any GeneratorVisitor::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
    return SpiceBaseVisitor::visitFunctionDef(ctx);
}

antlrcpp::Any GeneratorVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
    return SpiceBaseVisitor::visitProcedureDef(ctx);
}

antlrcpp::Any GeneratorVisitor::visitForLoop(SpiceParser::ForLoopContext *ctx) {
    return SpiceBaseVisitor::visitForLoop(ctx);
}

antlrcpp::Any GeneratorVisitor::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
    return SpiceBaseVisitor::visitWhileLoop(ctx);
}

antlrcpp::Any GeneratorVisitor::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
    return SpiceBaseVisitor::visitIfStmt(ctx);
}

antlrcpp::Any GeneratorVisitor::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
    return SpiceBaseVisitor::visitDeclStmt(ctx);
}

antlrcpp::Any GeneratorVisitor::visitFunctionCall(SpiceParser::FunctionCallContext *ctx) {
    return SpiceBaseVisitor::visitFunctionCall(ctx);
}

antlrcpp::Any GeneratorVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {
    return SpiceBaseVisitor::visitImportStmt(ctx);
}

antlrcpp::Any GeneratorVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
    return SpiceBaseVisitor::visitReturnStmt(ctx);
}

antlrcpp::Any GeneratorVisitor::visitAssignment(SpiceParser::AssignmentContext *ctx) {
    return SpiceBaseVisitor::visitAssignment(ctx);
}

antlrcpp::Any GeneratorVisitor::visitTernary(SpiceParser::TernaryContext *ctx) {
    return SpiceBaseVisitor::visitTernary(ctx);
}

antlrcpp::Any GeneratorVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
    return SpiceBaseVisitor::visitLogicalOrExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
    return SpiceBaseVisitor::visitLogicalAndExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
    return SpiceBaseVisitor::visitBitwiseOrExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
    return SpiceBaseVisitor::visitBitwiseAndExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) {
    return SpiceBaseVisitor::visitEqualityExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {
    return SpiceBaseVisitor::visitRelationalExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {
    return SpiceBaseVisitor::visitAdditiveExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
    return SpiceBaseVisitor::visitMultiplicativeExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitPrefixUnary(SpiceParser::PrefixUnaryContext *ctx) {
    return SpiceBaseVisitor::visitPrefixUnary(ctx);
}

antlrcpp::Any GeneratorVisitor::visitPostfixUnary(SpiceParser::PostfixUnaryContext *ctx) {
    return SpiceBaseVisitor::visitPostfixUnary(ctx);
}

antlrcpp::Any GeneratorVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
    return SpiceBaseVisitor::visitAtomicExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitValue(SpiceParser::ValueContext *ctx) {
    return SpiceBaseVisitor::visitValue(ctx);
}

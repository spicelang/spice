// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "AnalyzerVisitor.h"

antlrcpp::Any AnalyzerVisitor::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
    // Insert function name into the root symbol table
    currentScope->insert(ctx->IDENTIFIER()->toString(), TYPE_FUNCTION, INITIALIZED);
    // Create a new scope
    std::string scopeId = "f:" + std::to_string(ctx->F()->getSymbol()->getLine()) + ":" +
                          std::to_string(ctx->F()->getSymbol()->getCharPositionInLine());
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit children in new scope
    antlrcpp::Any childrenResult = SpiceBaseVisitor::visitFunctionDef(ctx);
    // Return to old scope
    currentScope = currentScope->getParent();
    std::cout << currentScope->toString() << std::endl; // ToDo: This line is only for testing, remove it in the future
    return childrenResult;
}

antlrcpp::Any AnalyzerVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
    // Insert procedure name into the root symbol table
    currentScope->insert(ctx->IDENTIFIER()->toString(), TYPE_PROCEDURE, INITIALIZED);
    // Create a new scope
    std::string scopeId = "p:" + std::to_string(ctx->P()->getSymbol()->getLine()) + ":" +
                          std::to_string(ctx->P()->getSymbol()->getCharPositionInLine());
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit children in new scope
    antlrcpp::Any childrenResult = SpiceBaseVisitor::visitProcedureDef(ctx);
    // Return to old scope
    currentScope = currentScope->getParent();
    return childrenResult;
}

antlrcpp::Any AnalyzerVisitor::visitForLoop(SpiceParser::ForLoopContext *ctx) {
    // Create a new scope
    std::string scopeId = "for:" + std::to_string(ctx->FOR()->getSymbol()->getLine()) + ":" +
            std::to_string(ctx->FOR()->getSymbol()->getCharPositionInLine());
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit children in new scope
    antlrcpp::Any childrenResult = SpiceBaseVisitor::visitForLoop(ctx);
    // Return to old scope
    currentScope = currentScope->getParent();
    return childrenResult;
}

antlrcpp::Any AnalyzerVisitor::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
    // Create a new scope
    std::string scopeId = "while:" + std::to_string(ctx->WHILE()->getSymbol()->getLine()) + ":" +
                          std::to_string(ctx->WHILE()->getSymbol()->getCharPositionInLine());
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit children in new scope
    antlrcpp::Any childrenResult = SpiceBaseVisitor::visitWhileLoop(ctx);
    // Return to old scope
    currentScope = currentScope->getParent();
    return childrenResult;
}

antlrcpp::Any AnalyzerVisitor::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
    std::string scopeId = "if:" + std::to_string(ctx->IF()->getSymbol()->getLine()) + ":" +
                          std::to_string(ctx->IF()->getSymbol()->getCharPositionInLine());
    // Create a new scope
    currentScope = currentScope->createChildBlock(scopeId);
    // Visit children in new scope
    antlrcpp::Any childrenResult = SpiceBaseVisitor::visitIfStmt(ctx);
    // Return to old scope
    currentScope = currentScope->getParent();
    return childrenResult;
}

antlrcpp::Any AnalyzerVisitor::visitParamLst(SpiceParser::ParamLstContext *ctx) {
    return SpiceBaseVisitor::visitParamLst(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
    // Insert variable name to symbol table
    SymbolType type;
    if (ctx->dataType()->TYPE_DOUBLE() != nullptr) type = TYPE_DOUBLE;
    if (ctx->dataType()->TYPE_INT() != nullptr) type = TYPE_INT;
    if (ctx->dataType()->TYPE_STRING() != nullptr) type = TYPE_STRING;
    if (ctx->dataType()->TYPE_BOOL() != nullptr) type = TYPE_BOOL;
    if (ctx->dataType()->TYPE_DYN() != nullptr) type = TYPE_DYN;
    currentScope->insert(ctx->IDENTIFIER()->toString(), type, DECLARED);
    return SpiceBaseVisitor::visitDeclStmt(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitFunctionCall(SpiceParser::FunctionCallContext *ctx) {
    std::string functionName = ctx->IDENTIFIER()->toString();
    // Check if function call exists in symbol table
    if (!currentScope->lookup(functionName))
        throw SemanticError(REFERENCED_UNDEFINED_FUNCTION,
                            "Function " + functionName + " was called before initialized.");
    return SpiceBaseVisitor::visitFunctionCall(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {
    // Check if imported library exists
    std::cout << "Reached import statement" << std::endl;
    return SpiceBaseVisitor::visitImportStmt(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
    return SpiceBaseVisitor::visitReturnStmt(ctx);
}

antlrcpp::Any AnalyzerVisitor::visitAssignment(SpiceParser::AssignmentContext *ctx) {
    /*ctx->declStmt()->
    std::string variableName = ctx->IDENTIFIER()->toString();
    // Check if function call exists in symbol table
    if (!currentScope->lookup(variableName))
        throw SemanticError(REFERENCED_UNDEFINED_FUNCTION,
                            "Function " + variableName + " was called before initialized.");*/
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

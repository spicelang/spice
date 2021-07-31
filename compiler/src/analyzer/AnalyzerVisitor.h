// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include "SpiceBaseVisitor.h"
#include "SpiceLexer.h"
#include "SymbolTable.h"
#include <util/ScopeIdUtil.h>
#include <exception/SemanticError.h>

const std::string RETURN_VARIABLE_NAME = "result";

class AnalyzerVisitor : public SpiceBaseVisitor {
public:
    // Public methods
    antlrcpp::Any visitEntry(SpiceParser::EntryContext* ctx) override;
    antlrcpp::Any visitMainFunctionDef(SpiceParser::MainFunctionDefContext* ctx) override;
    antlrcpp::Any visitFunctionDef(SpiceParser::FunctionDefContext* ctx) override;
    antlrcpp::Any visitProcedureDef(SpiceParser::ProcedureDefContext* ctx) override;
    antlrcpp::Any visitForLoop(SpiceParser::ForLoopContext* ctx) override;
    /*antlrcpp::Any visitForeachLoop(SpiceParser::ForeachLoopContext* ctx) override;*/
    antlrcpp::Any visitWhileLoop(SpiceParser::WhileLoopContext* ctx) override;
    antlrcpp::Any visitIfStmt(SpiceParser::IfStmtContext* ctx) override;
    antlrcpp::Any visitParamLstDef(SpiceParser::ParamLstDefContext *ctx) override;
    antlrcpp::Any visitDeclStmt(SpiceParser::DeclStmtContext* ctx) override;
    antlrcpp::Any visitFunctionCall(SpiceParser::FunctionCallContext* ctx) override;
    antlrcpp::Any visitImportStmt(SpiceParser::ImportStmtContext* ctx) override;
    antlrcpp::Any visitReturnStmt(SpiceParser::ReturnStmtContext* ctx) override;
    antlrcpp::Any visitPrintfStmt(SpiceParser::PrintfStmtContext* ctx) override;
    antlrcpp::Any visitAssignment(SpiceParser::AssignmentContext* ctx) override;
    antlrcpp::Any visitTernary(SpiceParser::TernaryContext* ctx) override;
    antlrcpp::Any visitLogicalOrExpr(SpiceParser::LogicalOrExprContext* ctx) override;
    antlrcpp::Any visitLogicalAndExpr(SpiceParser::LogicalAndExprContext* ctx) override;
    antlrcpp::Any visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext* ctx) override;
    antlrcpp::Any visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext* ctx) override;
    antlrcpp::Any visitEqualityExpr(SpiceParser::EqualityExprContext* ctx) override;
    antlrcpp::Any visitRelationalExpr(SpiceParser::RelationalExprContext* ctx) override;
    antlrcpp::Any visitAdditiveExpr(SpiceParser::AdditiveExprContext* ctx) override;
    antlrcpp::Any visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext* ctx) override;
    antlrcpp::Any visitPrefixUnary(SpiceParser::PrefixUnaryContext* ctx) override;
    antlrcpp::Any visitPostfixUnary(SpiceParser::PostfixUnaryContext* ctx) override;
    antlrcpp::Any visitAtomicExpr(SpiceParser::AtomicExprContext* ctx) override;
    antlrcpp::Any visitValue(SpiceParser::ValueContext* ctx) override;
private:
    // Members
    SymbolTable* currentScope = new SymbolTable(nullptr);
    bool parameterMode = false;
    bool hasMainFunction = false;

    // Private functions
    static SymbolType getSymbolTypeFromDataType(SpiceParser::DataTypeContext*);
};
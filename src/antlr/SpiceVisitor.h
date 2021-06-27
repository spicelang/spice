
// Generated from D:/Dokumente/JustForFun GitHub Clones/spice/src\Spice.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"
#include "SpiceParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by SpiceParser.
 */
class  SpiceVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by SpiceParser.
   */
    virtual antlrcpp::Any visitEntry(SpiceParser::EntryContext *context) = 0;

    virtual antlrcpp::Any visitFunctionDef(SpiceParser::FunctionDefContext *context) = 0;

    virtual antlrcpp::Any visitProcedureDef(SpiceParser::ProcedureDefContext *context) = 0;

    virtual antlrcpp::Any visitForLoop(SpiceParser::ForLoopContext *context) = 0;

    virtual antlrcpp::Any visitForeachLoop(SpiceParser::ForeachLoopContext *context) = 0;

    virtual antlrcpp::Any visitWhileLoop(SpiceParser::WhileLoopContext *context) = 0;

    virtual antlrcpp::Any visitIfStmt(SpiceParser::IfStmtContext *context) = 0;

    virtual antlrcpp::Any visitStmtLst(SpiceParser::StmtLstContext *context) = 0;

    virtual antlrcpp::Any visitParamLst(SpiceParser::ParamLstContext *context) = 0;

    virtual antlrcpp::Any visitStmt(SpiceParser::StmtContext *context) = 0;

    virtual antlrcpp::Any visitDeclStmt(SpiceParser::DeclStmtContext *context) = 0;

    virtual antlrcpp::Any visitFunctionCall(SpiceParser::FunctionCallContext *context) = 0;

    virtual antlrcpp::Any visitImportStmt(SpiceParser::ImportStmtContext *context) = 0;

    virtual antlrcpp::Any visitReturnStmt(SpiceParser::ReturnStmtContext *context) = 0;

    virtual antlrcpp::Any visitTopLvlExpr(SpiceParser::TopLvlExprContext *context) = 0;

    virtual antlrcpp::Any visitAssignment(SpiceParser::AssignmentContext *context) = 0;

    virtual antlrcpp::Any visitTernary(SpiceParser::TernaryContext *context) = 0;

    virtual antlrcpp::Any visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *context) = 0;

    virtual antlrcpp::Any visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *context) = 0;

    virtual antlrcpp::Any visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *context) = 0;

    virtual antlrcpp::Any visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *context) = 0;

    virtual antlrcpp::Any visitEquality(SpiceParser::EqualityContext *context) = 0;

    virtual antlrcpp::Any visitRelationalExpr(SpiceParser::RelationalExprContext *context) = 0;

    virtual antlrcpp::Any visitAdditiveExpr(SpiceParser::AdditiveExprContext *context) = 0;

    virtual antlrcpp::Any visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *context) = 0;

    virtual antlrcpp::Any visitPrefixUnary(SpiceParser::PrefixUnaryContext *context) = 0;

    virtual antlrcpp::Any visitPostfixUnary(SpiceParser::PostfixUnaryContext *context) = 0;

    virtual antlrcpp::Any visitAtomicExpr(SpiceParser::AtomicExprContext *context) = 0;

    virtual antlrcpp::Any visitValue(SpiceParser::ValueContext *context) = 0;


};



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

    virtual antlrcpp::Any visitFunction_def(SpiceParser::Function_defContext *context) = 0;

    virtual antlrcpp::Any visitProcedure_def(SpiceParser::Procedure_defContext *context) = 0;

    virtual antlrcpp::Any visitFor_loop(SpiceParser::For_loopContext *context) = 0;

    virtual antlrcpp::Any visitFor_each_loop(SpiceParser::For_each_loopContext *context) = 0;

    virtual antlrcpp::Any visitWhile_loop(SpiceParser::While_loopContext *context) = 0;

    virtual antlrcpp::Any visitIf_stmt(SpiceParser::If_stmtContext *context) = 0;

    virtual antlrcpp::Any visitStmt_lst(SpiceParser::Stmt_lstContext *context) = 0;

    virtual antlrcpp::Any visitParam_lst(SpiceParser::Param_lstContext *context) = 0;

    virtual antlrcpp::Any visitStmt(SpiceParser::StmtContext *context) = 0;

    virtual antlrcpp::Any visitDecl_stmt(SpiceParser::Decl_stmtContext *context) = 0;

    virtual antlrcpp::Any visitFunction_call(SpiceParser::Function_callContext *context) = 0;

    virtual antlrcpp::Any visitImport_stmt(SpiceParser::Import_stmtContext *context) = 0;

    virtual antlrcpp::Any visitReturn_stmt(SpiceParser::Return_stmtContext *context) = 0;

    virtual antlrcpp::Any visitTop_lvl_expr(SpiceParser::Top_lvl_exprContext *context) = 0;

    virtual antlrcpp::Any visitAssignment(SpiceParser::AssignmentContext *context) = 0;

    virtual antlrcpp::Any visitTernary(SpiceParser::TernaryContext *context) = 0;

    virtual antlrcpp::Any visitLogical_or_expr(SpiceParser::Logical_or_exprContext *context) = 0;

    virtual antlrcpp::Any visitLogical_and_expr(SpiceParser::Logical_and_exprContext *context) = 0;

    virtual antlrcpp::Any visitBitwise_or_expr(SpiceParser::Bitwise_or_exprContext *context) = 0;

    virtual antlrcpp::Any visitBitwise_and_expr(SpiceParser::Bitwise_and_exprContext *context) = 0;

    virtual antlrcpp::Any visitEquality(SpiceParser::EqualityContext *context) = 0;

    virtual antlrcpp::Any visitRelational_expr(SpiceParser::Relational_exprContext *context) = 0;

    virtual antlrcpp::Any visitAdditive_expr(SpiceParser::Additive_exprContext *context) = 0;

    virtual antlrcpp::Any visitMultiplicative_expr(SpiceParser::Multiplicative_exprContext *context) = 0;

    virtual antlrcpp::Any visitPrefix_unary(SpiceParser::Prefix_unaryContext *context) = 0;

    virtual antlrcpp::Any visitPostfix_unary(SpiceParser::Postfix_unaryContext *context) = 0;

    virtual antlrcpp::Any visitAtomic_expr(SpiceParser::Atomic_exprContext *context) = 0;

    virtual antlrcpp::Any visitValue(SpiceParser::ValueContext *context) = 0;


};


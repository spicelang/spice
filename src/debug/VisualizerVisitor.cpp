// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "VisualizerVisitor.h"

std::any VisualizerVisitor::visitEntry(SpiceParser::EntryContext *ctx) { return std::string("subgraph { <<contents>> }"); }

std::any VisualizerVisitor::visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) { return "subgraph { <<stmts>> }"; }

std::any VisualizerVisitor::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) { return ""; }

std::any VisualizerVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) { return ""; }

std::any VisualizerVisitor::visitExtDecl(SpiceParser::ExtDeclContext *ctx) { return ""; }

std::any VisualizerVisitor::visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) { return ""; }

std::any VisualizerVisitor::visitStructDef(SpiceParser::StructDefContext *ctx) { return ""; }

std::any VisualizerVisitor::visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) { return ""; }

std::any VisualizerVisitor::visitThreadDef(SpiceParser::ThreadDefContext *ctx) { return ""; }

std::any VisualizerVisitor::visitUnsafeBlockDef(SpiceParser::UnsafeBlockDefContext *ctx) { return ""; }

std::any VisualizerVisitor::visitForLoop(SpiceParser::ForLoopContext *ctx) { return ""; }

std::any VisualizerVisitor::visitForHead(SpiceParser::ForHeadContext *ctx) { return ""; }

std::any VisualizerVisitor::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) { return ""; }

std::any VisualizerVisitor::visitForeachHead(SpiceParser::ForeachHeadContext *ctx) { return ""; }

std::any VisualizerVisitor::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) { return ""; }

std::any VisualizerVisitor::visitIfStmt(SpiceParser::IfStmtContext *ctx) { return ""; }

std::any VisualizerVisitor::visitElseStmt(SpiceParser::ElseStmtContext *ctx) { return ""; }

std::any VisualizerVisitor::visitAssertStmt(SpiceParser::AssertStmtContext *ctx) { return ""; }

std::any VisualizerVisitor::visitStmtLst(SpiceParser::StmtLstContext *ctx) { return ""; }

std::any VisualizerVisitor::visitField(SpiceParser::FieldContext *ctx) { return ""; }

std::any VisualizerVisitor::visitTypeLst(SpiceParser::TypeLstContext *ctx) { return ""; }

std::any VisualizerVisitor::visitTypeLstEllipsis(SpiceParser::TypeLstEllipsisContext *ctx) { return ""; }

std::any VisualizerVisitor::visitTypeAlts(SpiceParser::TypeAltsContext *ctx) { return ""; }

std::any VisualizerVisitor::visitArgLstDef(SpiceParser::ArgLstDefContext *ctx) { return ""; }

std::any VisualizerVisitor::visitArgLst(SpiceParser::ArgLstContext *ctx) { return ""; }

std::any VisualizerVisitor::visitStmt(SpiceParser::StmtContext *ctx) { return ""; }

std::any VisualizerVisitor::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) { return ""; }

std::any VisualizerVisitor::visitDeclSpecifiers(SpiceParser::DeclSpecifiersContext *ctx) { return ""; }

std::any VisualizerVisitor::visitDeclSpecifier(SpiceParser::DeclSpecifierContext *ctx) { return ""; }

std::any VisualizerVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) { return ""; }

std::any VisualizerVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) { return ""; }

std::any VisualizerVisitor::visitBreakStmt(SpiceParser::BreakStmtContext *ctx) { return ""; }

std::any VisualizerVisitor::visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) { return ""; }

std::any VisualizerVisitor::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) { return ""; }

std::any VisualizerVisitor::visitPrintfCall(SpiceParser::PrintfCallContext *ctx) { return ""; }

std::any VisualizerVisitor::visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) { return ""; }

std::any VisualizerVisitor::visitLenCall(SpiceParser::LenCallContext *ctx) { return ""; }

std::any VisualizerVisitor::visitTidCall(SpiceParser::TidCallContext *ctx) { return ""; }

std::any VisualizerVisitor::visitJoinCall(SpiceParser::JoinCallContext *ctx) { return ""; }

std::any VisualizerVisitor::visitAssignExpr(SpiceParser::AssignExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitShiftExpr(SpiceParser::ShiftExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitCastExpr(SpiceParser::CastExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) { return ""; }

std::any VisualizerVisitor::visitValue(SpiceParser::ValueContext *ctx) { return ""; }

std::any VisualizerVisitor::visitFunctionCall(SpiceParser::FunctionCallContext *ctx) { return ""; }

std::any VisualizerVisitor::visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) { return ""; }

std::any VisualizerVisitor::visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) { return ""; }

std::any VisualizerVisitor::visitPrimitiveValue(SpiceParser::PrimitiveValueContext *ctx) { return ""; }

std::any VisualizerVisitor::visitDataType(SpiceParser::DataTypeContext *ctx) { return ""; }

std::any VisualizerVisitor::visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) { return ""; }

std::any VisualizerVisitor::visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) { return ""; }

std::any VisualizerVisitor::visitAssignOp(SpiceParser::AssignOpContext *ctx) { return ""; }

std::any VisualizerVisitor::visitPrefixUnaryOp(SpiceParser::PrefixUnaryOpContext *ctx) { return ""; }
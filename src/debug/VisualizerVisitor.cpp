// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "VisualizerVisitor.h"

std::any VisualizerVisitor::visitEntry(SpiceParser::EntryContext *ctx) {}

std::any VisualizerVisitor::visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) {}

std::any VisualizerVisitor::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {}

std::any VisualizerVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {}

std::any VisualizerVisitor::visitExtDecl(SpiceParser::ExtDeclContext *ctx) {}

std::any VisualizerVisitor::visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) {}

std::any VisualizerVisitor::visitStructDef(SpiceParser::StructDefContext *ctx) {}

std::any VisualizerVisitor::visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) {}

std::any VisualizerVisitor::visitThreadDef(SpiceParser::ThreadDefContext *ctx) {}

std::any VisualizerVisitor::visitUnsafeBlockDef(SpiceParser::UnsafeBlockDefContext *ctx) {}

std::any VisualizerVisitor::visitForLoop(SpiceParser::ForLoopContext *ctx) {}

std::any VisualizerVisitor::visitForHead(SpiceParser::ForHeadContext *ctx) {}

std::any VisualizerVisitor::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) {}

std::any VisualizerVisitor::visitForeachHead(SpiceParser::ForeachHeadContext *ctx) {}

std::any VisualizerVisitor::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {}

std::any VisualizerVisitor::visitIfStmt(SpiceParser::IfStmtContext *ctx) {}

std::any VisualizerVisitor::visitElseStmt(SpiceParser::ElseStmtContext *ctx) {}

std::any VisualizerVisitor::visitAssertStmt(SpiceParser::AssertStmtContext *ctx) {}

std::any VisualizerVisitor::visitStmtLst(SpiceParser::StmtLstContext *ctx) {}

std::any VisualizerVisitor::visitField(SpiceParser::FieldContext *ctx) {}

std::any VisualizerVisitor::visitTypeLst(SpiceParser::TypeLstContext *ctx) {}

std::any VisualizerVisitor::visitTypeLstEllipsis(SpiceParser::TypeLstEllipsisContext *ctx) {}

std::any VisualizerVisitor::visitTypeAlts(SpiceParser::TypeAltsContext *ctx) {}

std::any VisualizerVisitor::visitArgLstDef(SpiceParser::ArgLstDefContext *ctx) {}

std::any VisualizerVisitor::visitArgLst(SpiceParser::ArgLstContext *ctx) {}

std::any VisualizerVisitor::visitStmt(SpiceParser::StmtContext *ctx) {}

std::any VisualizerVisitor::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {}

std::any VisualizerVisitor::visitDeclSpecifiers(SpiceParser::DeclSpecifiersContext *ctx) {}

std::any VisualizerVisitor::visitDeclSpecifier(SpiceParser::DeclSpecifierContext *ctx) {}

std::any VisualizerVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {}

std::any VisualizerVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {}

std::any VisualizerVisitor::visitBreakStmt(SpiceParser::BreakStmtContext *ctx) {}

std::any VisualizerVisitor::visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) {}

std::any VisualizerVisitor::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) {}

std::any VisualizerVisitor::visitPrintfCall(SpiceParser::PrintfCallContext *ctx) {}

std::any VisualizerVisitor::visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) {}

std::any VisualizerVisitor::visitLenCall(SpiceParser::LenCallContext *ctx) {}

std::any VisualizerVisitor::visitTidCall(SpiceParser::TidCallContext *ctx) {}

std::any VisualizerVisitor::visitJoinCall(SpiceParser::JoinCallContext *ctx) {}

std::any VisualizerVisitor::visitAssignExpr(SpiceParser::AssignExprContext *ctx) {}

std::any VisualizerVisitor::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) {}

std::any VisualizerVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {}

std::any VisualizerVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {}

std::any VisualizerVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {}

std::any VisualizerVisitor::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) {}

std::any VisualizerVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {}

std::any VisualizerVisitor::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) {}

std::any VisualizerVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {}

std::any VisualizerVisitor::visitShiftExpr(SpiceParser::ShiftExprContext *ctx) {}

std::any VisualizerVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {}

std::any VisualizerVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {}

std::any VisualizerVisitor::visitCastExpr(SpiceParser::CastExprContext *ctx) {}

std::any VisualizerVisitor::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) {}

std::any VisualizerVisitor::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) {}

std::any VisualizerVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {}

std::any VisualizerVisitor::visitValue(SpiceParser::ValueContext *ctx) {}

std::any VisualizerVisitor::visitFunctionCall(SpiceParser::FunctionCallContext *ctx) {}

std::any VisualizerVisitor::visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) {}

std::any VisualizerVisitor::visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) {}

std::any VisualizerVisitor::visitPrimitiveValue(SpiceParser::PrimitiveValueContext *ctx) {}

std::any VisualizerVisitor::visitDataType(SpiceParser::DataTypeContext *ctx) {}

std::any VisualizerVisitor::visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) {}

std::any VisualizerVisitor::visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) {}

std::any VisualizerVisitor::visitAssignOp(SpiceParser::AssignOpContext *ctx) {}

std::any VisualizerVisitor::visitPrefixUnaryOp(SpiceParser::PrefixUnaryOpContext *ctx) {}
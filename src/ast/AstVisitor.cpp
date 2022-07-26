// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AstVisitor.h"

#include <ast/AstNodes.h>

std::any AstVisitor::visitEntry(const EntryNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitMainFctDef(const MainFctDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitFctDef(const FctDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitProcDef(const ProcDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitStructDef(const StructDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitGenericTypeDef(const GenericTypeDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitGlobalVarDef(const GlobalVarDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitExtDecl(const ExtDeclNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitThreadDef(const ThreadDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitUnsafeBlockDef(const UnsafeBlockDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitForLoop(const ForLoopNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitForeachLoop(const ForeachLoopNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitWhileLoop(const WhileLoopNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitIfStmt(const IfStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitAssertStmt(const AssertStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitStmtLst(const StmtLstNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitTypeLst(const TypeLstNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitTypeAltsLst(const TypeAltsLstNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitArgLstDef(const ArgLstDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitArgLst(const ArgLstNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitField(const FieldNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitStmt(const StmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitDeclStmt(const DeclStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitDeclSpecifiers(const DeclSpecifiersNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitImportStmt(const ImportStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitReturnStmt(const ReturnStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitBreakStmt(const BreakStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitContinueStmt(const ContinueStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitPrintfCall(const PrintfCallNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitSizeofCall(const SizeofCallNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitLenCall(const LenCallNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitTidCall(const TidCallNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitJoinCall(const JoinCallNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitAssignExpr(const AssignExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitTernaryExpr(const TernaryExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitLogicalOrExpr(const LogicalOrExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitLogicalAndExpr(const LogicalAndExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitBitwiseOrExpr(const BitwiseOrExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitBitwiseXorExpr(const BitwiseXorExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitBitwiseAndExpr(const BitwiseAndExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitEqualityExpr(const EqualityExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitRelationalExpr(const RelationalExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitShiftExpr(const ShiftExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitAdditiveExpr(const AdditiveExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitMultiplicativeExpr(const MultiplicativeExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitCastExpr(const CastExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitPrefixUnaryExpr(const PrefixUnaryExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitPostfixUnaryExpr(const PostfixUnaryExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitAtomicExpr(const AtomicExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitValue(const ValueNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitPrimitiveValue(const PrimitiveValueNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitFunctionCall(const FunctionCallNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitArrayInitialization(const ArrayInitializationNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitStructInstantiation(const StructInstantiationNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitDataType(const DataTypeNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitBaseDataType(const BaseDataTypeNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitCustomDataType(const CustomDataTypeNode *ctx) { return visitChildren(ctx); }
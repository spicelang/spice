// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AstVisitor.h"

#include <ast/AstNodes.h>

std::any AstVisitor::visitEntry(EntryNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitMainFctDef(MainFctDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitFctDef(FctDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitProcDef(ProcDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitStructDef(StructDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitGenericTypeDef(GenericTypeDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitGlobalVarDef(GlobalVarDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitExtDecl(ExtDeclNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitThreadDef(ThreadDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitUnsafeBlockDef(UnsafeBlockDefNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitForLoop(ForLoopNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitForeachLoop(ForeachLoopNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitWhileLoop(WhileLoopNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitIfStmt(IfStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitElseStmt(ElseStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitAssertStmt(AssertStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitStmtLst(StmtLstNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitTypeLst(TypeLstNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitTypeAltsLst(TypeAltsLstNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitParamLst(ParamLstNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitArgLst(ArgLstNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitField(FieldNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitStmt(StmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitDeclStmt(DeclStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitSpecifierLst(SpecifierLstNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitSpecifier(SpecifierNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitImportStmt(ImportStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitReturnStmt(ReturnStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitBreakStmt(BreakStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitContinueStmt(ContinueStmtNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitPrintfCall(PrintfCallNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitSizeofCall(SizeofCallNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitLenCall(LenCallNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitTidCall(TidCallNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitJoinCall(JoinCallNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitAssignExpr(AssignExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitTernaryExpr(TernaryExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitLogicalOrExpr(LogicalOrExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitLogicalAndExpr(LogicalAndExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitBitwiseOrExpr(BitwiseOrExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitBitwiseXorExpr(BitwiseXorExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitBitwiseAndExpr(BitwiseAndExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitEqualityExpr(EqualityExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitRelationalExpr(RelationalExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitShiftExpr(ShiftExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitAdditiveExpr(AdditiveExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitMultiplicativeExpr(MultiplicativeExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitCastExpr(CastExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitPrefixUnaryExpr(PrefixUnaryExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitPostfixUnaryExpr(PostfixUnaryExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitAtomicExpr(AtomicExprNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitValue(ValueNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitPrimitiveValue(PrimitiveValueNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitFunctionCall(FunctionCallNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitArrayInitialization(ArrayInitializationNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitStructInstantiation(StructInstantiationNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitDataType(DataTypeNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitBaseDataType(BaseDataTypeNode *ctx) { return visitChildren(ctx); }

std::any AstVisitor::visitCustomDataType(CustomDataTypeNode *ctx) { return visitChildren(ctx); }
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AstVisitor.h"

#include <ast/AstNodes.h>

std::any AstVisitor::visitEntry(EntryNode *node) { return visitChildren(node); }

std::any AstVisitor::visitMainFctDef(MainFctDefNode *node) { return visitChildren(node); }

std::any AstVisitor::visitFctDef(FctDefNode *node) { return visitChildren(node); }

std::any AstVisitor::visitProcDef(ProcDefNode *node) { return visitChildren(node); }

std::any AstVisitor::visitStructDef(StructDefNode *node) { return visitChildren(node); }

std::any AstVisitor::visitGenericTypeDef(GenericTypeDefNode *node) { return visitChildren(node); }

std::any AstVisitor::visitGlobalVarDef(GlobalVarDefNode *node) { return visitChildren(node); }

std::any AstVisitor::visitExtDecl(ExtDeclNode *node) { return visitChildren(node); }

std::any AstVisitor::visitThreadDef(ThreadDefNode *node) { return visitChildren(node); }

std::any AstVisitor::visitUnsafeBlockDef(UnsafeBlockDefNode *node) { return visitChildren(node); }

std::any AstVisitor::visitForLoop(ForLoopNode *node) { return visitChildren(node); }

std::any AstVisitor::visitForeachLoop(ForeachLoopNode *node) { return visitChildren(node); }

std::any AstVisitor::visitWhileLoop(WhileLoopNode *node) { return visitChildren(node); }

std::any AstVisitor::visitIfStmt(IfStmtNode *node) { return visitChildren(node); }

std::any AstVisitor::visitElseStmt(ElseStmtNode *node) { return visitChildren(node); }

std::any AstVisitor::visitAssertStmt(AssertStmtNode *node) { return visitChildren(node); }

std::any AstVisitor::visitStmtLst(StmtLstNode *node) { return visitChildren(node); }

std::any AstVisitor::visitTypeLst(TypeLstNode *node) { return visitChildren(node); }

std::any AstVisitor::visitTypeAltsLst(TypeAltsLstNode *node) { return visitChildren(node); }

std::any AstVisitor::visitParamLst(ParamLstNode *node) { return visitChildren(node); }

std::any AstVisitor::visitArgLst(ArgLstNode *node) { return visitChildren(node); }

std::any AstVisitor::visitField(FieldNode *node) { return visitChildren(node); }

std::any AstVisitor::visitStmt(StmtNode *node) { return visitChildren(node); }

std::any AstVisitor::visitDeclStmt(DeclStmtNode *node) { return visitChildren(node); }

std::any AstVisitor::visitSpecifierLst(SpecifierLstNode *node) { return visitChildren(node); }

std::any AstVisitor::visitSpecifier(SpecifierNode *node) { return visitChildren(node); }

std::any AstVisitor::visitImportStmt(ImportStmtNode *node) { return visitChildren(node); }

std::any AstVisitor::visitReturnStmt(ReturnStmtNode *node) { return visitChildren(node); }

std::any AstVisitor::visitBreakStmt(BreakStmtNode *node) { return visitChildren(node); }

std::any AstVisitor::visitContinueStmt(ContinueStmtNode *node) { return visitChildren(node); }

std::any AstVisitor::visitPrintfCall(PrintfCallNode *node) { return visitChildren(node); }

std::any AstVisitor::visitSizeofCall(SizeofCallNode *node) { return visitChildren(node); }

std::any AstVisitor::visitLenCall(LenCallNode *node) { return visitChildren(node); }

std::any AstVisitor::visitTidCall(TidCallNode *node) { return visitChildren(node); }

std::any AstVisitor::visitJoinCall(JoinCallNode *node) { return visitChildren(node); }

std::any AstVisitor::visitAssignExpr(AssignExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitTernaryExpr(TernaryExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitLogicalOrExpr(LogicalOrExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitLogicalAndExpr(LogicalAndExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitBitwiseOrExpr(BitwiseOrExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitBitwiseXorExpr(BitwiseXorExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitBitwiseAndExpr(BitwiseAndExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitEqualityExpr(EqualityExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitRelationalExpr(RelationalExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitShiftExpr(ShiftExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitAdditiveExpr(AdditiveExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitMultiplicativeExpr(MultiplicativeExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitCastExpr(CastExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitPrefixUnaryExpr(PrefixUnaryExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitPostfixUnaryExpr(PostfixUnaryExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitAtomicExpr(AtomicExprNode *node) { return visitChildren(node); }

std::any AstVisitor::visitValue(ValueNode *node) { return visitChildren(node); }

std::any AstVisitor::visitPrimitiveValue(PrimitiveValueNode *node) { return visitChildren(node); }

std::any AstVisitor::visitFunctionCall(FunctionCallNode *node) { return visitChildren(node); }

std::any AstVisitor::visitArrayInitialization(ArrayInitializationNode *node) { return visitChildren(node); }

std::any AstVisitor::visitStructInstantiation(StructInstantiationNode *node) { return visitChildren(node); }

std::any AstVisitor::visitDataType(DataTypeNode *node) { return visitChildren(node); }

std::any AstVisitor::visitBaseDataType(BaseDataTypeNode *node) { return visitChildren(node); }

std::any AstVisitor::visitCustomDataType(CustomDataTypeNode *node) { return visitChildren(node); }
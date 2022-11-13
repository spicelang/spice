// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <ast/AbstractASTVisitor.h>

template <typename T> class ASTVisitor : public AbstractASTVisitor<T> {
public:
  // Visitor methods
  T visitEntry(EntryNode *node) override;
  T visitMainFctDef(MainFctDefNode *node) override;
  T visitFctDef(FctDefNode *node) override;
  T visitProcDef(ProcDefNode *node) override;
  T visitStructDef(StructDefNode *node) override;
  T visitInterfaceDef(InterfaceDefNode *node) override;
  T visitEnumDef(EnumDefNode *node) override;
  T visitGenericTypeDef(GenericTypeDefNode *node) override;
  T visitGlobalVarDef(GlobalVarDefNode *node) override;
  T visitExtDecl(ExtDeclNode *node) override;
  T visitThreadDef(ThreadDefNode *node) override;
  T visitUnsafeBlockDef(UnsafeBlockDefNode *node) override;
  T visitForLoop(ForLoopNode *node) override;
  T visitForeachLoop(ForeachLoopNode *node) override;
  T visitWhileLoop(WhileLoopNode *node) override;
  T visitIfStmt(IfStmtNode *node) override;
  T visitElseStmt(ElseStmtNode *node) override;
  T visitAssertStmt(AssertStmtNode *node) override;
  T visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) override;
  T visitStmtLst(StmtLstNode *node) override;
  T visitTypeLst(TypeLstNode *node) override;
  T visitTypeAltsLst(TypeAltsLstNode *node) override;
  T visitParamLst(ParamLstNode *node) override;
  T visitArgLst(ArgLstNode *node) override;
  T visitEnumItemLst(EnumItemLstNode *node) override;
  T visitEnumItem(EnumItemNode *node) override;
  T visitField(FieldNode *node) override;
  T visitSignature(SignatureNode *node) override;
  T visitStmt(StmtNode *node) override;
  T visitDeclStmt(DeclStmtNode *node) override;
  T visitSpecifierLst(SpecifierLstNode *node) override;
  T visitSpecifier(SpecifierNode *node) override;
  T visitImportStmt(ImportStmtNode *node) override;
  T visitReturnStmt(ReturnStmtNode *node) override;
  T visitBreakStmt(BreakStmtNode *node) override;
  T visitContinueStmt(ContinueStmtNode *node) override;
  T visitPrintfCall(PrintfCallNode *node) override;
  T visitSizeofCall(SizeofCallNode *node) override;
  T visitLenCall(LenCallNode *node) override;
  T visitTidCall(TidCallNode *node) override;
  T visitJoinCall(JoinCallNode *node) override;
  T visitAssignExpr(AssignExprNode *node) override;
  T visitTernaryExpr(TernaryExprNode *node) override;
  T visitLogicalOrExpr(LogicalOrExprNode *node) override;
  T visitLogicalAndExpr(LogicalAndExprNode *node) override;
  T visitBitwiseOrExpr(BitwiseOrExprNode *node) override;
  T visitBitwiseXorExpr(BitwiseXorExprNode *node) override;
  T visitBitwiseAndExpr(BitwiseAndExprNode *node) override;
  T visitEqualityExpr(EqualityExprNode *node) override;
  T visitRelationalExpr(RelationalExprNode *node) override;
  T visitShiftExpr(ShiftExprNode *node) override;
  T visitAdditiveExpr(AdditiveExprNode *node) override;
  T visitMultiplicativeExpr(MultiplicativeExprNode *node) override;
  T visitCastExpr(CastExprNode *node) override;
  T visitPrefixUnaryExpr(PrefixUnaryExprNode *node) override;
  T visitPostfixUnaryExpr(PostfixUnaryExprNode *node) override;
  T visitAtomicExpr(AtomicExprNode *node) override;
  T visitValue(ValueNode *node) override;
  T visitPrimitiveValue(PrimitiveValueNode *node) override;
  T visitFunctionCall(FunctionCallNode *node) override;
  T visitArrayInitialization(ArrayInitializationNode *node) override;
  T visitStructInstantiation(StructInstantiationNode *node) override;
  T visitDataType(DataTypeNode *node) override;
  T visitBaseDataType(BaseDataTypeNode *node) override;
  T visitCustomDataType(CustomDataTypeNode *node) override;
};
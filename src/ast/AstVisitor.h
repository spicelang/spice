// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <ast/AbstractAstVisitor.h>

class AstVisitor : public AbstractAstVisitor {
public:
  // Visitor methods
  virtual std::any visitEntry(EntryNode *ctx) override;
  virtual std::any visitMainFctDef(MainFctDefNode *ctx) override;
  virtual std::any visitFctDef(FctDefNode *ctx) override;
  virtual std::any visitProcDef(ProcDefNode *ctx) override;
  virtual std::any visitStructDef(StructDefNode *ctx) override;
  virtual std::any visitGenericTypeDef(GenericTypeDefNode *ctx) override;
  virtual std::any visitGlobalVarDef(GlobalVarDefNode *ctx) override;
  virtual std::any visitExtDecl(ExtDeclNode *ctx) override;
  virtual std::any visitThreadDef(ThreadDefNode *ctx) override;
  virtual std::any visitUnsafeBlockDef(UnsafeBlockDefNode *ctx) override;
  virtual std::any visitForLoop(ForLoopNode *ctx) override;
  virtual std::any visitForeachLoop(ForeachLoopNode *ctx) override;
  virtual std::any visitWhileLoop(WhileLoopNode *ctx) override;
  virtual std::any visitIfStmt(IfStmtNode *ctx) override;
  virtual std::any visitElseStmt(ElseStmtNode *ctx) override;
  virtual std::any visitAssertStmt(AssertStmtNode *ctx) override;
  virtual std::any visitStmtLst(StmtLstNode *ctx) override;
  virtual std::any visitTypeLst(TypeLstNode *ctx) override;
  virtual std::any visitTypeAltsLst(TypeAltsLstNode *ctx) override;
  virtual std::any visitParamLst(ParamLstNode *ctx) override;
  virtual std::any visitArgLst(ArgLstNode *ctx) override;
  virtual std::any visitField(FieldNode *ctx) override;
  virtual std::any visitStmt(StmtNode *ctx) override;
  virtual std::any visitDeclStmt(DeclStmtNode *ctx) override;
  virtual std::any visitSpecifierLst(SpecifierLstNode *ctx) override;
  virtual std::any visitSpecifier(SpecifierNode *ctx) override;
  virtual std::any visitImportStmt(ImportStmtNode *ctx) override;
  virtual std::any visitReturnStmt(ReturnStmtNode *ctx) override;
  virtual std::any visitBreakStmt(BreakStmtNode *ctx) override;
  virtual std::any visitContinueStmt(ContinueStmtNode *ctx) override;
  virtual std::any visitPrintfCall(PrintfCallNode *ctx) override;
  virtual std::any visitSizeofCall(SizeofCallNode *ctx) override;
  virtual std::any visitLenCall(LenCallNode *ctx) override;
  virtual std::any visitTidCall(TidCallNode *ctx) override;
  virtual std::any visitJoinCall(JoinCallNode *ctx) override;
  virtual std::any visitAssignExpr(AssignExprNode *ctx) override;
  virtual std::any visitTernaryExpr(TernaryExprNode *ctx) override;
  virtual std::any visitLogicalOrExpr(LogicalOrExprNode *ctx) override;
  virtual std::any visitLogicalAndExpr(LogicalAndExprNode *ctx) override;
  virtual std::any visitBitwiseOrExpr(BitwiseOrExprNode *ctx) override;
  virtual std::any visitBitwiseXorExpr(BitwiseXorExprNode *ctx) override;
  virtual std::any visitBitwiseAndExpr(BitwiseAndExprNode *ctx) override;
  virtual std::any visitEqualityExpr(EqualityExprNode *ctx) override;
  virtual std::any visitRelationalExpr(RelationalExprNode *ctx) override;
  virtual std::any visitShiftExpr(ShiftExprNode *ctx) override;
  virtual std::any visitAdditiveExpr(AdditiveExprNode *ctx) override;
  virtual std::any visitMultiplicativeExpr(MultiplicativeExprNode *ctx) override;
  virtual std::any visitCastExpr(CastExprNode *ctx) override;
  virtual std::any visitPrefixUnaryExpr(PrefixUnaryExprNode *ctx) override;
  virtual std::any visitPostfixUnaryExpr(PostfixUnaryExprNode *ctx) override;
  virtual std::any visitAtomicExpr(AtomicExprNode *ctx) override;
  virtual std::any visitValue(ValueNode *ctx) override;
  virtual std::any visitPrimitiveValue(PrimitiveValueNode *ctx) override;
  virtual std::any visitFunctionCall(FunctionCallNode *ctx) override;
  virtual std::any visitArrayInitialization(ArrayInitializationNode *ctx) override;
  virtual std::any visitStructInstantiation(StructInstantiationNode *ctx) override;
  virtual std::any visitDataType(DataTypeNode *ctx) override;
  virtual std::any visitBaseDataType(BaseDataTypeNode *ctx) override;
  virtual std::any visitCustomDataType(CustomDataTypeNode *ctx) override;
};
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <ast/AbstractAstVisitor.h>

class AstVisitor : public AbstractAstVisitor {
public:
  // Visitor methods
  virtual std::any visitEntry(const EntryNode *ctx) override;
  virtual std::any visitMainFctDef(const MainFctDefNode *ctx) override;
  virtual std::any visitFctDef(const FctDefNode *ctx) override;
  virtual std::any visitProcDef(const ProcDefNode *ctx) override;
  virtual std::any visitStructDef(const StructDefNode *ctx) override;
  virtual std::any visitGenericTypeDef(const GenericTypeDefNode *ctx) override;
  virtual std::any visitGlobalVarDef(const GlobalVarDefNode *ctx) override;
  virtual std::any visitExtDecl(const ExtDeclNode *ctx) override;
  virtual std::any visitThreadDef(const ThreadDefNode *ctx) override;
  virtual std::any visitUnsafeBlockDef(const UnsafeBlockDefNode *ctx) override;
  virtual std::any visitForLoop(const ForLoopNode *ctx) override;
  virtual std::any visitForeachLoop(const ForeachLoopNode *ctx) override;
  virtual std::any visitWhileLoop(const WhileLoopNode *ctx) override;
  virtual std::any visitIfStmt(const IfStmtNode *ctx) override;
  virtual std::any visitAssertStmt(const AssertStmtNode *ctx) override;
  virtual std::any visitStmtLst(const StmtLstNode *ctx) override;
  virtual std::any visitTypeLst(const TypeLstNode *ctx) override;
  virtual std::any visitTypeAltsLst(const TypeAltsLstNode *ctx) override;
  virtual std::any visitArgLstDef(const ArgLstDefNode *ctx) override;
  virtual std::any visitArgLst(const ArgLstNode *ctx) override;
  virtual std::any visitField(const FieldNode *ctx) override;
  virtual std::any visitStmt(const StmtNode *ctx) override;
  virtual std::any visitDeclStmt(const DeclStmtNode *ctx) override;
  virtual std::any visitDeclSpecifiers(const DeclSpecifiersNode *ctx) override;
  virtual std::any visitImportStmt(const ImportStmtNode *ctx) override;
  virtual std::any visitReturnStmt(const ReturnStmtNode *ctx) override;
  virtual std::any visitBreakStmt(const BreakStmtNode *ctx) override;
  virtual std::any visitContinueStmt(const ContinueStmtNode *ctx) override;
  virtual std::any visitPrintfCall(const PrintfCallNode *ctx) override;
  virtual std::any visitSizeofCall(const SizeofCallNode *ctx) override;
  virtual std::any visitLenCall(const LenCallNode *ctx) override;
  virtual std::any visitTidCall(const TidCallNode *ctx) override;
  virtual std::any visitJoinCall(const JoinCallNode *ctx) override;
  virtual std::any visitAssignExpr(const AssignExprNode *ctx) override;
  virtual std::any visitTernaryExpr(const TernaryExprNode *ctx) override;
  virtual std::any visitLogicalOrExpr(const LogicalOrExprNode *ctx) override;
  virtual std::any visitLogicalAndExpr(const LogicalAndExprNode *ctx) override;
  virtual std::any visitBitwiseOrExpr(const BitwiseOrExprNode *ctx) override;
  virtual std::any visitBitwiseXorExpr(const BitwiseXorExprNode *ctx) override;
  virtual std::any visitBitwiseAndExpr(const BitwiseAndExprNode *ctx) override;
  virtual std::any visitEqualityExpr(const EqualityExprNode *ctx) override;
  virtual std::any visitRelationalExpr(const RelationalExprNode *ctx) override;
  virtual std::any visitShiftExpr(const ShiftExprNode *ctx) override;
  virtual std::any visitAdditiveExpr(const AdditiveExprNode *ctx) override;
  virtual std::any visitMultiplicativeExpr(const MultiplicativeExprNode *ctx) override;
  virtual std::any visitCastExpr(const CastExprNode *ctx) override;
  virtual std::any visitPrefixUnaryExpr(const PrefixUnaryExprNode *ctx) override;
  virtual std::any visitPostfixUnaryExpr(const PostfixUnaryExprNode *ctx) override;
  virtual std::any visitAtomicExpr(const AtomicExprNode *ctx) override;
  virtual std::any visitValue(const ValueNode *ctx) override;
  virtual std::any visitPrimitiveValue(const PrimitiveValueNode *ctx) override;
  virtual std::any visitFunctionCall(const FunctionCallNode *ctx) override;
  virtual std::any visitArrayInitialization(const ArrayInitializationNode *ctx) override;
  virtual std::any visitStructInstantiation(const StructInstantiationNode *ctx) override;
  virtual std::any visitDataType(const DataTypeNode *ctx) override;
  virtual std::any visitBaseDataType(const BaseDataTypeNode *ctx) override;
  virtual std::any visitCustomDataType(const CustomDataTypeNode *ctx) override;

};
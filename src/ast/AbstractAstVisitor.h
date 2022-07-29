// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <any>

// Forward declarations
class AstNode;
class EntryNode;
class MainFctDefNode;
class FctDefNode;
class ProcDefNode;
class StructDefNode;
class GenericTypeDefNode;
class GlobalVarDefNode;
class ExtDeclNode;
class ThreadDefNode;
class UnsafeBlockDefNode;
class ForLoopNode;
class ForeachLoopNode;
class WhileLoopNode;
class IfStmtNode;
class ElseStmtNode;
class AssertStmtNode;
class StmtLstNode;
class TypeLstNode;
class TypeAltsLstNode;
class ParamLstNode;
class ArgLstNode;
class FieldNode;
class StmtNode;
class DeclStmtNode;
class DeclSpecifiersNode;
class ImportStmtNode;
class ReturnStmtNode;
class BreakStmtNode;
class ContinueStmtNode;
class PrintfCallNode;
class SizeofCallNode;
class LenCallNode;
class TidCallNode;
class JoinCallNode;
class AssignExprNode;
class TernaryExprNode;
class LogicalOrExprNode;
class LogicalAndExprNode;
class BitwiseOrExprNode;
class BitwiseXorExprNode;
class BitwiseAndExprNode;
class EqualityExprNode;
class RelationalExprNode;
class ShiftExprNode;
class AdditiveExprNode;
class MultiplicativeExprNode;
class CastExprNode;
class PrefixUnaryExprNode;
class PostfixUnaryExprNode;
class AtomicExprNode;
class ValueNode;
class PrimitiveValueNode;
class FunctionCallNode;
class ArrayInitializationNode;
class StructInstantiationNode;
class DataTypeNode;
class BaseDataTypeNode;
class CustomDataTypeNode;

class AbstractAstVisitor {
public:
  // General visitor method
  virtual std::any visit(AstNode *ctx);

  // Visitor methods
  virtual std::any visitEntry(EntryNode *ctx) = 0;
  virtual std::any visitMainFctDef(MainFctDefNode *ctx) = 0;
  virtual std::any visitFctDef(FctDefNode *ctx) = 0;
  virtual std::any visitProcDef(ProcDefNode *ctx) = 0;
  virtual std::any visitStructDef(StructDefNode *ctx) = 0;
  virtual std::any visitGenericTypeDef(GenericTypeDefNode *ctx) = 0;
  virtual std::any visitGlobalVarDef(GlobalVarDefNode *ctx) = 0;
  virtual std::any visitExtDecl(ExtDeclNode *ctx) = 0;
  virtual std::any visitThreadDef(ThreadDefNode *ctx) = 0;
  virtual std::any visitUnsafeBlockDef(UnsafeBlockDefNode *ctx) = 0;
  virtual std::any visitForLoop(ForLoopNode *ctx) = 0;
  virtual std::any visitForeachLoop(ForeachLoopNode *ctx) = 0;
  virtual std::any visitWhileLoop(WhileLoopNode *ctx) = 0;
  virtual std::any visitIfStmt(IfStmtNode *ctx) = 0;
  virtual std::any visitElseStmt(ElseStmtNode *ctx) = 0;
  virtual std::any visitAssertStmt(AssertStmtNode *ctx) = 0;
  virtual std::any visitStmtLst(StmtLstNode *ctx) = 0;
  virtual std::any visitTypeLst(TypeLstNode *ctx) = 0;
  virtual std::any visitTypeAltsLst(TypeAltsLstNode *ctx) = 0;
  virtual std::any visitParamLst(ParamLstNode *ctx) = 0;
  virtual std::any visitArgLst(ArgLstNode *ctx) = 0;
  virtual std::any visitField(FieldNode *ctx) = 0;
  virtual std::any visitStmt(StmtNode *ctx) = 0;
  virtual std::any visitDeclStmt(DeclStmtNode *ctx) = 0;
  virtual std::any visitDeclSpecifiers(DeclSpecifiersNode *ctx) = 0;
  virtual std::any visitImportStmt(ImportStmtNode *ctx) = 0;
  virtual std::any visitReturnStmt(ReturnStmtNode *ctx) = 0;
  virtual std::any visitBreakStmt(BreakStmtNode *ctx) = 0;
  virtual std::any visitContinueStmt(ContinueStmtNode *ctx) = 0;
  virtual std::any visitPrintfCall(PrintfCallNode *ctx) = 0;
  virtual std::any visitSizeofCall(SizeofCallNode *ctx) = 0;
  virtual std::any visitLenCall(LenCallNode *ctx) = 0;
  virtual std::any visitTidCall(TidCallNode *ctx) = 0;
  virtual std::any visitJoinCall(JoinCallNode *ctx) = 0;
  virtual std::any visitAssignExpr(AssignExprNode *ctx) = 0;
  virtual std::any visitTernaryExpr(TernaryExprNode *ctx) = 0;
  virtual std::any visitLogicalOrExpr(LogicalOrExprNode *ctx) = 0;
  virtual std::any visitLogicalAndExpr(LogicalAndExprNode *ctx) = 0;
  virtual std::any visitBitwiseOrExpr(BitwiseOrExprNode *ctx) = 0;
  virtual std::any visitBitwiseXorExpr(BitwiseXorExprNode *ctx) = 0;
  virtual std::any visitBitwiseAndExpr(BitwiseAndExprNode *ctx) = 0;
  virtual std::any visitEqualityExpr(EqualityExprNode *ctx) = 0;
  virtual std::any visitRelationalExpr(RelationalExprNode *ctx) = 0;
  virtual std::any visitShiftExpr(ShiftExprNode *ctx) = 0;
  virtual std::any visitAdditiveExpr(AdditiveExprNode *ctx) = 0;
  virtual std::any visitMultiplicativeExpr(MultiplicativeExprNode *ctx) = 0;
  virtual std::any visitCastExpr(CastExprNode *ctx) = 0;
  virtual std::any visitPrefixUnaryExpr(PrefixUnaryExprNode *ctx) = 0;
  virtual std::any visitPostfixUnaryExpr(PostfixUnaryExprNode *ctx) = 0;
  virtual std::any visitAtomicExpr(AtomicExprNode *ctx) = 0;
  virtual std::any visitValue(ValueNode *ctx) = 0;
  virtual std::any visitPrimitiveValue(PrimitiveValueNode *ctx) = 0;
  virtual std::any visitFunctionCall(FunctionCallNode *ctx) = 0;
  virtual std::any visitArrayInitialization(ArrayInitializationNode *ctx) = 0;
  virtual std::any visitStructInstantiation(StructInstantiationNode *ctx) = 0;
  virtual std::any visitDataType(DataTypeNode *ctx) = 0;
  virtual std::any visitBaseDataType(BaseDataTypeNode *ctx) = 0;
  virtual std::any visitCustomDataType(CustomDataTypeNode *ctx) = 0;

  // Public methods
  virtual std::any visitChildren(AstNode *ctx);
};
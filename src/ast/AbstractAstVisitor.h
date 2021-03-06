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
class SpecifierLstNode;
class SpecifierNode;
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
  virtual std::any visit(AstNode *node);

  // Visitor methods
  virtual std::any visitEntry(EntryNode *node) = 0;
  virtual std::any visitMainFctDef(MainFctDefNode *node) = 0;
  virtual std::any visitFctDef(FctDefNode *node) = 0;
  virtual std::any visitProcDef(ProcDefNode *node) = 0;
  virtual std::any visitStructDef(StructDefNode *node) = 0;
  virtual std::any visitGenericTypeDef(GenericTypeDefNode *node) = 0;
  virtual std::any visitGlobalVarDef(GlobalVarDefNode *node) = 0;
  virtual std::any visitExtDecl(ExtDeclNode *node) = 0;
  virtual std::any visitThreadDef(ThreadDefNode *node) = 0;
  virtual std::any visitUnsafeBlockDef(UnsafeBlockDefNode *node) = 0;
  virtual std::any visitForLoop(ForLoopNode *node) = 0;
  virtual std::any visitForeachLoop(ForeachLoopNode *node) = 0;
  virtual std::any visitWhileLoop(WhileLoopNode *node) = 0;
  virtual std::any visitIfStmt(IfStmtNode *node) = 0;
  virtual std::any visitElseStmt(ElseStmtNode *node) = 0;
  virtual std::any visitAssertStmt(AssertStmtNode *node) = 0;
  virtual std::any visitStmtLst(StmtLstNode *node) = 0;
  virtual std::any visitTypeLst(TypeLstNode *node) = 0;
  virtual std::any visitTypeAltsLst(TypeAltsLstNode *node) = 0;
  virtual std::any visitParamLst(ParamLstNode *node) = 0;
  virtual std::any visitArgLst(ArgLstNode *node) = 0;
  virtual std::any visitField(FieldNode *node) = 0;
  virtual std::any visitStmt(StmtNode *node) = 0;
  virtual std::any visitDeclStmt(DeclStmtNode *node) = 0;
  virtual std::any visitSpecifierLst(SpecifierLstNode *node) = 0;
  virtual std::any visitSpecifier(SpecifierNode *node) = 0;
  virtual std::any visitImportStmt(ImportStmtNode *node) = 0;
  virtual std::any visitReturnStmt(ReturnStmtNode *node) = 0;
  virtual std::any visitBreakStmt(BreakStmtNode *node) = 0;
  virtual std::any visitContinueStmt(ContinueStmtNode *node) = 0;
  virtual std::any visitPrintfCall(PrintfCallNode *node) = 0;
  virtual std::any visitSizeofCall(SizeofCallNode *node) = 0;
  virtual std::any visitLenCall(LenCallNode *node) = 0;
  virtual std::any visitTidCall(TidCallNode *node) = 0;
  virtual std::any visitJoinCall(JoinCallNode *node) = 0;
  virtual std::any visitAssignExpr(AssignExprNode *node) = 0;
  virtual std::any visitTernaryExpr(TernaryExprNode *node) = 0;
  virtual std::any visitLogicalOrExpr(LogicalOrExprNode *node) = 0;
  virtual std::any visitLogicalAndExpr(LogicalAndExprNode *node) = 0;
  virtual std::any visitBitwiseOrExpr(BitwiseOrExprNode *node) = 0;
  virtual std::any visitBitwiseXorExpr(BitwiseXorExprNode *node) = 0;
  virtual std::any visitBitwiseAndExpr(BitwiseAndExprNode *node) = 0;
  virtual std::any visitEqualityExpr(EqualityExprNode *node) = 0;
  virtual std::any visitRelationalExpr(RelationalExprNode *node) = 0;
  virtual std::any visitShiftExpr(ShiftExprNode *node) = 0;
  virtual std::any visitAdditiveExpr(AdditiveExprNode *node) = 0;
  virtual std::any visitMultiplicativeExpr(MultiplicativeExprNode *node) = 0;
  virtual std::any visitCastExpr(CastExprNode *node) = 0;
  virtual std::any visitPrefixUnaryExpr(PrefixUnaryExprNode *node) = 0;
  virtual std::any visitPostfixUnaryExpr(PostfixUnaryExprNode *node) = 0;
  virtual std::any visitAtomicExpr(AtomicExprNode *node) = 0;
  virtual std::any visitValue(ValueNode *node) = 0;
  virtual std::any visitPrimitiveValue(PrimitiveValueNode *node) = 0;
  virtual std::any visitFunctionCall(FunctionCallNode *node) = 0;
  virtual std::any visitArrayInitialization(ArrayInitializationNode *node) = 0;
  virtual std::any visitStructInstantiation(StructInstantiationNode *node) = 0;
  virtual std::any visitDataType(DataTypeNode *node) = 0;
  virtual std::any visitBaseDataType(BaseDataTypeNode *node) = 0;
  virtual std::any visitCustomDataType(CustomDataTypeNode *node) = 0;

  // Public methods
  virtual std::any visitChildren(AstNode *node);
};
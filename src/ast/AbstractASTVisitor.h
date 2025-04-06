// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <any>

namespace spice::compiler {

// Forward declarations
class ASTNode;
class EntryNode;
class MainFctDefNode;
class FctNameNode;
class FctDefNode;
class ProcDefNode;
class StructDefNode;
class InterfaceDefNode;
class EnumDefNode;
class GenericTypeDefNode;
class AliasDefNode;
class GlobalVarDefNode;
class ExtDeclNode;
class ImportDefNode;
class UnsafeBlockNode;
class ForLoopNode;
class ForeachLoopNode;
class WhileLoopNode;
class DoWhileLoopNode;
class IfStmtNode;
class ElseStmtNode;
class SwitchStmtNode;
class CaseBranchNode;
class DefaultBranchNode;
class AnonymousBlockStmtNode;
class StmtLstNode;
class TypeLstNode;
class TypeAltsLstNode;
class ParamLstNode;
class ArgLstNode;
class EnumItemLstNode;
class EnumItemNode;
class FieldNode;
class SignatureNode;
class DeclStmtNode;
class ExprStmtNode;
class QualifierLstNode;
class QualifierNode;
class ModAttrNode;
class TopLevelDefinitionAttrNode;
class LambdaAttrNode;
class AttrLstNode;
class AttrNode;
class CaseConstantNode;
class ReturnStmtNode;
class BreakStmtNode;
class ContinueStmtNode;
class FallthroughStmtNode;
class AssertStmtNode;
class BuiltinCallNode;
class PrintfCallNode;
class SizeofCallNode;
class AlignofCallNode;
class TypeidCallNode;
class LenCallNode;
class PanicCallNode;
class SysCallNode;
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
class ConstantNode;
class FctCallNode;
class ArrayInitializationNode;
class StructInstantiationNode;
class LambdaFuncNode;
class LambdaProcNode;
class LambdaExprNode;
class DataTypeNode;
class BaseDataTypeNode;
class CustomDataTypeNode;
class FunctionDataTypeNode;

class AbstractASTVisitor {
protected:
  ~AbstractASTVisitor() = default;

public:
  // General visitor method
  std::any visit(ASTNode *node);
  std::any visitChildren(ASTNode *node);

  // Visitor methods
  virtual std::any visitEntry(EntryNode *node) = 0;
  virtual std::any visitMainFctDef(MainFctDefNode *node) = 0;
  virtual std::any visitFctDef(FctDefNode *node) = 0;
  virtual std::any visitProcDef(ProcDefNode *node) = 0;
  virtual std::any visitFctName(FctNameNode *node) = 0;
  virtual std::any visitStructDef(StructDefNode *node) = 0;
  virtual std::any visitInterfaceDef(InterfaceDefNode *node) = 0;
  virtual std::any visitEnumDef(EnumDefNode *node) = 0;
  virtual std::any visitGenericTypeDef(GenericTypeDefNode *node) = 0;
  virtual std::any visitAliasDef(AliasDefNode *node) = 0;
  virtual std::any visitGlobalVarDef(GlobalVarDefNode *node) = 0;
  virtual std::any visitExtDecl(ExtDeclNode *node) = 0;
  virtual std::any visitImportDef(ImportDefNode *node) = 0;
  virtual std::any visitUnsafeBlock(UnsafeBlockNode *node) = 0;
  virtual std::any visitForLoop(ForLoopNode *node) = 0;
  virtual std::any visitForeachLoop(ForeachLoopNode *node) = 0;
  virtual std::any visitWhileLoop(WhileLoopNode *node) = 0;
  virtual std::any visitDoWhileLoop(DoWhileLoopNode *node) = 0;
  virtual std::any visitIfStmt(IfStmtNode *node) = 0;
  virtual std::any visitElseStmt(ElseStmtNode *node) = 0;
  virtual std::any visitSwitchStmt(SwitchStmtNode *node) = 0;
  virtual std::any visitCaseBranch(CaseBranchNode *node) = 0;
  virtual std::any visitDefaultBranch(DefaultBranchNode *node) = 0;
  virtual std::any visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) = 0;
  virtual std::any visitStmtLst(StmtLstNode *node) = 0;
  virtual std::any visitTypeLst(TypeLstNode *node) = 0;
  virtual std::any visitTypeAltsLst(TypeAltsLstNode *node) = 0;
  virtual std::any visitParamLst(ParamLstNode *node) = 0;
  virtual std::any visitArgLst(ArgLstNode *node) = 0;
  virtual std::any visitEnumItemLst(EnumItemLstNode *node) = 0;
  virtual std::any visitEnumItem(EnumItemNode *node) = 0;
  virtual std::any visitField(FieldNode *node) = 0;
  virtual std::any visitSignature(SignatureNode *node) = 0;
  virtual std::any visitDeclStmt(DeclStmtNode *node) = 0;
  virtual std::any visitExprStmt(ExprStmtNode *node) = 0;
  virtual std::any visitQualifierLst(QualifierLstNode *node) = 0;
  virtual std::any visitQualifier(QualifierNode *node) = 0;
  virtual std::any visitModAttr(ModAttrNode *node) = 0;
  virtual std::any visitTopLevelDefinitionAttr(TopLevelDefinitionAttrNode *node) = 0;
  virtual std::any visitLambdaAttr(LambdaAttrNode *node) = 0;
  virtual std::any visitAttrLst(AttrLstNode *node) = 0;
  virtual std::any visitAttr(AttrNode *node) = 0;
  virtual std::any visitCaseConstant(CaseConstantNode *node) = 0;
  virtual std::any visitReturnStmt(ReturnStmtNode *node) = 0;
  virtual std::any visitBreakStmt(BreakStmtNode *node) = 0;
  virtual std::any visitContinueStmt(ContinueStmtNode *node) = 0;
  virtual std::any visitFallthroughStmt(FallthroughStmtNode *node) = 0;
  virtual std::any visitAssertStmt(AssertStmtNode *node) = 0;
  virtual std::any visitBuiltinCall(BuiltinCallNode *node) = 0;
  virtual std::any visitPrintfCall(PrintfCallNode *node) = 0;
  virtual std::any visitSizeofCall(SizeofCallNode *node) = 0;
  virtual std::any visitAlignofCall(AlignofCallNode *node) = 0;
  virtual std::any visitTypeidCall(TypeidCallNode *node) = 0;
  virtual std::any visitLenCall(LenCallNode *node) = 0;
  virtual std::any visitPanicCall(PanicCallNode *node) = 0;
  virtual std::any visitSysCall(SysCallNode *node) = 0;
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
  virtual std::any visitConstant(ConstantNode *node) = 0;
  virtual std::any visitFctCall(FctCallNode *node) = 0;
  virtual std::any visitArrayInitialization(ArrayInitializationNode *node) = 0;
  virtual std::any visitStructInstantiation(StructInstantiationNode *node) = 0;
  virtual std::any visitLambdaFunc(LambdaFuncNode *node) = 0;
  virtual std::any visitLambdaProc(LambdaProcNode *node) = 0;
  virtual std::any visitLambdaExpr(LambdaExprNode *node) = 0;
  virtual std::any visitDataType(DataTypeNode *node) = 0;
  virtual std::any visitBaseDataType(BaseDataTypeNode *node) = 0;
  virtual std::any visitCustomDataType(CustomDataTypeNode *node) = 0;
  virtual std::any visitFunctionDataType(FunctionDataTypeNode *node) = 0;
};

} // namespace spice::compiler
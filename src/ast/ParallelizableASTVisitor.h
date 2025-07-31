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
class AssertStmtNode;
class AnonymousBlockStmtNode;
class StmtLstNode;
class TypeLstNode;
class TypeLstWithEllipsisNode;
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
class ImportDefNode;
class ReturnStmtNode;
class BreakStmtNode;
class ContinueStmtNode;
class FallthroughStmtNode;
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

class ParallelizableASTVisitor {
public:
  // Destructor
  virtual ~ParallelizableASTVisitor() = default;

  // General visitor method
  std::any visit(const ASTNode *node);
  std::any visitChildren(const ASTNode *node);

  // Visitor methods
  virtual std::any visitEntry(const EntryNode *node);
  virtual std::any visitMainFctDef(const MainFctDefNode *node);
  virtual std::any visitFctDef(const FctDefNode *node);
  virtual std::any visitProcDef(const ProcDefNode *node);
  virtual std::any visitFctName(const FctNameNode *node);
  virtual std::any visitStructDef(const StructDefNode *node);
  virtual std::any visitInterfaceDef(const InterfaceDefNode *node);
  virtual std::any visitEnumDef(const EnumDefNode *node);
  virtual std::any visitGenericTypeDef(const GenericTypeDefNode *node);
  virtual std::any visitAliasDef(const AliasDefNode *node);
  virtual std::any visitGlobalVarDef(const GlobalVarDefNode *node);
  virtual std::any visitExtDecl(const ExtDeclNode *node);
  virtual std::any visitImportDef(const ImportDefNode *node);
  virtual std::any visitUnsafeBlockDef(const UnsafeBlockNode *node);
  virtual std::any visitForLoop(const ForLoopNode *node);
  virtual std::any visitForeachLoop(const ForeachLoopNode *node);
  virtual std::any visitWhileLoop(const WhileLoopNode *node);
  virtual std::any visitDoWhileLoop(const DoWhileLoopNode *node);
  virtual std::any visitIfStmt(const IfStmtNode *node);
  virtual std::any visitElseStmt(const ElseStmtNode *node);
  virtual std::any visitSwitchStmt(const SwitchStmtNode *node);
  virtual std::any visitCaseBranch(const CaseBranchNode *node);
  virtual std::any visitDefaultBranch(const DefaultBranchNode *node);
  virtual std::any visitAnonymousBlockStmt(const AnonymousBlockStmtNode *node);
  virtual std::any visitStmtLst(const StmtLstNode *node);
  virtual std::any visitTypeLst(const TypeLstNode *node);
  virtual std::any visitTypeLstWithEllipsis(const TypeLstWithEllipsisNode *node);
  virtual std::any visitTypeAltsLst(const TypeAltsLstNode *node);
  virtual std::any visitParamLst(const ParamLstNode *node);
  virtual std::any visitArgLst(const ArgLstNode *node);
  virtual std::any visitEnumItemLst(const EnumItemLstNode *node);
  virtual std::any visitEnumItem(const EnumItemNode *node);
  virtual std::any visitField(const FieldNode *node);
  virtual std::any visitSignature(const SignatureNode *node);
  virtual std::any visitDeclStmt(const DeclStmtNode *node);
  virtual std::any visitExprStmt(const ExprStmtNode *node);
  virtual std::any visitQualifierLst(const QualifierLstNode *node);
  virtual std::any visitQualifier(const QualifierNode *node);
  virtual std::any visitModAttr(const ModAttrNode *node);
  virtual std::any visitTopLevelDefinitionAttr(const TopLevelDefinitionAttrNode *node);
  virtual std::any visitLambdaAttr(const LambdaAttrNode *node);
  virtual std::any visitAttrLst(const AttrLstNode *node);
  virtual std::any visitAttr(const AttrNode *node);
  virtual std::any visitCaseConstant(const CaseConstantNode *node);
  virtual std::any visitReturnStmt(const ReturnStmtNode *node);
  virtual std::any visitBreakStmt(const BreakStmtNode *node);
  virtual std::any visitContinueStmt(const ContinueStmtNode *node);
  virtual std::any visitFallthroughStmt(const FallthroughStmtNode *node);
  virtual std::any visitAssertStmt(const AssertStmtNode *node);
  virtual std::any visitBuiltinCall(const BuiltinCallNode *node);
  virtual std::any visitPrintfCall(const PrintfCallNode *node);
  virtual std::any visitSizeofCall(const SizeofCallNode *node);
  virtual std::any visitAlignofCall(const AlignofCallNode *node);
  virtual std::any visitTypeidCall(const TypeidCallNode *node);
  virtual std::any visitLenCall(const LenCallNode *node);
  virtual std::any visitPanicCall(const PanicCallNode *node);
  virtual std::any visitSysCall(const SysCallNode *node);
  virtual std::any visitAssignExpr(const AssignExprNode *node);
  virtual std::any visitTernaryExpr(const TernaryExprNode *node);
  virtual std::any visitLogicalOrExpr(const LogicalOrExprNode *node);
  virtual std::any visitLogicalAndExpr(const LogicalAndExprNode *node);
  virtual std::any visitBitwiseOrExpr(const BitwiseOrExprNode *node);
  virtual std::any visitBitwiseXorExpr(const BitwiseXorExprNode *node);
  virtual std::any visitBitwiseAndExpr(const BitwiseAndExprNode *node);
  virtual std::any visitEqualityExpr(const EqualityExprNode *node);
  virtual std::any visitRelationalExpr(const RelationalExprNode *node);
  virtual std::any visitShiftExpr(const ShiftExprNode *node);
  virtual std::any visitAdditiveExpr(const AdditiveExprNode *node);
  virtual std::any visitMultiplicativeExpr(const MultiplicativeExprNode *node);
  virtual std::any visitCastExpr(const CastExprNode *node);
  virtual std::any visitPrefixUnaryExpr(const PrefixUnaryExprNode *node);
  virtual std::any visitPostfixUnaryExpr(const PostfixUnaryExprNode *node);
  virtual std::any visitAtomicExpr(const AtomicExprNode *node);
  virtual std::any visitValue(const ValueNode *node);
  virtual std::any visitConstant(const ConstantNode *node);
  virtual std::any visitFctCall(const FctCallNode *node);
  virtual std::any visitArrayInitialization(const ArrayInitializationNode *node);
  virtual std::any visitStructInstantiation(const StructInstantiationNode *node);
  virtual std::any visitLambdaFunc(const LambdaFuncNode *node);
  virtual std::any visitLambdaProc(const LambdaProcNode *node);
  virtual std::any visitLambdaExpr(const LambdaExprNode *node);
  virtual std::any visitDataType(const DataTypeNode *node);
  virtual std::any visitBaseDataType(const BaseDataTypeNode *node);
  virtual std::any visitCustomDataType(const CustomDataTypeNode *node);
  virtual std::any visitFunctionDataType(const FunctionDataTypeNode *node);
};

} // namespace spice::compiler
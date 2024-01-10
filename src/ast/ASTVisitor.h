// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <ast/AbstractASTVisitor.h>

namespace spice::compiler {

class ASTVisitor : public AbstractASTVisitor {
public:
  // Visitor methods
  std::any visitEntry(EntryNode *node) override;
  std::any visitMainFctDef(MainFctDefNode *node) override;
  std::any visitFctDef(FctDefNode *node) override;
  std::any visitProcDef(ProcDefNode *node) override;
  std::any visitFctName(FctNameNode *node) override;
  std::any visitStructDef(StructDefNode *node) override;
  std::any visitInterfaceDef(InterfaceDefNode *node) override;
  std::any visitEnumDef(EnumDefNode *node) override;
  std::any visitGenericTypeDef(GenericTypeDefNode *node) override;
  std::any visitAliasDef(AliasDefNode *node) override;
  std::any visitGlobalVarDef(GlobalVarDefNode *node) override;
  std::any visitExtDecl(ExtDeclNode *node) override;
  std::any visitUnsafeBlock(UnsafeBlockNode *node) override;
  std::any visitForLoop(ForLoopNode *node) override;
  std::any visitForeachLoop(ForeachLoopNode *node) override;
  std::any visitWhileLoop(WhileLoopNode *node) override;
  std::any visitDoWhileLoop(DoWhileLoopNode *node) override;
  std::any visitIfStmt(IfStmtNode *node) override;
  std::any visitElseStmt(ElseStmtNode *node) override;
  std::any visitSwitchStmt(SwitchStmtNode *node) override;
  std::any visitCaseBranch(CaseBranchNode *node) override;
  std::any visitDefaultBranch(DefaultBranchNode *node) override;
  std::any visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) override;
  std::any visitStmtLst(StmtLstNode *node) override;
  std::any visitTypeLst(TypeLstNode *node) override;
  std::any visitTypeAltsLst(TypeAltsLstNode *node) override;
  std::any visitParamLst(ParamLstNode *node) override;
  std::any visitArgLst(ArgLstNode *node) override;
  std::any visitEnumItemLst(EnumItemLstNode *node) override;
  std::any visitEnumItem(EnumItemNode *node) override;
  std::any visitField(FieldNode *node) override;
  std::any visitSignature(SignatureNode *node) override;
  std::any visitStmt(StmtNode *node) override;
  std::any visitDeclStmt(DeclStmtNode *node) override;
  std::any visitSpecifierLst(SpecifierLstNode *node) override;
  std::any visitSpecifier(SpecifierNode *node) override;
  std::any visitModAttr(ModAttrNode *node) override;
  std::any visitTopLevelDefinitionAttr(TopLevelDefinitionAttrNode *node) override;
  std::any visitLambdaAttr(LambdaAttrNode *node) override;
  std::any visitAttrLst(AttrLstNode *node) override;
  std::any visitAttr(AttrNode *node) override;
  std::any visitConstantLst(ConstantLstNode *node) override;
  std::any visitImportStmt(ImportStmtNode *node) override;
  std::any visitReturnStmt(ReturnStmtNode *node) override;
  std::any visitBreakStmt(BreakStmtNode *node) override;
  std::any visitContinueStmt(ContinueStmtNode *node) override;
  std::any visitFallthroughStmt(FallthroughStmtNode *node) override;
  std::any visitAssertStmt(AssertStmtNode *node) override;
  std::any visitPrintfCall(PrintfCallNode *node) override;
  std::any visitSizeofCall(SizeofCallNode *node) override;
  std::any visitAlignofCall(AlignofCallNode *node) override;
  std::any visitLenCall(LenCallNode *node) override;
  std::any visitPanicCall(PanicCallNode *node) override;
  std::any visitAssignExpr(AssignExprNode *node) override;
  std::any visitTernaryExpr(TernaryExprNode *node) override;
  std::any visitLogicalOrExpr(LogicalOrExprNode *node) override;
  std::any visitLogicalAndExpr(LogicalAndExprNode *node) override;
  std::any visitBitwiseOrExpr(BitwiseOrExprNode *node) override;
  std::any visitBitwiseXorExpr(BitwiseXorExprNode *node) override;
  std::any visitBitwiseAndExpr(BitwiseAndExprNode *node) override;
  std::any visitEqualityExpr(EqualityExprNode *node) override;
  std::any visitRelationalExpr(RelationalExprNode *node) override;
  std::any visitShiftExpr(ShiftExprNode *node) override;
  std::any visitAdditiveExpr(AdditiveExprNode *node) override;
  std::any visitMultiplicativeExpr(MultiplicativeExprNode *node) override;
  std::any visitCastExpr(CastExprNode *node) override;
  std::any visitPrefixUnaryExpr(PrefixUnaryExprNode *node) override;
  std::any visitPostfixUnaryExpr(PostfixUnaryExprNode *node) override;
  std::any visitAtomicExpr(AtomicExprNode *node) override;
  std::any visitValue(ValueNode *node) override;
  std::any visitConstant(ConstantNode *node) override;
  std::any visitFctCall(FctCallNode *node) override;
  std::any visitArrayInitialization(ArrayInitializationNode *node) override;
  std::any visitStructInstantiation(StructInstantiationNode *node) override;
  std::any visitLambdaFunc(LambdaFuncNode *node) override;
  std::any visitLambdaProc(LambdaProcNode *node) override;
  std::any visitLambdaExpr(LambdaExprNode *node) override;
  std::any visitDataType(DataTypeNode *node) override;
  std::any visitBaseDataType(BaseDataTypeNode *node) override;
  std::any visitCustomDataType(CustomDataTypeNode *node) override;
  std::any visitFunctionDataType(FunctionDataTypeNode *node) override;
};

} // namespace spice::compiler
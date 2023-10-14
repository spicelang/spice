// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "ASTVisitor.h"

#include <ast/ASTNodes.h>

namespace spice::compiler {

std::any ASTVisitor::visitEntry(EntryNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitMainFctDef(MainFctDefNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitFctDef(FctDefNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitProcDef(ProcDefNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitFctName(FctNameNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitStructDef(StructDefNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitInterfaceDef(InterfaceDefNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitEnumDef(EnumDefNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitGenericTypeDef(GenericTypeDefNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitAliasDef(AliasDefNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitGlobalVarDef(GlobalVarDefNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitExtDecl(ExtDeclNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitUnsafeBlock(UnsafeBlockNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitForLoop(ForLoopNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitForeachLoop(ForeachLoopNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitWhileLoop(WhileLoopNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitDoWhileLoop(DoWhileLoopNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitIfStmt(IfStmtNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitElseStmt(ElseStmtNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitStmtLst(StmtLstNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitTypeLst(TypeLstNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitTypeAltsLst(TypeAltsLstNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitParamLst(ParamLstNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitArgLst(ArgLstNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitEnumItemLst(EnumItemLstNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitEnumItem(EnumItemNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitField(FieldNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitSignature(SignatureNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitStmt(StmtNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitDeclStmt(DeclStmtNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitSpecifierLst(SpecifierLstNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitSpecifier(SpecifierNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitModAttr(ModAttrNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitTopLevelDefinitionAttr(TopLevelDefinitionAttrNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitAttrLst(AttrLstNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitAttr(AttrNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitImportStmt(ImportStmtNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitReturnStmt(ReturnStmtNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitBreakStmt(BreakStmtNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitContinueStmt(ContinueStmtNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitAssertStmt(AssertStmtNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitPrintfCall(PrintfCallNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitSizeofCall(SizeofCallNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitAlignofCall(AlignofCallNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitLenCall(LenCallNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitPanicCall(PanicCallNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitAssignExpr(AssignExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitTernaryExpr(TernaryExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitLogicalOrExpr(LogicalOrExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitLogicalAndExpr(LogicalAndExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitBitwiseOrExpr(BitwiseOrExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitBitwiseXorExpr(BitwiseXorExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitBitwiseAndExpr(BitwiseAndExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitEqualityExpr(EqualityExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitRelationalExpr(RelationalExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitShiftExpr(ShiftExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitAdditiveExpr(AdditiveExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitMultiplicativeExpr(MultiplicativeExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitCastExpr(CastExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitPrefixUnaryExpr(PrefixUnaryExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitPostfixUnaryExpr(PostfixUnaryExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitAtomicExpr(AtomicExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitValue(ValueNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitConstant(ConstantNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitFctCall(FctCallNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitArrayInitialization(ArrayInitializationNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitStructInstantiation(StructInstantiationNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitLambdaFunc(LambdaFuncNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitLambdaProc(LambdaProcNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitLambdaExpr(LambdaExprNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitDataType(DataTypeNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitBaseDataType(BaseDataTypeNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitCustomDataType(CustomDataTypeNode *node) { return visitChildren(node); }

std::any ASTVisitor::visitFunctionDataType(FunctionDataTypeNode *node) { return visitChildren(node); }

} // namespace spice::compiler
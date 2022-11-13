// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ASTVisitor.h"

template <typename T> T ASTVisitor<T>::visitEntry(EntryNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitMainFctDef(MainFctDefNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitFctDef(FctDefNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitProcDef(ProcDefNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitStructDef(StructDefNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitInterfaceDef(InterfaceDefNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitEnumDef(EnumDefNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitGenericTypeDef(GenericTypeDefNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitGlobalVarDef(GlobalVarDefNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitExtDecl(ExtDeclNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitThreadDef(ThreadDefNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitUnsafeBlockDef(UnsafeBlockDefNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitForLoop(ForLoopNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitForeachLoop(ForeachLoopNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitWhileLoop(WhileLoopNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitIfStmt(IfStmtNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitElseStmt(ElseStmtNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitAssertStmt(AssertStmtNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitStmtLst(StmtLstNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitTypeLst(TypeLstNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitTypeAltsLst(TypeAltsLstNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitParamLst(ParamLstNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitArgLst(ArgLstNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitEnumItemLst(EnumItemLstNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitEnumItem(EnumItemNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitField(FieldNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitSignature(SignatureNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitStmt(StmtNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitDeclStmt(DeclStmtNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitSpecifierLst(SpecifierLstNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitSpecifier(SpecifierNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitImportStmt(ImportStmtNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitReturnStmt(ReturnStmtNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitBreakStmt(BreakStmtNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitContinueStmt(ContinueStmtNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitPrintfCall(PrintfCallNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitSizeofCall(SizeofCallNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitLenCall(LenCallNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitTidCall(TidCallNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitJoinCall(JoinCallNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitAssignExpr(AssignExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitTernaryExpr(TernaryExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitLogicalOrExpr(LogicalOrExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitLogicalAndExpr(LogicalAndExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitBitwiseOrExpr(BitwiseOrExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitBitwiseXorExpr(BitwiseXorExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitBitwiseAndExpr(BitwiseAndExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitEqualityExpr(EqualityExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitRelationalExpr(RelationalExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitShiftExpr(ShiftExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitAdditiveExpr(AdditiveExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitMultiplicativeExpr(MultiplicativeExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitCastExpr(CastExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitPrefixUnaryExpr(PrefixUnaryExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitPostfixUnaryExpr(PostfixUnaryExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitAtomicExpr(AtomicExprNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitValue(ValueNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitPrimitiveValue(PrimitiveValueNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitFunctionCall(FunctionCallNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitArrayInitialization(ArrayInitializationNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitStructInstantiation(StructInstantiationNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitDataType(DataTypeNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitBaseDataType(BaseDataTypeNode *node) { return visitChildren<T>(node); }

template <typename T> T ASTVisitor<T>::visitCustomDataType(CustomDataTypeNode *node) { return visitChildren<T>(node); }
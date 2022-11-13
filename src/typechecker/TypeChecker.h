// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include "model/Function.h"
#include <CompilerPass.h>
#include <ast/ASTVisitor.h>
#include <symboltablebuilder/SymbolTableEntry.h>
#include <typechecker/OpRuleManager.h>

#define TCResult SymbolType

enum TypeCheckerMode { TC_MODE_LOOKUP, TC_MODE_ANALYZE };

/**
 * Jobs:
 * - Ensure that all actual types match the expected types
 * - Perform type inference
 */
class TypeChecker : private CompilerPass, public ASTVisitor<TCResult> {
public:
  // Constructors
  TypeChecker(GlobalResourceManager &resourceManager, SourceFile *sourceFile, TypeCheckerMode typeCheckerMode);

  // Friend classes
  friend class OpRuleManager;

  // Public methods
  TCResult visitEntry(EntryNode *node) override;
  TCResult visitMainFctDef(MainFctDefNode *node) override;
  TCResult visitMainFctDefAnalyze(MainFctDefNode *node);
  TCResult visitFctDef(FctDefNode *node) override;
  TCResult visitFctDefLookup(FctDefNode *node);
  TCResult visitFctDefAnalyze(FctDefNode *node);
  TCResult visitProcDef(ProcDefNode *node) override;
  TCResult visitProcDefLookup(ProcDefNode *node);
  TCResult visitProcDefAnalyze(ProcDefNode *node);
  TCResult visitStructDef(StructDefNode *node) override;
  TCResult visitStructDefLookup(StructDefNode *node);
  TCResult visitStructDefAnalyze(StructDefNode *node);
  TCResult visitInterfaceDef(InterfaceDefNode *node) override;
  TCResult visitEnumDef(EnumDefNode *node) override;
  TCResult visitGenericTypeDef(GenericTypeDefNode *node) override;
  TCResult visitGlobalVarDef(GlobalVarDefNode *node) override;
  TCResult visitExtDecl(ExtDeclNode *node) override;
  TCResult visitThreadDef(ThreadDefNode *node) override;
  TCResult visitUnsafeBlockDef(UnsafeBlockDefNode *node) override;
  TCResult visitForLoop(ForLoopNode *node) override;
  TCResult visitForeachLoop(ForeachLoopNode *node) override;
  TCResult visitWhileLoop(WhileLoopNode *node) override;
  TCResult visitIfStmt(IfStmtNode *node) override;
  TCResult visitElseStmt(ElseStmtNode *node) override;
  TCResult visitAssertStmt(AssertStmtNode *node) override;
  TCResult visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) override;
  TCResult visitParamLst(ParamLstNode *node) override;
  TCResult visitSignature(SignatureNode *node) override;
  TCResult visitDeclStmt(DeclStmtNode *node) override;
  TCResult visitReturnStmt(ReturnStmtNode *node) override;
  TCResult visitBreakStmt(BreakStmtNode *node) override;
  TCResult visitContinueStmt(ContinueStmtNode *node) override;
  TCResult visitPrintfCall(PrintfCallNode *node) override;
  TCResult visitSizeofCall(SizeofCallNode *node) override;
  TCResult visitLenCall(LenCallNode *node) override;
  TCResult visitTidCall(TidCallNode *node) override;
  TCResult visitJoinCall(JoinCallNode *node) override;
  TCResult visitAssignExpr(AssignExprNode *node) override;
  TCResult visitTernaryExpr(TernaryExprNode *node) override;
  TCResult visitLogicalOrExpr(LogicalOrExprNode *node) override;
  TCResult visitLogicalAndExpr(LogicalAndExprNode *node) override;
  TCResult visitBitwiseOrExpr(BitwiseOrExprNode *node) override;
  TCResult visitBitwiseXorExpr(BitwiseXorExprNode *node) override;
  TCResult visitBitwiseAndExpr(BitwiseAndExprNode *node) override;
  TCResult visitEqualityExpr(EqualityExprNode *node) override;
  TCResult visitRelationalExpr(RelationalExprNode *node) override;
  TCResult visitShiftExpr(ShiftExprNode *node) override;
  TCResult visitAdditiveExpr(AdditiveExprNode *node) override;
  TCResult visitMultiplicativeExpr(MultiplicativeExprNode *node) override;
  TCResult visitCastExpr(CastExprNode *node) override;
  TCResult visitPrefixUnaryExpr(PrefixUnaryExprNode *node) override;
  TCResult visitPostfixUnaryExpr(PostfixUnaryExprNode *node) override;
  TCResult visitAtomicExpr(AtomicExprNode *node) override;
  TCResult visitValue(ValueNode *node) override;
  TCResult visitPrimitiveValue(PrimitiveValueNode *node) override;
  TCResult visitFunctionCall(FunctionCallNode *node) override;
  TCResult visitArrayInitialization(ArrayInitializationNode *node) override;
  TCResult visitStructInstantiation(StructInstantiationNode *node) override;
  TCResult visitDataType(DataTypeNode *node) override;
  TCResult visitBaseDataType(BaseDataTypeNode *node) override;
  TCResult visitCustomDataType(CustomDataTypeNode *node) override;

  // Public members
  bool reVisitRequested = false;

private:
  // Private members
  OpRuleManager opRuleManager = OpRuleManager(this);
  Scope *rootScope;
  Scope *currentScope = nullptr;
  const TypeCheckerMode typeCheckerMode;
  NamedParamList namedParamList;

  // Private methods
  void insertAnonStringStructSymbol(const ASTNode *declNode);
  void insertDestructorCall(const ASTNode *node, const SymbolTableEntry *varEntry);
  void insertEmptyConstructorCall(const ASTNode *node, const SymbolTableEntry *varEntry);
  void insertStructMethodCall(const ASTNode *node, const SymbolTableEntry *varEntry, const std::string &name);
};
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>
#include <model/Function.h>
#include <symboltablebuilder/SymbolTableEntry.h>
#include <typechecker/OpRuleManager.h>

enum TypeCheckerMode { TC_MODE_PREPARE, TC_MODE_CHECK };

// For routing through the symbol type as well as the current variable entry
struct ExprResult {
  SymbolType type;
  SymbolTableEntry *entry = nullptr;
};

/**
 * Jobs:
 * - Ensure that all actual types match the expected types
 * - Perform type inference
 */
class TypeChecker : private CompilerPass, public ASTVisitor {
public:
  // Constructors
  TypeChecker(GlobalResourceManager &resourceManager, SourceFile *sourceFile, TypeCheckerMode typeCheckerMode);

  // Friend classes
  friend class OpRuleManager;

  // Public methods
  std::any visitEntry(EntryNode *node) override;
  std::any visitMainFctDef(MainFctDefNode *node) override;
  std::any visitMainFctDefCheck(MainFctDefNode *node);
  std::any visitFctDef(FctDefNode *node) override;
  std::any visitFctDefPrepare(FctDefNode *node);
  std::any visitFctDefCheck(FctDefNode *node);
  std::any visitProcDef(ProcDefNode *node) override;
  std::any visitProcDefPrepare(ProcDefNode *node);
  std::any visitProcDefCheck(ProcDefNode *node);
  std::any visitStructDef(StructDefNode *node) override;
  std::any visitStructDefPrepare(StructDefNode *node);
  std::any visitStructDefCheck(StructDefNode *node);
  std::any visitInterfaceDef(InterfaceDefNode *node) override;
  std::any visitInterfaceDefPrepare(InterfaceDefNode *node);
  std::any visitEnumDef(EnumDefNode *node) override;
  std::any visitEnumDefPrepare(EnumDefNode *node);
  std::any visitGenericTypeDef(GenericTypeDefNode *node) override;
  std::any visitGenericTypeDefPrepare(GenericTypeDefNode *node);
  std::any visitGlobalVarDef(GlobalVarDefNode *node) override;
  std::any visitGlobalVarDefPrepare(GlobalVarDefNode *node);
  std::any visitExtDecl(ExtDeclNode *node) override;
  std::any visitExtDeclPrepare(ExtDeclNode *node);
  std::any visitThreadDef(ThreadDefNode *node) override;
  std::any visitUnsafeBlockDef(UnsafeBlockDefNode *node) override;
  std::any visitForLoop(ForLoopNode *node) override;
  std::any visitForeachLoop(ForeachLoopNode *node) override;
  std::any visitWhileLoop(WhileLoopNode *node) override;
  std::any visitIfStmt(IfStmtNode *node) override;
  std::any visitElseStmt(ElseStmtNode *node) override;
  std::any visitAssertStmt(AssertStmtNode *node) override;
  std::any visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) override;
  std::any visitParamLst(ParamLstNode *node) override;
  std::any visitField(FieldNode *node) override;
  std::any visitSignature(SignatureNode *node) override;
  std::any visitDeclStmt(DeclStmtNode *node) override;
  std::any visitReturnStmt(ReturnStmtNode *node) override;
  std::any visitBreakStmt(BreakStmtNode *node) override;
  std::any visitContinueStmt(ContinueStmtNode *node) override;
  std::any visitPrintfCall(PrintfCallNode *node) override;
  std::any visitSizeofCall(SizeofCallNode *node) override;
  std::any visitLenCall(LenCallNode *node) override;
  std::any visitTidCall(TidCallNode *node) override;
  std::any visitJoinCall(JoinCallNode *node) override;
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
  std::any visitFunctionCall(FunctionCallNode *node) override;
  std::any visitArrayInitialization(ArrayInitializationNode *node) override;
  std::any visitStructInstantiation(StructInstantiationNode *node) override;
  std::any visitDataType(DataTypeNode *node) override;
  std::any visitBaseDataType(BaseDataTypeNode *node) override;
  std::any visitCustomDataType(CustomDataTypeNode *node) override;

  // Public members
  bool reVisitRequested = false;

private:
  // Private members
  OpRuleManager opRuleManager = OpRuleManager(this);
  Scope *rootScope;
  Scope *currentScope = nullptr;
  Scope *accessScope = nullptr;
  const TypeCheckerMode typeCheckerMode;

  // Private methods
  /*void insertAnonStringStructSymbol(const ASTNode *declNode);
  void insertDestructorCall(const ASTNode *node, const SymbolTableEntry *varEntry);
  void insertEmptyConstructorCall(const ASTNode *node, const SymbolTableEntry *varEntry);
  void insertStructMethodCall(const ASTNode *node, const SymbolTableEntry *varEntry, const std::string &name);*/
};
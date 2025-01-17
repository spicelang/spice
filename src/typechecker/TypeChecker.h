// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>
#include <model/Function.h>
#include <symboltablebuilder/Scope.h>
#include <typechecker/FunctionManager.h>
#include <typechecker/OpRuleManager.h>
#include <util/CompilerWarning.h>

namespace spice::compiler {

// Forward declarations
class LambdaBaseNode;

enum TypeCheckerMode : bool {
  TC_MODE_PRE,
  TC_MODE_POST,
};

/**
 * Jobs:
 * - Ensure that all actual types match the expected types
 * - Perform type inference
 */
class TypeChecker final : CompilerPass, public ASTVisitor {
public:
  // Constructors
  TypeChecker(GlobalResourceManager &resourceManager, SourceFile *sourceFile, TypeCheckerMode typeCheckerMode);

  // Friend classes
  friend class OpRuleManager;
  friend class FunctionManager;
  friend class StructManager;

  // Visitor methods
  // Top level definitions
  std::any visitEntry(EntryNode *node) override;
  std::any visitMainFctDef(MainFctDefNode *node) override;
  std::any visitMainFctDefPrepare(MainFctDefNode *node);
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
  std::any visitAliasDef(AliasDefNode *node) override;
  std::any visitAliasDefPrepare(AliasDefNode *node);
  std::any visitGlobalVarDef(GlobalVarDefNode *node) override;
  std::any visitGlobalVarDefPrepare(GlobalVarDefNode *node);
  std::any visitExtDecl(ExtDeclNode *node) override;
  std::any visitExtDeclPrepare(ExtDeclNode *node);
  std::any visitImportDef(ImportDefNode *node) override;
  std::any visitImportDefPrepare(ImportDefNode *node);
  // Control structures
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
  std::any visitAssertStmt(AssertStmtNode *node) override;
  std::any visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) override;
  // Statements
  std::any visitStmtLst(StmtLstNode *node) override;
  std::any visitParamLst(ParamLstNode *node) override;
  std::any visitField(FieldNode *node) override;
  std::any visitSignature(SignatureNode *node) override;
  std::any visitDeclStmt(DeclStmtNode *node) override;
  std::any visitCaseConstant(CaseConstantNode *node) override;
  std::any visitReturnStmt(ReturnStmtNode *node) override;
  std::any visitBreakStmt(BreakStmtNode *node) override;
  std::any visitContinueStmt(ContinueStmtNode *node) override;
  std::any visitFallthroughStmt(FallthroughStmtNode *node) override;
  // Builtin functions
  std::any visitBuiltinCall(BuiltinCallNode *node) override;
  std::any visitPrintfCall(PrintfCallNode *node) override;
  std::any visitSizeofCall(SizeofCallNode *node) override;
  std::any visitAlignofCall(AlignofCallNode *node) override;
  std::any visitLenCall(LenCallNode *node) override;
  std::any visitPanicCall(PanicCallNode *node) override;
  std::any visitSysCall(SysCallNode *node) override;
  // Expressions
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
  // Values and types
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

private:
  // Private members
  OpRuleManager opRuleManager = OpRuleManager(this);
  Scope *accessScope = nullptr;
  const TypeCheckerMode typeCheckerMode;
  std::vector<CompilerWarning> &warnings;
  TypeMapping typeMapping;
  bool typeCheckedMainFct = false;

  // Private methods
  bool visitOrdinaryFctCall(FctCallNode *node, QualTypeList &templateTypes, std::string fqFunctionName);
  bool visitFctPtrCall(const FctCallNode *node, const QualType &functionType) const;
  bool visitMethodCall(FctCallNode *node, Scope *structScope, QualTypeList &templateTypes);
  bool checkAsyncLambdaCaptureRules(const LambdaBaseNode *node, const LambdaAttrNode *attrs) const;
  [[nodiscard]] Function *matchCopyCtor(const QualType& thisType, const ASTNode* node);
  [[nodiscard]] QualType mapLocalTypeToImportedScopeType(const Scope *targetScope, const QualType &symbolType) const;
  [[nodiscard]] QualType mapImportedScopeTypeToLocalType(const Scope *sourceScope, const QualType &symbolType) const;
  static void autoDeReference(QualType &symbolType);
  std::vector<const Function *> &getOpFctPointers(ASTNode *node) const;
  void requestRevisitIfRequired(const Function *fct) const;
  void softError(const ASTNode *node, SemanticErrorType errorType, const std::string &message) const;

  // Implicit code generation
  void createDefaultStructMethod(const Struct &spiceStruct, const std::string &name, const ParamList &params) const;
  void createDefaultCtorIfRequired(const Struct &spiceStruct, Scope *structScope);
  void createDefaultCopyCtorIfRequired(const Struct &spiceStruct, Scope *structScope);
  void createDefaultDtorIfRequired(const Struct &spiceStruct, Scope *structScope);
  void createCtorBodyPreamble(const Scope *bodyScope);
  void createCopyCtorBodyPreamble(const Scope *bodyScope);
  void createDtorBodyPreamble(const Scope *bodyScope);
  Function *implicitlyCallStructMethod(const SymbolTableEntry *entry, const std::string &methodName, const ArgList &args,
                                       const ASTNode *node);
  void implicitlyCallStructCopyCtor(const SymbolTableEntry *entry, const ASTNode *node);
  void implicitlyCallStructDtor(SymbolTableEntry *entry, StmtLstNode *node);
  void implicitlyCallDeallocate(const ASTNode *node);
  void doScopeCleanup(StmtLstNode *node);
};

} // namespace spice::compiler
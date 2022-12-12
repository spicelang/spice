// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ParallelizableASTVisitor.h>
#include <irgenerator/DebugInfoGenerator.h>
#include <irgenerator/OpRuleConversionsManager.h>
#include <irgenerator/StdFunctionManager.h>

// Forward declarations
class SourceFile;

class IRGenerator : private CompilerPass, public ParallelizableASTVisitor {
private:
  // For routing through the symbol type as well as the current variable entry
  struct ExprResult {
    llvm::Value *ptr = nullptr;
    llvm::Value *value = nullptr;
    llvm::Constant *constant = nullptr;
    SymbolTableEntry *entry = nullptr;
  };

public:
  // Constructors
  IRGenerator(GlobalResourceManager &resourceManager, SourceFile *sourceFile);

  // Friend classes
  friend class StdFunctionManager;
  friend class OpRuleConversionsManager;
  friend class DebugInfoGenerator;

  // Visitor methods
  // Top level definitions
  std::any visitEntry(const EntryNode *node) override;
  std::any visitMainFctDef(const MainFctDefNode *node) override;
  std::any visitFctDef(const FctDefNode *node) override;
  std::any visitProcDef(const ProcDefNode *node) override;
  std::any visitStructDef(const StructDefNode *node) override;
  std::any visitInterfaceDef(const InterfaceDefNode *node) override;
  std::any visitEnumDef(const EnumDefNode *node) override;
  std::any visitGenericTypeDef(const GenericTypeDefNode *node) override;
  std::any visitGlobalVarDef(const GlobalVarDefNode *node) override;
  std::any visitExtDecl(const ExtDeclNode *node) override;
  // Control structures
  std::any visitThreadDef(const ThreadDefNode *node) override;
  std::any visitUnsafeBlockDef(const UnsafeBlockDefNode *node) override;
  std::any visitForLoop(const ForLoopNode *node) override;
  std::any visitForeachLoop(const ForeachLoopNode *node) override;
  std::any visitWhileLoop(const WhileLoopNode *node) override;
  std::any visitIfStmt(const IfStmtNode *node) override;
  std::any visitElseStmt(const ElseStmtNode *node) override;
  std::any visitAssertStmt(const AssertStmtNode *node) override;
  std::any visitAnonymousBlockStmt(const AnonymousBlockStmtNode *node) override;
  // Statements
  std::any visitStmtLst(const StmtLstNode *node) override;
  std::any visitTypeAltsLst(const TypeAltsLstNode *node) override;
  std::any visitDeclStmt(const DeclStmtNode *node) override;
  std::any visitSpecifierLst(const SpecifierLstNode *node) override;
  std::any visitReturnStmt(const ReturnStmtNode *node) override;
  std::any visitBreakStmt(const BreakStmtNode *node) override;
  std::any visitContinueStmt(const ContinueStmtNode *node) override;
  // Builtin functions
  std::any visitPrintfCall(const PrintfCallNode *node) override;
  std::any visitSizeofCall(const SizeofCallNode *node) override;
  std::any visitLenCall(const LenCallNode *node) override;
  std::any visitTidCall(const TidCallNode *node) override;
  std::any visitJoinCall(const JoinCallNode *node) override;
  // Expressions
  std::any visitAssignExpr(const AssignExprNode *node) override;
  std::any visitTernaryExpr(const TernaryExprNode *node) override;
  std::any visitLogicalOrExpr(const LogicalOrExprNode *node) override;
  std::any visitLogicalAndExpr(const LogicalAndExprNode *node) override;
  std::any visitBitwiseOrExpr(const BitwiseOrExprNode *node) override;
  std::any visitBitwiseXorExpr(const BitwiseXorExprNode *node) override;
  std::any visitBitwiseAndExpr(const BitwiseAndExprNode *node) override;
  std::any visitEqualityExpr(const EqualityExprNode *node) override;
  std::any visitRelationalExpr(const RelationalExprNode *node) override;
  std::any visitShiftExpr(const ShiftExprNode *node) override;
  std::any visitAdditiveExpr(const AdditiveExprNode *node) override;
  std::any visitMultiplicativeExpr(const MultiplicativeExprNode *node) override;
  std::any visitCastExpr(const CastExprNode *node) override;
  std::any visitPrefixUnaryExpr(const PrefixUnaryExprNode *node) override;
  std::any visitPostfixUnaryExpr(const PostfixUnaryExprNode *node) override;
  std::any visitAtomicExpr(const AtomicExprNode *node) override;
  // Values and types
  std::any visitValue(const ValueNode *node) override;
  std::any visitConstant(const ConstantNode *node) override;
  std::any visitFunctionCall(const FunctionCallNode *node) override;
  std::any visitArrayInitialization(const ArrayInitializationNode *node) override;
  std::any visitStructInstantiation(const StructInstantiationNode *node) override;
  std::any visitDataType(const DataTypeNode *node) override;

  // Public methods
  llvm::Value *insertAlloca(llvm::Type *llvmType, const std::string &varName = "");
  [[nodiscard]] std::string getIRString() const;
  void dumpIR() const;

private:
  // Private members
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  llvm::Module *module;
  OpRuleConversionsManager conversionManager = OpRuleConversionsManager(resourceManager, this);
  const StdFunctionManager stdFunctionManager;
  DebugInfoGenerator diGenerator = DebugInfoGenerator(this);
  Scope *currentScope;
  llvm::BasicBlock *allocaInsertBlock = nullptr;
  llvm::Instruction *allocaInsertInst = nullptr;
};
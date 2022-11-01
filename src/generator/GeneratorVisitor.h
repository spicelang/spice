// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <regex>

#include <ast/ASTNodes.h>
#include <ast/ASTVisitor.h>

#include <irgenerator/OpRuleConversionsManager.h>
#include <irgenerator/StdFunctionManager.h>
#include <symbol/ScopePath.h>
#include <symbol/SymbolType.h>

#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Passes/OptimizationLevel.h>
#include <llvm/Target/TargetMachine.h>

// Forward declarations
class SourceFile;
class GlobalResourceManager;
struct CliOptions;
class SymbolTable;
class SymbolTableEntry;
class Function;
class Struct;

/**
 * Visitor for generating a source file.
 *
 * Jobs:
 * - LLVM code gen
 * - Optimization
 * - Emitting object files
 */
class GeneratorVisitor : public ASTVisitor {
public:
  // Constructors
  explicit GeneratorVisitor(GlobalResourceManager &resourceManager, SourceFile &sourceFile, const std::string &objectFile);

  // Friend classes
  friend class StdFunctionManager;
  friend class OpRuleConversionsManager;

  // Public methods
  void optimize();
  void emit();
  void dumpIR();
  std::string getIRString();
  void dumpAsm();
  std::any visitEntry(EntryNode *node) override;
  std::any visitMainFctDef(MainFctDefNode *node) override;
  std::any visitFctDef(FctDefNode *node) override;
  std::any visitProcDef(ProcDefNode *node) override;
  std::any visitStructDef(StructDefNode *node) override;
  std::any visitInterfaceDef(InterfaceDefNode *node) override;
  std::any visitEnumDef(EnumDefNode *node) override;
  std::any visitGlobalVarDef(GlobalVarDefNode *node) override;
  std::any visitExtDecl(ExtDeclNode *node) override;
  std::any visitThreadDef(ThreadDefNode *node) override;
  std::any visitUnsafeBlockDef(UnsafeBlockDefNode *node) override;
  std::any visitForLoop(ForLoopNode *node) override;
  std::any visitForeachLoop(ForeachLoopNode *node) override;
  std::any visitWhileLoop(WhileLoopNode *node) override;
  std::any visitStmtLst(StmtLstNode *node) override;
  std::any visitTypeAltsLst(TypeAltsLstNode *node) override;
  std::any visitIfStmt(IfStmtNode *node) override;
  std::any visitElseStmt(ElseStmtNode *node) override;
  std::any visitAssertStmt(AssertStmtNode *node) override;
  std::any visitDeclStmt(DeclStmtNode *node) override;
  std::any visitImportStmt(ImportStmtNode *node) override;
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
  std::any visitPrimitiveValue(PrimitiveValueNode *node) override;
  std::any visitFunctionCall(FunctionCallNode *node) override;
  std::any visitArrayInitialization(ArrayInitializationNode *node) override;
  std::any visitStructInstantiation(StructInstantiationNode *node) override;
  std::any visitDataType(DataTypeNode *node) override;

private:
  // Members
  GlobalResourceManager &resourceManager;
  const CliOptions &cliOptions;
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  std::unique_ptr<StdFunctionManager> stdFunctionManager;
  std::unique_ptr<OpRuleConversionsManager> conversionsManager;
  const std::string &objectFile;
  llvm::TargetMachine *targetMachine;
  SourceFile &sourceFile;
  bool requiresMainFct = true;
  std::unique_ptr<llvm::Module> module;
  std::unique_ptr<llvm::DIBuilder> diBuilder;
  SymbolTable *currentScope;
  SymbolTable *rootScope;
  SymbolType currentSymbolType = SymbolType(TY_INVALID);
  llvm::Value *currentThisValuePtr = nullptr;
  ScopePath scopePath;
  bool blockAlreadyTerminated = false;
  llvm::BasicBlock *allocaInsertBlock = nullptr;
  llvm::Instruction *allocaInsertInst = nullptr;
  std::stack<llvm::BasicBlock *> breakBlocks;
  std::stack<llvm::BasicBlock *> continueBlocks;
  bool withinConstantArray = false;
  bool allArgsHardcoded = true;
  llvm::Constant *currentConstValue = nullptr;
  bool currentConstSigned = true;
  std::string currentVarName;
  std::string lhsVarName;
  llvm::Type *lhsType = nullptr;
  llvm::Value *structAccessAddress = nullptr;
  llvm::Type *structAccessType = nullptr;
  llvm::Value *dynamicArraySize = nullptr;
  llvm::Value *stackState = nullptr;
  SymbolType arraySymbolType = SymbolType(TY_INVALID);
  bool secondRun = false;

  // Private methods
  llvm::Value *resolveValue(ASTNode *node, SymbolTable *accessScope = nullptr);
  llvm::Value *resolveAddress(ASTNode *node, bool storeVolatile = false);
  void moveInsertPointToBlock(llvm::BasicBlock *block);
  void createBr(llvm::BasicBlock *targetBlock);
  void createCondBr(llvm::Value *condition, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock);
  llvm::Value *insertAlloca(llvm::Type *llvmType, const std::string &varName = "");
  llvm::Value *allocateDynamicallySizedArray(llvm::Type *itemType);
  llvm::Value *createGlobalArray(llvm::Constant *constArray);
  bool insertDestructorCall(const ASTNode *node, SymbolTableEntry *varEntry);
  bool insertEmptyCtorCall(const CodeLoc &codeLoc, const SymbolTableEntry *varEntry);
  bool insertStructMethodCall(const CodeLoc &codeLoc, const SymbolTableEntry *varEntry, const std::string &accessSuffix = "");
  llvm::Value *materializeString(llvm::Value *stringStruct);
  llvm::Constant *getDefaultValueForSymbolType(const SymbolType &symbolType);
  SymbolTableEntry *initExtGlobal(const std::string &globalName, const std::string &fqGlobalName);
  llvm::Value *doImplicitCast(llvm::Value *src, llvm::Type *dstTy, SymbolType srcType);
};
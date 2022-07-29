// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <regex>

#include <ast/AstNodes.h>
#include <ast/AstVisitor.h>

#include <generator/OpRuleConversionsManager.h>
#include <symbol/ScopePath.h>
#include <symbol/SymbolType.h>

#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Passes/OptimizationLevel.h>
#include <llvm/Target/TargetMachine.h>

// Forward declarations
class SourceFile;
class ThreadFactory;
class LinkerInterface;
struct CliOptions;
class LinkerInterface;
class OpRuleConversionsManager;
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
class GeneratorVisitor : public AstVisitor {
public:
  // Constructors
  explicit GeneratorVisitor(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                            ThreadFactory &threadFactory, const LinkerInterface &linker, const CliOptions &cliOptions,
                            const SourceFile &sourceFile, const std::string &objectFile);

  // Public methods
  void optimize();
  void emit();
  void dumpIR();
  std::string getIRString();
  void dumpAsm();
  std::any visitEntry(EntryNode *node) override;
  std::any visitMainFctDef(MainFctDefNode *ctx) override;
  std::any visitFctDef(FctDefNode *ctx) override;
  std::any visitProcDef(ProcDefNode *node) override;
  std::any visitStructDef(StructDefNode *ctx) override;
  std::any visitGlobalVarDef(GlobalVarDefNode *ctx) override;
  std::any visitExtDecl(ExtDeclNode *node) override;
  std::any visitThreadDef(ThreadDefNode *ctx) override;
  std::any visitUnsafeBlockDef(UnsafeBlockDefNode *ctx) override;
  std::any visitForLoop(ForLoopNode *ctx) override;
  std::any visitForeachLoop(ForeachLoopNode *ctx) override;
  std::any visitWhileLoop(WhileLoopNode *ctx) override;
  std::any visitStmtLst(StmtLstNode *ctx) override;
  std::any visitTypeAltsLst(TypeAltsLstNode *ctx) override;
  std::any visitIfStmt(IfStmtNode *ctx) override;
  std::any visitElseStmt(ElseStmtNode *ctx) override;
  std::any visitAssertStmt(AssertStmtNode *ctx) override;
  std::any visitDeclStmt(DeclStmtNode *ctx) override;
  std::any visitImportStmt(ImportStmtNode *ctx) override;
  std::any visitReturnStmt(ReturnStmtNode *ctx) override;
  std::any visitBreakStmt(BreakStmtNode *ctx) override;
  std::any visitContinueStmt(ContinueStmtNode *ctx) override;
  std::any visitPrintfCall(PrintfCallNode *ctx) override;
  std::any visitSizeofCall(SizeofCallNode *ctx) override;
  std::any visitLenCall(LenCallNode *ctx) override;
  std::any visitTidCall(TidCallNode *ctx) override;
  std::any visitJoinCall(JoinCallNode *ctx) override;
  std::any visitAssignExpr(AssignExprNode *ctx) override;
  std::any visitTernaryExpr(TernaryExprNode *ctx) override;
  std::any visitLogicalOrExpr(LogicalOrExprNode *ctx) override;
  std::any visitLogicalAndExpr(LogicalAndExprNode *ctx) override;
  std::any visitBitwiseOrExpr(BitwiseOrExprNode *ctx) override;
  std::any visitBitwiseXorExpr(BitwiseXorExprNode *ctx) override;
  std::any visitBitwiseAndExpr(BitwiseAndExprNode *ctx) override;
  std::any visitEqualityExpr(EqualityExprNode *ctx) override;
  std::any visitRelationalExpr(RelationalExprNode *ctx) override;
  std::any visitShiftExpr(ShiftExprNode *ctx) override;
  std::any visitAdditiveExpr(AdditiveExprNode *ctx) override;
  std::any visitMultiplicativeExpr(MultiplicativeExprNode *ctx) override;
  std::any visitCastExpr(CastExprNode *ctx) override;
  std::any visitPrefixUnaryExpr(PrefixUnaryExprNode *ctx) override;
  std::any visitPostfixUnaryExpr(PostfixUnaryExprNode *ctx) override;
  std::any visitAtomicExpr(AtomicExprNode *ctx) override;
  std::any visitValue(ValueNode *ctx) override;
  std::any visitPrimitiveValue(PrimitiveValueNode *ctx) override;
  std::any visitFunctionCall(FunctionCallNode*ctx) override;
  std::any visitArrayInitialization(ArrayInitializationNode *ctx) override;
  std::any visitStructInstantiation(StructInstantiationNode *ctx) override;
  std::any visitDataType(DataTypeNode *ctx) override;
  std::any visitBaseDataType(BaseDataTypeNode *ctx) override;
  std::any visitCustomDataType(CustomDataTypeNode *ctx) override;

private:
  // Members
  std::unique_ptr<OpRuleConversionsManager> conversionsManager;
  const std::string &objectFile;
  llvm::TargetMachine *targetMachine{};
  const CliOptions &cliOptions;
  const LinkerInterface &linker;
  bool requiresMainFct = true;
  std::shared_ptr<llvm::LLVMContext> context;
  std::shared_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<llvm::Module> module;
  std::unique_ptr<llvm::DIBuilder> diBuilder;
  SymbolTable *currentScope;
  SymbolTable *rootScope;
  SymbolType currentSymbolType;
  ScopePath scopePath;
  ThreadFactory &threadFactory;
  std::unique_ptr<ErrorFactory> err;
  bool blockAlreadyTerminated = false;
  llvm::BasicBlock *allocaInsertBlock = nullptr;
  llvm::Instruction *allocaInsertInst = nullptr;
  std::stack<llvm::BasicBlock *> breakBlocks;
  std::stack<llvm::BasicBlock *> continueBlocks;
  bool constNegate = false;
  bool allArgsHardcoded = true;
  llvm::Constant *currentConstValue = nullptr;
  bool currentConstSigned = true;
  std::string currentVarName;
  std::string lhsVarName;
  llvm::Type *lhsType = nullptr;
  llvm::Value *structAccessAddress = nullptr;
  llvm::Value *dynamicArraySize = nullptr;
  llvm::Value *stackState = nullptr;
  bool secondRun = false;
  struct DebugInfo {
    llvm::DIFile *diFile;
    llvm::DICompileUnit *compileUnit;
    std::vector<llvm::DIScope *> lexicalBlocks;
    llvm::DIType *doubleTy;
    llvm::DIType *intTy;
    llvm::DIType *uIntTy;
    llvm::DIType *shortTy;
    llvm::DIType *uShortTy;
    llvm::DIType *longTy;
    llvm::DIType *uLongTy;
    llvm::DIType *byteTy;
    llvm::DIType *uByteTy;
    llvm::DIType *charTy;
    llvm::DIType *uCharTy;
    llvm::DIType *stringTy;
    llvm::DIType *boolTy;
  } debugInfo;

  // Private methods
  llvm::Value *resolveValue(antlr4::tree::ParseTree *tree);
  llvm::Value *resolveAddress(antlr4::tree::ParseTree *tree, bool storeVolatile = false);
  void moveInsertPointToBlock(llvm::BasicBlock *block);
  void createBr(llvm::BasicBlock *targetBlock);
  void createCondBr(llvm::Value *condition, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock);
  llvm::Value *insertAlloca(llvm::Type *llvmType, const std::string &varName = "");
  llvm::Value *allocateDynamicallySizedArray(llvm::Type *itemType);
  llvm::Value *createGlobalArray(llvm::Type *arrayType, const std::vector<llvm::Constant *> &itemConstants);
  bool insertDestructorCall(const antlr4::Token &token, SymbolTableEntry *varEntry);
  llvm::Function *retrievePrintfFct();
  llvm::Function *retrieveExitFct();
  llvm::Function *retrieveStackSaveFct();
  llvm::Function *retrieveStackRestoreFct();
  llvm::Type *getTypeForSymbolType(SymbolType symbolType, SymbolTable *accessScope);
  llvm::Constant *getDefaultValueForType(llvm::Type *type, const std::string &subTypeName);
  SymbolTableEntry *initExtGlobal(const std::string &globalName, const std::string &fqGlobalName);
  bool compareLLVMTypes(llvm::Type *lhs, llvm::Type *rhs);
  llvm::Value *doImplicitCast(llvm::Value *lhs, llvm::Type *rhs);
  void initializeDIBuilder(const std::string &sourceFileName, const std::string &sourceFileDir);
  [[nodiscard]] llvm::DIType *getDITypeForSymbolType(const SymbolType &symbolType) const;
  void generateFunctionDebugInfo(llvm::Function *llvmFunction, const Function *spiceFunc);
  void generateDeclDebugInfo(const antlr4::Token &token, const std::string &varName, llvm::Value *address);
  void generateAssignDebugInfo(const antlr4::Token &token, const std::string &varName, llvm::Value *value);
  void emitSourceLocation(antlr4::ParserRuleContext *ctx);
  [[nodiscard]] llvm::DIType *generateStructDebugInfo(llvm::StructType *llvmStructTy, const Struct *spiceStruct) const;
  [[nodiscard]] llvm::OptimizationLevel getLLVMOptLevelFromSpiceOptLevel() const;
};
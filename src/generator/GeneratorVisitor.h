// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <regex>

#include <cli/CliInterface.h>
#include <generator/OpRuleConversionsManager.h>
#include <linker/LinkerInterface.h>
#include <symbol/ScopePath.h>
#include <symbol/SymbolTable.h>
#include <util/ThreadFactory.h>

#include <SpiceBaseVisitor.h>

#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/Host.h>
#include <llvm/Target/TargetMachine.h>

// Forward declaration (circular import)
class SourceFile;

/**
 * Visitor for generating a source file.
 *
 * Jobs:
 * - LLVM code gen
 * - Optimization
 * - Emitting object files
 */
class GeneratorVisitor : public SpiceBaseVisitor {
public:
  // Constructors
  explicit GeneratorVisitor(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                            ThreadFactory *threadFactory, LinkerInterface *linker, CliOptions *options, SourceFile *sourceFile,
                            const std::string &objectFile);

  // Public methods
  void optimize();
  void emit();
  void dumpIR();
  std::string getIRString();
  std::any visitEntry(SpiceParser::EntryContext *ctx) override;
  std::any visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) override;
  std::any visitFunctionDef(SpiceParser::FunctionDefContext *ctx) override;
  std::any visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) override;
  std::any visitExtDecl(SpiceParser::ExtDeclContext *ctx) override;
  std::any visitStructDef(SpiceParser::StructDefContext *ctx) override;
  std::any visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) override;
  std::any visitThreadDef(SpiceParser::ThreadDefContext *ctx) override;
  std::any visitUnsafeBlockDef(SpiceParser::UnsafeBlockDefContext *ctx) override;
  std::any visitForLoop(SpiceParser::ForLoopContext *ctx) override;
  std::any visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) override;
  std::any visitWhileLoop(SpiceParser::WhileLoopContext *ctx) override;
  std::any visitStmtLst(SpiceParser::StmtLstContext *ctx) override;
  std::any visitTypeAlts(SpiceParser::TypeAltsContext *ctx) override;
  std::any visitIfStmt(SpiceParser::IfStmtContext *ctx) override;
  std::any visitElseStmt(SpiceParser::ElseStmtContext *ctx) override;
  std::any visitDeclStmt(SpiceParser::DeclStmtContext *ctx) override;
  std::any visitImportStmt(SpiceParser::ImportStmtContext *ctx) override;
  std::any visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) override;
  std::any visitBreakStmt(SpiceParser::BreakStmtContext *ctx) override;
  std::any visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) override;
  std::any visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) override;
  std::any visitPrintfCall(SpiceParser::PrintfCallContext *ctx) override;
  std::any visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) override;
  std::any visitLenCall(SpiceParser::LenCallContext *ctx) override;
  std::any visitTidCall(SpiceParser::TidCallContext *ctx) override;
  std::any visitJoinCall(SpiceParser::JoinCallContext *ctx) override;
  std::any visitAssignExpr(SpiceParser::AssignExprContext *ctx) override;
  std::any visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) override;
  std::any visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) override;
  std::any visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) override;
  std::any visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) override;
  std::any visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) override;
  std::any visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) override;
  std::any visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) override;
  std::any visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) override;
  std::any visitShiftExpr(SpiceParser::ShiftExprContext *ctx) override;
  std::any visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) override;
  std::any visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) override;
  std::any visitCastExpr(SpiceParser::CastExprContext *ctx) override;
  std::any visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) override;
  std::any visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) override;
  std::any visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) override;
  std::any visitValue(SpiceParser::ValueContext *ctx) override;
  std::any visitFunctionCall(SpiceParser::FunctionCallContext *ctx) override;
  std::any visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) override;
  std::any visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) override;
  std::any visitPrimitiveValue(SpiceParser::PrimitiveValueContext *ctx) override;
  std::any visitDataType(SpiceParser::DataTypeContext *ctx) override;
  std::any visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) override;
  std::any visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) override;

private:
  // Members
  std::unique_ptr<OpRuleConversionsManager> conversionsManager;
  std::string objectFile;
  llvm::TargetMachine *targetMachine{};
  CliOptions *cliOptions;
  LinkerInterface *linker;
  bool requiresMainFct = true;
  std::shared_ptr<llvm::LLVMContext> context;
  std::shared_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<llvm::Module> module;
  SymbolTable *currentScope;
  SymbolTable *rootScope;
  std::string scopePrefix;
  SymbolType currentSymbolType;
  ScopePath scopePath;
  ThreadFactory *threadFactory;
  std::unique_ptr<ErrorFactory> err;
  bool blockAlreadyTerminated = false;
  llvm::Value *currentThisValue = nullptr;
  llvm::BasicBlock *allocaInsertBlock = nullptr;
  llvm::Instruction *allocaInsertInst = nullptr;
  std::stack<llvm::BasicBlock *> breakBlocks;
  std::stack<llvm::BasicBlock *> continueBlocks;
  bool constNegate = false;
  bool allArgsHardcoded = true;
  llvm::Constant *currentConstValue = nullptr;
  bool currentVarSigned = false;
  std::string currentVarName;
  std::string lhsVarName;
  llvm::Type *lhsType = nullptr;
  llvm::Type *structAccessType = nullptr;
  llvm::Value *structAccessAddress = nullptr;
  std::vector<llvm::Value *> structAccessIndices;

  // Private methods
  void moveInsertPointToBlock(llvm::BasicBlock *block);
  void createBr(llvm::BasicBlock *targetBlock);
  void createCondBr(llvm::Value *condition, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock);
  llvm::Value *insertAlloca(llvm::Type *llvmType, const std::string &varName = "", llvm::Value *arraySize = nullptr);
  llvm::Type *getTypeForSymbolType(SymbolType symbolType);
  llvm::Constant *getDefaultValueForType(llvm::Type *type);
  SymbolTableEntry *initExtGlobal(const std::string &globalName, const std::string &fqGlobalName);
  bool compareLLVMTypes(llvm::Type *lhs, llvm::Type *rhs);
  unsigned int getSizeOfType(llvm::Type *structType);
  llvm::Value *doImplicitCast(llvm::Value *lhs, llvm::Type *rhs);
  [[nodiscard]] llvm::OptimizationLevel getLLVMOptLevelFromSpiceOptLevel() const;
};
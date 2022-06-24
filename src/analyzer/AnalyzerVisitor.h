// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <SpiceBaseVisitor.h>

#include <analyzer/OpRuleManager.h>
#include <cli/CliInterface.h>
#include <exception/ErrorFactory.h>
#include <linker/LinkerInterface.h>
#include <symbol/ScopePath.h>
#include <symbol/SymbolTable.h>
#include <symbol/SymbolType.h>
#include <util/ThreadFactory.h>

#include <llvm/ADT/Triple.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/Host.h>

// Forward declaration (circular import)
class SourceFile;

const std::string MAIN_FUNCTION_NAME = "main";
const std::string RETURN_VARIABLE_NAME = "result";
const std::string THIS_VARIABLE_NAME = "this";
const std::string FOREACH_DEFAULT_IDX_VARIABLE_NAME = "idx";
const std::string CTOR_VARIABLE_NAME = "ctor";
const std::string DTOR_VARIABLE_NAME = "dtor";
const std::string STRUCT_SCOPE_PREFIX = "struct:";
const std::string UNUSED_VARIABLE_NAME = "_";
const std::vector<std::string> RESERVED_KEYWORDS = {"new", "switch", "case", "yield", "stash", "pick", "sync"};

/**
 * Visitor for analyzing a source file.
 *
 * Jobs:
 * - Semantic analysis
 * - Build symbol table
 * - Type inference
 * - Type checking
 * - Resolve generic functions/procedure/structs
 */
class AnalyzerVisitor : public SpiceBaseVisitor {
public:
  // Constructors
  explicit AnalyzerVisitor(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                           ThreadFactory *threadFactory, SourceFile *sourceFile, CliOptions *options, bool requiresMainFct,
                           bool stdFile);

  // Public methods
  std::any visitEntry(SpiceParser::EntryContext *ctx) override;
  std::any visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) override;
  std::any visitFunctionDef(SpiceParser::FunctionDefContext *ctx) override;
  std::any visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) override;
  std::any visitExtDecl(SpiceParser::ExtDeclContext *ctx) override;
  std::any visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) override;
  std::any visitStructDef(SpiceParser::StructDefContext *ctx) override;
  std::any visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) override;
  std::any visitThreadDef(SpiceParser::ThreadDefContext *ctx) override;
  std::any visitUnsafeBlockDef(SpiceParser::UnsafeBlockDefContext *ctx) override;
  std::any visitForLoop(SpiceParser::ForLoopContext *ctx) override;
  std::any visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) override;
  std::any visitWhileLoop(SpiceParser::WhileLoopContext *ctx) override;
  std::any visitIfStmt(SpiceParser::IfStmtContext *ctx) override;
  std::any visitElseStmt(SpiceParser::ElseStmtContext *ctx) override;
  std::any visitAssertStmt(SpiceParser::AssertStmtContext *ctx) override;
  std::any visitArgLstDef(SpiceParser::ArgLstDefContext *ctx) override;
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
  std::shared_ptr<llvm::LLVMContext> context;
  std::shared_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<OpRuleManager> opRuleManager;
  std::unique_ptr<ErrorFactory> err;
  ThreadFactory *threadFactory;
  bool requiresMainFct = true;
  bool hasMainFunction = false;
  bool isStdFile = false;
  bool secondRun = false;
  bool needsReAnalyze = false;
  SymbolTable *currentScope = nullptr;
  SymbolTable *rootScope = nullptr;
  ScopePath scopePath;
  int nestedLoopCounter = 0;
  std::string currentVarName;
  SymbolTableEntry *currentEntry = nullptr;
  SymbolType expectedType = SymbolType(TY_DYN);
  SymbolType currentThisType = SymbolType(TY_DYN);
  bool allowUnsafeOperations = false;

  // Private methods
  SymbolType initExtStruct(const antlr4::Token &token, SymbolTable *sourceScope, const std::string &structScopePrefix,
                           const std::string &structName, const std::vector<SymbolType> &templateTypes);
  SymbolType initExtGlobal(const antlr4::Token &token, SymbolTable *sourceScope, const std::string &globalScopePrefix,
                           const std::string &globalName);
};
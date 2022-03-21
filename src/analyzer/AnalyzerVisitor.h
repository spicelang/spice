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
#include <util/ModuleRegistry.h>
#include <util/ThreadFactory.h>

#include <llvm/ADT/Triple.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/Host.h>

const std::string MAIN_FUNCTION_NAME = "main";
const std::string RETURN_VARIABLE_NAME = "result";
const std::string THIS_VARIABLE_NAME = "this";
const std::string FOREACH_DEFAULT_IDX_VARIABLE_NAME = "idx";
const std::string UNUSED_VARIABLE_NAME = "_";
const std::vector<std::string> RESERVED_KEYWORDS = {"new", "switch", "case"};

class AnalyzerVisitor : public SpiceBaseVisitor {
public:
  // Constructors
  explicit AnalyzerVisitor(const std::shared_ptr<llvm::LLVMContext> &context, const std::shared_ptr<llvm::IRBuilder<>> &builder,
                           ModuleRegistry *moduleRegistry, ThreadFactory *threadFactory, CliOptions *options,
                           LinkerInterface *linker, const std::string &sourceFile, bool requiresMainFct, bool stdFile);
  ~AnalyzerVisitor() override;

  // Public methods
  antlrcpp::Any visitEntry(SpiceParser::EntryContext *ctx) override;
  antlrcpp::Any visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) override;
  antlrcpp::Any visitFunctionDef(SpiceParser::FunctionDefContext *ctx) override;
  antlrcpp::Any visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) override;
  antlrcpp::Any visitExtDecl(SpiceParser::ExtDeclContext *ctx) override;
  antlrcpp::Any visitStructDef(SpiceParser::StructDefContext *ctx) override;
  antlrcpp::Any visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) override;
  antlrcpp::Any visitThreadDef(SpiceParser::ThreadDefContext *ctx) override;
  antlrcpp::Any visitForLoop(SpiceParser::ForLoopContext *ctx) override;
  antlrcpp::Any visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) override;
  antlrcpp::Any visitWhileLoop(SpiceParser::WhileLoopContext *ctx) override;
  antlrcpp::Any visitIfStmt(SpiceParser::IfStmtContext *ctx) override;
  antlrcpp::Any visitElseStmt(SpiceParser::ElseStmtContext *ctx) override;
  antlrcpp::Any visitParamLstDef(SpiceParser::ParamLstDefContext *ctx) override;
  antlrcpp::Any visitDeclStmt(SpiceParser::DeclStmtContext *ctx) override;
  antlrcpp::Any visitImportStmt(SpiceParser::ImportStmtContext *ctx) override;
  antlrcpp::Any visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) override;
  antlrcpp::Any visitBreakStmt(SpiceParser::BreakStmtContext *ctx) override;
  antlrcpp::Any visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) override;
  antlrcpp::Any visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) override;
  antlrcpp::Any visitPrintfCall(SpiceParser::PrintfCallContext *ctx) override;
  antlrcpp::Any visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) override;
  antlrcpp::Any visitTidCall(SpiceParser::TidCallContext *ctx) override;
  antlrcpp::Any visitJoinCall(SpiceParser::JoinCallContext *ctx) override;
  antlrcpp::Any visitAssignExpr(SpiceParser::AssignExprContext *ctx) override;
  antlrcpp::Any visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) override;
  antlrcpp::Any visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) override;
  antlrcpp::Any visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) override;
  antlrcpp::Any visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) override;
  antlrcpp::Any visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) override;
  antlrcpp::Any visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) override;
  antlrcpp::Any visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) override;
  antlrcpp::Any visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) override;
  antlrcpp::Any visitShiftExpr(SpiceParser::ShiftExprContext *ctx) override;
  antlrcpp::Any visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) override;
  antlrcpp::Any visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) override;
  antlrcpp::Any visitCastExpr(SpiceParser::CastExprContext *ctx) override;
  antlrcpp::Any visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) override;
  antlrcpp::Any visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) override;
  antlrcpp::Any visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) override;
  antlrcpp::Any visitValue(SpiceParser::ValueContext *ctx) override;
  antlrcpp::Any visitPrimitiveValue(SpiceParser::PrimitiveValueContext *ctx) override;
  antlrcpp::Any visitDataType(SpiceParser::DataTypeContext *ctx) override;
  antlrcpp::Any visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) override;

private:
  // Members
  std::shared_ptr<llvm::LLVMContext> context;
  std::shared_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<OpRuleManager> opRuleManager;
  ErrorFactory *err;
  ModuleRegistry *moduleRegistry;
  ThreadFactory *threadFactory;
  CliOptions *cliOptions;
  LinkerInterface *linker;
  std::string sourceFile;
  bool requiresMainFct = true;
  bool hasMainFunction = false;
  bool isStdFile = false;
  SymbolTable *currentScope = nullptr;
  ScopePath scopePath;
  std::string scopePrefix;
  bool parameterMode = false;
  int nestedLoopCounter = 0;
  std::string currentVarName;
  SymbolTableEntry *currentEntry = nullptr;
  SymbolType expectedType = SymbolType(TY_DYN);

  // Private methods
  SymbolType initExtStruct(const antlr4::Token &token, SymbolTable *sourceScope, const std::string &structScopePrefix,
                           const std::string &structName);
  SymbolType initExtGlobal(const antlr4::Token &token, SymbolTable *sourceScope, const std::string &globalScopePrefix,
                           const std::string &globalName);
};
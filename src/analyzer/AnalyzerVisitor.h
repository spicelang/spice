// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <ast/AstNodes.h>
#include <ast/AstVisitor.h>

#include <analyzer/OpRuleManager.h>
#include <symbol/ScopePath.h>
#include <symbol/SymbolType.h>

#include <llvm/ADT/Triple.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/Host.h>

const std::string MAIN_FUNCTION_NAME = "main";
const std::string RETURN_VARIABLE_NAME = "result";
const std::string THIS_VARIABLE_NAME = "this";
const std::string FOREACH_DEFAULT_IDX_VARIABLE_NAME = "idx";
const std::string CTOR_VARIABLE_NAME = "ctor";
const std::string DTOR_VARIABLE_NAME = "dtor";
const std::string STRUCT_SCOPE_PREFIX = "struct:";
const std::string UNUSED_VARIABLE_NAME = "_";
const std::vector<std::string> RESERVED_KEYWORDS = {"new", "switch", "case", "yield", "stash", "pick", "sync"};

// Forward declarations
class OpRuleManager;
struct CliOptions;
class ErrorFactory;
class ThreadFactory;
class LinkerInterface;
class SymbolTable;
class SymbolTableEntry;
class SourceFile;

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
class AnalyzerVisitor : public AstVisitor {
public:
  // Constructors
  explicit AnalyzerVisitor(std::shared_ptr<llvm::LLVMContext> context, std::shared_ptr<llvm::IRBuilder<>> builder,
                           const ThreadFactory &threadFactory, const SourceFile &sourceFile, CliOptions &options,
                           bool requiresMainFct, bool stdFile);

  // Public methods
  std::any visitEntry(EntryNode *ctx) override;
  std::any visitMainFctDef(MainFctDefNode *node) override;
  std::any visitFctDef(FctDefNode *node) override;
  std::any visitProcDef(ProcDefNode *ctx) override;
  std::any visitStructDef(StructDefNode *node) override;
  std::any visitGenericTypeDef(GenericTypeDefNode *node) override;
  std::any visitGlobalVarDef(GlobalVarDefNode *ctx) override;
  std::any visitExtDecl(ExtDeclNode *node) override;
  std::any visitThreadDef(ThreadDefNode *ctx) override;
  std::any visitUnsafeBlockDef(UnsafeBlockDefNode *ctx) override;
  std::any visitForLoop(ForLoopNode *ctx) override;
  std::any visitForeachLoop(ForeachLoopNode *ctx) override;
  std::any visitWhileLoop(WhileLoopNode *ctx) override;
  std::any visitIfStmt(IfStmtNode *ctx) override;
  std::any visitElseStmt(ElseStmtNode *ctx) override;
  std::any visitAssertStmt(AssertStmtNode *ctx) override;
  std::any visitArgLstDef(ParamLstNode *ctx) override;
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
  std::any visitFunctionCall(FunctionCallNode *ctx) override;
  std::any visitArrayInitialization(ArrayInitializationNode *ctx) override;
  std::any visitStructInstantiation(StructInstantiationNode *ctx) override;
  std::any visitDataType(DataTypeNode *ctx) override;
  std::any visitBaseDataType(BaseDataTypeNode *ctx) override;
  std::any visitCustomDataType(CustomDataTypeNode *ctx) override;

private:
  // Members
  std::shared_ptr<llvm::LLVMContext> context;
  std::shared_ptr<llvm::IRBuilder<>> builder;
  std::unique_ptr<OpRuleManager> opRuleManager;
  std::unique_ptr<ErrorFactory> err;
  const ThreadFactory &threadFactory;
  bool requiresMainFct = true;
  bool hasMainFunction = false;
  bool isStdFile = false;
  unsigned int runNumber = 1;
  bool reAnalyzeRequired = false;
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
  void insertDestructorCall(const CodeLoc &codeLoc, SymbolTableEntry *varEntry);
  SymbolType initExtStruct(const CodeLoc &codeLoc, SymbolTable *sourceScope, const std::string &structScopePrefix,
                           const std::string &structName, const std::vector<SymbolType> &templateTypes);
  SymbolType initExtGlobal(const CodeLoc &codeLoc, SymbolTable *sourceScope, const std::string &globalScopePrefix,
                           const std::string &globalName);
};
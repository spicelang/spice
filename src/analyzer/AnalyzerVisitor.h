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

const char *const MAIN_FUNCTION_NAME = "main";
const char *const RETURN_VARIABLE_NAME = "result";
const char *const THIS_VARIABLE_NAME = "this";
const char *const FOREACH_DEFAULT_IDX_VARIABLE_NAME = "idx";
const char *const CTOR_FUNCTION_NAME = "ctor";
const char *const DTOR_FUNCTION_NAME = "dtor";
const char *const STRUCT_SCOPE_PREFIX = "struct:";
const char *const INTERFACE_SCOPE_PREFIX = "interface:";
const char *const ENUM_SCOPE_PREFIX = "enum:";
const char *const UNUSED_VARIABLE_NAME = "_";
const char *const RESERVED_KEYWORDS[8] = {"new", "switch", "case", "yield", "stash", "pick", "sync", "class"};

// Forward declarations
class OpRuleManager;
class GlobalResourceManager;
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
 * - Detect usages of runtime modules
 */
class AnalyzerVisitor : public AstVisitor {
public:
  // Constructors
  explicit AnalyzerVisitor(GlobalResourceManager &resourceManager, SourceFile &sourceFile, bool requiresMainFct);

  // Friend classes
  friend class OpRuleManager;

  // Public methods
  std::any visitEntry(EntryNode *node) override;
  std::any visitMainFctDef(MainFctDefNode *node) override;
  std::any visitFctDef(FctDefNode *node) override;
  std::any visitProcDef(ProcDefNode *node) override;
  std::any visitStructDef(StructDefNode *node) override;
  std::any visitInterfaceDef(InterfaceDefNode *node) override;
  std::any visitEnumDef(EnumDefNode *node) override;
  std::any visitGenericTypeDef(GenericTypeDefNode *node) override;
  std::any visitGlobalVarDef(GlobalVarDefNode *node) override;
  std::any visitExtDecl(ExtDeclNode *node) override;
  std::any visitThreadDef(ThreadDefNode *node) override;
  std::any visitUnsafeBlockDef(UnsafeBlockDefNode *node) override;
  std::any visitForLoop(ForLoopNode *node) override;
  std::any visitForeachLoop(ForeachLoopNode *node) override;
  std::any visitWhileLoop(WhileLoopNode *node) override;
  std::any visitIfStmt(IfStmtNode *node) override;
  std::any visitElseStmt(ElseStmtNode *node) override;
  std::any visitAssertStmt(AssertStmtNode *node) override;
  std::any visitParamLst(ParamLstNode *node) override;
  std::any visitSignature(SignatureNode *node) override;
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
  std::any visitBaseDataType(BaseDataTypeNode *node) override;
  std::any visitCustomDataType(CustomDataTypeNode *node) override;

private:
  // Members
  const GlobalResourceManager &resourceManager;
  std::unique_ptr<OpRuleManager> opRuleManager;
  SourceFile &sourceFile;
  bool requiresMainFct = true;
  bool hasMainFunction = false;
  size_t runNumber = 1;
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
  void insertDestructorCall(const AstNode *node, const SymbolTableEntry *varEntry);
  SymbolType initExtStruct(SymbolTable *sourceScope, const std::string &structScopePrefix, const std::string &structName,
                           const std::vector<SymbolType> &templateTypes, const AstNode *node);
  SymbolType initExtGlobal(SymbolTable *sourceScope, const std::string &globalScopePrefix, const std::string &globalName,
                           const AstNode *node);
  static void checkForReservedKeyword(const AstNode *node, const std::string &identifier) ;
};
// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ParallelizableASTVisitor.h>
#include <irgenerator/DebugInfoGenerator.h>
#include <irgenerator/OpRuleConversionManager.h>
#include <irgenerator/StdFunctionManager.h>
#include <symboltablebuilder/Scope.h>

namespace spice::compiler {

const char *const ANON_GLOBAL_STRING_NAME = "anon.string.";
const char *const ANON_GLOBAL_ARRAY_NAME = "anon.array.";
const char *const ANON_GLOBAL_STRUCT_NAME = "anon.struct.";
const char *const ANON_GLOBAL_CAPTURES_ARRAY_NAME = "anon.captures.";
const char *const CAPTURES_PARAM_NAME = "captures";

enum Likeliness { UNSPECIFIED, LIKELY, UNLIKELY };

// Forward declarations
class SourceFile;

class IRGenerator : private CompilerPass, public ParallelizableASTVisitor {
public:
  // Type definitions
  using ParamInfoList = std::vector<std::pair<std::string, SymbolTableEntry *>>;

  // Constructors
  IRGenerator(GlobalResourceManager &resourceManager, SourceFile *sourceFile);

  // Friend classes
  friend class StdFunctionManager;
  friend class OpRuleConversionManager;
  friend class DebugInfoGenerator;
  friend class ScopeHandle;

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
  std::any visitAliasDef(const AliasDefNode *node) override;
  std::any visitGlobalVarDef(const GlobalVarDefNode *node) override;
  std::any visitExtDecl(const ExtDeclNode *node) override;
  // Control structures
  std::any visitUnsafeBlockDef(const UnsafeBlockDefNode *node) override;
  std::any visitForLoop(const ForLoopNode *node) override;
  std::any visitForeachLoop(const ForeachLoopNode *node) override;
  std::any visitWhileLoop(const WhileLoopNode *node) override;
  std::any visitDoWhileLoop(const DoWhileLoopNode *node) override;
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
  std::any visitAlignofCall(const AlignofCallNode *node) override;
  std::any visitLenCall(const LenCallNode *node) override;
  std::any visitPanicCall(const PanicCallNode *node) override;
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
  std::any visitFctCall(const FctCallNode *node) override;
  std::any visitArrayInitialization(const ArrayInitializationNode *node) override;
  std::any visitStructInstantiation(const StructInstantiationNode *node) override;
  std::any visitLambdaFunc(const LambdaFuncNode *node) override;
  std::any visitLambdaProc(const LambdaProcNode *node) override;
  std::any visitLambdaExpr(const LambdaExprNode *node) override;
  std::any visitDataType(const DataTypeNode *node) override;

  // Public methods
  llvm::Value *insertAlloca(llvm::Type *llvmType, const std::string &varName = "");
  llvm::Value *resolveValue(const ASTNode *node, Scope *accessScope = nullptr);
  llvm::Value *resolveValue(const ASTNode *node, LLVMExprResult &exprResult, Scope *accessScope = nullptr);
  llvm::Value *resolveValue(const SymbolType &symbolType, LLVMExprResult &exprResult, Scope *accessScope = nullptr);
  llvm::Value *resolveAddress(const ASTNode *node, bool storeVolatile = false);
  llvm::Value *resolveAddress(LLVMExprResult &exprResult, bool storeVolatile = false);
  [[nodiscard]] llvm::Constant *getDefaultValueForSymbolType(const SymbolType &symbolType);
  [[nodiscard]] llvm::Constant *getConst(const CompileTimeValue &compileTimeValue, const SymbolType &type, const ASTNode *node);
  [[nodiscard]] std::string getIRString() const;

private:
  // Private methods
  llvm::BasicBlock *createBlock(const std::string &blockName = "");
  void switchToBlock(llvm::BasicBlock *block, llvm::Function *parentFct = nullptr);
  void insertJump(llvm::BasicBlock *targetBlock);
  void insertCondJump(llvm::Value *condition, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock,
                      Likeliness likeliness = UNSPECIFIED);
  void verifyFunction(llvm::Function *fct, const CodeLoc &codeLoc) const;
  void verifyModule(const CodeLoc &codeLoc) const;
  LLVMExprResult doAssignment(const ASTNode *lhsNode, const ASTNode *rhsNode);
  LLVMExprResult doAssignment(llvm::Value *lhsAddress, SymbolTableEntry *lhsEntry, const ASTNode *rhsNode, bool isDecl = false);
  LLVMExprResult doAssignment(llvm::Value *lhsAddress, SymbolTableEntry *lhsEntry, LLVMExprResult &rhs,
                              const SymbolType &rhsSType, bool isDecl);
  llvm::Value *createShallowCopy(llvm::Value *oldAddress, llvm::Type *varType, llvm::Value *targetAddress,
                                 const std::string &name = "", bool isVolatile = false);
  void generateScopeCleanup(const StmtLstNode *node) const;
  void generateDtorCall(SymbolTableEntry *entry, Function *dtor, const StmtLstNode *node) const;
  void autoDeReferencePtr(llvm::Value *&ptr, SymbolType &symbolType, Scope *accessScope) const;
  llvm::GlobalVariable *createGlobalConst(const std::string &baseName, llvm::Constant *constant);
  llvm::Constant *createGlobalStringConst(const std::string &baseName, const std::string &value, const CodeLoc &codeLoc);
  [[nodiscard]] std::string getUnusedGlobalName(const std::string &baseName) const;
  void materializeConstant(LLVMExprResult &exprResult);
  llvm::Value *doImplicitCast(llvm::Value *src, SymbolType dstSTy, SymbolType srcSTy);
  const std::vector<const Function *> &getOpFctPointers(const ASTNode *node) const;
  llvm::Value *buildFatFctPtr(Scope *bodyScope, llvm::StructType *capturesStructType, llvm::Value *lambda);

  // Private members
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  llvm::Module *module;
  OpRuleConversionManager conversionManager = OpRuleConversionManager(resourceManager, this);
  const StdFunctionManager stdFunctionManager;
  DebugInfoGenerator diGenerator = DebugInfoGenerator(this);
  struct CommonLLVMTypes {
    llvm::StructType *fatPtrType = nullptr;
  } llvmTypes;
  std::vector<llvm::BasicBlock *> breakBlocks;
  std::vector<llvm::BasicBlock *> continueBlocks;
  llvm::BasicBlock *allocaInsertBlock = nullptr;
  llvm::Instruction *allocaInsertInst = nullptr;
  bool blockAlreadyTerminated = false;
  size_t manIdx = 0;
};

} // namespace spice::compiler
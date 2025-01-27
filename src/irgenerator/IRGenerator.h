// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ParallelizableASTVisitor.h>
#include <irgenerator/DebugInfoGenerator.h>
#include <irgenerator/OpRuleConversionManager.h>
#include <irgenerator/StdFunctionManager.h>
#include <symboltablebuilder/Scope.h>
#include <util/DeferredLogic.h>

namespace spice::compiler {

const char *const ANON_GLOBAL_STRING_NAME = "anon.string.";
const char *const ANON_GLOBAL_ARRAY_NAME = "anon.array.";
const char *const CAPTURES_PARAM_NAME = "captures";
static const std::string PRODUCER_STRING = "spice version " + std::string(SPICE_VERSION) + " (https://github.com/spicelang/spice)";

enum Likeliness : uint8_t {
  UNSPECIFIED,
  LIKELY,
  UNLIKELY,
};

// Forward declarations
class SourceFile;

class IRGenerator final : CompilerPass, public ParallelizableASTVisitor {
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
  std::any visitUnsafeBlockDef(const UnsafeBlockNode *node) override;
  std::any visitForLoop(const ForLoopNode *node) override;
  std::any visitForeachLoop(const ForeachLoopNode *node) override;
  std::any visitWhileLoop(const WhileLoopNode *node) override;
  std::any visitDoWhileLoop(const DoWhileLoopNode *node) override;
  std::any visitIfStmt(const IfStmtNode *node) override;
  std::any visitElseStmt(const ElseStmtNode *node) override;
  std::any visitSwitchStmt(const SwitchStmtNode *node) override;
  std::any visitCaseBranch(const CaseBranchNode *node) override;
  std::any visitDefaultBranch(const DefaultBranchNode *node) override;
  std::any visitAssertStmt(const AssertStmtNode *node) override;
  std::any visitAnonymousBlockStmt(const AnonymousBlockStmtNode *node) override;
  // Statements
  std::any visitStmtLst(const StmtLstNode *node) override;
  std::any visitTypeAltsLst(const TypeAltsLstNode *node) override;
  std::any visitDeclStmt(const DeclStmtNode *node) override;
  std::any visitQualifierLst(const QualifierLstNode *node) override;
  std::any visitModAttr(const ModAttrNode *node) override;
  std::any visitTopLevelDefinitionAttr(const TopLevelDefinitionAttrNode *node) override;
  std::any visitCaseConstant(const CaseConstantNode *node) override;
  std::any visitReturnStmt(const ReturnStmtNode *node) override;
  std::any visitBreakStmt(const BreakStmtNode *node) override;
  std::any visitContinueStmt(const ContinueStmtNode *node) override;
  std::any visitFallthroughStmt(const FallthroughStmtNode *node) override;
  // Builtin functions
  std::any visitBuiltinCall(const BuiltinCallNode *node) override;
  std::any visitPrintfCall(const PrintfCallNode *node) override;
  std::any visitSizeofCall(const SizeofCallNode *node) override;
  std::any visitAlignofCall(const AlignofCallNode *node) override;
  std::any visitLenCall(const LenCallNode *node) override;
  std::any visitPanicCall(const PanicCallNode *node) override;
  std::any visitSysCall(const SysCallNode *node) override;
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
  llvm::Value *insertAlloca(llvm::Type *llvmType, std::string varName = "");
  llvm::Value *insertLoad(llvm::Type *llvmType, llvm::Value *ptr, bool isVolatile = false, const std::string &varName = "") const;
  void insertStore(llvm::Value *val, llvm::Value *ptr, bool isVolatile = false) const;
  llvm::Value *insertInBoundsGEP(llvm::Type *type, llvm::Value *basePtr, llvm::ArrayRef<llvm::Value *> indices,
                                 std::string varName = "") const;
  llvm::Value *insertStructGEP(llvm::Type *type, llvm::Value *basePtr, unsigned index, std::string varName = "") const;
  llvm::Value *resolveValue(const ASTNode *node);
  llvm::Value *resolveValue(const ASTNode *node, LLVMExprResult &exprResult) const;
  llvm::Value *resolveValue(const QualType &qualType, LLVMExprResult &exprResult) const;
  llvm::Value *resolveAddress(const ASTNode *node);
  llvm::Value *resolveAddress(LLVMExprResult &exprResult);
  [[nodiscard]] llvm::Constant *getDefaultValueForSymbolType(const QualType &symbolType);
  [[nodiscard]] static std::string getIRString(llvm::Module *llvmModule, bool withoutTargetInfo);

private:
  // Private methods
  llvm::Constant *getConst(const CompileTimeValue &compileTimeValue, const QualType &type, const ASTNode *node) const;
  llvm::BasicBlock *createBlock(const std::string &blockName = "") const;
  void switchToBlock(llvm::BasicBlock *block, llvm::Function *parentFct = nullptr);
  void terminateBlock(const StmtLstNode *stmtLstNode);
  void insertJump(llvm::BasicBlock *targetBlock);
  void insertCondJump(llvm::Value *condition, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock,
                      Likeliness likeliness = UNSPECIFIED);
  void verifyFunction(const llvm::Function *fct, const CodeLoc &codeLoc) const;
  void verifyModule(const CodeLoc &codeLoc) const;
  LLVMExprResult doAssignment(const ASTNode *lhsNode, const ASTNode *rhsNode, const ASTNode *node);
  LLVMExprResult doAssignment(llvm::Value *lhsAddress, SymbolTableEntry *lhsEntry, const ASTNode *rhsNode, const ASTNode *node,
                              bool isDecl = false);
  LLVMExprResult doAssignment(llvm::Value *lhsAddress, SymbolTableEntry *lhsEntry, LLVMExprResult &rhs, const QualType &rhsSType,
                              const ASTNode *node, bool isDecl);
  void generateShallowCopy(llvm::Value *oldAddress, llvm::Type *varType, llvm::Value *targetAddress, bool isVolatile) const;
  void autoDeReferencePtr(llvm::Value *&ptr, QualType &symbolType) const;
  llvm::GlobalVariable *createGlobalConst(const std::string &baseName, llvm::Constant *constant) const;
  llvm::Constant *createGlobalStringConst(const std::string &baseName, const std::string &value, const CodeLoc &codeLoc) const;
  [[nodiscard]] std::string getUnusedGlobalName(const std::string &baseName) const;
  static void materializeConstant(LLVMExprResult &exprResult);
  const std::vector<const Function *> &getOpFctPointers(const ASTNode *node) const;
  llvm::Value *buildFatFctPtr(Scope *bodyScope, llvm::Type *capturesStructType, llvm::Value *lambda);
  llvm::Type *buildCapturesContainerType(const CaptureMap &captures) const;
  void unpackCapturesToLocalVariables(const CaptureMap &captures, llvm::Value *val, llvm::Type *structType);

  // Generate implicit
  llvm::Value *doImplicitCast(llvm::Value *src, QualType dstSTy, QualType srcSTy);
  void generateScopeCleanup(const StmtLstNode *node) const;
  llvm::Value *generateFctCall(const Function *fct, const std::vector<llvm::Value *> &args) const;
  void generateProcCall(const Function *proc, std::vector<llvm::Value *> &args) const;
  void generateCtorOrDtorCall(const SymbolTableEntry *entry, const Function *ctorOrDtor,
                              const std::vector<llvm::Value *> &args) const;
  void generateCtorOrDtorCall(llvm::Value *structAddr, const Function *ctorOrDtor, const std::vector<llvm::Value *> &args) const;
  void generateDeallocCall(llvm::Value *variableAddress) const;
  llvm::Function *generateImplicitFunction(const std::function<void(void)> &generateBody, const Function *spiceFunc);
  llvm::Function *generateImplicitProcedure(const std::function<void(void)> &generateBody, const Function *spiceProc);
  void generateCtorBodyPreamble(Scope *bodyScope);
  void generateDefaultCtor(const Function *ctorFunction);
  void generateCopyCtorBodyPreamble(const Function *copyCtorFunction);
  void generateDefaultCopyCtor(const Function *copyCtorFunction);
  void generateDtorBodyPreamble(const Function *dtorFunction) const;
  void generateDefaultDtor(const Function *dtorFunction);
  void generateTestMain();

  // Generate target dependent
  const char *getSysCallAsmString() const;
  const char *getSysCallConstraintString() const;

  // Generate VTable
  llvm::Constant *generateTypeInfoName(StructBase *spiceStruct) const;
  llvm::Constant *generateTypeInfo(StructBase *spiceStruct) const;
  llvm::Constant *generateVTable(StructBase *spiceStruct) const;
  void generateVTableInitializer(const StructBase *spiceStruct) const;

  // Private members
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  llvm::Module *module;
  OpRuleConversionManager conversionManager;
  const StdFunctionManager stdFunctionManager;
  DebugInfoGenerator diGenerator = DebugInfoGenerator(this);
  struct CommonLLVMTypes {
    llvm::StructType *fatPtrType = nullptr;
  } llvmTypes;
  std::vector<llvm::BasicBlock *> breakBlocks;
  std::vector<llvm::BasicBlock *> continueBlocks;
  std::stack<llvm::BasicBlock *> fallthroughBlocks;
  llvm::BasicBlock *allocaInsertBlock = nullptr;
  llvm::Instruction *allocaInsertInst = nullptr;
  bool blockAlreadyTerminated = false;
  std::vector<DeferredLogic> deferredVTableInitializations;
};

} // namespace spice::compiler
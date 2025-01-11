// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <queue>
#include <utility>
#include <vector>

#include <ast/ASTVisitor.h>
#include <ast/ParallelizableASTVisitor.h>
#include <exception/CompilerError.h>
#include <model/Function.h>
#include <model/Struct.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/TypeSpecifiers.h>
#include <typechecker/ExprResult.h>
#include <util/CodeLoc.h>
#include <util/CommonUtil.h>

namespace spice::compiler {

// Forward declarations
class TopLevelDefNode;

// Macros
#define GET_CHILDREN(...)                                                                                                        \
  std::vector<ASTNode *> getChildren() const override { return collectChildren(__VA_ARGS__); }

// Operator overload function names
constexpr const char *const OP_FCT_PREFIX = "op.";
constexpr const char *const OP_FCT_PLUS = "op.plus";
constexpr const char *const OP_FCT_MINUS = "op.minus";
constexpr const char *const OP_FCT_MUL = "op.mul";
constexpr const char *const OP_FCT_DIV = "op.div";
constexpr const char *const OP_FCT_EQUAL = "op.equal";
constexpr const char *const OP_FCT_NOT_EQUAL = "op.notequal";
constexpr const char *const OP_FCT_SHL = "op.shl";
constexpr const char *const OP_FCT_SHR = "op.shr";
constexpr const char *const OP_FCT_PLUS_EQUAL = "op.plusequal";
constexpr const char *const OP_FCT_MINUS_EQUAL = "op.minusequal";
constexpr const char *const OP_FCT_MUL_EQUAL = "op.mulequal";
constexpr const char *const OP_FCT_DIV_EQUAL = "op.divequal";
constexpr const char *const OP_FCT_POSTFIX_PLUS_PLUS = "op.plusplus.post";
constexpr const char *const OP_FCT_POSTFIX_MINUS_MINUS = "op.minusminus.post";

/**
 * Saves a constant value for an AST node to realize features like array-out-of-bounds checks
 */
struct CompileTimeValue {
  double_t doubleValue = 0.0;
  int32_t intValue = 0;
  int16_t shortValue = 0;
  int64_t longValue = 0;
  int8_t charValue = 0;
  bool boolValue = false;
  size_t stringValueOffset = 0; // Offset into vector of strings in GlobalResourceManager
};

// Make sure we have no unexpected increases in memory consumption
static_assert(sizeof(CompileTimeValue) == 40);

// =========================================================== AstNode ===========================================================

class ASTNode {
public:
  // Constructors
  explicit ASTNode(const CodeLoc &codeLoc) : codeLoc(codeLoc) {}
  virtual ~ASTNode() = default;
  ASTNode(const ASTNode &) = delete;

  // Virtual methods
  virtual std::any accept(AbstractASTVisitor *visitor) = 0;
  virtual std::any accept(ParallelizableASTVisitor *visitor) const = 0;

  template <typename... Args> ALWAYS_INLINE std::vector<ASTNode *> collectChildren(Args &&...args) const {
    std::vector<ASTNode *> children;

    // Lambda to handle each argument
    [[maybe_unused]] const auto addChild = [&children]<typename T>(T &&arg) ALWAYS_INLINE {
      using TDecayed = std::decay_t<T>;
      if constexpr (std::is_pointer_v<TDecayed>) {
        if (arg != nullptr)
          children.push_back(arg);
      } else if constexpr (is_vector_of_derived_from_v<TDecayed, ASTNode>) {
        children.insert(children.end(), arg.begin(), arg.end());
      } else {
        static_assert(false, "Unsupported type");
      }
    };

    (addChild(std::forward<Args>(args)), ...);
    return children;
  }

  virtual std::vector<ASTNode *> getChildren() const = 0;

  void resizeToNumberOfManifestations(const size_t manifestationCount) { // NOLINT(misc-no-recursion)
    // Resize children
    for (ASTNode *child : getChildren()) {
      assert(child != nullptr);
      child->resizeToNumberOfManifestations(manifestationCount);
    }
    // Reserve this node
    symbolTypes.resize(manifestationCount, QualType(TY_INVALID));
    // Do custom work
    customItemsInitialization(manifestationCount);
  }

  virtual std::vector<std::vector<const Function *>> *getOpFctPointers() {         // LCOV_EXCL_LINE
    assert_fail("The given node does not overload the getOpFctPointers function"); // LCOV_EXCL_LINE
    return nullptr;                                                                // LCOV_EXCL_LINE
  } // LCOV_EXCL_LINE
  [[nodiscard]] virtual const std::vector<std::vector<const Function *>> *getOpFctPointers() const { // LCOV_EXCL_LINE
    assert_fail("The given node does not overload the getOpFctPointers function");                   // LCOV_EXCL_LINE
    return nullptr;                                                                                  // LCOV_EXCL_LINE
  } // LCOV_EXCL_LINE

  virtual void customItemsInitialization(size_t) {} // Noop

  QualType setEvaluatedSymbolType(const QualType &symbolType, const size_t idx) {
    assert(symbolTypes.size() > idx);
    symbolTypes.at(idx) = symbolType;
    return symbolType;
  }

  [[nodiscard]] const QualType &getEvaluatedSymbolType(const size_t idx) const { // NOLINT(misc-no-recursion)
    if (!symbolTypes.empty() && !symbolTypes.at(idx).is(TY_INVALID))
      return symbolTypes.at(idx);
    const std::vector<ASTNode *> children = getChildren();
    if (children.size() != 1)
      throw CompilerError(INTERNAL_ERROR, "Cannot deduce evaluated symbol type");
    return children.front()->getEvaluatedSymbolType(idx);
  }

  [[nodiscard]] virtual bool hasCompileTimeValue() const { // NOLINT(misc-no-recursion)
    const std::vector<ASTNode *> children = getChildren();
    if (children.size() != 1)
      return false;
    return children.front()->hasCompileTimeValue();
  }

  [[nodiscard]] virtual CompileTimeValue getCompileTimeValue() const { // NOLINT(misc-no-recursion)
    const std::vector<ASTNode *> children = getChildren();
    if (children.size() != 1)
      return {};
    return children.front()->getCompileTimeValue();
  }

  [[nodiscard]] std::string getErrorMessage() const;

  [[nodiscard]] virtual bool returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const { // NOLINT(misc-no-recursion)
    const std::vector<ASTNode *> children = getChildren();
    return children.size() == 1 && children.front()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
  }

  [[nodiscard]] virtual std::vector<Function *> *getFctManifestations(const std::string &) {                 // LCOV_EXCL_LINE
    assert_fail("Must be called on a FctDefNode, ProcDefNode, ExtDeclNode, StructDefNode or SignatureNode"); // LCOV_EXCL_LINE
    return nullptr;                                                                                          // LCOV_EXCL_LINE
  } // LCOV_EXCL_LINE

  [[nodiscard]] virtual std::vector<Struct *> *getStructManifestations() { // LCOV_EXCL_LINE
    assert_fail("Must be called on a StructDefNode");                      // LCOV_EXCL_LINE
    return nullptr;                                                        // LCOV_EXCL_LINE
  } // LCOV_EXCL_LINE

  [[nodiscard]] virtual std::vector<Interface *> *getInterfaceManifestations() { // LCOV_EXCL_LINE
    assert_fail("Must be called on a InterfaceDefNode");                         // LCOV_EXCL_LINE
    return nullptr;                                                              // LCOV_EXCL_LINE
  } // LCOV_EXCL_LINE

  [[nodiscard]] const StmtLstNode *getNextOuterStmtLst() const;

  [[nodiscard]] virtual bool isFctOrProcDef() const { return false; }
  [[nodiscard]] virtual bool isStructDef() const { return false; }
  [[nodiscard]] virtual bool isParam() const { return false; }
  [[nodiscard]] virtual bool isStmtLst() const { return false; }
  [[nodiscard]] virtual bool isAssignExpr() const { return false; }
  [[nodiscard]] virtual bool isExprStmt() const { return false; }

  // Public members
  ASTNode *parent = nullptr;
  const CodeLoc codeLoc;
  QualTypeList symbolTypes;
  bool unreachable = false;
};

// Make sure we have no unexpected increases in memory consumption
static_assert(sizeof(ASTNode) == 80);

// ========================================================== EntryNode ==========================================================

class EntryNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitEntry(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitEntry(this); }

  // Other methods
  GET_CHILDREN(modAttrs, importDefs, topLevelDefs);

  // Public members
  std::vector<ModAttrNode *> modAttrs;
  std::vector<ImportDefNode *> importDefs;
  std::vector<TopLevelDefNode *> topLevelDefs;
};

// ======================================================= TopLevelDefNode =======================================================

class TopLevelDefNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override = 0;
  std::any accept(ParallelizableASTVisitor *visitor) const override = 0;
};

// =========================================================== StmtNode ==========================================================

class StmtNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override = 0;
  std::any accept(ParallelizableASTVisitor *visitor) const override = 0;
};

// ========================================================== ExprNode ===========================================================

class ExprNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override = 0;
  std::any accept(ParallelizableASTVisitor *visitor) const override = 0;
};

// ======================================================== MainFctDefNode =======================================================

class MainFctDefNode final : public TopLevelDefNode {
public:
  // Constructors
  using TopLevelDefNode::TopLevelDefNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitMainFctDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitMainFctDef(this); }

  // Other methods
  GET_CHILDREN(attrs, paramLst, body);
  [[nodiscard]] static std::string getScopeId() { return "fct:main"; }
  bool returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const override;
  [[nodiscard]] bool isFctOrProcDef() const override { return true; }

  // Public members
  TopLevelDefinitionAttrNode *attrs = nullptr;
  ParamLstNode *paramLst = nullptr;
  StmtLstNode *body = nullptr;
  bool takesArgs = false;
  SymbolTableEntry *entry = nullptr;
  Scope *bodyScope = nullptr;
};

// ========================================================== FctNameNode =======================================================

class FctNameNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitFctName(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitFctName(this); }

  // Other methods
  GET_CHILDREN();
  [[nodiscard]] constexpr bool isOperatorOverload() const { return name.starts_with(OP_FCT_PREFIX); }
  [[nodiscard]] bool supportsInverseOperator() const { return name == OP_FCT_EQUAL || name == OP_FCT_NOT_EQUAL; }

  // Public members
  std::string name;
  std::string structName;
  std::string fqName;
  std::vector<std::string> nameFragments;
};

// ======================================================== FctDefBaseNode =======================================================

class FctDefBaseNode : public TopLevelDefNode {
public:
  // Constructors
  using TopLevelDefNode::TopLevelDefNode;

  // Other methods
  [[nodiscard]] std::string getSymbolTableEntryName() const { return Function::getSymbolTableEntryName(name->name, codeLoc); }
  std::vector<Function *> *getFctManifestations(const std::string &) override { return &manifestations; }
  [[nodiscard]] bool isFctOrProcDef() const override { return true; }
  bool returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const override;

  // Public members
  TopLevelDefinitionAttrNode *attrs = nullptr;
  SpecifierLstNode *specifierLst = nullptr;
  FctNameNode *name;
  TypeLstNode *templateTypeLst = nullptr;
  ParamLstNode *paramLst = nullptr;
  StmtLstNode *body = nullptr;
  bool isMethod = false;
  bool hasTemplateTypes = false;
  bool hasParams = false;
  TypeSpecifiers specifiers = TypeSpecifiers::of(TY_FUNCTION);
  SymbolTableEntry *entry = nullptr;
  Scope *structScope = nullptr;
  Scope *scope = nullptr;
  std::vector<Function *> manifestations;
};

// ========================================================== FctDefNode =========================================================

class FctDefNode final : public FctDefBaseNode {
public:
  // Constructors
  using FctDefBaseNode::FctDefBaseNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitFctDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitFctDef(this); }

  // Other methods
  GET_CHILDREN(attrs, specifierLst, returnType, name, templateTypeLst, paramLst, body);
  [[nodiscard]] std::string getScopeId() const { return "fct:" + codeLoc.toString(); }

  // Public members
  DataTypeNode *returnType = nullptr;
};

// ========================================================== ProcDefNode ========================================================

class ProcDefNode final : public FctDefBaseNode {
public:
  // Constructors
  using FctDefBaseNode::FctDefBaseNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitProcDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitProcDef(this); }

  // Other methods
  GET_CHILDREN(attrs, specifierLst, name, templateTypeLst, paramLst, body);
  [[nodiscard]] std::string getScopeId() const { return "proc:" + codeLoc.toString(); }

  // Public members
  bool isCtor = false;
};

// ========================================================= StructDefNode =======================================================

class StructDefNode final : public TopLevelDefNode {
public:
  // Constructors
  using TopLevelDefNode::TopLevelDefNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitStructDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitStructDef(this); }

  // Other methods
  GET_CHILDREN(attrs, specifierLst, templateTypeLst, interfaceTypeLst, fields);
  std::vector<Struct *> *getStructManifestations() override { return &structManifestations; }
  std::vector<Function *> *getFctManifestations(const std::string &fctName) override {
    if (!defaultFctManifestations.contains(fctName))
      defaultFctManifestations.insert({fctName, {}});
    return &defaultFctManifestations.at(fctName);
  }
  [[nodiscard]] bool isStructDef() const override { return true; }

  // Public members
  TopLevelDefinitionAttrNode *attrs = nullptr;
  SpecifierLstNode *specifierLst = nullptr;
  TypeLstNode *templateTypeLst = nullptr;
  TypeLstNode *interfaceTypeLst = nullptr;
  std::vector<FieldNode *> fields;
  bool hasTemplateTypes = false;
  bool hasInterfaces = false;
  bool emitVTable = false;
  TypeSpecifiers structSpecifiers = TypeSpecifiers::of(TY_STRUCT);
  std::string structName;
  uint64_t typeId;
  SymbolTableEntry *entry = nullptr;
  std::vector<Struct *> structManifestations;
  std::map<const std::string, std::vector<Function *>> defaultFctManifestations;
  Scope *structScope = nullptr;
};

// ======================================================= InterfaceDefNode ======================================================

class InterfaceDefNode final : public TopLevelDefNode {
public:
  // Constructors
  using TopLevelDefNode::TopLevelDefNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitInterfaceDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitInterfaceDef(this); }

  // Other methods
  GET_CHILDREN(attrs, specifierLst, templateTypeLst, signatures);
  std::vector<Interface *> *getInterfaceManifestations() override { return &interfaceManifestations; }

  // Public members
  TopLevelDefinitionAttrNode *attrs = nullptr;
  SpecifierLstNode *specifierLst = nullptr;
  TypeLstNode *templateTypeLst = nullptr;
  std::vector<SignatureNode *> signatures;
  bool hasTemplateTypes = false;
  TypeSpecifiers interfaceSpecifiers = TypeSpecifiers::of(TY_INTERFACE);
  std::string interfaceName;
  uint64_t typeId;
  SymbolTableEntry *entry = nullptr;
  std::vector<Interface *> interfaceManifestations;
  Scope *interfaceScope = nullptr;
};

// ========================================================== EnumDefNode ========================================================

class EnumDefNode final : public TopLevelDefNode {
public:
  // Constructors
  using TopLevelDefNode::TopLevelDefNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitEnumDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitEnumDef(this); }

  // Other methods
  GET_CHILDREN(specifierLst, itemLst);

  // Public members
  SpecifierLstNode *specifierLst = nullptr;
  EnumItemLstNode *itemLst = nullptr;
  TypeSpecifiers enumSpecifiers = TypeSpecifiers::of(TY_ENUM);
  std::string enumName;
  uint64_t typeId;
  SymbolTableEntry *entry = nullptr;
  Scope *enumScope;
};

// ====================================================== GenericTypeDefNode =====================================================

class GenericTypeDefNode final : public TopLevelDefNode {
public:
  // Constructors
  using TopLevelDefNode::TopLevelDefNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitGenericTypeDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitGenericTypeDef(this); }

  // Other methods
  GET_CHILDREN(typeAltsLst);

  // Public members
  TypeAltsLstNode *typeAltsLst = nullptr;
  std::string typeName;
  SymbolTableEntry *entry = nullptr;
};

// ========================================================= AliasDefNode ========================================================

class AliasDefNode final : public TopLevelDefNode {
public:
  // Constructors
  using TopLevelDefNode::TopLevelDefNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAliasDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAliasDef(this); }

  // Other methods
  GET_CHILDREN(specifierLst, dataType);

  // Public members
  SpecifierLstNode *specifierLst = nullptr;
  DataTypeNode *dataType = nullptr;
  TypeSpecifiers aliasSpecifiers = TypeSpecifiers::of(TY_ALIAS);
  std::string aliasName;
  std::string dataTypeString;
  uint64_t typeId;
  SymbolTableEntry *entry = nullptr;
  SymbolTableEntry *aliasedTypeContainerEntry = nullptr;
};

// ======================================================= GlobalVarDefNode ======================================================

class GlobalVarDefNode final : public TopLevelDefNode {
public:
  // Constructors
  using TopLevelDefNode::TopLevelDefNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitGlobalVarDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitGlobalVarDef(this); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override { return true; }
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;

  // Other methods
  GET_CHILDREN(dataType, constant);

  // Public members
  DataTypeNode *dataType = nullptr;
  ConstantNode *constant = nullptr;
  bool hasValue = false;
  std::string varName;
  SymbolTableEntry *entry = nullptr;
};

// ========================================================== ExtDeclNode ========================================================

class ExtDeclNode final : public TopLevelDefNode {
public:
  // Constructors
  using TopLevelDefNode::TopLevelDefNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitExtDecl(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitExtDecl(this); }

  // Other methods
  GET_CHILDREN(attrs, returnType, argTypeLst);
  std::vector<Function *> *getFctManifestations(const std::string &) override { return &extFunctionManifestations; }
  [[nodiscard]] std::string getScopeId() const {
    const char *prefix = hasReturnType ? "func:" : "proc:";
    return prefix + codeLoc.toString();
  }

  // Public members
  TopLevelDefinitionAttrNode *attrs = nullptr;
  DataTypeNode *returnType = nullptr;
  TypeLstNode *argTypeLst = nullptr;
  bool hasArgs = false;
  bool isVarArg = false;
  bool hasReturnType = false;
  std::string extFunctionName;
  SymbolTableEntry *entry = nullptr;
  Function *extFunction = nullptr;
  std::vector<Function *> extFunctionManifestations;
};

// ======================================================== ImportDefNode ========================================================

class ImportDefNode final : public TopLevelDefNode {
public:
  // Constructors
  using TopLevelDefNode::TopLevelDefNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitImportDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitImportDef(this); }

  // Other methods
  GET_CHILDREN();

  // Public members
  std::string importPath;
  std::string importName;
  SymbolTableEntry *entry = nullptr;
};

// ======================================================== UnsafeBlockNode ======================================================

class UnsafeBlockNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitUnsafeBlock(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitUnsafeBlockDef(this); }

  // Other methods
  GET_CHILDREN(body);
  [[nodiscard]] std::string getScopeId() const { return "unsafe:" + codeLoc.toString(); }

  // Public members
  StmtLstNode *body = nullptr;
  Scope *bodyScope = nullptr;
};

// ========================================================== ForLoopNode ========================================================

class ForLoopNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitForLoop(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitForLoop(this); }

  // Other methods
  GET_CHILDREN(initDecl, condAssign, incAssign, body);
  [[nodiscard]] std::string getScopeId() const { return "for:" + codeLoc.toString(); }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const override;

  // Public members
  DeclStmtNode *initDecl = nullptr;
  AssignExprNode *condAssign = nullptr;
  AssignExprNode *incAssign = nullptr;
  StmtLstNode *body = nullptr;
  Scope *bodyScope = nullptr;
};

// ======================================================== ForeachLoopNode ======================================================

class ForeachLoopNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitForeachLoop(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitForeachLoop(this); }

  // Other methods
  GET_CHILDREN(idxVarDecl, itemVarDecl, iteratorAssign, body);
  [[nodiscard]] std::string getScopeId() const { return "foreach:" + codeLoc.toString(); }

  // Public members
  DeclStmtNode *idxVarDecl = nullptr;
  DeclStmtNode *itemVarDecl = nullptr;
  AssignExprNode *iteratorAssign = nullptr;
  StmtLstNode *body = nullptr;
  Scope *bodyScope = nullptr;
  Function *getIteratorFct = nullptr;
  Function *getFct = nullptr;
  Function *getIdxFct = nullptr;
  Function *isValidFct = nullptr;
  Function *nextFct = nullptr;
};

// ========================================================= WhileLoopNode =======================================================

class WhileLoopNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitWhileLoop(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitWhileLoop(this); }

  // Other methods
  GET_CHILDREN(condition, body);
  [[nodiscard]] std::string getScopeId() const { return "while:" + codeLoc.toString(); }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const override;

  // Public members
  AssignExprNode *condition = nullptr;
  StmtLstNode *body = nullptr;
  Scope *bodyScope = nullptr;
};

// ======================================================== DoWhileLoopNode ======================================================

class DoWhileLoopNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitDoWhileLoop(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitDoWhileLoop(this); }

  // Other methods
  GET_CHILDREN(body, condition);
  [[nodiscard]] std::string getScopeId() const { return "dowhile:" + codeLoc.toString(); }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const override;

  // Public members
  StmtLstNode *body = nullptr;
  AssignExprNode *condition = nullptr;
  Scope *bodyScope = nullptr;
};

// ========================================================== IfStmtNode =========================================================

class IfStmtNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitIfStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitIfStmt(this); }

  // Other methods
  GET_CHILDREN(condition, thenBody, elseStmt);
  [[nodiscard]] std::string getScopeId() const { return "if:" + codeLoc.toString(); }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const override;

  // Public members
  AssignExprNode *condition = nullptr;
  StmtLstNode *thenBody = nullptr;
  ElseStmtNode *elseStmt = nullptr;
  Scope *thenBodyScope = nullptr;
};

// ========================================================= ElseStmtNode ========================================================

class ElseStmtNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitElseStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitElseStmt(this); }

  // Other methods
  GET_CHILDREN(ifStmt, body);
  [[nodiscard]] std::string getScopeId() const { return "if:" + codeLoc.toString(); }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const override;

  // Public members
  bool isElseIf = false;
  IfStmtNode *ifStmt = nullptr;
  StmtLstNode *body = nullptr;
  Scope *elseBodyScope = nullptr;
};

// ======================================================== SwitchStmtNode =======================================================

class SwitchStmtNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitSwitchStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitSwitchStmt(this); }

  // Other methods
  GET_CHILDREN(assignExpr, caseBranches, defaultBranch);
  [[nodiscard]] bool returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const override;

  // Public members
  AssignExprNode *assignExpr = nullptr;
  std::vector<CaseBranchNode *> caseBranches;
  DefaultBranchNode *defaultBranch = nullptr;
  bool hasDefaultBranch = false;
};

// ======================================================== CaseBranchNode =======================================================

class CaseBranchNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitCaseBranch(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitCaseBranch(this); }

  // Other methods
  GET_CHILDREN(caseConstants, body);
  [[nodiscard]] std::string getScopeId() const { return "case:" + codeLoc.toString(); }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const override;

  // Public members
  std::vector<CaseConstantNode *> caseConstants;
  StmtLstNode *body = nullptr;
  Scope *bodyScope = nullptr;
};

// ======================================================= DefaultBranchNode =====================================================

class DefaultBranchNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitDefaultBranch(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitDefaultBranch(this); }

  // Other methods
  GET_CHILDREN(body);
  [[nodiscard]] std::string getScopeId() const { return "default:" + codeLoc.toString(); }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const override;

  // Public members
  StmtLstNode *body = nullptr;
  Scope *bodyScope = nullptr;
};

// ==================================================== AnonymousBlockStmtNode ===================================================

class AnonymousBlockStmtNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAnonymousBlockStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAnonymousBlockStmt(this); }

  // Other methods
  GET_CHILDREN(body);
  [[nodiscard]] std::string getScopeId() const { return "anon:" + codeLoc.toString(); }

  // Public members
  StmtLstNode *body = nullptr;
  Scope *bodyScope = nullptr;
};

// ========================================================= StmtLstNode =========================================================

class StmtLstNode final : public ASTNode {
public:
  // Structs
  struct ResourcesForManifestationToCleanup {
    std::vector<std::pair<SymbolTableEntry *, Function *>> dtorFunctionsToCall;
    std::vector<SymbolTableEntry *> heapVarsToFree;
  };

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitStmtLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitStmtLst(this); }

  // Other methods
  GET_CHILDREN(statements);
  [[nodiscard]] bool returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const override;
  void customItemsInitialization(const size_t manifestationCount) override { resourcesToCleanup.resize(manifestationCount); }
  [[nodiscard]] bool isStmtLst() const override { return true; }

  // Public members
  std::vector<StmtNode *> statements;
  size_t complexity = 0;
  std::vector<ResourcesForManifestationToCleanup> resourcesToCleanup;
  CodeLoc closingBraceCodeLoc = CodeLoc(1, 0);
};

// ========================================================= TypeLstNode =========================================================

class TypeLstNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitTypeLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitTypeLst(this); }

  // Other methods
  GET_CHILDREN(dataTypes);

  // Public members
  std::vector<DataTypeNode *> dataTypes;
};

// ======================================================= TypeAltsLstNode =======================================================

class TypeAltsLstNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitTypeAltsLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitTypeAltsLst(this); }

  // Other methods
  GET_CHILDREN(dataTypes);

  // Public members
  std::vector<DataTypeNode *> dataTypes;
};

// ======================================================== ParamLstNode =========================================================

class ParamLstNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitParamLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitParamLst(this); }

  // Other methods
  GET_CHILDREN(params);

  // Public members
  std::vector<DeclStmtNode *> params;
};

// ========================================================== ArgLstNode =========================================================

class ArgLstNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitArgLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitArgLst(this); }

  // Other methods
  GET_CHILDREN(args);

  // Public members
  std::vector<AssignExprNode *> args;
};

// ======================================================== EnumItemLstNode ======================================================

class EnumItemLstNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitEnumItemLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitEnumItemLst(this); }

  // Other methods
  GET_CHILDREN(items);

  // Public members
  std::vector<EnumItemNode *> items;
};

// ========================================================= EnumItemNode ========================================================

class EnumItemNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitEnumItem(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitEnumItem(this); }

  // Other methods
  GET_CHILDREN();

  // Public members
  bool hasValue = false;
  uint32_t itemValue;
  std::string itemName;
  SymbolTableEntry *entry = nullptr;
  EnumDefNode *enumDef = nullptr;
};

// ========================================================== FieldNode ==========================================================

class FieldNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitField(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitField(this); }

  // Other methods
  GET_CHILDREN(dataType, defaultValue);

  // Public members
  DataTypeNode *dataType = nullptr;
  TernaryExprNode *defaultValue = nullptr;
  std::string fieldName;
  SymbolTableEntry *entry = nullptr;
};

// ======================================================== SignatureNode ========================================================

class SignatureNode final : public ASTNode {
public:
  // Enums
  enum SignatureType : uint8_t {
    TYPE_NONE,
    TYPE_FUNCTION,
    TYPE_PROCEDURE,
  };

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitSignature(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitSignature(this); }

  // Other methods
  GET_CHILDREN(specifierLst, returnType, templateTypeLst, paramTypeLst);
  std::vector<Function *> *getFctManifestations(const std::string &) override { return &signatureManifestations; }

  // Public members
  SpecifierLstNode *specifierLst = nullptr;
  DataTypeNode *returnType = nullptr;
  TypeLstNode *templateTypeLst = nullptr;
  TypeLstNode *paramTypeLst = nullptr;
  bool hasReturnType = false;
  bool hasTemplateTypes = false;
  bool hasParams = false;
  SignatureType signatureType = TYPE_NONE;
  TypeSpecifiers signatureSpecifiers;
  std::string methodName;
  SymbolTableEntry *entry = nullptr;
  std::vector<Function *> signatureManifestations;
};

// ========================================================= DeclStmtNode ========================================================

class DeclStmtNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitDeclStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitDeclStmt(this); }

  // Other methods
  GET_CHILDREN(dataType, assignExpr);
  void customItemsInitialization(const size_t manifestationCount) override { entries.resize(manifestationCount); }
  [[nodiscard]] bool isParam() const override { return isFctParam; }

  // Public members
  DataTypeNode *dataType = nullptr;
  AssignExprNode *assignExpr = nullptr;
  bool hasAssignment = false;
  bool isFctParam = false;
  bool isForEachItem = false;
  bool isCtorCallRequired = false; // For struct, in case there are reference fields, we need to call a user-defined ctor
  std::string varName;
  std::vector<SymbolTableEntry *> entries;
  Function *calledInitCtor = nullptr;
  Function *calledCopyCtor = nullptr;
};

// ========================================================= ExprStmtNode ========================================================

class ExprStmtNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitExprStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitExprStmt(this); }

  // Other methods
  GET_CHILDREN(expr);
  [[nodiscard]] bool isExprStmt() const override { return true; }

  // Public members
  AssignExprNode *expr = nullptr;
};

// ======================================================= SpecifierLstNode ======================================================

class SpecifierLstNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitSpecifierLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitSpecifierLst(this); }

  // Other methods
  GET_CHILDREN(specifiers);

  // Public members
  std::vector<SpecifierNode *> specifiers;
};

// ========================================================= SpecifierNode =======================================================

class SpecifierNode final : public ASTNode {
public:
  // Enums
  enum SpecifierType : uint8_t {
    TY_NONE,
    TY_CONST,
    TY_SIGNED,
    TY_UNSIGNED,
    TY_INLINE,
    TY_PUBLIC,
    TY_HEAP,
    TY_COMPOSITION,
  };

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitSpecifier(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitSpecifier(this); }

  // Other methods
  GET_CHILDREN();

  // Public members
  SpecifierType type = TY_NONE;
};

// ========================================================== ModAttrNode ========================================================

class ModAttrNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitModAttr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitModAttr(this); }

  // Other methods
  GET_CHILDREN(attrLst);

  // Public members
  AttrLstNode *attrLst = nullptr;
};

// =================================================== TopLevelDefinitionAttrNode ================================================

class TopLevelDefinitionAttrNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitTopLevelDefinitionAttr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitTopLevelDefinitionAttr(this); }

  // Other methods
  GET_CHILDREN(attrLst);

  // Public members
  AttrLstNode *attrLst = nullptr;
};

// ========================================================= LambdaAttrNode ======================================================

class LambdaAttrNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitLambdaAttr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitLambdaAttr(this); }

  // Other methods
  GET_CHILDREN(attrLst);

  // Public members
  AttrLstNode *attrLst = nullptr;
};

// ========================================================== AttrLstNode ========================================================

class AttrLstNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAttrLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAttrLst(this); }

  // Other methods
  GET_CHILDREN(attributes);
  [[nodiscard]] std::vector<const CompileTimeValue *> getAttrValuesByName(const std::string &key) const;
  [[nodiscard]] const CompileTimeValue *getAttrValueByName(const std::string &key) const;
  [[nodiscard]] bool hasAttr(const std::string &key) const;

  // Public members
  std::vector<AttrNode *> attributes;
};

// ============================================================ AttrNode =========================================================

class AttrNode final : public ASTNode {
public:
  // Enums
  enum AttrTarget : uint8_t {
    TARGET_INVALID = 0,
    TARGET_MODULE = 1 << 0,
    TARGET_STRUCT = 1 << 1,
    TARGET_INTERFACE = 1 << 2,
    TARGET_FCT_PROC = 1 << 3,
    TARGET_EXT_DECL = 1 << 4,
    TARGET_LAMBDA = 1 << 5,
  };

  enum AttrType : uint8_t {
    ATTR_TYPE_INVALID,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_INT,
  };

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAttr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAttr(this); }

  // Other methods
  GET_CHILDREN(value);
  [[nodiscard]] const CompileTimeValue *getValue() const;

  // Public members
  ConstantNode *value = nullptr;
  AttrType type = ATTR_TYPE_INVALID;
  AttrTarget target = TARGET_INVALID;
  std::string key;
};

// ======================================================== CaseConstantNode =====================================================

class CaseConstantNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitCaseConstant(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitCaseConstant(this); }

  // Other methods
  GET_CHILDREN(constant);

  // Public members
  ConstantNode *constant = nullptr;
  std::vector<std::string> identifierFragments;
  std::string fqIdentifier;
  const SymbolTableEntry *enumItemEntry = nullptr;
};

// ======================================================== ReturnStmtNode =======================================================

class ReturnStmtNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitReturnStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitReturnStmt(this); }

  // Other methods
  GET_CHILDREN(assignExpr);
  [[nodiscard]] bool returnsOnAllControlPaths(bool *) const override { return true; }
  [[nodiscard]] StmtLstNode *getParentScopeNode() const { return spice_pointer_cast<StmtLstNode *>(parent); }

  // Public members
  AssignExprNode *assignExpr = nullptr;
  bool hasReturnValue = false;
};

// ======================================================== BreakStmtNode ========================================================

class BreakStmtNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitBreakStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitBreakStmt(this); }

  // Other methods
  GET_CHILDREN();

  // Public members
  int breakTimes = 1;
};

// ======================================================= ContinueStmtNode ======================================================

class ContinueStmtNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitContinueStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitContinueStmt(this); }

  // Other methods
  GET_CHILDREN();

  // Public members
  int continueTimes = 1;
};

// ====================================================== FallthroughStmtNode ====================================================

class FallthroughStmtNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitFallthroughStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitFallthroughStmt(this); }

  // Other methods
  GET_CHILDREN();
};

// ======================================================== AssertStmtNode =======================================================

class AssertStmtNode final : public StmtNode {
public:
  // Constructors
  using StmtNode::StmtNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAssertStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAssertStmt(this); }

  // Other methods
  GET_CHILDREN(assignExpr);
  [[nodiscard]] bool returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const override;

  // Public members
  AssignExprNode *assignExpr = nullptr;
  std::string expressionString;
};

// ======================================================== BuiltinCallNode ======================================================

class BuiltinCallNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitBuiltinCall(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitBuiltinCall(this); }

  // Other methods
  GET_CHILDREN(printfCall, sizeofCall, alignofCall, lenCall, panicCall, sysCall);

  // Public members
  PrintfCallNode *printfCall = nullptr;
  SizeofCallNode *sizeofCall = nullptr;
  AlignofCallNode *alignofCall = nullptr;
  LenCallNode *lenCall = nullptr;
  PanicCallNode *panicCall = nullptr;
  SysCallNode *sysCall = nullptr;
};

// ======================================================== PrintfCallNode =======================================================

class PrintfCallNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitPrintfCall(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitPrintfCall(this); }

  // Other methods
  GET_CHILDREN(args);
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }

  // Public members
  std::vector<AssignExprNode *> args;
  std::string templatedString;
};

// ======================================================== SizeofCallNode =======================================================

class SizeofCallNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitSizeofCall(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitSizeofCall(this); }

  // Other methods
  GET_CHILDREN(assignExpr, dataType);
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }

  // Public members
  union {
    AssignExprNode *assignExpr = nullptr;
    DataTypeNode *dataType;
  };
  bool isType = false;
};

// ======================================================== AlignofCallNode ======================================================

class AlignofCallNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAlignofCall(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAlignofCall(this); }

  // Other methods
  GET_CHILDREN(assignExpr, dataType);
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }

  // Public members
  union {
    AssignExprNode *assignExpr = nullptr;
    DataTypeNode *dataType;
  };
  bool isType = false;
};

// ========================================================= LenCallNode =========================================================

class LenCallNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitLenCall(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitLenCall(this); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }

  // Other methods
  GET_CHILDREN(assignExpr);

  // Public members
  AssignExprNode *assignExpr = nullptr;
};

// ======================================================== PanicCallNode ========================================================

class PanicCallNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitPanicCall(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitPanicCall(this); }

  // Other methods
  GET_CHILDREN(assignExpr);
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *) const override { return true; }

  // Public members
  AssignExprNode *assignExpr = nullptr;
};

// ========================================================= SysCallNode =========================================================

class SysCallNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitSysCall(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitSysCall(this); }

  // Other methods
  GET_CHILDREN(args);

  // Public members
  std::vector<AssignExprNode *> args;
};

// ======================================================= AssignExprNode ========================================================

class AssignExprNode final : public ExprNode {
public:
  // Enums
  enum AssignOp : uint8_t {
    OP_NONE,
    OP_ASSIGN,
    OP_PLUS_EQUAL,
    OP_MINUS_EQUAL,
    OP_MUL_EQUAL,
    OP_DIV_EQUAL,
    OP_REM_EQUAL,
    OP_SHL_EQUAL,
    OP_SHR_EQUAL,
    OP_AND_EQUAL,
    OP_OR_EQUAL,
    OP_XOR_EQUAL
  };

  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAssignExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAssignExpr(this); }

  // Other methods
  GET_CHILDREN(lhs, rhs, ternaryExpr);
  [[nodiscard]] bool returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const override;
  [[nodiscard]] bool isAssignExpr() const override { return true; }
  [[nodiscard]] std::vector<std::vector<const Function *>> *getOpFctPointers() override { return &opFct; }
  [[nodiscard]] const std::vector<std::vector<const Function *>> *getOpFctPointers() const override { return &opFct; }
  void customItemsInitialization(const size_t manifestationCount) override { opFct.resize(manifestationCount, {nullptr}); }

  // Public members
  PrefixUnaryExprNode *lhs = nullptr;
  AssignExprNode *rhs = nullptr;
  TernaryExprNode *ternaryExpr = nullptr;
  AssignOp op = OP_NONE;
  std::vector<std::vector<const Function *>> opFct; // Operator overloading functions
};

// ======================================================= TernaryExprNode =======================================================

class TernaryExprNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitTernaryExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitTernaryExpr(this); }

  // Other methods
  GET_CHILDREN(condition, trueExpr, falseExpr);
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;

  // Public members
  LogicalOrExprNode *condition = nullptr;
  LogicalOrExprNode *trueExpr = nullptr;
  LogicalOrExprNode *falseExpr = nullptr;
  bool isShortened = false;
};

// ===================================================== LogicalOrExprNode =======================================================

class LogicalOrExprNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitLogicalOrExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitLogicalOrExpr(this); }

  // Other methods
  GET_CHILDREN(operands);

  // Public members
  std::vector<LogicalAndExprNode *> operands;
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;
};

// ===================================================== LogicalAndExprNode ======================================================

class LogicalAndExprNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitLogicalAndExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitLogicalAndExpr(this); }

  // Other methods
  GET_CHILDREN(operands);

  // Public members
  std::vector<BitwiseOrExprNode *> operands;
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;
};

// ===================================================== BitwiseOrExprNode =======================================================

class BitwiseOrExprNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitBitwiseOrExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitBitwiseOrExpr(this); }

  // Other methods
  GET_CHILDREN(operands);

  // Public members
  std::vector<BitwiseXorExprNode *> operands;
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;
};

// ==================================================== BitwiseXorExprNode =======================================================

class BitwiseXorExprNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitBitwiseXorExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitBitwiseXorExpr(this); }

  // Other methods
  GET_CHILDREN(operands);

  // Public members
  std::vector<BitwiseAndExprNode *> operands;
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;
};

// ==================================================== BitwiseAndExprNode =======================================================

class BitwiseAndExprNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitBitwiseAndExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitBitwiseAndExpr(this); }

  // Other methods
  GET_CHILDREN(operands);

  // Public members
  std::vector<EqualityExprNode *> operands;
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;
};

// ===================================================== EqualityExprNode ========================================================

class EqualityExprNode final : public ExprNode {
public:
  // Enums
  enum EqualityOp : uint8_t {
    OP_NONE,
    OP_EQUAL,
    OP_NOT_EQUAL,
  };

  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitEqualityExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitEqualityExpr(this); }

  // Other methods
  GET_CHILDREN(operands);
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;
  [[nodiscard]] std::vector<std::vector<const Function *>> *getOpFctPointers() override { return &opFct; }
  [[nodiscard]] const std::vector<std::vector<const Function *>> *getOpFctPointers() const override { return &opFct; }
  void customItemsInitialization(const size_t manifestationCount) override { opFct.resize(manifestationCount, {nullptr}); }

  // Public members
  std::vector<RelationalExprNode *> operands;
  EqualityOp op = OP_NONE;
  std::vector<std::vector<const Function *>> opFct; // Operator overloading functions
};

// ==================================================== RelationalExprNode =======================================================

class RelationalExprNode final : public ExprNode {
public:
  // Enums
  enum RelationalOp : uint8_t {
    OP_NONE,
    OP_LESS,
    OP_GREATER,
    OP_LESS_EQUAL,
    OP_GREATER_EQUAL,
  };

  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitRelationalExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitRelationalExpr(this); }

  // Other methods
  GET_CHILDREN(operands);
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;

  // Public members
  std::vector<ShiftExprNode *> operands;
  RelationalOp op = OP_NONE;
};

// ====================================================== ShiftExprNode ==========================================================

class ShiftExprNode final : public ExprNode {
public:
  // Enums
  enum ShiftOp : uint8_t {
    OP_NONE,
    OP_SHIFT_LEFT,
    OP_SHIFT_RIGHT,
  };

  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitShiftExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitShiftExpr(this); }

  // Other methods
  GET_CHILDREN(operands);
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;
  [[nodiscard]] std::vector<std::vector<const Function *>> *getOpFctPointers() override { return &opFct; }
  [[nodiscard]] const std::vector<std::vector<const Function *>> *getOpFctPointers() const override { return &opFct; }
  void customItemsInitialization(const size_t manifestationCount) override { opFct.resize(manifestationCount, {nullptr}); }

  // Public members
  std::vector<AdditiveExprNode *> operands;
  ShiftOp op = OP_NONE;
  std::vector<std::vector<const Function *>> opFct; // Operator overloading functions
};

// ==================================================== AdditiveExprNode =========================================================

class AdditiveExprNode final : public ExprNode {
public:
  // Enums
  enum AdditiveOp : uint8_t {
    OP_PLUS,
    OP_MINUS,
  };

  // Typedefs
  using OpQueue = std::queue<std::pair<AdditiveOp, QualType>>;

  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAdditiveExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAdditiveExpr(this); }

  // Other methods
  GET_CHILDREN(operands);
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;
  [[nodiscard]] std::vector<std::vector<const Function *>> *getOpFctPointers() override { return &opFct; }
  [[nodiscard]] const std::vector<std::vector<const Function *>> *getOpFctPointers() const override { return &opFct; }
  void customItemsInitialization(const size_t manifestationCount) override { opFct.resize(manifestationCount, {nullptr}); }

  // Public members
  std::vector<MultiplicativeExprNode *> operands;
  OpQueue opQueue;
  std::vector<std::vector<const Function *>> opFct; // Operator overloading functions
};

// ================================================== MultiplicativeExprNode =====================================================

class MultiplicativeExprNode final : public ExprNode {
public:
  // Enums
  enum MultiplicativeOp : uint8_t {
    OP_MUL,
    OP_DIV,
    OP_REM,
  };

  // Typedefs
  using OpQueue = std::queue<std::pair<MultiplicativeOp, QualType>>;

  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitMultiplicativeExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitMultiplicativeExpr(this); }

  // Other methods
  GET_CHILDREN(operands);
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;
  [[nodiscard]] std::vector<std::vector<const Function *>> *getOpFctPointers() override { return &opFct; }
  [[nodiscard]] const std::vector<std::vector<const Function *>> *getOpFctPointers() const override { return &opFct; }
  void customItemsInitialization(const size_t manifestationCount) override { opFct.resize(manifestationCount, {nullptr}); }

  // Public members
  std::vector<CastExprNode *> operands;
  OpQueue opQueue;
  std::vector<std::vector<const Function *>> opFct; // Operator overloading functions
};

// ======================================================= CastExprNode ==========================================================

class CastExprNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitCastExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitCastExpr(this); }

  // Other methods
  GET_CHILDREN(dataType, prefixUnaryExpr);
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;

  // Public members
  DataTypeNode *dataType = nullptr;
  PrefixUnaryExprNode *prefixUnaryExpr = nullptr;
  bool isCast = false;
};

// ==================================================== PrefixUnaryExprNode ======================================================

class PrefixUnaryExprNode final : public ExprNode {
public:
  // Enums
  enum PrefixUnaryOp : uint8_t {
    OP_NONE,
    OP_MINUS,
    OP_PLUS_PLUS,
    OP_MINUS_MINUS,
    OP_NOT,
    OP_BITWISE_NOT,
    OP_DEREFERENCE,
    OP_ADDRESS_OF,
  };

  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitPrefixUnaryExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitPrefixUnaryExpr(this); }

  // Other methods
  GET_CHILDREN(prefixUnaryExpr, postfixUnaryExpr);
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;

  // Public members
  PrefixUnaryExprNode *prefixUnaryExpr = nullptr;
  PostfixUnaryExprNode *postfixUnaryExpr = nullptr;
  PrefixUnaryOp op = OP_NONE;
};

// =================================================== PostfixUnaryExprNode ======================================================

class PostfixUnaryExprNode final : public ExprNode {
public:
  // Enums
  enum PostfixUnaryOp : uint8_t {
    OP_NONE,
    OP_SUBSCRIPT,
    OP_MEMBER_ACCESS,
    OP_PLUS_PLUS,
    OP_MINUS_MINUS,
  };

  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitPostfixUnaryExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitPostfixUnaryExpr(this); }

  // Other methods
  GET_CHILDREN(atomicExpr, postfixUnaryExpr, subscriptIndexExpr);
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;
  [[nodiscard]] std::vector<std::vector<const Function *>> *getOpFctPointers() override { return &opFct; }
  [[nodiscard]] const std::vector<std::vector<const Function *>> *getOpFctPointers() const override { return &opFct; }
  void customItemsInitialization(const size_t manifestationCount) override { opFct.resize(manifestationCount, {nullptr}); }

  // Public members
  AtomicExprNode *atomicExpr = nullptr;
  PostfixUnaryExprNode *postfixUnaryExpr = nullptr;
  AssignExprNode *subscriptIndexExpr = nullptr;
  PostfixUnaryOp op = OP_NONE;
  std::vector<std::vector<const Function *>> opFct; // Operator overloading functions
  std::string identifier;                           // Only set when operator is member access
};

// ====================================================== AtomicExprNode =========================================================

class AtomicExprNode final : public ExprNode {
public:
  // Structs
  struct VarAccessData {
    SymbolTableEntry *entry = nullptr;
    Scope *accessScope = nullptr;
    Capture *capture = nullptr;
  };

  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAtomicExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAtomicExpr(this); }

  // Other methods
  GET_CHILDREN(constant, value, assignExpr, builtinCall);
  void customItemsInitialization(const size_t manifestationCount) override { data.resize(manifestationCount); }

  // Public members
  ConstantNode *constant = nullptr;
  ValueNode *value = nullptr;
  AssignExprNode *assignExpr = nullptr;
  BuiltinCallNode *builtinCall = nullptr;
  std::vector<std::string> identifierFragments;
  std::string fqIdentifier;
  std::vector<VarAccessData> data; // Only set if identifier is set as well
};

// ======================================================== ValueNode ============================================================

class ValueNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitValue(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitValue(this); }

  // Other methods
  GET_CHILDREN(fctCall, arrayInitialization, structInstantiation, lambdaFunc, lambdaProc, lambdaExpr, nilType);
  [[nodiscard]] bool hasCompileTimeValue() const override { return isNil; }

  // Public members
  FctCallNode *fctCall = nullptr;
  ArrayInitializationNode *arrayInitialization = nullptr;
  StructInstantiationNode *structInstantiation = nullptr;
  LambdaFuncNode *lambdaFunc = nullptr;
  LambdaProcNode *lambdaProc = nullptr;
  LambdaExprNode *lambdaExpr = nullptr;
  DataTypeNode *nilType = nullptr;
  bool isNil = false;
};

// ====================================================== ConstantNode ===========================================================

class ConstantNode final : public ExprNode {
public:
  // Enum
  enum PrimitiveValueType : uint8_t {
    TYPE_NONE,
    TYPE_DOUBLE,
    TYPE_INT,
    TYPE_SHORT,
    TYPE_LONG,
    TYPE_BYTE,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_BOOL
  };

  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitConstant(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitConstant(this); }

  // Other methods
  GET_CHILDREN();
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override { return compileTimeValue; }
  [[nodiscard]] bool hasCompileTimeValue() const override { return true; }

  // Public members
  PrimitiveValueType type = TYPE_NONE;
  CompileTimeValue compileTimeValue;
};

// ====================================================== FctCallNode ============================================================

class FctCallNode final : public ExprNode {
public:
  // Enums
  enum FctCallType : uint8_t {
    TYPE_ORDINARY,
    TYPE_METHOD,
    TYPE_CTOR,
    TYPE_FCT_PTR,
  };

  // Structs
  struct FctCallData {
    // Members
    FctCallType callType = TYPE_ORDINARY;
    bool isImported = false;
    QualType thisType = QualType(TY_DYN); // Is filled if method or ctor call
    std::vector<ExprResult> argResults;
    Function *callee = nullptr;
    Scope *calleeParentScope = nullptr;

    // Methods
    [[nodiscard]] bool isOrdinaryCall() const { return callType == TYPE_ORDINARY; }
    [[nodiscard]] bool isMethodCall() const { return callType == TYPE_METHOD; }
    [[nodiscard]] bool isVirtualMethodCall() const { return isMethodCall() && thisType.isBase(TY_INTERFACE); }
    [[nodiscard]] bool isCtorCall() const { return callType == TYPE_CTOR; }
    [[nodiscard]] bool isFctPtrCall() const { return callType == TYPE_FCT_PTR; }
  };

  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitFctCall(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitFctCall(this); }

  // Other methods
  GET_CHILDREN(templateTypeLst, argLst);
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }
  void customItemsInitialization(const size_t manifestationCount) override { data.resize(manifestationCount); }
  [[nodiscard]] bool hasReturnValueReceiver() const;

  // Public members
  TypeLstNode *templateTypeLst = nullptr;
  ArgLstNode *argLst = nullptr;
  bool hasArgs = false;
  bool hasTemplateTypes = false;
  std::string fqFunctionName;
  std::vector<std::string> functionNameFragments;
  std::vector<FctCallData> data;
};

// ================================================= ArrayInitializationNode =====================================================

class ArrayInitializationNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitArrayInitialization(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitArrayInitialization(this); }

  // Other methods
  GET_CHILDREN(itemLst);

  // Public members
  ArgLstNode *itemLst = nullptr;
  long actualSize = 0;
};

// ================================================= StructInstantiationNode =====================================================

class StructInstantiationNode final : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitStructInstantiation(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitStructInstantiation(this); }

  // Other methods
  GET_CHILDREN(templateTypeLst, fieldLst);
  void customItemsInitialization(const size_t manifestationCount) override { instantiatedStructs.resize(manifestationCount); }

  // Public members
  TypeLstNode *templateTypeLst = nullptr;
  ArgLstNode *fieldLst = nullptr;
  bool hasTemplateTypes = false;
  std::string fqStructName;
  std::vector<std::string> structNameFragments;
  std::vector<Struct *> instantiatedStructs;
};

// ====================================================== LambdaBaseNode =========================================================

class LambdaBaseNode : public ExprNode {
public:
  // Constructors
  using ExprNode::ExprNode;

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "lambda:" + codeLoc.toString(); }
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }
  void customItemsInitialization(const size_t manifestationCount) override { manifestations.resize(manifestationCount); }

  // Public members
  ParamLstNode *paramLst = nullptr;
  bool hasParams = false;
  Scope *bodyScope = nullptr;
  std::vector<Function> manifestations;
};

// ====================================================== LambdaFuncNode =========================================================

class LambdaFuncNode final : public LambdaBaseNode {
public:
  // Constructors
  using LambdaBaseNode::LambdaBaseNode;

  // Visit methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitLambdaFunc(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitLambdaFunc(this); }

  // Other methods
  GET_CHILDREN(returnType, paramLst, body, lambdaAttr);
  [[nodiscard]] bool returnsOnAllControlPaths(bool *overrideUnreachable) const override;

  // Public members
  DataTypeNode *returnType = nullptr;
  StmtLstNode *body = nullptr;
  LambdaAttrNode *lambdaAttr = nullptr;
};

// ====================================================== LambdaProcNode =========================================================

class LambdaProcNode final : public LambdaBaseNode {
public:
  // Constructors
  using LambdaBaseNode::LambdaBaseNode;

  // Visit methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitLambdaProc(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitLambdaProc(this); }

  // Other methods
  GET_CHILDREN(paramLst, body, lambdaAttr);
  bool returnsOnAllControlPaths(bool *overrideUnreachable) const override;

  // Public members
  StmtLstNode *body = nullptr;
  LambdaAttrNode *lambdaAttr = nullptr;
};

// ====================================================== LambdaExprNode =========================================================

class LambdaExprNode final : public LambdaBaseNode {
public:
  // Constructors
  using LambdaBaseNode::LambdaBaseNode;

  // Visit methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitLambdaExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitLambdaExpr(this); }

  // Other methods
  GET_CHILDREN(paramLst, lambdaExpr);

  // Public members
  AssignExprNode *lambdaExpr = nullptr;
};

// ======================================================= DataTypeNode ==========================================================

class DataTypeNode final : public ASTNode {
public:
  // Enums
  enum TypeModifierType : uint8_t {
    TYPE_PTR,
    TYPE_REF,
    TYPE_ARRAY,
  };

  // Structs
  struct TypeModifier {
    TypeModifierType modifierType = TYPE_PTR;
    bool hasSize = false;
    unsigned int hardcodedSize = 0;
    std::string sizeVarName;
  };

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitDataType(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitDataType(this); }

  // Other methods
  GET_CHILDREN(specifierLst, baseDataType);
  void setFieldTypeRecursive();

  // Public members
  SpecifierLstNode *specifierLst = nullptr;
  BaseDataTypeNode *baseDataType = nullptr;
  bool isParamType = false;
  bool isGlobalType = false;
  bool isFieldType = false;
  bool isReturnType = false;
  std::queue<TypeModifier> tmQueue;
};

// ==================================================== BaseDataTypeNode =========================================================

class BaseDataTypeNode final : public ASTNode {
public:
  // Enums
  enum Type : uint8_t {
    TYPE_NONE,
    TYPE_DOUBLE,
    TYPE_INT,
    TYPE_SHORT,
    TYPE_LONG,
    TYPE_BYTE,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_DYN,
    TYPE_CUSTOM,
    TYPE_FUNCTION
  };

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitBaseDataType(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitBaseDataType(this); }

  // Other methods
  GET_CHILDREN(customDataType, functionDataType);

  // Public members
  CustomDataTypeNode *customDataType = nullptr;
  FunctionDataTypeNode *functionDataType = nullptr;
  Type type = TYPE_NONE;
};

// ==================================================== CustomDataTypeNode =======================================================

class CustomDataTypeNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitCustomDataType(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitCustomDataType(this); }

  // Other methods
  GET_CHILDREN(templateTypeLst);
  void customItemsInitialization(const size_t manifestationCount) override { customTypes.resize(manifestationCount); }

  // Public members
  TypeLstNode *templateTypeLst = nullptr;
  std::string fqTypeName;
  std::vector<std::string> typeNameFragments;
  std::vector<SymbolTableEntry *> customTypes;
};

// =================================================== FunctionDataTypeNode ======================================================

class FunctionDataTypeNode final : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitFunctionDataType(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitFunctionDataType(this); }

  // Other methods
  GET_CHILDREN(returnType, paramTypeLst);
  void customItemsInitialization(const size_t manifestationCount) override { customTypes.resize(manifestationCount); }

  // Public members
  DataTypeNode *returnType = nullptr;
  TypeLstNode *paramTypeLst = nullptr;
  bool isFunction = false; // Function or procedure
  std::vector<SymbolTableEntry *> customTypes;
};

} // namespace spice::compiler
// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <queue>
#include <stack>
#include <utility>
#include <vector>

#include <ast/ASTVisitor.h>
#include <ast/ParallelizableASTVisitor.h>
#include <exception/CompilerError.h>
#include <model/Function.h>
#include <model/Struct.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/TypeSpecifiers.h>
#include <util/CodeLoc.h>

namespace spice::compiler {

/**
 * Saves a constant value for an AST node to realize features like array-out-of-bounds checks
 */
union CompileTimeValue {
  std::double_t doubleValue;
  std::int32_t intValue;
  std::int16_t shortValue;
  std::int64_t longValue;
  std::int8_t charValue;
  const char *stringValue;
  bool boolValue;
};

// =========================================================== AstNode ===========================================================

class ASTNode {
public:
  // Constructors
  ASTNode(ASTNode *parent, CodeLoc codeLoc) : parent(parent), codeLoc(std::move(codeLoc)) {}
  ASTNode(const ASTNode &) = delete;

  // Destructors
  virtual ~ASTNode() = default;

  // Virtual methods
  virtual std::any accept(AbstractASTVisitor *visitor) = 0;
  virtual std::any accept(ParallelizableASTVisitor *visitor) const = 0;

  // Public methods
  template <typename T> T *createChild(const CodeLoc &loc) {
    static_assert(std::is_base_of_v<ASTNode, T>, "T must be derived from ASTNode");
    T *node = new T(this, loc);
    children.push_back(node);
    return node;
  }

  template <typename T> void addChild(T *node) {
    static_assert(std::is_base_of_v<ASTNode, T>, "T must be derived from ASTNode");
    children.push_back(node);
  }

  template <typename T> [[nodiscard]] T *getChild(size_t i = 0) const {
    static_assert(std::is_base_of_v<ASTNode, T>, "T must be derived from ASTNode");
    size_t j = 0;
    for (ASTNode *child : children) {
      if (auto *typedChild = dynamic_cast<T *>(child); typedChild != nullptr) {
        if (j++ == i)
          return typedChild;
      }
    }
    return nullptr;
  }

  template <typename T> [[nodiscard]] std::vector<T *> getChildren() const {
    static_assert(std::is_base_of_v<ASTNode, T>, "T must be derived from ASTNode");
    std::vector<T *> nodes;
    for (ASTNode *child : children) {
      if (auto *typedChild = dynamic_cast<T *>(child); typedChild != nullptr)
        nodes.push_back(typedChild);
    }
    return nodes;
  }

  inline void reserveChildren(size_t numberOfChildren) { children.reserve(numberOfChildren); }

  void replaceInParent(ASTNode *replacementNode) {
    assert(parent != nullptr);
    for (auto &child : parent->children) {
      if (child == this) [[unlikely]] {
        // Replace in children vector
        child = replacementNode;
        break;
      }
    }
  }

  void removeFromParent() {
    assert(parent != nullptr);
    for (auto &child : parent->children) {
      if (child == this) [[unlikely]] {
        // Remove from children vector
        child = nullptr;
        break;
      }
    }
  }

  virtual void resizeToNumberOfManifestations(size_t manifestationCount) { // NOLINT(misc-no-recursion)
    // Resize children
    for (ASTNode *child : children)
      if (child != nullptr)
        child->resizeToNumberOfManifestations(manifestationCount);
    // Reserve this node
    symbolTypes.resize(manifestationCount, SymbolType(TY_INVALID));
    // Reserve operator functions
    opFct.resize(manifestationCount, {nullptr});
    // Do custom work
    customItemsInitialization(manifestationCount);
  }

  virtual void customItemsInitialization(size_t) {} // Noop

  SymbolType setEvaluatedSymbolType(const SymbolType &symbolType, const size_t idx) {
    assert(symbolTypes.size() > idx);
    symbolTypes.at(idx) = symbolType;
    return symbolType;
  }

  [[nodiscard]] const SymbolType &getEvaluatedSymbolType(const size_t idx) const { // NOLINT(misc-no-recursion)
    if (!symbolTypes.empty() && !symbolTypes.at(idx).is(TY_INVALID))
      return symbolTypes.at(idx);
    if (children.size() != 1)
      throw CompilerError(INTERNAL_ERROR, "Cannot deduce evaluated symbol type");
    return children.front()->getEvaluatedSymbolType(idx);
  }

  [[nodiscard]] virtual bool hasCompileTimeValue() const { // NOLINT(misc-no-recursion)
    if (hasDirectCompileTimeValue)
      return true;
    if (children.size() != 1)
      return false;
    return children.front()->hasCompileTimeValue();
  }

  [[nodiscard]] virtual CompileTimeValue getCompileTimeValue() const { // NOLINT(misc-no-recursion)
    if (hasDirectCompileTimeValue || children.empty())
      return compileTimeValue;
    return children.front()->getCompileTimeValue();
  }

  [[nodiscard]] virtual bool returnsOnAllControlPaths(bool *overrideUnreachable) const { // NOLINT(misc-no-recursion)
    return children.size() == 1 && children.front()->returnsOnAllControlPaths(overrideUnreachable);
  }

  [[nodiscard]] virtual std::vector<Function *> *getFctManifestations() {
    assert(false && "Must be called on a FctDefNode, ProcDefNode or ExtDeclNode");
    return nullptr;
  }

  [[nodiscard]] virtual std::vector<Struct *> *getStructManifestations() {
    assert(false && "Must be called on a StructDefNode");
    return nullptr;
  }

  [[nodiscard]] virtual std::vector<Interface *> *getInterfaceManifestations() {
    assert(false && "Must be called on a InterfaceDefNode");
    return nullptr;
  }

  [[nodiscard]] virtual bool isFctOrProcDef() const { return false; }
  [[nodiscard]] virtual bool isStmtNode() const { return false; }
  [[nodiscard]] virtual bool isAssignExpr() const { return false; }

  // Public members
  ASTNode *parent;
  std::vector<ASTNode *> children;
  const CodeLoc codeLoc;
  std::string errorMessage;
  std::vector<SymbolType> symbolTypes;
  CompileTimeValue compileTimeValue = {.boolValue = false};
  std::string compileTimeStringValue;
  bool hasDirectCompileTimeValue = false;
  bool unreachable = false;
  std::vector<std::vector<const Function *>> opFct; // Operator overloading functions
};

// ========================================================== EntryNode ==========================================================

class EntryNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitEntry(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitEntry(this); }

  // Public get methods
  [[nodiscard]] std::vector<ModAttrNode *> modAttrs() const { return getChildren<ModAttrNode>(); }
  [[nodiscard]] std::vector<ImportStmtNode *> importStmts() const { return getChildren<ImportStmtNode>(); }
};

// ======================================================== MainFctDefNode =======================================================

class MainFctDefNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitMainFctDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitMainFctDef(this); }

  // Public get methods
  [[nodiscard]] FctAttrNode *attrs() const { return getChild<FctAttrNode>(); }
  [[nodiscard]] ParamLstNode *paramLst() const { return getChild<ParamLstNode>(); }
  [[nodiscard]] StmtLstNode *body() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "fct:main"; }
  [[nodiscard]] std::string getSignature() const { return takesArgs ? "main(int, string[])" : "main()"; }
  bool returnsOnAllControlPaths(bool *overrideUnreachable) const override;
  [[nodiscard]] bool isFctOrProcDef() const override { return true; }

  // Public members
  SymbolTableEntry *entry = nullptr;
  Scope *fctScope = nullptr;
  bool takesArgs = false;
};

// ========================================================== FctNameNode =======================================================

class FctNameNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Enums
  enum OverloadedOperator {
    OP_NONE,
    OP_PLUS,
    OP_MINUS,
    OP_MUL,
    OP_DIV,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_SHL,
    OP_SHR,
    OP_PLUS_EQUAL,
    OP_MINUS_EQUAL,
    OP_MUL_EQUAL,
    OP_DIV_EQUAL,
    OP_PLUS_PLUS,
    OP_MINUS_MINUS
  };

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitFctName(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitFctName(this); }

  // Public members
  std::string name;
  std::string structName;
  std::string fqName;
  std::vector<std::string> nameFragments;
  OverloadedOperator overloadedOperator = OP_NONE;
};

// ========================================================== FctDefNode =========================================================

class FctDefNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitFctDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitFctDef(this); }

  // Public get methods
  [[nodiscard]] FctAttrNode *attrs() const { return getChild<FctAttrNode>(); }
  [[nodiscard]] SpecifierLstNode *specifierLst() const { return getChild<SpecifierLstNode>(); }
  [[nodiscard]] DataTypeNode *returnType() const { return getChild<DataTypeNode>(); }
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(); }
  [[nodiscard]] ParamLstNode *paramLst() const { return getChild<ParamLstNode>(); }
  [[nodiscard]] StmtLstNode *body() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "fct:" + codeLoc.toString(); }
  [[nodiscard]] std::string getSymbolTableEntryName() const {
    const FctNameNode *functionName = getChild<FctNameNode>();
    assert(functionName != nullptr);
    return functionName->name + ":" + codeLoc.toPrettyLineAndColumn();
  }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *overrideUnreachable) const override;
  std::vector<Function *> *getFctManifestations() override { return &fctManifestations; }
  [[nodiscard]] bool isFctOrProcDef() const override { return true; }

  // Public members
  FctNameNode *fctName;
  bool isMethod = false;
  bool hasTemplateTypes = false;
  bool hasParams = false;
  SymbolTableEntry *entry = nullptr;
  TypeSpecifiers functionSpecifiers = TypeSpecifiers::of(TY_FUNCTION);
  Scope *structScope = nullptr;
  Scope *fctScope = nullptr;
  std::vector<Function *> fctManifestations;
};

// ========================================================== ProcDefNode ========================================================

class ProcDefNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitProcDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitProcDef(this); }

  // Public get methods
  [[nodiscard]] FctAttrNode *attrs() const { return getChild<FctAttrNode>(); }
  [[nodiscard]] SpecifierLstNode *specifierLst() const { return getChild<SpecifierLstNode>(); }
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(); }
  [[nodiscard]] ParamLstNode *paramLst() const { return getChild<ParamLstNode>(); }
  [[nodiscard]] StmtLstNode *body() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "proc:" + codeLoc.toString(); }
  [[nodiscard]] std::string getSymbolTableEntryName() const {
    const FctNameNode *functionName = getChild<FctNameNode>();
    assert(functionName != nullptr);
    return functionName->name + ":" + codeLoc.toPrettyLineAndColumn();
  }
  bool returnsOnAllControlPaths(bool *overrideUnreachable) const override;
  std::vector<Function *> *getFctManifestations() override { return &procManifestations; }
  [[nodiscard]] bool isFctOrProcDef() const override { return true; }

  // Public members
  FctNameNode *procName;
  bool isMethod = false;
  bool hasTemplateTypes = false;
  bool hasParams = false;
  bool isCtor = false;
  SymbolTableEntry *entry = nullptr;
  TypeSpecifiers procedureSpecifiers = TypeSpecifiers::of(TY_PROCEDURE);
  Scope *structScope = nullptr;
  Scope *procScope = nullptr;
  std::vector<Function *> procManifestations;
};

// ========================================================= StructDefNode =======================================================

class StructDefNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitStructDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitStructDef(this); }

  // Public get methods
  [[nodiscard]] SpecifierLstNode *specifierLst() const { return getChild<SpecifierLstNode>(); }
  [[nodiscard]] FieldLstNode *fieldLst() const { return getChild<FieldLstNode>(); }
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(0); }
  [[nodiscard]] TypeLstNode *interfaceTypeLst() const { return getChild<TypeLstNode>(hasTemplateTypes ? 1 : 0); }

  // Other methods
  std::vector<Struct *> *getStructManifestations() override { return &structManifestations; }

  // Public members
  std::string structName;
  bool hasTemplateTypes = false;
  bool hasInterfaces = false;
  SymbolTableEntry *entry = nullptr;
  TypeSpecifiers structSpecifiers = TypeSpecifiers::of(TY_STRUCT);
  std::vector<Struct *> structManifestations;
  Scope *structScope = nullptr;
};

// ======================================================= InterfaceDefNode ======================================================

class InterfaceDefNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitInterfaceDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitInterfaceDef(this); }

  // Public get methods
  [[nodiscard]] SpecifierLstNode *specifierLst() const { return getChild<SpecifierLstNode>(); }
  [[nodiscard]] std::vector<SignatureNode *> signatures() const { return getChildren<SignatureNode>(); }
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(0); }

  // Other methods
  std::vector<Interface *> *getInterfaceManifestations() override { return &interfaceManifestations; }

  // Public members
  std::string interfaceName;
  bool hasTemplateTypes = false;
  SymbolTableEntry *entry = nullptr;
  TypeSpecifiers interfaceSpecifiers = TypeSpecifiers::of(TY_INTERFACE);
  std::vector<Interface *> interfaceManifestations;
  Scope *interfaceScope = nullptr;
};

// ========================================================== EnumDefNode ========================================================

class EnumDefNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitEnumDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitEnumDef(this); }

  // Public get methods
  [[nodiscard]] SpecifierLstNode *specifierLst() const { return getChild<SpecifierLstNode>(); }
  [[nodiscard]] EnumItemLstNode *itemLst() const { return getChild<EnumItemLstNode>(); }

  // Public members
  std::string enumName;
  SymbolTableEntry *entry = nullptr;
  TypeSpecifiers enumSpecifiers = TypeSpecifiers::of(TY_ENUM);
  Scope *enumScope;
};

// ====================================================== GenericTypeDefNode =====================================================

class GenericTypeDefNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitGenericTypeDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitGenericTypeDef(this); }

  // Public get methods
  [[nodiscard]] TypeAltsLstNode *typeAltsLst() const { return getChild<TypeAltsLstNode>(); }

  // Public members
  std::string typeName;
};

// ========================================================= AliasDefNode ========================================================

class AliasDefNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAliasDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAliasDef(this); }

  // Public get methods
  [[nodiscard]] DataTypeNode *dataType() const { return getChild<DataTypeNode>(); }

  // Public members
  std::string aliasName;
  std::string dataTypeString;
  SymbolTableEntry *entry = nullptr;
  SymbolTableEntry *aliasedTypeContainerEntry = nullptr;
};

// ======================================================= GlobalVarDefNode ======================================================

class GlobalVarDefNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitGlobalVarDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitGlobalVarDef(this); }

  // Public get methods
  [[nodiscard]] DataTypeNode *dataType() const { return getChild<DataTypeNode>(); }
  [[nodiscard]] ConstantNode *constant() const { return getChild<ConstantNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override { return true; }
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;

  // Public members
  std::string varName;
  SymbolTableEntry *entry = nullptr;
  bool hasValue = false;
};

// ========================================================== ExtDeclNode ========================================================

class ExtDeclNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitExtDecl(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitExtDecl(this); }

  // Public get methods
  [[nodiscard]] FctAttrNode *attrs() const { return getChild<FctAttrNode>(); }
  [[nodiscard]] DataTypeNode *returnType() const { return getChild<DataTypeNode>(); }
  [[nodiscard]] TypeLstNode *argTypeLst() const { return getChild<TypeLstNode>(); }

  // Other methods
  std::vector<Function *> *getFctManifestations() override { return &extFunctionManifestations; }

  // Public members
  std::string extFunctionName;
  bool hasArgs = false;
  bool isVarArg = false;
  SymbolTableEntry *entry = nullptr;
  Function *extFunction = nullptr;
  std::vector<Function *> extFunctionManifestations;
};

// ====================================================== UnsafeBlockDefNode =====================================================

class UnsafeBlockDefNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitUnsafeBlockDef(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitUnsafeBlockDef(this); }

  // Public get methods
  [[nodiscard]] StmtLstNode *body() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "unsafe:" + codeLoc.toString(); }

  // Public members
  Scope *bodyScope = nullptr;
};

// ========================================================== ForLoopNode ========================================================

class ForLoopNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitForLoop(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitForLoop(this); }

  // Public get methods
  [[nodiscard]] DeclStmtNode *initDecl() const { return getChild<DeclStmtNode>(); }
  [[nodiscard]] AssignExprNode *condAssign() const { return getChild<AssignExprNode>(0); }
  [[nodiscard]] AssignExprNode *incAssign() const { return getChild<AssignExprNode>(1); }
  [[nodiscard]] StmtLstNode *body() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "for:" + codeLoc.toString(); }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *overrideUnreachable) const override;

  // Public members
  Scope *bodyScope = nullptr;
};

// ======================================================== ForeachLoopNode ======================================================

class ForeachLoopNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitForeachLoop(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitForeachLoop(this); }

  // Public get methods
  [[nodiscard]] DeclStmtNode *idxVarDecl() const {
    std::vector<DeclStmtNode *> declStmtNodes = getChildren<DeclStmtNode>();
    return declStmtNodes.size() == 2 ? declStmtNodes.front() : nullptr;
  }
  [[nodiscard]] DeclStmtNode *itemVarDecl() const { return getChildren<DeclStmtNode>().back(); }
  [[nodiscard]] AssignExprNode *iteratorAssign() const { return getChild<AssignExprNode>(); }
  [[nodiscard]] StmtLstNode *body() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "foreach:" + codeLoc.toString(); }

  // Public members
  Scope *bodyScope = nullptr;
  Function *getFct = nullptr;
  Function *getIdxFct = nullptr;
  Function *isValidFct = nullptr;
  Function *nextFct = nullptr;
};

// ========================================================= WhileLoopNode =======================================================

class WhileLoopNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitWhileLoop(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitWhileLoop(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *condition() const { return getChild<AssignExprNode>(); }
  [[nodiscard]] StmtLstNode *body() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "while:" + codeLoc.toString(); }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *overrideUnreachable) const override;

  // Public members
  Scope *bodyScope = nullptr;
};

// ======================================================== DoWhileLoopNode ======================================================

class DoWhileLoopNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitDoWhileLoop(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitDoWhileLoop(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *condition() const { return getChild<AssignExprNode>(); }
  [[nodiscard]] StmtLstNode *body() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "dowhile:" + codeLoc.toString(); }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *overrideUnreachable) const override;

  // Public members
  Scope *bodyScope = nullptr;
};

// ========================================================== IfStmtNode =========================================================

class IfStmtNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitIfStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitIfStmt(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *condition() const { return getChild<AssignExprNode>(); }
  [[nodiscard]] StmtLstNode *thenBody() const { return getChild<StmtLstNode>(); }
  [[nodiscard]] ElseStmtNode *elseStmt() const { return getChild<ElseStmtNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "if:" + codeLoc.toString(); }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *overrideUnreachable) const override;

  // Public members
  Scope *thenBodyScope = nullptr;
};

// ========================================================= ElseStmtNode ========================================================

class ElseStmtNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitElseStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitElseStmt(this); }

  // Public get methods
  [[nodiscard]] IfStmtNode *ifStmt() const { return getChild<IfStmtNode>(); }
  [[nodiscard]] StmtLstNode *body() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "if:" + codeLoc.toString(); }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *overrideUnreachable) const override;

  // Public members
  bool isElseIf = false;
  Scope *elseBodyScope = nullptr;
};

// ========================================================== ScopeNode ==========================================================

class AnonymousBlockStmtNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAnonymousBlockStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAnonymousBlockStmt(this); }

  // Public get methods
  [[nodiscard]] StmtLstNode *body() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "anon:" + codeLoc.toString(); }

  // Public members
  Scope *bodyScope = nullptr;
};

// ========================================================= StmtLstNode =========================================================

class StmtLstNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitStmtLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitStmtLst(this); }

  // Other methods
  [[nodiscard]] bool returnsOnAllControlPaths(bool *overrideUnreachable) const override;
  void resizeToNumberOfManifestations(size_t manifestationCount) override {
    ASTNode::resizeToNumberOfManifestations(manifestationCount);
    dtorFunctions.resize(manifestationCount, std::vector<std::pair<SymbolTableEntry *, Function *>>());
  }

  // Public members
  size_t complexity = 0;
  // Outer vector: manifestation index; inner vector: list of dtor functions
  std::vector<std::vector<std::pair<SymbolTableEntry *, Function *>>> dtorFunctions;
};

// ========================================================= TypeLstNode =========================================================

class TypeLstNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitTypeLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitTypeLst(this); }

  // Public get methods
  [[nodiscard]] std::vector<DataTypeNode *> dataTypes() const { return getChildren<DataTypeNode>(); }

  // Public members
  size_t numberOfTypes = 0;
};

// ======================================================= TypeAltsLstNode =======================================================

class TypeAltsLstNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitTypeAltsLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitTypeAltsLst(this); }

  // Public get methods
  [[nodiscard]] std::vector<DataTypeNode *> dataTypes() const { return getChildren<DataTypeNode>(); }

  // Public members
  size_t numberOfAlts = 0;
};

// ======================================================== ParamLstNode =========================================================

class ParamLstNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitParamLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitParamLst(this); }

  // Public get methods
  [[nodiscard]] std::vector<DeclStmtNode *> params() const { return getChildren<DeclStmtNode>(); }
};

// ========================================================== ArgLstNode =========================================================

class ArgLstNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitArgLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitArgLst(this); }

  // Public get methods
  [[nodiscard]] std::vector<AssignExprNode *> args() const { return getChildren<AssignExprNode>(); }
};

// ======================================================== EnumItemLstNode ======================================================

class EnumItemLstNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitEnumItemLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitEnumItemLst(this); }

  // Public get methods
  [[nodiscard]] std::vector<EnumItemNode *> items() const { return getChildren<EnumItemNode>(); }

  // Public members
  EnumDefNode *enumDef = nullptr;
};

// ========================================================= EnumItemNode ========================================================

class EnumItemNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitEnumItem(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitEnumItem(this); }

  // Public get methods

  // Public members
  std::string itemName;
  uint32_t itemValue;
  bool hasValue = false;
  SymbolTableEntry *entry = nullptr;
  EnumDefNode *enumDef = nullptr;
};

// ========================================================= FieldLstNode ========================================================

class FieldLstNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitFieldLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitFieldLst(this); }

  // Public get methods
  [[nodiscard]] std::vector<FieldNode *> fields() const { return getChildren<FieldNode>(); }
};

// ========================================================== FieldNode ==========================================================

class FieldNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitField(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitField(this); }

  // Public get methods
  [[nodiscard]] DataTypeNode *dataType() const { return getChild<DataTypeNode>(); }
  [[nodiscard]] TernaryExprNode *defaultValue() const { return getChild<TernaryExprNode>(); }

  // Public members
  std::string fieldName;
  SymbolTableEntry *entry = nullptr;
};

// ======================================================== SignatureNode ========================================================

class SignatureNode : public ASTNode {
public:
  // Enums
  enum Type {
    TYPE_NONE,
    TYPE_FUNCTION,
    TYPE_PROCEDURE,
  };

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitSignature(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitSignature(this); }

  // Public get methods
  [[nodiscard]] SpecifierLstNode *specifierLst() const { return getChild<SpecifierLstNode>(); }
  [[nodiscard]] DataTypeNode *returnType() const { return getChild<DataTypeNode>(); }
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(0); }
  [[nodiscard]] TypeLstNode *paramTypeLst() const { return getChild<TypeLstNode>(hasTemplateTypes ? 1 : 0); }

  // Other methods
  std::vector<Function *> *getFctManifestations() override { return &signatureManifestations; }

  // Public members
  Type signatureType = SignatureNode::TYPE_NONE;
  std::string methodName;
  SymbolTableEntry *entry = nullptr;
  TypeSpecifiers signatureSpecifiers;
  bool hasParams = false;
  bool hasTemplateTypes = false;
  std::vector<Function *> signatureManifestations;
};

// =========================================================== StmtNode ==========================================================

class StmtNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitStmt(this); }

  // Other methods
  [[nodiscard]] bool isStmtNode() const override { return true; }
};

// ========================================================= DeclStmtNode ========================================================

class DeclStmtNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitDeclStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitDeclStmt(this); }

  // Public get methods
  [[nodiscard]] DataTypeNode *dataType() const { return getChild<DataTypeNode>(); }
  [[nodiscard]] AssignExprNode *assignExpr() const { return getChild<AssignExprNode>(); }

  // Util methods
  void customItemsInitialization(size_t manifestationCount) override { entries.resize(manifestationCount, nullptr); }

  // Public members
  std::string varName;
  bool hasAssignment = false;
  std::vector<SymbolTableEntry *> entries;
  bool isParam = false;
  bool isForEachItem = false;
  Function *defaultCtor = nullptr;
};

// ======================================================= SpecifierLstNode ======================================================

class SpecifierLstNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitSpecifierLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitSpecifierLst(this); }

  // Public get methods
  [[nodiscard]] std::vector<SpecifierNode *> specifiers() const { return getChildren<SpecifierNode>(); }
};

// ========================================================= SpecifierNode =======================================================

class SpecifierNode : public ASTNode {
public:
  // Enums
  enum SpecifierType { TY_NONE, TY_CONST, TY_SIGNED, TY_UNSIGNED, TY_INLINE, TY_PUBLIC, TY_HEAP };

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitSpecifier(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitSpecifier(this); }

  // Public members
  SpecifierType type = TY_NONE;
};

// ========================================================== ModAttrNode ========================================================

class ModAttrNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitModAttr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitModAttr(this); }

  // Public get methods
  [[nodiscard]] AttrLstNode *attrLst() const { return getChild<AttrLstNode>(); }
};

// ========================================================== FctAttrNode ========================================================

class FctAttrNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitFctAttr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitFctAttr(this); }

  // Public get methods
  [[nodiscard]] AttrLstNode *attrLst() const { return getChild<AttrLstNode>(); }
};

// ========================================================== AttrLstNode ========================================================

class AttrLstNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAttrLst(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAttrLst(this); }

  // Public get methods
  [[nodiscard]] std::vector<AttrNode *> attributes() const { return getChildren<AttrNode>(); }

  // Other methods
  [[nodiscard]] std::vector<AttrNode *> getAttrsByName(const char *key) const;
  [[nodiscard]] AttrNode *getAttrByName(const char *key) const;
};

// ============================================================ AttrNode =========================================================

class AttrNode : public ASTNode {
public:
  // Enums
  static constexpr const char *const ATTR_CORE_LINKER_FLAG = "core.linker.flag";
  static constexpr const char *const ATTR_CORE_LINKER_DLL = "core.linker.dll";
  static constexpr const char *const ATTR_CORE_COMPILER_MANGLED_NAME = "core.compiler.mangledName";
  static constexpr const char *const ATTR_CORE_COMPILER_MANGLE = "core.compiler.mangle";

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAttr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAttr(this); }

  // Public get methods
  [[nodiscard]] ConstantNode *value() const { return getChild<ConstantNode>(); }

  // Other methods
  [[nodiscard]] const CompileTimeValue &getValue() const;

  // Public members
  std::string key;
};

// ======================================================== ImportStmtNode =======================================================

class ImportStmtNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitImportStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitImportStmt(this); }

  // Public members
  std::string importPath;
  std::string importName;
  SymbolTableEntry *entry = nullptr;
};

// ======================================================== ReturnStmtNode =======================================================

class ReturnStmtNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitReturnStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitReturnStmt(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *assignExpr() const { return getChild<AssignExprNode>(); }

  // Other methods
  [[nodiscard]] bool returnsOnAllControlPaths(bool *overrideUnreachable) const override { return true; }
  [[nodiscard]] StmtLstNode *getParentScopeNode() const {
    assert(dynamic_cast<StmtLstNode *>(parent->parent) != nullptr);
    return spice_pointer_cast<StmtLstNode *>(parent->parent);
  }

  // Public members
  bool hasReturnValue = false;
};

// ======================================================== BreakStmtNode ========================================================

class BreakStmtNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitBreakStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitBreakStmt(this); }

  // Public members
  int breakTimes = 1;
};

// ======================================================= ContinueStmtNode ======================================================

class ContinueStmtNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitContinueStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitContinueStmt(this); }

  // Public members
  int continueTimes = 1;
};

// ======================================================== AssertStmtNode =======================================================

class AssertStmtNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAssertStmt(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAssertStmt(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *assignExpr() const { return getChild<AssignExprNode>(); }

  // Other methods
  [[nodiscard]] bool returnsOnAllControlPaths(bool *overrideUnreachable) const override;

  // Public members
  std::string expressionString;
};

// ======================================================== PrintfCallNode =======================================================

class PrintfCallNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitPrintfCall(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitPrintfCall(this); }

  // Public get methods
  [[nodiscard]] std::vector<AssignExprNode *> args() const { return getChildren<AssignExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }

  // Public members
  std::string templatedString;
};

// ======================================================== SizeofCallNode =======================================================

class SizeofCallNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitSizeofCall(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitSizeofCall(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *assignExpr() const { return getChild<AssignExprNode>(); }
  [[nodiscard]] DataTypeNode *dataType() const { return getChild<DataTypeNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }

  // Public members
  bool isType = false;
};

// ======================================================== AlignofCallNode ======================================================

class AlignofCallNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAlignofCall(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAlignofCall(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *assignExpr() const { return getChild<AssignExprNode>(); }
  [[nodiscard]] DataTypeNode *dataType() const { return getChild<DataTypeNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }

  // Public members
  bool isType = false;
};

// ========================================================= LenCallNode =========================================================

class LenCallNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitLenCall(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitLenCall(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *assignExpr() const { return getChild<AssignExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }
};

// ======================================================== PanicCallNode ========================================================

class PanicCallNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitPanicCall(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitPanicCall(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *assignExpr() const { return getChild<AssignExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }
};

// ======================================================= AssignExprNode ========================================================

class AssignExprNode : public ASTNode {
public:
  // Enums
  enum AssignOp {
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
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAssignExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAssignExpr(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *rhs() const { return getChild<AssignExprNode>(); }
  [[nodiscard]] PrefixUnaryExprNode *lhs() const { return getChild<PrefixUnaryExprNode>(); }
  [[nodiscard]] TernaryExprNode *ternaryExpr() const { return getChild<TernaryExprNode>(); }

  // Other methods
  [[nodiscard]] bool returnsOnAllControlPaths(bool *overrideUnreachable) const override;
  [[nodiscard]] bool isAssignExpr() const override { return true; }

  // Public members
  AssignOp op = OP_NONE;
  bool hasOperator = false;
};

// ======================================================= TernaryExprNode =======================================================

class TernaryExprNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitTernaryExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitTernaryExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<LogicalOrExprNode *> operands() const { return getChildren<LogicalOrExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;

  bool isShortened = false;
};

// ===================================================== LogicalOrExprNode =======================================================

class LogicalOrExprNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitLogicalOrExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitLogicalOrExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<LogicalAndExprNode *> operands() const { return getChildren<LogicalAndExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;
};

// ===================================================== LogicalAndExprNode ======================================================

class LogicalAndExprNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitLogicalAndExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitLogicalAndExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<BitwiseOrExprNode *> operands() const { return getChildren<BitwiseOrExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;
};

// ===================================================== BitwiseOrExprNode =======================================================

class BitwiseOrExprNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitBitwiseOrExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitBitwiseOrExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<BitwiseXorExprNode *> operands() const { return getChildren<BitwiseXorExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;
};

// ==================================================== BitwiseXorExprNode =======================================================

class BitwiseXorExprNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitBitwiseXorExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitBitwiseXorExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<BitwiseAndExprNode *> operands() const { return getChildren<BitwiseAndExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;
};

// ==================================================== BitwiseAndExprNode =======================================================

class BitwiseAndExprNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitBitwiseAndExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitBitwiseAndExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<EqualityExprNode *> operands() const { return getChildren<EqualityExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;
};

// ===================================================== EqualityExprNode ========================================================

class EqualityExprNode : public ASTNode {
public:
  // Enums
  enum EqualityOp { OP_NONE, OP_EQUAL, OP_NOT_EQUAL };

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitEqualityExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitEqualityExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<RelationalExprNode *> operands() const { return getChildren<RelationalExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;

  // Public members
  EqualityOp op = OP_NONE;
};

// ==================================================== RelationalExprNode =======================================================

class RelationalExprNode : public ASTNode {
public:
  // Enums
  enum RelationalOp { OP_NONE, OP_LESS, OP_GREATER, OP_LESS_EQUAL, OP_GREATER_EQUAL };

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitRelationalExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitRelationalExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<ShiftExprNode *> operands() const { return getChildren<ShiftExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;

  // Public members
  RelationalOp op = OP_NONE;
};

// ====================================================== ShiftExprNode ==========================================================

class ShiftExprNode : public ASTNode {
public:
  // Enums
  enum ShiftOp {
    OP_NONE,
    OP_SHIFT_LEFT,
    OP_SHIFT_RIGHT,
  };

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitShiftExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitShiftExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<AdditiveExprNode *> operands() const { return getChildren<AdditiveExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;

  // Public members
  ShiftOp op = OP_NONE;
};

// ==================================================== AdditiveExprNode =========================================================

class AdditiveExprNode : public ASTNode {
public:
  // Enums
  enum AdditiveOp {
    OP_NONE,
    OP_PLUS,
    OP_MINUS,
  };

  // Typedefs
  typedef std::queue<std::pair<AdditiveOp, SymbolType>> OpQueue;

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAdditiveExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAdditiveExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<MultiplicativeExprNode *> operands() const { return getChildren<MultiplicativeExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;

  // Public members
  OpQueue opQueue;
};

// ================================================== MultiplicativeExprNode =====================================================

class MultiplicativeExprNode : public ASTNode {
public:
  // Enums
  enum MultiplicativeOp {
    OP_NONE,
    OP_MUL,
    OP_DIV,
    OP_REM,
  };

  // Typedefs
  typedef std::queue<std::pair<MultiplicativeOp, SymbolType>> OpQueue;

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitMultiplicativeExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitMultiplicativeExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<CastExprNode *> operands() const { return getChildren<CastExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;

  // Public members
  OpQueue opQueue;
};

// ======================================================= CastExprNode ==========================================================

class CastExprNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitCastExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitCastExpr(this); }

  // Public get methods
  [[nodiscard]] DataTypeNode *dataType() const { return getChild<DataTypeNode>(); }
  [[nodiscard]] PrefixUnaryExprNode *prefixUnaryExpr() const { return getChild<PrefixUnaryExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;

  // Public members
  bool isCast = false;
};

// ==================================================== PrefixUnaryExprNode ======================================================

class PrefixUnaryExprNode : public ASTNode {
public:
  // Enums
  enum PrefixUnaryOp { OP_NONE, OP_MINUS, OP_PLUS_PLUS, OP_MINUS_MINUS, OP_NOT, OP_BITWISE_NOT, OP_DEREFERENCE, OP_ADDRESS_OF };

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitPrefixUnaryExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitPrefixUnaryExpr(this); }

  // Public get methods
  [[nodiscard]] PrefixUnaryExprNode *prefixUnary() const { return getChild<PrefixUnaryExprNode>(); }
  [[nodiscard]] PostfixUnaryExprNode *postfixUnaryExpr() const { return getChild<PostfixUnaryExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;

  // Public members
  PrefixUnaryOp op = OP_NONE;
};

// =================================================== PostfixUnaryExprNode ======================================================

class PostfixUnaryExprNode : public ASTNode {
public:
  // Enums
  enum PostfixUnaryOp { OP_NONE, OP_SUBSCRIPT, OP_MEMBER_ACCESS, OP_PLUS_PLUS, OP_MINUS_MINUS };

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitPostfixUnaryExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitPostfixUnaryExpr(this); }

  // Public get methods
  [[nodiscard]] AtomicExprNode *atomicExpr() const { return getChild<AtomicExprNode>(); }
  [[nodiscard]] PostfixUnaryExprNode *postfixUnaryExpr() const { return getChild<PostfixUnaryExprNode>(); }
  [[nodiscard]] AssignExprNode *assignExpr() const { return getChild<AssignExprNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override;
  [[nodiscard]] CompileTimeValue getCompileTimeValue() const override;

  // Public members
  PostfixUnaryOp op = OP_NONE;
  std::string identifier; // Only set when operator is member access
};

// ====================================================== AtomicExprNode =========================================================

class AtomicExprNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitAtomicExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitAtomicExpr(this); }

  // Public get methods
  [[nodiscard]] ConstantNode *constant() const { return getChild<ConstantNode>(); }
  [[nodiscard]] ValueNode *value() const { return getChild<ValueNode>(); }
  [[nodiscard]] AssignExprNode *assignExpr() const { return getChild<AssignExprNode>(); }
  [[nodiscard]] PrintfCallNode *printfCall() const { return getChild<PrintfCallNode>(); }
  [[nodiscard]] SizeofCallNode *sizeofCall() const { return getChild<SizeofCallNode>(); }
  [[nodiscard]] AlignofCallNode *alignofCall() const { return getChild<AlignofCallNode>(); }
  [[nodiscard]] LenCallNode *lenCall() const { return getChild<LenCallNode>(); }
  [[nodiscard]] PanicCallNode *panicCall() const { return getChild<PanicCallNode>(); }

  // Util methods
  void customItemsInitialization(size_t manifestationCount) override {
    entries.resize(manifestationCount, nullptr);
    accessScopes.resize(manifestationCount, nullptr);
  }

  // Public members
  std::vector<std::string> identifierFragments;
  std::string fqIdentifier;
  std::vector<SymbolTableEntry *> entries; // Only set if identifier is set as well
  std::vector<Scope *> accessScopes;       // Only set if identifier is set as well
};

// ======================================================== ValueNode ============================================================

class ValueNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitValue(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitValue(this); }

  // Public get methods
  [[nodiscard]] FctCallNode *fctCall() const { return getChild<FctCallNode>(); }
  [[nodiscard]] ArrayInitializationNode *arrayInitialization() const { return getChild<ArrayInitializationNode>(); }
  [[nodiscard]] StructInstantiationNode *structInstantiation() const { return getChild<StructInstantiationNode>(); }
  [[nodiscard]] LambdaFuncNode *lambdaFunc() const { return getChild<LambdaFuncNode>(); }
  [[nodiscard]] LambdaProcNode *lambdaProc() const { return getChild<LambdaProcNode>(); }
  [[nodiscard]] LambdaExprNode *lambdaExpr() const { return getChild<LambdaExprNode>(); }
  [[nodiscard]] DataTypeNode *nilType() const { return getChild<DataTypeNode>(); }

  // Public members
  bool isNil = false;
};

// =================================================== PrimitiveValueNode ========================================================

class ConstantNode : public ASTNode {
public:
  // Enum
  enum PrimitiveValueType {
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
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitConstant(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitConstant(this); }

  // Public members
  PrimitiveValueType type = TYPE_NONE;
  bool isSigned = true;
};

// ====================================================== FctCallNode ============================================================

class FctCallNode : public ASTNode {
public:
  enum FctCallType : uint8_t { TYPE_ORDINARY, TYPE_METHOD, TYPE_CTOR, TYPE_FCT_PTR };

  // Structs
  struct FctCallData {
    // Members
    FctCallType callType = TYPE_ORDINARY;
    bool isImported = false;
    SymbolType thisType = SymbolType(TY_DYN); // Is filled if method or ctor call
    std::vector<SymbolType> argTypes;
    Function *callee = nullptr;
    Scope *calleeParentScope = nullptr;

    // Methods
    [[nodiscard]] bool isOrdinaryCall() const { return callType == TYPE_ORDINARY; }
    [[nodiscard]] bool isMethodCall() const { return callType == TYPE_METHOD; }
    [[nodiscard]] bool isCtorCall() const { return callType == TYPE_CTOR; }
    [[nodiscard]] bool isFctPtrCall() const { return callType == TYPE_FCT_PTR; }
  };

  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitFctCall(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitFctCall(this); }

  // Public get methods
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(); }
  [[nodiscard]] ArgLstNode *argLst() const { return getChild<ArgLstNode>(); }

  // Other methods
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }
  void customItemsInitialization(size_t manifestationCount) override { data.resize(manifestationCount); }
  [[nodiscard]] bool hasReturnValueReceiver() const;

  // Public members
  std::string fqFunctionName;
  std::vector<std::string> functionNameFragments;
  bool hasArgs = false;
  bool hasTemplateTypes = false;
  std::vector<FctCallData> data;
};

// ================================================= ArrayInitializationNode =====================================================

class ArrayInitializationNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitArrayInitialization(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitArrayInitialization(this); }

  // Public get methods
  [[nodiscard]] ArgLstNode *itemLst() const { return getChild<ArgLstNode>(); }

  // Public members
  long actualSize = 0;
};

// ================================================= StructInstantiationNode =====================================================

class StructInstantiationNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitStructInstantiation(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitStructInstantiation(this); }

  // Public get methods
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(); }
  [[nodiscard]] ArgLstNode *fieldLst() const { return getChild<ArgLstNode>(); }

  // Util methods
  void customItemsInitialization(size_t manifestationCount) override { instantiatedStructs.resize(manifestationCount, nullptr); }

  // Public members
  std::string fqStructName;
  std::vector<std::string> structNameFragments;
  std::vector<Struct *> instantiatedStructs;
};

// ====================================================== LambdaFuncNode =========================================================

class LambdaFuncNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visit methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitLambdaFunc(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitLambdaFunc(this); }

  // Public get methods
  [[nodiscard]] ParamLstNode *paramLst() const { return getChild<ParamLstNode>(); }
  [[nodiscard]] DataTypeNode *returnType() const { return getChild<DataTypeNode>(); }
  [[nodiscard]] StmtLstNode *body() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "lambda:" + codeLoc.toString(); }
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *overrideUnreachable) const override;
  void customItemsInitialization(size_t manifestationCount) override { lambdaFunction.resize(manifestationCount); }

  // Public members
  bool hasParams = false;
  Scope *bodyScope = nullptr;
  std::vector<Function> lambdaFunction;
};

// ====================================================== LambdaProcNode =========================================================

class LambdaProcNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visit methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitLambdaProc(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitLambdaProc(this); }

  // Public get methods
  [[nodiscard]] ParamLstNode *paramLst() const { return getChild<ParamLstNode>(); }
  [[nodiscard]] StmtLstNode *body() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "lambda:" + codeLoc.toString(); }
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }
  [[nodiscard]] bool returnsOnAllControlPaths(bool *overrideUnreachable) const override;
  void customItemsInitialization(size_t manifestationCount) override { lambdaProcedure.resize(manifestationCount); }

  // Public members
  bool hasParams = false;
  Scope *bodyScope = nullptr;
  std::vector<Function> lambdaProcedure;
};

// ====================================================== LambdaExprNode =========================================================

class LambdaExprNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visit methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitLambdaExpr(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitLambdaExpr(this); }

  // Public get methods
  [[nodiscard]] ParamLstNode *paramLst() const { return getChild<ParamLstNode>(); }
  [[nodiscard]] AssignExprNode *lambdaExpr() const { return getChild<AssignExprNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "lambda:" + codeLoc.toString(); }
  [[nodiscard]] bool hasCompileTimeValue() const override { return false; }
  void customItemsInitialization(size_t manifestationCount) override { lambdaFunction.resize(manifestationCount); }

  // Public members
  bool hasParams = false;
  Scope *bodyScope = nullptr;
  std::vector<Function> lambdaFunction;
};

// ======================================================= DataTypeNode ==========================================================

class DataTypeNode : public ASTNode {
public:
  // Enums
  enum TypeModifierType { TYPE_PTR, TYPE_REF, TYPE_ARRAY };

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

  // Public get methods
  [[nodiscard]] SpecifierLstNode *specifierLst() const { return getChild<SpecifierLstNode>(); }
  [[nodiscard]] BaseDataTypeNode *baseDataType() const { return getChild<BaseDataTypeNode>(); }

  // Public members
  std::queue<TypeModifier> tmQueue;
  bool isParamType = false;
  bool isGlobalType = false;
  bool isFieldType = false;
  bool isReturnType = false;
};

// ==================================================== BaseDataTypeNode =========================================================

class BaseDataTypeNode : public ASTNode {
public:
  // Enums
  enum Type {
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

  // Public get methods
  [[nodiscard]] CustomDataTypeNode *customDataType() const { return getChild<CustomDataTypeNode>(); }
  [[nodiscard]] FunctionDataTypeNode *functionDataType() const { return getChild<FunctionDataTypeNode>(); }

  // Public members
  Type type = TYPE_NONE;
};

// ==================================================== CustomDataTypeNode =======================================================

class CustomDataTypeNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitCustomDataType(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitCustomDataType(this); }

  // Public get methods
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(); }

  // Util methods
  void customItemsInitialization(size_t manifestationCount) override { customTypes.resize(manifestationCount, nullptr); }

  // Public members
  std::string fqTypeName;
  std::vector<std::string> typeNameFragments;
  std::vector<SymbolTableEntry *> customTypes;
};

// =================================================== FunctionDataTypeNode ======================================================

class FunctionDataTypeNode : public ASTNode {
public:
  // Constructors
  using ASTNode::ASTNode;

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return visitor->visitFunctionDataType(this); }
  std::any accept(ParallelizableASTVisitor *visitor) const override { return visitor->visitFunctionDataType(this); }

  // Public get methods
  [[nodiscard]] DataTypeNode *returnType() const { return getChild<DataTypeNode>(); }
  [[nodiscard]] TypeLstNode *paramTypeLst() const { return getChild<TypeLstNode>(); }

  // Util methods
  void customItemsInitialization(size_t manifestationCount) override { customTypes.resize(manifestationCount, nullptr); }

  // Public members
  std::vector<SymbolTableEntry *> customTypes;
  bool isFunction = false; // Function or procedure
};

} // namespace spice::compiler
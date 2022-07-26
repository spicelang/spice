// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <vector>

#include <ast/AstVisitor.h>
#include <symbol/Function.h>
#include <symbol/Struct.h>
#include <util/CodeLoc.h>

// =========================================================== AstNode ===========================================================

class AstNode {
public:
  // Constructors
  explicit AstNode(AstNode *parent, const CodeLoc &codeLoc) : parent(parent), codeLoc(codeLoc) {}

  // Destructors
  virtual ~AstNode() {
    for (auto child : children)
      delete child;
  }

  // Virtual methods
  virtual std::any accept(AbstractAstVisitor *visitor) const = 0;

  // Public methods
  template <typename T> T *createChild(const CodeLoc &codeLoc) {
    static_assert(std::is_base_of_v<AstNode, T>, "T must be derived from AstNode");
    T *node = new T(this, codeLoc);
    children.push_back(node);
    return node;
  }
  template <typename T> T *getChild() const {
    static_assert(std::is_base_of_v<AstNode, T>, "T must be derived from AstNode");
    for (auto &child : children) {
      if (auto *typedChild = dynamic_cast<T*>(child); typedChild != nullptr)
        return typedChild;
    }
    return nullptr;
  }
  template <typename T> std::vector<T *> getChildren() const {
    static_assert(std::is_base_of_v<AstNode, T>, "T must be derived from AstNode");
    std::vector<T *> nodes;
    for (auto &child : children) {
      if (auto *typedChild = dynamic_cast<T*>(child); typedChild != nullptr)
        nodes.push_back(typedChild);
    }
    return nodes;
  }

  // Public members
  AstNode *parent;
  std::vector<AstNode *> children;
  const CodeLoc codeLoc;
};

// ========================================================== EntryNode ==========================================================

class EntryNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitEntry(this); }

  // Public get methods
  std::vector<MainFctDefNode *> mainFctDefNode() const { return getChildren<MainFctDefNode>(); }
  std::vector<FctDefNode *> fctDefNode() const { return getChildren<FctDefNode>(); }
  std::vector<ProcDefNode *> procDefNode() const { return getChildren<ProcDefNode>(); }
  std::vector<StructDefNode *> structDefNode() const { return getChildren<StructDefNode>(); }
  std::vector<GenericTypeDefNode *> genericTypeDefNode() const { return getChildren<GenericTypeDefNode>(); }
  std::vector<GlobalVarDefNode *> globalVarDefNode() const { return getChildren<GlobalVarDefNode>(); }
  std::vector<ImportStmtNode *> importStmtNode() const { return getChildren<ImportStmtNode>(); }
  std::vector<ExtDeclNode *> extDeclNode() const { return getChildren<ExtDeclNode>(); }
};

// ======================================================== MainFctDefNode =======================================================

class MainFctDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitMainFctDef(this); }

private:
  // Members
  Function *spiceFunc = nullptr;
};

// ========================================================== FctDefNode =========================================================

class FctDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitFctDef(this); }

private:
  // Members
  std::string functionName;
  Function *spiceFunc = nullptr;
};

// ========================================================== ProcDefNode ========================================================

class ProcDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitProcDef(this); }

private:
  // Members
  std::string procedureName;
  Function *spiceProc = nullptr;
};

// ========================================================= StructDefNode =======================================================

class StructDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitStructDef(this); }

private:
  // Members
  std::string structName;
  Struct *spiceProc = nullptr;
};

// ====================================================== GenericTypeDefNode =====================================================

class GenericTypeDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitGenericTypeDef(this); }

private:
  // Members
  std::string typeName;
};

// ======================================================= GlobalVarDefNode ======================================================

class GlobalVarDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitGlobalVarDef(this); }

private:
  // Members
  std::string varName;
};

// ========================================================== ExtDeclNode ========================================================

class ExtDeclNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitExtDecl(this); }

private:
  // Members
  std::string extFunctionName;
};

// ========================================================= ThreadDefNode =======================================================

class ThreadDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitThreadDef(this); }
};

// ====================================================== UnsafeBlockDefNode =====================================================

class UnsafeBlockDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitUnsafeBlockDef(this); }
};

// ========================================================== ForLoopNode ========================================================

class ForLoopNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitForLoop(this); }
};

// ======================================================== ForeachLoopNode ======================================================

class ForeachLoopNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitForeachLoop(this); }
};

// ========================================================= WhileLoopNode =======================================================

class WhileLoopNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitWhileLoop(this); }
};

// ========================================================== IfStmtNode =========================================================

class IfStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitIfStmt(this); }
};

// ======================================================== AssertStmtNode =======================================================

class AssertStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitAssertStmt(this); }
};

// ========================================================= StmtLstNode =========================================================

class StmtLstNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitStmtLst(this); }
};

// ========================================================= TypeLstNode =========================================================

class TypeLstNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitTypeLst(this); }
};

// ======================================================= TypeAltsLstNode =======================================================

class TypeAltsLstNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitTypeAltsLst(this); }
};

// ======================================================== ArgLstDefNode ========================================================

class ArgLstDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitArgLstDef(this); }
};

// ========================================================== ArgLstNode =========================================================

class ArgLstNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitArgLst(this); }
};

// ========================================================== FieldNode ==========================================================

class FieldNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitField(this); }
};

// =========================================================== StmtNode ==========================================================

class StmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitStmt(this); }
};

// ========================================================= DeclStmtNode ========================================================

class DeclStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitDeclStmt(this); }
};

// ====================================================== DeclSpecifiersNode =====================================================

class DeclSpecifiersNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitDeclSpecifiers(this); }
};




// ======================================================== ImportStmtNode =======================================================

class ImportStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitImportStmt(this); }

private:
  // Members
  std::string identifier;
};
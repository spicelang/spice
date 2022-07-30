// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <queue>
#include <stack>
#include <utility>
#include <vector>

#include <ast/AstVisitor.h>
#include <symbol/Function.h>
#include <symbol/Struct.h>
#include <util/CodeLoc.h>

// =========================================================== AstNode ===========================================================

class AstNode {
public:
  // Constructors
  explicit AstNode(AstNode *parent, CodeLoc codeLoc) : parent(parent), codeLoc(std::move(codeLoc)) {}

  // Destructors
  virtual ~AstNode() {
    for (auto child : children)
      delete child;
  }

  // Virtual methods
  virtual std::any accept(AbstractAstVisitor *visitor) = 0;

  // Public methods
  template <typename T> T *createChild(const CodeLoc &codeLoc) {
    static_assert(std::is_base_of_v<AstNode, T>, "T must be derived from AstNode");
    T *node = new T(this, codeLoc);
    children.push_back(node);
    return node;
  }

  template <typename T> [[nodiscard]] T *getChild(size_t i = 0) const {
    static_assert(std::is_base_of_v<AstNode, T>, "T must be derived from AstNode");
    size_t j = 0;
    for (auto &child : children) {
      if (auto *typedChild = dynamic_cast<T *>(child); typedChild != nullptr) {
        if (j++ == i)
          return typedChild;
      }
    }
    return nullptr;
  }

  template <typename T> [[nodiscard]] std::vector<T *> getChildren() const {
    static_assert(std::is_base_of_v<AstNode, T>, "T must be derived from AstNode");
    std::vector<T *> nodes;
    for (auto &child : children) {
      if (auto *typedChild = dynamic_cast<T *>(child); typedChild != nullptr)
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
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitEntry(this); }

  // Public get methods
  [[nodiscard]] std::vector<MainFctDefNode *> mainFctDefNode() const { return getChildren<MainFctDefNode>(); }
  [[nodiscard]] std::vector<FctDefNode *> fctDefNode() const { return getChildren<FctDefNode>(); }
  [[nodiscard]] std::vector<ProcDefNode *> procDefNode() const { return getChildren<ProcDefNode>(); }
  [[nodiscard]] std::vector<StructDefNode *> structDefNode() const { return getChildren<StructDefNode>(); }
  [[nodiscard]] std::vector<GenericTypeDefNode *> genericTypeDefNode() const { return getChildren<GenericTypeDefNode>(); }
  [[nodiscard]] std::vector<GlobalVarDefNode *> globalVarDefNode() const { return getChildren<GlobalVarDefNode>(); }
  [[nodiscard]] std::vector<ImportStmtNode *> importStmtNode() const { return getChildren<ImportStmtNode>(); }
  [[nodiscard]] std::vector<ExtDeclNode *> extDeclNode() const { return getChildren<ExtDeclNode>(); }
};

// ======================================================== MainFctDefNode =======================================================

class MainFctDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitMainFctDef(this); }

  // Public get methods
  [[nodiscard]] ParamLstNode *paramLst() const { return getChild<ParamLstNode>(); }
  [[nodiscard]] StmtLstNode *stmtLst() const { return getChild<StmtLstNode>(); }

  // Public members
  SymbolTable *fctScope = nullptr;
  bool hasArgs = false;
};

// ========================================================== FctDefNode =========================================================

class FctDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitFctDef(this); }

  // Public get methods
  [[nodiscard]] SpecifierLstNode *specifierLst() const { return getChild<SpecifierLstNode>(); }
  [[nodiscard]] DataTypeNode *returnType() const { return getChild<DataTypeNode>(); }
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(); }
  [[nodiscard]] ParamLstNode *paramLst() const { return getChild<ParamLstNode>(); }
  [[nodiscard]] StmtLstNode *stmtLst() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "fct:" + codeLoc.toString(); }

  // Public members
  std::string functionName;
  std::string structName;
  std::string fqFunctionName;
  std::vector<std::string> functionNameFragments;
  bool isMethod = false;
  bool isGeneric = false;
  bool hasParams = false;
  SymbolTable *fctScope = nullptr;
  Function *spiceFunc = nullptr;
};

// ========================================================== ProcDefNode ========================================================

class ProcDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitProcDef(this); }

  // Public get methods
  [[nodiscard]] SpecifierLstNode *specifierLst() const { return getChild<SpecifierLstNode>(); }
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(); }
  [[nodiscard]] ParamLstNode *paramLst() const { return getChild<ParamLstNode>(); }
  [[nodiscard]] StmtLstNode *stmtLst() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "proc:" + codeLoc.toString(); }

  // Public members
  std::string procedureName;
  std::string structName;
  std::string fqProcedureName;
  std::vector<std::string> procedureNameFragments;
  bool isMethod = false;
  bool isGeneric = false;
  bool hasParams = false;
  SymbolTable *procScope = nullptr;
  Function *spiceProc = nullptr;
};

// ========================================================= StructDefNode =======================================================

class StructDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  [[nodiscard]] SpecifierLstNode *specifierLst() const { return getChild<SpecifierLstNode>(); }
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitStructDef(this); }
  [[nodiscard]] std::vector<FieldNode *> fields() const { return getChildren<FieldNode>(); }

  // Public get methods
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "struct:" + codeLoc.toString(); }

  // Public members
  std::string structName;
  bool isGeneric = false;
  Struct *spiceProc = nullptr;
};

// ====================================================== GenericTypeDefNode =====================================================

class GenericTypeDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitGenericTypeDef(this); }

  // Public get methods
  [[nodiscard]] TypeAltsLstNode *typeAltsLst() const { return getChild<TypeAltsLstNode>(); }
  [[nodiscard]] SpecifierLstNode *specifierLst() const { return getChild<SpecifierLstNode>(); }

  // Public members
  std::string typeName;
};

// ======================================================= GlobalVarDefNode ======================================================

class GlobalVarDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitGlobalVarDef(this); }

  // Public get methods
  [[nodiscard]] SpecifierLstNode *specifierLst() const { return getChild<SpecifierLstNode>(); }
  [[nodiscard]] DataTypeNode *dataType() const { return getChild<DataTypeNode>(); }
  [[nodiscard]] ValueNode *value() const { return getChild<ValueNode>(); }

  // Public members
  std::string varName;
  bool negative;
};

// ========================================================== ExtDeclNode ========================================================

class ExtDeclNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitExtDecl(this); }

  // Public get methods
  [[nodiscard]] DataTypeNode *returnType() const { return getChild<DataTypeNode>(); }
  [[nodiscard]] TypeLstNode *argTypeLst() const { return getChild<TypeLstNode>(); }

  // Public members
  std::string extFunctionName;
  bool hasArgs = false;
  bool isVarArg = false;
  bool isDll = false;
};

// ========================================================= ThreadDefNode =======================================================

class ThreadDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitThreadDef(this); }

  // Public get methods
  [[nodiscard]] StmtLstNode *stmtLst() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "thread:" + codeLoc.toString(); }
};

// ====================================================== UnsafeBlockDefNode =====================================================

class UnsafeBlockDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitUnsafeBlockDef(this); }

  // Public get methods
  [[nodiscard]] StmtLstNode *stmtLst() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "unsafe:" + codeLoc.toString(); }
};

// ========================================================== ForLoopNode ========================================================

class ForLoopNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitForLoop(this); }

  // Public get methods
  [[nodiscard]] DeclStmtNode *initDecl() const { return getChild<DeclStmtNode>(); }
  [[nodiscard]] AssignExprNode *condAssign() const { return getChild<AssignExprNode>(0); }
  [[nodiscard]] AssignExprNode *incAssign() const { return getChild<AssignExprNode>(1); }
  [[nodiscard]] StmtLstNode *stmtLst() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "for:" + codeLoc.toString(); }
};

// ======================================================== ForeachLoopNode ======================================================

class ForeachLoopNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitForeachLoop(this); }

  // Public get methods
  [[nodiscard]] DeclStmtNode *idxVarDecl() const {
    std::vector<DeclStmtNode *> declStmtNodes = getChildren<DeclStmtNode>();
    return declStmtNodes.size() == 2 ? declStmtNodes.front() : nullptr;
  }
  [[nodiscard]] DeclStmtNode *itemDecl() const { return getChildren<DeclStmtNode>().back(); }
  [[nodiscard]] AssignExprNode *arrayAssign() const { return getChild<AssignExprNode>(); }
  [[nodiscard]] StmtLstNode *stmtLst() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "foreach:" + codeLoc.toString(); }
};

// ========================================================= WhileLoopNode =======================================================

class WhileLoopNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitWhileLoop(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *condition() const { return getChild<AssignExprNode>(); }
  [[nodiscard]] StmtLstNode *stmtLst() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "while:" + codeLoc.toString(); }
};

// ========================================================== IfStmtNode =========================================================

class IfStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitIfStmt(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *condition() const { return getChild<AssignExprNode>(); }
  [[nodiscard]] StmtLstNode *stmtLst() const { return getChild<StmtLstNode>(); }
  [[nodiscard]] ElseStmtNode *elseStmt() const { return getChild<ElseStmtNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "if:" + codeLoc.toString(); }
};

// ========================================================= ElseStmtNode ========================================================

class ElseStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitElseStmt(this); }

  // Public get methods
  [[nodiscard]] IfStmtNode *ifStmt() const { return getChild<IfStmtNode>(); }
  [[nodiscard]] StmtLstNode *stmtLst() const { return getChild<StmtLstNode>(); }

  // Other methods
  [[nodiscard]] std::string getScopeId() const { return "if:" + codeLoc.toString(); }

  // Public members
  bool isElseIf = false;
};

// ======================================================== AssertStmtNode =======================================================

class AssertStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitAssertStmt(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *assignExpr() const { return getChild<AssignExprNode>(); }

  // Public members
  std::string expressionString;
};

// ========================================================= StmtLstNode =========================================================

class StmtLstNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitStmtLst(this); }
};

// ========================================================= TypeLstNode =========================================================

class TypeLstNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitTypeLst(this); }

  // Public get methods
  [[nodiscard]] std::vector<DataTypeNode *> dataTypes() const { return getChildren<DataTypeNode>(); }

  // Public members
  size_t numberOfTypes = 0;
};

// ======================================================= TypeAltsLstNode =======================================================

class TypeAltsLstNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitTypeAltsLst(this); }

  // Public get methods
  [[nodiscard]] std::vector<DataTypeNode *> dataTypes() const { return getChildren<DataTypeNode>(); }

  // Public members
  size_t numberOfAlts = 0;
};

// ======================================================== ParamLstNode =========================================================

class ParamLstNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitParamLst(this); }

  // Public get methods
  [[nodiscard]] std::vector<DeclStmtNode *> params() const { return getChildren<DeclStmtNode>(); }
};

// ========================================================== ArgLstNode =========================================================

class ArgLstNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitArgLst(this); }

  // Public get methods
  [[nodiscard]] std::vector<AssignExprNode *> args() const { return getChildren<AssignExprNode>(); }
};

// ========================================================== FieldNode ==========================================================

class FieldNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitField(this); }

  // Public get methods
  [[nodiscard]] SpecifierLstNode *specifierLst() const { return getChild<SpecifierLstNode>(); }
  [[nodiscard]] DataTypeNode *dataType() const { return getChild<DataTypeNode>(); }

  // Public members
  std::string name;
};

// =========================================================== StmtNode ==========================================================

class StmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitStmt(this); }
};

// ========================================================= DeclStmtNode ========================================================

class DeclStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitDeclStmt(this); }

  // Public get methods
  [[nodiscard]] SpecifierLstNode *specifierLst() const { return getChild<SpecifierLstNode>(); }
  [[nodiscard]] DataTypeNode *dataType() const { return getChild<DataTypeNode>(); }
  [[nodiscard]] AssignExprNode *assignExpr() const { return getChild<AssignExprNode>(); }

  // Public members
  std::string varName;
  bool hasAssignment = false;
};

// ======================================================= SpecifierLstNode ======================================================

class SpecifierLstNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitSpecifierLst(this); }

  // Public get methods
  [[nodiscard]] std::vector<SpecifierNode *> specifiers() const { return getChildren<SpecifierNode>(); }
};

// ========================================================= SpecifierNode =======================================================

class SpecifierNode : public AstNode {
public:
  // Enums
  enum SpecifierType { TY_CONST, TY_SIGNED, TY_UNSIGNED, TY_INLINE, TY_PUBLIC };

  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitSpecifier(this); }

  // Public members
  SpecifierType type;
};

// ======================================================== ImportStmtNode =======================================================

class ImportStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitImportStmt(this); }

  // Public members
  std::string importPath;
  std::string importName;
};

// ======================================================== ReturnStmtNode =======================================================

class ReturnStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitReturnStmt(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *assignExpr() const { return getChild<AssignExprNode>(); }

  // Public members
  bool hasReturnValue = false;
};

// ======================================================== BreakStmtNode ========================================================

class BreakStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitBreakStmt(this); }

  // Public members
  int breakTimes = 1;
};

// ======================================================= ContinueStmtNode ======================================================

class ContinueStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitContinueStmt(this); }

  // Public members
  int continueTimes = 1;
};

// ======================================================== PrintfCallNode =======================================================

class PrintfCallNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitPrintfCall(this); }

  // Public get methods
  [[nodiscard]] std::vector<AssignExprNode *> assignExpr() const { return getChildren<AssignExprNode>(); }

  // Public members
  std::string templatedString;
};

// ======================================================== SizeofCallNode =======================================================

class SizeofCallNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitSizeofCall(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *assignExpr() const { return getChild<AssignExprNode>(); }
  [[nodiscard]] DataTypeNode *dataType() const { return getChild<DataTypeNode>(); }

  // Public members
  bool isType = false;
};

// ========================================================= LenCallNode =========================================================

class LenCallNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitLenCall(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *assignExpr() const { return getChild<AssignExprNode>(); }
};

// ========================================================= TidCallNode =========================================================

class TidCallNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitTidCall(this); }
};

// ======================================================== JoinCallNode =========================================================

class JoinCallNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitJoinCall(this); }

  // Public get methods
  [[nodiscard]] std::vector<AssignExprNode *> assignExpressions() const { return getChildren<AssignExprNode>(); }
};

// ======================================================= AssignExprNode ========================================================

class AssignExprNode : public AstNode {
public:
  // Enums
  enum AssignOp {
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
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitAssignExpr(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *rhs() const { return getChild<AssignExprNode>(); }
  [[nodiscard]] PrefixUnaryExprNode *lhs() const { return getChild<PrefixUnaryExprNode>(); }
  [[nodiscard]] TernaryExprNode *ternaryExpr() const { return getChild<TernaryExprNode>(); }
  [[nodiscard]] ThreadDefNode *threadDef() const { return getChild<ThreadDefNode>(); }

  // Public members
  AssignOp op;
  bool hasOperator = false;
};

// ======================================================= TernaryExprNode =======================================================

class TernaryExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitTernaryExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<LogicalOrExprNode *> operands() const { return getChildren<LogicalOrExprNode>(); }
};

// ===================================================== LogicalOrExprNode =======================================================

class LogicalOrExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitLogicalOrExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<LogicalAndExprNode *> operands() const { return getChildren<LogicalAndExprNode>(); }
};

// ===================================================== LogicalAndExprNode ======================================================

class LogicalAndExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitLogicalAndExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<BitwiseOrExprNode *> operands() const { return getChildren<BitwiseOrExprNode>(); }
};

// ===================================================== BitwiseOrExprNode =======================================================

class BitwiseOrExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitBitwiseOrExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<BitwiseXorExprNode *> operands() const { return getChildren<BitwiseXorExprNode>(); }
};

// ==================================================== BitwiseXorExprNode =======================================================

class BitwiseXorExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitBitwiseXorExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<BitwiseAndExprNode *> operands() const { return getChildren<BitwiseAndExprNode>(); }
};

// ==================================================== BitwiseAndExprNode =======================================================

class BitwiseAndExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitBitwiseAndExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<EqualityExprNode *> operands() const { return getChildren<EqualityExprNode>(); }
};

// ===================================================== EqualityExprNode ========================================================

class EqualityExprNode : public AstNode {
public:
  // Enums
  enum EqualityOp { OP_EQUAL, OP_NOT_EQUAL };

  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitEqualityExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<RelationalExprNode *> operands() const { return getChildren<RelationalExprNode>(); }

  // Public members
  EqualityOp op;
};

// ==================================================== RelationalExprNode =======================================================

class RelationalExprNode : public AstNode {
public:
  // Enums
  enum RelationalOp { OP_LESS, OP_GREATER, OP_LESS_EQUAL, OP_GREATER_EQUAL };

  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitRelationalExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<ShiftExprNode *> operands() const { return getChildren<ShiftExprNode>(); }

  // Public members
  RelationalOp op;
};

// ====================================================== ShiftExprNode ==========================================================

class ShiftExprNode : public AstNode {
public:
  // Enums
  enum ShiftOp {
    OP_SHIFT_LEFT,
    OP_SHIFT_RIGHT,
  };

  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitShiftExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<AdditiveExprNode *> operands() const { return getChildren<AdditiveExprNode>(); }

  // Public members
  ShiftOp op;
};

// ==================================================== AdditiveExprNode =========================================================

class AdditiveExprNode : public AstNode {
public:
  // Enums
  enum AdditiveOp {
    OP_PLUS,
    OP_MINUS,
  };

  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitAdditiveExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<MultiplicativeExprNode *> operands() const { return getChildren<MultiplicativeExprNode>(); }

  // Public members
  std::queue<AdditiveOp> opQueue;
};

// ================================================== MultiplicativeExprNode =====================================================

class MultiplicativeExprNode : public AstNode {
public:
  // Enums
  enum MultiplicativeOp {
    OP_MUL,
    OP_DIV,
    OP_REM,
  };

  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitMultiplicativeExpr(this); }

  // Public get methods
  [[nodiscard]] std::vector<CastExprNode *> operands() const { return getChildren<CastExprNode>(); }

  // Public members
  std::queue<MultiplicativeOp> opQueue;
};

// ======================================================= CastExprNode ==========================================================

class CastExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitCastExpr(this); }

  // Public get methods
  [[nodiscard]] DataTypeNode *dataType() const { return getChild<DataTypeNode>(); }
  [[nodiscard]] PrefixUnaryExprNode *prefixUnaryExpr() const { return getChild<PrefixUnaryExprNode>(); }

  // Public members
  bool isCasted = false;
};

// ==================================================== PrefixUnaryExprNode ======================================================

class PrefixUnaryExprNode : public AstNode {
public:
  // Enums
  enum PrefixUnaryOp {
    OP_MINUS,
    OP_PLUS_PLUS,
    OP_MINUS_MINUS,
    OP_NOT,
    OP_BITWISE_NOT,
    OP_INDIRECTION,
    OP_ADDRESS_OF,
  };

  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitPrefixUnaryExpr(this); }

  // Public get methods
  [[nodiscard]] PostfixUnaryExprNode *postfixUnaryExpr() const { return getChild<PostfixUnaryExprNode>(); }

  // Public members
  std::stack<PrefixUnaryOp> opStack;
};

// =================================================== PostfixUnaryExprNode ======================================================

class PostfixUnaryExprNode : public AstNode {
public:
  // Enums
  enum PostfixUnaryOp { OP_SUBSCRIPT, OP_MEMBER_ACCESS, OP_PLUS_PLUS, OP_MINUS_MINUS };

  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitPostfixUnaryExpr(this); }

  // Public get methods
  [[nodiscard]] AtomicExprNode *atomicExpr() const { return getChild<AtomicExprNode>(); }
  [[nodiscard]] std::vector<AssignExprNode *> assignExpr() const { return getChildren<AssignExprNode>(); }
  [[nodiscard]] std::vector<PostfixUnaryExprNode *> postfixUnaryExpr() const { return getChildren<PostfixUnaryExprNode>(); }

  // Public members
  std::queue<PostfixUnaryOp> opQueue;
};

// ====================================================== AtomicExprNode =========================================================

class AtomicExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitAtomicExpr(this); }

  // Public get methods
  [[nodiscard]] AssignExprNode *assignExpr() const { return getChild<AssignExprNode>(); }
  [[nodiscard]] ValueNode *value() const { return getChild<ValueNode>(); }
  [[nodiscard]] PrintfCallNode *printfCall() const { return getChild<PrintfCallNode>(); }
  [[nodiscard]] SizeofCallNode *sizeofCall() const { return getChild<SizeofCallNode>(); }
  [[nodiscard]] LenCallNode *lenCall() const { return getChild<LenCallNode>(); }
  [[nodiscard]] TidCallNode *tidCall() const { return getChild<TidCallNode>(); }
  [[nodiscard]] JoinCallNode *joinCall() const { return getChild<JoinCallNode>(); }

  // Public members
  std::string identifier;
};

// ======================================================== ValueNode ============================================================

class ValueNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitValue(this); }

  // Public get methods
  [[nodiscard]] PrimitiveValueNode *primitiveValue() const { return getChild<PrimitiveValueNode>(); }
  [[nodiscard]] FunctionCallNode *functionCall() const { return getChild<FunctionCallNode>(); }
  [[nodiscard]] ArrayInitializationNode *arrayInitialization() const { return getChild<ArrayInitializationNode>(); }
  [[nodiscard]] StructInstantiationNode *structInstantiation() const { return getChild<StructInstantiationNode>(); }
  [[nodiscard]] DataTypeNode *nilType() const { return getChild<DataTypeNode>(); }

  // Public members
  bool isNil = false;
};

// =================================================== PrimitiveValueNode ========================================================

class PrimitiveValueNode : public AstNode {
public:
  // Enum
  enum PrimitiveValueType { TY_DOUBLE, TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_STRING, TY_BOOL };

  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitPrimitiveValue(this); }

  // Public members
  PrimitiveValueType type;
  struct {
    double_t doubleValue;
    int32_t intValue;
    int16_t shortValue;
    int64_t longValue;
    int8_t charValue;
    std::string stringValue;
    bool boolValue;
  } data;
};

// ==================================================== FunctionCallNode =========================================================

class FunctionCallNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitFunctionCall(this); }

  // Public get methods
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(); }
  [[nodiscard]] ArgLstNode *argLst() const { return getChild<ArgLstNode>(); }

  // Public members
  std::string fqFunctionName;
  std::vector<std::string> functionNameFragments;
  bool isGeneric = false;
  bool hasArgs = false;
  Function *functionAccessPtr;
};

// ================================================= ArrayInitializationNode =====================================================

class ArrayInitializationNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitArrayInitialization(this); }

  // Public get methods
  [[nodiscard]] ArgLstNode *itemLst() const { return getChild<ArgLstNode>(); }
};

// ================================================= StructInstantiationNode =====================================================

class StructInstantiationNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitStructInstantiation(this); }

  // Public get methods
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(); }
  [[nodiscard]] ArgLstNode *fieldLst() const { return getChild<ArgLstNode>(); }

  // Public members
  std::string fqStructName;
  std::vector<std::string> structNameFragments;
  Struct *structAccessPtr;
};

// ======================================================= DataTypeNode ==========================================================

class DataTypeNode : public AstNode {
public:
  // Enums
  enum TypeModifierType { TY_POINTER, TY_ARRAY };

  // Structs
  struct TypeModifier {
    TypeModifierType modifierType = TY_POINTER;
    bool isSizeHardcoded = false;
    int hardcodedSize = 0;
  };

  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitDataType(this); }

  // Public get methods
  [[nodiscard]] BaseDataTypeNode *baseDataType() const { return getChild<BaseDataTypeNode>(); }
  [[nodiscard]] std::vector<AssignExprNode *> arraySizeExpr() const { return getChildren<AssignExprNode>(); }

  // Public members
  std::queue<TypeModifier> tmQueue;
  SymbolType symbolType = SymbolType(TY_INVALID);
};

// ==================================================== BaseDataTypeNode =========================================================

class BaseDataTypeNode : public AstNode {
public:
  // Enums
  enum Type { TY_DOUBLE, TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_STRING, TY_BOOL, TY_DYN, TY_CUSTOM };

  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitBaseDataType(this); }

  // Public get methods
  [[nodiscard]] CustomDataTypeNode *customDataType() const { return getChild<CustomDataTypeNode>(); }

  // Public members
  Type type;
  SymbolType symbolType = SymbolType(TY_INVALID);
};

// ==================================================== CustomDataTypeNode =======================================================

class CustomDataTypeNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitCustomDataType(this); }

  // Public get methods
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(); }

  // Public members
  std::string fqTypeName;
  std::vector<std::string> typeNameFragments;
};
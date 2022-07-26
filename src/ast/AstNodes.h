// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <vector>
#include <queue>

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

  template <typename T> T *getChild(size_t i = 0) const {
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

  template <typename T> std::vector<T *> getChildren() const {
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

  // Public members
  Function *spiceFunc = nullptr;
};

// ========================================================== FctDefNode =========================================================

class FctDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitFctDef(this); }

  // Public members
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

  // Public members
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

  // Public members
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

  // Public members
  std::string typeName;
};

// ======================================================= GlobalVarDefNode ======================================================

class GlobalVarDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitGlobalVarDef(this); }

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
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitExtDecl(this); }

  // Public members
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

  // Public get methods
  DeclStmtNode *initDecl() const { return getChild<DeclStmtNode>(); }
  AssignExprNode *condAssign() const { return getChild<AssignExprNode>(0); }
  AssignExprNode *incAssign() const { return getChild<AssignExprNode>(1); }
};

// ======================================================== ForeachLoopNode ======================================================

class ForeachLoopNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitForeachLoop(this); }

  // Public get methods
  DeclStmtNode *idxVarDecl() const {
    std::vector<DeclStmtNode *> declStmtNodes = getChildren<DeclStmtNode>();
    return declStmtNodes.size() == 2 ? declStmtNodes.front() : nullptr;
  }
  DeclStmtNode *varDecl() const { return getChildren<DeclStmtNode>().back(); }
  AssignExprNode *arrayAssign() const { return getChild<AssignExprNode>(); }
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

// ========================================================= ElseStmtNode ========================================================

class ElseStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitElseStmt(this); }
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

  // Public members
  bool hasConstKeyword = false;
  bool hasSignedKeyword = false;
  bool hasUnsignedKeyword = false;
  bool hasInlineKeyword = false;
  bool hasPublicKeyword = false;
};

// ======================================================== ImportStmtNode =======================================================

class ImportStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitImportStmt(this); }

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
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitReturnStmt(this); }
};

// ======================================================== BreakStmtNode ========================================================

class BreakStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitBreakStmt(this); }

  // Public members
  int breakTimes;
};

// ======================================================= ContinueStmtNode ======================================================

class ContinueStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitContinueStmt(this); }

  // Public members
  int continueTimes;
};

// ======================================================== PrintfCallNode =======================================================

class PrintfCallNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitPrintfCall(this); }

  // Public members
  std::string templatedString;
};

// ======================================================== SizeofCallNode =======================================================

class SizeofCallNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitSizeofCall(this); }

  // Public members
  bool isType = false;
};

// ========================================================= LenCallNode =========================================================

class LenCallNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitLenCall(this); }
};

// ========================================================= TidCallNode =========================================================

class TidCallNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitTidCall(this); }
};

// ======================================================== JoinCallNode =========================================================

class JoinCallNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitJoinCall(this); }
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
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitAssignExpr(this); }

  // Public members
  AssignOp op;
};

// ======================================================= TernaryExprNode =======================================================

class TernaryExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitTernaryExpr(this); }
};

// ===================================================== LogicalOrExprNode =======================================================

class LogicalOrExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitLogicalOrExpr(this); }
};

// ===================================================== LogicalAndExprNode ======================================================

class LogicalAndExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitLogicalAndExpr(this); }
};

// ===================================================== BitwiseOrExprNode =======================================================

class BitwiseOrExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitBitwiseOrExpr(this); }
};

// ==================================================== BitwiseXorExprNode =======================================================

class BitwiseXorExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitBitwiseXorExpr(this); }
};

// ==================================================== BitwiseAndExprNode =======================================================

class BitwiseAndExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitBitwiseAndExpr(this); }
};

// ===================================================== EqualityExprNode ========================================================

class EqualityExprNode : public AstNode {
public:
  // Enums
  enum EqualityOp {
    OP_EQUAL,
    OP_NOT_EQUAL
  };

  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitEqualityExpr(this); }
};

// ==================================================== RelationalExprNode =======================================================

class RelationalExprNode : public AstNode {
public:
  // Enums
  enum RelationalOp {
    OP_LESS,
    OP_GREATER,
    OP_LESS_EQUAL,
    OP_GREATER_EQUAL
  };

  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitRelationalExpr(this); }
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
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitShiftExpr(this); }
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
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitAdditiveExpr(this); }
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
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitMultiplicativeExpr(this); }
};

// ======================================================= CastExprNode ==========================================================

class CastExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitCastExpr(this); }
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
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitPrefixUnaryExpr(this); }

  // Public members
  std::queue<PrefixUnaryOp> opQueue;
};

// =================================================== PostfixUnaryExprNode ======================================================

class PostfixUnaryExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitPostfixUnaryExpr(this); }
};

// ====================================================== AtomicExprNode =========================================================

class AtomicExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitAtomicExpr(this); }
};

// ======================================================== ValueNode ============================================================

class ValueNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitValue(this); }
};

// =================================================== PrimitiveValueNode ========================================================

class PrimitiveValueNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitPrimitiveValue(this); }
};

// ==================================================== FunctionCallNode =========================================================

class FunctionCallNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitFunctionCall(this); }

  // Public members
  Function *functionAccessPtr;
};

// ================================================= ArrayInitializationNode =====================================================

class ArrayInitializationNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitArrayInitialization(this); }
};

// ================================================= StructInstantiationNode =====================================================

class StructInstantiationNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitStructInstantiation(this); }

  // Public members
  Struct *structAccessPtr;
};

// ======================================================= DataTypeNode ==========================================================

class DataTypeNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitDataType(this); }
};

// ==================================================== BaseDataTypeNode =========================================================

class BaseDataTypeNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitBaseDataType(this); }
};

// ==================================================== CustomDataTypeNode =======================================================

class CustomDataTypeNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) const override { return visitor->visitCustomDataType(this); }
};
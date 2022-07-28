// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <queue>
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
  [[nodiscard]] ParamLstNode *argLstDef() const { return getChild<ParamLstNode>(); }
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
  [[nodiscard]] DeclSpecifiersNode *declSpecifiers() const { return getChild<DeclSpecifiersNode>(); }
  [[nodiscard]] DataTypeNode *returnType() const { return getChild<DataTypeNode>(); }
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(); }
  [[nodiscard]] ParamLstNode *argLstDef() const { return getChild<ParamLstNode>(); }
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
  [[nodiscard]] DeclSpecifiersNode *declSpecifiers() const { return getChild<DeclSpecifiersNode>(); }
  [[nodiscard]] TypeLstNode *templateTypeLst() const { return getChild<TypeLstNode>(); }
  [[nodiscard]] ParamLstNode *argLstDef() const { return getChild<ParamLstNode>(); }
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
  [[nodiscard]] DeclSpecifiersNode *declSpecifiers() const { return getChild<DeclSpecifiersNode>(); }
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
};

// ========================================================= ThreadDefNode =======================================================

class ThreadDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitThreadDef(this); }
};

// ====================================================== UnsafeBlockDefNode =====================================================

class UnsafeBlockDefNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitUnsafeBlockDef(this); }
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
  [[nodiscard]] DeclStmtNode *varDecl() const { return getChildren<DeclStmtNode>().back(); }
  [[nodiscard]] AssignExprNode *arrayAssign() const { return getChild<AssignExprNode>(); }
};

// ========================================================= WhileLoopNode =======================================================

class WhileLoopNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitWhileLoop(this); }
};

// ========================================================== IfStmtNode =========================================================

class IfStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitIfStmt(this); }
};

// ========================================================= ElseStmtNode ========================================================

class ElseStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitElseStmt(this); }
};

// ======================================================== AssertStmtNode =======================================================

class AssertStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitAssertStmt(this); }
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
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitArgLstDef(this); }

  // Public get methods
  [[nodiscard]] std::vector<DeclStmtNode *> declStmts() const { return getChildren<DeclStmtNode>(); }
};

// ========================================================== ArgLstNode =========================================================

class ArgLstNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitArgLst(this); }
};

// ========================================================== FieldNode ==========================================================

class FieldNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitField(this); }

  // Public get methods
  [[nodiscard]] DeclSpecifiersNode *declSpecifiers() const { return getChild<DeclSpecifiersNode>(); }
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

  // Public members
  std::string varName;
};

// ====================================================== DeclSpecifiersNode =====================================================

class DeclSpecifiersNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitDeclSpecifiers(this); }

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
};

// ======================================================== BreakStmtNode ========================================================

class BreakStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitBreakStmt(this); }

  // Public members
  int breakTimes;
};

// ======================================================= ContinueStmtNode ======================================================

class ContinueStmtNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitContinueStmt(this); }

  // Public members
  int continueTimes;
};

// ======================================================== PrintfCallNode =======================================================

class PrintfCallNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitPrintfCall(this); }

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

  // Public members
  AssignOp op;
};

// ======================================================= TernaryExprNode =======================================================

class TernaryExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitTernaryExpr(this); }
};

// ===================================================== LogicalOrExprNode =======================================================

class LogicalOrExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitLogicalOrExpr(this); }
};

// ===================================================== LogicalAndExprNode ======================================================

class LogicalAndExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitLogicalAndExpr(this); }
};

// ===================================================== BitwiseOrExprNode =======================================================

class BitwiseOrExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitBitwiseOrExpr(this); }
};

// ==================================================== BitwiseXorExprNode =======================================================

class BitwiseXorExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitBitwiseXorExpr(this); }
};

// ==================================================== BitwiseAndExprNode =======================================================

class BitwiseAndExprNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitBitwiseAndExpr(this); }
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

  // Public members
  std::queue<PrefixUnaryOp> opQueue;
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

  // Public members
  std::string fqFunctionName;
  std::vector<std::string> functionNameFragments;
  Function *functionAccessPtr;
};

// ================================================= ArrayInitializationNode =====================================================

class ArrayInitializationNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitArrayInitialization(this); }
};

// ================================================= StructInstantiationNode =====================================================

class StructInstantiationNode : public AstNode {
public:
  // Constructors
  using AstNode::AstNode;

  // Visitor methods
  std::any accept(AbstractAstVisitor *visitor) override { return visitor->visitStructInstantiation(this); }

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
    TypeModifierType modifierType;
    bool isSizeHardcoded;
    int hardcodedSize;
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
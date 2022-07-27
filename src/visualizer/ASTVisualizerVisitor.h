// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <ast/AbstractAstVisitor.h>
#include <ast/AstNodes.h>

/**
 * Visitor for debug purposes (is only executed in the compiler debug mode)
 *
 * Jobs:
 * - Visualize AST
 */
class ASTVisualizerVisitor : public AbstractAstVisitor {
public:
  // Constructors
  explicit ASTVisualizerVisitor(const AstNode *ast) : ast(ast){};

  // Visitor methods
  std::any visitEntry(const EntryNode *ctx) override { return buildNode(ctx); }
  std::any visitMainFctDef(const MainFctDefNode *ctx) override { return buildNode(ctx); }
  std::any visitFctDef(const FctDefNode *ctx) override { return buildNode(ctx); }
  std::any visitProcDef(const ProcDefNode *ctx) override { return buildNode(ctx); }
  std::any visitStructDef(const StructDefNode *ctx) override { return buildNode(ctx); }
  std::any visitGenericTypeDef(const GenericTypeDefNode *ctx) override { return buildNode(ctx); }
  std::any visitGlobalVarDef(const GlobalVarDefNode *ctx) override { return buildNode(ctx); }
  std::any visitExtDecl(const ExtDeclNode *ctx) override { return buildNode(ctx); }
  std::any visitThreadDef(const ThreadDefNode *ctx) override { return buildNode(ctx); }
  std::any visitUnsafeBlockDef(const UnsafeBlockDefNode *ctx) override { return buildNode(ctx); }
  std::any visitForLoop(const ForLoopNode *ctx) override { return buildNode(ctx); }
  std::any visitForeachLoop(const ForeachLoopNode *ctx) override { return buildNode(ctx); }
  std::any visitWhileLoop(const WhileLoopNode *ctx) override { return buildNode(ctx); }
  std::any visitIfStmt(const IfStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitElseStmt(const ElseStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitAssertStmt(const AssertStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitStmtLst(const StmtLstNode *ctx) override { return buildNode(ctx); }
  std::any visitTypeLst(const TypeLstNode *ctx) override { return buildNode(ctx); }
  std::any visitTypeAltsLst(const TypeAltsLstNode *ctx) override { return buildNode(ctx); }
  std::any visitArgLstDef(const ArgLstDefNode *ctx) override { return buildNode(ctx); }
  std::any visitArgLst(const ArgLstNode *ctx) override { return buildNode(ctx); }
  std::any visitField(const FieldNode *ctx) override { return buildNode(ctx); }
  std::any visitStmt(const StmtNode *ctx) override { return buildNode(ctx); }
  std::any visitDeclStmt(const DeclStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitDeclSpecifiers(const DeclSpecifiersNode *ctx) override { return buildNode(ctx); }
  std::any visitImportStmt(const ImportStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitReturnStmt(const ReturnStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitBreakStmt(const BreakStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitContinueStmt(const ContinueStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitPrintfCall(const PrintfCallNode *ctx) override { return buildNode(ctx); }
  std::any visitSizeofCall(const SizeofCallNode *ctx) override { return buildNode(ctx); }
  std::any visitLenCall(const LenCallNode *ctx) override { return buildNode(ctx); }
  std::any visitTidCall(const TidCallNode *ctx) override { return buildNode(ctx); }
  std::any visitJoinCall(const JoinCallNode *ctx) override { return buildNode(ctx); }
  std::any visitAssignExpr(const AssignExprNode *ctx) override { return buildNode(ctx); }
  std::any visitTernaryExpr(const TernaryExprNode *ctx) override { return buildNode(ctx); }
  std::any visitLogicalOrExpr(const LogicalOrExprNode *ctx) override { return buildNode(ctx); }
  std::any visitLogicalAndExpr(const LogicalAndExprNode *ctx) override { return buildNode(ctx); }
  std::any visitBitwiseOrExpr(const BitwiseOrExprNode *ctx) override { return buildNode(ctx); }
  std::any visitBitwiseXorExpr(const BitwiseXorExprNode *ctx) override { return buildNode(ctx); }
  std::any visitBitwiseAndExpr(const BitwiseAndExprNode *ctx) override { return buildNode(ctx); }
  std::any visitEqualityExpr(const EqualityExprNode *ctx) override { return buildNode(ctx); }
  std::any visitRelationalExpr(const RelationalExprNode *ctx) override { return buildNode(ctx); }
  std::any visitShiftExpr(const ShiftExprNode *ctx) override { return buildNode(ctx); }
  std::any visitAdditiveExpr(const AdditiveExprNode *ctx) override { return buildNode(ctx); }
  std::any visitMultiplicativeExpr(const MultiplicativeExprNode *ctx) override { return buildNode(ctx); }
  std::any visitCastExpr(const CastExprNode *ctx) override { return buildNode(ctx); }
  std::any visitPrefixUnaryExpr(const PrefixUnaryExprNode *ctx) override { return buildNode(ctx); }
  std::any visitPostfixUnaryExpr(const PostfixUnaryExprNode *ctx) override { return buildNode(ctx); }
  std::any visitAtomicExpr(const AtomicExprNode *ctx) override { return buildNode(ctx); }
  std::any visitValue(const ValueNode *ctx) override { return buildNode(ctx); }
  std::any visitPrimitiveValue(const PrimitiveValueNode *ctx) override { return buildNode(ctx); }
  std::any visitFunctionCall(const FunctionCallNode *ctx) override { return buildNode(ctx); }
  std::any visitArrayInitialization(const ArrayInitializationNode *ctx) override { return buildNode(ctx); }
  std::any visitStructInstantiation(const StructInstantiationNode *ctx) override { return buildNode(ctx); }
  std::any visitDataType(const DataTypeNode *ctx) override { return buildNode(ctx); }
  std::any visitBaseDataType(const BaseDataTypeNode *ctx) override { return buildNode(ctx); }
  std::any visitCustomDataType(const CustomDataTypeNode *ctx) override { return buildNode(ctx); }

private:
  // Members
  const AstNode *ast;
  const std::vector<std::string> nodeNames;
  int currentTabs = 1;
  std::string parentNodeId;

  // Private methods
  template <typename T>
  std::string buildNode(const T *ctx) {
    // Prepare strings
    std::string codeLoc = ctx->codeLoc.toString();
    std::string nodeName = getNodeName(ctx);
    std::string nodeId = codeLoc + "_" + nodeName;

    // Build result
    std::string result = nodeId + R"( [color="lightgreen",label=")" + nodeName + "\"];\n";

    // Link parent node with the current one
    std::string parentNodeIdBackup = parentNodeId;
    if (!parentNodeId.empty())
      result += getSpaces() + parentNodeId + " -> " + nodeId + ";\n";
    parentNodeId = nodeId; // Set parentNodeId for children

    // Visit all the children
    for (auto &child : ctx->children) {
      result += getSpaces();
      result += any_cast<std::string>(visit(child));
    }

    // Restore parent node id
    parentNodeId = parentNodeIdBackup;

    return result;
  }

  template <typename T>
  std::string getNodeName(const T*) const {
    if (std::is_same<EntryNode, T>())
      return "Entry";
    if (std::is_same<MainFctDefNode, T>())
      return "MainFctDef";
    if (std::is_same<FctDefNode, T>())
      return "FctDef";
    if (std::is_same<ProcDefNode, T>())
      return "ProcDef";
    if (std::is_same<StructDefNode, T>())
      return "StructDef";
    if (std::is_same<GenericTypeDefNode, T>())
      return "GenericTypeDef";
    if (std::is_same<GlobalVarDefNode, T>())
      return "GlobalVarDef";
    if (std::is_same<ExtDeclNode, T>())
      return "ExtDecl";
    if (std::is_same<ThreadDefNode, T>())
      return "ThreadDef";
    if (std::is_same<UnsafeBlockDefNode, T>())
      return "UnsafeBlockDef";
    if (std::is_same<ForLoopNode, T>())
      return "ForLoop";
    if (std::is_same<ForeachLoopNode, T>())
      return "ForeachLoop";
    if (std::is_same<WhileLoopNode, T>())
      return "WhileLoop";
    if (std::is_same<IfStmtNode, T>())
      return "IfStmt";
    if (std::is_same<ElseStmtNode, T>())
      return "ElseStmt";
    if (std::is_same<AssertStmtNode, T>())
      return "AssertStmt";
    if (std::is_same<StmtLstNode, T>())
      return "StmtLst";
    if (std::is_same<TypeLstNode, T>())
      return "TypeLst";
    if (std::is_same<TypeAltsLstNode, T>())
      return "TypeAltsLst";
    if (std::is_same<ArgLstDefNode, T>())
      return "ArgLstDef";
    if (std::is_same<ArgLstNode, T>())
      return "ArgLst";
    if (std::is_same<FieldNode, T>())
      return "Field";
    if (std::is_same<StmtNode, T>())
      return "Stmt";
    if (std::is_same<DeclStmtNode, T>())
      return "DeclStmt";
    if (std::is_same<DeclSpecifiersNode, T>())
      return "DeclSpecifiers";
    if (std::is_same<ImportStmtNode, T>())
      return "ImportStmt";
    if (std::is_same<ReturnStmtNode, T>())
      return "ReturnStmt";
    if (std::is_same<BreakStmtNode, T>())
      return "BreakStmt";
    if (std::is_same<ContinueStmtNode, T>())
      return "ContinueStmt";
    if (std::is_same<PrintfCallNode, T>())
      return "PrintfCall";
    if (std::is_same<SizeofCallNode, T>())
      return "SizeofCall";
    if (std::is_same<LenCallNode, T>())
      return "LenCall";
    if (std::is_same<TidCallNode, T>())
      return "TidCall";
    if (std::is_same<JoinCallNode, T>())
      return "JoinCall";
    if (std::is_same<AssignExprNode, T>())
      return "AssignExpr";
    if (std::is_same<TernaryExprNode, T>())
      return "TernaryExpr";
    if (std::is_same<LogicalOrExprNode, T>())
      return "LogicalOrExpr";
    if (std::is_same<LogicalAndExprNode, T>())
      return "LogicalAndExpr";
    if (std::is_same<BitwiseOrExprNode, T>())
      return "BitwiseOrExpr";
    if (std::is_same<BitwiseXorExprNode, T>())
      return "BitwiseXorExpr";
    if (std::is_same<BitwiseAndExprNode, T>())
      return "BitwiseAndExpr";
    if (std::is_same<EqualityExprNode, T>())
      return "EqualityExpr";
    if (std::is_same<RelationalExprNode, T>())
      return "RelationalExpr";
    if (std::is_same<ShiftExprNode, T>())
      return "ShiftExpr";
    if (std::is_same<AdditiveExprNode, T>())
      return "AdditiveExpr";
    if (std::is_same<MultiplicativeExprNode, T>())
      return "MultiplicativeExpr";
    if (std::is_same<CastExprNode, T>())
      return "CastExpr";
    if (std::is_same<PrefixUnaryExprNode, T>())
      return "PrefixUnaryExpr";
    if (std::is_same<PostfixUnaryExprNode, T>())
      return "PostfixUnaryExpr";
    if (std::is_same<AtomicExprNode, T>())
      return "AtomicExpr";
    if (std::is_same<ValueNode, T>())
      return "Value";
    if (std::is_same<PrimitiveValueNode, T>())
      return "PrimitiveValue";
    if (std::is_same<FunctionCallNode, T>())
      return "FunctionCall";
    if (std::is_same<ArrayInitializationNode, T>())
      return "ArrayInitialization";
    if (std::is_same<StructInstantiationNode, T>())
      return "StructInstantiation";
    if (std::is_same<DataTypeNode, T>())
      return "DataType";
    if (std::is_same<BaseDataTypeNode, T>())
      return "BaseDataType";
    if (std::is_same<CustomDataTypeNode, T>())
      return "CustomDataType";
    throw std::runtime_error("Unknown node name");
  }

  [[nodiscard]] std::string getSpaces() const;
};

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
  std::any visitEntry(EntryNode *ctx) override { return buildNode(ctx); }
  std::any visitMainFctDef(MainFctDefNode *ctx) override { return buildNode(ctx); }
  std::any visitFctDef(FctDefNode *ctx) override { return buildNode(ctx); }
  std::any visitProcDef(ProcDefNode *ctx) override { return buildNode(ctx); }
  std::any visitStructDef(StructDefNode *ctx) override { return buildNode(ctx); }
  std::any visitGenericTypeDef(GenericTypeDefNode *ctx) override { return buildNode(ctx); }
  std::any visitGlobalVarDef(GlobalVarDefNode *ctx) override { return buildNode(ctx); }
  std::any visitExtDecl(ExtDeclNode *ctx) override { return buildNode(ctx); }
  std::any visitThreadDef(ThreadDefNode *ctx) override { return buildNode(ctx); }
  std::any visitUnsafeBlockDef(UnsafeBlockDefNode *ctx) override { return buildNode(ctx); }
  std::any visitForLoop(ForLoopNode *ctx) override { return buildNode(ctx); }
  std::any visitForeachLoop(ForeachLoopNode *ctx) override { return buildNode(ctx); }
  std::any visitWhileLoop(WhileLoopNode *ctx) override { return buildNode(ctx); }
  std::any visitIfStmt(IfStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitElseStmt(ElseStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitAssertStmt(AssertStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitStmtLst(StmtLstNode *ctx) override { return buildNode(ctx); }
  std::any visitTypeLst(TypeLstNode *ctx) override { return buildNode(ctx); }
  std::any visitTypeAltsLst(TypeAltsLstNode *ctx) override { return buildNode(ctx); }
  std::any visitParamLst(ParamLstNode *ctx) override { return buildNode(ctx); }
  std::any visitArgLst(ArgLstNode *ctx) override { return buildNode(ctx); }
  std::any visitField(FieldNode *ctx) override { return buildNode(ctx); }
  std::any visitStmt(StmtNode *ctx) override { return buildNode(ctx); }
  std::any visitDeclStmt(DeclStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitSpecifierLst(SpecifierLstNode *ctx) override { return buildNode(ctx); }
  std::any visitSpecifier(SpecifierNode *ctx) override { return buildNode(ctx); }
  std::any visitImportStmt(ImportStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitReturnStmt(ReturnStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitBreakStmt(BreakStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitContinueStmt(ContinueStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitPrintfCall(PrintfCallNode *ctx) override { return buildNode(ctx); }
  std::any visitSizeofCall(SizeofCallNode *ctx) override { return buildNode(ctx); }
  std::any visitLenCall(LenCallNode *ctx) override { return buildNode(ctx); }
  std::any visitTidCall(TidCallNode *ctx) override { return buildNode(ctx); }
  std::any visitJoinCall(JoinCallNode *ctx) override { return buildNode(ctx); }
  std::any visitAssignExpr(AssignExprNode *ctx) override { return buildNode(ctx); }
  std::any visitTernaryExpr(TernaryExprNode *ctx) override { return buildNode(ctx); }
  std::any visitLogicalOrExpr(LogicalOrExprNode *ctx) override { return buildNode(ctx); }
  std::any visitLogicalAndExpr(LogicalAndExprNode *ctx) override { return buildNode(ctx); }
  std::any visitBitwiseOrExpr(BitwiseOrExprNode *ctx) override { return buildNode(ctx); }
  std::any visitBitwiseXorExpr(BitwiseXorExprNode *ctx) override { return buildNode(ctx); }
  std::any visitBitwiseAndExpr(BitwiseAndExprNode *ctx) override { return buildNode(ctx); }
  std::any visitEqualityExpr(EqualityExprNode *ctx) override { return buildNode(ctx); }
  std::any visitRelationalExpr(RelationalExprNode *ctx) override { return buildNode(ctx); }
  std::any visitShiftExpr(ShiftExprNode *ctx) override { return buildNode(ctx); }
  std::any visitAdditiveExpr(AdditiveExprNode *ctx) override { return buildNode(ctx); }
  std::any visitMultiplicativeExpr(MultiplicativeExprNode *ctx) override { return buildNode(ctx); }
  std::any visitCastExpr(CastExprNode *ctx) override { return buildNode(ctx); }
  std::any visitPrefixUnaryExpr(PrefixUnaryExprNode *ctx) override { return buildNode(ctx); }
  std::any visitPostfixUnaryExpr(PostfixUnaryExprNode *ctx) override { return buildNode(ctx); }
  std::any visitAtomicExpr(AtomicExprNode *ctx) override { return buildNode(ctx); }
  std::any visitValue(ValueNode *ctx) override { return buildNode(ctx); }
  std::any visitPrimitiveValue(PrimitiveValueNode *ctx) override { return buildNode(ctx); }
  std::any visitFunctionCall(FunctionCallNode *ctx) override { return buildNode(ctx); }
  std::any visitArrayInitialization(ArrayInitializationNode *ctx) override { return buildNode(ctx); }
  std::any visitStructInstantiation(StructInstantiationNode *ctx) override { return buildNode(ctx); }
  std::any visitDataType(DataTypeNode *ctx) override { return buildNode(ctx); }
  std::any visitBaseDataType(BaseDataTypeNode *ctx) override { return buildNode(ctx); }
  std::any visitCustomDataType(CustomDataTypeNode *ctx) override { return buildNode(ctx); }

private:
  // Members
  const AstNode *ast;
  const std::vector<std::string> nodeNames;
  int currentTabs = 1;
  std::string parentNodeId;

  // Private methods
  template <typename T> std::string buildNode(const T *ctx) {
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

  template <typename T> std::string getNodeName(const T *) const {
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
    if (std::is_same<ParamLstNode, T>())
      return "ParamLst";
    if (std::is_same<ArgLstNode, T>())
      return "ArgLst";
    if (std::is_same<FieldNode, T>())
      return "Field";
    if (std::is_same<StmtNode, T>())
      return "Stmt";
    if (std::is_same<DeclStmtNode, T>())
      return "DeclStmt";
    if (std::is_same<SpecifierLstNode, T>())
      return "SpecifierLst";
    if (std::is_same<SpecifierNode, T>())
      return "Specifier";
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
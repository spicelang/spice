// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <CompilerPass.h>
#include <ast/ASTNodes.h>
#include <ast/AbstractASTVisitor.h>

#define ASTVResult std::string

/**
 * Visitor for debug purposes (is only executed in the compiler debug mode and when explicitly enabling it via cli flag)
 *
 * Jobs:
 * - Visualize AST
 */
class ASTVisualizer : private CompilerPass, public AbstractASTVisitor<ASTVResult> {
public:
  // Constructors
  ASTVisualizer(GlobalResourceManager &resourceManager, SourceFile *sourceFile, const ASTNode *ast)
      : CompilerPass(resourceManager, sourceFile), ast(ast){};

  // Visitor methods
  ASTVResult visitEntry(EntryNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitMainFctDef(MainFctDefNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitFctDef(FctDefNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitProcDef(ProcDefNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitStructDef(StructDefNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitInterfaceDef(InterfaceDefNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitEnumDef(EnumDefNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitGenericTypeDef(GenericTypeDefNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitGlobalVarDef(GlobalVarDefNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitExtDecl(ExtDeclNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitThreadDef(ThreadDefNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitUnsafeBlockDef(UnsafeBlockDefNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitForLoop(ForLoopNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitForeachLoop(ForeachLoopNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitWhileLoop(WhileLoopNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitIfStmt(IfStmtNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitElseStmt(ElseStmtNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitAssertStmt(AssertStmtNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitAnonymousBlockStmt(AnonymousBlockStmtNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitStmtLst(StmtLstNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitTypeLst(TypeLstNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitTypeAltsLst(TypeAltsLstNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitParamLst(ParamLstNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitArgLst(ArgLstNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitEnumItemLst(EnumItemLstNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitEnumItem(EnumItemNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitField(FieldNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitSignature(SignatureNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitStmt(StmtNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitDeclStmt(DeclStmtNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitSpecifierLst(SpecifierLstNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitSpecifier(SpecifierNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitImportStmt(ImportStmtNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitReturnStmt(ReturnStmtNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitBreakStmt(BreakStmtNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitContinueStmt(ContinueStmtNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitPrintfCall(PrintfCallNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitSizeofCall(SizeofCallNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitLenCall(LenCallNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitTidCall(TidCallNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitJoinCall(JoinCallNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitAssignExpr(AssignExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitTernaryExpr(TernaryExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitLogicalOrExpr(LogicalOrExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitLogicalAndExpr(LogicalAndExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitBitwiseOrExpr(BitwiseOrExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitBitwiseXorExpr(BitwiseXorExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitBitwiseAndExpr(BitwiseAndExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitEqualityExpr(EqualityExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitRelationalExpr(RelationalExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitShiftExpr(ShiftExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitAdditiveExpr(AdditiveExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitMultiplicativeExpr(MultiplicativeExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitCastExpr(CastExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitPrefixUnaryExpr(PrefixUnaryExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitPostfixUnaryExpr(PostfixUnaryExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitAtomicExpr(AtomicExprNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitValue(ValueNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitPrimitiveValue(PrimitiveValueNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitFunctionCall(FunctionCallNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitArrayInitialization(ArrayInitializationNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitStructInstantiation(StructInstantiationNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitDataType(DataTypeNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitBaseDataType(BaseDataTypeNode *ctx) override { return buildNode(ctx); }
  ASTVResult visitCustomDataType(CustomDataTypeNode *ctx) override { return buildNode(ctx); }

private:
  // Members
  const ASTNode *ast;
  const std::vector<std::string> nodeNames;
  int currentTabs = 1;
  std::string parentNodeId;

  // Private methods
  template <typename T> std::string buildNode(const T *ctx) {
    std::stringstream result;

    // Prepare strings
    std::string codeLoc = ctx->codeLoc.toString();
    std::string nodeName = getNodeName(ctx);
    std::string nodeId = codeLoc + "_" + nodeName;

    // Build result
    result << nodeId << R"( [color="lightgreen",label=")" << nodeName << "\"];\n";

    // Link parent node with the current one
    std::string parentNodeIdBackup = parentNodeId;
    if (!parentNodeId.empty())
      result << getSpaces() << parentNodeId << " -> " << nodeId << ";\n";
    parentNodeId = nodeId; // Set parentNodeId for children

    // Visit all the children
    for (const auto &child : ctx->children) {
      result << getSpaces();
      result << visit(child);
    }

    // Restore parent node id
    parentNodeId = parentNodeIdBackup;

    return result.str();
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
    if (std::is_same<InterfaceDefNode, T>())
      return "InterfaceDef";
    if (std::is_same<EnumDefNode, T>())
      return "EnumDef";
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
    if (std::is_same<AnonymousBlockStmtNode, T>())
      return "AnonymousBlockStmt";
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
    if (std::is_same<EnumItemLstNode, T>())
      return "EnumItemLst";
    if (std::is_same<EnumItemNode, T>())
      return "EnumItem";
    if (std::is_same<FieldNode, T>())
      return "Field";
    if (std::is_same<SignatureNode, T>())
      return "Signature";
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
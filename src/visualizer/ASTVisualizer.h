// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <CompilerPass.h>
#include <ast/ASTNodes.h>
#include <ast/AbstractASTVisitor.h>

namespace spice::compiler {

/**
 * Visitor for debug purposes (is only executed in the compiler debug mode and when explicitly enabling it via cli flag)
 *
 * Jobs:
 * - Visualize AST
 */
class ASTVisualizer : private CompilerPass, public AbstractASTVisitor {
public:
  // Constructors
  using CompilerPass::CompilerPass;

  // Visitor methods
  std::any visitEntry(EntryNode *ctx) override { return buildNode(ctx); }
  std::any visitMainFctDef(MainFctDefNode *ctx) override { return buildNode(ctx); }
  std::any visitFctDef(FctDefNode *ctx) override { return buildNode(ctx); }
  std::any visitProcDef(ProcDefNode *ctx) override { return buildNode(ctx); }
  std::any visitFctName(FctNameNode *ctx) override { return buildNode(ctx); }
  std::any visitStructDef(StructDefNode *ctx) override { return buildNode(ctx); }
  std::any visitInterfaceDef(InterfaceDefNode *ctx) override { return buildNode(ctx); }
  std::any visitEnumDef(EnumDefNode *ctx) override { return buildNode(ctx); }
  std::any visitGenericTypeDef(GenericTypeDefNode *ctx) override { return buildNode(ctx); }
  std::any visitAliasDef(AliasDefNode *ctx) override { return buildNode(ctx); }
  std::any visitGlobalVarDef(GlobalVarDefNode *ctx) override { return buildNode(ctx); }
  std::any visitExtDecl(ExtDeclNode *ctx) override { return buildNode(ctx); }
  std::any visitImportDef(ImportDefNode *ctx) override { return buildNode(ctx); }
  std::any visitUnsafeBlock(UnsafeBlockNode *ctx) override { return buildNode(ctx); }
  std::any visitForLoop(ForLoopNode *ctx) override { return buildNode(ctx); }
  std::any visitForeachLoop(ForeachLoopNode *ctx) override { return buildNode(ctx); }
  std::any visitWhileLoop(WhileLoopNode *ctx) override { return buildNode(ctx); }
  std::any visitDoWhileLoop(DoWhileLoopNode *ctx) override { return buildNode(ctx); }
  std::any visitIfStmt(IfStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitElseStmt(ElseStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitSwitchStmt(SwitchStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitCaseBranch(CaseBranchNode *ctx) override { return buildNode(ctx); }
  std::any visitDefaultBranch(DefaultBranchNode *ctx) override { return buildNode(ctx); }
  std::any visitAssertStmt(AssertStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitAnonymousBlockStmt(AnonymousBlockStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitStmtLst(StmtLstNode *ctx) override { return buildNode(ctx); }
  std::any visitTypeLst(TypeLstNode *ctx) override { return buildNode(ctx); }
  std::any visitTypeAltsLst(TypeAltsLstNode *ctx) override { return buildNode(ctx); }
  std::any visitParamLst(ParamLstNode *ctx) override { return buildNode(ctx); }
  std::any visitArgLst(ArgLstNode *ctx) override { return buildNode(ctx); }
  std::any visitEnumItemLst(EnumItemLstNode *ctx) override { return buildNode(ctx); }
  std::any visitEnumItem(EnumItemNode *ctx) override { return buildNode(ctx); }
  std::any visitField(FieldNode *ctx) override { return buildNode(ctx); }
  std::any visitSignature(SignatureNode *ctx) override { return buildNode(ctx); }
  std::any visitDeclStmt(DeclStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitExprStmt(ExprStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitSpecifierLst(SpecifierLstNode *ctx) override { return buildNode(ctx); }
  std::any visitSpecifier(SpecifierNode *ctx) override { return buildNode(ctx); }
  std::any visitModAttr(ModAttrNode *ctx) override { return buildNode(ctx); }
  std::any visitTopLevelDefinitionAttr(TopLevelDefinitionAttrNode *ctx) override { return buildNode(ctx); }
  std::any visitLambdaAttr(LambdaAttrNode *ctx) override { return buildNode(ctx); }
  std::any visitAttrLst(AttrLstNode *ctx) override { return buildNode(ctx); }
  std::any visitAttr(AttrNode *ctx) override { return buildNode(ctx); }
  std::any visitCaseConstant(CaseConstantNode *ctx) override { return buildNode(ctx); }
  std::any visitReturnStmt(ReturnStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitBreakStmt(BreakStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitContinueStmt(ContinueStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitFallthroughStmt(FallthroughStmtNode *ctx) override { return buildNode(ctx); }
  std::any visitBuiltinCall(BuiltinCallNode *ctx) override { return buildNode(ctx); }
  std::any visitPrintfCall(PrintfCallNode *ctx) override { return buildNode(ctx); }
  std::any visitSizeofCall(SizeofCallNode *ctx) override { return buildNode(ctx); }
  std::any visitAlignofCall(AlignofCallNode *ctx) override { return buildNode(ctx); }
  std::any visitLenCall(LenCallNode *ctx) override { return buildNode(ctx); }
  std::any visitPanicCall(PanicCallNode *ctx) override { return buildNode(ctx); }
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
  std::any visitConstant(ConstantNode *ctx) override { return buildNode(ctx); }
  std::any visitFctCall(FctCallNode *ctx) override { return buildNode(ctx); }
  std::any visitArrayInitialization(ArrayInitializationNode *ctx) override { return buildNode(ctx); }
  std::any visitStructInstantiation(StructInstantiationNode *ctx) override { return buildNode(ctx); }
  std::any visitLambdaFunc(LambdaFuncNode *ctx) override { return buildNode(ctx); }
  std::any visitLambdaProc(LambdaProcNode *ctx) override { return buildNode(ctx); }
  std::any visitLambdaExpr(LambdaExprNode *ctx) override { return buildNode(ctx); }
  std::any visitDataType(DataTypeNode *ctx) override { return buildNode(ctx); }
  std::any visitBaseDataType(BaseDataTypeNode *ctx) override { return buildNode(ctx); }
  std::any visitCustomDataType(CustomDataTypeNode *ctx) override { return buildNode(ctx); }
  std::any visitFunctionDataType(FunctionDataTypeNode *ctx) override { return buildNode(ctx); }

private:
  // Members
  const std::vector<std::string> nodeNames;
  std::stack<std::string> parentNodeIds;

  // Private methods
  template <typename T>
  std::string buildNode(const T *node)
    requires std::is_base_of_v<ASTNode, T>
  {
    std::stringstream result;

    // Prepare strings
    std::string typeName(demangleTypeName(typeid(T).name()));
    std::string codeLoc = node->codeLoc.toString();
    std::string nodeName = typeName.substr(typeName.rfind("::") + 2);
    std::string nodeId = codeLoc + "_" + nodeName;

    // Build result
    result << nodeId << R"( [color="lightgreen",label=")" << nodeName << "\"];\n";

    // Link parent node with the current one
    if (!parentNodeIds.empty())
      result << " " << parentNodeIds.top() << " -> " << nodeId << ";\n";

    parentNodeIds.push(nodeId); // Set parentNodeId for children

    // Visit all the children
    for (ASTNode *child : node->children)
      if (child != nullptr)
        result << " " << std::any_cast<std::string>(visit(child));

    // Remove parent node id from the stack
    parentNodeIds.pop();

    return result.str();
  }

  static std::string demangleTypeName(const char *mangledName);
};

} // namespace spice::compiler
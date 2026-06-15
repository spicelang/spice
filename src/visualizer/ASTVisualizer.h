// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <CompilerPass.h>
#include <ast/ASTNodes.h>
#include <ast/AbstractASTVisitor.h>
#include <util/CommonUtil.h>
#include <util/SaveAndRestore.h>

namespace spice::compiler {

/**
 * Visitor for debug purposes (is only executed in the compiler debug mode and when explicitly enabling it via cli flag)
 *
 * Jobs:
 * - Visualize AST
 */
class ASTVisualizer final : CompilerPass, public AbstractASTVisitor {
public:
  // Constructors
  using CompilerPass::CompilerPass;

  // Visitor methods
  std::any visitEntry(EntryNode *node) override { return buildNode(node); }
  std::any visitMainFctDef(MainFctDefNode *node) override { return buildNode(node); }
  std::any visitFctDef(FctDefNode *node) override { return buildNode(node); }
  std::any visitProcDef(ProcDefNode *node) override { return buildNode(node); }
  std::any visitFctName(FctNameNode *node) override { return buildNode(node); }
  std::any visitStructDef(StructDefNode *node) override { return buildNode(node); }
  std::any visitInterfaceDef(InterfaceDefNode *node) override { return buildNode(node); }
  std::any visitEnumDef(EnumDefNode *node) override { return buildNode(node); }
  std::any visitGenericTypeDef(GenericTypeDefNode *node) override { return buildNode(node); }
  std::any visitAliasDef(AliasDefNode *node) override { return buildNode(node); }
  std::any visitGlobalVarDef(GlobalVarDefNode *node) override { return buildNode(node); }
  std::any visitExtDecl(ExtDeclNode *node) override { return buildNode(node); }
  std::any visitImportDef(ImportDefNode *node) override { return buildNode(node); }
  std::any visitUnsafeBlock(UnsafeBlockNode *node) override { return buildNode(node); }
  std::any visitForLoop(ForLoopNode *node) override { return buildNode(node); }
  std::any visitForeachLoop(ForeachLoopNode *node) override { return buildNode(node); }
  std::any visitWhileLoop(WhileLoopNode *node) override { return buildNode(node); }
  std::any visitDoWhileLoop(DoWhileLoopNode *node) override { return buildNode(node); }
  std::any visitIfStmt(IfStmtNode *node) override { return buildNode(node); }
  std::any visitElseStmt(ElseStmtNode *node) override { return buildNode(node); }
  std::any visitSwitchStmt(SwitchStmtNode *node) override { return buildNode(node); }
  std::any visitCaseBranch(CaseBranchNode *node) override { return buildNode(node); }
  std::any visitDefaultBranch(DefaultBranchNode *node) override { return buildNode(node); }
  std::any visitAssertStmt(AssertStmtNode *node) override { return buildNode(node); }
  std::any visitAnonymousBlockStmt(AnonymousBlockStmtNode *node) override { return buildNode(node); }
  std::any visitStmtLst(StmtLstNode *node) override { return buildNode(node); }
  std::any visitTypeLst(TypeLstNode *node) override { return buildNode(node); }
  std::any visitTypeLstWithEllipsis(TypeLstWithEllipsisNode *node) override { return buildNode(node); }
  std::any visitTypeAltsLst(TypeAltsLstNode *node) override { return buildNode(node); }
  std::any visitParamLst(ParamLstNode *node) override { return buildNode(node); }
  std::any visitArgLst(ArgLstNode *node) override { return buildNode(node); }
  std::any visitEnumItemLst(EnumItemLstNode *node) override { return buildNode(node); }
  std::any visitEnumItem(EnumItemNode *node) override { return buildNode(node); }
  std::any visitField(FieldNode *node) override { return buildNode(node); }
  std::any visitSignature(SignatureNode *node) override { return buildNode(node); }
  std::any visitDeclStmt(DeclStmtNode *node) override { return buildNode(node); }
  std::any visitExprStmt(ExprStmtNode *node) override { return buildNode(node); }
  std::any visitQualifierLst(QualifierLstNode *node) override { return buildNode(node); }
  std::any visitQualifier(QualifierNode *node) override { return buildNode(node); }
  std::any visitModAttr(ModAttrNode *node) override { return buildNode(node); }
  std::any visitTopLevelDefinitionAttr(TopLevelDefAttrNode *node) override { return buildNode(node); }
  std::any visitLambdaAttr(LambdaAttrNode *node) override { return buildNode(node); }
  std::any visitAttrLst(AttrLstNode *node) override { return buildNode(node); }
  std::any visitAttr(AttrNode *node) override { return buildNode(node); }
  std::any visitCaseConstant(CaseConstantNode *node) override { return buildNode(node); }
  std::any visitReturnStmt(ReturnStmtNode *node) override { return buildNode(node); }
  std::any visitBreakStmt(BreakStmtNode *node) override { return buildNode(node); }
  std::any visitContinueStmt(ContinueStmtNode *node) override { return buildNode(node); }
  std::any visitFallthroughStmt(FallthroughStmtNode *node) override { return buildNode(node); }
  std::any visitAssignExpr(AssignExprNode *node) override { return buildNode(node); }
  std::any visitTernaryExpr(TernaryExprNode *node) override { return buildNode(node); }
  std::any visitLogicalOrExpr(LogicalOrExprNode *node) override { return buildNode(node); }
  std::any visitLogicalAndExpr(LogicalAndExprNode *node) override { return buildNode(node); }
  std::any visitBitwiseOrExpr(BitwiseOrExprNode *node) override { return buildNode(node); }
  std::any visitBitwiseXorExpr(BitwiseXorExprNode *node) override { return buildNode(node); }
  std::any visitBitwiseAndExpr(BitwiseAndExprNode *node) override { return buildNode(node); }
  std::any visitEqualityExpr(EqualityExprNode *node) override { return buildNode(node); }
  std::any visitRelationalExpr(RelationalExprNode *node) override { return buildNode(node); }
  std::any visitShiftExpr(ShiftExprNode *node) override { return buildNode(node); }
  std::any visitAdditiveExpr(AdditiveExprNode *node) override { return buildNode(node); }
  std::any visitMultiplicativeExpr(MultiplicativeExprNode *node) override { return buildNode(node); }
  std::any visitCastExpr(CastExprNode *node) override { return buildNode(node); }
  std::any visitPrefixUnaryExpr(PrefixUnaryExprNode *node) override { return buildNode(node); }
  std::any visitPostfixUnaryExpr(PostfixUnaryExprNode *node) override { return buildNode(node); }
  std::any visitAtomicExpr(AtomicExprNode *node) override { return buildNode(node); }
  std::any visitValue(ValueNode *node) override { return buildNode(node); }
  std::any visitConstant(ConstantNode *node) override { return buildNode(node); }
  std::any visitFctCall(FctCallNode *node) override { return buildNode(node); }
  std::any visitArrayInitialization(ArrayInitializationNode *node) override { return buildNode(node); }
  std::any visitStructInstantiation(StructInstantiationNode *node) override { return buildNode(node); }
  std::any visitLambdaFunc(LambdaFuncNode *node) override { return buildNode(node); }
  std::any visitLambdaProc(LambdaProcNode *node) override { return buildNode(node); }
  std::any visitLambdaExpr(LambdaExprNode *node) override { return buildNode(node); }
  std::any visitDataType(DataTypeNode *node) override { return buildNode(node); }
  std::any visitBaseDataType(BaseDataTypeNode *node) override { return buildNode(node); }
  std::any visitCustomDataType(CustomDataTypeNode *node) override { return buildNode(node); }
  std::any visitFunctionDataType(FunctionDataTypeNode *node) override { return buildNode(node); }

private:
  // Members
  std::string parentNodeId;

  // Private methods
  template <typename T>
  std::string buildNode(const T *node)
    requires std::is_base_of_v<ASTNode, T>
  {
    std::stringstream result;

    // Prepare strings
    const std::string typeName(CommonUtil::demangleTypeName(typeid(T).name()));
    const std::string codeLoc = node->codeLoc.toString();
    const std::string nodeName = typeName.substr(typeName.rfind("::") + 2);
    const std::string nodeId = codeLoc + "_" + nodeName;

    // Build result
    result << nodeId << R"( [color="lightgreen",label=")" << nodeName << "\"];\n";

    // Link parent node with the current one
    if (!parentNodeId.empty())
      result << " " << parentNodeId << " -> " << nodeId << ";\n";

    // Set parentNodeId for children
    SaveAndRestore restoreParentNodeId(parentNodeId, nodeId);

    // Visit all the children
    for (ASTNode *child : node->getChildren()) {
      assert(child != nullptr);
      result << " " << std::any_cast<std::string>(visit(child));
    }

    return result.str();
  }
};

} // namespace spice::compiler
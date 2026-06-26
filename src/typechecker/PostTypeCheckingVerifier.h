// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>

namespace spice::compiler {

/**
 * Verifies that the TypeChecker has fully annotated the AST before codegen begins.
 *
 * Runs only in debug builds (guarded by #ifndef NDEBUG in SourceFile.cpp). Asserts
 * that every field the IRGenerator reads — entry pointers, per-manifestation vectors,
 * FctCallData::callee — is non-null and correctly sized. A failure here means a TC
 * regression left a gap in the semantic model, which would otherwise surface as a
 * cryptic null-dereference inside the IRGenerator.
 */
class PostTypeCheckingVerifier final : CompilerPass, public ASTVisitor {
public:
  // Constructors
  PostTypeCheckingVerifier(GlobalResourceManager &resourceManager, SourceFile *sourceFile);

  // Public methods
  void verify(ASTNode *ast);

private:
  // Visitor methods
  std::any visitFctDef(FctDefNode *node) override;
  std::any visitProcDef(ProcDefNode *node) override;
  std::any visitStructDef(StructDefNode *node) override;
  std::any visitInterfaceDef(InterfaceDefNode *node) override;
  std::any visitEnumDef(EnumDefNode *node) override;
  std::any visitEnumItem(EnumItemNode *node) override;
  std::any visitAliasDef(AliasDefNode *node) override;
  std::any visitGlobalVarDef(GlobalVarDefNode *node) override;
  std::any visitSignature(SignatureNode *node) override;
  std::any visitDeclStmt(DeclStmtNode *node) override;
  std::any visitFctCall(FctCallNode *node) override;
  std::any visitAtomicExpr(AtomicExprNode *node) override;
  std::any visitStructInstantiation(StructInstantiationNode *node) override;
  std::any visitLambdaFunc(LambdaFuncNode *node) override;
  std::any visitLambdaProc(LambdaProcNode *node) override;
  std::any visitLambdaExpr(LambdaExprNode *node) override;
};

} // namespace spice::compiler

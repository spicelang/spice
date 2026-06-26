// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "PostTypeCheckingVerifier.h"

#include <algorithm>
#include <cassert>
#include <ranges>

#include <ast/ASTNodes.h>

namespace spice::compiler {

PostTypeCheckingVerifier::PostTypeCheckingVerifier(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
    : CompilerPass(resourceManager, sourceFile) {}

void PostTypeCheckingVerifier::verify(ASTNode *ast) { visit(ast); }

std::any PostTypeCheckingVerifier::visitFctDef(FctDefNode *node) {
  assert(node->entry != nullptr);
  assert(!node->manifestations.empty());
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitProcDef(ProcDefNode *node) {
  assert(node->entry != nullptr);
  assert(!node->manifestations.empty());
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitStructDef(StructDefNode *node) {
  assert(node->entry != nullptr);
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitInterfaceDef(InterfaceDefNode *node) {
  assert(node->entry != nullptr);
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitEnumDef(EnumDefNode *node) {
  assert(node->entry != nullptr);
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitEnumItem(EnumItemNode *node) {
  assert(node->entry != nullptr);
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitAliasDef(AliasDefNode *node) {
  assert(node->entry != nullptr);
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitGlobalVarDef(GlobalVarDefNode *node) {
  assert(node->entry != nullptr);
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitSignature(SignatureNode *node) {
  assert(node->entry != nullptr);
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitDeclStmt(DeclStmtNode *node) {
  if (!node->isForEachItem) {
    // For regular declarations the per-manifestation entries vector must be fully populated
    assert(!node->entries.empty());
    assert(std::ranges::all_of(node->entries, [](const SymbolTableEntry *e) { return e != nullptr; }));
  }
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitFctCall(FctCallNode *node) {
  assert(!node->data.empty());
  assert(std::ranges::all_of(node->data, [](const FctCallNode::FctCallData &d) {
    // Function pointer calls do not have a statically-resolved callee
    return (d.isFctPtrCall() || d.callee != nullptr) && d.calleeParentScope != nullptr;
  }));
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitAtomicExpr(AtomicExprNode *node) {
  // data is only populated for identifier accesses (not for constants / nested exprs)
  if (!node->identifierFragments.empty()) {
    assert(!node->data.empty());
    assert(std::ranges::all_of(node->data, [](const AtomicExprNode::VarAccessData &d) { return d.entry != nullptr; }));
  }
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitStructInstantiation(StructInstantiationNode *node) {
  assert(!node->instantiatedStructs.empty());
  assert(std::ranges::all_of(node->instantiatedStructs, [](const Struct *s) { return s != nullptr; }));
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitLambdaFunc(LambdaFuncNode *node) {
  assert(!node->manifestations.empty());
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitLambdaProc(LambdaProcNode *node) {
  assert(!node->manifestations.empty());
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitLambdaExpr(LambdaExprNode *node) {
  assert(!node->manifestations.empty());
  return visitChildren(node);
}

} // namespace spice::compiler

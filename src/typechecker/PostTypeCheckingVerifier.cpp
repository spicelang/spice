// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "PostTypeCheckingVerifier.h"

#include <algorithm>
#include <cassert>
#include <ranges>

#include <ast/ASTNodes.h>
#include <typechecker/Builtins.h>

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
  // EnumItemNode::entry is intentionally not stored on the node; the entry lives in the enum scope.
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
  // entries is resized to manifestationCount; unsubstantiated generic slots remain null.
  // We can only assert the vector was populated at all.
  if (!node->isForEachItem)
    assert(!node->entries.empty());
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitFctCall(FctCallNode *node) {
  // Builtin calls (printf, len, panic, etc.) return early in the TypeChecker before
  // populating callee/calleeParentScope — skip them, matching the IRGenerator's behavior.
  for (const auto &[builtinFctName, _] : BUILTIN_FUNCTIONS)
    if (node->fqFunctionName == builtinFctName)
      return visitChildren(node);

  assert(!node->data.empty());
  // calleeParentScope is set for every visited non-fct-ptr non-builtin slot; use it as the
  // "was this slot actually resolved?" indicator. Unsubstantiated/uncompiled slots have it null.
  assert(std::ranges::all_of(node->data, [](const FctCallNode::FctCallData &d) {
    return d.calleeParentScope == nullptr || d.callee != nullptr;
  }));
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitAtomicExpr(AtomicExprNode *node) {
  // data is only populated for identifier accesses (not for constants / nested exprs).
  // Within identifier accesses, only substantiated manifestation slots have accessScope set;
  // unsubstantiated generic slots are left at the default-initialized nullptr.
  if (!node->fqIdentifier.empty()) {
    assert(!node->data.empty());
    assert(std::ranges::all_of(node->data, [](const AtomicExprNode::VarAccessData &d) {
      return d.accessScope == nullptr || d.entry != nullptr;
    }));
  }
  return visitChildren(node);
}

std::any PostTypeCheckingVerifier::visitStructInstantiation(StructInstantiationNode *node) {
  // instantiatedStructs is resized to manifestationCount; unsubstantiated generic slots remain null.
  // We can only safely assert the vector was populated — element-level null checks would false-positive
  // on unsubstantiated generic manifestation slots.
  assert(!node->instantiatedStructs.empty());
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

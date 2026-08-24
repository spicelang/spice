// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "LintPass.h"

#include <ast/ASTNodes.h>
#include <linter/rules/NamingConventionRule.h>

namespace spice::compiler {

LintPass::LintPass(GlobalResourceManager &resourceManager, SourceFile *sourceFile) : CompilerPass(resourceManager, sourceFile) {
  // Register all built-in lint rules here. Adding a rule never requires touching the traversal below.
  rules.push_back(std::make_unique<NamingConventionRule>());
}

std::vector<LintFinding> LintPass::lint(ASTNode *ast) {
  visit(ast);
  return std::move(findings);
}

std::any LintPass::visitFctDef(FctDefNode *node) {
  for (const std::unique_ptr<LintRule> &rule : rules)
    rule->checkFctDef(node, findings);
  return visitChildren(node);
}

std::any LintPass::visitProcDef(ProcDefNode *node) {
  for (const std::unique_ptr<LintRule> &rule : rules)
    rule->checkProcDef(node, findings);
  return visitChildren(node);
}

std::any LintPass::visitStructDef(StructDefNode *node) {
  for (const std::unique_ptr<LintRule> &rule : rules)
    rule->checkStructDef(node, findings);
  return visitChildren(node);
}

std::any LintPass::visitInterfaceDef(InterfaceDefNode *node) {
  for (const std::unique_ptr<LintRule> &rule : rules)
    rule->checkInterfaceDef(node, findings);
  return visitChildren(node);
}

std::any LintPass::visitGlobalVarDef(GlobalVarDefNode *node) {
  for (const std::unique_ptr<LintRule> &rule : rules)
    rule->checkGlobalVarDef(node, findings);
  return visitChildren(node);
}

} // namespace spice::compiler

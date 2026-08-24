// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <memory>
#include <vector>

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>
#include <linter/LintFinding.h>
#include <linter/LintRule.h>

namespace spice::compiler {

/**
 * Runs all registered LintRules over a fully type-checked AST in a single read-only traversal.
 *
 * Modeled on PostTypeCheckingVerifier: rules are registered once in the constructor and each
 * overridden visit method fans out to every rule interested in that node kind, so a new rule
 * never needs its own AST walk.
 */
class LintPass final : CompilerPass, public ASTVisitor {
public:
  // Constructors
  LintPass(GlobalResourceManager &resourceManager, SourceFile *sourceFile);

  // Public methods
  std::vector<LintFinding> lint(ASTNode *ast);

private:
  // Visitor methods
  std::any visitFctDef(FctDefNode *node) override;
  std::any visitProcDef(ProcDefNode *node) override;
  std::any visitStructDef(StructDefNode *node) override;
  std::any visitInterfaceDef(InterfaceDefNode *node) override;

  // Members
  std::vector<std::unique_ptr<LintRule>> rules;
  std::vector<LintFinding> findings;
};

} // namespace spice::compiler

// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "CompilerPass.h"

#include <SourceFile.h>
#include <global/GlobalResourceManager.h>
#include <symboltablebuilder/Scope.h>

namespace spice::compiler {

CompilerPass::CompilerPass(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
    : resourceManager(resourceManager), cliOptions(resourceManager.cliOptions), sourceFile(sourceFile),
      rootScope(sourceFile != nullptr ? sourceFile->globalScope.get() : nullptr), currentScope(rootScope) {}

/**
 * Change to the passed scope.
 * For nested scopes in generic functions/procedures it is important to have the right parent for symbol lookups
 * Therefore, changeToScope sets the children's parent to the old scope to always have the right parent
 *
 * @param scope Scope to change to
 * @param scopeType Expected type of the given scope
 */
void CompilerPass::changeToScope(Scope *scope, ScopeType scopeType) {
  assert(scope != nullptr);
  assert(scope->type == scopeType);
  assert(!scope->isGenericScope);
  // Adjust members of the new scope
  scope->parent = currentScope;
  scope->symbolTable.parent = &currentScope->symbolTable;
  // Set the scope
  currentScope = scope;
}

/**
 * Change to the scope with the given name.
 *
 * @param scopeName Name of the scope to change to
 * @param scopeType Expected type of the given scope
 */
void CompilerPass::changeToScope(const std::string &scopeName, ScopeType scopeType) {
  assert(!scopeName.empty());
  changeToScope(currentScope->getChildScope(scopeName), scopeType);
}

/**
 * Change to the parent scope of the current.
 *
 * @param oldScopeType Expected type of the scope to leave
 */
void CompilerPass::changeToParentScope(ScopeType oldScopeType) {
  assert(currentScope->type == oldScopeType);
  assert(currentScope->parent != nullptr);
  currentScope = currentScope->parent;
}

} // namespace spice::compiler
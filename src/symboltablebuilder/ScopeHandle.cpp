// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "ScopeHandle.h"

#include "CompilerPass.h"
#include <ast/ASTNodes.h>
#include <irgenerator/IRGenerator.h>
#include <symboltablebuilder/Scope.h>

namespace spice::compiler {

ScopeHandle::ScopeHandle(CompilerPass *pass, Scope *childScope, const ScopeType &scopeType)
    : DeferredLogic([=]() { pass->changeToParentScope(scopeType); }) {
  pass->changeToScope(childScope, scopeType);
}

ScopeHandle::ScopeHandle(CompilerPass *pass, const std::string &childScopeId, const ScopeType &scopeType)
    : ScopeHandle(pass, pass->currentScope->getChildScope(childScopeId), scopeType) {}

ScopeHandle::ScopeHandle(IRGenerator *generator, Scope *childScope, const ScopeType &scopeType, const ASTNode *node)
    : DeferredLogic([=]() {
        generator->changeToParentScope(scopeType);
        generator->diGenerator.popLexicalBlock();
      }) {
  assert(scopeType != ScopeType::FUNC_PROC_BODY); // Functions/procedures manage scopes manually
  generator->changeToScope(childScope, scopeType);
  generator->diGenerator.pushLexicalBlock(node);
}

ScopeHandle::ScopeHandle(IRGenerator *generator, const std::string &childScopeId, const ScopeType &scopeType, const ASTNode *node)
    : ScopeHandle(generator, generator->currentScope->getChildScope(childScopeId), scopeType, node) {}

} // namespace spice::compiler
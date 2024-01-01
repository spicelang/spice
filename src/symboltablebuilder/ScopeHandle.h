// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <string>

#include <util/DeferredLogic.h>

namespace spice::compiler {

// Forward declarations
class CompilerPass;
class IRGenerator;
class Scope;
enum class ScopeType : uint8_t;
class ASTNode;

/**
 * This class is used to change the current scope to a child scope and change it back to the parent scope when the handle object
 * is destroyed. This can be used in visit functions to automatically leave the scope when the function returns.
 */
class ScopeHandle : private DeferredLogic {
public:
  // Constructors
  ScopeHandle(CompilerPass *pass, Scope *childScope, const ScopeType &scopeType);
  ScopeHandle(CompilerPass *pass, const std::string &childScopeId, const ScopeType &scopeType);
  ScopeHandle(IRGenerator *pass, Scope *childScope, const ScopeType &scopeType, const ASTNode *node);
  ScopeHandle(IRGenerator *generator, const std::string &childScopeId, const ScopeType &scopeType, const ASTNode *node);

  // Public methods
  void leaveScopeEarly() { execute(); }
};

} // namespace spice::compiler
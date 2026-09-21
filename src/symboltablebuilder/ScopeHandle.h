// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

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
class ExprNode;

/**
 * This class is used to change the current scope to a child scope and change it back to the parent scope when the handle object
 * is destroyed. This can be used in visit functions to automatically leave the scope when the function returns.
 */
class ScopeHandle : DeferredLogic {
public:
  // Constructors
  ScopeHandle(CompilerPass *pass, Scope *childScope, const ScopeType &scopeType);
  ScopeHandle(CompilerPass *pass, const std::string &childScopeId, const ScopeType &scopeType);
  ScopeHandle(IRGenerator *generator, Scope *childScope, const ScopeType &scopeType, const ASTNode *node);
  ScopeHandle(IRGenerator *generator, const std::string &childScopeId, const ScopeType &scopeType, const ASTNode *node);

  // Public methods
  void leaveScopeEarly() { execute(); }
};

/**
 * This class enters the expression scope of an expression, if it has one, and leaves it again when the handle object is
 * destroyed.
 *
 * An expression scope holds the temporaries of an expression that is evaluated as a unit: a condition or an operand that is only
 * evaluated conditionally. This allows destructing the temporaries right after the expression was evaluated, on every path.
 * If the expression has no scope, the handle does nothing and the temporaries stay in the current scope.
 */
class ExprScopeHandle : DeferredLogic {
public:
  // Constructors
  ExprScopeHandle(CompilerPass *pass, const ExprNode *expr);

  // Public methods
  [[nodiscard]] Scope *getExprScope() const { return exprScope; }

private:
  // Private constructors
  ExprScopeHandle(CompilerPass *pass, Scope *exprScope);

  // Private members
  Scope *exprScope;
};

} // namespace spice::compiler
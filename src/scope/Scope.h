// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <symbol/SymbolTable.h>

enum ScopeType {
  SCOPE_GLOBAL,
  SCOPE_FUNC_PROC_BODY,
  SCOPE_STRUCT,
  SCOPE_INTERFACE,
  SCOPE_ENUM,
  SCOPE_IF_BODY,
  SCOPE_WHILE_BODY,
  SCOPE_FOR_BODY,
  SCOPE_FOREACH_BODY,
  SCOPE_THREAD_BODY,
  SCOPE_UNSAFE_BODY
};

/**
 * Represents an enclosed group of instructions, which are semantically separated from other scopes.
 * In the source code, scopes usually are written as curly braces.
 *
 * Following language structures use scopes:
 * - functions/procedures
 * - thread expressions
 * - unsafe block statements
 * - for loops
 * - foreach loops
 * - while loops
 * - if/elseif/else statements
 * - anonymous scopes
 */
class Scope {
public:
  // Constructors
  Scope(Scope *parent, const ScopeType &scopeType) : parent(parent), type(scopeType) {}
  ~Scope();

  // Public methods
  Scope *createChildScope(const std::string &scopeName, const ScopeType &scopeType);
  [[nodiscard]] Scope *getGlobalScope();
  [[nodiscard]] Scope *getFunctionScope();
  [[nodiscard]] Scope *getChildScope(const std::string &scopeName) const;
  [[nodiscard]] std::vector<SymbolTableEntry *> getVarsGoingOutOfScope();
  [[nodiscard]] std::vector<CompilerWarning> collectWarnings() const;
  [[nodiscard]] size_t getFieldCount() const;

  // Public members
  Scope *parent;
  std::unordered_map<std::string, Scope *> children;
  const ScopeType type;
  SymbolTable symbolTable = SymbolTable(parent->symbolTable);

private:
  // Private methods
  Scope *searchForScope(const ScopeType &scopeType);
};

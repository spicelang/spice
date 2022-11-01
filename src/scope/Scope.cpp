// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "Scope.h"

#include <analyzer/AnalyzerVisitor.h>

Scope::~Scope() {
  for (const auto &child : children)
    delete child.second;
}

/**
 * Create a child scope and return it
 *
 * @param scopeName Name of the child scope
 * @param scopeType Type of the child scope
 * @return Child scope (heap allocated)
 */
Scope *Scope::createChildScope(const std::string &scopeName, const ScopeType &scopeType) {
  children.insert({scopeName, new Scope(this, scopeType)});
  return children.at(scopeName);
}

/**
 * Retrieve the global scope of the current source file
 *
 * @return Global scope
 */
Scope *Scope::getGlobalScope() { return searchForScope(SCOPE_GLOBAL); }

/**
 * Retrieve the scope of the function or procedure, where this scope lives
 *
 * @return Function or procedure scope
 */
Scope *Scope::getFunctionScope() { return searchForScope(SCOPE_FUNC_PROC_BODY); }

/**
 * Get a child scope of the current scope by its name
 *
 * @param scopeName Child scope name
 * @return Child scope
 */
Scope *Scope::getChildScope(const std::string &scopeName) const {
  if (!children.empty() && children.contains(scopeName))
    return children.at(scopeName);
  return nullptr;
}

/**
 * Retrieve all variables in the current scope, that have reached the end of their lifetime at the end of this scope
 *
 * @return Collection of EOL variables
 */
std::vector<SymbolTableEntry *> Scope::getVarsGoingOutOfScope() { // NOLINT(misc-no-recursion)
  assert(parent != nullptr); // Should not be called in root scope
  std::vector<SymbolTableEntry *> varsGoingOutOfScope;

  // Collect all variables in this scope
  for (const auto &[name, entry] : symbolTable.symbols) {
    // Skip 'this' variables
    if (name == THIS_VARIABLE_NAME)
      continue;
    // For dtor calls, only anonymous structs are relevant
    if (entry.type.isOneOf({TY_STRUCT, TY_STROBJ}) && !entry.isDead() && entry.name.starts_with("anon."))
      varsGoingOutOfScope.push_back(&symbolTable.symbols.at(name));
  }

  // Collect all variables in the child scopes
  for (const auto &[_, child] : children) {
    const ScopeType scopeType = child->type;
    // Exclude enum, global, struct and thread body (is a LLVM function) scopes
    if (scopeType != SCOPE_ENUM && scopeType != SCOPE_GLOBAL && scopeType != SCOPE_STRUCT && scopeType != SCOPE_THREAD_BODY) {
      std::vector childVars = child->getVarsGoingOutOfScope();
      varsGoingOutOfScope.insert(varsGoingOutOfScope.end(), childVars.begin(), childVars.end());
    }
  }

  return varsGoingOutOfScope;
}

/**
 * Get the number of fields if this is a struct scope
 *
 * @return Number of fields
 */
size_t Scope::getFieldCount() const {
  assert(type == SCOPE_STRUCT);
  size_t fieldCount = 0;
  for (const auto &symbol : symbolTable.symbols) {
    const SymbolType &symbolType = symbol.second.type;
    if (symbolType.isPrimitive() || symbolType.isOneOf({TY_STRUCT, TY_STROBJ}))
      fieldCount++;
  }
  return fieldCount;
}

/**
 * Collect all warnings, produces within this scope
 *
 * @return Collection of warnings
 */
std::vector<CompilerWarning> Scope::collectWarnings() const { // NOLINT(misc-no-recursion)
  std::vector<CompilerWarning> warnings;
  // Visit own symbols
  for (const auto &[key, entry] : symbolTable.symbols) {
    // Do not produce a warning if the symbol is used or has a special name
    if (entry.isUsed || entry.name == UNUSED_VARIABLE_NAME)
      continue;
  }

  return warnings;
}


/**
 * Searches in the parent scopes for a scope with a certain type
 *
 * @param scopeType Type to search for
 * @return Next higher scope that has the expected type
 */
Scope *Scope::searchForScope(const ScopeType &scopeType) {
  Scope *searchResult = this;
  while (searchResult && searchResult->type != scopeType)
    searchResult = searchResult->parent;
  return searchResult;
}

/**
 * Check if unsafe operations are allowed in this scope
 *
 * @return Allowed or not
 */
bool Scope::allowsUnsafeOperations() const { // NOLINT(misc-no-recursion)
  if (type == SCOPE_UNSAFE_BODY)
    return true;
  return parent != nullptr && parent->allowsUnsafeOperations();
}

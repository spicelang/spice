// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ScopePath.h"

/**
 * Add a new fragment and the associated symbolTable to the scope path
 *
 * @param fragmentName Fragment name
 * @param symbolTable Associated symbol table
 */
void ScopePath::pushFragment(const std::string &fragmentName, SymbolTable *symbolTable) {
  fragments.emplace_back(fragmentName, symbolTable);
}

/**
 * Clear the scope path
 */
void ScopePath::clear() { fragments.clear(); }

/**
 * Return the current access scope of the scope path
 *
 * @return Pointer to the symbol table, representing the current access scope
 */
SymbolTable *ScopePath::getCurrentScope() const {
  if (fragments.empty())
    return nullptr;
  return fragments.back().second;
}

/**
 * Retrieve the fully qualified prefix of a scope, e.g.: "a.b." for "a.b.C"
 *
 * @return Scope prefix
 */
std::string ScopePath::getScopePrefix(bool includeLast) const {
  // Early return when there is no path
  if (fragments.empty())
    return "";

  // Join prefix together
  std::string fqn;
  int offset = includeLast ? 0 : 1;
  for (int i = 0; i < fragments.size() - offset; i++)
    fqn += fragments[i].first + ".";
  return fqn;
}
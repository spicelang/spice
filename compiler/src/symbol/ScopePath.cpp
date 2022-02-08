// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ScopePath.h"

/**
 * Add a new fragment and the associated symbolTable to the scope path
 *
 * @param fragmentName Fragment name
 * @param symbolTable Associated symbol table
 */
void ScopePath::pushFragment(const std::string& fragmentName, SymbolTable* symbolTable) {
    fragments.emplace_back(fragmentName, symbolTable);
}

/**
 * Clear the scope path
 */
void ScopePath::clear() {
    fragments.clear();
}

/**
 * Return the current access scope of the scope path
 *
 * @return Pointer to the symbol table, representing the current access scope
 */
SymbolTable* ScopePath::getCurrentScope() {
    if (fragments.empty()) return nullptr;
    return std::get<1>(fragments.back());
}
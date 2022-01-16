// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ScopePath.h"

/**
 * Adds a new fragment and symbolTable to the scope path
 *
 * @param fragment
 * @param symbolTable
 */
void ScopePath::pushFragment(const std::string& fragmentName, SymbolTable* symbolTable) {
    fragments.emplace_back(fragmentName, symbolTable);
}

/**
 * Clears the scope path
 */
void ScopePath::clear() {
    fragments.clear();
}

/**
 * Returns the current access scope of the scope path
 *
 * @return Pointer to the symbol table, representing the current access scope
 */
SymbolTable* ScopePath::getCurrentScope() {
    if (fragments.empty()) return nullptr;
    return std::get<1>(fragments.back());
}

/**
 * Returns the scope path in a string representation
 *
 * @return Symbol path string
 */
std::string ScopePath::toString() {
    std::string stringRepresentation;
    for (auto& fragment : fragments) {
        std::string fragmentName = std::get<0>(fragment);
        stringRepresentation += stringRepresentation.empty() ? fragmentName : "." + fragmentName;
    }
    return stringRepresentation;
}

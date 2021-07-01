// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SymbolTable.h"

void SymbolTable::insertSymbol(const std::string& identifier, const std::string& scope, SymbolType type) {
    symbols.insert({
        getKeyForCombination(identifier, scope),
        SymbolTableEntry(identifier, scope, type)
    });
}

void SymbolTable::deleteSymbol(const std::string& identifier, const std::string& scope) {
    symbols.erase(getKeyForCombination(identifier, scope));
}

bool SymbolTable::isSymbolPresent(const std::string& identifier, const std::string& scope) {
    return symbols.find(getKeyForCombination(identifier, scope)) != symbols.end();
}

std::string SymbolTable::getKeyForCombination(const std::string& identifier, const std::string& scope) {
    return scope + ":" + identifier;
}

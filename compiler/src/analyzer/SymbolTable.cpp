// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SymbolTable.h"

void SymbolTable::insert(const std::string& name, SymbolType type, SymbolState state, bool isConstant, bool isParameter) {
    symbols.insert({ name, SymbolTableEntry(name, type, state, isConstant) });
    if (isParameter) paramNames.push_back(name);
}

SymbolTableEntry* SymbolTable::lookup(const std::string& name) {
    // If not available in the current scope, search in the parent scope
    if (symbols.find(name) == symbols.end()) {
        if (parent == nullptr) return nullptr;
        return parent->lookup(name);
    }
    // Otherwise, return the entry
    return &symbols.at(name);
}

void SymbolTable::update(const std::string& name, SymbolState newState) {
    // If not available in the current scope, search in the parent scope
    if (symbols.find(name) == symbols.end()) {
        if (parent == nullptr) throw std::runtime_error("Updating a non-existent symbol: " + name);
        parent->update(name, newState);
    }
    // Otherwise, update the entry
    symbols.at(name).updateState(newState);
}

void SymbolTable::update(const std::string& name, SymbolType newType) {
    // If not available in the current scope, search in the parent scope
    if (symbols.find(name) == symbols.end()) {
        if (parent == nullptr) throw std::runtime_error("Updating a non-existent symbol: " + name);
        parent->update(name, newType);
    }
    // Otherwise, update the entry
    symbols.at(name).updateType(newType);
}

SymbolTable* SymbolTable::createChildBlock(const std::string& blockName) {
    children.insert({ blockName, SymbolTable(this) });
    return &children.at(blockName);
}

SymbolTable* SymbolTable::getParent() {
    return parent;
}

SymbolTable *SymbolTable::getChild(std::string& scopeId) {
    return &children.at(scopeId);
}

std::vector<std::string> SymbolTable::getParamNames() {
    return paramNames;
}

std::string SymbolTable::toString() {
    std::string symbolsString, childrenString;
    for (auto& symbol : symbols) {
        symbolsString.append("(" + symbol.second.toString() + ")\n");
    }
    for (auto& child : children) {
        childrenString.append(child.second.toString() + "\n");
    }
    return "SymbolTable(\n" + symbolsString + ") {\n" + childrenString + "}";
}
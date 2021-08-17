// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SymbolTable.h"

void SymbolTable::insert(const std::string& name, SymbolType type, SymbolState state, bool isConstant, bool isParameter) {
    symbols.insert({name, SymbolTableEntry(name, type, state, isConstant)});
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

SymbolTableEntry* SymbolTable::lookup(const std::vector<std::string>& nameSpace) {
    return &lookupTable(nameSpace)->symbols.at(nameSpace[nameSpace.size() - 1]);
}

SymbolTable* SymbolTable::lookupTable(const std::vector<std::string>& nameSpace) {
    // Check if scope contains this namespace
    SymbolTable* currentTable = this;
    for (int i = 0; i < nameSpace.size(); i++) {
        if (i == nameSpace.size() - 1) {
            if (currentTable->symbols.find(nameSpace[i]) == currentTable->symbols.end()) break;
            return currentTable;
        } else {
            if (currentTable->children.find(nameSpace[i]) == currentTable->children.end()) break;
            currentTable = &children.at(nameSpace[i]);
        }
    }
    // Current scope does not contain the namespace => go up one table
    if (parent == nullptr) return nullptr;
    return parent->lookupTable(nameSpace);
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
    children.insert({blockName, SymbolTable(this)});
    return &children.at(blockName);
}

void SymbolTable::mountChildBlock(const std::string& blockName, SymbolTable* childBlock) {
    children.insert({blockName, *childBlock});
}

void SymbolTable::renameChildBlock(const std::string& oldName, const std::string& newName) {
    auto nodeHandler = children.extract(oldName);
    nodeHandler.key() = newName;
    children.insert(std::move(nodeHandler));
}

SymbolTable* SymbolTable::getParent() {
    return parent;
}

SymbolTable* SymbolTable::getChild(const std::string& scopeId) {
    if (children.empty()) return nullptr;
    return &children.at(scopeId);
}

void SymbolTable::insertFunctionDeclaration(const std::string& signature, const std::vector<SymbolType>& types) {
    functionDeclarations.insert({signature, types});
}

std::vector<SymbolType> SymbolTable::getFunctionDeclaration(const std::string& signature) {
    if (functionDeclarations.find(signature) == functionDeclarations.end()) return {};
    return functionDeclarations.at(signature);
}

void SymbolTable::insertProcedureDeclaration(const std::string& signature, const std::vector<SymbolType>& types) {
    procedureDeclarations.insert({signature, types});
}

std::vector<SymbolType> SymbolTable::getProcedureDeclaration(const std::string& signature) {
    if (procedureDeclarations.find(signature) == procedureDeclarations.end()) return {};
    return procedureDeclarations.at(signature);
}

void SymbolTable::pushSignature(const FunctionSignature& signature) {
    functionSignatures.push(signature);
}

FunctionSignature SymbolTable::popSignature() {
    auto signature = functionSignatures.front();
    functionSignatures.pop();
    return signature;
}

llvm::BasicBlock* SymbolTable::getContinueBlock() const {
    return continueBlock;
}

void SymbolTable::setContinueBlock(llvm::BasicBlock* block) {
    continueBlock = block;
}

llvm::BasicBlock* SymbolTable::getBreakBlock() const {
    return breakBlock;
}

void SymbolTable::setBreakBlock(llvm::BasicBlock* block) {
    breakBlock = block;
}

std::string SymbolTable::toString() {
    std::string symbolsString, childrenString;
    for (auto& symbol : symbols)
        symbolsString.append("(" + symbol.second.toString() + ")\n");
    for (auto& child : children)
        childrenString.append(child.first + ": " + child.second.toString() + "\n");
    return "SymbolTable(\n" + symbolsString + ") {\n" + childrenString + "}";
}

// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>
#include <map>
#include <vector>
#include "SymbolTableEntry.h"

class SymbolTable {
public:
    // Constructors
    explicit SymbolTable(SymbolTable* parent): parent(parent) {};
    // Public methods
    void insert(const std::string&, SymbolType, SymbolState, bool, bool);
    SymbolTableEntry* lookup(const std::string&);
    void update(const std::string&, SymbolState);
    void update(const std::string&, SymbolType);
    SymbolTable* createChildBlock(const std::string&);
    void renameChildBlock(const std::string&, const std::string&);
    SymbolTable* getParent();
    SymbolTable* getChild(std::string&);
    std::vector<std::string> getParamNames();
    std::string toString();
private:
    // Members
    std::map<std::string, SymbolTableEntry> symbols;
    std::vector<std::string> paramNames;
    SymbolTable* parent;
    std::map<std::string, SymbolTable> children;
};
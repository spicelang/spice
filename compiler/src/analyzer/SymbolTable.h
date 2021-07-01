// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>
#include <unordered_map>

enum SymbolType {
    INT,
    DOUBLE,
    STRING,
    BOOL,
    FUNCTION,
    PROCEDURE
};

class SymbolTableEntry {
public:
    SymbolTableEntry(std::string identifier, std::string scope, SymbolType type):
        identifier(std::move(identifier)), scope(std::move(scope)), type(type) {};
private:
    std::string identifier, scope;
    SymbolType type;
};

class SymbolTable {
public:
    // Public methods
    void insertSymbol(const std::string&, const std::string&, SymbolType);
    void deleteSymbol(const std::string&, const std::string&);
    bool isSymbolPresent(const std::string&, const std::string&);
private:
    // Private methods
    static std::string getKeyForCombination(const std::string&, const std::string&);

    // Members
    std::unordered_map<std::string, SymbolTableEntry> symbols;
};
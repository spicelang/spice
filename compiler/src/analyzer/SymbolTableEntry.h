// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <stdexcept>

#include <exception/SemanticError.h>

enum SymbolType {
    TYPE_DOUBLE,     TYPE_INT,     TYPE_STRING,     TYPE_BOOL,
    TYPE_DOUBLE_PTR, TYPE_INT_PTR, TYPE_STRING_PTR, TYPE_BOOL_PTR,
    TYPE_DYN,
    TYPE_FUNCTION, TYPE_PROCEDURE, TYPE_IMPORT
};

enum SymbolState {
    DECLARED,
    INITIALIZED
};

/**
 * Entry of a symbol table, representing an individual symbol with all its properties
 */
class SymbolTableEntry {
public:
    // Constructors
    SymbolTableEntry(std::string name, SymbolType type, SymbolState state, bool isConstant) :
            name(std::move(name)), type(type), state(state), isConstant(isConstant) {};

    // Public methods
    SymbolState getState();

    SymbolType getType();

    void updateState(SymbolState);

    void updateType(SymbolType);

    std::string toString();

private:
    // Members
    std::string name;
    SymbolType type;
    SymbolState state;
    bool isConstant;
};
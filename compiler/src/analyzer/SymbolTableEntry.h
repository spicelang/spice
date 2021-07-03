// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <stdexcept>

enum SymbolType {
    TYPE_INT, TYPE_DOUBLE, TYPE_STRING, TYPE_BOOL,
    TYPE_DYN, TYPE_FUNCTION, TYPE_PROCEDURE,
    TYPE_WHILE, TYPE_FOR, TYPE_IF
};

enum SymbolState {
    DECLARED,
    INITIALIZED
};

class SymbolTableEntry {
public:
    // Constructors
    SymbolTableEntry(std::string name, SymbolType type, SymbolState state):
            name(std::move(name)), type(type), state(state) {};

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
};
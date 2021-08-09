// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SymbolTableEntry.h"

SymbolState SymbolTableEntry::getState() {
    return state;
}

SymbolType SymbolTableEntry::getType() {
    return type;
}

void SymbolTableEntry::updateState(SymbolState newState) {
    if (state == INITIALIZED && isConstant)
        throw SemanticError(REASSIGN_CONST_VARIABLE, "Not re-assignable variable '" + name + "'");
    if (newState == INITIALIZED && type == TYPE_DYN)
        throw std::runtime_error("Internal compiler error: could not determine type of variable '" + name + "'");
    state = newState;
}

void SymbolTableEntry::updateType(SymbolType newType) {
    if (type != TYPE_DYN) throw std::runtime_error("Internal compiler error: Cannot change type of non-dyn");
    type = newType;
}

std::string SymbolTableEntry::toString() {
    return "Name: " + name + ", Type: " + std::to_string(type) + ", State: " + std::to_string(state) + ", Const: " +
        std::to_string(isConstant);
}

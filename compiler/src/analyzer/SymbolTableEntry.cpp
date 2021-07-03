// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SymbolTableEntry.h"

SymbolState SymbolTableEntry::getState() {
    return state;
}

SymbolType SymbolTableEntry::getType() {
    return type;
}

void SymbolTableEntry::updateState(SymbolState newState) {
    state = newState;
}

void SymbolTableEntry::updateType(SymbolType newType) {
    if (type != TYPE_DYN) throw std::runtime_error("Compiler error: Cannot change type of non-dyn");
    type = newType;
}

std::string SymbolTableEntry::toString() {
    return "Name: " + name + ", Type: " + std::to_string(type) + ", State: " + std::to_string(state);
}

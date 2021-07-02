// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SymbolTableEntry.h"

SymbolState SymbolTableEntry::getState() {
    return state;
}

void SymbolTableEntry::updateState(SymbolState newState) {
    state = newState;
}

std::string SymbolTableEntry::toString() {
    return "Name: " + name + ", Type: " + std::to_string(type) + ", State: " + std::to_string(state);
}

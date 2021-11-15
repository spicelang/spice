// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SymbolTableEntry.h"

/**
 * Retrieve the state of the current symbol
 *
 * @return State of the current symbol
 */
SymbolState SymbolTableEntry::getState() {
    return state;
}

/**
 * Retrieve the type of the current symbol
 *
 * @return Type of the current symbol
 */
SymbolType SymbolTableEntry::getType() {
    return type;
}

/**
 * Update the state of the current symbol
 *
 * @throws SemanticError When trying to re-assign a constant variable
 * @throws runtime_error When the state of the symbol is set to initialized before a concrete type was set
 * @param newState New state of the current symbol
 */
void SymbolTableEntry::updateState(SymbolState newState) {
    if (state == INITIALIZED && isConstant)
        throw SemanticError(REASSIGN_CONST_VARIABLE, "Not re-assignable variable '" + name + "'");
    if (newState == INITIALIZED && type == TYPE_DYN)
        throw std::runtime_error("Internal compiler error: could not determine type of variable '" + name + "'");
    state = newState;
}

/**
 * Update the type of a symbol. This is used for substantiate types in the process of type inference
 *
 * @param newType New type of the current symbol
 */
void SymbolTableEntry::updateType(SymbolType newType) {
    if (type != TYPE_DYN) throw std::runtime_error("Internal compiler error: Cannot change type of non-dyn");
    type = newType;
}

/**
 * Stringify the current symbol to a human-readable form. Used to dump whole symbol tables with their contents.
 *
 * @return Symbol table entry in form of a string
 */
std::string SymbolTableEntry::toString() {
    return "Name: " + name + ", Type: " + std::to_string(type) + ", State: " + std::to_string(state) + ", Const: " +
           std::to_string(isConstant);
}

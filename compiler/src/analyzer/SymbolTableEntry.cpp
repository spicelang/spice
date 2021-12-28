// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SymbolTableEntry.h"

/**
 * Retrieve the name of the current symbol
 *
 * @return Name of the curren symbol
 */
std::string SymbolTableEntry::getName() {
    return name;
}

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
 * Retrieve the token where the symbol was defined
 *
 * @return Definition token
 */
const antlr4::Token& SymbolTableEntry::getDefinitionToken() {
    return definitionToken;
}

/**
 * Retrieve the llvm type of the current symbol
 *
 * @return LLVM type of the current symbol
 */
llvm::Type* SymbolTableEntry::getLLVMType() {
    return llvmType;
}

/**
 * Retrieve the address of the assigned value
 *
 * @return
 */
llvm::Value* SymbolTableEntry::getAddress() {
    return memAddress;
}

/**
 * Retrieve the order index of the symbol table entry
 *
 * @return Order index
 */
unsigned int SymbolTableEntry::getOrderIndex() const {
    return orderIndex;
}

/**
 * Returns if the symbol is in a local scope or in the global scope
 *
 * @return isLocal
 */
bool SymbolTableEntry::isLocal() const {
    return !isGlobal;
}

/**
 * Returns if the symbol is used somewhere
 *
 * @return isUsed
 */
bool  SymbolTableEntry::isUsed() const {
    return used;
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
        throw SemanticError(definitionToken, REASSIGN_CONST_VARIABLE,
                            "Not re-assignable variable '" + name + "'");
    if (newState == INITIALIZED && type == SymbolType(TY_DYN))
        throw std::runtime_error("Internal compiler error: could not determine type of variable '" + name + "'");
    state = newState;
}

/**
 * Update the type of a symbol. This is used for substantiate types in the process of type inference
 *
 * @param newType New type of the current symbol
 */
void SymbolTableEntry::updateType(SymbolType newType) {
    if (type != SymbolType(TY_DYN)) throw std::runtime_error("Internal compiler error: Cannot change type of non-dyn");
    type = std::move(newType);
}

/**
 * Update the LLVM type of a symbol
 *
 * @param newType New LLVM type
 */
void SymbolTableEntry::updateLLVMType(llvm::Type* newType) {
    llvmType = newType;
}

/**
 * Update the value of a symbol. This is used to save the allocated address where the symbol lives
 *
 * @param address Address of the value in memory
 */
void SymbolTableEntry::updateAddress(llvm::Value* address) {
    memAddress = address;
}

/**
 * Sets the state of the symbol to used
 */
void SymbolTableEntry::setUsed() {
    used = true;
}

/**
 * Stringify the current symbol to a human-readable form. Used to dump whole symbol tables with their contents.
 *
 * @return Symbol table entry in form of a string
 */
std::string SymbolTableEntry::toString() {
    std::string stateStr = state == INITIALIZED ? "initialized" : "declared";
    std::string constStr = isConstant ? "yes" : "no";
    std::string globalStr = isGlobal ? "yes" : "no";
    return "Name: " + name + ", Type: " + type.getName() + ", OrderIndex: " + std::to_string(orderIndex) + ", State: " +
        stateStr + ", Const: " + constStr + ", IsGlobal: " + globalStr;
}

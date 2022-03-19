// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SymbolTableEntry.h"

#include <exception/SemanticError.h>

/**
 * Retrieve the name of the current symbol
 *
 * @return Name of the curren symbol
 */
std::string SymbolTableEntry::getName() const {
    return name;
}

/**
 * Retrieve the type of the current symbol
 *
 * @return Type of the current symbol
 */
SymbolType SymbolTableEntry::getType() const {
    return type;
}

/**
 * Update the type of a symbol. This is used for substantiate types in the process of type inference
 *
 * @param newType New type of the current symbol
 */
void SymbolTableEntry::updateType(SymbolType newType, bool force) {
    if (!force && type != SymbolType(TY_DYN))                                      // GCOV_EXCL_LINE
        throw std::runtime_error("Internal compiler error: Cannot change type of non-dyn"); // GCOV_EXCL_LINE
    type = std::move(newType);
}

/**
 * Retrieve the symbol specifiers of the current symbol
 *
 * @return Symbol Specifiers of the current symbol
 */
SymbolSpecifiers SymbolTableEntry::getSpecifiers() const {
    return specifiers;
}

/**
 * Retrieve the state of the current symbol
 *
 * @return State of the current symbol
 */
SymbolState SymbolTableEntry::getState() const {
    return state;
}

/**
 * Update the state of the current symbol
 *
 * @throws SemanticError When trying to re-assign a constant variable
 * @throws runtime_error When the state of the symbol is set to initialized before a concrete type was set
 * @param newState New state of the current symbol
 */
void SymbolTableEntry::updateState(SymbolState newState, const ErrorFactory* err, const antlr4::Token& token) {
    if (state == INITIALIZED && specifiers.isConst())
        throw err->get(token, REASSIGN_CONST_VARIABLE, "Not re-assignable variable '" + name + "'");
    if (newState == INITIALIZED && type == SymbolType(TY_DYN))                                               // GCOV_EXCL_LINE
        throw std::runtime_error("Internal compiler error: could not determine type of variable '" + name + "'"); // GCOV_EXCL_LINE
    state = newState;
}

/**
 * Retrieve the token where the symbol was defined
 *
 * @return Definition token
 */
const antlr4::Token& SymbolTableEntry::getDefinitionToken() const {
    return definitionToken;
}

/**
 * Retrieve the llvm type of the current symbol
 *
 * @return LLVM type of the current symbol
 */
llvm::Type* SymbolTableEntry::getLLVMType() const {
    return llvmType;
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
 * Retrieve the address of the assigned value
 *
 * @return Address of the value in memory
 */
llvm::Value* SymbolTableEntry::getAddress() const {
    return memAddress.empty() ? nullptr : memAddress.top();
}

/**
 * Update the address of a symbol. This is used to save the allocated address where the symbol lives.
 *
 * @param address Address of the symbol in memory
 */
void SymbolTableEntry::updateAddress(llvm::Value* address) {
    if (!memAddress.empty()) memAddress.pop();
    memAddress.push(address);
}

/**
 * Pushes an address onto the stack. Can be called when entering a nested function to ensure that a valid address is given
 *
 * @param address Address of the symbol in memory
 */
void SymbolTableEntry::pushAddress(llvm::Value* address) {
    memAddress.push(address);
}

/**
 * Pop an address from the stack. Can be called when leaving a nested function
 */
void SymbolTableEntry::popAddress() {
    memAddress.pop();
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
 * Sets the state of the symbol to used
 */
void SymbolTableEntry::setUsed() {
    used = true;
}

/**
 * Stringify the current symbol to a human-readable form. Used to dump whole symbol tables with their contents.
 *
 * Example:
 * {
 *   "name": "testIden",
 *   "type": "int[]*",
 *   "orderIndex": 4,
 *   "state": "initialized",
 *   "specifiers: [
 *     "const": true,
 *     "signed": false
 *   ],
 *   "isGlobal": false
 * }
 *
 * @return Symbol table entry as a JSON object
 */
nlohmann::ordered_json SymbolTableEntry::toJSON() const {
    nlohmann::json result;
    result["name"] = name;
    result["type"] = type.getName(true);
    result["orderIndex"] = orderIndex;
    result["state"] = state == INITIALIZED ? "initialized" : "declared";
    result["specifiers"] = specifiers.toJSON();
    result["isGlobal"] = isGlobal;
    return result;
}
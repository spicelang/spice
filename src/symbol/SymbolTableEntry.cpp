// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SymbolTableEntry.h"

#include <ast/AstNodes.h>
#include <exception/SemanticError.h>
#include <util/CodeLoc.h>

/**
 * Update the type of a symbol. This is used for substantiateOptionalParams types in the process of type inference
 *
 * @param newType New type of the current symbol
 */
void SymbolTableEntry::updateType(const SymbolType &newType, bool force) {
  if (force || type.is(TY_DYN)) {
    type = newType;
  } else if (type.isBaseType(TY_STRING) && newType.is(TY_STRING)) {
    type = type.replaceBaseType(newType);
  } else {
    throw std::runtime_error("Internal compiler error: Cannot change type of non-dyn"); // GCOV_EXCL_LINE
  }
}

/**
 * Update the state of the current symbol
 *
 * @throws SemanticError When trying to re-assign a constant variable
 * @throws runtime_error When the state of the symbol is set to initialized before a concrete type was set
 *
 * @param newState New state of the current symbol
 * @param node AST node where the update takes place
 * @param force Force update. This can only be used compiler-internal
 */
void SymbolTableEntry::updateState(SymbolState newState, const AstNode *node, bool force) {
  // Check if this is a constant variable and is already initialized
  if (state == INITIALIZED && specifiers.isConst() && !force)
    throw SemanticError(node, REASSIGN_CONST_VARIABLE, "Not re-assignable variable '" + name + "'");
  // Check if the type is known at time of initialization
  if (newState == INITIALIZED && type == SymbolType(TY_DYN))                                                  // GCOV_EXCL_LINE
    throw std::runtime_error("Internal compiler error: could not determine type of variable '" + name + "'"); // GCOV_EXCL_LINE
  state = newState;
}

/**
 * Retrieve the code location where the symbol was declared
 *
 * @return Declaration code location
 */
const CodeLoc &SymbolTableEntry::getDeclCodeLoc() const { return declNode->codeLoc; }

/**
 * Retrieve the llvm type of the current struct symbol
 *
 * @return LLVM type of the current struct symbol
 */
llvm::Type *SymbolTableEntry::getStructLLVMType() const {
  assert(type.is(TY_STRUCT));
  return llvmType;
}

/**
 * Update the struct LLVM type of a symbol
 *
 * @param newStructType New struct LLVM type
 */
void SymbolTableEntry::setStructLLVMType(llvm::Type *newStructType) {
  assert(type.is(TY_STRUCT));
  llvmType = newStructType;
}

/**
 * Retrieve the address of the assigned value
 *
 * @return Address of the value in memory
 */
llvm::Value *SymbolTableEntry::getAddress() const { return memAddress.empty() ? nullptr : memAddress.top(); }

/**
 * Update the address of a symbol. This is used to save the allocated address where the symbol lives.
 *
 * @param address Address of the symbol in memory
 */
void SymbolTableEntry::updateAddress(llvm::Value *address) {
  assert(address != nullptr);
  if (!memAddress.empty())
    memAddress.pop();
  memAddress.push(address);
}

/**
 * Pushes an address onto the stack. Can be called when entering a nested function to ensure that a valid address is given
 *
 * @param address Address of the symbol in memory
 */
void SymbolTableEntry::pushAddress(llvm::Value *address) {
  assert(address != nullptr);
  memAddress.push(address);
}

/**
 * Pop an address from the stack. Can be called when leaving a nested function
 */
void SymbolTableEntry::popAddress() { memAddress.pop(); }

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
 *   "isGlobal": false,
 *   "isVolatile": false
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
  result["isVolatile"] = isVolatile;
  return result;
}
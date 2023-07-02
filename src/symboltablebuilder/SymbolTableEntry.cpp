// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "SymbolTableEntry.h"

#include <ast/ASTNodes.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/Scope.h>
#include <util/CodeLoc.h>

namespace spice::compiler {

/**
 * Retrieve the symbol type of this symbol
 *
 * @return Current symbol type of this symbol
 */
const SymbolType &SymbolTableEntry::getType() const { return type; }

/**
 * Update the type of this symbol.
 *
 * @param newType New type of the current symbol
 * @param overwriteExistingType Overwrites the existing type without throwing an error
 */
void SymbolTableEntry::updateType(const SymbolType &newType, bool overwriteExistingType) {
  assert(overwriteExistingType || type.isOneOf({TY_INVALID, TY_DYN}));
  type = newType;
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
void SymbolTableEntry::updateState(const LifecycleState &newState, ASTNode *node, bool force) {
  const LifecycleState oldState = lifecycle.getCurrentState();
  // Check if this is a constant variable and is already initialized
  if (newState != DEAD && oldState != DECLARED && type.isConst() && !force)
    throw SemanticError(node, REASSIGN_CONST_VARIABLE, "Not re-assignable variable '" + name + "'");
  if (newState == DEAD && oldState == DECLARED)                                                   // GCOV_EXCL_LINE
    throw CompilerError(INTERNAL_ERROR, "Cannot destruct uninitialized variable '" + name + "'"); // GCOV_EXCL_LINE
  if (newState == DEAD && oldState == DEAD)                                                       // GCOV_EXCL_LINE
    throw CompilerError(INTERNAL_ERROR, "Cannot destruct already freed variable '" + name + "'"); // GCOV_EXCL_LINE
  lifecycle.addEvent({newState, node});
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
llvm::StructType *SymbolTableEntry::getStructLLVMType() const {
  assert(type.is(TY_STRUCT));
  return llvmStructType;
}

/**
 * Update the struct LLVM type of a symbol
 *
 * @param newStructType New struct LLVM type
 */
void SymbolTableEntry::setStructLLVMType(llvm::StructType *newStructType) {
  assert(type.is(TY_STRUCT));
  llvmStructType = newStructType;
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
  // Ensure that structs fields get no addresses assigned, as the addresses are meant for the struct instances
  assert((scope->type != SCOPE_STRUCT && scope->type != SCOPE_INTERFACE) || type.isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  if (memAddress.empty())
    memAddress.push(address);
  else
    memAddress.top() = address;
}

/**
 * Check if this symbol is a struct field
 *
 * @return Struct field or not
 */
bool SymbolTableEntry::isField() const { return scope->type == SCOPE_STRUCT && !type.isOneOf({TY_FUNCTION, TY_PROCEDURE}); }

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
  std::string stateStr;
  switch (lifecycle.getCurrentState()) {
  case DECLARED:
    stateStr = "declared";
    break;
  case INITIALIZED:
    stateStr = "initialized";
    break;
  case MOVED:
    stateStr = "moved";
    break;
  case RETURNED:
    stateStr = "returned";
    break;
  case DEAD:
    stateStr = "destructed";
    break;
  }

  nlohmann::json result;
  result["name"] = name;
  result["type"] = type.getName(true);
  result["codeLoc"] = declNode->codeLoc.toString();
  result["orderIndex"] = orderIndex;
  result["state"] = stateStr;
  result["isGlobal"] = global;
  result["isVolatile"] = isVolatile;
  return result;
}

} // namespace spice::compiler
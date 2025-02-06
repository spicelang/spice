// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "SymbolTableEntry.h"

#include <ast/ASTNodes.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/Scope.h>
#include <util/CodeLoc.h>

namespace spice::compiler {

/**
 * Retrieve the qualified type of this symbol
 *
 * @return Qualified type of this symbol
 */
const QualType &SymbolTableEntry::getQualType() const { return qualType; }

/**
 * Update the type of this symbol.
 *
 * @param newType New type of the current symbol
 * @param overwriteExistingType Overwrites the existing type without throwing an error
 */
void SymbolTableEntry::updateType(const QualType &newType, [[maybe_unused]] bool overwriteExistingType) {
  assert(overwriteExistingType || qualType.isOneOf({TY_INVALID, TY_DYN}));
  qualType = newType;
}

/**
 * Update the state of the current symbol
 *
 * @throws CompilerError When the state of the symbol is set to initialized before a concrete type was set
 *
 * @param newState New state of the current symbol
 * @param node AST node where the update takes place
 */
void SymbolTableEntry::updateState(const LifecycleState &newState, const ASTNode *node) {
  const LifecycleState oldState = lifecycle.getCurrentState();
  if (newState == DEAD && oldState == DECLARED)                                                      // GCOV_EXCL_LINE
    throw CompilerError(INTERNAL_ERROR, "Cannot destroy uninitialized variable '" + name + "'");     // GCOV_EXCL_LINE
  if (newState == DEAD && oldState == DEAD)                                                          // GCOV_EXCL_LINE
    throw CompilerError(INTERNAL_ERROR, "Cannot destroy already destroyed variable '" + name + "'"); // GCOV_EXCL_LINE
  lifecycle.addEvent({newState, node});
}

/**
 * Retrieve the code location where the symbol was declared
 *
 * @return Declaration code location
 */
const CodeLoc &SymbolTableEntry::getDeclCodeLoc() const { return declNode->codeLoc; }

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
  assert(address->getType()->isPointerTy());
  // Ensure that structs fields get no addresses assigned, as the addresses are meant for the struct instances
  assert((scope->type != ScopeType::STRUCT && scope->type != ScopeType::INTERFACE) ||
         qualType.isOneOf({TY_FUNCTION, TY_PROCEDURE}));
  if (memAddress.empty())
    memAddress.push(address);
  else
    memAddress.top() = address;
}

/**
 * Push a new address to the stack
 *
 * @param address Address to push
 */
void SymbolTableEntry::pushAddress(llvm::Value *address) {
  assert(address != nullptr);
  memAddress.push(address);
}

/**
 * Remove the last address from the stack
 */
void SymbolTableEntry::popAddress() {
  assert(!memAddress.empty());
  memAddress.pop();
}

/**
 * Check if this symbol is a struct field
 *
 * @return Struct field or not
 */
bool SymbolTableEntry::isField() const {
  return scope->type == ScopeType::STRUCT && orderIndex < scope->getFieldCount() && !anonymous;
}

/**
 * Check if this symbol is a function/procedure definition
 *
 * @return Function/procedure or not
 */
bool SymbolTableEntry::isFunctionOrProcedure() const {
  return scope->type == ScopeType::GLOBAL && qualType.isOneOf({TY_FUNCTION, TY_PROCEDURE});
}

/**
 * Check if this symbol is a method definition
 *
 * @return Method or not
 */
bool SymbolTableEntry::isMethod() const {
  return scope->type == ScopeType::STRUCT && !isField() && qualType.isOneOf({TY_FUNCTION, TY_PROCEDURE});
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
 *   "qualifiers: [
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
  result["type"] = qualType.getName(true);
  result["codeLoc"] = declNode->codeLoc.toString();
  result["orderIndex"] = orderIndex;
  result["state"] = lifecycle.getCurrentStateName();
  result["isGlobal"] = global;
  result["isVolatile"] = isVolatile;
  return result;
}

} // namespace spice::compiler
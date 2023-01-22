// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "SymbolTable.h"

#include <stdexcept>
#include <utility>

#include <ast/ASTNodes.h>
#include <model/Function.h>
#include <model/GenericType.h>
#include <model/Struct.h>
#include <util/CodeLoc.h>
#include <util/CompilerWarning.h>

namespace spice::compiler {

/**
 * Insert a new symbol into the current symbol table. If it is a parameter, append its name to the paramNames vector
 *
 * @param name Name of the symbol
 * @param specifiers Specifiers of the symbol
 * @param declNode AST node where the symbol is declared
 * @return Inserted entry
 */
SymbolTableEntry *SymbolTable::insert(const std::string &name, const SymbolSpecifiers &specifiers, ASTNode *declNode) {
  bool isGlobal = parent == nullptr;
  size_t orderIndex = symbols.size();
  // Insert into symbols map. The type is 'dyn', because concrete types are determined by the type checker later on
  symbols.insert({name, SymbolTableEntry(name, SymbolType(TY_INVALID), scope, specifiers, declNode, orderIndex, isGlobal)});
  return &symbols.at(name);
}

/**
 * Insert a new anonymous symbol into the current symbol table.
 * The anonymous symbol will be identified via the definition code location
 *
 * @param type Type of the symbol
 * @param declNode AST node where the anonymous symbol is declared
 * @return Inserted entry
 */
SymbolTableEntry *SymbolTable::insertAnonymous(const SymbolType &type, ASTNode *declNode) {
  // Check if the anonymous entry already exists
  if (SymbolTableEntry *anonSymbol = lookupAnonymous(declNode->codeLoc))
    return anonSymbol;
  // Otherwise, create an anonymous entry
  const std::string name = "anon." + declNode->codeLoc.toString();
  insert(name, SymbolSpecifiers::of(type), declNode);
  SymbolTableEntry *anonSymbol = lookupAnonymous(declNode->codeLoc);
  anonSymbol->updateType(type, false);
  anonSymbol->anonymous = true;
  anonSymbol->used = true;
  return anonSymbol;
}

/**
 * Copy a symbol by its name
 *
 * @param originalName Original symbol name
 * @param newName New symbol name
 */
void SymbolTable::copySymbol(const std::string &originalName, const std::string &newName) {
  SymbolTableEntry *entryToCopy = lookupStrict(originalName);
  assert(entryToCopy != nullptr);
  symbols.insert({newName, *entryToCopy});
}

/**
 * Check if a symbol exists in the current or any parent scope and return it if possible
 *
 * @param name Name of the desired symbol
 * @return Desired symbol / nullptr if the symbol was not found
 */
SymbolTableEntry *SymbolTable::lookup(const std::string &name) { // NOLINT(misc-no-recursion)
  // Check if the symbol exists in the current scope. If yes, take it
  SymbolTableEntry *entry = lookupStrict(name);
  if (!entry) { // Symbol was not found in the current scope
    // We reached the root scope, the symbol does not exist at all
    if (parent == nullptr)
      return nullptr;
    // If there is a parent scope, continue the search there
    entry = parent->lookup(name);
    // Symbol was also not found in all the parent scopes, return nullptr
    if (!entry)
      return nullptr;

    // Check if this scope requires capturing and capture the variable if appropriate
    if (capturingRequired && !captures.contains(name) && !entry->getType().isOneOf({TY_IMPORT, TY_FUNCTION, TY_PROCEDURE}))
      captures.insert({name, Capture(entry)});
  }

  return entry;
}

/**
 * Check if a symbol exists in the current scope and return it if possible
 *
 * @param name Name of the desired symbol
 * @return Desired symbol / nullptr if the symbol was not found
 */
SymbolTableEntry *SymbolTable::lookupStrict(const std::string &name) {
  if (name.empty())
    return nullptr;
  // Check if a symbol with this name exists in this scope
  if (symbols.contains(name))
    return &symbols.at(name);
  // Check if a capture with this name exists in this scope
  if (captures.contains(name))
    return captures.at(name).capturedEntry;
  // Otherwise, return a nullptr
  return nullptr;
}

/**
 * Check if an order index exists in the current or any parent scope and returns it if possible.
 * Warning: Unlike the `lookup` method, this one doesn't consider the parent scopes
 *
 * @param orderIndex Order index of the desired symbol
 * @return Desired symbol / nullptr if the symbol was not found
 */
SymbolTableEntry *SymbolTable::lookupByIndex(unsigned int orderIndex) {
  for (auto &[key, val] : symbols) {
    if (val.orderIndex == orderIndex)
      return &val;
  }
  return nullptr;
}

/**
 * Check if an anonymous symbol exists in the current scope and return it if possible
 *
 * @param codeLoc Definition code loc
 * @return Anonymous symbol
 */
SymbolTableEntry *SymbolTable::lookupAnonymous(const CodeLoc &codeLoc) { return lookup("anon." + codeLoc.toString()); }

/**
 * Check if a capture exists in the current or any parent scope scope and return it if possible
 *
 * @param name Name of the desired captured symbol
 * @return Capture / nullptr if the capture was not found
 */
Capture *SymbolTable::lookupCapture(const std::string &name) { // NOLINT(misc-no-recursion)
  // Check if the capture exists in the current scope. If yes, take it
  Capture *capture = lookupCaptureStrict(name);
  if (capture)
    return capture;

  // We reached the root scope, the symbol does not exist at all
  if (parent == nullptr)
    return nullptr;

  return parent->lookupCapture(name);
}

/**
 * Check if a capture exists in the current scope and return it if possible
 *
 * @param name Name of the desired captured symbol
 * @return Capture / nullptr if the capture was not found
 */
Capture *SymbolTable::lookupCaptureStrict(const std::string &name) {
  // If available in the current scope, return it
  if (captures.contains(name))
    return &captures.at(name);
  // Otherwise, return nullptr
  return nullptr;
}

/**
 * Deletes an existing anonymous symbol
 *
 * @param name Anonymous symbol name
 */
void SymbolTable::deleteAnonymous(const std::string &name) { symbols.erase(name); }

/**
 * Stringify a symbol table to a human-readable form. This is used to realize dumps of symbol tables
 *
 * Example:
 * {
 *   "name": "<SymbolTableName>"
 *   "symbols": [
 *     ... (SymbolTableEntry)
 *   ],
 *   "children": [
 *     ... (SymbolTable)
 *   ]
 * }
 *
 * @return Symbol table if form of a string
 */
nlohmann::json SymbolTable::toJSON() const {
  // Collect all symbols
  std::vector<nlohmann::json> jsonSymbols;
  jsonSymbols.reserve(symbols.size());
  for (const auto &symbol : symbols)
    jsonSymbols.emplace_back(symbol.second.toJSON());

  // Collect all captures
  std::vector<nlohmann::json> jsonCaptures;
  jsonCaptures.reserve(captures.size());
  for (const auto &capture : captures)
    jsonCaptures.emplace_back(capture.second.toJSON());

  // Generate json
  nlohmann::json result;
  result["symbols"] = jsonSymbols;
  result["captures"] = jsonCaptures;
  return result;
}

} // namespace spice::compiler
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SymbolTable.h"

#include <stdexcept>
#include <utility>

#include <analyzer/AnalyzerVisitor.h>
#include <symbol/Function.h>
#include <symbol/GenericType.h>
#include <symbol/Struct.h>
#include <util/CodeLoc.h>
#include <util/CompilerWarning.h>

/**
 * Insert a new symbol into the current symbol table. If it is a parameter, append its name to the paramNames vector
 *
 * @param name Name of the symbol
 * @param type Type of the symbol
 * @param specifiers Specifiers of the symbol
 * @param state State of the symbol (declared or initialized)
 * @param declNode AST node where the symbol is declared
 */
void SymbolTable::insert(const std::string &name, const SymbolType &type, const SymbolSpecifiers &specifiers,
                         const ASTNode *declNode) {
  bool isGlobal = parent == nullptr;
  size_t orderIndex = symbols.size();
  // Insert into symbols map
  symbols.insert({name, SymbolTableEntry(name, type, this, specifiers, declNode, orderIndex, isGlobal)});
}

/**
 * Insert a new anonymous symbol into the current symbol table.
 * The anonymous symbol will be identified via the definition code location
 *
 * @param type Type of the symbol
 * @param declNode AST node where the anonymous symbol is declared
 */
void SymbolTable::insertAnonymous(const SymbolType &type, const ASTNode *declNode) {
  std::string name = "anon." + declNode->codeLoc.toString();
  insert(name, type, SymbolSpecifiers(type), declNode);
  lookupAnonymous(declNode->codeLoc)->isUsed = true;
}

/**
 * Add a capture to the capture list manually
 *
 * @param name Capture name
 * @param capture Capture
 */
void SymbolTable::insertCapture(const std::string &name, const Capture &capture) { captures.insert({name, capture}); }

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
    if (isCapturingRequired && !captures.contains(name) && !entry->type.isOneOf({TY_IMPORT, TY_FUNCTION, TY_PROCEDURE}))
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
 * Check if a global variable exists in any of the imported modules and return it if found
 *
 * @param globalName Name of the global variable
 * @return Desired symbol / nullptr if the global was not found
 */
SymbolTableEntry *SymbolTable::lookupGlobal(const std::string &globalName, bool skipThisScope) { // NOLINT(misc-no-recursion)
  // Search in the current scope
  if (!skipThisScope) {
    SymbolTableEntry *globalSymbol = lookupStrict(globalName);
    if (globalSymbol)
      return globalSymbol;
  }
  // Loop through all children to find the global var
  for (const auto &[scopeName, childScope] : children) {
    if (childScope->isImported(this)) { // Only consider if it is an imported module scope
      SymbolTableEntry *globalSymbol = childScope->lookupGlobal(globalName);
      if (globalSymbol)
        return globalSymbol;
    }
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
 * Insert a new generic type in this scope
 *
 * @param typeName Name of the generic type
 * @param genericType Generic type itself
 */
void SymbolTable::insertGenericType(const std::string &typeName, const GenericType &genericType) {
  genericTypes.insert({typeName, genericType});
}

/**
 * Search for a generic type by its name. If it was not found, the parent scopes will be searched.
 * If the generic type does not exist at all, the function will return a nullptr.
 *
 * @param typeName Name of the generic type
 * @return Generic type
 */
GenericType *SymbolTable::lookupGenericType(const std::string &typeName) { // NOLINT(misc-no-recursion)
  if (genericTypes.contains(typeName))
    return &genericTypes.at(typeName);
  return parent ? parent->lookupGenericType(typeName) : nullptr;
}

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
nlohmann::json SymbolTable::toJSON() const { // NOLINT(misc-no-recursion)
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

  // Collect all children
  std::vector<nlohmann::json> jsonChildren;
  jsonChildren.reserve(symbols.size());
  for (const auto &child : children) {
    nlohmann::json c = child.second->toJSON();
    c["name"] = child.first; // Inject symbol table name into JSON object
    jsonChildren.emplace_back(c);
  }

  // Generate json
  nlohmann::json result;
  result["symbols"] = jsonSymbols;
  result["captures"] = jsonCaptures;
  result["children"] = jsonChildren;
  return result;
}
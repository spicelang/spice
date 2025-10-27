// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "SymbolTable.h"

#include "SourceFile.h"
#include <ast/ASTNodes.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <util/CodeLoc.h>
#include <util/CompilerWarning.h>

namespace spice::compiler {

/**
 * Insert a new symbol into the current symbol table. If it is a parameter, append its name to the paramNames vector
 *
 * @param name Name of the symbol
 * @param declNode AST node where the symbol is declared
 * @param isAnonymousSymbol If this symbol should be anonymous
 * @return Inserted entry
 */
SymbolTableEntry *SymbolTable::insert(const std::string &name, ASTNode *declNode, bool isAnonymousSymbol) {
  const bool isGlobal = parent == nullptr;
  size_t orderIndex = SIZE_MAX;
  if (!isAnonymousSymbol)
    orderIndex = std::ranges::count_if(symbols, [](const auto &entry) { return !entry.second.anonymous; });
  // Insert into symbols map. The type is 'dyn', because concrete types are determined by the type checker later on
  assert(!symbols.contains(name));
  symbols.insert({name, SymbolTableEntry(name, QualType(TY_INVALID), scope, declNode, orderIndex, isGlobal)});
  // Set entry to declared
  SymbolTableEntry *entry = &symbols.at(name);
  entry->updateState(DECLARED, declNode);

  // Check if shadowed
  if (parent != nullptr && parent->lookup(name) != nullptr && !declNode->isParam()) {
    const std::string warningMsg = "Variable '" + name + "' shadows a variable in a parent scope";
    const CompilerWarning warning(declNode->codeLoc, SHADOWED_VARIABLE, warningMsg);
    scope->sourceFile->compilerOutput.warnings.push_back(warning);
  }

  return entry;
}

/**
 * Insert a new anonymous symbol into the current symbol table.
 * The anonymous symbol will be identified via the definition code location
 *
 * @param qualType Type of the symbol
 * @param declNode AST node where the anonymous symbol is declared
 * @param numericSuffix Custom numeric suffix
 * @return Inserted entry
 */
SymbolTableEntry *SymbolTable::insertAnonymous(const QualType &qualType, ASTNode *declNode, size_t numericSuffix) {
  // Check if the anonymous entry already exists
  if (SymbolTableEntry *anonSymbol = lookupAnonymous(declNode->codeLoc, numericSuffix))
    return anonSymbol;
  // Otherwise, create an anonymous entry
  std::stringstream name;
  name << "anon." << declNode->codeLoc.toString();
  if (numericSuffix > 0)
    name << '.' << std::to_string(numericSuffix);
  SymbolTableEntry *anonSymbol = insert(name.str(), declNode, true);
  anonSymbol->updateType(qualType, false);
  anonSymbol->updateState(DECLARED, declNode);
  anonSymbol->updateState(INITIALIZED, declNode);
  anonSymbol->anonymous = true;
  anonSymbol->used = true;
  return anonSymbol;
}

/**
 * Copy a symbol by its name
 *
 * @param originalName Original symbol name
 * @param newName New symbol name
 * @return Copied entry
 */
SymbolTableEntry *SymbolTable::copySymbol(const std::string &originalName, const std::string &newName) {
  SymbolTableEntry *entryToCopy = lookupStrict(originalName);
  assert(entryToCopy != nullptr);
  auto [it, success] = symbols.insert({newName, *entryToCopy});
  assert(success);
  return &it->second;
}

/**
 * Check if a symbol exists in the current or any parent scope and return it if possible
 *
 * @param name Name of the desired symbol
 * @return Desired symbol / nullptr if the symbol was not found
 */
SymbolTableEntry *SymbolTable::lookup(const std::string &name) { // NOLINT(misc-no-recursion)
  // Check if the symbol exists in the current scope. If yes, take it
  if (SymbolTableEntry *entry = lookupStrict(name))
    return entry;

  // Symbol was not found in the current scope
  // We reached the root scope, the symbol does not exist at all
  if (!parent)
    return nullptr;
  // If we search for the result variable, we want to stop the search when exiting a lambda body
  if (name == RETURN_VARIABLE_NAME && scope->type == ScopeType::LAMBDA_BODY)
    return nullptr;
  // If there is a parent scope, continue the search there
  SymbolTableEntry *entry = parent->lookup(name);
  // Symbol was also not found in all the parent scopes, return nullptr
  if (!entry)
    return nullptr;
  // Check if this scope requires capturing and capture the variable if appropriate
  if (capturingRequired && !captures.contains(name) && !entry->getQualType().isOneOf({TY_IMPORT, TY_FUNCTION, TY_PROCEDURE})) {
    // We need to make the symbol volatile if we are in an async scope and try to access a symbol that is not in an async scope
    entry->isVolatile = scope->isInAsyncScope() && !entry->scope->isInAsyncScope();
    // Add the capture to the current scope
    captures.emplace(name, Capture(entry));
  }
  return entry;
}

/**
 * Check if a symbol exists in the current or any parent scope and return it if possible.
 * If the target symbol is an alias symbol, resolve the alias container entry.
 *
 * @param name Name of the desired symbol
 * @return Desired symbol / nullptr if the symbol was not found + alias or not
 */
std::pair<SymbolTableEntry *, bool> SymbolTable::lookupWithAliasResolution(const std::string &name) {
  SymbolTableEntry *entry = lookup(name);
  if (!entry || !entry->getQualType().is(TY_ALIAS))
    return {entry, false};

  // We have an alias type here, resolve it
  assert(entry->scope->isRootScope());
  entry->used = true;
  const std::string aliasedContainerEntryName = entry->name + ALIAS_CONTAINER_SUFFIX;
  SymbolTableEntry *aliasedTypeContainerEntry = entry->scope->lookupStrict(aliasedContainerEntryName);
  assert(aliasedTypeContainerEntry != nullptr);
  return {aliasedTypeContainerEntry, true};
}

/**
 * Check if a symbol exists in the current scope and return it if possible
 *
 * @param symbolName Name of the desired symbol
 * @return Desired symbol / nullptr if the symbol was not found
 */
SymbolTableEntry *SymbolTable::lookupStrict(const std::string &symbolName) {
  if (symbolName.empty())
    return nullptr;
  // Check if a symbol with this name exists in this scope
  if (symbols.contains(symbolName))
    return &symbols.at(symbolName);
  // Check if a capture with this name exists in this scope
  if (captures.contains(symbolName))
    return captures.at(symbolName).capturedSymbol;
  // Otherwise, return a nullptr
  return nullptr;
}

/**
 * Check if a symbol exists in one of the composed field scopes of the current scope and return it if possible.
 * This only works if the current scope is a struct scope.
 *
 * @param name Name of the desired symbol
 * @param indexPath How to index the found symbol using order indices (e.g. for GEP)
 * @return Desired symbol / nullptr if the symbol was not found
 */
SymbolTableEntry *SymbolTable::lookupInComposedFields(const std::string &name, // NOLINT(misc-no-recursion)
                                                      std::vector<size_t> &indexPath) {
  assert(scope->type == ScopeType::STRUCT);

  // Check if we have a symbol with this name in the current scope
  if (SymbolTableEntry *result = lookupStrict(name)) {
    indexPath.push_back(result->orderIndex);
    return result;
  }

  // If it was not found in the current scope, loop through all composed fields in this scope
  for (size_t i = 0; i < scope->getFieldCount(); i++) {
    const SymbolTableEntry *fieldEntry = lookupStrictByIndex(i);

    // Skip all fields that are not composition fields
    if (!fieldEntry->getQualType().isComposition())
      continue;

    // Add the current field's order index to the index path
    indexPath.push_back(fieldEntry->orderIndex);

    // Search in the composed field's body scope
    Scope *searchScope = fieldEntry->getQualType().getBodyScope();
    assert(searchScope != nullptr);
    if (SymbolTableEntry *result = searchScope->symbolTable.lookupInComposedFields(name, indexPath))
      return result;

    // Remove the current field's order index from the index path
    indexPath.pop_back();
  }

  // Symbol was not found in current scope, return nullptr
  return nullptr;
}

/**
 * Check if an order index exists in the current or any parent scope and returns it if possible.
 * Warning: Unlike the `lookup` method, this one doesn't consider the parent scopes
 *
 * @param orderIndex Order index of the desired symbol
 * @return Desired symbol / nullptr if the symbol was not found
 */
SymbolTableEntry *SymbolTable::lookupStrictByIndex(unsigned int orderIndex) {
  for (auto &val : symbols | std::views::values) {
    if (val.orderIndex == orderIndex)
      return &val;
  }
  return nullptr;
}

/**
 * Check if an anonymous symbol exists in the current scope and return it if possible
 *
 * @param codeLoc Definition code loc
 * @param numericSuffix Numeric suffix of the anonymous symbol
 * @return Anonymous symbol
 */
SymbolTableEntry *SymbolTable::lookupAnonymous(const CodeLoc &codeLoc, size_t numericSuffix) {
  std::string name = "anon." + codeLoc.toString();
  if (numericSuffix > 0)
    name += "." + std::to_string(numericSuffix);
  return lookup(name);
}

/**
 * Check if a capture exists in the current or any parent scope and return it if possible
 *
 * @param name Name of the desired captured symbol
 * @return Capture / nullptr if the capture was not found
 */
Capture *SymbolTable::lookupCapture(const std::string &name) { // NOLINT(misc-no-recursion)
  // Check if the capture exists in the current scope. If yes, take it
  if (Capture *capture = lookupCaptureStrict(name))
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
 * Set capturing for this scope required.
 */
void SymbolTable::setCapturingRequired() { capturingRequired = true; }

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
  for (const SymbolTableEntry &symbol : symbols | std::views::values)
    jsonSymbols.emplace_back(symbol.toJSON());

  // Collect all captures
  std::vector<nlohmann::json> jsonCaptures;
  jsonCaptures.reserve(captures.size());
  for (const Capture &capture : captures | std::views::values)
    jsonCaptures.emplace_back(capture.toJSON());

  // Generate json
  nlohmann::json result;
  result["symbols"] = jsonSymbols;
  result["captures"] = jsonCaptures;
  return result;
}

} // namespace spice::compiler
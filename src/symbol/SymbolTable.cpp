// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "symbol/SymbolTable.h"

#include <stdexcept>
#include <utility>

#include <analyzer/AnalyzerVisitor.h>
#include <util/CompilerWarning.h>

/**
 * Insert a new symbol into the current symbol table. If it is a parameter, append its name to the paramNames vector
 *
 * @param name Name of the symbol
 * @param type Type of the symbol
 * @param state State of the symbol (declared or initialized)
 * @param isConstant Enabled if the symbol is a constant
 * @param isParameter Enabled if the symbol is a function/procedure parameter
 */
void SymbolTable::insert(const std::string &name, const SymbolType &type, SymbolSpecifiers specifiers, SymbolState state,
                         const antlr4::Token &token, bool isParameter) {
  bool isGlobal = getParent() == nullptr;
  unsigned int orderIndex = symbols.size();
  // Insert into symbols map
  symbols.insert({name, SymbolTableEntry(name, type, specifiers, state, token, orderIndex, isGlobal)});
  // If the symbol is a parameter, add it to the parameters list
  if (isParameter)
    paramNames.push_back(name);
}

/**
 * Check if a symbol exists in the current or any parent scope and return it if possible
 *
 * @param name Name of the desired symbol
 * @return Desired symbol / nullptr if the symbol was not found
 */
SymbolTableEntry *SymbolTable::lookup(const std::string &name) {
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
    if (requiresCapturing && captures.find(name) == captures.end() &&
        !entry->getType().isOneOf({TY_IMPORT, TY_FUNCTION, TY_PROCEDURE})) {
      captures.insert({name, Capture(entry)});
    }
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
  // If not available in the current scope, return nullptr
  if (symbols.find(name) == symbols.end())
    return nullptr;
  // Otherwise, return the symbol
  return &symbols.at(name);
}

/**
 * Check if an order index exists in the current or any parent scope and returns it if possible.
 * Warning: Unlike the `lookup` method, this one doesn't consider the parent scopes
 *
 * @param orderIndex Order index of the desired symbol
 * @return Desired symbol / nullptr if the symbol was not found
 */
SymbolTableEntry *SymbolTable::lookupByIndexInCurrentScope(unsigned int orderIndex) {
  for (auto &[key, val] : symbols) {
    if (val.getOrderIndex() == orderIndex)
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
SymbolTableEntry *SymbolTable::lookupGlobalByName(const std::string &globalName, bool skipThisScope) {
  // Search in the current scope
  if (!skipThisScope) {
    SymbolTableEntry *globalSymbol = lookupStrict(globalName);
    if (globalSymbol)
      return globalSymbol;
  }
  // Loop through all children to find the global var
  for (auto &[scopeName, childScope] : children) {
    if (childScope.isImported()) { // Only consider if it is an imported module scope
      SymbolTableEntry *globalSymbol = childScope.lookupGlobalByName(globalName);
      if (globalSymbol)
        return globalSymbol;
    }
  }
  return nullptr;
}

/**
 * Check if a capture exists in the current or any parent scope scope and return it if possible
 *
 * @param symbolName Name of the desired captured symbol
 * @return Capture / nullptr if the capture was not found
 */
Capture *SymbolTable::lookupCapture(const std::string &name) {
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
 * @param symbolName Name of the desired captured symbol
 * @return Capture / nullptr if the capture was not found
 */
Capture *SymbolTable::lookupCaptureStrict(const std::string &name) {
  // If not available in the current scope, return nullptr
  if (captures.find(name) == captures.end())
    return nullptr;
  // Otherwise, return the capture
  return &captures.at(name);
}

/**
 * Search for a symbol table by its name, where a function is defined. Used for function calls to function/procedures
 * which were linked in from other modules
 *
 * @param scopeId Scope ID of the desired symbol table
 * @return Desired symbol table
 */
SymbolTable *SymbolTable::lookupTable(const std::string &scopeId) {
  // If not available in the current scope, search in the parent scope
  if (children.find(scopeId) == children.end()) {
    if (parent == nullptr)
      return nullptr;
    return parent->lookupTable(scopeId);
  }
  // Otherwise, return the entry
  return &children.at(scopeId);
}

/**
 * Search for a symbol table by its name, where a symbol is defined. Used for function calls to function/procedures
 * which were linked in from other modules
 *
 * @param signature Signature of the function/procedure
 * @return Desired symbol table
 */
SymbolTable *SymbolTable::lookupTableWithSignature(const std::string &signature) {
  // Check if scope contains this signature
  if (symbols.find(signature) != symbols.end())
    return this;
  // Current scope does not contain the signature => go up one table
  if (parent == nullptr)
    return nullptr;
  return parent->lookupTableWithSignature(signature);
}

/**
 * Create a child leaf for the tree of symbol tables and return it
 *
 * @param blockName Name of the child scope
 * @return Newly created child table
 */
SymbolTable *SymbolTable::createChildBlock(const std::string &blockName) {
  children.insert({blockName, SymbolTable(this, inMainSourceFile)});
  return &children.at(blockName);
}

/**
 * Mount in symbol tables manually. This is used to hook in symbol tables of imported modules into the symbol table of
 * the source file, which imported the modules
 *
 * @param blockName Name of the child scope
 * @param childBlock Child symbol table
 */
void SymbolTable::mountChildBlock(const std::string &blockName, SymbolTable *childBlock) {
  childBlock->parent = this;
  children.insert({blockName, *childBlock});
}

/**
 * Rename the scope of a symbol table. This is useful for realizing function overloading by storing a function with not
 * only its name, but also its signature
 *
 * @param oldName Old name of the child table
 * @param newName New name of the child table
 */
void SymbolTable::renameChildBlock(const std::string &oldName, const std::string &newName) {
  auto nodeHandler = children.extract(oldName);
  nodeHandler.key() = newName;
  children.insert(std::move(nodeHandler));
}

/**
 * Navigate to parent table of the current one in the tree structure
 *
 * @return Pointer to the parent symbol table
 */
SymbolTable *SymbolTable::getParent() const { return parent; }

/**
 * Navigate to a child table of the current one in the tree structure
 *
 * @param scopeId Name of the child scope
 * @return Pointer to the child symbol table
 */
SymbolTable *SymbolTable::getChild(const std::string &scopeId) {
  if (children.empty())
    return nullptr;
  if (children.find(scopeId) == children.end())
    return nullptr;
  return &children.at(scopeId);
}

/**
 * Returns all symbols of this particular sub-table
 *
 * @return Map of names and the corresponding symbol table entries
 */
std::map<std::string, SymbolTableEntry> &SymbolTable::getSymbols() { return symbols; }

/**
 * Returns all captures of this particular sub-table
 *
 * @return Map of names and the corresponding capture
 */
std::map<std::string, Capture> &SymbolTable::getCaptures() { return captures; }

/**
 * Returns the number of symbols in the table, which are no functions, procedures or import
 *
 * @return Number of fields
 */
unsigned int SymbolTable::getFieldCount() const {
  unsigned int count = 0;
  for (auto &[key, symbol] : symbols) {
    if (!symbol.getType().isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_IMPORT}))
      count++;
  }
  return count;
}

/**
 * Insert an item to the list of function declarations. This list is used to link in functions from other modules and
 * therefore not storing their definition, but their declaration.
 *
 * @param signature Signature of the function declaration
 * @param types List of parameter types of the function declaration
 */
void SymbolTable::insertFunctionDeclaration(const std::string &signature, const std::vector<SymbolType> &types) {
  functionDeclarations.insert({signature, types});
}

/**
 * Retrieve an item from the list of function declarations.
 *
 * @param signature Signature of the desired function declaration
 * @return List of parameter types of the desired function declaration
 */
std::vector<SymbolType> SymbolTable::getFunctionDeclaration(const std::string &signature) const {
  if (functionDeclarations.find(signature) == functionDeclarations.end())
    return {};
  return functionDeclarations.at(signature);
}

/**
 * Insert an item to the list of procedure declarations. This list is used to link in procedures from other modules and
 * therefore not storing their definition, but their declaration.
 *
 * @param signature Signature of the procedure declaration
 * @param types List of parameter types of the procedure declaration
 */
void SymbolTable::insertProcedureDeclaration(const std::string &signature, const std::vector<SymbolType> &types) {
  procedureDeclarations.insert({signature, types});
}

/**
 * Retrieve an item from the list of procedure declarations.
 *
 * @param signature Signature of the desired procedure declaration
 * @return List of parameter types of the desired procedure declaration
 */
std::vector<SymbolType> SymbolTable::getProcedureDeclaration(const std::string &signature) const {
  if (procedureDeclarations.find(signature) == procedureDeclarations.end())
    return {};
  return procedureDeclarations.at(signature);
}

/**
 * Changes a specific type to another in the whole sub-table
 * ToDo: Currently unused. Maybe remove in the future
 *
 * @param oldType Old symbol type
 * @param newType Replacement type
 */
void SymbolTable::updateSymbolTypes(ErrorFactory *err, const antlr4::Token &token, const SymbolType &oldType,
                                    const SymbolType &newType) {
  // Update types in the symbol list
  for (auto &[key, symbol] : symbols) {
    SymbolType currentType = symbol.getType();
    std::vector<SymbolSuperType> ptrArrayList;
    while (currentType.isOneOf({TY_PTR, TY_ARRAY})) {
      if (currentType.isPointer())
        ptrArrayList.push_back(TY_PTR);
      else
        ptrArrayList.push_back(TY_ARRAY);
      currentType = currentType.getContainedTy();
    }
    if (currentType == oldType) {
      std::reverse(ptrArrayList.begin(), ptrArrayList.end());
      SymbolType currentNewType = newType;
      for (auto it = ptrArrayList.rbegin(); it != ptrArrayList.rend(); ++it) {
        if (*it == TY_PTR)
          currentNewType = currentNewType.toPointer(err, token);
        else
          currentNewType = currentNewType.toArray(err, token);
      }
      symbol.updateType(currentNewType, true);
    }
  }
  // Update function declarations
  for (auto &[functionSignature, types] : functionDeclarations) {
    for (auto &i : types) {
      if (i == oldType)
        i = newType;
    }
  }
  // Visit all child tables
  for (auto &[key, child] : children)
    child.updateSymbolTypes(err, token, oldType, newType);
}

/**
 * Push a function/procedure signature to a queue of function/procedure signatures. This is used to push the signatures
 * of function/procedure definitions and calls in the semantic analysis
 *
 * @param signature Signature of the function/procedure
 */
void SymbolTable::pushSignature(const FunctionSignature &signature) { functionSignatures.push(signature); }

/**
 * Pop a function/procedure signature from a queue of function/procedure signatures. This is used to pop the signatures
 * of function/procedure definitions and calls in the generator component
 *
 * @return Signature of the function/procedure
 */
FunctionSignature SymbolTable::popSignature() {
  assert(!functionSignatures.empty());
  auto signature = functionSignatures.front();
  functionSignatures.pop();
  return signature;
}

/**
 * Prints compiler values with regard to the symbol table
 */
void SymbolTable::printCompilerWarnings() {
  // Omit this table if it is an imported sub-table
  if (imported)
    return;
  // Visit own symbols
  for (auto &[key, entry] : symbols) {
    if (!entry.isUsed()) {
      if (entry.getType().is(TY_FUNCTION)) {
        CompilerWarning(entry.getDefinitionToken(), UNUSED_FUNCTION, "The function '" + entry.getName() + "' is unused").print();
      } else if (entry.getType().is(TY_PROCEDURE)) {
        CompilerWarning(entry.getDefinitionToken(), UNUSED_PROCEDURE, "The procedure '" + entry.getName() + "' is unused")
            .print();
      } else if (entry.getType().is(TY_STRUCT) || entry.getType().isPointerOf(TY_STRUCT)) {
        CompilerWarning(entry.getDefinitionToken(), UNUSED_STRUCT, "The struct '" + entry.getName() + "' is unused").print();
      } else if (entry.getType().isOneOf({TY_IMPORT})) {
        CompilerWarning(entry.getDefinitionToken(), UNUSED_IMPORT, "The import '" + entry.getName() + "' is unused").print();
      } else {
        if (entry.getName() != UNUSED_VARIABLE_NAME && entry.getName() != FOREACH_DEFAULT_IDX_VARIABLE_NAME)
          CompilerWarning(entry.getDefinitionToken(), UNUSED_VARIABLE, "The variable '" + entry.getName() + "' is unused")
              .print();
      }
    }
  }
  // Visit children
  for (auto &[key, child] : children)
    child.printCompilerWarnings();
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
nlohmann::json SymbolTable::toJSON() const {
  // Collect all symbols
  std::vector<nlohmann::json> jsonSymbols;
  jsonSymbols.reserve(symbols.size());
  for (auto &symbol : symbols)
    jsonSymbols.emplace_back(symbol.second.toJSON());

  // Collect all captures
  std::vector<nlohmann::json> jsonCaptures;
  jsonCaptures.reserve(captures.size());
  for (auto &capture : captures)
    jsonCaptures.emplace_back(capture.second.toJSON());

  // Collect all children
  std::vector<nlohmann::json> jsonChildren;
  jsonChildren.reserve(symbols.size());
  for (auto &child : children) {
    nlohmann::json c = child.second.toJSON();
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

/**
 * Marks this symbol table as imported. This means, that it is a nested symbol table in the main symbol table
 */
void SymbolTable::setImported() { imported = true; }

/**
 * Checks if this symbol table is imported
 *
 * @return Imported / not imported
 */
bool SymbolTable::isImported() const { return imported; }

/**
 * Mark this scope so that the compiler knows that accessing variables from outside within the scope requires capturing
 */
void SymbolTable::setCapturingRequired() { requiresCapturing = true; }
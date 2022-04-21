// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "symbol/SymbolTable.h"
#include "GenericType.h"
#include "util/FileUtil.h"

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
 * @param isArg Enabled if the symbol is a function/procedure parameter
 */
void SymbolTable::insert(const std::string &name, const SymbolType &type, SymbolSpecifiers specifiers, SymbolState state,
                         const antlr4::Token &token) {
  bool isGlobal = parent == nullptr;
  unsigned int orderIndex = symbols.size();
  // Insert into symbols map
  symbols.insert({name, SymbolTableEntry(name, type, specifiers, state, token, orderIndex, isGlobal)});
}

/**
 * Add a capture to the capture list manually
 *
 * @param capture
 */
void SymbolTable::addCapture(const std::string &name, const Capture &capture) { captures.insert({name, capture}); }

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
    if (requiresCapturing && !captures.contains(name) && !entry->getType().isOneOf({TY_IMPORT, TY_FUNCTION, TY_PROCEDURE}))
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
  // Check if a symbol with this name exists in this scope
  if (symbols.contains(name))
    return &symbols.at(name);
  // Check if a capture with this name exists in this scope
  if (captures.contains(name))
    return captures.at(name).getEntry();
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
SymbolTableEntry *SymbolTable::lookupGlobal(const std::string &globalName, bool skipThisScope) {
  // Search in the current scope
  if (!skipThisScope) {
    SymbolTableEntry *globalSymbol = lookupStrict(globalName);
    if (globalSymbol)
      return globalSymbol;
  }
  // Loop through all children to find the global var
  for (const auto &[scopeName, childScope] : children) {
    if (childScope->isImported()) { // Only consider if it is an imported module scope
      SymbolTableEntry *globalSymbol = childScope->lookupGlobal(globalName);
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
  // If available in the current scope, return it
  if (captures.contains(name))
    return &captures.at(name);
  // Otherwise, return nullptr
  return nullptr;
}

/**
 * Search for a symbol table by its name, where a function is defined. Used for function calls to function/procedures
 * which were linked in from other modules
 *
 * @param scopeId Scope ID of the desired symbol table
 * @return Desired symbol table
 */
SymbolTable *SymbolTable::lookupTable(const std::string &tableName) {
  // If not available in the current scope, search in the parent scope
  if (!children.contains(tableName))
    return parent ? parent->lookupTable(tableName) : nullptr;
  // Otherwise, return the entry
  return children.at(tableName);
}

/**
 * Create a child leaf for the tree of symbol tables and return it
 *
 * @param childBlockName Name of the child scope
 * @return Newly created child table
 */
SymbolTable *SymbolTable::createChildBlock(const std::string &childBlockName) {
  children.insert({childBlockName, new SymbolTable(this, isMainSourceFile)});
  return children.at(childBlockName);
}

/**
 * Insert a new generic type in this scope
 *
 * @param typeName Name of the generic type
 * @param genericType Generic type itself
 */
void SymbolTable::insertGenericType(const std::string &typeName, GenericType &genericType) {
  genericTypes.insert({typeName, genericType});
}

/**
 * Search for a generic type by its name. If it was not found, the parent scopes will be searched.
 * If the generic type does not exist at all, the function will return a nullptr.
 *
 * @param typeName Name of the generic type
 * @return Generic type
 */
GenericType *SymbolTable::lookupGenericType(const std::string &typeName) {
  if (genericTypes.contains(typeName))
    return &genericTypes.at(typeName);
  return parent ? parent->lookupGenericType(typeName) : nullptr;
}

/**
 * Mount in symbol tables manually. This is used to hook in symbol tables of imported modules into the symbol table of
 * the source file, which imported the modules
 *
 * @param childBlockName Name of the child block
 * @param childBlock Child symbol table
 */
void SymbolTable::mountChildBlock(const std::string &childBlockName, SymbolTable *childBlock, bool alterParent) {
  if (alterParent)
    childBlock->parent = this;
  children.insert({childBlockName, childBlock});
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
 * Duplicates the child block entry, but it points to the same child block
 *
 * @param originalChildBlockName Original name of the child block
 * @param newChildBlockName New name
 */
void SymbolTable::duplicateChildBlockEntry(const std::string &originalChildBlockName, const std::string &newChildBlockName) {
  SymbolTable *childBlock = children.at(originalChildBlockName);
  assert(childBlock != nullptr);
  children.insert({newChildBlockName, childBlock});
}

/**
 * Set the parent table of the current one in the tree structure
 */
void SymbolTable::setParent(SymbolTable *parent) { this->parent = parent; }

/**
 * Retrieve the parent table of the current one in the tree structure
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
  if (children.empty() || !children.contains(scopeId))
    return nullptr;
  return children.at(scopeId);
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
  for (const auto &[key, symbol] : symbols) {
    if (!symbol.getType().isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_IMPORT}))
      count++;
  }
  return count;
}

/**
 * Insert a function object into this symbol table scope
 */
void SymbolTable::insertFunction(const Function &function, ErrorFactory *err, const antlr4::Token &token) {
  // Open a new function declaration pointer list. Which gets filled by the 'insertSubstantiatedFunction' method
  functions.insert({FileUtil::tokenToCodeLoc(token), std::make_shared<std::map<std::string, Function>>()});

  // Check if function is already substantiated
  if (function.hasSubstantiatedArgs()) {
    insertSubstantiatedFunction(function, err, token, FileUtil::tokenToCodeLoc(token));
    return;
  }

  // Substantiate the function and insert the substantiated instances
  for (const auto &fct : function.substantiateOptionalArgs())
    insertSubstantiatedFunction(fct, err, token, FileUtil::tokenToCodeLoc(token));
}

/**
 * Check if there is a function in this scope, fulfilling all given requirements and if found, return it.
 * If more than one function matches the requirement, an error gets thrown
 *
 * @param functionName Function name requirement
 * @param thisType This type requirement
 * @param argTypes Argument types requirement
 * @param templateTypes Template types requirement
 * @param err Error Factory
 * @param token Definition token for the error message
 * @return Matched function or nullptr
 */
Function *SymbolTable::matchFunction(const std::string &functionName, const SymbolType &thisType,
                                     const std::vector<SymbolType> &argTypes, const std::vector<SymbolType> &templateTypes,
                                     ErrorFactory *err, const antlr4::Token &token) {
  std::vector<Function *> matches;

  // Loop through function and add any matches to the matches vector
  auto oldFunctionsList = functions;
  for (const auto &[codeLoc, manifestations] : oldFunctionsList) {
    auto oldManifestations = *manifestations;
    for (auto &[mangledName, f] : oldManifestations) {
      // Check name requirement
      if (f.getName() != functionName)
        continue;

      // Check this type requirement
      if (f.getThisType() != thisType)
        continue;

      // Check arg types requirement
      std::vector<SymbolType> curArgTypes = f.getArgTypes();
      if (curArgTypes.size() != argTypes.size())
        continue;
      bool differentArgTypes = false; // Note: This is a workaround for a break from an inner loop
      for (int i = 0; i < argTypes.size(); i++) {
        if (!curArgTypes[i].is(TY_GENERIC) && !equalsIgnoreArraySizes(curArgTypes[i], argTypes[i])) {
          differentArgTypes = true;
          break;
        }
      }
      if (differentArgTypes)
        continue;

      // Check template types requirement
      std::vector<GenericType> curTemplateTypes = f.getTemplateTypes();
      if (curTemplateTypes.empty()) {
        // It's a match!
        matches.push_back(&functions.at(codeLoc)->at(f.getMangledName()));
      } else {
        if (curTemplateTypes.size() != templateTypes.size())
          continue;
        std::vector<SymbolType> concreteTemplateTypes;
        std::vector<GenericTypeReplacement> typeReplacements;
        bool differentTemplateTypes = false; // Note: This is a workaround for a break from an inner loop
        for (int i = 0; i < templateTypes.size(); i++) {
          GenericType curTemplateType = curTemplateTypes[i];
          SymbolType templateType = templateTypes[i];

          // Unpack the types
          std::stack<std::pair<SymbolSuperType, std::string>> ptrArrayLst;
          while ((curTemplateType.isPointer() && templateType.isPointer()) ||
                 (curTemplateType.isArray() && templateType.isArray())) {
            ptrArrayLst.emplace(templateType.getSuperType(), templateType.getSubType());
            curTemplateType = curTemplateType.getContainedTy();
            templateType = templateType.getContainedTy();
          }

          // Check if the types meet the conditions
          if (!curTemplateType.meetsConditions(templateType)) {
            differentTemplateTypes = true;
            break;
          }

          // Add replacement to the type replacements list
          typeReplacements.emplace_back(curTemplateType.getSubType(), templateType);

          // Pack the types again
          for (const auto &[superType, subType] : ptrArrayLst) {
            if (superType == TY_PTR)
              templateType.toPointer(err, token);
            else
              templateType.toArray(err, token, );
          }

          // Add to concrete template types
          concreteTemplateTypes.push_back(templateType);
        }
        if (differentTemplateTypes)
          continue;

        // Duplicate function
        Function newFunction = f.substantiateGenerics(concreteTemplateTypes);
        insertSubstantiatedFunction(newFunction, err, token, f.getDefinitionCodeLoc());
        duplicateChildBlockEntry(f.getSignature(), newFunction.getSignature());

        matches.push_back(&functions.at(codeLoc)->at(newFunction.getMangledName()));
      }
    }
  }

  if (matches.empty())
    return nullptr;

  // Throw error if more than one function matches the criteria
  if (matches.size() > 1)
    throw err->get(
        token, FUNCTION_AMBIGUITY,
        "More than one function matches your requested signature criteria. Please try to specify the return type explicitly");

  // Add function access pointer for function call
  functionAccessPointers.push(matches[0]);

  return matches[0];
}

/**
 * Retrieve the manifestations of the function, defined at defToken
 *
 * @return Function manifestations
 */
std::shared_ptr<std::map<std::string, Function>> SymbolTable::getFunctionManifestations(const antlr4::Token &defToken) const {
  std::string accessId = FileUtil::tokenToCodeLoc(defToken);
  if (!functions.contains(accessId))
    throw std::runtime_error("Internal compiler error: Cannot get function manifestations at " + accessId);
  return functions.at(accessId);
}

/**
 * Get the next function access in order of visiting
 *
 * @return Function pointer for the function access
 */
Function *SymbolTable::popFunctionAccessPointer() {
  if (functionAccessPointers.empty())
    throw std::runtime_error("Internal compiler error: Could not pop function access");
  Function *function = functionAccessPointers.front();
  functionAccessPointers.pop();
  return function;
}

/**
 * Insert a substantiated function into the function list. If the list already contains a function with the same signature,
 * an exception will be thrown
 *
 * @param function Substantiated function
 * @param err Error factory
 * @param token Token, where the function is declared
 */
void SymbolTable::insertSubstantiatedFunction(const Function &function, ErrorFactory *err, const antlr4::Token &token,
                                              const std::string &codeLoc) {
  if (!function.hasSubstantiatedArgs())
    throw std::runtime_error("Internal compiler error: Expected substantiated function");

  // Check if the function exists already
  for (const auto &[_, manifestations] : functions) {
    if (manifestations->contains(function.getMangledName()))
      throw err->get(token, FUNCTION_DECLARED_TWICE,
                     "The function/procedure '" + function.getSignature() + "' is declared twice");
  }
  // Add function to function list
  functions.at(codeLoc)->insert({function.getMangledName(), function});
  // Add symbol table entry for the function
  insert(function.getSignature(), function.getSymbolType(), function.getSpecifiers(), INITIALIZED, token);
}

/**
 * Insert a struct object into this symbol table scope
 */
void SymbolTable::insertStruct(const Struct &s, ErrorFactory *err, const antlr4::Token &token) {
  // Open a new function declaration pointer list. Which gets filled by the 'insertSubstantiatedFunction' method
  structs.insert({FileUtil::tokenToCodeLoc(token), std::make_shared<std::map<std::string, Struct>>()});
  insertSubstantiatedStruct(s, err, token, FileUtil::tokenToCodeLoc(token));
}

/**
 * Check if there is a struct in this scope, fulfilling all given requirements and if found, return it.
 * If more than one struct matches the requirement, an error gets thrown
 *
 * @param structName Struct name
 * @param templateTypes Template type requirements
 * @param errorFactory Error factory
 * @param token Definition token for the error message
 * @return Matched struct or nullptr
 */
Struct *SymbolTable::matchStruct(const std::string &structName, const std::vector<SymbolType> &templateTypes, ErrorFactory *err,
                                 const antlr4::Token &token) {
  std::vector<Struct *> matches;

  // Loop through function and add any matches to the matches vector
  auto oldStructList = structs;
  for (const auto &[codeLoc, manifestations] : oldStructList) {
    auto oldManifestations = *manifestations;
    for (auto &[mangledName, s] : oldManifestations) {
      // Check name requirement
      if (s.getName() != structName)
        continue;

      // Check template types requirement
      std::vector<GenericType> curTemplateTypes = s.getTemplateTypes();
      if (curTemplateTypes.empty()) {
        // It's a match!
        matches.push_back(&structs.at(codeLoc)->at(s.getMangledName()));
      } else {
        if (curTemplateTypes.size() != templateTypes.size())
          continue;
        std::vector<SymbolType> concreteTemplateTypes;
        std::vector<GenericTypeReplacement> typeReplacements;
        bool differentTemplateTypes = false; // Note: This is a workaround for a break from an inner loop
        for (int i = 0; i < templateTypes.size(); i++) {
          if (!curTemplateTypes[i].meetsConditions(templateTypes[i])) {
            differentTemplateTypes = true;
            break;
          }
          concreteTemplateTypes.push_back(templateTypes[i]);
          typeReplacements.emplace_back(curTemplateTypes[i].getSubType(), templateTypes[i]);
        }
        if (differentTemplateTypes)
          continue;

        // Duplicate function
        Struct newStruct = s.substantiateGenerics(concreteTemplateTypes, this, err, token);
        insertSubstantiatedStruct(newStruct, err, token, s.getDefinitionCodeLoc());
        duplicateChildBlockEntry(STRUCT_SCOPE_PREFIX + newStruct.getName(), STRUCT_SCOPE_PREFIX + newStruct.getSignature());

        matches.push_back(&structs.at(codeLoc)->at(newStruct.getMangledName()));
      }
    }
  }

  if (matches.empty())
    return nullptr;

  // Throw error if more than one function matches the criteria
  if (matches.size() > 1)
    throw err->get(
        token, STRUCT_AMBIGUITY,
        "More than one struct matches your requested signature criteria. Please try to specify the return type explicitly");

  // Add function access pointer for function call
  structAccessPointers.push(matches[0]);

  return matches[0];
}

/**
 * Retrieve the manifestations of the struct, defined at defToken
 *
 * @return Struct manifestations
 */
std::shared_ptr<std::map<std::string, Struct>> SymbolTable::getStructManifestations(const antlr4::Token &defToken) const {
  std::string accessId = FileUtil::tokenToCodeLoc(defToken);
  if (!structs.contains(accessId))
    throw std::runtime_error("Internal compiler error: Cannot get struct manifestations at " + accessId);
  return structs.at(accessId);
}

/**
 * Get the next struct access in order of visiting
 *
 * @return Struct pointer for the struct access
 */
Struct *SymbolTable::popStructAccessPointer() {
  if (structAccessPointers.empty())
    throw std::runtime_error("Internal compiler error: Could not pop struct access");
  Struct *s = structAccessPointers.front();
  structAccessPointers.pop();
  return s;
}

/**
 * Mark this scope so that the compiler knows that accessing variables from outside within the scope requires capturing
 */
void SymbolTable::setCapturingRequired() { requiresCapturing = true; }

/**
 * Insert a substantiated struct into the struct list. If the list already contains a struct with the same signature,
 * an exception will be thrown
 *
 * @param s Substantiated struct
 * @param err Error factory
 * @param token Token, where the struct is declared
 */
void SymbolTable::insertSubstantiatedStruct(const Struct &s, ErrorFactory *err, const antlr4::Token &token,
                                            const std::string &codeLoc) {
  // Check if the struct exists already
  for (const auto &[_, manifestations] : structs) {
    if (manifestations->contains(s.getMangledName()))
      throw err->get(token, STRUCT_DECLARED_TWICE, "The struct '" + s.getSignature() + "' is declared twice");
  }
  // Add struct to struct list
  structs.at(codeLoc)->insert({s.getMangledName(), s});
  // Add symbol table entry for the struct
  insert(s.getSignature(), s.getSymbolType(), s.getSpecifiers(), INITIALIZED, token);
}

/**
 * Prints compiler values with regard to the symbol table
 */
void SymbolTable::printCompilerWarnings() {
  // Omit this table if it is an imported sub-table
  if (compilerWarningsEnabled)
    return;
  // Visit own symbols
  for (const auto &[key, entry] : symbols) {
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
  for (const auto &[key, child] : children)
    child->printCompilerWarnings();
}

/**
 * Disable compiler warnings for this table and all sub-tables
 */
void SymbolTable::disableCompilerWarnings() {
  for (auto &child : children)
    child.second->disableCompilerWarnings();
  compilerWarningsEnabled = false;
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
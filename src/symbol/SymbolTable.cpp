// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "symbol/SymbolTable.h"

#include <stdexcept>
#include <utility>

#include <analyzer/AnalyzerVisitor.h>
#include <exception/ErrorFactory.h>
#include <symbol/Function.h>
#include <symbol/GenericType.h>
#include <symbol/Struct.h>
#include <util/CodeLoc.h>
#include <util/CommonUtil.h>
#include <util/CompilerWarning.h>

/**
 * Insert a new symbol into the current symbol table. If it is a parameter, append its name to the paramNames vector
 *
 * @param name Name of the symbol
 * @param type Type of the symbol
 * @param specifiers Specifiers of the symbol
 * @param state State of the symbol (declared or initialized)
 * @param codeLoc Code location where the symbol is declared
 */
void SymbolTable::insert(const std::string &name, const SymbolType &type, SymbolSpecifiers specifiers, SymbolState state,
                         const CodeLoc &declCodeLoc) {
  bool isGlobal = parent == nullptr;
  unsigned int orderIndex = symbols.size();
  // Insert into symbols map
  symbols.insert({name, SymbolTableEntry(name, type, this, specifiers, state, declCodeLoc, orderIndex, isGlobal)});
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
  if (name.empty())
    return nullptr;
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
    if (childScope->isImported(this)) { // Only consider if it is an imported module scope
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
SymbolTable *SymbolTable::createChildBlock(const std::string &childBlockName, const ScopeType &scopeType) {
  children.insert({childBlockName, new SymbolTable(this, scopeType, isMainSourceFile)});
  return children.at(childBlockName);
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
 * Duplicates the child block by copying it. The duplicated symbols point to the original ones.
 *
 * @param originalChildBlockName Original name of the child block
 * @param newChildBlockName New block name
 */
void SymbolTable::copyChildBlock(const std::string &originalChildBlockName, const std::string &newChildBlockName) {
  assert(children.contains(originalChildBlockName));
  SymbolTable *originalChildBlock = children.at(originalChildBlockName);
  // Copy child block
  auto newChildBlock = new SymbolTable(*originalChildBlock);
  // Save the new child block
  children.insert({newChildBlockName, newChildBlock});
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
 * Retrieve all variables that can be freed, because the ref count went down to 0.
 *
 * @return Variables that can be de-allocated
 */
std::vector<SymbolTableEntry *> SymbolTable::getVarsGoingOutOfScope(bool filterForStructs) {
  assert(parent != nullptr); // Should not be called in root scope

  // Collect all variables
  std::vector<SymbolTableEntry *> varsGoingOutOfScope;
  varsGoingOutOfScope.reserve(symbols.size());
  for (auto [name, entry] : symbols) {
    if (name == THIS_VARIABLE_NAME)
      continue;
    if (!filterForStructs || entry.getType().is(TY_STRUCT))
      varsGoingOutOfScope.push_back(&symbols.at(name));
  }
  return varsGoingOutOfScope;
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
 * Insert a function object into this symbol table scope
 *
 * @param function Function object
 * @param err Error factory
 */
void SymbolTable::insertFunction(const Function &function, ErrorFactory *err) {
  const CodeLoc &codeLoc = function.getDeclCodeLoc();

  // Open a new function declaration pointer list. Which gets filled by the 'insertSubstantiatedFunction' method
  std::string codeLocStr = codeLoc.toString();
  functions.insert({codeLocStr, std::make_shared<std::map<std::string, Function>>()});

  // Check if function is already substantiated
  if (function.hasSubstantiatedArgs()) {
    insertSubstantiatedFunction(function, err, codeLoc);
    return;
  }

  // Substantiate the function and insert the substantiated instances
  for (const auto &fct : function.substantiateOptionalArgs())
    insertSubstantiatedFunction(fct, err, codeLoc);
}

/**
 * Check if there is a function in this scope, fulfilling all given requirements and if found, return it.
 * If more than one function matches the requirement, an error gets thrown
 *
 * @param currentScope Current scope
 * @param callFunctionName Function name requirement
 * @param callThisType This type requirement
 * @param callArgTypes Argument types requirement
 * @param err Error Factory
 * @param codeLoc Declaration code location for the error message
 * @return Matched function or nullptr
 */
Function *SymbolTable::matchFunction(SymbolTable *currentScope, const std::string &callFunctionName,
                                     const SymbolType &callThisType, const std::vector<SymbolType> &callArgTypes,
                                     ErrorFactory *err, const CodeLoc &codeLoc) {
  std::vector<Function *> matches;

  // Loop through functions and add any matches to the matches vector
  auto oldFunctionsList = functions;
  for (const auto &[defCodeLocStr, manifestations] : oldFunctionsList) {
    auto oldManifestations = *manifestations;
    for (auto &[mangledName, f] : oldManifestations) {
      // Check name requirement
      if (f.getName() != callFunctionName)
        continue;

      // Initialize mapping from generic type name to concrete symbol type
      std::map<std::string, SymbolType> concreteGenericTypes;

      // Check 'this' type requirement
      SymbolType fctThisType = f.getThisType();
      if (fctThisType.getTemplateTypes().empty()) { // The 'this' type is a non-generic struct
        if (callThisType != fctThisType)
          continue;
      } else { // The 'this' type is a generic struct
        for (int i = 0; i < fctThisType.getTemplateTypes().size(); i++) {
          SymbolType genericType = fctThisType.getTemplateTypes()[i];
          SymbolType concreteGenericType = callThisType.getTemplateTypes()[i];
          concreteGenericTypes.insert({genericType.getSubType(), concreteGenericType});
        }
      }

      // Check arg types requirement
      auto argList = f.getArgList();
      if (argList.size() != callArgTypes.size())
        continue;
      bool differentArgTypes = false; // Note: This is a workaround for a break from an inner loop
      for (int i = 0; i < argList.size(); i++) {
        if (argList[i].first.isBaseType(TY_GENERIC)) { // Resolve concrete type for arguments with generic type
          std::string genericTypeName = argList[i].first.getBaseType().getSubType();
          if (concreteGenericTypes.contains(genericTypeName)) { // This generic type was already resolved => check if it matches
            if (concreteGenericTypes.at(genericTypeName) != callArgTypes[i].getBaseType()) {
              differentArgTypes = true;
              break;
            }
          } else { // This generic type was not resolved yet => resolve it and add it to the list
            // De-assemble the concrete type to match the generic type
            SymbolType curGenericType = argList[i].first;
            SymbolType curArgType = callArgTypes[i];
            while (curGenericType.isArray() || curGenericType.isPointer()) {
              if ((curGenericType.isArray() && curArgType.isArray()) || (curGenericType.isPointer() && curArgType.isPointer())) {
                curArgType = curArgType.getContainedTy();
                curGenericType = curGenericType.getContainedTy();
              } else {
                break;
              }
            }

            if (!lookupGenericType(genericTypeName)->checkConditionsOf(curArgType)) {
              differentArgTypes = true;
              break;
            }
            concreteGenericTypes.insert({genericTypeName, curArgType});
          }
          argList[i].first = argList[i].first.replaceBaseType(concreteGenericTypes.at(genericTypeName));
        } else { // For arguments with non-generic type, check if the candidate type matches with the call
          if (!equalsIgnoreArraySizes(argList[i].first, callArgTypes[i])) {
            differentArgTypes = true;
            break;
          }
        }
      }
      if (differentArgTypes)
        continue;

      // Duplicate function
      Function newFunction = f.substantiateGenerics(argList, callThisType, concreteGenericTypes);
      if (!getChild(newFunction.getSignature())) { // Insert function
        insertSubstantiatedFunction(newFunction, err, f.getDeclCodeLoc());
        copyChildBlock(f.getSignature(), newFunction.getSignature());

        // Insert symbols for generic type names with concrete types into the child block
        SymbolTable *childBlock = getChild(newFunction.getSignature());
        for (const auto &[typeName, symbolType] : concreteGenericTypes)
          childBlock->insertGenericType(typeName, GenericType(symbolType));

        // Replace this type with concrete one
        if ((f.isMethodFunction() || f.isMethodProcedure()) && !fctThisType.getTemplateTypes().empty()) {
          SymbolTableEntry *thisEntry = childBlock->lookup(THIS_VARIABLE_NAME);
          assert(thisEntry != nullptr);
          thisEntry->updateType(callThisType.toPointer(err, codeLoc), true);
        }
      }

      assert(functions.contains(defCodeLocStr) && functions.at(defCodeLocStr)->contains(newFunction.getMangledName()));
      matches.push_back(&functions.at(defCodeLocStr)->at(newFunction.getMangledName()));
      break;
    }
  }

  if (matches.empty())
    return nullptr;

  // Throw error if more than one function matches the criteria
  if (matches.size() > 1)
    throw err->get(
        codeLoc, FUNCTION_AMBIGUITY,
        "More than one function matches your requested signature criteria. Please try to specify the return type explicitly");

  // Add function access pointer for function call
  if (currentScope != nullptr)
    currentScope->insertFunctionAccessPointer(codeLoc, matches.front());

  return matches.front();
}

/**
 * Retrieve the manifestations of the function, defined at defToken
 *
 * @param defCodeLoc Definition code location
 *
 * @return Function manifestations
 */
std::map<std::string, Function> *SymbolTable::getFunctionManifestations(const CodeLoc &defCodeLoc) const {
  std::string codeLocStr = defCodeLoc.toString();
  return functions.contains(codeLocStr) ? functions.at(codeLocStr).get() : nullptr;
}

/**
 * Add function access pointer to the current scope
 *
 * @param codeLoc Call code location
 * @param spiceFunc Function
 */
void SymbolTable::insertFunctionAccessPointer(const CodeLoc &codeLoc, Function *spiceFunc) {
  functionAccessPointers.insert({codeLoc.toString(), spiceFunc});
}

/**
 * Get the function access pointer by code location
 *
 * @param codeLoc Code location
 *
 * @return Function pointer for the function access
 */
Function *SymbolTable::getFunctionAccessPointer(const CodeLoc &codeLoc) {
  std::string codeLocStr = codeLoc.toString();
  if (!functionAccessPointers.contains(codeLocStr))
    return nullptr;
  return functionAccessPointers.at(codeLocStr);
}

/**
 * Insert a substantiated function into the function list. If the list already contains a function with the same signature,
 * an exception will be thrown
 *
 * @param function Substantiated function
 * @param err Error factory
 * @param codeLoc Code location
 */
void SymbolTable::insertSubstantiatedFunction(const Function &function, ErrorFactory *err, const CodeLoc &codeLoc) {
  if (!function.hasSubstantiatedArgs())
    throw std::runtime_error("Internal compiler error: Expected substantiated function");

  // Check if the function exists already
  std::string mangledFctName = function.getMangledName();
  for (const auto &[_, manifestations] : functions) {
    if (manifestations->contains(mangledFctName))
      throw err->get(codeLoc, FUNCTION_DECLARED_TWICE,
                     "The function/procedure '" + function.getSignature() + "' is declared twice");
  }
  // Add function to function list
  assert(functions.contains(codeLoc.toString()));
  functions.at(codeLoc.toString())->insert({mangledFctName, function});
  // Add symbol table entry for the function
  insert(function.getSignature(), function.getSymbolType(), function.getSpecifiers(), INITIALIZED, codeLoc);
}

/**
 * Insert a struct object into this symbol table scope
 *
 * @param s Struct object
 * @param err Error factory
 */
void SymbolTable::insertStruct(const Struct &s, ErrorFactory *err) {
  // Open a new function declaration pointer list. Which gets filled by the 'insertSubstantiatedFunction' method
  const CodeLoc &codeLoc = s.getDeclCodeLoc();
  std::string codeLocStr = codeLoc.toString();
  structs.insert({codeLocStr, std::make_shared<std::map<std::string, Struct>>()});
  insertSubstantiatedStruct(s, err, codeLoc);
}

/**
 * Check if there is a struct in this scope, fulfilling all given requirements and if found, return it.
 * If more than one struct matches the requirement, an error gets thrown
 *
 * @param currentScope Current scope
 * @param structName Struct name
 * @param templateTypes Template type requirements
 * @param errorFactory Error factory
 * @param codeLoc Declaration code location for the error message
 * @return Matched struct or nullptr
 */
Struct *SymbolTable::matchStruct(SymbolTable *currentScope, const std::string &structName,
                                 const std::vector<SymbolType> &templateTypes, ErrorFactory *err, const CodeLoc &codeLoc) {
  std::vector<Struct *> matches;

  // Loop through structs and add any matches to the matches vector
  auto oldStructList = structs;
  for (const auto &[defCodeLocStr, manifestations] : oldStructList) {
    auto oldManifestations = *manifestations;
    for (auto &[mangledName, s] : oldManifestations) {
      // Check name requirement
      if (s.getName() != structName)
        continue;

      // Check template types requirement
      std::vector<GenericType> curTemplateTypes = s.getTemplateTypes();
      if (curTemplateTypes.empty()) {
        // It's a match!
        matches.push_back(&structs.at(defCodeLocStr)->at(s.getMangledName()));
      } else {
        if (curTemplateTypes.size() != templateTypes.size())
          continue;
        std::vector<SymbolType> concreteTemplateTypes;
        bool differentTemplateTypes = false; // Note: This is a workaround for a break from an inner loop
        for (int i = 0; i < templateTypes.size(); i++) {
          if (!curTemplateTypes[i].checkConditionsOf(templateTypes[i])) {
            differentTemplateTypes = true;
            break;
          }
          concreteTemplateTypes.push_back(templateTypes[i]);
        }
        if (differentTemplateTypes)
          continue;

        // Duplicate function
        SymbolTable *structScope = getChild(STRUCT_SCOPE_PREFIX + structName);
        Struct newStruct = s.substantiateGenerics(concreteTemplateTypes, structScope);
        if (!getChild(STRUCT_SCOPE_PREFIX + newStruct.getSignature())) { // Insert struct
          insertSubstantiatedStruct(newStruct, err, s.getDeclCodeLoc());
          copyChildBlock(STRUCT_SCOPE_PREFIX + structName, STRUCT_SCOPE_PREFIX + newStruct.getSignature());
        }

        assert(structs.contains(defCodeLocStr) && structs.at(defCodeLocStr)->contains(newStruct.getMangledName()));
        matches.push_back(&structs.at(defCodeLocStr)->at(newStruct.getMangledName()));
        break;
      }
    }
  }

  if (matches.empty() && parent)
    matches.push_back(parent->matchStruct(currentScope, structName, templateTypes, err, codeLoc));

  if (matches.empty())
    return nullptr;

  // Throw error if more than one struct matches the criteria
  if (matches.size() > 1)
    throw err->get(
        codeLoc, STRUCT_AMBIGUITY,
        "More than one struct matches your requested signature criteria. Please try to specify the return type explicitly");

  // Add struct access pointer for struct reference
  if (currentScope != nullptr)
    currentScope->insertStructAccessPointer(codeLoc, matches.front());

  return matches.front();
}

/**
 * Retrieve the manifestations of the struct, defined at defToken
 *
 * @return Struct manifestations
 */
std::map<std::string, Struct> *SymbolTable::getStructManifestations(const CodeLoc &defCodeLoc) const {
  std::string codeLocStr = defCodeLoc.toString();
  if (!structs.contains(codeLocStr))
    throw std::runtime_error("Internal compiler error: Cannot get struct manifestations at " + codeLocStr);
  return structs.at(codeLocStr).get();
}

/**
 * Add struct access pointer to the current scope
 *
 * @param token Reference token
 * @param struct Struct
 */
void SymbolTable::insertStructAccessPointer(const CodeLoc &codeLoc, Struct *spiceStruct) {
  structAccessPointers.insert({codeLoc.toString(), spiceStruct});
}

/**
 * Get the next struct access in order of visiting
 *
 * @return Struct pointer for the struct access
 */
Struct *SymbolTable::getStructAccessPointer(const CodeLoc &codeLoc) {
  std::string codeLocStr = codeLoc.toString();
  if (!structAccessPointers.contains(codeLocStr))
    throw std::runtime_error("Internal compiler error: Could not get struct access pointer");
  return structAccessPointers.at(codeLocStr);
}

/**
 * Insert a substantiated struct into the struct list. If the list already contains a struct with the same signature,
 * an exception will be thrown
 *
 * @param s Substantiated struct
 * @param err Error factory
 * @param token Token, where the struct is declared
 * @param codeLoc Code location
 */
void SymbolTable::insertSubstantiatedStruct(const Struct &s, ErrorFactory *err, const CodeLoc &codeLoc) {
  // Check if the struct exists already
  for (const auto &[_, manifestations] : structs) {
    if (manifestations->contains(s.getMangledName()))
      throw err->get(codeLoc, STRUCT_DECLARED_TWICE, "The struct '" + s.getSignature() + "' is declared twice");
  }
  // Add struct to struct list
  assert(structs.at(codeLoc.toString()) != nullptr);
  structs.at(codeLoc.toString())->insert({s.getMangledName(), s});
  // Add symbol table entry for the struct
  insert(s.getSignature(), s.getSymbolType(), s.getSpecifiers(), INITIALIZED, codeLoc);
}

/**
 * Purge all non-substantiated manifestations of functions and structs
 */
void SymbolTable::purgeSubstantiationRemnants() {
  // Prune non-substantiated functions
  std::erase_if(functions, [&](const auto &kvOuter) {
    std::erase_if(*kvOuter.second, [&](const auto &kvInner) {
      if (!kvInner.second.isFullySubstantiated()) {
        children.erase(kvInner.first); // Delete associated symbol table
        return true;
      }
      return false;
    });
    return kvOuter.second->empty();
  });

  // Prune non-substantiated structs
  std::erase_if(structs, [&](const auto &kvOuter) {
    std::erase_if(*kvOuter.second, [&](const auto &kvInner) {
      if (!kvInner.second.isFullySubstantiated()) {
        children.erase(STRUCT_SCOPE_PREFIX + kvInner.second.getName()); // Delete associated symbol table
        return true;
      }
      return false;
    });
    return kvOuter.second->empty();
  });

  // Recursion
  for (const auto &[_, child] : children)
    child->purgeSubstantiationRemnants();
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
        CompilerWarning(entry.getDeclCodeLoc(), UNUSED_FUNCTION, "The function '" + entry.getName() + "' is unused").print();
      } else if (entry.getType().is(TY_PROCEDURE)) {
        CompilerWarning(entry.getDeclCodeLoc(), UNUSED_PROCEDURE, "The procedure '" + entry.getName() + "' is unused").print();
      } else if (entry.getType().is(TY_STRUCT) || entry.getType().isPointerOf(TY_STRUCT)) {
        CompilerWarning(entry.getDeclCodeLoc(), UNUSED_STRUCT, "The struct '" + entry.getName() + "' is unused").print();
      } else if (entry.getType().isOneOf({TY_IMPORT})) {
        CompilerWarning(entry.getDeclCodeLoc(), UNUSED_IMPORT, "The import '" + entry.getName() + "' is unused").print();
      } else {
        if (entry.getName() != UNUSED_VARIABLE_NAME && entry.getName() != FOREACH_DEFAULT_IDX_VARIABLE_NAME)
          CompilerWarning(entry.getDeclCodeLoc(), UNUSED_VARIABLE, "The variable '" + entry.getName() + "' is unused").print();
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
 * Checks if this symbol table is imported
 *
 * @param askingScope Symbol table, which asks whether the current one is imported from its point of view or not
 *
 * @return Imported / not imported
 */
bool SymbolTable::isImported(const SymbolTable *askingScope) const {
  // Get root scope of the source file where askingScope scope lives
  const SymbolTable *askingRootScope = askingScope;
  while (!askingRootScope->isSourceFileRootScope && askingRootScope->getParent())
    askingRootScope = askingRootScope->getParent();

  // Get root scope of the source file where the current scope lives
  const SymbolTable *thisRootScope = this;
  while (!thisRootScope->isSourceFileRootScope && thisRootScope->getParent())
    thisRootScope = thisRootScope->getParent();

  return askingRootScope != thisRootScope;
}

/**
 * Retrieves the scope type of the current scope
 *
 * @return Scope type
 */
ScopeType SymbolTable::getScopeType() const { return scopeType; }

/**
 * Mark this scope so that the compiler knows that accessing variables from outside within the scope requires capturing
 */
void SymbolTable::setCapturingRequired() { requiresCapturing = true; }

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
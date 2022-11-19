// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "Scope.h"

#include <ast/ASTNodes.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

Scope::~Scope() {
  for (const auto &child : children)
    delete child.second;
}

/**
 * Create a child scope and return it
 *
 * @param scopeName Name of the child scope
 * @param scopeType Type of the child scope
 * @return Child scope (heap allocated)
 */
Scope *Scope::createChildScope(const std::string &scopeName, const ScopeType &scopeType) {
  children.insert({scopeName, new Scope(this, scopeType)});
  return children.at(scopeName);
}

/**
 * Rename the child scope. This is useful for realizing function overloading by storing a function with not
 * only its name, but also its signature
 *
 * @param oldName Old name of the child table
 * @param newName New name of the child table
 */
void Scope::renameChildScope(const std::string &oldName, const std::string &newName) {
  auto nodeHandler = children.extract(oldName);
  nodeHandler.key() = newName;
  children.insert(std::move(nodeHandler));
}

/**
 * Duplicates the child scope by copying it. The duplicated symbols point to the original ones.
 *
 * @param oldName Old name of the child block
 * @param newName New block name
 */
void Scope::copyChildScope(const std::string &oldName, const std::string &newName) {
  assert(children.contains(oldName));
  const Scope origChildScope = *children.at(oldName);
  children.insert({newName, new Scope(origChildScope)});
}

/**
 * Retrieve the global scope of the current source file
 *
 * @return Global scope
 */
Scope *Scope::getGlobalScope() { return searchForScope(SCOPE_GLOBAL); }

/**
 * Retrieve the scope of the function or procedure, where this scope lives
 *
 * @return Function or procedure scope
 */
Scope *Scope::getFunctionScope() { return searchForScope(SCOPE_FUNC_PROC_BODY); }

/**
 * Get a child scope of the current scope by its name
 *
 * @param scopeName Child scope name
 * @return Child scope
 */
Scope *Scope::getChildScope(const std::string &scopeName) const {
  if (!children.empty() && children.contains(scopeName))
    return children.at(scopeName);
  return nullptr;
}

/**
 * Retrieve all variables in the current scope, that have reached the end of their lifetime at the end of this scope
 *
 * @return Collection of EOL variables
 */
std::vector<SymbolTableEntry *> Scope::getVarsGoingOutOfScope() { // NOLINT(misc-no-recursion)
  assert(parent != nullptr);                                      // Should not be called in root scope
  std::vector<SymbolTableEntry *> varsGoingOutOfScope;

  // Collect all variables in this scope
  for (const auto &[name, entry] : symbolTable.symbols) {
    // Skip 'this' variables
    if (name == THIS_VARIABLE_NAME)
      continue;
    // For dtor calls, only anonymous structs are relevant
    if (entry.getType().isOneOf({TY_STRUCT, TY_STROBJ}) && !entry.isDead() && entry.name.starts_with("anon."))
      varsGoingOutOfScope.push_back(&symbolTable.symbols.at(name));
  }

  // Collect all variables in the child scopes
  for (const auto &[_, child] : children) {
    const ScopeType scopeType = child->type;
    // Exclude enum, global, struct and thread body (is a LLVM function) scopes
    if (scopeType != SCOPE_ENUM && scopeType != SCOPE_GLOBAL && scopeType != SCOPE_STRUCT && scopeType != SCOPE_THREAD_BODY) {
      std::vector childVars = child->getVarsGoingOutOfScope();
      varsGoingOutOfScope.insert(varsGoingOutOfScope.end(), childVars.begin(), childVars.end());
    }
  }

  return varsGoingOutOfScope;
}

/**
 * Insert a new generic type in this scope
 *
 * @param genericType Generic type itself
 */
void Scope::insertGenericType(const GenericType &genericType) { genericTypes.insert({genericType.getName(), genericType}); }

/**
 * Search for a generic type by its name. If it was not found, the parent scopes will be searched.
 * If the generic type does not exist at all, the function will return a nullptr.
 *
 * @param typeName Name of the generic type
 * @return Generic type
 */
GenericType *Scope::lookupGenericType(const std::string &typeName) { // NOLINT(misc-no-recursion)
  if (genericTypes.contains(typeName))
    return &genericTypes.at(typeName);
  return parent ? parent->lookupGenericType(typeName) : nullptr;
}

/**
 * Insert a function object into this symbol table scope
 *
 * @param function Function object
 * @param manifestations Output parameter for collecting the
 */
void Scope::insertFunction(const Function &function, std::vector<Function *> *manifestations /*=nullptr*/) {
  // Open a new function declaration pointer list. Which gets filled by the 'insertSubstantiatedFunction' method
  functions.insert({function.declNode->codeLoc.toString(), std::unordered_map<std::string, Function>()});

  // Substantiate the optional params of the function
  for (const Function &spiceFunc : function.substantiateOptionalParams()) {
    Function *manifestation = insertSubstantiatedFunction(spiceFunc, function.declNode);
    assert(manifestation != nullptr);
    if (manifestations)
      manifestations->push_back(manifestation);
  }
}

/**
 * Check if there is a function in this scope, fulfilling all given requirements and if found, return it.
 * If more than one function matches the requirement, an error gets thrown
 *
 * @param callFunctionName Function name requirement
 * @param callThisType This type requirement
 * @param callTemplateTypes Template types to substantiate generic types
 * @param callArgTypes Argument types requirement
 * @param node Declaration node for a potential error message
 * @return Matched function or nullptr
 */
Function *Scope::matchFunction(const std::string &callFunctionName, const SymbolType &callThisType,
                               const std::vector<SymbolType> &callTemplateTypes, const std::vector<SymbolType> &callArgTypes,
                               const ASTNode *node) {
  std::vector<Function *> matches;

  // Loop through functions and add any matches to the matches vector
  auto oldFunctionsList = functions;
  for (const auto &[defCodeLocStr, manifestations] : oldFunctionsList) {
    auto oldManifestations = manifestations;
    for (auto &[mangledName, f] : oldManifestations) {
      // Check name requirement
      if (f.name != callFunctionName)
        continue;

      // Initialize mapping from generic type name to concrete symbol type
      std::map<std::string, SymbolType> concreteGenericTypes;

      // Check 'this' type requirement
      const SymbolType fctThisType = f.thisType;
      if (fctThisType.getTemplateTypes().empty()) { // The 'this' type is a non-generic struct
        if (callThisType != fctThisType)
          continue;
      } else { // The 'this' type is a generic struct
        for (int i = 0; i < fctThisType.getTemplateTypes().size(); i++) {
          SymbolType genericType = fctThisType.getTemplateTypes()[i];
          SymbolType concreteGenericType = callThisType.getTemplateTypes()[i];
          concreteGenericTypes.emplace(genericType.getSubType(), concreteGenericType);
        }
      }

      // Check arg types requirement
      ParamList argList = f.paramList;
      if (argList.size() != callArgTypes.size())
        continue;
      bool differentArgTypes = false; // Note: This is a workaround for a break from an inner loop
      for (int i = 0; i < argList.size(); i++) {
        if (argList[i].type.isBaseType(TY_GENERIC)) { // Resolve concrete type for arguments with generic type
          std::string genericTypeName = argList[i].type.getBaseType().getSubType();
          if (concreteGenericTypes.contains(genericTypeName)) { // This generic type was already resolved => check if it matches
            if (concreteGenericTypes.at(genericTypeName) != callArgTypes[i].getBaseType()) {
              differentArgTypes = true;
              break;
            }
          } else { // This generic type was not resolved yet => resolve it and add it to the list
            // De-assemble the concrete type to match the generic type
            SymbolType curGenericType = argList[i].type;
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
            concreteGenericTypes.emplace(genericTypeName, curArgType);
          }
          argList[i].type = argList[i].type.replaceBaseType(concreteGenericTypes.at(genericTypeName));
        } else { // For arguments with non-generic type, check if the candidate type matches with the call
          const SymbolType &expected = argList[i].type;
          const SymbolType &actual = callArgTypes[i];
          if (!equalsIgnoreArraySizes(actual, expected) && !(actual.is(TY_STROBJ) && expected.is(TY_STRUCT, STROBJ_NAME))) {
            differentArgTypes = true;
            break;
          }
        }
      }
      if (differentArgTypes)
        continue;

      // Determine the concrete types of all generic types, that were not yet resolved via the arg types
      size_t callTemplateTypeIndex = 0;
      for (const GenericType &definitionTemplateType : f.templateTypes) {
        std::string genericTypeName = definitionTemplateType.getSubType();

        // Check if the generic type was already resolved
        if (concreteGenericTypes.contains(genericTypeName))
          continue;

        // Check if we exceeded the length of the call template list
        if (callTemplateTypeIndex >= callTemplateTypes.size())
          throw SemanticError(node, GENERIC_TYPE_NOT_IN_TEMPLATE,
                              "Could not determine concrete type of the generic type '" + genericTypeName +
                                  "'. Please specify it in the template list at the call site.");

        concreteGenericTypes.emplace(genericTypeName, callTemplateTypes[callTemplateTypeIndex]);
        callTemplateTypeIndex++;
      }
      // If types remain in the callTemplateTypes vector, skip this function substantiation (useful for generic return types)
      if (callTemplateTypeIndex < callTemplateTypes.size() && f.name != CTOR_FUNCTION_NAME)
        continue;

      // Duplicate function
      Function newFunction = f.substantiateGenerics(argList, callThisType, concreteGenericTypes);
      if (!getChildScope(newFunction.getSignature())) { // Insert function
        insertSubstantiatedFunction(newFunction, f.declNode);
        copyChildScope(f.getSignature(), newFunction.getSignature());

        // Insert symbols for generic type names with concrete types into the child block
        Scope *childBlock = getChildScope(newFunction.getSignature());
        for (const auto &[typeName, symbolType] : concreteGenericTypes)
          childBlock->insertGenericType(GenericType(symbolType));

        // Replace this type with concrete one
        if ((f.isMethodFunction() || f.isMethodProcedure()) && !fctThisType.getTemplateTypes().empty()) {
          SymbolTableEntry *thisEntry = childBlock->lookup(THIS_VARIABLE_NAME);
          assert(thisEntry != nullptr);
          SymbolType newThisType = callThisType.toPointer(node);
          thisEntry->updateType(newThisType, true);
        }
      }

      assert(functions.contains(defCodeLocStr) && functions.at(defCodeLocStr).contains(newFunction.getMangledName()));
      matches.push_back(&functions.at(defCodeLocStr).at(newFunction.getMangledName()));
      break;
    }
  }

  if (matches.empty())
    return nullptr;

  // Throw error if more than one function matches the criteria
  if (matches.size() > 1)
    throw SemanticError(
        node, FUNCTION_AMBIGUITY,
        "More than one function matches your requested signature criteria. Please try to specify the return type explicitly");

  return matches.front();
}

/**
 * Retrieve the manifestations of the function, defined at defToken
 *
 * @param defCodeLoc Definition code location
 * @return Function manifestations
 */
std::unordered_map<std::string, Function> *Scope::getFunctionManifestations(const CodeLoc &defCodeLoc) {
  std::string codeLocStr = defCodeLoc.toString();
  return functions.contains(codeLocStr) ? &functions.at(codeLocStr) : nullptr;
}

/**
 * Insert a substantiated function into the function list. If the list already contains a function with the same signature,
 * an exception will be thrown
 *
 * @param function Substantiated function
 * @param declNode Declaration AST node
 */
Function *Scope::insertSubstantiatedFunction(const Function &function, const ASTNode *declNode) {
  if (!function.hasSubstantiatedParams())
    throw std::runtime_error("Internal compiler error: Expected substantiated function");

  // Check if the function exists already
  const std::string mangledFctName = function.getMangledName();
  for (const auto &[_, manifestations] : functions) {
    if (manifestations.contains(mangledFctName))
      throw SemanticError(declNode, FUNCTION_DECLARED_TWICE,
                          "The function/procedure '" + function.getSignature() + "' is declared twice");
  }
  // Add function to function list
  const std::string codeLocStr = declNode->codeLoc.toString();
  assert(functions.contains(codeLocStr));
  functions.at(codeLocStr).emplace(mangledFctName, function);
  // Add symbol table entry for the function
  SymbolTableEntry *functionEntry = insert(function.getSignature(), function.entry->specifiers, declNode);
  functionEntry->updateType(function.getSymbolType(), true);
  return &functions.at(codeLocStr).at(mangledFctName);
}

/**
 * Insert a struct object into this symbol table scope
 *
 * @param spiceStruct Struct object
 */
Struct *Scope::insertStruct(const Struct &spiceStruct) {
  // Open a new struct declaration pointer list. Which gets filled by the 'insertSubstantiatedStruct' method
  std::string codeLocStr = spiceStruct.declNode->codeLoc.toString();
  structs.insert({codeLocStr, std::unordered_map<std::string, Struct>()});
  return insertSubstantiatedStruct(spiceStruct, spiceStruct.declNode);
}

/**
 * Check if there is a struct in this scope, fulfilling all given requirements and if found, return it.
 * If more than one struct matches the requirement, an error gets thrown
 *
 * @param currentScope Current scope
 * @param structName Struct name
 * @param templateTypes Template type requirements
 * @param node Declaration node for the error message
 * @return Matched struct or nullptr
 */
Struct *Scope::matchStruct(Scope *currentScope, const std::string &structName, // NOLINT(misc-no-recursion)
                           const std::vector<SymbolType> &templateTypes, const ASTNode *node) {
  std::vector<Struct *> matches;

  // Loop through structs and add any matches to the matches vector
  auto oldStructList = structs;
  for (const auto &[defCodeLocStr, manifestations] : oldStructList) {
    auto oldManifestations = manifestations;
    for (auto &[mangledName, s] : oldManifestations) {
      // Check name requirement
      if (s.name != structName)
        continue;

      // Check template types requirement
      const std::vector<GenericType> structTemplateTypes = s.templateTypes;
      if (structTemplateTypes.empty()) {
        // It's a match!
        matches.push_back(&structs.at(defCodeLocStr).at(s.getMangledName()));
      } else {
        if (structTemplateTypes.size() != templateTypes.size())
          continue;
        std::vector<SymbolType> concreteTemplateTypes;
        bool differentTemplateTypes = false; // Note: This is a workaround for a break from an inner loop
        for (int i = 0; i < templateTypes.size(); i++) {
          if (!structTemplateTypes[i].checkConditionsOf(templateTypes[i])) {
            differentTemplateTypes = true;
            break;
          }
          concreteTemplateTypes.push_back(templateTypes[i]);
        }
        if (differentTemplateTypes)
          continue;

        // Duplicate function
        Scope *structScope = getChildScope(STRUCT_SCOPE_PREFIX + structName);
        Struct newStruct = s.substantiateGenerics(concreteTemplateTypes, structScope);
        if (!getChildScope(STRUCT_SCOPE_PREFIX + newStruct.getSignature())) { // Insert struct
          insertSubstantiatedStruct(newStruct, s.declNode);
          copyChildScope(STRUCT_SCOPE_PREFIX + structName, STRUCT_SCOPE_PREFIX + newStruct.getSignature());
        }

        assert(structs.contains(defCodeLocStr) && structs.at(defCodeLocStr).contains(newStruct.getMangledName()));
        matches.push_back(&structs.at(defCodeLocStr).at(newStruct.getMangledName()));
        break;
      }
    }
  }

  if (matches.empty() && parent)
    matches.push_back(parent->matchStruct(currentScope, structName, templateTypes, node));

  if (matches.empty())
    return nullptr;

  // Throw error if more than one struct matches the criteria
  if (matches.size() > 1)
    throw SemanticError(
        node, STRUCT_AMBIGUITY,
        "More than one struct matches your requested signature criteria. Please try to specify the return type explicitly");

  return matches.front();
}

/**
 * Retrieve the manifestations of the struct, defined at defToken
 *
 * @return Struct manifestations
 */
std::unordered_map<std::string, Struct> *Scope::getStructManifestations(const CodeLoc &defCodeLoc) {
  std::string codeLocStr = defCodeLoc.toString();
  if (!structs.contains(codeLocStr))
    throw std::runtime_error("Internal compiler error: Cannot get struct manifestations at " + codeLocStr);
  return &structs.at(codeLocStr);
}

/**
 * Insert a substantiated struct into the struct list. If the list already contains a struct with the same signature,
 * an exception will be thrown
 *
 * @param s Substantiated struct
 * @param declNode Declaration AST node
 */
Struct *Scope::insertSubstantiatedStruct(const Struct &s, const ASTNode *declNode) {
  // Check if the struct exists already
  for (const auto &[_, manifestations] : structs) {
    if (manifestations.contains(s.getMangledName()))
      throw SemanticError(declNode, STRUCT_DECLARED_TWICE, "The struct '" + s.getSignature() + "' is declared twice");
  }
  // Add struct to struct list
  const std::string codeLocStr = declNode->codeLoc.toString();
  assert(structs.contains(codeLocStr));
  structs.at(codeLocStr).emplace(s.getMangledName(), s);
  // Add symbol table entry for the struct
  SymbolTableEntry *structEntry = insert(s.getSignature(), s.entry->specifiers, declNode);
  structEntry->updateType(s.getSymbolType(), true);
  return &structs.at(codeLocStr).at(s.getMangledName());
}

/**
 * Retrieve an interface instance by its name
 *
 * @param interfaceName Name of the interface
 * @return Interface object
 */
Interface *Scope::lookupInterface(const std::string &interfaceName) {
  if (!interfaces.contains(interfaceName))
    return nullptr;
  return &interfaces.at(interfaceName);
}

/**
 * Insert an interface object into this symbol table scope
 *
 * @param interface Interface object
 */
void Scope::insertInterface(const Interface &interface) {
  // Add interface to interface list
  assert(!interfaces.contains(interface.name));
  interfaces.insert({interface.name, interface});
  // Add symbol table entry for the interface
  SymbolTableEntry *interfaceEntry = insert(interface.name, interface.specifiers, interface.declNode);
  interfaceEntry->updateType(SymbolType(TY_INTERFACE), false);
}

/**
 * Get the number of fields if this is a struct scope
 *
 * @return Number of fields
 */
size_t Scope::getFieldCount() const {
  assert(type == SCOPE_STRUCT);
  size_t fieldCount = 0;
  for (const auto &symbol : symbolTable.symbols) {
    const SymbolType &symbolType = symbol.second.getType();
    if (symbolType.isPrimitive() || symbolType.isOneOf({TY_STRUCT, TY_STROBJ}))
      fieldCount++;
  }
  return fieldCount;
}

/**
 * Get the current number of nested loops
 *
 * @return Number of loops
 */
size_t Scope::getLoopNestingDepth() const { // NOLINT(misc-no-recursion)
  assert(parent != nullptr);
  if (parent->parent == nullptr)
    return 0;
  size_t loopCount = parent->getLoopNestingDepth();
  if (type == SCOPE_WHILE_BODY || type == SCOPE_FOR_BODY || type == SCOPE_FOREACH_BODY)
    loopCount++;
  return loopCount;
}

/**
 * Collect all warnings, produces within this scope
 *
 * @return Collection of warnings
 */
std::vector<CompilerWarning> Scope::collectWarnings() const { // NOLINT(misc-no-recursion)
  std::vector<CompilerWarning> warnings;
  // Visit own symbols
  for (const auto &[key, entry] : symbolTable.symbols) {
    // Do not produce a warning if the symbol is used or has a special name
    if (entry.isUsed || entry.name == UNUSED_VARIABLE_NAME)
      continue;
  }

  return warnings;
}

/**
 * Searches in the parent scopes for a scope with a certain type
 *
 * @param scopeType Type to search for
 * @return Next higher scope that has the expected type
 */
Scope *Scope::searchForScope(const ScopeType &scopeType) {
  Scope *searchResult = this;
  while (searchResult && searchResult->type != scopeType)
    searchResult = searchResult->parent;
  return searchResult;
}

/**
 * Checks if this scope is imported
 *
 * @param askingScope Scope, which asks whether the current one is imported from its point of view or not
 *
 * @return Imported / not imported
 */
bool Scope::isImportedBy(const Scope *askingScope) const {
  // Get root scope of the source file where askingScope scope lives
  const Scope *askingRootScope = askingScope;
  while (askingRootScope->type != SCOPE_GLOBAL && askingRootScope->parent)
    askingRootScope = askingRootScope->parent;

  // Get root scope of the source file where the current scope lives
  const Scope *thisRootScope = this;
  while (thisRootScope->type != SCOPE_GLOBAL && thisRootScope->parent)
    thisRootScope = thisRootScope->parent;

  return askingRootScope != thisRootScope;
}

/**
 * Check if unsafe operations are allowed in this scope
 *
 * @return Allowed or not
 */
bool Scope::doesAllowUnsafeOperations() const { // NOLINT(misc-no-recursion)
  if (type == SCOPE_UNSAFE_BODY)
    return true;
  return parent != nullptr && parent->doesAllowUnsafeOperations();
}

nlohmann::json Scope::getSymbolTableJSON() const {
  nlohmann::json result = symbolTable.toJSON();

  // Collect all children
  std::vector<nlohmann::json> jsonChildren;
  jsonChildren.reserve(children.size());
  for (const auto &child : children) {
    nlohmann::json c = child.second->getSymbolTableJSON();
    c["name"] = child.first; // Inject symbol table name into JSON object
    jsonChildren.emplace_back(c);
  }
  result["children"] = jsonChildren;

  return result;
}
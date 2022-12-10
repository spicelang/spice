// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "Scope.h"

#include <ast/ASTNodes.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

Scope::~Scope() {
  // Reset fields
  parent = nullptr;
  codeLoc = nullptr;
  // Notify all parents, that the scope is de-allocated now
  for (Scope *parent : parents) {
    if (parent)
      for (auto &child : parent->children)
        if (child.second == this)
          child.second = nullptr;
  }
  // Destroy child scopes
  for (const auto &scope : children)
    delete scope.second;
  children.clear();
}

/**
 * Create a child scope and return it
 *
 * @param scopeName Name of the child scope
 * @param scopeType Type of the child scope
 * @param codeLoc Code location of the scope
 * @return Child scope (heap allocated)
 */
Scope *Scope::createChildScope(const std::string &scopeName, const ScopeType &scopeType, const CodeLoc *codeLoc) {
  auto newScope = new Scope(this, sourceFile, scopeType, codeLoc);
  children.insert({scopeName, newScope});
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
  assert(children.contains(oldName) && !children.contains(newName));
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
  assert(children.contains(oldName) && !children.contains(newName));
  const Scope *origChildScope = children.at(oldName);
  auto newChildBlock = new Scope(*origChildScope);
  // Add newChildBlock as parent of all children for tracking de-allocation
  for (const auto &child : newChildBlock->children)
    child.second->parents.push_back(newChildBlock);
  children.insert({newName, newChildBlock});
}

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
    if (entry.getType().is(TY_STRUCT) && !entry.isDead() && entry.name.starts_with("anon."))
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

void Scope::addParent(Scope *parent) { parents.push_back(parent); }

/**
 * Insert a new generic type in this scope
 *
 * @param genericType Generic type itself
 */
void Scope::insertGenericType(const std::string &typeName, const GenericType &genericType) {
  assert(!genericTypes.contains(typeName));
  genericTypes.insert({typeName, genericType});
}

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
  interfaceEntry->updateType(SymbolType(TY_INTERFACE), true);
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
    if (symbolType.isPrimitive() || symbolType.isOneOf({TY_STRUCT, TY_INTERFACE}))
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
    if (entry.used || entry.name == UNUSED_VARIABLE_NAME)
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
bool Scope::isImportedBy(Scope *askingScope) const {
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

/**
 * Get JSON representation of the symbol table
 *
 * @return Symbol table as JSON object
 */
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
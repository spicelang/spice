// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "Scope.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

/**
 * Create a child scope and return it
 *
 * @param scopeName Name of the child scope
 * @param scopeType Type of the child scope
 * @param declCodeLoc Code location of the scope
 * @return Child scope (heap allocated)
 */
Scope *Scope::createChildScope(const std::string &scopeName, ScopeType scopeType, const CodeLoc *declCodeLoc) {
  children.insert({scopeName, std::make_shared<Scope>(this, sourceFile, scopeType, declCodeLoc)});
  return children.at(scopeName).get();
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
Scope *Scope::copyChildScope(const std::string &oldName, const std::string &newName) {
  assert(children.contains(oldName) && !children.contains(newName));
  // Create copy
  const std::shared_ptr<Scope> newScope = children.at(oldName)->deepCopyScope();
  // Save copy under new name
  children.insert({newName, newScope});
  return newScope.get();
}

/**
 * Deep copy the current scope and all its children
 *
 * @return Deep copy of the current scope
 */
std::shared_ptr<Scope> Scope::deepCopyScope() { // NOLINT(misc-no-recursion)
  const std::shared_ptr<Scope> newScope = std::make_shared<Scope>(*this);
  for (const auto &[childName, oldChild] : children) {
    newScope->children[childName] = oldChild->deepCopyScope();
    newScope->children[childName]->parent = newScope.get();
    newScope->children[childName]->symbolTable.scope = newScope->children[childName].get();
    newScope->children[childName]->symbolTable.parent = &newScope->symbolTable;
  }
  newScope->symbolTable.scope = newScope.get();
  return newScope;
}

/**
 * Get a child scope of the current scope by its name
 *
 * @param scopeName Child scope name
 * @return Child scope
 */
Scope *Scope::getChildScope(const std::string &scopeName) const {
  if (!children.empty() && children.contains(scopeName))
    return children.at(scopeName).get();
  return nullptr;
}

/**
 * Retrieve all variables in the current scope, that have reached the end of their lifetime at the end of this scope
 *
 * @return Collection of EOL variables
 */
std::vector<SymbolTableEntry *> Scope::getVarsGoingOutOfScope() { // NOLINT(misc-no-recursion)
  assert(!isRootScope());                                         // Should not be called in root scope
  std::vector<SymbolTableEntry *> varsGoingOutOfScope;

  // Collect all variables in this scope
  for (const auto &[name, entry] : symbolTable.symbols) {
    // Skip 'this' and result variables
    if (name == THIS_VARIABLE_NAME || name == RETURN_VARIABLE_NAME)
      continue;
    // Skip parameters (ToDo: Remove when copy constructors work for by-value argument passing)
    if (entry.isParam)
      continue;
    // Found variable, that goes out of scope
    varsGoingOutOfScope.push_back(&symbolTable.symbols.at(name));
  }

  // If this is the scope of a dtor, also return all fields of the struct
  if (isDtorScope) {
    assert(!isRootScope() && parent->type == ScopeType::STRUCT);
    // Get all fields of the struct
    for (const auto &[name, entry] : parent->symbolTable.symbols)
      if (!entry.getQualType().isOneOf({TY_FUNCTION, TY_PROCEDURE}))
        varsGoingOutOfScope.push_back(&parent->symbolTable.symbols.at(name));
  }

  return varsGoingOutOfScope;
}

/**
 * Insert a new generic type in this scope
 *
 * @param typeName Generic type name
 * @param genericType Generic type itself
 */
void Scope::insertGenericType(const std::string &typeName, const GenericType &genericType) {
  assert(!genericTypes.contains(typeName));
  genericTypes.insert({typeName, genericType});
}

/**
 * Search for a generic type by its name in this scope.
 * If the generic type is not found, a nullptr is returned.
 *
 * @param typeName Name of the generic type
 * @return Generic type
 */
GenericType *Scope::lookupGenericTypeStrict(const std::string &typeName) {
  return genericTypes.contains(typeName) ? &genericTypes.at(typeName) : nullptr;
}

/**
 * Collect all warnings, produced within this scope
 *
 * @param warnings List of warnings
 * @return Collection of warnings
 */
void Scope::collectWarnings(std::vector<CompilerWarning> &warnings) const { // NOLINT(misc-no-recursion)
  // Visit own symbols
  for (const auto &entry : symbolTable.symbols | std::views::values) {
    // Do not produce a warning if the symbol is used or has a special name
    const std::string &name = entry.name;
    if (entry.used || name.starts_with(UNUSED_VARIABLE_NAME))
      continue;

    const QualType entryType = entry.getQualType();

    // Determine warning type and message by the scope type and the symbol type
    CompilerWarningType warningType = UNUSED_VARIABLE;
    std::string warningMessage;
    switch (type) {
    case ScopeType::GLOBAL: {
      // Skip generic function/procedure/struct/interface entries
      if (entryType.isOneOf({TY_FUNCTION, TY_PROCEDURE, TY_STRUCT, TY_INTERFACE}) && !entryType.getTemplateTypes().empty())
        continue;

      if (entryType.is(TY_FUNCTION)) {
        const std::vector<Function *> *fctManifestations = entry.declNode->getFctManifestations(name);
        warningType = UNUSED_FUNCTION;
        warningMessage = "'" + fctManifestations->front()->getSignature() + "' is unused";
      } else if (entryType.is(TY_PROCEDURE)) {
        const std::vector<Function *> *fctManifestations = entry.declNode->getFctManifestations(name);
        warningType = UNUSED_PROCEDURE;
        warningMessage = "'" + fctManifestations->front()->getSignature() + "' is unused";
      } else if (entryType.is(TY_STRUCT)) {
        warningType = UNUSED_STRUCT;
        warningMessage = "The struct '" + entry.name + "' is unused";
      } else if (entryType.is(TY_INTERFACE)) {
        warningType = UNUSED_INTERFACE;
        warningMessage = "The interface '" + entry.name + "' is unused";
      } else if (entryType.is(TY_ENUM)) {
        continue; // Do not report unused enums. Only unused enum items are reported
      } else if (entryType.is(TY_IMPORT)) {
        warningType = UNUSED_IMPORT;
        warningMessage = "The import '" + entry.name + "' is unused";
      } else if (entryType.is(TY_ALIAS)) {
        warningType = UNUSED_ALIAS;
        warningMessage = "The type alias '" + entry.name + "' is unused";
      } else {
        warningType = UNUSED_VARIABLE;
        warningMessage = "The variable '" + entry.name + "' is unused";
      }

      break;
    }
    case ScopeType::STRUCT: // fall-through
    case ScopeType::INTERFACE: {
      if (entry.isField()) {
        warningType = UNUSED_FIELD;
        warningMessage = "The field '" + entry.name + "' is unused";
      } else if (entryType.isOneOf({TY_FUNCTION, TY_PROCEDURE})) {
        // Skip implicit method entries and generic templates
        const std::vector<Function *> *fctManifestations = entry.declNode->getFctManifestations(name);
        if (fctManifestations->empty() || fctManifestations->front()->implicitDefault)
          continue;

        warningType = UNUSED_METHOD;
        warningMessage = "The method '" + fctManifestations->front()->getSignature() + "' is unused";
      }
      break;
    }
    case ScopeType::ENUM: {
      warningType = UNUSED_ENUM_ITEM;
      warningMessage = "The enum item '" + entry.name + "' is unused";
      break;
    }
    case ScopeType::FOREACH_BODY: {
      // Skip idx variables
      if (entry.name == FOREACH_DEFAULT_IDX_VARIABLE_NAME)
        continue;
      [[fallthrough]];
    }
    default: {
      warningType = UNUSED_VARIABLE;
      warningMessage = "The variable '" + entry.name + "' is unused";
      break;
    }
    }

    // Add warning
    warnings.emplace_back(entry.getDeclCodeLoc(), warningType, warningMessage);
  }

  // Visit children
  for (const auto &childScope : children | std::views::values)
    if (!childScope->isGenericScope)
      childScope->collectWarnings(warnings);
}

/**
 * Checks if all variables of this and all child scopes are of an explicit type.
 * This is executed after type inference to check that all variables could be inferred correctly.
 */
void Scope::ensureSuccessfulTypeInference() const { // NOLINT(misc-no-recursion)
  // Check symbols in this scope
  for (auto &[name, entry] : symbolTable.symbols)
    if (entry.getQualType().is(TY_DYN))
      throw SemanticError(entry.declNode, UNEXPECTED_DYN_TYPE, "For the variable '" + name + "' no type could be inferred");

  // Check child scopes
  for (const auto &scope : children | std::views::values)
    scope->ensureSuccessfulTypeInference();
}

/**
 * Get the number of fields if this is a struct scope
 *
 * @return Number of fields
 */
size_t Scope::getFieldCount() const {
  assert(type == ScopeType::STRUCT);
  size_t fieldCount = 0;
  for (const auto &symbol : symbolTable.symbols | std::views::values) {
    if (symbol.anonymous)
      continue;
    const QualType &symbolType = symbol.getQualType();
    if (symbolType.is(TY_IMPORT))
      continue;
    const ASTNode *declNode = symbol.declNode;
    if (declNode->isFctOrProcDef() || declNode->isStructDef())
      continue;
    fieldCount++;
  }
  return fieldCount;
}

/**
 * Get all virtual methods in this scope, sorted by declaration code location
 *
 * @return List of virtual method pointers
 */
std::vector<Function *> Scope::getVirtualMethods() {
  assert(type == ScopeType::STRUCT || type == ScopeType::INTERFACE);

  // Collect all virtual methods
  std::vector<Function *> methods;
  for (auto &[fctId, manifestationList] : functions) {
    assert(!manifestationList.empty());
    for (auto &[mangledName, function] : manifestationList)
      if (function.isVirtualMethod())
        methods.push_back(&functions.at(fctId).at(mangledName));
  }

  // Sort the list
  std::ranges::sort(methods, [](const Function *a, const Function *b) { return a->getDeclCodeLoc() < b->getDeclCodeLoc(); });

  return methods;
}

/**
 * Retrieve all struct manifestations in this scope in the order of their declaration
 *
 * @return All struct manifestations in declaration order
 */
std::vector<const Struct *> Scope::getAllStructManifestationsInDeclarationOrder() const {
  // Retrieve all struct manifestations in this scope
  std::vector<const Struct *> manifestations;
  manifestations.reserve(structs.size()); // Reserve at least the size of individual generic structs
  for (const auto &structManifestations : structs | std::views::values)
    for (const auto &manifestation : structManifestations | std::views::values)
      manifestations.push_back(&manifestation);

  // Sort manifestations by declaration code location
  auto sortLambda = [](const Struct *lhs, const Struct *rhs) { return lhs->getDeclCodeLoc() < rhs->getDeclCodeLoc(); };
  std::ranges::sort(manifestations, sortLambda);
  return manifestations;
}

/**
 * Check if this struct has any reference fields
 *
 * @return Has reference fields or not
 */
bool Scope::hasRefFields() {
  assert(type == ScopeType::STRUCT);
  for (size_t i = 0; i < getFieldCount(); i++)
    if (lookupField(i)->getQualType().isRef())
      return true;
  return false;
}

/**
 * Get the current number of nested loops
 *
 * @return Number of loops
 */
unsigned int Scope::getLoopNestingDepth() const { // NOLINT(misc-no-recursion)
  assert(!isRootScope());
  if (parent->parent == nullptr)
    return 0;
  unsigned int loopCount = parent->getLoopNestingDepth();
  if (type == ScopeType::WHILE_BODY || type == ScopeType::FOR_BODY || type == ScopeType::FOREACH_BODY)
    loopCount++;
  return loopCount;
}

/**
 * Check if this scope is one of the child scopes of a switch statement
 *
 * @return Child scope of switch statement or not
 */
bool Scope::isInCaseBranch() const { // NOLINT(misc-no-recursion)
  assert(!isRootScope());
  if (parent->parent == nullptr)
    return false;
  if (type == ScopeType::CASE_BODY)
    return true;
  return parent->isInCaseBranch();
}

/**
 * Check if this scope is within an async scope
 *
 * @return Within async scope or not
 */
bool Scope::isInAsyncScope() const { // NOLINT(misc-no-recursion)
  if (isAsyncScope)
    return true;
  return !isRootScope() && parent->isInAsyncScope();
}

/**
 * Check if unsafe operations are allowed in this scope
 *
 * @return Allowed or not
 */
bool Scope::doesAllowUnsafeOperations() const { // NOLINT(misc-no-recursion)
  if (type == ScopeType::UNSAFE_BODY)
    return true;
  return !isRootScope() && parent->doesAllowUnsafeOperations();
}

/**
 * Checks if this scope is imported
 *
 * @param askingScope Scope, which asks whether the current one is imported from its point of view or not
 *
 * @return Imported / not imported
 */
bool Scope::isImportedBy(const Scope *askingScope) const { return askingScope->sourceFile->imports(sourceFile); }

/**
 * Get JSON representation of the symbol table
 *
 * @return Symbol table as JSON object
 */
nlohmann::json Scope::getSymbolTableJSON() const { // NOLINT(misc-no-recursion)
  nlohmann::json result = symbolTable.toJSON();

  // Collect all children
  std::vector<nlohmann::json> jsonChildren;
  jsonChildren.reserve(children.size());
  for (const auto &[name, childScope] : children) {
    nlohmann::json c = childScope->getSymbolTableJSON();
    c["name"] = name; // Inject symbol table name into JSON object
    jsonChildren.emplace_back(c);
  }
  result["children"] = jsonChildren;

  return result;
}

} // namespace spice::compiler
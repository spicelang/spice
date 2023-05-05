// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "Scope.h"

#include <ast/ASTNodes.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

Scope::~Scope() {
  // Reset fields
  parent = nullptr;
  codeLoc = nullptr;
  // Notify all parents, that the scope is de-allocated now
  for (Scope *p : parents) {
    if (p)
      for (auto &child : p->children)
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
  // Create copy
  const Scope *origChildScope = children.at(oldName);
  auto newChildBlock = new Scope(*origChildScope);

  // Add newChildBlock as parent of all children for tracking de-allocation
  for (const auto &child : newChildBlock->children)
    child.second->parents.push_back(newChildBlock);

  // Insert the new child scope as child to the current scope
  children.insert({newName, newChildBlock});
}

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
 * Get the number of fields if this is a struct scope
 *
 * @return Number of fields
 */
size_t Scope::getFieldCount() const {
  assert(type == SCOPE_STRUCT);
  size_t fieldCount = 0;
  for (const auto &symbol : symbolTable.symbols) {
    const SymbolType &symbolType = symbol.second.getType();
    if (!symbolType.isOneOf({TY_IMPORT, TY_FUNCTION, TY_PROCEDURE}))
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
 * Collect all warnings, produced within this scope
 *
 * @param List of warnings
 * @return Collection of warnings
 */
void Scope::collectWarnings(std::vector<CompilerWarning> &warnings) const { // NOLINT(misc-no-recursion)
  // Visit own symbols
  CompilerWarningType warningType;
  std::string warningMessage;
  for (const auto &[_, entry] : symbolTable.symbols) {
    // Do not produce a warning if the symbol is used or has a special name
    if (entry.used || entry.name.starts_with(UNUSED_VARIABLE_NAME))
      continue;

    switch (entry.getType().getSuperType()) {
    case TY_FUNCTION: {
      // Skip generic function entries
      if (!entry.getType().getTemplateTypes().empty())
        continue;

      if (type == SCOPE_GLOBAL) {
        warningType = UNUSED_FUNCTION;
        warningMessage = "The function '" + entry.declNode->getFctManifestations()->front()->getSignature() + "' is unused";
      } else {
        warningType = UNUSED_VARIABLE;
        warningMessage = "The variable '" + entry.name + "' is unused";
      }

      break;
    }
    case TY_PROCEDURE: {
      // Skip generic procedure entries
      if (!entry.getType().getTemplateTypes().empty())
        continue;

      if (type == SCOPE_GLOBAL) {
        warningType = UNUSED_PROCEDURE;
        warningMessage = "The procedure '" + entry.declNode->getFctManifestations()->front()->getSignature() + "' is unused";
      } else {
        warningType = UNUSED_VARIABLE;
        warningMessage = "The variable '" + entry.name + "' is unused";
      }

      break;
    }
    case TY_STRUCT: {
      if (entry.scope->type == SCOPE_GLOBAL) {
        // Skip generic struct entries
        if (!entry.getType().getTemplateTypes().empty())
          continue;

        warningType = UNUSED_STRUCT;
        warningMessage = "The struct '" + entry.name + "' is unused";
      } else {
        warningType = UNUSED_VARIABLE;
        warningMessage = "The variable '" + entry.name + "' is unused";
      }
      break;
    }
    case TY_INTERFACE: {
      warningType = UNUSED_INTERFACE;
      warningMessage = "The interface '" + entry.name + "' is unused";
      break;
    }
    case TY_IMPORT: {
      warningType = UNUSED_IMPORT;
      warningMessage = "The import '" + entry.name + "' is unused";
      break;
    }
    case TY_ENUM: {
      continue; // Do not report unused enums. Only unused enum items are reported
    }
    case TY_ALIAS: {
      warningType = UNUSED_ALIAS;
      warningMessage = "The type alias '" + entry.name + "' is unused";
      break;
    }
    default: {
      // Check parent scope type
      switch (type) {
      case SCOPE_STRUCT: {
        warningType = UNUSED_FIELD;
        warningMessage = "The field '" + entry.name + "' is unused";
        break;
      }
      case SCOPE_ENUM: {
        warningType = UNUSED_ENUM_ITEM;
        warningMessage = "The enum item '" + entry.name + "' is unused";
        break;
      }
      case SCOPE_FOREACH_BODY: {
        // Skip idx variables, otherwise fall-through
        if (entry.name == FOREACH_DEFAULT_IDX_VARIABLE_NAME)
          continue;
      }
      default: {
        warningType = UNUSED_VARIABLE;
        warningMessage = "The variable '" + entry.name + "' is unused";
      }
      }
    }
    }

    // Add warning
    warnings.emplace_back(entry.getDeclCodeLoc(), warningType, warningMessage);
  }

  // Visit children
  for (const auto &child : children)
    if (!child.second->isGenericScope)
      child.second->collectWarnings(warnings);
}

/**
 * Checks if all variables of this and all child scopes are of an explicit type.
 * This is executed after type inference to check that all variables could be inferred correctly.
 */
void Scope::checkSuccessfulTypeInference() const {
  // Check symbols in this scope
  for (auto &[name, entry] : symbolTable.symbols)
    if (entry.getType().is(TY_DYN))
      throw SemanticError(entry.declNode, UNEXPECTED_DYN_TYPE, "For the variable '" + name + "' no type could be inferred");

  // Check child scopes
  for (auto &[_, scope] : children)
    scope->checkSuccessfulTypeInference();
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
  for (const auto &child : children) {
    nlohmann::json c = child.second->getSymbolTableJSON();
    c["name"] = child.first; // Inject symbol table name into JSON object
    jsonChildren.emplace_back(c);
  }
  result["children"] = jsonChildren;

  return result;
}

} // namespace spice::compiler
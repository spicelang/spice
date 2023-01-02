// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <model/Interface.h>
#include <model/Struct.h>
#include <symboltablebuilder/SymbolTable.h>
#include <typechecker/FunctionManager.h>
#include <typechecker/StructManager.h>

namespace spice::compiler {

// Forward declarations
class FctDefNode;
class ProcDefNode;
class SourceFile;

enum ScopeType {
  SCOPE_GLOBAL,
  SCOPE_FUNC_PROC_BODY,
  SCOPE_STRUCT,
  SCOPE_INTERFACE,
  SCOPE_ENUM,
  SCOPE_IF_ELSE_BODY,
  SCOPE_WHILE_BODY,
  SCOPE_FOR_BODY,
  SCOPE_FOREACH_BODY,
  SCOPE_THREAD_BODY,
  SCOPE_UNSAFE_BODY,
  SCOPE_ANONYMOUS_BLOCK_BODY
};

/**
 * Represents an enclosed group of instructions, which are semantically separated from other scopes.
 * In the source code, scopes usually are written as curly braces.
 *
 * Following language structures use scopes:
 * - functions/procedures
 * - thread expressions
 * - unsafe block statements
 * - for loops
 * - foreach loops
 * - while loops
 * - if statements
 * - anonymous scopes
 */
class Scope {
public:
  // Constructors
  Scope(Scope *parent, SourceFile *sourceFile, const ScopeType &scopeType, const CodeLoc *codeLoc)
      : parent(parent), parents({parent}), sourceFile(sourceFile), type(scopeType), codeLoc(codeLoc) {}
  ~Scope();

  // Friend classes
  friend class FunctionManager;
  friend class StructManager;

  // Public methods
  // Scope management
  Scope *createChildScope(const std::string &scopeName, const ScopeType &scopeType, const CodeLoc *codeLoc);
  void renameChildScope(const std::string &oldName, const std::string &newName);
  void copyChildScope(const std::string &oldName, const std::string &newName);
  [[nodiscard]] Scope *getFunctionScope();
  [[nodiscard]] Scope *getChildScope(const std::string &scopeName) const;
  [[nodiscard]] std::vector<SymbolTableEntry *> getVarsGoingOutOfScope();
  void addParent(Scope *parent);

  // Generic types
  void insertGenericType(const std::string &typeName, const GenericType &genericType);
  GenericType *lookupGenericType(const std::string &typeName);

  // Interfaces
  Interface *lookupInterface(const std::string &interfaceName);
  void insertInterface(const Interface &interface);

  // Util
  void collectWarnings(std::vector<CompilerWarning> &warnings) const;
  [[nodiscard]] size_t getFieldCount() const;
  [[nodiscard]] size_t getLoopNestingDepth() const;
  [[nodiscard]] bool doesAllowUnsafeOperations() const;
  [[nodiscard]] bool isImportedBy(const Scope *askingScope) const;
  [[nodiscard]] nlohmann::json getSymbolTableJSON() const;

  // Wrapper methods for symbol table
  inline SymbolTableEntry *insert(const std::string &name, const SymbolSpecifiers &specifiers, ASTNode *declNode) {
    return symbolTable.insert(name, specifiers, declNode);
  }
  inline SymbolTableEntry *lookup(const std::string &symbolName) { return symbolTable.lookup(symbolName); }
  inline SymbolTableEntry *lookupStrict(const std::string &symbolName) { return symbolTable.lookupStrict(symbolName); }

  // Public members
  Scope *parent;
  SourceFile *sourceFile;
  std::unordered_map<std::string, Scope *> children;
  const ScopeType type;
  SymbolTable symbolTable = SymbolTable(parent == nullptr ? nullptr : &parent->symbolTable, this);
  const CodeLoc *codeLoc = nullptr;

private:
  // Private members
  std::vector<Scope *> parents;
  FunctionRegistry functions;
  StructRegistry structs;
  std::unordered_map<std::string, Interface> interfaces;
  std::unordered_map<std::string, GenericType> genericTypes;

  // Private methods
  Scope *searchForScope(const ScopeType &scopeType);
};

} // namespace spice::compiler
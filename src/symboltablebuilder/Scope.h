// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <model/Interface.h>
#include <model/Struct.h>
#include <symboltablebuilder/SymbolTable.h>
#include <typechecker/FunctionManager.h>
#include <typechecker/InterfaceManager.h>
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
  SCOPE_UNSAFE_BODY,
  SCOPE_ANONYMOUS_BLOCK_BODY
};

/**
 * Represents an enclosed group of instructions, which are semantically separated from other scopes.
 * In the source code, scopes usually are written as curly braces.
 *
 * Following language structures use scopes:
 * - global scope
 * - functions/procedures
 * - structs
 * - enums
 * - interfaces
 * - thread blocks
 * - unsafe blocks
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
      : parent(parent), sourceFile(sourceFile), type(scopeType), codeLoc(codeLoc) {}

  // Friend classes
  friend class FunctionManager;
  friend class StructManager;
  friend class InterfaceManager;

  // Public methods
  // Scope management
  Scope *createChildScope(const std::string &scopeName, const ScopeType &scopeType, const CodeLoc *codeLoc);
  void renameChildScope(const std::string &oldName, const std::string &newName);
  void copyChildScope(const std::string &oldName, const std::string &newName);
  [[nodiscard]] Scope *getChildScope(const std::string &scopeName) const;
  [[nodiscard]] std::vector<SymbolTableEntry *> getVarsGoingOutOfScope();

  // Generic types
  void insertGenericType(const std::string &typeName, const GenericType &genericType);
  GenericType *lookupGenericType(const std::string &typeName);

  // Util
  void collectWarnings(std::vector<CompilerWarning> &warnings) const;
  void checkSuccessfulTypeInference() const;
  [[nodiscard]] size_t getFieldCount() const;
  [[nodiscard]] size_t getLoopNestingDepth() const;
  [[nodiscard]] bool doesAllowUnsafeOperations() const;
  [[nodiscard]] bool isImportedBy(const Scope *askingScope) const;
  [[nodiscard]] nlohmann::json getSymbolTableJSON() const;

  // Wrapper methods for symbol table
  inline SymbolTableEntry *insert(const std::string &name, ASTNode *declNode) { return symbolTable.insert(name, declNode); }
  inline SymbolTableEntry *lookup(const std::string &symbolName) { return symbolTable.lookup(symbolName); }
  inline SymbolTableEntry *lookupStrict(const std::string &symbolName) { return symbolTable.lookupStrict(symbolName); }

  // Public members
  Scope *parent;
  SourceFile *sourceFile;
  std::unordered_map<std::string, std::shared_ptr<Scope>> children;
  const ScopeType type;
  SymbolTable symbolTable = SymbolTable(parent == nullptr ? nullptr : &parent->symbolTable, this);
  const CodeLoc *codeLoc = nullptr;
  bool isGenericScope = false;
  bool isDtorScope = false;

private:
  // Private members
  FunctionRegistry functions;
  StructRegistry structs;
  InterfaceRegistry interfaces;
  std::unordered_map<std::string, GenericType> genericTypes;
};

} // namespace spice::compiler
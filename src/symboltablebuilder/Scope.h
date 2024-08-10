// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

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

enum class ScopeType : uint8_t {
  GLOBAL,
  FUNC_PROC_BODY,
  LAMBDA_BODY,
  STRUCT,
  INTERFACE,
  ENUM,
  IF_ELSE_BODY,
  WHILE_BODY,
  FOR_BODY,
  FOREACH_BODY,
  CASE_BODY,
  DEFAULT_BODY,
  UNSAFE_BODY,
  ANONYMOUS_BLOCK_BODY
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
  Scope(Scope *parent, SourceFile *sourceFile, ScopeType scopeType, const CodeLoc *codeLoc)
      : parent(parent), sourceFile(sourceFile), codeLoc(codeLoc), type(scopeType) {}

  // Friend classes
  friend class FunctionManager;
  friend class StructManager;
  friend class InterfaceManager;

  // Public methods
  // Scope management
  Scope *createChildScope(const std::string &scopeName, ScopeType scopeType, const CodeLoc *declCodeLoc);
  void renameChildScope(const std::string &oldName, const std::string &newName);
  Scope *copyChildScope(const std::string &oldName, const std::string &newName);
  std::shared_ptr<Scope> deepCopyScope();
  [[nodiscard]] Scope *getChildScope(const std::string &scopeName) const;
  [[nodiscard]] std::vector<SymbolTableEntry *> getVarsGoingOutOfScope();

  // Generic types
  void insertGenericType(const std::string &typeName, const GenericType &genericType);
  GenericType *lookupGenericType(const std::string &typeName);

  // Util
  void collectWarnings(std::vector<CompilerWarning> &warnings) const;
  void ensureSuccessfulTypeInference() const;
  [[nodiscard]] size_t getFieldCount() const;
  [[nodiscard]] std::vector<Function *> getVirtualMethods();
  [[nodiscard]] std::vector<const Struct *> getAllStructManifestationsInDeclarationOrder() const;
  [[nodiscard]] bool hasRefFields();
  [[nodiscard]] unsigned int getLoopNestingDepth() const;
  [[nodiscard]] bool isInCaseBranch() const;
  [[nodiscard]] bool isInAsyncScope() const;
  [[nodiscard]] bool doesAllowUnsafeOperations() const;
  [[nodiscard]] bool isImportedBy(const Scope *askingScope) const;
  [[nodiscard]] nlohmann::json getSymbolTableJSON() const;

  // Wrapper methods for symbol table
  ALWAYS_INLINE SymbolTableEntry *insert(const std::string &name, ASTNode *declNode) {
    return symbolTable.insert(name, declNode);
  }
  ALWAYS_INLINE SymbolTableEntry *lookup(const std::string &symbolName) { return symbolTable.lookup(symbolName); }
  ALWAYS_INLINE SymbolTableEntry *lookupStrict(const std::string &symbolName) { return symbolTable.lookupStrict(symbolName); }
  ALWAYS_INLINE SymbolTableEntry *lookupField(unsigned int n) {
    assert(type == ScopeType::STRUCT);
    return symbolTable.lookupStrictByIndex(n);
  }

  // Public members
  Scope *parent;
  SourceFile *sourceFile;
  std::unordered_map<std::string, std::shared_ptr<Scope>> children;
  SymbolTable symbolTable = SymbolTable(parent == nullptr ? nullptr : &parent->symbolTable, this);
  const CodeLoc *codeLoc = nullptr;
  const ScopeType type;
  bool isGenericScope = false;
  bool isAsyncScope = false;
  bool isDtorScope = false;

private:
  // Private members
  FunctionRegistry functions;
  StructRegistry structs;
  InterfaceRegistry interfaces;
  std::unordered_map<std::string, GenericType> genericTypes;
};

} // namespace spice::compiler
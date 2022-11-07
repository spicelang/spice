// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <symbol/Interface.h>
#include <symbol/Struct.h>
#include <symbol/SymbolTable.h>

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
  SCOPE_ANONYMOUS
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
 * - if/elseif/else statements
 * - anonymous scopes
 */
class Scope {
public:
  // Constructors
  Scope(Scope *parent, const ScopeType &scopeType) : parent(parent), type(scopeType) {}
  ~Scope();

  // Public methods
  // Scope management
  Scope *createChildScope(const std::string &scopeName, const ScopeType &scopeType);
  void renameChildScope(const std::string &oldName, const std::string &newName);
  void copyChildScope(const std::string &oldName, const std::string &newName);
  [[nodiscard]] Scope *getGlobalScope();
  [[nodiscard]] Scope *getFunctionScope();
  [[nodiscard]] Scope *getChildScope(const std::string &scopeName) const;
  [[nodiscard]] std::vector<SymbolTableEntry *> getVarsGoingOutOfScope();

  // Generic types
  void insertGenericType(const std::string &typeName, const GenericType &genericType);
  GenericType *lookupGenericType(const std::string &typeName);

  // Functions
  Function *insertFunction(const Function &function);
  Function *matchFunction(SymbolTable *currentScope, const std::string &callFunctionName, const SymbolType &callThisType,
                          const std::vector<SymbolType> &callTemplateTypes, const std::vector<SymbolType> &callArgTypes,
                          const ASTNode *node);
  [[nodiscard]] std::unordered_map<std::string, Function> *getFunctionManifestations(const CodeLoc &defCodeLoc);
  Function *insertSubstantiatedFunction(const Function &function, const ASTNode *declNode);

  // Structs
  Struct *insertStruct(const Struct &spiceStruct);
  Struct *matchStruct(SymbolTable *currentScope, const std::string &structName, const std::vector<SymbolType> &templateTypes,
                      const ASTNode *node);
  [[nodiscard]] std::unordered_map<std::string, Struct> *getStructManifestations(const CodeLoc &defCodeLoc);
  Struct *insertSubstantiatedStruct(const Struct &s, const ASTNode *declNode);

  // Interfaces
  Interface *lookupInterface(const std::string &interfaceName);
  void insertInterface(const Interface &interface);

  // Util
  [[nodiscard]] std::vector<CompilerWarning> collectWarnings() const;
  [[nodiscard]] size_t getFieldCount() const;
  [[nodiscard]] size_t getLoopNestingDepth() const;
  [[nodiscard]] bool doesAllowUnsafeOperations() const;

  // Wrapper methods for symbol table
  inline SymbolTableEntry *insert(const std::string &name, const SymbolType &symbolType, const SymbolSpecifiers &specifiers,
                                  const ASTNode *declNode);
  inline SymbolTableEntry *lookup(const std::string &symbolName);

  // Public members
  Scope *parent;
  std::unordered_map<std::string, Scope *> children;
  const ScopeType type;
  SymbolTable symbolTable = SymbolTable(parent == nullptr ? nullptr : &parent->symbolTable);

private:
  // Private members
  std::unordered_map<std::string, std::unordered_map<std::string, Function>> functions; // <code-loc-str, map-of-representations>
  std::unordered_map<std::string, std::unordered_map<std::string, Struct>> structs;     // <code-loc-str, map-of-representations>
  std::unordered_map<std::string, Interface> interfaces;
  std::unordered_map<std::string, GenericType> genericTypes;

  // Private methods
  Scope *searchForScope(const ScopeType &scopeType);
};
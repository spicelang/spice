// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <queue>
#include <string>
#include <vector>

#include <symbol/Capture.h>
#include <symbol/GenericType.h>
#include <symbol/SymbolTableEntry.h>

#include "../../lib/json/json.hpp"

// Forward declarations
class Function;
class Struct;
class SymbolSpecifiers;
class SymbolType;
class ErrorFactory;
struct CodeLoc;

enum ScopeType {
  SCOPE_GLOBAL,
  SCOPE_FUNC_PROC_BODY,
  SCOPE_STRUCT,
  SCOPE_ENUM,
  SCOPE_IF_BODY,
  SCOPE_WHILE_BODY,
  SCOPE_FOR_BODY,
  SCOPE_FOREACH_BODY,
  SCOPE_THREAD_BODY,
  SCOPE_UNSAFE_BODY
};

/**
 * Class for storing information about symbols of the AST. Symbol tables are meant to be arranged in a tree structure,
 * so that you can navigate with the getParent() and getChild() methods up and down the tree.
 */
class SymbolTable {
public:
  // Constructors
  explicit SymbolTable(SymbolTable *parent, const ScopeType &scopeType, bool inMainSourceFile = false,
                       bool isSourceFileRoot = false)
      : parent(parent), scopeType(scopeType), isMainSourceFile(inMainSourceFile), isSourceFileRootScope(isSourceFileRoot){};

  // Public methods
  void insert(const std::string &name, const SymbolType &type, SymbolSpecifiers specifiers, SymbolState state,
              const AstNode *declNode);
  void addCapture(const std::string &name, const Capture &capture);
  SymbolTableEntry *lookup(const std::string &symbolName);
  SymbolTableEntry *lookupStrict(const std::string &symbolName);
  SymbolTableEntry *lookupByIndex(unsigned int orderIndex);
  SymbolTableEntry *lookupGlobal(const std::string &globalName, bool skipThisScope = false);
  Capture *lookupCapture(const std::string &symbolName);
  Capture *lookupCaptureStrict(const std::string &symbolName);
  SymbolTable *lookupTable(const std::string &tableName);
  SymbolTable *createChildBlock(const std::string &childBlockName, const ScopeType &type);
  void insertGenericType(const std::string &typeName, const GenericType &genericType);
  GenericType *lookupGenericType(const std::string &typeName);
  void mountChildBlock(const std::string &childBlockName, SymbolTable *symbolTable, bool alterParent = true);
  void renameChildBlock(const std::string &oldName, const std::string &newName);
  void copyChildBlock(const std::string &originalChildBlockName, const std::string &newChildBlockName);

  void setParent(SymbolTable *parentTable);
  [[nodiscard]] SymbolTable *getParent() const;
  SymbolTable *getChild(const std::string &tableName);

  std::vector<SymbolTableEntry *> getVarsGoingOutOfScope(bool filterForStructs = false);

  std::map<std::string, SymbolTableEntry> &getSymbols();
  std::map<std::string, Capture> &getCaptures();
  [[nodiscard]] size_t getFieldCount() const;

  void insertFunction(const Function &function, ErrorFactory *err);
  Function *matchFunction(SymbolTable *currentScope, const std::string &callFunctionName, const SymbolType &callThisType,
                          const std::vector<SymbolType> &callArgTypes, ErrorFactory *errorFactory, const CodeLoc &codeLoc);
  [[nodiscard]] std::map<std::string, Function> *getFunctionManifestations(const CodeLoc &defCodeLoc) const;
  void insertFunctionAccessPointer(const CodeLoc &codeLoc, Function *spiceFunc);
  Function *getFunctionAccessPointer(const CodeLoc &codeLoc);
  void insertSubstantiatedFunction(const Function &function, const AstNode *declNode);

  void insertStruct(const Struct &s, ErrorFactory *err);
  Struct *matchStruct(SymbolTable *currentScope, const std::string &structName, const std::vector<SymbolType> &templateTypes,
                      ErrorFactory *errorFactory, const CodeLoc &codeLoc);
  [[nodiscard]] std::map<std::string, Struct> *getStructManifestations(const CodeLoc &defCodeLoc) const;
  void insertStructAccessPointer(const CodeLoc &codeLoc, Struct *spiceStruct);
  Struct *getStructAccessPointer(const CodeLoc &codeLoc);
  void insertSubstantiatedStruct(const Struct &s, ErrorFactory *err, const AstNode *declNode);

  void purgeSubstantiationRemnants();

  void printCompilerWarnings();
  void disableCompilerWarnings();

  [[nodiscard]] bool isImported(const SymbolTable *askingScope) const;

  [[nodiscard]] ScopeType getScopeType() const;

  void setCapturingRequired();

  [[nodiscard]] nlohmann::json toJSON() const;

private:
  // Members
  SymbolTable *parent;
  const ScopeType scopeType;
  std::map<std::string, SymbolTable *> children;
  std::map<std::string, SymbolTableEntry> symbols;
  std::map<std::string, Capture> captures;
  std::map<std::string, GenericType> genericTypes;
  std::map<std::string, std::shared_ptr<std::map<std::string, Function>>> functions; // <code-loc, vector-of-representations>
  std::map<std::string, Function *> functionAccessPointers;
  std::map<std::string, std::shared_ptr<std::map<std::string, Struct>>> structs; // <code-loc, vector-of-representations>
  std::map<std::string, Struct *> structAccessPointers;
  bool isMainSourceFile;
  bool isSourceFileRootScope = false;
  bool compilerWarningsEnabled = true;
  bool requiresCapturing = false;
};
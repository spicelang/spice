// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <queue>
#include <string>
#include <vector>

#include <symbol/Capture.h>
#include <symbol/GenericType.h>
#include <symbol/SymbolTableEntry.h>
#include <util/CompilerWarning.h>

#include "../../lib/json/json.hpp"

// Forward declarations
class Function;
class Struct;
class SymbolSpecifiers;
class SymbolType;
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
  void insertAnonymous(const SymbolType &type, const AstNode *declNode);
  void addCapture(const std::string &name, const Capture &capture);
  SymbolTableEntry *lookup(const std::string &symbolName);
  SymbolTableEntry *lookupStrict(const std::string &symbolName);
  SymbolTableEntry *lookupByIndex(unsigned int orderIndex);
  SymbolTableEntry *lookupGlobal(const std::string &globalName, bool skipThisScope = false);
  SymbolTableEntry *lookupAnonymous(const CodeLoc &codeLoc);
  Capture *lookupCapture(const std::string &symbolName);
  Capture *lookupCaptureStrict(const std::string &symbolName);
  SymbolTable *lookupTable(const std::string &tableName);
  SymbolTable *createChildBlock(const std::string &childBlockName, const ScopeType &type);
  void insertGenericType(const std::string &typeName, const GenericType &genericType);
  GenericType *lookupGenericType(const std::string &typeName);
  void mountChildBlock(const std::string &childBlockName, SymbolTable *symbolTable, bool alterParent = true);
  void renameChildBlock(const std::string &oldName, const std::string &newName);
  void copyChildBlock(const std::string &originalChildBlockName, const std::string &newChildBlockName);

  SymbolTable *getChild(const std::string &tableName);

  std::vector<SymbolTableEntry *> getVarsGoingOutOfScope(bool filterForDtorStructs = false);

  std::map<std::string, SymbolTableEntry> &getSymbols();
  std::map<std::string, Capture> &getCaptures();
  [[nodiscard]] size_t getFieldCount() const;

  void insertFunction(const Function &function);
  Function *matchFunction(SymbolTable *currentScope, const std::string &callFunctionName, const SymbolType &callThisType,
                          const std::vector<SymbolType> &callArgTypes, const AstNode *node);
  [[nodiscard]] std::map<std::string, Function> *getFunctionManifestations(const CodeLoc &defCodeLoc) const;
  void insertFunctionAccessPointer(Function *spiceFunc, const CodeLoc &codeLoc, const std::string &suffix);
  Function *getFunctionAccessPointer(const CodeLoc &codeLoc, const std::string &suffix = "");
  void insertSubstantiatedFunction(const Function &function, const AstNode *declNode);

  void insertStruct(const Struct &s);
  Struct *matchStruct(SymbolTable *currentScope, const std::string &structName, const std::vector<SymbolType> &templateTypes,
                      const AstNode *node);
  [[nodiscard]] std::map<std::string, Struct> *getStructManifestations(const CodeLoc &defCodeLoc) const;
  void insertStructAccessPointer(const CodeLoc &codeLoc, Struct *spiceStruct);
  Struct *getStructAccessPointer(const CodeLoc &codeLoc);
  void insertSubstantiatedStruct(const Struct &s, const AstNode *declNode);

  void purgeSubstantiationRemnants();

  std::vector<CompilerWarning> collectWarnings();

  [[nodiscard]] bool isImported(const SymbolTable *askingScope) const;

  [[nodiscard]] nlohmann::json toJSON() const;

  // Public members
  SymbolTable *parent;
  const ScopeType scopeType;
  bool isSourceFileRootScope = false;
  bool isShadowTable = false;
  bool isCapturingRequired = false;

private:
  // Members
  std::map<std::string, SymbolTable *> children;
  std::map<std::string, SymbolTableEntry> symbols;
  std::map<std::string, Capture> captures;
  std::map<std::string, GenericType> genericTypes;
  std::map<std::string, std::shared_ptr<std::map<std::string, Function>>> functions; // <code-loc, vector-of-representations>
  std::map<std::string, Function *> functionAccessPointers;
  std::map<std::string, std::shared_ptr<std::map<std::string, Struct>>> structs; // <code-loc, vector-of-representations>
  std::map<std::string, Struct *> structAccessPointers;
  bool isMainSourceFile;
};
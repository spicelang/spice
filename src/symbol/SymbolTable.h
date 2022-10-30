// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <queue>
#include <string>
#include <vector>

#include <symbol/Capture.h>
#include <symbol/GenericType.h>
#include <symbol/Interface.h>
#include <symbol/SymbolTableEntry.h>
#include <util/CompilerWarning.h>

#include "../../lib/json/json.hpp"

// Forward declarations
class SymbolSpecifiers;
class SymbolType;
struct CodeLoc;

/**
 * Class for storing information about symbols of the AST. Symbol tables are meant to be arranged in a tree structure,
 * so that you can navigate with the .parent property and getChild() method up and down the tree.
 */
class SymbolTable {
public:
  // Constructors
  explicit SymbolTable(SymbolTable *parent, bool inMainSourceFile = false) : parent(parent), isMainSourceFile(inMainSourceFile){};

  // Friend classes
  friend class Scope;

  // Public methods
  void insert(const std::string &name, const SymbolType &type, const SymbolSpecifiers &specifiers, const ASTNode *declNode);
  void insertAnonymous(const SymbolType &type, const ASTNode *declNode);
  void insertCapture(const std::string &name, const Capture &capture);
  SymbolTableEntry *lookup(const std::string &symbolName);
  SymbolTableEntry *lookupStrict(const std::string &symbolName);
  SymbolTableEntry *lookupByIndex(unsigned int orderIndex);
  SymbolTableEntry *lookupGlobal(const std::string &globalName, bool skipThisScope = false);
  SymbolTableEntry *lookupAnonymous(const CodeLoc &codeLoc);
  Capture *lookupCapture(const std::string &symbolName);
  Capture *lookupCaptureStrict(const std::string &symbolName);
  void insertGenericType(const std::string &typeName, const GenericType &genericType);
  GenericType *lookupGenericType(const std::string &typeName);
  [[nodiscard]] nlohmann::json toJSON() const;

  // Public members
  SymbolTable *parent;
  bool isShadowTable = false;
  bool isCapturingRequired = false;

private:
  // Members
  std::map<std::string, SymbolTable *> children;
  std::map<std::string, SymbolTableEntry> symbols;
  std::map<std::string, Capture> captures;
  std::map<std::string, GenericType> genericTypes;
  bool isMainSourceFile;
};
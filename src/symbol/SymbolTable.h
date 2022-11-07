// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include <symbol/Capture.h>
#include <symbol/GenericType.h>
#include <symbol/SymbolTableEntry.h>
#include <util/CompilerWarning.h>

#include "../../lib/json/json.hpp"

// Forward declarations
class SymbolSpecifiers;
class SymbolType;
struct CodeLoc;

/**
 * Class for storing information about symbols of the program.
 * Symbol tables are arranged in a tree structure, so that you can navigate with the .parent property and getChild() method up
 * and down the tree.
 */
class SymbolTable {
public:
  // Constructors
  explicit SymbolTable(SymbolTable *parent) : parent(parent){};

  // Friend classes
  friend class Scope;

  // Public methods
  SymbolTableEntry *insert(const std::string &name, const SymbolType &symbolType, const SymbolSpecifiers &specifiers,
                           const ASTNode *declNode);
  SymbolTableEntry *insertAnonymous(const SymbolType &type, const ASTNode *declNode);
  void insertCapture(const std::string &name, const Capture &capture);
  SymbolTableEntry *lookup(const std::string &symbolName);
  SymbolTableEntry *lookupStrict(const std::string &symbolName);
  SymbolTableEntry *lookupByIndex(unsigned int orderIndex);
  SymbolTableEntry *lookupAnonymous(const CodeLoc &codeLoc);
  Capture *lookupCapture(const std::string &symbolName);
  Capture *lookupCaptureStrict(const std::string &symbolName);
  [[nodiscard]] nlohmann::json toJSON() const;

  // Public members
  SymbolTable *parent;
  std::unordered_map<std::string, SymbolTable *> children;
  std::unordered_map<std::string, SymbolTableEntry> symbols;
  std::unordered_map<std::string, Capture> captures;
  bool capturingRequired = false;
};
// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include <model/GenericType.h>
#include <symboltablebuilder/Capture.h>
#include <symboltablebuilder/SymbolTableEntry.h>
#include <util/CompilerWarning.h>

#include "../../lib/json/json.hpp"

namespace spice::compiler {

// Forward declarations
class Scope;
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
  SymbolTable(SymbolTable *parent, Scope *scope) : parent(parent), scope(scope){};

  // Friend classes
  friend class Scope;

  // Public methods
  SymbolTableEntry *insert(const std::string &name, ASTNode *declNode);
  SymbolTableEntry *insertAnonymous(const SymbolType &type, ASTNode *declNode);
  void copySymbol(const std::string &originalName, const std::string &newName);
  SymbolTableEntry *lookup(const std::string &symbolName);
  SymbolTableEntry *lookupStrict(const std::string &symbolName);
  SymbolTableEntry *lookupByIndex(unsigned int orderIndex);
  SymbolTableEntry *lookupAnonymous(const CodeLoc &codeLoc);
  Capture *lookupCapture(const std::string &symbolName);
  Capture *lookupCaptureStrict(const std::string &symbolName);
  void deleteAnonymous(const std::string &name);
  [[nodiscard]] nlohmann::json toJSON() const;

  // Public members
  SymbolTable *parent;
  Scope *scope;
  std::unordered_map<std::string, SymbolTableEntry> symbols;
  std::unordered_map<std::string, Capture> captures;
  bool capturingRequired = false;
};

} // namespace spice::compiler
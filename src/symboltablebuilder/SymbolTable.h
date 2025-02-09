// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <model/GenericType.h>
#include <symboltablebuilder/Capture.h>
#include <symboltablebuilder/SymbolTableEntry.h>

#include "../../lib/json/json.hpp"

namespace spice::compiler {

// Forward declarations
class Scope;
class Type;
struct CodeLoc;

using CaptureMap = std::unordered_map<std::string /*name*/, Capture /*capture*/>;
using SymbolMap = std::unordered_map<std::string /*name*/, SymbolTableEntry /*entry*/>;

/**
 * Class for storing information about symbols of the program.
 * Symbol tables are arranged in a tree structure, so that you can navigate with the .parent property and getChild() method up
 * and down the tree.
 */
class SymbolTable {
public:
  // Constructors
  SymbolTable(SymbolTable *parent, Scope *scope) : parent(parent), scope(scope) {}

  // Friend classes
  friend class Scope;

  // Public methods
  SymbolTableEntry *insert(const std::string &name, ASTNode *declNode, bool isAnonymousSymbol = false);
  SymbolTableEntry *insertAnonymous(const QualType &qualType, ASTNode *declNode, size_t numericSuffix = 0);
  SymbolTableEntry *copySymbol(const std::string &originalName, const std::string &newName);
  SymbolTableEntry *lookup(const std::string &name);
  std::pair<SymbolTableEntry *, bool> lookupWithAliasResolution(const std::string &name);
  SymbolTableEntry *lookupStrict(const std::string &symbolName);
  SymbolTableEntry *lookupInComposedFields(const std::string &name, std::vector<size_t> &indexPath);
  SymbolTableEntry *lookupStrictByIndex(unsigned int orderIndex);
  SymbolTableEntry *lookupAnonymous(const CodeLoc &codeLoc, size_t numericSuffix = 0);
  Capture *lookupCapture(const std::string &name);
  Capture *lookupCaptureStrict(const std::string &name);
  void setCapturingRequired();
  void deleteAnonymous(const std::string &name);
  [[nodiscard]] nlohmann::json toJSON() const;

  // Public members
  SymbolTable *parent;
  Scope *scope;
  SymbolMap symbols;
  CaptureMap captures;
  bool capturingRequired = false;
};

} // namespace spice::compiler
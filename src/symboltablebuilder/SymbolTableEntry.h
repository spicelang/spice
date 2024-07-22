// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <stack>
#include <string>
#include <utility>

#include <symboltablebuilder/Lifecycle.h>
#include <symboltablebuilder/QualType.h>
#include <symboltablebuilder/Type.h>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Value.h>

#include "../../lib/json/json.hpp"

namespace spice::compiler {

// Forward declarations
class Scope;
class ASTNode;
struct CodeLoc;
struct CompileTimeValue;

/**
 * Entry of a symbol table, representing an individual symbol with all its properties
 */
class SymbolTableEntry {
public:
  // Constructors
  SymbolTableEntry(std::string name, const QualType &qualType, Scope *scope, ASTNode *declNode, size_t orderIndex, bool global)
      : name(std::move(name)), scope(scope), declNode(declNode), orderIndex(orderIndex), global(global), qualType(qualType){};

  // Public methods
  [[nodiscard]] const QualType &getQualType() const;
  void updateType(const QualType &newType, bool overwriteExistingType);
  void updateState(const LifecycleState &newState, const ASTNode *node, bool force = false);
  [[nodiscard]] const CodeLoc &getDeclCodeLoc() const;
  [[nodiscard]] virtual llvm::Value *getAddress() const;
  void updateAddress(llvm::Value *address);
  void pushAddress(llvm::Value *address);
  void popAddress();
  [[nodiscard]] bool isField() const;
  [[nodiscard]] const Lifecycle &getLifecycle() const { return lifecycle; }
  [[nodiscard]] bool isInitialized() const { return lifecycle.isInitialized(); }
  [[nodiscard]] nlohmann::ordered_json toJSON() const;

  // Public members
  const std::string name;
  Scope *scope;
  ASTNode *declNode;
  const size_t orderIndex;
  const bool global;
  bool isVolatile = false;
  bool isParam = false;
  bool anonymous = false;
  bool used = false;
  bool omitDtorCall = false;
  bool isImplicitField = false;

private:
  // Members
  QualType qualType;
  std::stack<llvm::Value *> memAddress;
  Lifecycle lifecycle;
};

} // namespace spice::compiler
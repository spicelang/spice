// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include <borrowchecker/Lifecycle.h>
#include <symboltablebuilder/SymbolType.h>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Value.h>

#include "../../lib/json/json.hpp"

namespace spice::compiler {

// Forward declarations
class Scope;
class ASTNode;
struct CodeLoc;
union CompileTimeValue;

/**
 * Entry of a symbol table, representing an individual symbol with all its properties
 */
class SymbolTableEntry {
public:
  // Constructors
  SymbolTableEntry(std::string name, SymbolType type, Scope *scope, ASTNode *declNode, size_t orderIndex, const bool global)
      : name(std::move(name)), type(std::move(type)), scope(scope), declNode(declNode), orderIndex(orderIndex), global(global){};

  // Public methods
  [[nodiscard]] const SymbolType &getType() const;
  void updateType(const SymbolType &newType, bool overwriteExistingType);
  void updateState(const LifecycleState &newState, ASTNode *node, bool force = false);
  [[nodiscard]] const CodeLoc &getDeclCodeLoc() const;
  [[nodiscard]] llvm::StructType *getStructLLVMType() const;
  void setStructLLVMType(llvm::StructType *newStructType);
  [[nodiscard]] virtual llvm::Value *getAddress() const;
  void updateAddress(llvm::Value *address);
  [[nodiscard]] bool isField() const;
  [[nodiscard]] bool isInitialized() const { return lifecycle.isInitialized(); }
  [[nodiscard]] bool isDead() const { return lifecycle.isDead(); }
  [[nodiscard]] nlohmann::ordered_json toJSON() const;

  // Public members
  const std::string name;
  Scope *scope;
  ASTNode *declNode;
  const size_t orderIndex;
  const bool global;
  const CompileTimeValue *compileTimeValue = nullptr;
  bool isVolatile = false;
  bool isParam = false;
  bool anonymous = false;
  bool used = false;
  bool omitDtorCall = false;

private:
  // Members
  SymbolType type;
  llvm::StructType *llvmStructType = nullptr;
  std::stack<llvm::Value *> memAddress;
  Lifecycle lifecycle;
};

} // namespace spice::compiler
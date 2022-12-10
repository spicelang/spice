// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include "SymbolSpecifiers.h"
#include "SymbolType.h"

#include <llvm/IR/Value.h>

#include "../../lib/json/json.hpp"
#include "borrowchecker/Lifecycle.h"

// Forward declarations
class Scope;
class ASTNode;
struct CodeLoc;

/**
 * Entry of a symbol table, representing an individual symbol with all its properties
 */
class SymbolTableEntry {
public:
  // Constructors
  SymbolTableEntry(std::string name, SymbolType type, SymbolTable *symbolTable, SymbolSpecifiers specifiers, ASTNode *declNode,
                   size_t orderIndex, const bool global)
      : name(std::move(name)), type(std::move(type)), symbolTable(symbolTable), specifiers(specifiers), declNode(declNode),
        orderIndex(orderIndex), global(global){};

  // Public methods
  [[nodiscard]] const SymbolType &getType() const;
  void updateType(const SymbolType &newType, bool overwriteExistingType);
  void updateState(const LifecycleState &newState, ASTNode *node, bool force = false);
  [[nodiscard]] const ASTNode *getDeclNode() const;
  [[nodiscard]] const CodeLoc &getDeclCodeLoc() const;
  [[nodiscard]] llvm::Type *getStructLLVMType() const;
  void setStructLLVMType(llvm::Type *newStructType);
  [[nodiscard]] virtual llvm::Value *getAddress() const;
  void updateAddress(llvm::Value *address);
  void pushAddress(llvm::Value *address);
  void popAddress();
  [[nodiscard]] bool isDead() const { return lifecycle.isDead(); }
  [[nodiscard]] nlohmann::ordered_json toJSON() const;

  // Public members
  const std::string name;
  SymbolTable *symbolTable;
  SymbolSpecifiers specifiers;
  ASTNode *declNode;
  size_t orderIndex;
  const bool global;
  bool isVolatile = false;
  bool anonymous = false;
  bool used = false;

private:
  // Members
  SymbolType type;
  llvm::Type *llvmType = nullptr;
  std::stack<llvm::Value *> memAddress;
  Lifecycle lifecycle;
};
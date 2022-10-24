// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include <symbol/SymbolSpecifiers.h>
#include <symbol/SymbolType.h>

#include <llvm/IR/Value.h>

#include "../../lib/json/json.hpp"

// Forward declarations
class SymbolTable;
struct AstNode;
struct CodeLoc;

enum SymbolState { DECLARED, INITIALIZED };

/**
 * Entry of a symbol table, representing an individual symbol with all its properties
 */
class SymbolTableEntry {
public:
  // Constructors
  SymbolTableEntry(std::string name, SymbolType type, SymbolTable *scope, SymbolSpecifiers specifiers, SymbolState state,
                   const AstNode *declNode, size_t orderIndex, const bool global)
      : name(std::move(name)), type(std::move(type)), scope(scope), specifiers(specifiers), state(state), declNode(declNode),
        orderIndex(orderIndex), isGlobal(global){};

  // Public methods
  void updateType(const SymbolType &newType, bool force);
  void updateState(SymbolState newState, const AstNode *node, bool force = false);
  [[nodiscard]] const CodeLoc &getDeclCodeLoc() const;
  [[nodiscard]] llvm::Type *getStructLLVMType() const;
  void setStructLLVMType(llvm::Type *newStructType);
  [[nodiscard]] virtual llvm::Value *getAddress() const;
  void updateAddress(llvm::Value *address);
  void pushAddress(llvm::Value *address);
  void popAddress();
  [[nodiscard]] nlohmann::ordered_json toJSON() const;

  // Public members
  const std::string name;
  SymbolType type;
  SymbolTable *scope;
  SymbolSpecifiers specifiers;
  SymbolState state;
  const AstNode *declNode;
  size_t orderIndex;
  const bool isGlobal;
  bool isVolatile = false;
  bool isUsed = false;

private:
  // Members
  llvm::Type *llvmType = nullptr;
  std::stack<llvm::Value *> memAddress;
};
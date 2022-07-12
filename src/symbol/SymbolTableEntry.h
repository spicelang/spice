// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include <Token.h>

#include <symbol/SymbolSpecifiers.h>
#include <symbol/SymbolType.h>

#include <llvm/IR/Value.h>

#include "../../lib/json/json.hpp"

// Forward declarations
class SymbolTable;
class ErrorFactory;

enum SymbolState { DECLARED, INITIALIZED };

/**
 * Entry of a symbol table, representing an individual symbol with all its properties
 */
class SymbolTableEntry {
public:
  // Constructors
  SymbolTableEntry(std::string name, const SymbolType &type, SymbolTable *scope, SymbolSpecifiers specifiers, SymbolState state,
                   const antlr4::Token &declToken, unsigned int orderIndex, const bool global)
      : name(std::move(name)), type(type), scope(scope), specifiers(specifiers), state(state), declToken(declToken),
        orderIndex(orderIndex), global(global){};

  // Public methods
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] const SymbolType &getType() const;
  void updateType(const SymbolType &newType, bool force);
  [[nodiscard]] const SymbolTable *getScope() const;
  [[nodiscard]] SymbolSpecifiers getSpecifiers() const;
  [[nodiscard]] SymbolState getState() const;
  void updateState(SymbolState newState, const ErrorFactory *errorFactory, const antlr4::Token &token, bool force = false);
  [[nodiscard]] const antlr4::Token &getDeclToken() const;
  [[nodiscard]] llvm::Type *getLLVMType() const;
  void updateLLVMType(llvm::Type *newType);
  [[nodiscard]] virtual llvm::Value *getAddress() const;
  void updateAddress(llvm::Value *address);
  void pushAddress(llvm::Value *address);
  void popAddress();
  [[nodiscard]] unsigned int getOrderIndex() const;
  [[nodiscard]] bool isGlobal() const;
  [[nodiscard]] bool isVolatile() const;
  void setVolatile(bool volatility = true);
  [[nodiscard]] bool isUsed() const;
  void setUsed();
  [[nodiscard]] nlohmann::ordered_json toJSON() const;

private:
  // Members
  const std::string name;
  SymbolType type;
  const SymbolTable *scope;
  SymbolSpecifiers specifiers;
  llvm::Type *llvmType = nullptr;
  SymbolState state;
  size_t referenceCount = 0;
  const antlr4::Token &declToken;
  std::stack<llvm::Value *> memAddress;
  unsigned int orderIndex;
  const bool global;
  bool volatility = false;
  bool used = false;
};
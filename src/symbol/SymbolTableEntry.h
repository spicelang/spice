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
class ErrorFactory;
struct CodeLoc;

enum SymbolState { DECLARED, INITIALIZED };

/**
 * Entry of a symbol table, representing an individual symbol with all its properties
 */
class SymbolTableEntry {
public:
  // Constructors
  SymbolTableEntry(std::string name, const SymbolType &type, SymbolTable *scope, SymbolSpecifiers specifiers, SymbolState state,
                   const CodeLoc &declCodeLoc, unsigned int orderIndex, const bool global)
      : name(std::move(name)), type(type), scope(scope), specifiers(specifiers), state(state), declCodeLoc(declCodeLoc),
        orderIndex(orderIndex), global(global){};

  // Public methods
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] const SymbolType &getType() const;
  void updateType(const SymbolType &newType, bool force);
  [[nodiscard]] SymbolTable *getScope() const;
  [[nodiscard]] SymbolSpecifiers getSpecifiers() const;
  [[nodiscard]] SymbolState getState() const;
  void updateState(SymbolState newState, const ErrorFactory *errorFactory, const CodeLoc &codeLoc, bool force = false);
  [[nodiscard]] const CodeLoc &getDeclCodeLoc() const;
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
  //[[nodiscard]] size_t getRefCount() const;
  // void increaseRefCount();
  [[nodiscard]] bool isUsed() const;
  void setUsed();
  [[nodiscard]] nlohmann::ordered_json toJSON() const;

private:
  // Members
  const std::string name;
  SymbolType type;
  SymbolTable *scope;
  SymbolSpecifiers specifiers;
  llvm::Type *llvmType = nullptr;
  SymbolState state;
  // size_t refCount = 0;
  const CodeLoc &declCodeLoc;
  std::stack<llvm::Value *> memAddress;
  unsigned int orderIndex;
  const bool global;
  bool volatility = false;
  bool used = false;
};
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include "Token.h"

#include <symbol/SymbolType.h>
#include <symbol/SymbolSpecifiers.h>

#include <llvm/IR/Value.h>

#include "../../lib/json/json.hpp"

enum SymbolState {
    DECLARED,
    INITIALIZED
};

/**
 * Entry of a symbol table, representing an individual symbol with all its properties
 */
class SymbolTableEntry {
public:
    // Constructors
    SymbolTableEntry(std::string name, SymbolType type, SymbolSpecifiers specifiers, SymbolState state,
                     const antlr4::Token& token, unsigned int orderIndex, const bool isGlobal) :
            name(std::move(name)), type(std::move(type)), specifiers(specifiers), state(state), definitionToken(token),
            orderIndex(orderIndex), isGlobal(isGlobal) {};

    // Public methods
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] SymbolType getType() const;
    void updateType(SymbolType newType, bool force);
    [[nodiscard]] SymbolSpecifiers getSpecifiers() const;
    [[nodiscard]] SymbolState getState() const;
    void updateState(SymbolState newState, const ErrorFactory* errorFactory, const antlr4::Token& token);
    [[nodiscard]] const antlr4::Token& getDefinitionToken() const;
    [[nodiscard]] llvm::Type* getLLVMType() const;
    void updateLLVMType(llvm::Type* newType);
    [[nodiscard]] virtual llvm::Value* getAddress() const;
    void updateAddress(llvm::Value* address);
    void pushAddress(llvm::Value* address);
    void popAddress();
    [[nodiscard]] unsigned int getOrderIndex() const;
    [[nodiscard]] bool isLocal() const;
    [[nodiscard]] bool isUsed() const;
    void setUsed();
    [[nodiscard]] nlohmann::ordered_json toJSON() const;

private:
    // Members
    const std::string name;
    SymbolType type;
    SymbolSpecifiers specifiers;
    llvm::Type* llvmType = nullptr;
    SymbolState state;
    const antlr4::Token& definitionToken;
    std::stack<llvm::Value*> memAddress;
    unsigned int orderIndex;
    const bool isGlobal;
    bool used = false;
};
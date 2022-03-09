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
    std::string getName();
    SymbolType getType();
    void updateType(SymbolType newType, bool force);
    SymbolSpecifiers getSpecifiers();
    SymbolState getState();
    void updateState(SymbolState newState, ErrorFactory* errorFactory, const antlr4::Token& token);
    const antlr4::Token& getDefinitionToken();
    llvm::Type* getLLVMType();
    void updateLLVMType(llvm::Type* newType);
    virtual llvm::Value* getAddress();
    void updateAddress(llvm::Value* address);
    void pushAddress(llvm::Value* address);
    void popAddress();
    [[nodiscard]] unsigned int getOrderIndex() const;
    [[nodiscard]] bool isLocal() const;
    [[nodiscard]] bool isUsed() const;
    void setUsed();
    nlohmann::ordered_json toJSON();

private:
    // Members
    std::string name;
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
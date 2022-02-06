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
            orderIndex(orderIndex), isGlobal(isGlobal), used(false) {};

    // Public methods
    std::string getName();
    SymbolType getType();
    SymbolSpecifiers getSpecifiers();
    SymbolState getState();
    const antlr4::Token& getDefinitionToken();
    llvm::Type* getLLVMType();
    llvm::Value* getAddress();
    unsigned int getOrderIndex() const;
    bool isLocal() const;
    bool isUsed() const;
    void updateState(SymbolState newState, const antlr4::Token& token);
    void updateType(SymbolType newType, bool force);
    void updateLLVMType(llvm::Type* newType);
    void updateAddress(llvm::Value* newAddress);
    void setUsed();
    nlohmann::ordered_json toJSON();

private:
    // Members
    std::string name;
    SymbolType type;
    SymbolSpecifiers specifiers;
    llvm::Type* llvmType;
    SymbolState state;
    const antlr4::Token& definitionToken;
    llvm::Value* memAddress = nullptr;
    unsigned int orderIndex;
    const bool isGlobal;
    bool used;
};
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <stdexcept>
#include <utility>

#include "symbol/SymbolType.h"
#include "exception/SemanticError.h"
#include "SymbolSpecifiers.h"
#include <llvm/IR/Value.h>

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
            name(std::move(name)), type(std::move(type)), state(state), definitionToken(token), orderIndex(orderIndex),
            isGlobal(isGlobal), used(false) {};

    // Public methods
    std::string getName();
    SymbolType getType();
    SymbolState getState();
    const antlr4::Token& getDefinitionToken();
    llvm::Type* getLLVMType();
    llvm::Value* getAddress();
    unsigned int getOrderIndex() const;
    bool isLocal() const;
    bool isUsed() const;
    void updateState(SymbolState);
    void updateType(SymbolType, bool);
    void updateLLVMType(llvm::Type*);
    void updateAddress(llvm::Value*);
    void setUsed();
    std::string toString();

private:
    // Members
    std::string name;
    SymbolType type;
    llvm::Type* llvmType;
    SymbolState state;
    const antlr4::Token& definitionToken;
    llvm::Value* memAddress;
    unsigned int orderIndex;
    const bool isGlobal;
    bool used;
};
// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <stdexcept>
#include <utility>

#include "SymbolType.h"
#include <exception/SemanticError.h>
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
    SymbolTableEntry(std::string name, SymbolType type, SymbolState state, unsigned int orderIndex,
                     bool isConstant, bool isGlobal) :
            name(std::move(name)), type(std::move(type)), state(state),
            orderIndex(orderIndex), isConstant(isConstant), isGlobal(isGlobal) {};

    // Public methods
    std::string getName();

    SymbolState getState();

    SymbolType getType();

    llvm::Type* getLLVMType();

    llvm::Value* getAddress();

    unsigned int getOrderIndex() const;

    bool isLocal() const;

    void updateState(SymbolState);

    void updateType(SymbolType);

    void updateLLVMType(llvm::Type*);

    void updateAddress(llvm::Value*);

    std::string toString();

private:
    // Members
    std::string name;
    SymbolType type;
    llvm::Type* llvmType;
    SymbolState state;
    llvm::Value* memAddress;
    unsigned int orderIndex;
    bool isConstant;
    bool isGlobal;
};
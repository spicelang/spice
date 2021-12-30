// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <utility>
#include <algorithm>
#include <string>
#include <vector>
#include <stack>
#include <tuple>

#include "exception/SemanticError.h"

enum SymbolSuperType {
    TY_DOUBLE,
    TY_INT,
    TY_SHORT,
    TY_LONG,
    TY_BYTE,
    TY_CHAR,
    TY_STRING,
    TY_BOOL,
    TY_STRUCT,
    TY_DYN,
    TY_PTR,
    TY_ARRAY,
    TY_FUNCTION,
    TY_PROCEDURE,
    TY_IMPORT
};

typedef std::tuple<SymbolSuperType, std::string> TypeChainElement;
typedef std::stack<TypeChainElement> TypeChain;

class SymbolType {
public:
    // Constructors
    explicit SymbolType(SymbolSuperType superType) : typeChain({ std::make_tuple(superType, "") }) {}
    SymbolType(SymbolSuperType superType, const std::string& subType) : typeChain({ std::make_tuple(superType, subType) }) {}
    explicit SymbolType(TypeChain types) : typeChain(std::move(types)) {}
    SymbolType() = default;

    // Public methods
    SymbolType toPointer();
    SymbolType toArray();
    SymbolType getContainedTy();
    bool isPointer();
    bool isPointerOf(SymbolSuperType);
    bool isArray();
    bool isArrayOf(SymbolSuperType);
    bool is(SymbolSuperType);
    bool isOneOf(const std::vector<SymbolSuperType>&);
    bool matches(SymbolType);
    bool matches(SymbolType, SymbolSuperType);
    SymbolSuperType getSuperType();
    std::string getSubType();
    std::string getName();
    friend bool operator== (const SymbolType& lhs, const SymbolType& rhs);
    friend bool operator!= (const SymbolType& lhs, const SymbolType& rhs);
private:
    // Members
    TypeChain typeChain;

    // Private methods
    static std::string getNameFromChainElement(const TypeChainElement&);
};
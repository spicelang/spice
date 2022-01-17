// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

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
    SymbolType toArray(unsigned int = 0);
    SymbolType getContainedTy();
    SymbolType replaceSubType(const std::string& newSubType);
    bool isPointer();
    bool isPointerOf(SymbolSuperType);
    bool isArray();
    bool isArrayOf(SymbolSuperType);
    bool is(SymbolSuperType);
    bool is(SymbolSuperType, const std::string&);
    bool isBaseType(SymbolSuperType);
    bool isBaseType(SymbolSuperType, const std::string&);
    bool isOneOf(const std::vector<SymbolSuperType>&);
    bool matches(SymbolType);
    bool matches(SymbolType, SymbolSuperType);
    SymbolSuperType getSuperType();
    std::string getSubType();
    std::string getName(bool);
    void setArraySize(unsigned int);
    unsigned int getArraySize();
    friend bool operator== (const SymbolType& lhs, const SymbolType& rhs);
    friend bool operator!= (const SymbolType& lhs, const SymbolType& rhs);
private:
    // Members
    TypeChain typeChain;

    // Private methods
    static std::string getNameFromChainElement(const TypeChainElement&, bool);
};
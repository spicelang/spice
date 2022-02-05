// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

//#include <utility>
//#include <algorithm>
#include <string>
#include <vector>
#include <stack>
#include <tuple>

enum SymbolSuperType {
    TY_INVALID,
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
    SymbolType toArray(unsigned int size = 0);
    SymbolType getContainedTy();
    SymbolType replaceSubType(const std::string& newSubType);
    bool isPointer();
    bool isPointerOf(SymbolSuperType superType);
    bool isArray();
    bool isArrayOf(SymbolSuperType superType);
    bool is(SymbolSuperType superType);
    bool is(SymbolSuperType superType, const std::string& subType);
    bool isBaseType(SymbolSuperType superType);
    bool isBaseType(SymbolSuperType superType, const std::string& subType);
    bool isOneOf(const std::vector<SymbolSuperType>& superTypes);
    SymbolSuperType getSuperType();
    std::string getSubType();
    SymbolType getBaseType();
    std::string getName(bool withSize);
    void setArraySize(unsigned int newArraySize);
    unsigned int getArraySize();
    friend bool operator== (const SymbolType& lhs, const SymbolType& rhs);
    friend bool operator!= (const SymbolType& lhs, const SymbolType& rhs);
private:
    // Members
    TypeChain typeChain;

    // Private methods
    static std::string getNameFromChainElement(const TypeChainElement& chainElement, bool withSize);
};
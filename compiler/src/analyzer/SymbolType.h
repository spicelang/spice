// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <utility>
#include <string>

enum SymbolSuperType {
    TYPE_DOUBLE,     TYPE_INT,     TYPE_STRING,     TYPE_BOOL,
    TYPE_DOUBLE_PTR, TYPE_INT_PTR, TYPE_STRING_PTR, TYPE_BOOL_PTR,
    TYPE_DYN,
    TYPE_FUNCTION, TYPE_PROCEDURE,
    TYPE_STRUCT, TYPE_STRUCT_PTR,
    TYPE_IMPORT
};

class SymbolType {
public:
    // Constructors
    SymbolType(SymbolSuperType superType, std::string subType) : superType(superType), subType(std::move(subType)) {};
    explicit SymbolType(SymbolSuperType superType) : superType(superType) {};
    SymbolType() = default;

    // Public methods
    SymbolSuperType getSuperType();
    std::string getSubType();
    friend bool operator== (const SymbolType& lhs, const SymbolType& rhs);
    friend bool operator!= (const SymbolType& lhs, const SymbolType& rhs);
private:
    // Members
    SymbolSuperType superType;
    std::string subType;
};
// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <utility>
#include <string>
#include <vector>
#include <stdexcept>

enum SymbolSuperType {
    TYPE_DOUBLE,           TYPE_INT,           TYPE_BYTE,           TYPE_CHAR,           TYPE_STRING,           TYPE_BOOL,           TYPE_STRUCT,
    TYPE_DOUBLE_PTR,       TYPE_INT_PTR,       TYPE_BYTE_PTR,       TYPE_CHAR_PTR,       TYPE_STRING_PTR,       TYPE_BOOL_PTR,       TYPE_STRUCT_PTR,
    TYPE_DOUBLE_ARRAY,     TYPE_INT_ARRAY,     TYPE_BYTE_ARRAY,     TYPE_CHAR_ARRAY,     TYPE_STRING_ARRAY,     TYPE_BOOL_ARRAY,     TYPE_STRUCT_ARRAY,
    TYPE_DOUBLE_PTR_ARRAY, TYPE_INT_PTR_ARRAY, TYPE_BYTE_PTR_ARRAY, TYPE_CHAR_PTR_ARRAY, TYPE_STRING_PTR_ARRAY, TYPE_BOOL_PTR_ARRAY, TYPE_STRUCT_PTR_ARRAY,
    TYPE_DYN,
    TYPE_FUNCTION, TYPE_PROCEDURE,
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
    std::string getName();
    bool isPointer();
    bool isArray();
    SymbolType getPointerType();
    SymbolType getScalarType();
    SymbolType getArrayType();
    SymbolType getItemType();
    bool isOneOf(const std::vector<SymbolSuperType>&);
    bool is(SymbolSuperType);
    bool matches(SymbolType);
    bool matches(SymbolType, SymbolSuperType);
    friend bool operator== (const SymbolType& lhs, const SymbolType& rhs);
    friend bool operator!= (const SymbolType& lhs, const SymbolType& rhs);
private:
    // Members
    SymbolSuperType superType;
    std::string subType;
};
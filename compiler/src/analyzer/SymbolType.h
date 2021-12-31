// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>
#include <algorithm>
#include <string>
#include <vector>
#include <stdexcept>

enum SymbolSuperTypeOld {
    TYPE_DOUBLE,           TYPE_INT,           TYPE_BYTE,           TYPE_CHAR,           TYPE_STRING,           TYPE_BOOL,           TYPE_STRUCT,
    TYPE_DOUBLE_PTR,       TYPE_INT_PTR,       TYPE_BYTE_PTR,       TYPE_CHAR_PTR,       TYPE_STRING_PTR,       TYPE_BOOL_PTR,       TYPE_STRUCT_PTR,
    TYPE_DOUBLE_ARRAY,     TYPE_INT_ARRAY,     TYPE_BYTE_ARRAY,     TYPE_CHAR_ARRAY,     TYPE_STRING_ARRAY,     TYPE_BOOL_ARRAY,     TYPE_STRUCT_ARRAY,
    TYPE_DOUBLE_PTR_ARRAY, TYPE_INT_PTR_ARRAY, TYPE_BYTE_PTR_ARRAY, TYPE_CHAR_PTR_ARRAY, TYPE_STRING_PTR_ARRAY, TYPE_BOOL_PTR_ARRAY, TYPE_STRUCT_PTR_ARRAY,
    TYPE_DYN,
    TYPE_FUNCTION, TYPE_PROCEDURE,
    TYPE_IMPORT
};

class SymbolTypeOld {
public:
    // Constructors
    SymbolTypeOld(SymbolSuperTypeOld superType, std::string subType) : superType(superType), subType(std::move(subType)) {};
    explicit SymbolTypeOld(SymbolSuperTypeOld superType) : superType(superType) {};
    SymbolTypeOld() = default;

    // Public methods
    SymbolSuperTypeOld getSuperType();
    std::string getSubType();
    std::string getName();
    bool isPointer();
    bool isArray();
    SymbolTypeOld getPointerType();
    SymbolTypeOld getScalarType();
    SymbolTypeOld getArrayType();
    SymbolTypeOld getItemType();
    bool isImplicitCastCompatibleWith(const SymbolTypeOld&);
    bool isOneOf(const std::vector<SymbolSuperTypeOld>&);
    bool is(SymbolSuperTypeOld);
    bool matches(SymbolTypeOld);
    bool matches(SymbolTypeOld, SymbolSuperTypeOld);
    friend bool operator== (const SymbolTypeOld& lhs, const SymbolTypeOld& rhs);
    friend bool operator!= (const SymbolTypeOld& lhs, const SymbolTypeOld& rhs);
private:
    // Members
    SymbolSuperTypeOld superType;
    std::string subType;
};
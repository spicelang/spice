// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SymbolType.h"

/**
 * Retrieves the super type of a symbol
 *
 * @return Super type: e.g. TYPE_STRING
 */
SymbolSuperType SymbolType::getSuperType() {
    return superType;
}

/**
 * Retrieves the sub type of a symbol. This is only relevant for structs. The sub type will be the custom type name here.
 *
 * @return Sub type: e.g. TestStruct
 */
std::string SymbolType::getSubType() {
    return subType;
}

/**
 * Checks if the super type is a pointer type
 *
 * @return True if it a pointer type, otherwise false
 */
bool SymbolType::isPointer() {
    return superType == TYPE_DOUBLE_PTR || superType == TYPE_INT_PTR || superType == TYPE_STRING_PTR ||
        superType == TYPE_BOOL_PTR || superType == TYPE_STRUCT_PTR;
}

/**
 * Retrieve the pointer version of the current super type
 *
 * @return Pointer version: e.g. TYPE_STRUCT_PTR for TYPE_STRUCT
 */
SymbolType SymbolType::getPointerType() {
    if (superType == TYPE_DOUBLE) return SymbolType(TYPE_DOUBLE_PTR);
    if (superType == TYPE_INT) return SymbolType(TYPE_INT_PTR);
    if (superType == TYPE_STRING) return SymbolType(TYPE_STRING_PTR);
    if (superType == TYPE_BOOL) return SymbolType(TYPE_BOOL_PTR);
    if (superType == TYPE_STRUCT) return {TYPE_STRUCT_PTR, subType};
    return SymbolType(TYPE_DYN);
}

/**
 * Retrieve the normal version of the current super type
 *
 * @return Normal version: e.g. TYPE_DOUBLE for TYPE_DOUBLE_PTR
 */
SymbolType SymbolType::getScalarType() {
    if (superType == TYPE_DOUBLE_PTR) return SymbolType(TYPE_DOUBLE);
    if (superType == TYPE_INT_PTR) return SymbolType(TYPE_INT);
    if (superType == TYPE_STRING_PTR) return SymbolType(TYPE_STRING);
    if (superType == TYPE_BOOL_PTR) return SymbolType(TYPE_BOOL);
    if (superType == TYPE_STRUCT_PTR) return {TYPE_STRUCT, subType};
    return SymbolType(TYPE_DYN);
}

/**
 * Checks if this SymbolType's super type is one of the passed
 *
 * @param superTypes Super types to check
 * @return True = super type is one of the stated, otherwise false
 */
bool SymbolType::isOneOf(const std::vector<SymbolSuperType>& superTypes) {
    for (auto& type : superTypes) {
        if (type == superType) return true;
    }
    return false;
}

/**
 * Checks if this SymbolType's super type is the passed
 *
 * @param superType Super type to check
 * @return True = super type is one of the stated, otherwise false
 */
bool SymbolType::is(SymbolSuperType type) {
    return superType == type;
}


bool operator== (const SymbolType& lhs, const SymbolType& rhs) {
    return lhs.superType == rhs.superType && lhs.subType == rhs.subType;
}

bool operator!= (const SymbolType& lhs, const SymbolType& rhs) {
    return !(lhs == rhs);
}

bool SymbolType::matches(SymbolType type) {
    return superType == type.getSuperType();
}

bool SymbolType::matches(SymbolType symbolType, SymbolSuperType superSymbolType) {
    return superType == superSymbolType && symbolType.getSuperType() == superSymbolType;
}

std::string SymbolType::getName() {
    switch (superType) {
        case TYPE_DOUBLE: return "double";
        case TYPE_INT: return "int";
        case TYPE_STRING: return "string";
        case TYPE_BOOL: return "bool";
        case TYPE_DOUBLE_PTR: return "double*";
        case TYPE_INT_PTR: return "int*";
        case TYPE_STRING_PTR: return "string*";
        case TYPE_BOOL_PTR: return "bool*";
        case TYPE_DYN: return "dyn";
        case TYPE_FUNCTION: return "function";
        case TYPE_PROCEDURE: return "procedure";
        case TYPE_STRUCT: return "struct(" + subType + ")";
        case TYPE_STRUCT_PTR: return "struct(" + subType + ")*";;
        case TYPE_IMPORT: return "import";
    }
    return "";
}

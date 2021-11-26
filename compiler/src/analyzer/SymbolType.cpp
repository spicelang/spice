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
SymbolType SymbolType::getPointerVersion() {
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
SymbolType SymbolType::getNormalVersion() {
    if (superType == TYPE_DOUBLE_PTR) return SymbolType(TYPE_DOUBLE);
    if (superType == TYPE_INT_PTR) return SymbolType(TYPE_INT);
    if (superType == TYPE_STRING_PTR) return SymbolType(TYPE_STRING);
    if (superType == TYPE_BOOL_PTR) return SymbolType(TYPE_BOOL);
    if (superType == TYPE_STRUCT_PTR) return SymbolType(TYPE_STRUCT, subType);
    return SymbolType(TYPE_DYN);
}

bool operator== (const SymbolType& lhs, const SymbolType& rhs) {
    return lhs.superType == rhs.superType && lhs.subType == rhs.subType;
}

bool operator!= (const SymbolType& lhs, const SymbolType& rhs) {
    return !(lhs == rhs);
}
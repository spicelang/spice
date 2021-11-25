// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SymbolType.h"

SymbolSuperType SymbolType::getSuperType() {
    return superType;
}

std::string SymbolType::getSubType() {
    return subType;
}

bool SymbolType::isPointer() {
    return superType == TYPE_DOUBLE_PTR || superType == TYPE_INT_PTR || superType == TYPE_STRING_PTR ||
        superType == TYPE_BOOL_PTR || superType == TYPE_STRUCT_PTR;
}

SymbolType SymbolType::getPointerVersion() {
    if (superType == TYPE_DOUBLE) return SymbolType(TYPE_DOUBLE_PTR);
    if (superType == TYPE_INT) return SymbolType(TYPE_INT_PTR);
    if (superType == TYPE_STRING) return SymbolType(TYPE_STRING_PTR);
    if (superType == TYPE_BOOL) return SymbolType(TYPE_BOOL_PTR);
    if (superType == TYPE_STRUCT) return {TYPE_STRUCT_PTR, subType};
    return SymbolType(TYPE_DYN);
}

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
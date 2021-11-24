// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SymbolType.h"

SymbolSuperType SymbolType::getSuperType() {
    return superType;
}

std::string SymbolType::getSubType() {
    return subType;
}

bool operator== (const SymbolType& lhs, const SymbolType& rhs) {
    return lhs.superType == rhs.superType && lhs.subType == rhs.subType;
}

bool operator!= (const SymbolType& lhs, const SymbolType& rhs) {
    return !(lhs == rhs);
}

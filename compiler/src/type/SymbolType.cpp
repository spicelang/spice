// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SymbolType.h"

SymbolType SymbolType::toPointer() {
    // Do not allow pointers of dyn
    if (std::get<0>(typeChain.top()) == TY_DYN)
        throw SemanticError(DYN_POINTERS_NOT_ALLOWED, "Just use the dyn type without '*' instead");

    TypeChain newTypeChain = typeChain;
    newTypeChain.push(std::make_tuple(TY_PTR, ""));
    return SymbolType(newTypeChain);
}

SymbolType SymbolType::toArray() {
    // Do not allow arrays of dyn
    if (std::get<0>(typeChain.top()) == TY_DYN)
        throw SemanticError(DYN_ARRAYS_NOT_ALLOWED, "Just use the dyn type without '[]' instead");

    TypeChain newTypeChain = typeChain;
    newTypeChain.push(std::make_tuple(TY_ARRAY, ""));
    return SymbolType(newTypeChain);
}

SymbolType SymbolType::getContainedTy() {
    if (typeChain.empty()) throw std::runtime_error("Internal compiler error: Cannot get contained type of empty type");
    TypeChain newTypeChain = typeChain;
    newTypeChain.pop();
    return SymbolType(newTypeChain);
}

bool SymbolType::isPointer() {
    return getSuperType() == TY_PTR;
}

bool SymbolType::isPointerOf(SymbolSuperType elementSuperType) {
    if (isPointer()) return getContainedTy().is(elementSuperType);
    return false;
}

bool SymbolType::isArray() {
    return getSuperType() == TY_ARRAY;
}

bool SymbolType::isArrayOf(SymbolSuperType elementSuperType) {
    if (isArray()) return getContainedTy().is(elementSuperType);
    return false;
}

bool SymbolType::is(SymbolSuperType superType) {
    return getSuperType() == superType;
}

bool SymbolType::isOneOf(const std::vector<SymbolSuperType>& superTypes) {
    SymbolSuperType superType = getSuperType();
    return std::any_of(superTypes.begin(), superTypes.end(), [&superType](int type) {
        return type == superType;
    });
}

bool SymbolType::matches(SymbolType type) {
    return type.getSuperType() == getSuperType();
}

bool SymbolType::matches(SymbolType symbolType, SymbolSuperType superSymbolType) {
    SymbolSuperType superType = getSuperType();
    return symbolType.getSuperType() == superType && superSymbolType == superType;
}

SymbolSuperType SymbolType::getSuperType() {
    return std::get<0>(typeChain.top());
}

std::string SymbolType::getSubType() {
    return std::get<1>(typeChain.top());
}

std::string SymbolType::getName() {
    std::string name;
    TypeChain chain = typeChain;
    for (int i = 0; i < typeChain.size(); i++) {
        TypeChainElement chainElement = chain.top();
        name.insert(0, getNameFromChainElement(chainElement));
        chain.pop();
    }
    return name;
}

bool operator==(const SymbolType& lhs, const SymbolType& rhs) {
    return lhs.typeChain == rhs.typeChain;
}

bool operator!=(const SymbolType& lhs, const SymbolType& rhs) {
    return lhs.typeChain != rhs.typeChain;
}

std::string SymbolType::getNameFromChainElement(const TypeChainElement& chainElement) {
    switch (std::get<0>(chainElement)) {
        case TY_PTR: return "*";
        case TY_ARRAY: return "[]";
        case TY_DOUBLE: return "double";
        case TY_INT: return "int";
        case TY_SHORT: return "short";
        case TY_LONG: return "long";
        case TY_BYTE: return "byte";
        case TY_CHAR: return "char";
        case TY_STRING: return "string";
        case TY_BOOL: return "bool";
        case TY_STRUCT: return "struct(" + std::get<1>(chainElement) + ")";
        case TY_DYN: return "dyn";
        case TY_FUNCTION: return "function";
        case TY_PROCEDURE: return "procedure";
        case TY_IMPORT: return "import";
    }
    return "unknown";
}

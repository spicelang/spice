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

SymbolType SymbolType::toArray(unsigned int size) {
    // Do not allow arrays of dyn
    if (std::get<0>(typeChain.top()) == TY_DYN)
        throw SemanticError(DYN_ARRAYS_NOT_ALLOWED, "Just use the dyn type without '[]' instead");

    TypeChain newTypeChain = typeChain;
    newTypeChain.push(std::make_tuple(TY_ARRAY, std::to_string(size)));
    return SymbolType(newTypeChain);
}

SymbolType SymbolType::getContainedTy() {
    if (typeChain.empty()) throw std::runtime_error("Internal compiler error: Cannot get contained type of empty type");
    if (std::get<0>(typeChain.top()) == TY_STRING) return SymbolType(TY_CHAR);
    TypeChain newTypeChain = typeChain;
    newTypeChain.pop();
    return SymbolType(newTypeChain);
}

SymbolType SymbolType::replaceSubType(const std::string& newSubType) {
    // Copy the stack to not destroy the present one
    TypeChain chainCopy = typeChain;
    // Unwrap the chain until the base type can be retrieved. To be able to restore the structure later, save it to the tmp chain
    TypeChain tmp;
    while (std::get<0>(chainCopy.top()) == TY_PTR || std::get<0>(chainCopy.top()) == TY_ARRAY) {
        tmp.push(chainCopy.top());
        chainCopy.pop();
    }
    // Replace the subType of the base chain element
    std::get<1>(chainCopy.top()) = newSubType;
    // Restore the other chain elements
    for (unsigned int i = 0; i < tmp.size(); i++) {
        chainCopy.push(tmp.top());
        tmp.pop();
    }
    // Return the new chain as a symbol type
    return SymbolType(chainCopy);
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

bool SymbolType::is(SymbolSuperType superType, const std::string& subType) {
    return getSuperType() == superType && getSubType() == subType;
}

bool SymbolType::isBaseType(SymbolSuperType superType) {
    // Copy the stack to not destroy the present one
    TypeChain chainCopy = typeChain;
    // Unwrap the chain until the base type can be retrieved
    while (std::get<0>(chainCopy.top()) == TY_PTR || std::get<0>(chainCopy.top()) == TY_ARRAY) chainCopy.pop();
    // Check if it is of the given superType and subType
    return std::get<0>(chainCopy.top()) == superType;
}

bool SymbolType::isBaseType(SymbolSuperType superType, const std::string& subType) {
    // Copy the stack to not destroy the present one
    TypeChain chainCopy = typeChain;
    // Unwrap the chain until the base type can be retrieved
    while (std::get<0>(chainCopy.top()) == TY_PTR || std::get<0>(chainCopy.top()) == TY_ARRAY) chainCopy.pop();
    // Check if it is of the given superType and subType
    return std::get<0>(chainCopy.top()) == superType && std::get<1>(chainCopy.top()) == subType;
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

SymbolType SymbolType::getBaseType() {
    // Copy the stack to not destroy the present one
    TypeChain chainCopy = typeChain;
    // Unwrap the chain until the base type can be retrieved
    while (std::get<0>(chainCopy.top()) == TY_PTR || std::get<0>(chainCopy.top()) == TY_ARRAY) chainCopy.pop();
    // Check if it is of the given superType and subType
    return SymbolType(chainCopy);
}

std::string SymbolType::getName(bool withSize) {
    std::string name;
    TypeChain chain = typeChain;
    for (int i = 0; i < typeChain.size(); i++) {
        TypeChainElement chainElement = chain.top();
        name.insert(0, getNameFromChainElement(chainElement, withSize));
        chain.pop();
    }
    return name;
}

void SymbolType::setArraySize(unsigned int size) {
    if (std::get<0>(typeChain.top()) != TY_ARRAY)
        throw std::runtime_error("Internal compiler error: Cannot set size of non-array type");

    std::get<1>(typeChain.top()) = std::to_string(size);
}

unsigned int SymbolType::getArraySize() {
    if (std::get<0>(typeChain.top()) != TY_ARRAY)
        throw std::runtime_error("Internal compiler error: Cannot get size of non-array type");

    return std::stoi(std::get<1>(typeChain.top()));
}

bool operator==(const SymbolType& lhs, const SymbolType& rhs) {
    return lhs.typeChain == rhs.typeChain;
}

bool operator!=(const SymbolType& lhs, const SymbolType& rhs) {
    return lhs.typeChain != rhs.typeChain;
}

std::string SymbolType::getNameFromChainElement(const TypeChainElement& chainElement, bool withSize) {
    switch (std::get<0>(chainElement)) {
        case TY_PTR: return "*";
        case TY_ARRAY: return !withSize || std::get<1>(chainElement) == "0" ? "[]" : "[" + std::get<1>(chainElement) + "]";
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
        case TY_INVALID: return "invalid";
    }
    return "unknown";
}

// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SymbolType.h"

#include <stdexcept>

#include <exception/SemanticError.h>

/**
 * Get the pointer type of the current type as a new type
 *
 * @return Pointer type of the current type
 */
SymbolType SymbolType::toPointer() {
    // Do not allow pointers of dyn
    if (std::get<0>(typeChain.top()) == TY_DYN)
        throw SemanticError(DYN_POINTERS_NOT_ALLOWED, "Just use the dyn type without '*' instead");

    TypeChain newTypeChain = typeChain;
    newTypeChain.push(std::make_pair(TY_PTR, ""));
    return SymbolType(newTypeChain);
}

/**
 * Get the array type of the current type as a new type
 *
 * @return Array type of the current type
 */
SymbolType SymbolType::toArray(unsigned int size) {
    // Do not allow arrays of dyn
    if (std::get<0>(typeChain.top()) == TY_DYN)
        throw SemanticError(DYN_ARRAYS_NOT_ALLOWED, "Just use the dyn type without '[]' instead");

    TypeChain newTypeChain = typeChain;
    newTypeChain.push(std::make_pair(TY_ARRAY, std::to_string(size)));
    return SymbolType(newTypeChain);
}

/**
 * Retrieve the base type of an array or a pointer
 *
 * @return Base type
 */
SymbolType SymbolType::getContainedTy() {
    if (typeChain.empty())                                                                            // GCOV_EXCL_LINE
        throw std::runtime_error("Internal compiler error: Cannot get contained type of empty type"); // GCOV_EXCL_LINE
    if (std::get<0>(typeChain.top()) == TY_STRING) return SymbolType(TY_CHAR);
    TypeChain newTypeChain = typeChain;
    newTypeChain.pop();
    return SymbolType(newTypeChain);
}

/**
 * Replace the subtype of the base type with another one
 *
 * @param newSubType New sub type of the base type
 * @return The new type with the adjusted type chain
 */
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

/**
 * Check is the current type is of type pointer
 *
 * @return Pointer or not
 */
bool SymbolType::isPointer() {
    return getSuperType() == TY_PTR;
}

/**
 * Check if the current type is a pointer of a certain super type
 *
 * @param elementSuperType Super type to check for
 * @return Pointer or not
 */
bool SymbolType::isPointerOf(SymbolSuperType elementSuperType) {
    if (isPointer()) return getContainedTy().is(elementSuperType);
    return false;
}

/**
 * Check is the current type is of type array
 *
 * @return Array or not
 */
bool SymbolType::isArray() {
    return getSuperType() == TY_ARRAY;
}

/**
 * Check if the current type is a array of a certain super type
 *
 * @param elementSuperType Super type to check for
 * @return Array or not
 */
bool SymbolType::isArrayOf(SymbolSuperType elementSuperType) {
    if (isArray()) return getContainedTy().is(elementSuperType);
    return false;
}

/**
 * Check if the current type is of a certain super type
 *
 * @param superType Super type to check for
 * @return Applicable or not
 */
bool SymbolType::is(SymbolSuperType superType) {
    return getSuperType() == superType;
}

/**
 * Check if the current type is of a certain super type and sub type
 *
 * @param superType Super type to check for
 * @param subType Sub type to check for
 * @return Applicable or not
 */
bool SymbolType::is(SymbolSuperType superType, const std::string& subType) {
    return getSuperType() == superType && getSubType() == subType;
}

/**
 * Check if the current type is one of the primitive types
 *
 * @return Primitive or not
 */
bool SymbolType::isPrimitive() {
    return isOneOf({ TY_DOUBLE, TY_INT, TY_SHORT, TY_LONG, TY_BYTE, TY_CHAR, TY_STRING, TY_BOOL });
}

/**
 * Check if the base type of the current type chain is of a certain super type
 *
 * @param superType Super type to check for
 * @return Applicable or not
 */
bool SymbolType::isBaseType(SymbolSuperType superType) {
    // Copy the stack to not destroy the present one
    TypeChain chainCopy = typeChain;
    // Unwrap the chain until the base type can be retrieved
    while (std::get<0>(chainCopy.top()) == TY_PTR || std::get<0>(chainCopy.top()) == TY_ARRAY) chainCopy.pop();
    // Check if it is of the given superType and subType
    return std::get<0>(chainCopy.top()) == superType;
}

/**
 * Check if the current type is amongst a collection of certain super types
 *
 * @param superTypes Vector of super types
 * @return Applicable or not
 */
bool SymbolType::isOneOf(const std::vector<SymbolSuperType>& superTypes) {
    SymbolSuperType superType = getSuperType();
    return std::any_of(superTypes.begin(), superTypes.end(), [&superType](int type) {
        return type == superType;
    });
}

/**
 * Retrieve the super type of the current type
 *
 * @return Super type
 */
SymbolSuperType SymbolType::getSuperType() {
    return std::get<0>(typeChain.top());
}

/**
 * Retrieve the sub type of the current type
 *
 * @return Sub type
 */
std::string SymbolType::getSubType() {
    return std::get<1>(typeChain.top());
}

/**
 * Retrieve the base type of the current type. E.g. int of int[]*[]**
 *
 * @return Base type
 */
SymbolType SymbolType::getBaseType() {
    // Copy the stack to not destroy the present one
    TypeChain chainCopy = typeChain;
    // Unwrap the chain until the base type can be retrieved
    while (std::get<0>(chainCopy.top()) == TY_PTR || std::get<0>(chainCopy.top()) == TY_ARRAY) chainCopy.pop();
    // Check if it is of the given superType and subType
    return SymbolType(chainCopy);
}

/**
 * Get the name of the symbol type as a string
 *
 * @param withSize Include the array size for sized types
 * @return Symbol type name
 */
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

/**
 * Get the size of the current type
 *
 * @return Size
 */
unsigned int SymbolType::getArraySize() {
    if (std::get<0>(typeChain.top()) != TY_ARRAY)                                             // GCOV_EXCL_LINE
        throw std::runtime_error("Internal compiler error: Cannot get size of non-array type");  // GCOV_EXCL_LINE

    return std::stoi(std::get<1>(typeChain.top()));
}

bool operator==(const SymbolType& lhs, const SymbolType& rhs) {
    return lhs.typeChain == rhs.typeChain;
}

bool operator!=(const SymbolType& lhs, const SymbolType& rhs) {
    return lhs.typeChain != rhs.typeChain;
}

/**
 * Get the name of a type chain element
 *
 * @param chainElement Input chain element
 * @param withSize Include size in string
 * @return Type chain element name
 */
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
        case TY_INVALID: return "invalid"; // GCOV_EXCL_LINE
    }
    return "unknown"; // GCOV_EXCL_LINE
}

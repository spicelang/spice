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
    return isOneOf({ TYPE_DOUBLE_PTR, TYPE_INT_PTR, TYPE_STRING_PTR, TYPE_BOOL_PTR, TYPE_STRUCT_PTR });
}

/**
 * Checks if the super type is an array type
 *
 * @return True if it an array type, otherwise false
 */
bool SymbolType::isArray() {
    return isOneOf({ TYPE_DOUBLE_ARRAY, TYPE_INT_ARRAY, TYPE_STRING_ARRAY, TYPE_BOOL_ARRAY, TYPE_STRUCT_ARRAY,
                     TYPE_DOUBLE_PTR_ARRAY, TYPE_INT_PTR_ARRAY, TYPE_STRING_PTR_ARRAY, TYPE_BOOL_PTR_ARRAY,
                     TYPE_STRUCT_PTR_ARRAY});
}

/**
 * Retrieve the pointer version of the current super type
 *
 * @return Pointer version: e.g. TYPE_STRUCT_PTR for TYPE_STRUCT
 */
SymbolType SymbolType::getPointerType() {
    switch (superType) {
        case TYPE_DOUBLE: return SymbolType(TYPE_DOUBLE_PTR);
        case TYPE_INT: return SymbolType(TYPE_INT_PTR);
        case TYPE_STRING: return SymbolType(TYPE_STRING_PTR);
        case TYPE_BOOL: return SymbolType(TYPE_BOOL_PTR);
        case TYPE_STRUCT: return {TYPE_STRUCT_PTR, subType};
        default: throw std::runtime_error("Compiler error: Could not get the pointer type of " + getName());
    }
}

/**
 * Retrieve the normal version of the current super type
 *
 * @return Normal version: e.g. TYPE_DOUBLE for TYPE_DOUBLE_PTR
 */
SymbolType SymbolType::getScalarType() {
    switch (superType) {
        case TYPE_DOUBLE_PTR: return SymbolType(TYPE_DOUBLE);
        case TYPE_INT_PTR: return SymbolType(TYPE_INT);
        case TYPE_STRING_PTR: return SymbolType(TYPE_STRING);
        case TYPE_BOOL_PTR: return SymbolType(TYPE_BOOL);
        case TYPE_STRUCT_PTR: return {TYPE_STRUCT, subType};
        default: throw std::runtime_error("Compiler error: Could not get the scalar type of " + getName());
    }
}

/**
 * Retrieve the array version of the current super type
 *
 * @return Array version: e.g.: TYPE_BOOL_PTR_ARRAY for TYPE_BOOL_PTR
 */
SymbolType SymbolType::getArrayType() {
    switch (superType) {
        case TYPE_DOUBLE: return SymbolType(TYPE_DOUBLE_ARRAY);
        case TYPE_INT: return SymbolType(TYPE_INT_ARRAY);
        case TYPE_STRING: return SymbolType(TYPE_STRING_ARRAY);
        case TYPE_BOOL: return SymbolType(TYPE_BOOL_ARRAY);
        case TYPE_STRUCT: return {TYPE_STRUCT_ARRAY, subType};
        case TYPE_DOUBLE_PTR: return SymbolType(TYPE_DOUBLE_PTR_ARRAY);
        case TYPE_INT_PTR: return SymbolType(TYPE_INT_PTR_ARRAY);
        case TYPE_STRING_PTR: return SymbolType(TYPE_STRING_PTR_ARRAY);
        case TYPE_BOOL_PTR: return SymbolType(TYPE_BOOL_PTR_ARRAY);
        case TYPE_STRUCT_PTR: return {TYPE_STRUCT_PTR_ARRAY, subType};
        default: throw std::runtime_error("Compiler error: Could not get the array type of " + getName());
    }
}

/**
 * Retrieve the item version of the current super type
 *
 * @return Item version: e.g.: TYPE_INT for TYPE_INT_ARRAY
 */
SymbolType SymbolType::getItemType() {
    switch (superType) {
        case TYPE_DOUBLE_ARRAY: return SymbolType(TYPE_DOUBLE);
        case TYPE_INT_ARRAY: return SymbolType(TYPE_INT);
        case TYPE_STRING_ARRAY: return SymbolType(TYPE_STRING);
        case TYPE_BOOL_ARRAY: return SymbolType(TYPE_BOOL);
        case TYPE_STRUCT_ARRAY: return {TYPE_STRUCT, subType};
        case TYPE_DOUBLE_PTR_ARRAY: return SymbolType(TYPE_DOUBLE_PTR);
        case TYPE_INT_PTR_ARRAY: return SymbolType(TYPE_INT_PTR);
        case TYPE_STRING_PTR_ARRAY: return SymbolType(TYPE_STRING_PTR);
        case TYPE_BOOL_PTR_ARRAY: return SymbolType(TYPE_BOOL_PTR);
        case TYPE_STRUCT_PTR_ARRAY: return {TYPE_STRUCT_PTR, subType};
        default: throw std::runtime_error("Compiler error: Could not get the item type of " + getName());
    }
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
        case TYPE_STRUCT: return "struct(" + subType + ")";
        case TYPE_DOUBLE_PTR: return "double*";
        case TYPE_INT_PTR: return "int*";
        case TYPE_STRING_PTR: return "string*";
        case TYPE_BOOL_PTR: return "bool*";
        case TYPE_STRUCT_PTR: return "struct(" + subType + ")*";;
        case TYPE_DOUBLE_ARRAY: return "double[]";
        case TYPE_INT_ARRAY: return "int[]";
        case TYPE_STRING_ARRAY: return "string[]";
        case TYPE_BOOL_ARRAY: return "bool[]";
        case TYPE_STRUCT_ARRAY: return "struct(" + subType + ")[]";
        case TYPE_DYN: return "dyn";
        case TYPE_FUNCTION: return "function";
        case TYPE_PROCEDURE: return "procedure";
        case TYPE_IMPORT: return "import";
    }
    return "";
}

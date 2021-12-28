// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SymbolType.h"

/**
 * Retrieves the super type of a symbol
 *
 * @return Super type: e.g. TYPE_STRING
 */
SymbolSuperTypeOld SymbolTypeOld::getSuperType() {
    return superType;
}

/**
 * Retrieves the sub type of a symbol. This is only relevant for structs. The sub type will be the custom type name here.
 *
 * @return Sub type: e.g. TestStruct
 */
std::string SymbolTypeOld::getSubType() {
    return subType;
}

/**
 * Checks if the super type is a pointer type
 *
 * @return True if it a pointer type, otherwise false
 */
bool SymbolTypeOld::isPointer() {
    return isOneOf({ TYPE_DOUBLE_PTR, TYPE_INT_PTR, TYPE_BYTE_PTR, TYPE_CHAR_PTR,
                     TYPE_STRING_PTR, TYPE_BOOL_PTR, TYPE_STRUCT_PTR });
}

/**
 * Checks if the super type is an array type
 *
 * @return True if it an array type, otherwise false
 */
bool SymbolTypeOld::isArray() {
    return isOneOf({ TYPE_DOUBLE_ARRAY, TYPE_INT_ARRAY, TYPE_BYTE_ARRAY, TYPE_CHAR_ARRAY, TYPE_STRING_ARRAY,
                     TYPE_BOOL_ARRAY, TYPE_STRUCT_ARRAY, TYPE_DOUBLE_PTR_ARRAY, TYPE_INT_PTR_ARRAY, TYPE_BYTE_PTR_ARRAY,
                     TYPE_CHAR_PTR_ARRAY, TYPE_STRING_PTR_ARRAY, TYPE_BOOL_PTR_ARRAY, TYPE_STRUCT_PTR_ARRAY });
}

/**
 * Retrieve the pointer version of the current super type
 *
 * @return Pointer version: e.g. TYPE_STRUCT_PTR for TYPE_STRUCT
 */
SymbolTypeOld SymbolTypeOld::getPointerType() {
    switch (superType) {
        case TYPE_DOUBLE: return SymbolTypeOld(TYPE_DOUBLE_PTR);
        case TYPE_INT: return SymbolTypeOld(TYPE_INT_PTR);
        case TYPE_BYTE: return SymbolTypeOld(TYPE_BYTE_PTR);
        case TYPE_CHAR: return SymbolTypeOld(TYPE_CHAR_PTR);
        case TYPE_STRING: return SymbolTypeOld(TYPE_STRING_PTR);
        case TYPE_BOOL: return SymbolTypeOld(TYPE_BOOL_PTR);
        case TYPE_STRUCT: return {TYPE_STRUCT_PTR, subType};
        default: throw std::runtime_error("Compiler error: Could not get the pointer type of " + getName());
    }
}

/**
 * Retrieve the normal version of the current super type
 *
 * @return Normal version: e.g. TYPE_DOUBLE for TYPE_DOUBLE_PTR
 */
SymbolTypeOld SymbolTypeOld::getScalarType() {
    switch (superType) {
        case TYPE_DOUBLE_PTR: return SymbolTypeOld(TYPE_DOUBLE);
        case TYPE_INT_PTR: return SymbolTypeOld(TYPE_INT);
        case TYPE_BYTE_PTR: return SymbolTypeOld(TYPE_BYTE);
        case TYPE_CHAR_PTR: return SymbolTypeOld(TYPE_CHAR);
        case TYPE_STRING_PTR: return SymbolTypeOld(TYPE_STRING);
        case TYPE_BOOL_PTR: return SymbolTypeOld(TYPE_BOOL);
        case TYPE_STRUCT_PTR: return {TYPE_STRUCT, subType};
        default: throw std::runtime_error("Compiler error: Could not get the scalar type of " + getName());
    }
}

/**
 * Retrieve the array version of the current super type
 *
 * @return Array version: e.g.: TYPE_BOOL_PTR_ARRAY for TYPE_BOOL_PTR
 */
SymbolTypeOld SymbolTypeOld::getArrayType() {
    switch (superType) {
        case TYPE_DOUBLE: return SymbolTypeOld(TYPE_DOUBLE_ARRAY);
        case TYPE_INT: return SymbolTypeOld(TYPE_INT_ARRAY);
        case TYPE_BYTE: return SymbolTypeOld(TYPE_BYTE_ARRAY);
        case TYPE_CHAR: return SymbolTypeOld(TYPE_CHAR_ARRAY);
        case TYPE_STRING: return SymbolTypeOld(TYPE_STRING_ARRAY);
        case TYPE_BOOL: return SymbolTypeOld(TYPE_BOOL_ARRAY);
        case TYPE_STRUCT: return {TYPE_STRUCT_ARRAY, subType};
        case TYPE_DOUBLE_PTR: return SymbolTypeOld(TYPE_DOUBLE_PTR_ARRAY);
        case TYPE_INT_PTR: return SymbolTypeOld(TYPE_INT_PTR_ARRAY);
        case TYPE_BYTE_PTR: return SymbolTypeOld(TYPE_BYTE_PTR_ARRAY);
        case TYPE_CHAR_PTR: return SymbolTypeOld(TYPE_CHAR_PTR_ARRAY);
        case TYPE_STRING_PTR: return SymbolTypeOld(TYPE_STRING_PTR_ARRAY);
        case TYPE_BOOL_PTR: return SymbolTypeOld(TYPE_BOOL_PTR_ARRAY);
        case TYPE_STRUCT_PTR: return {TYPE_STRUCT_PTR_ARRAY, subType};
        default: throw std::runtime_error("Compiler error: Could not get the array type of " + getName());
    }
}

/**
 * Retrieve the item version of the current super type
 *
 * @return Item version: e.g.: TYPE_INT for TYPE_INT_ARRAY
 */
SymbolTypeOld SymbolTypeOld::getItemType() {
    switch (superType) {
        case TYPE_DOUBLE_ARRAY: return SymbolTypeOld(TYPE_DOUBLE);
        case TYPE_INT_ARRAY: return SymbolTypeOld(TYPE_INT);
        case TYPE_BYTE_ARRAY: return SymbolTypeOld(TYPE_BYTE);
        case TYPE_CHAR_ARRAY: return SymbolTypeOld(TYPE_CHAR);
        case TYPE_STRING_ARRAY: return SymbolTypeOld(TYPE_STRING);
        case TYPE_BOOL_ARRAY: return SymbolTypeOld(TYPE_BOOL);
        case TYPE_STRUCT_ARRAY: return {TYPE_STRUCT, subType};
        case TYPE_DOUBLE_PTR_ARRAY: return SymbolTypeOld(TYPE_DOUBLE_PTR);
        case TYPE_INT_PTR_ARRAY: return SymbolTypeOld(TYPE_INT_PTR);
        case TYPE_BYTE_PTR_ARRAY: return SymbolTypeOld(TYPE_BYTE_PTR);
        case TYPE_CHAR_PTR_ARRAY: return SymbolTypeOld(TYPE_CHAR_PTR);
        case TYPE_STRING_PTR_ARRAY: return SymbolTypeOld(TYPE_STRING_PTR);
        case TYPE_BOOL_PTR_ARRAY: return SymbolTypeOld(TYPE_BOOL_PTR);
        case TYPE_STRUCT_PTR_ARRAY: return {TYPE_STRUCT_PTR, subType};
        default: throw std::runtime_error("Compiler error: Could not get the item type of " + getName());
    }
}

/**
 * Checks if the current type can be implicitly casted to the passed symbol type
 *
 * @param otherType Symbol type to cast to
 * @return Cast compatible or not
 */
bool SymbolTypeOld::isImplicitCastCompatibleWith(const SymbolTypeOld& otherType) {
    // Always return true when types are matching
    if (superType == otherType.superType) return true;
    // Define implicit casting rules
    bool castCompatible =
            (superType == TYPE_STRING && otherType.superType == TYPE_CHAR_PTR) ||           // Cast: string -> char*
            (superType == TYPE_CHAR_PTR && otherType.superType == TYPE_STRING) ||           // Cast: char* -> string
            (superType == TYPE_STRING && otherType.superType == TYPE_BYTE_PTR) ||           // Cast: string -> byte*
            (superType == TYPE_BYTE_PTR && otherType.superType == TYPE_STRING) ||           // Cast: byte* -> string
            (superType == TYPE_CHAR && otherType.superType == TYPE_BYTE) ||                 // Cast: char -> byte
            (superType == TYPE_BYTE && otherType.superType == TYPE_CHAR) ||                 // Cast: byte -> char
            (superType == TYPE_INT && otherType.superType == TYPE_BYTE);                    // Cast: int -> byte
    return castCompatible;
}

/**
 * Checks if this SymbolType's super type is one of the passed
 *
 * @param superTypes Super types to check
 * @return True = super type is one of the stated, otherwise false
 */
bool SymbolTypeOld::isOneOf(const std::vector<SymbolSuperTypeOld>& superTypes) {
    /*for (auto& type : superTypes) {
        if (type == superType) return true;
    }
    return false;*/
    return std::any_of(superTypes.begin(), superTypes.end(), [this](int type) {
        return type == superType;
    });
}

/**
 * Checks if this SymbolType's super type is the passed
 *
 * @param superType Super type to check
 * @return True = super type is one of the stated, otherwise false
 */
bool SymbolTypeOld::is(SymbolSuperTypeOld type) {
    return superType == type;
}


bool operator== (const SymbolTypeOld& lhs, const SymbolTypeOld& rhs) {
    return lhs.superType == rhs.superType && lhs.subType == rhs.subType;
}

bool operator!= (const SymbolTypeOld& lhs, const SymbolTypeOld& rhs) {
    return !(lhs == rhs);
}

bool SymbolTypeOld::matches(SymbolTypeOld type) {
    return superType == type.getSuperType();
}

bool SymbolTypeOld::matches(SymbolTypeOld symbolType, SymbolSuperTypeOld superSymbolType) {
    return superType == superSymbolType && symbolType.getSuperType() == superSymbolType;
}

std::string SymbolTypeOld::getName() {
    switch (superType) {
        case TYPE_DOUBLE: return "double";
        case TYPE_INT: return "int";
        case TYPE_BYTE: return "byte";
        case TYPE_CHAR: return "char";
        case TYPE_STRING: return "string";
        case TYPE_BOOL: return "bool";
        case TYPE_STRUCT: return "struct(" + subType + ")";
        case TYPE_DOUBLE_PTR: return "double*";
        case TYPE_INT_PTR: return "int*";
        case TYPE_BYTE_PTR: return "byte*";
        case TYPE_CHAR_PTR: return "char*";
        case TYPE_STRING_PTR: return "string*";
        case TYPE_BOOL_PTR: return "bool*";
        case TYPE_STRUCT_PTR: return "struct(" + subType + ")*";;
        case TYPE_DOUBLE_ARRAY: return "double[]";
        case TYPE_INT_ARRAY: return "int[]";
        case TYPE_BYTE_ARRAY: return "byte[]";
        case TYPE_CHAR_ARRAY: return "char[]";
        case TYPE_STRING_ARRAY: return "string[]";
        case TYPE_BOOL_ARRAY: return "bool[]";
        case TYPE_STRUCT_ARRAY: return "struct(" + subType + ")[]";
        case TYPE_DOUBLE_PTR_ARRAY: return "double*[]";
        case TYPE_INT_PTR_ARRAY: return "int*[]";
        case TYPE_BYTE_PTR_ARRAY: return "byte*[]";
        case TYPE_CHAR_PTR_ARRAY: return "char*[]";
        case TYPE_STRING_PTR_ARRAY: return "string*[]";
        case TYPE_BOOL_PTR_ARRAY: return "bool*[]";
        case TYPE_STRUCT_PTR_ARRAY: return "struct(" + subType + ")*[]";
        case TYPE_DYN: return "dyn";
        case TYPE_FUNCTION: return "function";
        case TYPE_PROCEDURE: return "procedure";
        case TYPE_IMPORT: return "import";
    }
    return "";
}

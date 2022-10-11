// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SemanticError.h"

#include <ast/AstNodes.h>
#include <util/CodeLoc.h>

SemanticError::SemanticError(const AstNode *node, const SemanticErrorType &type, const std::string &message) {
  errorMessage = "Semantic error in : " + node->codeLoc.toPrettyString() + ":\n";
  errorMessage += getMessagePrefix(type) + ": " + message + "\n\n";
  errorMessage += node->errorMessage;
}

SemanticError::SemanticError(const CodeLoc &codeLoc, const SemanticErrorType &type, const std::string &message) {
  errorMessage = "Semantic error in " + codeLoc.toPrettyString() + ": " + getMessagePrefix(type) + ": " + message;
}

/**
 * Get the message for this particular error instance
 *
 * @return Error message in form of a char array
 */
const char *SemanticError::what() const noexcept { return errorMessage.c_str(); }

/**
 * Get the prefix of the error message for a particular error
 *
 * @param type Type of the error
 * @return Prefix string for the error type
 */
std::string SemanticError::getMessagePrefix(SemanticErrorType type) {
  switch (type) {
  case REFERENCED_UNDEFINED_FUNCTION:
    return "Referenced undefined function";
  case REFERENCED_UNDEFINED_VARIABLE:
    return "Referenced undefined variable";
  case REFERENCED_UNDEFINED_STRUCT:
    return "Referenced undefined struct";
  case FUNCTION_AMBIGUITY:
    return "Function ambiguity";
  case STRUCT_AMBIGUITY:
    return "Struct ambiguity";
  case VARIABLE_DECLARED_TWICE:
    return "Multiple declarations of the same variable";
  case FUNCTION_DECLARED_TWICE:
    return "Multiple declarations of a function/procedure";
  case GENERIC_TYPE_DECLARED_TWICE:
    return "Multiple declarations of a generic type with the same name";
  case STRUCT_DECLARED_TWICE:
    return "Multiple declarations of a struct with the same name";
  case ENUM_DECLARED_TWICE:
    return "Multiple declarations of an enum with the same name";
  case DUPLICATE_ENUM_ITEM_NAME:
    return "Duplicate enum item name";
  case DUPLICATE_ENUM_ITEM_VALUE:
    return "Duplicate enum item value";
  case GLOBAL_OF_TYPE_DYN:
    return "Global of type dyn";
  case GLOBAL_OF_INVALID_TYPE:
    return "Global of invalid type";
  case GLOBAL_CONST_WITHOUT_VALUE:
    return "Global const without value";
  case FUNCTION_WITHOUT_RETURN_STMT:
    return "Missing return statement";
  case INVALID_PARAM_ORDER:
    return "Invalid argument order";
  case DTOR_MUST_BE_PROCEDURE:
    return "Destructor must be a procedure";
  case DTOR_WITH_PARAMS:
    return "Destructors must not have parameters";
  case OPERATOR_WRONG_DATA_TYPE:
    return "Wrong data type for operator";
  case UNEXPECTED_DYN_TYPE_SA:
    return "Unexpected dyn type";
  case REASSIGN_CONST_VARIABLE:
    return "Cannot re-assign constant variable";
  case CONDITION_MUST_BE_BOOL:
    return "Condition must be bool";
  case MISSING_MAIN_FUNCTION:
    return "Spice programs must contain a main function";
  case FCT_PARAM_IS_TYPE_DYN:
    return "Parameter type dyn not valid in function/procedure definition without default value";
  case INVALID_BREAK_NUMBER:
    return "Invalid number of break calls";
  case INVALID_CONTINUE_NUMBER:
    return "Invalid number of continue calls";
  case PRINTF_TYPE_ERROR:
    return "Types of printf call not matching";
  case PRINTF_ARG_COUNT_ERROR:
    return "Printf arg number not matching template string";
  case STD_NOT_FOUND:
    return "Std lib not found";
  case DUPLICATE_IMPORT_NAME:
    return "Duplicate import name";
  case IMPORTED_FILE_NOT_EXISTING:
    return "Imported source file not existing";
  case CIRCULAR_DEPENDENCY:
    return "Circular import detected";
  case MEMBER_ACCESS_ONLY_STRUCTS:
    return "Member access is only allowed on structs";
  case SCOPE_ACCESS_ONLY_IMPORTS:
    return "Scope access is only allowed on imports";
  case UNKNOWN_DATATYPE:
    return "Unknown datatype";
  case NUMBER_OF_FIELDS_NOT_MATCHING:
    return "Number of struct fields not matching declaration";
  case FIELD_TYPE_NOT_MATCHING:
    return "The type of a field value does not match the declaration";
  case ARRAY_SIZE_INVALID:
    return "Array size invalid";
  case ARRAY_INDEX_NOT_INT_OR_LONG:
    return "Array index not of type int or long";
  case ARRAY_ITEM_TYPE_NOT_MATCHING:
    return "Array item type not matching";
  case EXPECTED_ARRAY_TYPE:
    return "Expected array type";
  case SIZEOF_DYNAMIC_SIZED_ARRAY:
    return "Sizeof dynamically sized array";
  case RETURN_WITHOUT_VALUE_RESULT:
    return "Return without initialization of result variable";
  case RETURN_WITH_VALUE_IN_PROCEDURE:
    return "Return with value in procedure";
  case DYN_POINTERS_NOT_ALLOWED:
    return "Dyn pointers not allowed";
  case DYN_ARRAYS_NOT_ALLOWED:
    return "Dyn arrays not allowed";
  case GENERIC_TYPE_NOT_IN_TEMPLATE:
    return "Generic type not contained in template";
  case SPECIFIER_AT_ILLEGAL_CONTEXT:
    return "Specifier at illegal context";
  case INSUFFICIENT_VISIBILITY:
    return "Insufficient symbol visibility";
  case TID_INVALID:
    return "Invalid thread id";
  case JOIN_ARG_MUST_BE_TID:
    return "Argument of join builtin must be a tid";
  case EXPECTED_GENERIC_TYPE:
    return "Expected a generic type";
  case EXPECTED_VALUE:
    return "Expected value";
  case EXPECTED_TYPE:
    return "Expected type";
  case UNSAFE_OPERATION_IN_SAFE_CONTEXT:
    return "Unsafe operation in safe context";
  case ASSERTION_CONDITION_BOOL:
    return "Assertion condition must be bool";
  case ARRAY_INDEX_OUT_OF_BOUNDS:
    return "Array index out of bounds";
  case RESERVED_KEYWORD:
    return "Reserved keyword used as identifier";
  case COMING_SOON_SA:
    return "Coming soon";
  }
  return "Unknown error"; // GCOV_EXCL_LINE
}
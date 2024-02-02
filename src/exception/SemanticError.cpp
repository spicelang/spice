// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "SemanticError.h"

#include <ast/ASTNodes.h>
#include <util/CodeLoc.h>

namespace spice::compiler {

SemanticError::SemanticError(const ASTNode *node, const SemanticErrorType &type, const std::string &msg, bool printErrorMessage) {
  assert(node != nullptr);
  errorMessage = "[Error|Semantic] " + node->codeLoc.toPrettyString() + ":\n";
  errorMessage += getMessagePrefix(type) + ": " + msg;
  if (printErrorMessage) {
    const std::string nodeErrorMessage = node->getErrorMessage();
    errorMessage += "\n\n" + nodeErrorMessage;
  }
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
  case REFERENCED_UNDEFINED_FIELD:
    return "Referenced undefined struct field";
  case USED_BEFORE_DECLARED:
    return "Used before declared";
  case FUNCTION_AMBIGUITY:
    return "Function ambiguity";
  case STRUCT_AMBIGUITY:
    return "Struct ambiguity";
  case INTERFACE_AMBIGUITY:
    return "Interface ambiguity";
  case DUPLICATE_SYMBOL:
    return "Duplicate symbol";
  case VARIABLE_DECLARED_TWICE:
    return "Multiple declarations of the same variable";
  case GLOBAL_DECLARED_TWICE:
    return "Multiple declarations of the same global variable";
  case FUNCTION_DECLARED_TWICE:
    return "Multiple declarations of a function/procedure";
  case GENERIC_TYPE_DECLARED_TWICE:
    return "Multiple declarations of a generic type with the same name";
  case STRUCT_WITH_ILLEGAL_NAME:
    return "Struct name illegal";
  case STRUCT_INFINITE_SIZE:
    return "Struct with infinite size";
  case STRUCT_DECLARED_TWICE:
    return "Multiple declarations of a struct with the same name";
  case MISSING_NO_ARGS_CTOR:
    return "No matching ctor found";
  case INTERFACE_DECLARED_TWICE:
    return "Multiple declarations of an interface with the same name";
  case INTERFACE_METHOD_NOT_IMPLEMENTED:
    return "Interface method not implemented";
  case ENUM_DECLARED_TWICE:
    return "Multiple declarations of an enum with the same name";
  case INVALID_SYMBOL_ACCESS:
    return "Invalid symbol access";
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
  case MISSING_RETURN_STMT:
    return "Missing return statement";
  case INVALID_PARAM_ORDER:
    return "Invalid argument order";
  case LAMBDA_WITH_OPTIONAL_PARAMS:
    return "Lambda with optional parameters";
  case REFERENCED_OVERLOADED_FCT:
    return "Referenced overloaded function";
  case DTOR_MUST_BE_PROCEDURE:
    return "Destructor must be a procedure";
  case DTOR_WITH_PARAMS:
    return "Destructors must not have parameters";
  case OPERATOR_WRONG_DATA_TYPE:
    return "Wrong data type for operator";
  case INVALID_ITERATOR:
    return "Invalid iterator";
  case UNEXPECTED_DYN_TYPE:
    return "Unexpected dyn type";
  case REASSIGN_CONST_VARIABLE:
    return "Cannot re-assign constant variable";
  case CONDITION_MUST_BE_BOOL:
    return "Condition must be bool";
  case SWITCH_EXPR_MUST_BE_PRIMITIVE:
    return "Switch expression of wrong type";
  case SWITCH_CASE_TYPE_MISMATCH:
    return "Case constant type does not match switch expression type";
  case MISSING_MAIN_FUNCTION:
    return "Spice programs must contain a main function";
  case FCT_PARAM_IS_TYPE_DYN:
    return "Parameter type dyn not valid in function/procedure definition without default value";
  case INVALID_BREAK_NUMBER:
    return "Invalid number of break calls";
  case INVALID_CONTINUE_NUMBER:
    return "Invalid number of continue calls";
  case FALLTHROUGH_NOT_ALLOWED:
    return "Fallthrough statement not allowed here";
  case PRINTF_TYPE_ERROR:
    return "Types of printf call not matching";
  case PRINTF_ARG_COUNT_ERROR:
    return "Printf arg number not matching template string";
  case DUPLICATE_IMPORT_NAME:
    return "Duplicate import name";
  case IMPORTED_FILE_NOT_EXISTING:
    return "Imported source file not existing";
  case CIRCULAR_DEPENDENCY:
    return "Circular import detected";
  case ACCESS_TO_NON_EXISTING_MEMBER:
    return "Access to non-existing member";
  case INVALID_MEMBER_ACCESS:
    return "Member access is only allowed on structs and Strings";
  case SCOPE_ACCESS_ONLY_IMPORTS:
    return "Scope access is only allowed on imports";
  case UNKNOWN_DATATYPE:
    return "Unknown datatype";
  case UNKNOWN_ATTR:
    return "Unknown attribute";
  case INVALID_ATTR_TARGET:
    return "Invalid attribute target";
  case MISSING_ATTR_VALUE:
    return "Missing attribute value";
  case NUMBER_OF_FIELDS_NOT_MATCHING:
    return "Number of struct fields not matching declaration";
  case FIELD_TYPE_NOT_MATCHING:
    return "The type of a field value does not match the declaration";
  case ARRAY_SIZE_INVALID:
    return "Array size invalid";
  case FOREACH_IDX_NOT_LONG:
    return "Foreach index not of type long";
  case ARRAY_INDEX_NOT_INT_OR_LONG:
    return "Array index not of type int or long";
  case ARRAY_ITEM_TYPE_NOT_MATCHING:
    return "Array item type not matching";
  case EXPECTED_ARRAY_TYPE:
    return "Expected array type";
  case EXPECTED_ERROR_TYPE:
    return "Expected error type";
  case RETURN_WITHOUT_VALUE_RESULT:
    return "Return without initialization of result variable";
  case RETURN_WITH_VALUE_IN_PROCEDURE:
    return "Return with value in procedure";
  case DYN_POINTERS_NOT_ALLOWED:
    return "Dyn pointers not allowed";
  case REF_POINTERS_ARE_NOT_ALLOWED:
    return "Ref pointers not allowed";
  case DYN_REFERENCES_NOT_ALLOWED:
    return "Dyn references not allowed";
  case MULTI_REF_NOT_ALLOWED:
    return "Multi references not allowed";
  case DYN_ARRAYS_NOT_ALLOWED:
    return "Dyn arrays not allowed";
  case REFERENCE_WITHOUT_INITIALIZER:
    return "Reference without initializer";
  case TEMP_TO_NON_CONST_REF:
    return "Temporary bound to non-const reference";
  case GENERIC_TYPE_NOT_IN_TEMPLATE:
    return "Generic type not contained in template";
  case SPECIFIER_AT_ILLEGAL_CONTEXT:
    return "Specifier at illegal context";
  case INSUFFICIENT_VISIBILITY:
    return "Insufficient symbol visibility";
  case JOIN_ARG_MUST_BE_TID:
    return "Argument of join builtin must be a tid";
  case EXPECTED_GENERIC_TYPE:
    return "Expected a generic type";
  case EXPECTED_NON_GENERIC_TYPE:
    return "Expected a non-generic type";
  case EXPECTED_STRUCT_TYPE:
    return "Expected a struct type";
  case EXPECTED_INTERFACE_TYPE:
    return "Expected an interface type";
  case ALIAS_WITH_TEMPLATE_LIST:
    return "Alias with template list";
  case INTERFACE_WITH_TEMPLATE_LIST:
    return "Interface reference with template list";
  case INVALID_TEMPLATE_TYPES:
    return "Invalid template types";
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
  case EXPECTED_CONST_VARIABLE:
    return "Expected const variable";
  case DIVISION_BY_ZERO:
    return "Division by zero";
  case TEST_FUNCTION_WITH_PARAMS:
    return "Test function with parameters";
  case TEST_FUNCTION_WRONG_RETURN_TYPE:
    return "Test function with wrong return type";
  case COMING_SOON_SA:
    return "Coming soon";
  }
  return "Unknown error"; // GCOV_EXCL_LINE
}

} // namespace spice::compiler
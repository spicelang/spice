// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>

#include <Token.h>

namespace spice::compiler {

// Forward declarations
class ASTNode;
class SourceFile;
struct CodeLoc;

enum SemanticErrorType : uint8_t {
  REFERENCED_UNDEFINED_FUNCTION,
  REFERENCED_UNDEFINED_VARIABLE,
  REFERENCED_UNDEFINED_STRUCT,
  REFERENCED_UNDEFINED_FIELD,
  USED_BEFORE_DECLARED,
  FUNCTION_AMBIGUITY,
  STRUCT_AMBIGUITY,
  INTERFACE_AMBIGUITY,
  DUPLICATE_SYMBOL,
  VARIABLE_DECLARED_TWICE,
  GLOBAL_DECLARED_TWICE,
  FUNCTION_DECLARED_TWICE,
  GENERIC_TYPE_DECLARED_TWICE,
  STRUCT_WITH_ILLEGAL_NAME,
  STRUCT_INFINITE_SIZE,
  STRUCT_DECLARED_TWICE,
  MISSING_NO_ARGS_CTOR,
  INTERFACE_DECLARED_TWICE,
  INTERFACE_METHOD_NOT_IMPLEMENTED,
  ENUM_DECLARED_TWICE,
  INVALID_SYMBOL_ACCESS,
  DUPLICATE_ENUM_ITEM_NAME,
  DUPLICATE_ENUM_ITEM_VALUE,
  GLOBAL_OF_TYPE_DYN,
  GLOBAL_OF_INVALID_TYPE,
  GLOBAL_CONST_WITHOUT_VALUE,
  MISSING_RETURN_STMT,
  INVALID_PARAM_ORDER,
  LAMBDA_WITH_OPTIONAL_PARAMS,
  REFERENCED_OVERLOADED_FCT,
  DTOR_MUST_BE_PROCEDURE,
  DTOR_WITH_PARAMS,
  OPERATOR_WRONG_DATA_TYPE,
  INVALID_ITERATOR,
  UNEXPECTED_DYN_TYPE,
  REASSIGN_CONST_VARIABLE,
  CONDITION_MUST_BE_BOOL,
  SWITCH_EXPR_MUST_BE_PRIMITIVE,
  SWITCH_CASE_TYPE_MISMATCH,
  MISSING_MAIN_FUNCTION,
  FCT_PARAM_IS_TYPE_DYN,
  INVALID_BREAK_NUMBER,
  INVALID_CONTINUE_NUMBER,
  FALLTHROUGH_NOT_ALLOWED,
  PRINTF_TYPE_ERROR,
  PRINTF_ARG_COUNT_ERROR,
  DUPLICATE_IMPORT_NAME,
  IMPORTED_FILE_NOT_EXISTING,
  CIRCULAR_DEPENDENCY,
  ACCESS_TO_NON_EXISTING_MEMBER,
  INVALID_MEMBER_ACCESS,
  SCOPE_ACCESS_ONLY_IMPORTS,
  UNKNOWN_DATATYPE,
  UNKNOWN_ATTR,
  INVALID_ATTR_TARGET,
  MISSING_ATTR_VALUE,
  NUMBER_OF_FIELDS_NOT_MATCHING,
  FIELD_TYPE_NOT_MATCHING,
  ARRAY_SIZE_INVALID,
  FOREACH_IDX_NOT_LONG,
  ARRAY_INDEX_NOT_INT_OR_LONG,
  ARRAY_ITEM_TYPE_NOT_MATCHING,
  EXPECTED_ARRAY_TYPE,
  EXPECTED_ERROR_TYPE,
  RETURN_WITHOUT_VALUE_RESULT,
  RETURN_WITH_VALUE_IN_PROCEDURE,
  DYN_POINTERS_NOT_ALLOWED,
  REF_POINTERS_ARE_NOT_ALLOWED,
  DYN_REFERENCES_NOT_ALLOWED,
  MULTI_REF_NOT_ALLOWED,
  DYN_ARRAYS_NOT_ALLOWED,
  REFERENCE_WITHOUT_INITIALIZER,
  TEMP_TO_NON_CONST_REF,
  GENERIC_TYPE_NOT_IN_TEMPLATE,
  GENERIC_TYPE_NOT_USED,
  SPECIFIER_AT_ILLEGAL_CONTEXT,
  INSUFFICIENT_VISIBILITY,
  JOIN_ARG_MUST_BE_TID,
  EXPECTED_GENERIC_TYPE,
  EXPECTED_NON_GENERIC_TYPE,
  EXPECTED_STRUCT_TYPE,
  EXPECTED_INTERFACE_TYPE,
  ALIAS_WITH_TEMPLATE_LIST,
  INTERFACE_WITH_TEMPLATE_LIST,
  INVALID_TEMPLATE_TYPES,
  EXPECTED_VALUE,
  EXPECTED_TYPE,
  UNSAFE_OPERATION_IN_SAFE_CONTEXT,
  ASSERTION_CONDITION_BOOL,
  ARRAY_INDEX_OUT_OF_BOUNDS,
  EXPECTED_CONST_VARIABLE,
  DIVISION_BY_ZERO,
  TEST_FUNCTION_WITH_PARAMS,
  TEST_FUNCTION_WRONG_RETURN_TYPE,
  COMING_SOON_SA
};

/**
 * Custom exception for errors, occurring in the semantic analysis phase
 */
class SemanticError : public std::exception {
public:
  // Constructors
  SemanticError(const ASTNode *node, const SemanticErrorType &type, const std::string &message, bool printErrorMessage = true);

  // Public methods
  [[nodiscard]] const char *what() const noexcept override;
  [[nodiscard]] static std::string getMessagePrefix(SemanticErrorType errorType);

  // Public members
  std::string errorMessage;
};

} // namespace spice::compiler
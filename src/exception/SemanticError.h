// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>

#include <Token.h>

namespace spice::compiler {

// Forward declarations
class ASTNode;
struct CodeLoc;

enum SemanticErrorType {
  REFERENCED_UNDEFINED_FUNCTION,
  REFERENCED_UNDEFINED_VARIABLE,
  REFERENCED_UNDEFINED_STRUCT,
  REFERENCED_UNDEFINED_FIELD,
  USED_BEFORE_DECLARED,
  FUNCTION_AMBIGUITY,
  STRUCT_AMBIGUITY,
  VARIABLE_DECLARED_TWICE,
  GLOBAL_DECLARED_TWICE,
  FUNCTION_DECLARED_TWICE,
  GENERIC_TYPE_DECLARED_TWICE,
  STRUCT_WITH_ILLEGAL_NAME,
  STRUCT_INFINITE_SIZE,
  STRUCT_DECLARED_TWICE,
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
  DTOR_MUST_BE_PROCEDURE,
  DTOR_WITH_PARAMS,
  OPERATOR_WRONG_DATA_TYPE,
  UNEXPECTED_DYN_TYPE_SA,
  REASSIGN_CONST_VARIABLE,
  CONDITION_MUST_BE_BOOL,
  MISSING_MAIN_FUNCTION,
  FCT_PARAM_IS_TYPE_DYN,
  INVALID_BREAK_NUMBER,
  INVALID_CONTINUE_NUMBER,
  PRINTF_TYPE_ERROR,
  PRINTF_ARG_COUNT_ERROR,
  STD_NOT_FOUND,
  DUPLICATE_IMPORT_NAME,
  IMPORTED_FILE_NOT_EXISTING,
  CIRCULAR_DEPENDENCY,
  INVALID_MEMBER_ACCESS,
  SCOPE_ACCESS_ONLY_IMPORTS,
  UNKNOWN_DATATYPE,
  NUMBER_OF_FIELDS_NOT_MATCHING,
  FIELD_TYPE_NOT_MATCHING,
  ARRAY_SIZE_INVALID,
  ARRAY_INDEX_NOT_INT,
  ARRAY_INDEX_NOT_INT_OR_LONG,
  ARRAY_ITEM_TYPE_NOT_MATCHING,
  EXPECTED_ARRAY_TYPE,
  RETURN_WITHOUT_VALUE_RESULT,
  RETURN_WITH_VALUE_IN_PROCEDURE,
  DYN_POINTERS_NOT_ALLOWED,
  DYN_REFERENCES_NOT_ALLOWED,
  DYN_ARRAYS_NOT_ALLOWED,
  REFERENCE_WITHOUT_INITIALIZER,
  GENERIC_TYPE_NOT_IN_TEMPLATE,
  SPECIFIER_AT_ILLEGAL_CONTEXT,
  INSUFFICIENT_VISIBILITY,
  TID_INVALID,
  JOIN_ARG_MUST_BE_TID,
  EXPECTED_GENERIC_TYPE,
  EXPECTED_NON_GENERIC_TYPE,
  EXPECTED_STRUCT_TYPE,
  EXPECTED_INTERFACE_TYPE,
  INTERFACE_WITH_TEMPLATE_LIST,
  EXPECTED_VALUE,
  EXPECTED_TYPE,
  UNSAFE_OPERATION_IN_SAFE_CONTEXT,
  ASSERTION_CONDITION_BOOL,
  ARRAY_INDEX_OUT_OF_BOUNDS,
  COMING_SOON_SA
};

/**
 * Custom exception for errors, occurring in the semantic analysis phase
 */
class SemanticError : public std::exception {
public:
  // Constructors
  SemanticError(const ASTNode *node, const SemanticErrorType &type, const std::string &message);

  // Public methods
  [[nodiscard]] const char *what() const noexcept override;

private:
  // Members
  std::string errorMessage;

  // Private methods
  [[nodiscard]] static std::string getMessagePrefix(SemanticErrorType errorType);
};

} // namespace spice::compiler
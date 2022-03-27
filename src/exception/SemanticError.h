// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <exception>
#include <string>

#include <Token.h>

enum SemanticErrorType {
  REFERENCED_UNDEFINED_FUNCTION,
  REFERENCED_UNDEFINED_VARIABLE,
  REFERENCED_UNDEFINED_STRUCT,
  FUNCTION_AMBIGUITY,
  VARIABLE_DECLARED_TWICE,
  FUNCTION_DECLARED_TWICE,
  GENERIC_TYPE_DECLARED_TWICE,
  STRUCT_DECLARED_TWICE,
  GLOBAL_OF_TYPE_DYN,
  GLOBAL_OF_INVALID_TYPE,
  FUNCTION_WITHOUT_RETURN_STMT,
  INVALID_ARGUMENT_ORDER,
  OPERATOR_WRONG_DATA_TYPE,
  UNEXPECTED_DYN_TYPE_SA,
  REASSIGN_CONST_VARIABLE,
  CONDITION_MUST_BE_BOOL,
  MISSING_MAIN_FUNCTION,
  FCT_ARG_IS_TYPE_DYN,
  INVALID_BREAK_NUMBER,
  INVALID_CONTINUE_NUMBER,
  PRINTF_TYPE_ERROR,
  PRINTF_ARG_COUNT_ERROR,
  STD_NOT_FOUND,
  IMPORTED_FILE_NOT_EXISTING,
  CIRCULAR_DEPENDENCY,
  UNKNOWN_DATATYPE,
  NUMBER_OF_FIELDS_NOT_MATCHING,
  FIELD_TYPE_NOT_MATCHING,
  ARRAY_SIZE_INVALID,
  ARRAY_INDEX_NO_INTEGER,
  ARRAY_ITEM_TYPE_NOT_MATCHING,
  RETURN_WITHOUT_VALUE_RESULT,
  RETURN_WITH_VALUE_IN_PROCEDURE,
  DYN_POINTERS_NOT_ALLOWED,
  DYN_ARRAYS_NOT_ALLOWED,
  SPECIFIER_AT_ILLEGAL_CONTEXT,
  INSUFFICIENT_VISIBILITY,
  TID_INVALID,
  JOIN_ARG_MUST_BE_TID,
  RESERVED_KEYWORD,
  COMING_SOON_SA,
};

/**
 * Custom exception for errors, occurring in the semantic analysis phase
 */
class SemanticError : public std::exception {
public:
  // Constructors
  explicit SemanticError(const std::string &fileName, const antlr4::Token &token, const SemanticErrorType &type,
                         const std::string &message);

  // Public methods
  [[nodiscard]] const char *what() const noexcept override;

private:
  // Members
  std::string errorMessage;

  [[nodiscard]] static std::string getMessagePrefix(SemanticErrorType errorType);
};
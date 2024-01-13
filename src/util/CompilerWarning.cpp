// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "CompilerWarning.h"

#include <util/CodeLoc.h>

namespace spice::compiler {

/**
 * Constructor: Used in case that the exact code position where the warning occurred is known
 *
 * @param codeLoc Code location, where the warning occurred
 * @param type Type of the warning
 * @param message Warning message suffix
 */
CompilerWarning::CompilerWarning(const CodeLoc &codeLoc, CompilerWarningType type, const std::string &message) {
  warningMessage = "[Warning] " + codeLoc.toPrettyString() + ": " + getMessagePrefix(type) + ": " + message;
}

/**
 * Constructor: Used in case the exact code position where the warning occurred is not known
 *
 * @param type Type of the warning
 * @param message Warning message suffix
 */
CompilerWarning::CompilerWarning(CompilerWarningType type, const std::string &message) {
  warningMessage = "[Warning] " + getMessagePrefix(type) + ": " + message;
}

/**
 * Print the compiler warning to the standard error output
 */
void CompilerWarning::print() const { std::cout << "\033[33m" << warningMessage << "\033[0m\n"; }

/**
 * Get the prefix of the warning message for a particular error
 *
 * @param type Type of the warning
 * @return Prefix string for the warning type
 */
std::string CompilerWarning::getMessagePrefix(CompilerWarningType type) {
  switch (type) {
  case UNUSED_FUNCTION:
    return "Unused function";
  case UNUSED_PROCEDURE:
    return "Unused procedure";
  case UNUSED_METHOD:
    return "Unused method";
  case UNUSED_STRUCT:
    return "Unused struct";
  case UNUSED_INTERFACE:
    return "Unused interface";
  case UNUSED_IMPORT:
    return "Unused import";
  case UNUSED_FIELD:
    return "Unused field";
  case UNUSED_ENUM_ITEM:
    return "Unused enum item";
  case UNUSED_ALIAS:
    return "Unused type alias";
  case UNUSED_VARIABLE:
    return "Unused variable";
  case UNUSED_RETURN_VALUE:
    return "Unused return value";
  case UNREACHABLE_CODE:
    return "Unreachable code detected";
  case SHADOWED_VARIABLE:
    return "Shadowed variable";
  case IDENTITY_CAST:
    return "Identity cast";
  case SINGLE_GENERIC_TYPE_CONDITION:
    return "Only one type condition";
  case BOOL_ASSIGN_AS_CONDITION:
    return "Bool assignment as condition";
  case ASYNC_LAMBDA_CAPTURE_RULE_VIOLATION:
    return "Lambda violates async lambda capture rules";
  case UNINSTALL_FAILED:
    return "Uninstall failed";
  case VERIFIER_DISABLED:
    return "Verifier disabled";
  }
  return "Unknown warning"; // GCOV_EXCL_LINE
}

} // namespace spice::compiler
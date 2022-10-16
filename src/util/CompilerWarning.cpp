// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CompilerWarning.h"

#include <util/CodeLoc.h>

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
void CompilerWarning::print() const { std::cout << warningMessage << "\n"; }

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
  case UNUSED_STRUCT:
    return "Unused struct";
  case UNUSED_IMPORT:
    return "Unused import";
  case UNUSED_FIELD:
    return "Unused field";
  case UNUSED_VARIABLE:
    return "Unused variable";
  case SINGLE_GENERIC_TYPE_CONDITION:
    return "Only one type condition";
  case BOOL_ASSIGN_AS_CONDITION:
    return "Bool assignment as condition";
  case INDEX_EXCEEDS_ARRAY_SIZE:
    return "Array index exceeds its size";
  case UNINSTALL_FAILED:
    return "Uninstall failed";
  case VERIFIER_DISABLED:
    return "Verifier disabled";
  }
  return "Unknown warning"; // GCOV_EXCL_LINE
}
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

// Forward declaration
struct CodeLoc;

enum CompilerWarningType {
  UNUSED_FUNCTION,
  UNUSED_PROCEDURE,
  UNUSED_STRUCT,
  UNUSED_INTERFACE,
  UNUSED_IMPORT,
  UNUSED_FIELD,
  UNUSED_VARIABLE,
  INTERFACE_WITHOUT_SIGNATURE,
  SINGLE_GENERIC_TYPE_CONDITION,
  BOOL_ASSIGN_AS_CONDITION,
  INDEX_EXCEEDS_ARRAY_SIZE,
  UNINSTALL_FAILED,
  VERIFIER_DISABLED
};

/**
 * Compiler warning template engine
 */
class CompilerWarning {
public:
  // Constructors
  explicit CompilerWarning(const CodeLoc &codeLoc, CompilerWarningType type, const std::string &message);
  explicit CompilerWarning(CompilerWarningType type, const std::string &message);

  // Public methods
  void print() const;

  // Public members
  std::string warningMessage;

private:
  // Private methods
  [[nodiscard]] static std::string getMessagePrefix(CompilerWarningType warningType);
};
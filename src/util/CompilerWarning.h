// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

// Forward declaration
struct CodeLoc;

enum CompilerWarningType {
  UNUSED_FUNCTION,
  UNUSED_PROCEDURE,
  UNUSED_STRUCT,
  UNUSED_IMPORT,
  UNUSED_VARIABLE,
  ARRAY_TOO_MANY_VALUES,
  INDEX_EXCEEDS_ARRAY_SIZE,
  NEGATIVE_VALUE_TO_UNSIGNED_VAR,
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

  void print() const;

private:
  // Members
  std::string warningMessage{};

  [[nodiscard]] static std::string getMessagePrefix(CompilerWarningType warningType);
};
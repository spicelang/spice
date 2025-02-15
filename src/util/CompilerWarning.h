// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <string>

namespace spice::compiler {

// Forward declaration
struct CodeLoc;

enum CompilerWarningType : uint8_t {
  UNUSED_FUNCTION,
  UNUSED_PROCEDURE,
  UNUSED_METHOD,
  UNUSED_STRUCT,
  UNUSED_INTERFACE,
  UNUSED_IMPORT,
  UNUSED_FIELD,
  UNUSED_ENUM_ITEM,
  UNUSED_ALIAS,
  UNUSED_VARIABLE,
  UNUSED_RETURN_VALUE,
  UNREACHABLE_CODE,
  SHADOWED_VARIABLE,
  IDENTITY_CAST,
  SINGLE_GENERIC_TYPE_CONDITION,
  INEFFECTIVE_GENERIC_TYPE_CONDITION,
  BOOL_ASSIGN_AS_CONDITION,
  ASYNC_LAMBDA_CAPTURE_RULE_VIOLATION,
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

} // namespace spice::compiler
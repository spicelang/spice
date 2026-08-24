// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace spice::compiler {

// Forward declaration
struct CodeLoc;

enum class LintSeverity : uint8_t {
  INFO,
  WARNING,
  ERROR,
};

/**
 * A single diagnostic produced by a LintRule, e.g. a naming convention violation.
 */
class LintFinding {
public:
  // Constructors
  LintFinding(const CodeLoc &codeLoc, std::string_view ruleId, LintSeverity severity, const std::string &message);

  // Public methods
  void print() const;

  // Public members
  std::string ruleId;
  LintSeverity severity;
  std::string findingMessage;

private:
  // Private methods
  [[nodiscard]] static const char *getSeverityLabel(LintSeverity severity);
};

} // namespace spice::compiler

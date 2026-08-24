// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "LintFinding.h"

#include <iostream>

#include <util/CodeLoc.h>
#include <util/GlobalDefinitions.h>

namespace spice::compiler {

/**
 * Constructor
 *
 * @param codeLoc Code location, where the lint finding occurred
 * @param ruleId Id of the rule that produced this finding, e.g. "naming-convention"
 * @param severity Severity of the finding
 * @param message Finding message suffix
 */
LintFinding::LintFinding(const CodeLoc &codeLoc, const std::string_view ruleId, const LintSeverity severity, const std::string &message)
    : ruleId(ruleId), severity(severity) {
  findingMessage = "[" + std::string(getSeverityLabel(severity)) + "] " + codeLoc.toPrettyString() + ": " + this->ruleId + ": " + message;
}

/**
 * Print the lint finding to standard output
 */
void LintFinding::print() const {
  auto color = "\033[36m"; // INFO: cyan
  if (severity == LintSeverity::WARNING)
    color = "\033[33m"; // yellow
  else if (severity == LintSeverity::ERROR)
    color = "\033[31m"; // red
  std::cout << color << findingMessage << "\033[0m\n";
}

/**
 * Get the human-readable label for a lint severity
 *
 * @param severity Severity of the finding
 * @return Label string for the severity
 */
const char *LintFinding::getSeverityLabel(const LintSeverity severity) {
  switch (severity) {
  case LintSeverity::INFO:
    return "Info";
  case LintSeverity::WARNING:
    return "Lint";
  case LintSeverity::ERROR:
    return "Lint error";
  }
  assert_fail("Unknown lint severity"); // GCOV_EXCL_LINE
  return "Unknown";                     // GCOV_EXCL_LINE
}

} // namespace spice::compiler

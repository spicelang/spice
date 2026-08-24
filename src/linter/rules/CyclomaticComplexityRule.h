// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <linter/LintRule.h>

namespace spice::compiler {

/**
 * Flags functions/procedures whose body has grown too many independent branches to review or test
 * with confidence in one piece. McCabe cyclomatic complexity: starts at 1 for the single
 * straight-line path through the body, +1 for every `if`, loop, and `case` branch. A
 * style/maintainability check, not a correctness one, so it lives in `spice lint` rather than as a
 * compiler warning.
 */
class CyclomaticComplexityRule final : public LintRule {
public:
  [[nodiscard]] constexpr std::string_view id() const override { return "cyclomatic-complexity"; }

  void checkFctDef(FctDefNode *node, std::vector<LintFinding> &findings) override;
  void checkProcDef(ProcDefNode *node, std::vector<LintFinding> &findings) override;
};

} // namespace spice::compiler

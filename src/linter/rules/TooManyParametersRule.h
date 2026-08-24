// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <linter/LintRule.h>

namespace spice::compiler {

/**
 * Flags functions/procedures with more parameters than a caller can reasonably keep straight at a
 * call site. A style/maintainability check (grouping related parameters into a struct is usually
 * the fix), not a correctness one, so it lives in `spice lint` rather than as a compiler warning.
 */
class TooManyParametersRule final : public LintRule {
public:
  [[nodiscard]] constexpr std::string_view id() const override { return "too-many-parameters"; }

  void checkFctDef(FctDefNode *node, std::vector<LintFinding> &findings) override;
  void checkProcDef(ProcDefNode *node, std::vector<LintFinding> &findings) override;
};

} // namespace spice::compiler

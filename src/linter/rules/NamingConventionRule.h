// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <linter/LintRule.h>

namespace spice::compiler {

/**
 * Flags Spice-source identifiers that violate the naming conventions documented in STYLE_GUIDE.md
 * (functions/procedures/methods: camelCase; structs/interfaces: PascalCase).
 *
 * This is deliberately a style rule, not a semantic one: unlike unused-symbol or shadowing checks
 * (which stay compiler warnings, see CompilerWarning.h), naming conventions are opinionated and
 * belong to the opt-in `spice lint` command rather than every `build`/`run`/`test` invocation.
 */
class NamingConventionRule final : public LintRule {
public:
  [[nodiscard]] constexpr std::string_view id() const override { return "naming-convention"; }

  void checkFctDef(FctDefNode *node, std::vector<LintFinding> &findings) override;
  void checkProcDef(ProcDefNode *node, std::vector<LintFinding> &findings) override;
  void checkStructDef(StructDefNode *node, std::vector<LintFinding> &findings) override;
  void checkInterfaceDef(InterfaceDefNode *node, std::vector<LintFinding> &findings) override;
  void checkGlobalVarDef(GlobalVarDefNode *node, std::vector<LintFinding> &findings) override;
};

} // namespace spice::compiler

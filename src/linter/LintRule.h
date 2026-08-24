// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <string_view>
#include <vector>

#include <linter/LintFinding.h>

namespace spice::compiler {

// Forward declarations
class FctDefNode;
class ProcDefNode;
class StructDefNode;
class InterfaceDefNode;
class GlobalVarDefNode;

/**
 * Base class for a single lint rule.
 *
 * A rule overrides only the check hooks for the node kinds it cares about; LintPass drives the
 * AST traversal once and calls into every registered rule from the matching visit method, so
 * adding a new rule never requires touching the traversal itself. See linter/rules/ for concrete
 * rules and linter/LintPass.h for how rules get registered and dispatched.
 */
class LintRule {
public:
  virtual ~LintRule() = default;

  // Rule identity
  [[nodiscard]] constexpr virtual std::string_view id() const = 0;

  // Check hooks. Default to no-op; override only the ones relevant for this rule.
  virtual void checkFctDef(FctDefNode *node, std::vector<LintFinding> &findings) {}
  virtual void checkProcDef(ProcDefNode *node, std::vector<LintFinding> &findings) {}
  virtual void checkStructDef(StructDefNode *node, std::vector<LintFinding> &findings) {}
  virtual void checkInterfaceDef(InterfaceDefNode *node, std::vector<LintFinding> &findings) {}
  virtual void checkGlobalVarDef(GlobalVarDefNode *node, std::vector<LintFinding> &findings) {}
};

} // namespace spice::compiler

// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "CyclomaticComplexityRule.h"

#include <ast/ASTNodes.h>
#include <util/GlobalDefinitions.h>

namespace spice::compiler {

// McCabe's original recommendation for the complexity ceiling of a single routine.
static constexpr size_t MAX_CYCLOMATIC_COMPLEXITY = 10;

/**
 * Count the decision points in a subtree (each if/loop/case branch counts as 1). Walks into every
 * descendant, including nested lambda bodies, as a deliberate simplification: a lambda dense
 * enough to matter on its own will usually also push the enclosing function over the threshold.
 *
 * @param node Root of the subtree to walk
 * @return Number of decision points found
 */
static size_t countDecisionPoints(const ASTNode *node) { // NOLINT(misc-no-recursion)
  size_t count = 0;
  if (is<const IfStmtNode *>(node) || is<const ForLoopNode *>(node) || is<const ForeachLoopNode *>(node) ||
      is<const WhileLoopNode *>(node) || is<const DoWhileLoopNode *>(node) || is<const CaseBranchNode *>(node))
    count++;

  for (const ASTNode *child : node->getChildren())
    count += countDecisionPoints(child);

  return count;
}

/**
 * Emit a finding if the given body's cyclomatic complexity exceeds MAX_CYCLOMATIC_COMPLEXITY.
 *
 * @param body Function/procedure body (nullptr for declarations without a body)
 * @param codeLoc Location to attach the finding to
 * @param name Function/procedure name
 * @param kindLabel "Function" or "Procedure", for the finding message
 * @param ruleId Id of the rule producing the finding
 * @param findings Findings vector to append to
 */
static void checkComplexity(const StmtLstNode *body, const CodeLoc &codeLoc, const std::string &name,
                            std::string_view kindLabel, std::string_view ruleId, std::vector<LintFinding> &findings) {
  if (body == nullptr)
    return;
  const size_t complexity = 1 + countDecisionPoints(body);
  if (complexity > MAX_CYCLOMATIC_COMPLEXITY)
    findings.emplace_back(codeLoc, ruleId, LintSeverity::WARNING,
                          std::string(kindLabel) + " '" + name + "' has a cyclomatic complexity of " +
                              std::to_string(complexity) + " (max " + std::to_string(MAX_CYCLOMATIC_COMPLEXITY) +
                              "); consider splitting it up");
}

void CyclomaticComplexityRule::checkFctDef(FctDefNode *node, std::vector<LintFinding> &findings) {
  checkComplexity(node->body, node->codeLoc, node->name->name, "Function", id(), findings);
}

void CyclomaticComplexityRule::checkProcDef(ProcDefNode *node, std::vector<LintFinding> &findings) {
  checkComplexity(node->body, node->codeLoc, node->name->name, "Procedure", id(), findings);
}

} // namespace spice::compiler

// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TooManyParametersRule.h"

#include <ast/ASTNodes.h>

namespace spice::compiler {

// A caller has to hold this many arguments in mind (and their order) at every call site.
static constexpr size_t MAX_PARAM_COUNT = 6;

/**
 * Emit a finding if the given parameter list has more entries than MAX_PARAM_COUNT.
 *
 * @param paramLst Parameter list of the function/procedure (nullptr if it has none)
 * @param codeLoc Location to attach the finding to
 * @param name Function/procedure name
 * @param kindLabel "Function" or "Procedure", for the finding message
 * @param ruleId Id of the rule producing the finding
 * @param findings Findings vector to append to
 */
static void checkParamCount(const ParamLstNode *paramLst, const CodeLoc &codeLoc, const std::string &name,
                            std::string_view kindLabel, std::string_view ruleId, std::vector<LintFinding> &findings) {
  const size_t paramCount = paramLst != nullptr ? paramLst->params.size() : 0;
  if (paramCount > MAX_PARAM_COUNT)
    findings.emplace_back(codeLoc, ruleId, LintSeverity::WARNING,
                          std::string(kindLabel) + " '" + name + "' has " + std::to_string(paramCount) + " parameters (max " +
                              std::to_string(MAX_PARAM_COUNT) + "); consider grouping related parameters into a struct");
}

void TooManyParametersRule::checkFctDef(FctDefNode *node, std::vector<LintFinding> &findings) {
  checkParamCount(node->paramLst, node->codeLoc, node->name->name, "Function", id(), findings);
}

void TooManyParametersRule::checkProcDef(ProcDefNode *node, std::vector<LintFinding> &findings) {
  checkParamCount(node->paramLst, node->codeLoc, node->name->name, "Procedure", id(), findings);
}

} // namespace spice::compiler

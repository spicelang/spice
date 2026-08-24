// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "NamingConventionRule.h"

#include <cctype>

#include <ast/ASTNodes.h>

namespace spice::compiler {

/**
 * Check whether an identifier is written in camelCase (starts lowercase, no underscores)
 *
 * @param input Identifier to check
 * @return True if the identifier is camelCase
 */
static bool isCamelCase(const std::string &input) {
  return !input.empty() && std::islower(static_cast<unsigned char>(input.front())) != 0 && !input.contains('_');
}

/**
 * Check whether an identifier is written in PascalCase (starts uppercase, no underscores)
 *
 * @param input Identifier to check
 * @return True if the identifier is PascalCase
 */
static bool isPascalCase(const std::string &input) {
  return !input.empty() && std::isupper(static_cast<unsigned char>(input.front())) != 0 && !input.contains('_');
}

/**
 * Check whether an identifier is written in SNAKE_CASE (all uppercase, underscores)
 *
 * @param input Identifier to check
 * @return True if the identifier is SNAKE_CASE
 */
static bool isSnakeCase(const std::string &input) {
  return !input.empty() && std::ranges::all_of(input, [](unsigned char c) { return std::isupper(c) || c == '_'; });
}

void NamingConventionRule::checkFctDef(FctDefNode *node, std::vector<LintFinding> &findings) {
  // Operator overloads (e.g. "op.plus") intentionally do not follow camelCase
  if (node->name->isOperatorOverload())
    return;
  if (!isCamelCase(node->name->name))
    findings.emplace_back(node->name->codeLoc, id(), LintSeverity::WARNING,
                          "Function name '" + node->name->name + "' should be camelCase");
}

void NamingConventionRule::checkProcDef(ProcDefNode *node, std::vector<LintFinding> &findings) {
  // Constructors are always named "ctor" by language convention
  if (node->isCtor)
    return;
  if (!isCamelCase(node->name->name))
    findings.emplace_back(node->name->codeLoc, id(), LintSeverity::WARNING,
                          "Procedure name '" + node->name->name + "' should be camelCase");
}

void NamingConventionRule::checkStructDef(StructDefNode *node, std::vector<LintFinding> &findings) {
  if (!isPascalCase(node->structName))
    findings.emplace_back(node->codeLoc, id(), LintSeverity::WARNING,
                          "Struct name '" + node->structName + "' should be PascalCase");
}

void NamingConventionRule::checkInterfaceDef(InterfaceDefNode *node, std::vector<LintFinding> &findings) {
  if (!isPascalCase(node->interfaceName))
    findings.emplace_back(node->codeLoc, id(), LintSeverity::WARNING,
                          "Interface name '" + node->interfaceName + "' should be PascalCase");
}

void NamingConventionRule::checkGlobalVarDef(GlobalVarDefNode *node, std::vector<LintFinding> &findings) {
  if (!isSnakeCase(node->varName))
    findings.emplace_back(node->codeLoc, id(), LintSeverity::WARNING,
                          "Global variable name '" + node->varName + "' should be SNAKE_CASE");
}

} // namespace spice::compiler

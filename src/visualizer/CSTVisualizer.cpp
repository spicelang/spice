// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "CSTVisualizer.h"

#include <util/CommonUtil.h>

namespace spice::compiler {

std::string CSTVisualizer::buildRule(antlr4::ParserRuleContext *ctx) {
  std::stringstream result;

  // Prepare strings
  std::string codeLoc = tokenToCodeLoc(*ctx->start);
  std::string ruleName = ruleNames[ctx->getRuleIndex()];
  std::string nodeId = codeLoc + "_" + ruleName;

  // Build result
  result << nodeId << R"( [color="lightgreen",label=")" << ruleName << "\"];\n";

  // Link parent node with the current one
  std::string parentNodeIdBackup = parentNodeId;
  if (!parentNodeId.empty())
    result << getSpaces() << parentNodeId << " -> " << nodeId << ";\n";
  parentNodeId = nodeId; // Set parentNodeId for children

  // Visit all the children
  for (const auto &child : ctx->children) {
    result << getSpaces();

    auto token = dynamic_cast<antlr4::tree::TerminalNode *>(child);
    if (token) { // Terminal node
      std::string terminalCodeLoc = tokenToCodeLoc(*token->getSymbol());
      std::string terminalText = token->getText();
      CommonUtil::replaceAll(terminalText, "\\", "\\\\");
      CommonUtil::replaceAll(terminalText, "\"", "\\\"");
      std::string terminalName = std::string(vocabulary.getSymbolicName(token->getSymbol()->getType())) + ": " + terminalText;

      result << terminalCodeLoc << R"( [color="lightblue",label=")" << terminalName << "\"];\n";
      result << getSpaces() << nodeId << " -> " << terminalCodeLoc << "\n";
    } else { // Non-terminal node
      result << std::any_cast<std::string>(visit(child));
    }
  }

  // Restore parent node id
  parentNodeId = parentNodeIdBackup;

  return result.str();
}

std::string CSTVisualizer::getSpaces() const {
  std::string spaces;
  for (int i = 0; i < currentTabs; i++)
    spaces += " ";
  return spaces;
}

std::string CSTVisualizer::tokenToCodeLoc(const antlr4::Token &token) {
  return "L" + std::to_string(token.getLine()) + "C" + std::to_string(token.getCharPositionInLine());
}

} // namespace spice::compiler
// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "ErrorManager.h"

#include <ast/ASTNodes.h>

namespace spice::compiler {

void ErrorManager::addSoftError(const ASTNode *astNode, SemanticErrorType errorType, const std::string &message) {
  // Build error message
  const SemanticError semanticError(astNode, errorType, message);
  // Add to soft errors list
  addSoftError(astNode->codeLoc, semanticError.what());
}

void ErrorManager::addSoftError(const CodeLoc &codeLoc, const std::string &message) {
  // Avoid duplicate errors
  if (std::ranges::none_of(softErrors, [&](const SoftError &err) { return err.codeLoc == codeLoc; }))
    softErrors.emplace_back(SoftError{codeLoc, message});
}

} // namespace spice::compiler
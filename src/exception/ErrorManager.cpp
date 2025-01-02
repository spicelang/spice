// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

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
  for (const auto &[errorLoc, message] : softErrors)
    if (errorLoc == codeLoc)
      return;
  softErrors.emplace_back(SoftError{codeLoc, message});
}

} // namespace spice::compiler
// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <exception/SemanticError.h>
#include <util/CodeLoc.h>

namespace spice::compiler {

// Forward declarations
class ASTNode;

class ErrorManager {
public:
  // Structs
  struct SoftError {
    const CodeLoc codeLoc;
    std::string message;
  };

  // Constructors
  ErrorManager() = default;

  // Public methods
  void addSoftError(const ASTNode *astNode, SemanticErrorType errorType, const std::string &message);

  // Public members
  std::vector<SoftError> softErrors;

private:
  // Private methods
  void addSoftError(const CodeLoc &codeLoc, const std::string &message);
};

} // namespace spice::compiler
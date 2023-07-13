// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <string>

#include <exception/CompilerError.h>
#include <exception/LexerError.h>
#include <exception/LinkerError.h>
#include <exception/ParserError.h>
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
  void addSoftError(const ASTNode *astNode, CompilerErrorType errorType, const std::string &message);
  void addSoftError(const ASTNode *astNode, SemanticErrorType errorType, const std::string &message);
  void addSoftError(const ASTNode *astNode, LexerErrorType errorType, const std::string &message);
  void addSoftError(const ASTNode *astNode, ParserErrorType errorType, const std::string &message);
  void addSoftError(const ASTNode *astNode, LinkerErrorType errorType, const std::string &message);

  // Public members
  std::vector<SoftError> softErrors;
};

} // namespace spice::compiler
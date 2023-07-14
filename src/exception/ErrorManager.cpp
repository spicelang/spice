// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "ErrorManager.h"

#include <ast/ASTNodes.h>

namespace spice::compiler {

void ErrorManager::addSoftError(const ASTNode *astNode, CompilerErrorType errorType, const std::string &message) {
  // Build error message
  std::stringstream errorMessage;
  errorMessage << "[Error|Compiler]:\n" << CompilerError::getMessagePrefix(errorType) << ": " << message;
  // Add to soft errors list
  addSoftError(astNode->codeLoc, errorMessage.str());
}

void ErrorManager::addSoftError(const ASTNode *astNode, SemanticErrorType errorType, const std::string &message) {
  // Build error message
  std::stringstream errorMessage;
  errorMessage << "[Error|Semantic] " << astNode->codeLoc.toPrettyString() << ":\n";
  errorMessage << SemanticError::getMessagePrefix(errorType) << ": " << message;
  if (!astNode->errorMessage.empty())
    errorMessage << "\n\n" << astNode->errorMessage;
  // Add to soft errors list
  addSoftError(astNode->codeLoc, errorMessage.str());
}

void ErrorManager::addSoftError(const ASTNode *astNode, LexerErrorType errorType, const std::string &message) {
  // Build error message
  std::stringstream errorMessage;
  errorMessage << "[Error|Lexer] " << astNode->codeLoc.toPrettyString() << ": ";
  errorMessage << LexerError::getMessagePrefix(errorType) << ": " << message;
  // Add to soft errors list
  addSoftError(astNode->codeLoc, errorMessage.str());
}

void ErrorManager::addSoftError(const ASTNode *astNode, ParserErrorType errorType, const std::string &message) {
  // Build error message
  std::stringstream errorMessage;
  errorMessage << "[Error|Parser] " << astNode->codeLoc.toPrettyString() << ": ";
  errorMessage << ParserError::getMessagePrefix(errorType) << ": " << message;
  // Add to soft errors list
  addSoftError(astNode->codeLoc, errorMessage.str());
}

void ErrorManager::addSoftError(const ASTNode *astNode, LinkerErrorType errorType, const std::string &message) {
  // Build error message
  std::stringstream errorMessage;
  errorMessage << "[Error|Linker] " << LinkerError::getMessagePrefix(errorType) << ": " << message;
  // Add to soft errors list
  addSoftError(astNode->codeLoc, errorMessage.str());
}

void ErrorManager::addSoftError(const CodeLoc &codeLoc, const std::string &message) {
  // Avoid duplicate errors
  for (const SoftError &error : softErrors)
    if (error.codeLoc == codeLoc)
      return;
  softErrors.emplace_back(SoftError{codeLoc, message});
}

} // namespace spice::compiler
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ErrorFactory.h"

#include <util/CodeLoc.h>

SemanticError ErrorFactory::get(const CodeLoc &codeLoc, SemanticErrorType type, const std::string &message) {
  return SemanticError(codeLoc, type, message);
}

IRError ErrorFactory::get(const CodeLoc &codeLoc, IRErrorType type, const std::string &message) {
  return IRError(codeLoc, type, message);
}

IRError ErrorFactory::get(IRErrorType type, const std::string &message) { return IRError(type, message); }

CliError ErrorFactory::get(CliErrorType type, const std::string &message) { return CliError(type, message); }

LinkerError ErrorFactory::get(LinkerErrorType type, const std::string &message) { return LinkerError(type, message); }
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include <exception/CliError.h>
#include <exception/IRError.h>
#include <exception/LinkerError.h>
#include <exception/SemanticError.h>

// Forward declarations
struct CodeLoc;

class ErrorFactory {
public:
  // Constructors
  ErrorFactory() = default;

  // Public methods
  [[nodiscard]] static SemanticError get(const CodeLoc &codeLoc, SemanticErrorType type, const std::string &message) ;
  [[nodiscard]] static IRError get(const CodeLoc &codeLoc, IRErrorType type, const std::string &message) ;
  [[nodiscard]] static IRError get(IRErrorType type, const std::string &message) ;
  [[nodiscard]] static CliError get(CliErrorType type, const std::string &message);
  [[nodiscard]] static LinkerError get(LinkerErrorType type, const std::string &message);
};
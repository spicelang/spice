// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <global/GlobalResourceManager.h>

namespace spice::compiler {

// Forward declarations
class Scope;
enum class ScopeType : uint8_t;

class CompilerPass {
public:
  // Constructors
  explicit CompilerPass(GlobalResourceManager &resourceManager, SourceFile *sourceFile = nullptr);

  // Public methods
  void changeToScope(Scope *scope, const ScopeType &scopeType);
  void changeToScope(const std::string &scopeName, const ScopeType &scopeType);
  void changeToParentScope(ScopeType oldScopeType);

protected:
  // Protected members
  GlobalResourceManager &resourceManager;
  const CliOptions &cliOptions;
  SourceFile *sourceFile;
  Scope *rootScope;
  size_t manIdx = 0;

public:
  // Public members
  Scope *currentScope;
};

} // namespace spice::compiler
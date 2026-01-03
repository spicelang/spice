// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <string>

namespace spice::compiler {

// Forward declarations
class Scope;
class GlobalResourceManager;
class SourceFile;
struct CliOptions;
enum class ScopeType : uint8_t;

class CompilerPass {
public:
  // Constructors
  explicit CompilerPass(GlobalResourceManager &resourceManager, SourceFile *sourceFile = nullptr);
  CompilerPass(const CompilerPass &) = delete;

  // Destructor
  virtual ~CompilerPass() = default;

  // Public methods
  void changeToScope(Scope *scope, ScopeType scopeType);
  void changeToScope(const std::string &scopeName, ScopeType scopeType);
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
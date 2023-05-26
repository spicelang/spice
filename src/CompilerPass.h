// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <global/GlobalResourceManager.h>

namespace spice::compiler {

// Forward declarations
class Scope;

class CompilerPass {
public:
  // Constructors
  explicit CompilerPass(GlobalResourceManager &resourceManager, SourceFile *sourceFile = nullptr);

protected:
  // Protected members
  GlobalResourceManager &resourceManager;
  const CliOptions &cliOptions;
  SourceFile *sourceFile;
  Scope *rootScope;
};

} // namespace spice::compiler
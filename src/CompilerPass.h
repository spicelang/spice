// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <global/GlobalResourceManager.h>

// Forward declarations
class Scope;

class CompilerPass {
public:
  // Constructors
  CompilerPass(GlobalResourceManager &resourceManager, SourceFile *sourceFile);

protected:
  // Protected members
  GlobalResourceManager &resourceManager;
  const CliOptions &cliOptions;
  SourceFile *sourceFile;
  Scope *rootScope;
};
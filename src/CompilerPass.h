// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <SourceFile.h>
#include <global/GlobalResourceManager.h>

class CompilerPass {
public:
  // Constructors
  CompilerPass(GlobalResourceManager &resourceManager, SourceFile *sourceFile)
      : resourceManager(resourceManager), cliOptions(resourceManager.cliOptions), sourceFile(sourceFile),
        rootScope(sourceFile->globalScope.get()){};

protected:
  // Protected members
  GlobalResourceManager &resourceManager;
  const CliOptions &cliOptions;
  SourceFile *sourceFile;
  Scope *rootScope;
};

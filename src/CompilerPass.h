// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <SourceFile.h>
#include <ast/AstVisitor.h>
#include <global/GlobalResourceManager.h>

class CompilerPass : public AstVisitor {
public:
  // Constructors
  CompilerPass(GlobalResourceManager &resourceManager, SourceFile &sourceFile)
      : resourceManager(resourceManager), sourceFile(sourceFile){};

protected:
  // Protected members
  GlobalResourceManager &resourceManager;
  SourceFile &sourceFile;
};

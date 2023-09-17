// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <SourceFile.h>
#include <ast/ASTVisitor.h>

namespace spice::compiler {

class ObjectEmitter : private CompilerPass {
public:
  // Constructors
  ObjectEmitter(GlobalResourceManager &resourceManager, SourceFile *sourceFile) : CompilerPass(resourceManager, sourceFile) {}

  // Public methods
  void emit() const;
  void getASMString(std::string &output) const;

private:
  // Private members
  llvm::Module &module = resourceManager.cliOptions.useLTO ? *resourceManager.ltoModule : *sourceFile->llvmModule;
};

} // namespace spice::compiler
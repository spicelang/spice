// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <SourceFile.h>
#include <ast/ASTVisitor.h>

namespace spice::compiler {

// Forward declarations
class GlobalResourceManager;

class ObjectEmitter : private CompilerPass {
public:
  // Constructors
  ObjectEmitter(GlobalResourceManager &resourceManager, SourceFile *sourceFile);

  // Public methods
  void emit() const;
  void getASMString(std::string &output) const;

private:
  // Private members
  llvm::Module &module;
};

} // namespace spice::compiler
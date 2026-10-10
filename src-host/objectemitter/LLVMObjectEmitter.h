// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <filesystem>

#include <CompilerPass.h>
#include <objectemitter/AbstractObjectEmitter.h>

// Forward declarations
namespace llvm {
  class Module;
} // namespace llvm

namespace spice::compiler {

// Forward declarations
class GlobalResourceManager;
class SourceFile;

/**
 * Object emitter backed by LLVM's own CodeGen pipeline (the default and stable path). Feeds the
 * module through llvm::TargetMachine::addPassesToEmitFile to produce an object or assembly file.
 */
class LLVMObjectEmitter : public AbstractObjectEmitter, CompilerPass {
public:
  // Constructors
  LLVMObjectEmitter(GlobalResourceManager &resourceManager, SourceFile *sourceFile);

  // Public methods
  void emit(const std::filesystem::path &objectPath) const override;
  void getASMString(std::string &output) const override;

private:
  // Private members
  llvm::Module &module;
};

} // namespace spice::compiler

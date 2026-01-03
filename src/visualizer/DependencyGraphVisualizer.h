// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <unordered_set>

#include <CompilerPass.h>

namespace spice::compiler {

// Forward declarations
class SourceFile;

class DependencyGraphVisualizer final : CompilerPass {
public:
  // Constructors
  using CompilerPass::CompilerPass;

  // Public methods
  void getDependencyGraph(std::stringstream &output);

private:
  // Private members
  std::unordered_set<const SourceFile *> printedFiles;

  // Private methods
  void getDependencyGraphNode(std::stringstream &output, const SourceFile *currentSourceFile);
};

} // namespace spice::compiler

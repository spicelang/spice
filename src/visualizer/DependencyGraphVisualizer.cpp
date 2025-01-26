// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "DependencyGraphVisualizer.h"

#include <cassert>

#include <SourceFile.h>

namespace spice::compiler {

void DependencyGraphVisualizer::getDependencyGraph(std::stringstream& output) {
  assert(sourceFile->isMainFile);
  printedFiles.clear();
  getDependencyGraphNode(output, sourceFile);
}

void DependencyGraphVisualizer::getDependencyGraphNode(std::stringstream& output, const SourceFile *currentSourceFile) {
  // Skip if already printed
  if (printedFiles.contains(currentSourceFile))
    return;

  // Append code for this source file
  const std::filesystem::path relativePath = relative(currentSourceFile->filePath, sourceFile->filePath).generic_string();
  output << " \"" << relativePath.c_str() << "\" [color=\"lightgreen\",label=\"" << currentSourceFile->fileName << "\"];\n";
  printedFiles.insert(currentSourceFile);

  // Append code for dependencies
  for (const SourceFile *dependency : currentSourceFile->dependencies | std::views::values) {
    getDependencyGraphNode(output, dependency);
    const std::filesystem::path dependencyPathRelative = relative(dependency->filePath, sourceFile->filePath).generic_string();
    output << " \"" << relativePath.string() << "\" -> \"" << dependencyPathRelative.c_str() << "\";\n";
  }
}

} // namespace spice::compiler

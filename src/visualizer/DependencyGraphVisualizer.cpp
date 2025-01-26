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
  const std::filesystem::path filePath = relative(currentSourceFile->filePath, sourceFile->filePath).generic_string();
  output << " \"" << filePath.string() << "\" [color=\"lightgreen\",label=\"" << currentSourceFile->fileName << "\"];\n";
  printedFiles.insert(currentSourceFile);

  // Append code for dependencies
  for (const SourceFile *dependency : currentSourceFile->dependencies | std::views::values) {
    getDependencyGraphNode(output, dependency);
    output << " \"" << filePath.string() << "\" -> \"" << dependency->filePath.c_str() << "\";\n";
  }
}

} // namespace spice::compiler

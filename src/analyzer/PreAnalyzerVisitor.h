// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <ast/AstVisitor.h>

// Forward declarations
struct CliOptions;
class SourceFile;

/**
 * Visitor for pre-analyzing all source files in hierarchical order from bottom to top.
 *
 * Jobs:
 * - Collect imports
 */
class PreAnalyzerVisitor : public AstVisitor {
public:
  // Constructors
  explicit PreAnalyzerVisitor(const CliOptions &options, SourceFile &sourceFile) : cliOptions(options), sourceFile(sourceFile) {}

  // Public methods
  std::any visitEntry(EntryNode *node) override;
  std::any visitImportStmt(ImportStmtNode *node) override;

private:
  // Members
  const CliOptions &cliOptions;
  SourceFile &sourceFile;
};
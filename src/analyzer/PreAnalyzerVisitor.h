// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <ast/AstVisitor.h>

#include <exception/ErrorFactory.h>

// Forward declarations
struct CliOptions;
class SourceFile;
class ErrorFactory;

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
  ErrorFactory err = ErrorFactory();
  SourceFile &sourceFile;
};
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <SpiceBaseVisitor.h>

#include <cli/CliInterface.h>
#include <dependency/SourceFile.h>
#include <exception/ErrorFactory.h>

/**
 * Visitor for pre-analyzing all source files in hierarchical order from bottom to top.
 *
 * Jobs:
 * - Collect imports
 */
class PreAnalyzerVisitor : public SpiceBaseVisitor {
public:
  // Constructors
  explicit PreAnalyzerVisitor(const CliOptions &options, SourceFile &sourceFile);

  // Public methods
  std::any visitImportStmt(SpiceParser::ImportStmtContext *ctx) override;

private:
  // Members
  const CliOptions &cliOptions;
  ErrorFactory err;
  SourceFile &sourceFile;
};
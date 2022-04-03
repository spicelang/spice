// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <SpiceBaseVisitor.h>

#include <cli/CliInterface.h>
#include <dependency/SourceFile.h>
#include <exception/ErrorFactory.h>

class PreAnalyzerVisitor : public SpiceBaseVisitor {
public:
  explicit PreAnalyzerVisitor(CliOptions *options, SourceFile *sourceFile);

  // Public methods
  antlrcpp::Any visitImportStmt(SpiceParser::ImportStmtContext *ctx) override;

private:
  CliOptions *cliOptions;
  ErrorFactory err;
  SourceFile *sourceFile;
};
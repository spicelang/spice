// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>
#include <global/GlobalResourceManager.h>

/**
 * Jobs:
 * - Visit the import statements of a source file and register the imported files as dependencies to the current one
 */
class ImportCollector : private CompilerPass, public ASTVisitor {
public:
  // Constructors
  ImportCollector(GlobalResourceManager &resourceManager, SourceFile *sourcefile)
      : CompilerPass(resourceManager, sourcefile), cliOptions(resourceManager.cliOptions) {}

  // Public methods
  std::any visitEntry(EntryNode *node) override;
  std::any visitImportStmt(ImportStmtNode *node) override;

private:
  // Private members
  const CliOptions &cliOptions;
};
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>

/**
 * Jobs:
 * - Visit the import statements of a source file and register the imported files as dependencies to the current one
 * - Load the exposed symbols of the imported file and register them in the name registry of the current source file
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

  // Private methods
  void registerExternalGlobalNames(SourceFile *importedSourceFile, const std::string &importName);
};

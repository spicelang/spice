// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>

namespace spice::compiler {

// Forward declarations
class GlobalResourceManager;

/**
 * Jobs:
 * - Visit the import statements of a source file and register the imported files as dependencies to the current one
 */
class ImportCollector final : CompilerPass, public ASTVisitor {
public:
  // Constructors
  ImportCollector(GlobalResourceManager &resourceManager, SourceFile *sourcefile) : CompilerPass(resourceManager, sourcefile) {}

  // Public methods
  std::any visitEntry(EntryNode *node) override;
  std::any visitImportDef(ImportDefNode *node) override;
  std::any visitModAttr(ModAttrNode *node) override;
};

} // namespace spice::compiler
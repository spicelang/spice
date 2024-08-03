// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "ImportCollector.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <ast/Attributes.h>
#include <exception/SemanticError.h>
#include <util/FileUtil.h>

namespace spice::compiler {

std::any ImportCollector::visitEntry(EntryNode *node) {
  // Visit all module attributes
  for (ModAttrNode *attr : node->modAttrs())
    visit(attr);

  // Visit all import defs
  for (ImportDefNode *importDef : node->importDefs())
    visit(importDef);

  return nullptr;
}

std::any ImportCollector::visitImportDef(ImportDefNode *node) {
  const bool isStd = node->importPath.starts_with("std/");
  const bool isBootstrap = node->importPath.starts_with("bootstrap/");

  std::filesystem::path basePath;
  if (isStd) { // Include source file from standard library
    // Find std library
    const std::filesystem::path stdPath = FileUtil::getStdDir();
    if (stdPath.empty())
      throw CompilerError(STD_NOT_FOUND, "Standard library could not be found. Check if the env var SPICE_STD_DIR exists");
    // Format: /dir/to/path/file
    basePath = stdPath / node->importPath.substr(node->importPath.find("std/") + 4);
  } else if (isBootstrap) { // Include source file from bootstrap library
    // Find bootstrap library
    const std::filesystem::path bootstrapPath = FileUtil::getBootstrapDir();
    if (bootstrapPath.empty())
      throw CompilerError(BOOTSTRAP_NOT_FOUND,
                          "Bootstrap compiler could not be found. Check if the env var SPICE_BOOTSTRAP_DIR exists");
    // Format: /dir/to/path/file
    basePath = bootstrapPath / node->importPath.substr(node->importPath.find("bootstrap/") + 10);
  } else { // Include own source file
    // Format: /dir/to/path/file
    basePath = sourceFile->filePath.parent_path() / node->importPath;
  }
  basePath.make_preferred();

  // Format: /dir/to/path/file.spice
  std::filesystem::path defaultPath = basePath;
  defaultPath.replace_filename(basePath.stem().string() + ".spice");
  // Format: /dir/to/path/file_linux.spice
  std::filesystem::path osPath = basePath;
  osPath.replace_filename(basePath.stem().string() + "_" + cliOptions.targetOs + ".spice");
  // Format: /dir/to/path/file_linux_x86_64.spice
  std::filesystem::path osArchPath = basePath;
  osArchPath.replace_filename(basePath.stem().string() + "_" + cliOptions.targetOs + "_" + cliOptions.targetArch + ".spice");

  // Check which source file to import
  std::filesystem::path importPath;
  if (std::filesystem::exists(osArchPath)) // file_os_arch.spice is first choice
    importPath = osArchPath;
  else if (std::filesystem::exists(osPath)) // file_os.spice is second choice
    importPath = osPath;
  else if (std::filesystem::exists(defaultPath)) // file.spice is third choice
    importPath = defaultPath;
  else
    throw SemanticError(node, IMPORTED_FILE_NOT_EXISTING, "The source file '" + node->importPath + ".spice' does not exist");

  // Check if the import already exists
  SymbolTableEntry *importEntry = rootScope->lookupStrict(node->importName);
  if (importEntry != nullptr)
    throw SemanticError(node, DUPLICATE_IMPORT_NAME, "Duplicate import '" + node->importName + "'");

  // Create symbol for import
  node->entry = rootScope->insert(node->importName, node);

  // Create the imported source file
  const auto importedSourceFile = resourceManager.createSourceFile(sourceFile, node->importName, importPath, isStd);
  // Register it as a dependency to the current source file
  sourceFile->addDependency(importedSourceFile, node, node->importName, importPath.generic_string());

  return nullptr;
}

std::any ImportCollector::visitModAttr(ModAttrNode *node) {
  // Retrieve attributes
  const AttrLstNode *attrs = node->attrLst();

  // !!! Only bool attributes allowed here, due to missing attribute value checks being executed in a later stage !!!

  // core.compiler.keep-on-name-collision
  if (attrs->hasAttr(ATTR_CORE_COMPILER_KEEP_ON_NAME_COLLISION)) {
    const bool keepOnCollision = attrs->getAttrValueByName(ATTR_CORE_COMPILER_KEEP_ON_NAME_COLLISION)->boolValue;
    sourceFile->alwaysKeepSymbolsOnNameCollision = keepOnCollision;
  }

  // core.compiler.warnings.ignore
  if (attrs->hasAttr(ATTR_CORE_COMPILER_WARNINGS_IGNORE)) {
    const bool ignoreWarnings = attrs->getAttrValueByName(ATTR_CORE_COMPILER_WARNINGS_IGNORE)->boolValue;
    sourceFile->ignoreWarnings = ignoreWarnings;
  }

  return nullptr;
}

} // namespace spice::compiler
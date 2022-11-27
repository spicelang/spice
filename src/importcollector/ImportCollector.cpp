// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ImportCollector.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <exception/SemanticError.h>
#include <util/CommonUtil.h>
#include <util/FileUtil.h>

std::any ImportCollector::visitEntry(EntryNode *node) {
  // Visit all import statements
  for (ImportStmtNode *importStmt : node->importStmts())
    visit(importStmt);

  // Reset the AST
  node->reset();

  return nullptr;
}

std::any ImportCollector::visitImportStmt(ImportStmtNode *node) {
  std::string importPath = node->importPath;
  bool isStd = importPath.rfind("std/", 0) == 0;

  std::string basePath;
  if (isStd) { // Include source file from standard library
    // Find std library
    std::string stdPath = FileUtil::getStdDir();
    if (stdPath.empty())
      throw SemanticError(node, STD_NOT_FOUND, "Standard library could not be found. Check if the env var SPICE_STD_DIR exists");
    // Format: /dir/to/path/file
    basePath = stdPath + importPath.substr(importPath.find("std/") + 4);
  } else { // Include own source file
    // Format: /dir/to/path/file
    basePath = FileUtil::getFileDir(sourceFile->filePath) + FileUtil::DIR_SEPARATOR + importPath;
  }

  // Format: /dir/to/path/file.spice
  const std::string defaultPath = basePath + ".spice";
  // Format: /dir/to/path/file_linux.spice
  const std::string osPath = basePath + "_" + cliOptions.targetOs + ".spice";
  // Format: /dir/to/path/file_linux_x86_64.spice
  const std::string osArchPath = basePath + "_" + cliOptions.targetOs + "_" + cliOptions.targetArch + ".spice";

  // Check which source file to import
  if (FileUtil::fileExists(osArchPath)) // file_os_arch.spice is third choice
    importPath = osArchPath;
  else if (FileUtil::fileExists(osPath)) // file_os.spice is second choice
    importPath = osPath;
  else if (FileUtil::fileExists(defaultPath)) // file.spice is first choice
    importPath = defaultPath;
  else
    throw SemanticError(node, IMPORTED_FILE_NOT_EXISTING, "The source file '" + importPath + ".spice' does not exist");

  // Use the correct dir separator for the current OS
  CommonUtil::replaceAll(importPath, "/", std::string(1, FileUtil::DIR_SEPARATOR));

  // Create the imported source file
  const auto importedSourceFile = sourceFile->createSourceFile(node->importName, importPath, isStd);
  // Register it as a dependency to the current source file
  sourceFile->addDependency(importedSourceFile, node, node->importName, importPath);

  return nullptr;
}
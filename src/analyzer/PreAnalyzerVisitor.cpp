// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "PreAnalyzerVisitor.h"

#include <ast/AstNodes.h>
#include <cli/CliInterface.h>
#include <dependency/SourceFile.h>
#include <exception/AntlrThrowingErrorListener.h>
#include <exception/SemanticError.h>
#include <util/CommonUtil.h>
#include <util/FileUtil.h>

std::any PreAnalyzerVisitor::visitEntry(EntryNode *node) {
  // Visit all import statements
  for (const auto &importStmt : node->importStmts())
    visit(importStmt);

  // Reset the AST
  node->reset();

  return nullptr;
}

std::any PreAnalyzerVisitor::visitImportStmt(ImportStmtNode *node) {
  // Get the file path from the identifier
  bool isImportStd = node->importPath.rfind("std/", 0) == 0;
  std::string importPath;
  if (node->importPath.rfind("std/", 0) == 0) { // Include source file from standard library
    std::string sourceFileIden = node->importPath.substr(node->importPath.find("std/") + 4);
    // Find std library
    std::string stdPath;
    if (FileUtil::fileExists("/usr/lib/spice/std")) {
      stdPath = "/usr/lib/spice/std/";
    } else if (std::getenv("SPICE_STD_DIR") && FileUtil::dirExists(std::string(std::getenv("SPICE_STD_DIR")))) {
      stdPath = std::string(std::getenv("SPICE_STD_DIR"));
      if (stdPath.rfind(FileUtil::DIR_SEPARATOR) != stdPath.size() - 1)
        stdPath += FileUtil::DIR_SEPARATOR;
    } else {
      throw SemanticError(node->codeLoc, STD_NOT_FOUND,
                          "Standard library could not be found. Check if the env var SPICE_STD_DIR exists");
    }
    // Check if source file exists
    std::string defaultPath = stdPath + sourceFileIden + ".spice";
    std::string osPath = stdPath + sourceFileIden + "_" + cliOptions.targetOs + ".spice";
    std::string osArchPath = stdPath + sourceFileIden + "_" + cliOptions.targetOs + "_" + cliOptions.targetArch + ".spice";

    if (FileUtil::fileExists(defaultPath)) {
      importPath = defaultPath;
    } else if (FileUtil::fileExists(osPath)) {
      importPath = osPath;
    } else if (FileUtil::fileExists(osArchPath)) {
      importPath = osArchPath;
    } else {
      throw SemanticError(node->codeLoc, IMPORTED_FILE_NOT_EXISTING,
                          "The source file '" + node->importPath + ".spice' was not found in the standard library");
    }
  } else { // Include own source file
    // Check in module registry if the file can be imported
    std::string sourceFileDir = FileUtil::getFileDir(sourceFile.filePath);
    // Import file
    std::string defaultPath = sourceFileDir + FileUtil::DIR_SEPARATOR + node->importPath + ".spice";
    std::string osPath = sourceFileDir + FileUtil::DIR_SEPARATOR + node->importPath + "_" + cliOptions.targetOs + ".spice";
    std::string osArchPath = sourceFileDir + FileUtil::DIR_SEPARATOR + node->importPath + "_" + cliOptions.targetOs + "_" +
                             cliOptions.targetArch + ".spice";

    if (FileUtil::fileExists(defaultPath)) {
      importPath = defaultPath;
    } else if (FileUtil::fileExists(osPath)) {
      importPath = osPath;
    } else if (FileUtil::fileExists(osArchPath)) {
      importPath = osArchPath;
    } else {
      throw SemanticError(node->codeLoc, IMPORTED_FILE_NOT_EXISTING,
                          "The source file '" + node->importPath + ".spice' does not exist");
    }
  }
  CommonUtil::replaceAll(importPath, "/", std::string(1, FileUtil::DIR_SEPARATOR));

  // Visit the imported file
  sourceFile.addDependency(node, node->importName, importPath, isImportStd);

  return nullptr;
}
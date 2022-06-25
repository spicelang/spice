// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "PreAnalyzerVisitor.h"

#include <SpiceLexer.h>
#include <exception/AntlrThrowingErrorListener.h>

#include <util/FileUtil.h>

PreAnalyzerVisitor::PreAnalyzerVisitor(const CliOptions &options, SourceFile &sourceFile)
    : cliOptions(options), sourceFile(sourceFile) {
  // Create error factory
  this->err = ErrorFactory(sourceFile.filePath);
}

std::any PreAnalyzerVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {
  // Get import name and identifier
  std::string importName = ctx->IDENTIFIER()->toString();
  std::string importIdentifier = ctx->STRING_LITERAL()->toString();
  importIdentifier = importIdentifier.substr(1, importIdentifier.size() - 2);

  // Get the file path from the identifier
  bool isImportStd = importIdentifier.rfind("std/", 0) == 0;
  std::string importPath;
  if (importIdentifier.rfind("std/", 0) == 0) { // Include source file from standard library
    std::string sourceFileIden = importIdentifier.substr(importIdentifier.find("std/") + 4);
    // Find std library
    std::string stdPath;
    if (FileUtil::fileExists("/usr/lib/spice/std")) {
      stdPath = "/usr/lib/spice/std/";
    } else if (FileUtil::dirExists(std::string(std::getenv("SPICE_STD_DIR")))) {
      stdPath = std::string(std::getenv("SPICE_STD_DIR"));
      if (stdPath.rfind('/') != stdPath.size() - 1)
        stdPath += "/";
    } else {
      throw err.get(*ctx->STRING_LITERAL()->getSymbol(), STD_NOT_FOUND,
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
      throw err.get(*ctx->STRING_LITERAL()->getSymbol(), IMPORTED_FILE_NOT_EXISTING,
                    "The source file '" + importIdentifier + ".spice' was not found in the standard library");
    }
  } else { // Include own source file
    // Check in module registry if the file can be imported
    std::string sourceFileDir = FileUtil::getFileDir(sourceFile.filePath);
    // Import file
    std::string defaultPath = sourceFileDir + "/" + importIdentifier + ".spice";
    std::string osPath = sourceFileDir + "/" + importIdentifier + "_" + cliOptions.targetOs + ".spice";
    std::string osArchPath =
        sourceFileDir + "/" + importIdentifier + "_" + cliOptions.targetOs + "_" + cliOptions.targetArch + ".spice";

    if (FileUtil::fileExists(defaultPath)) {
      importPath = defaultPath;
    } else if (FileUtil::fileExists(osPath)) {
      importPath = osPath;
    } else if (FileUtil::fileExists(osArchPath)) {
      importPath = osArchPath;
    } else {
      throw err.get(*ctx->STRING_LITERAL()->getSymbol(), IMPORTED_FILE_NOT_EXISTING,
                    "The source file '" + importIdentifier + ".spice' does not exist");
    }
  }

  // Visit the imported file
  sourceFile.addDependency(&err, *ctx->STRING_LITERAL()->getSymbol(), importName, importPath, isImportStd);

  return nullptr;
}
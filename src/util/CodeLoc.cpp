// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "CodeLoc.h"

#include <filesystem>
#include <string>

#include <SourceFile.h>

namespace spice::compiler {

/**
 * Returns the code location as a string for using it as a map key or similar
 *
 * @return Code location string
 */
std::string CodeLoc::toString() const { return "L" + std::to_string(line) + "C" + std::to_string(col); }

/**
 * Returns the code location in a pretty form
 *
 * @return Pretty code location
 */
std::string CodeLoc::toPrettyString() const {
  const std::filesystem::path &rootSourceFilePath = sourceFile->getRootSourceFile()->filePath;
  std::filesystem::path sourceFilePath = relative(sourceFile->filePath, rootSourceFilePath);
  if (sourceFilePath == ".")
    sourceFilePath /= sourceFile->fileName;
  const std::string prefix = sourceFilePath.empty() ? "" : sourceFilePath.generic_string() + ":";
  return prefix + std::to_string(line) + ":" + std::to_string(col);
}

/**
 * Returns the line number in a pretty form
 *
 * @return Pretty line number
 */
std::string CodeLoc::toPrettyLine() const { return "L" + std::to_string(line); }

/**
 * Returns the line and column numbers in a pretty form
 *
 * @return Pretty line and column numbers
 */
std::string CodeLoc::toPrettyLineAndColumn() const { return toString(); }

bool operator==(const CodeLoc &a, const CodeLoc &b) {
  return a.sourceFile->filePath == b.sourceFile->filePath && a.line == b.line && a.col == b.col;
}

} // namespace spice::compiler
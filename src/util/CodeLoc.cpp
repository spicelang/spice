// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CodeLoc.h"

#include <util/CommonUtil.h>

#include <utility>

CodeLoc::CodeLoc(size_t line, size_t col, std::string sourceFilePath)
    : line(line), col(col), sourceFilePath(std::move(sourceFilePath)) {
  CommonUtil::replaceAll(this->sourceFilePath, "\\", "/");
}

CodeLoc::CodeLoc(const antlr4::Token *token, std::string sourceFilePath)
    : line(token->getLine()), col(token->getCharPositionInLine() + 1), sourceFilePath(std::move(sourceFilePath)) {
  CommonUtil::replaceAll(this->sourceFilePath, "\\", "/");
}

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
  return (sourceFilePath.empty() ? "" : sourceFilePath + ":") + std::to_string(line) + ":" + std::to_string(col);
}

/**
 * Returns the line number in a pretty form
 *
 * @return Pretty line number
 */
std::string CodeLoc::toPrettyLine() const { return "L" + std::to_string(line); }
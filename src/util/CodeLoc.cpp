// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CodeLoc.h"

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
  if (sourceFilePath.empty())
    return std::to_string(line) + ":" + std::to_string(col);
  return sourceFilePath + ":" + std::to_string(line) + ":" + std::to_string(col);
}

/**
 * Returns the line number in a pretty form
 *
 * @return Pretty line number
 */
std::string CodeLoc::toPrettyLine() const { return "l" + std::to_string(line); }
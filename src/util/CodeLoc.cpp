// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "CodeLoc.h"

#include <util/CommonUtil.h>

#include <utility>

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
  return (sourceFilePath.empty() ? "" : sourceFilePath.generic_string() + ":") + std::to_string(line) + ":" + std::to_string(col);
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

} // namespace spice::compiler
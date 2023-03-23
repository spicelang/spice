// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <queue>
#include <string>

#include <Token.h>

namespace spice::compiler {

/**
 * Util for general simplification of tasks
 */
class CommonUtil {
public:
  static void replaceAll(std::string &haystack, const std::string &needle, const std::string &replacement);
  static std::string getLastFragment(const std::string &haystack, const std::string &needle);
};

} // namespace spice::compiler
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include "Token.h"
#include <string>

/**
 * Util for general simplification of tasks
 */
class CommonUtil {
public:
  static void replaceAll(std::string &haystack, const std::string &needle, const std::string &replacement);
  static std::string tokenToCodeLoc(const antlr4::Token &token);
};
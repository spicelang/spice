// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CommonUtil.h"

/**
 * Search all occurrences of needle in haystack and replace them with the replacement
 *
 * @param haystack Input string
 * @param needle String to search
 * @param replacement String to replace
 */
void CommonUtil::replaceAll(std::string &haystack, const std::string &needle, const std::string &replacement) {
  size_t start_pos = 0;
  while ((start_pos = haystack.find(needle, start_pos)) != std::string::npos) {
    haystack.replace(start_pos, needle.length(), replacement);
    start_pos += replacement.length();
  }
}

/**
 * Generate a code loc string with for a token
 *
 * @param token Token
 * @return Code loc string
 */
std::string CommonUtil::tokenToCodeLoc(const antlr4::Token &token) {
  return "L" + std::to_string(token.getLine()) + "C" + std::to_string(token.getCharPositionInLine());
}
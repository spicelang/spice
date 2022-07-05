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
 * Split the given haystack by the needle and return the last fragment
 *
 * @param haystack Input string
 * @param needle String to search
 * @return Last fragment
 */
std::string CommonUtil::getLastFragment(const std::string &haystack, const std::string &needle) {
  return haystack.substr(haystack.find(needle) + needle.length());
}

/**
 * Get scope prefix from scope name
 *
 * @param haystack Scope name
 * @return Scope prefix
 */
std::string CommonUtil::getPrefix(const std::string &haystack, const std::string &needle) {
  return haystack.substr(0, haystack.find(needle) + needle.length());
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
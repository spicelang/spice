// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "CommonUtil.h"

namespace spice::compiler {

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
  const size_t index = haystack.rfind(needle);
  return index == std::string::npos ? haystack : haystack.substr(index + needle.length());
}

/**
 * Trim the given input string
 *
 * @param input Input string
 * @return Trimmed string
 */
std::string CommonUtil::trim(const std::string &input) {
  size_t first = input.find_first_not_of(' ');
  if (std::string::npos == first)
    return input;
  size_t last = input.find_last_not_of(' ');
  return input.substr(first, (last - first + 1));
}

/**
 * Split the given input string by spaces
 *
 * @param input Input string
 * @return Vector of fragments
 */
std::vector<std::string> CommonUtil::split(const std::string &input) {
  std::vector<std::string> result;
  std::istringstream stream(input);

  std::string token;
  while (std::getline(stream, token, ' '))
    result.push_back(trim(token));

  return result;
}

} // namespace spice::compiler
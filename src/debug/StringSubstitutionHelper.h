// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <map>
#include <string>

const char *PLACEHOLDER_START = "<<";
const char *PLACEHOLDER_END = ">>";

/**
 * Helper for string substitution
 */
class StringSubstitutionHelper {
public:
  // Public methods
  void set(const std::string &key, const std::string &value);
  std::string getResult(const std::string &stringTemplate);

private:
  // Members
  std::map<const std::string, const std::string> fields;
};
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "StringSubstitutionHelper.h"

#include <stdexcept>

void StringSubstitutionHelper::set(const std::string &key, const std::string &value) { fields.insert({key, value}); }

std::string StringSubstitutionHelper::getResult(const std::string &stringTemplate) {
  std::string result;
  size_t curIndex = 0, oldIndex = 0;

  // Substitute all the placeholders
  while ((curIndex = stringTemplate.find(PLACEHOLDER_START, curIndex)) != std::string::npos) {
    curIndex += 2; // Consume placeholder start sequence

    // Search for the placeholder end sequence
    size_t placeholderEndIndex = stringTemplate.find(PLACEHOLDER_END, curIndex);

    // Error if no end sequence was found
    if (placeholderEndIndex == std::string::npos)
      throw std::runtime_error("Template placeholder at index " + std::to_string(curIndex) + " is not complete");

    // Get the required placeholder key
    std::string placeholderKey = stringTemplate.substr(curIndex, placeholderEndIndex - curIndex);

    if (!fields.contains(placeholderKey))
      throw std::runtime_error("Template placeholder key '" + placeholderKey + "' could not be found");

    result += stringTemplate.substr(oldIndex, curIndex - oldIndex); // Concat part before the placeholder to the result
    result += fields.at(placeholderKey);                            // Concatenate placeholder replacement

    curIndex += placeholderEndIndex + 2; // Consume placeholder end sequence
    oldIndex = curIndex;
  }

  // Clear fields
  fields.clear();

  return result;
}
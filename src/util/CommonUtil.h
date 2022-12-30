// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <queue>
#include <string>

#include <Token.h>

/**
 * Util for general simplification of tasks
 */
class CommonUtil {
public:
  static void replaceAll(std::string &haystack, const std::string &needle, const std::string &replacement);
  static std::string getLastFragment(const std::string &haystack, const std::string &needle);
  static std::string getPrefix(const std::string &haystack, const std::string &needle);
  template <typename T> static void reverseQueue(std::queue<T> &queue) {
    std::stack<T> tempStack;
    while (!queue.empty()) {
      tempStack.push(queue.front());
      queue.pop();
    }
    while (!tempStack.empty()) {
      queue.push(tempStack.top());
      tempStack.pop();
    }
  }
};
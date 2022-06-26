// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

// GCOV_EXCL_START

#pragma once

#include <string>
#include <vector>

extern bool updateRefs;

class TestUtil {
public:
  static std::vector<std::string> getSubdirs(const std::string &basePath);
  static std::string getFileContent(const std::string &filePath);
  static std::vector<std::string> getFileContentLinesVector(const std::string &filePath);
  static std::string toCamelCase(std::string input);
  static std::string getDefaultExecutableName();
  static bool isUpdateRefsEnabled();
};

// GCOV_EXCL_STOP
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

// GCOV_EXCL_START

#pragma once

#include <string>
#include <vector>

const unsigned int IR_FILE_SKIP_LINES = 4;
extern bool updateRefs;

struct TestCase {
  const std::string testName;
  const std::string testPath;
};

typedef std::vector<TestCase> TestSuite;

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
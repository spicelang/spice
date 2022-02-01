// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <sstream>

class TestUtil {
public:
    static bool fileExists(const std::string& filePath);
    static std::vector<std::string> getSubdirs(const std::string& basePath);
    static std::string getFileContent(const std::string& filePath);
    static std::string toCamelCase(std::string input);
    //template <typename T1, typename T2> static std::vector<T1> detectTestSuites(const std::string& testFilesPath);
    //template<typename T> static std::vector<T> detectTestCases(const std::string& suitePath);
};
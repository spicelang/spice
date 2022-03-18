// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

/**
 * Util class for file-related work
 */
class FileUtil {
public:
    static bool fileExists(const std::string& filePath);
    static bool dirExists(const std::string& dirPath);
    static bool createDirs(const std::string& dirPath);
    static std::string getFileName(const std::string& filePath);
    static std::string getFileDir(const std::string& filePath);
    static std::string exec(const std::string& cmd);
    static char getDirSeparator();
};
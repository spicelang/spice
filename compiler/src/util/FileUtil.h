// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Util class for file-related work
 */
class FileUtil {
public:
    static bool fileExists(const std::string&);
    static bool dirExists(const std::string&);
    static std::string getFileName(const std::string&);
    static std::string getFileDir(const std::string&);
};

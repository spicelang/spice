// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "FileUtil.h"

/**
 * Checks if a certain file exists on the file system
 *
 * @param filePath Path to the file
 * @return Existence of the file
 */
bool FileUtil::fileExists(const std::string& filePath) {
    std::ifstream file(filePath.c_str());
    if (file) return true;
    return false;
}

/**
 * Extracts the name of a file from its full path and returns it
 *
 * @param filePath Full path to the file (absolute or relative)
 */
std::string FileUtil::getFileName(const std::string& filePath) {
    char sep = '/';
#ifdef _WIN32
    sep = '\\';
#endif
    size_t i = filePath.rfind(sep, filePath.length());
    if (i != std::string::npos) return(filePath.substr(i + 1, filePath.length() - i));
    return "";
}
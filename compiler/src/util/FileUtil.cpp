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
 * @return Name of the file
 */
std::string FileUtil::getFileName(const std::string& filePath) {
    return filePath.substr(filePath.find_last_of("/\\") + 1);
}

/**
 * Extracts the directory of a file from its full path and returns it
 *
 * @param filePath Full path to the file (absolute or relative)
 * @return Path of dir
 */
std::string FileUtil::getFileDir(const std::string& filePath) {
    return filePath.substr(0, filePath.find_last_of("/\\"));
}

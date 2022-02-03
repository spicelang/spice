// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "FileUtil.h"

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Checks if a certain file exists on the file system
 *
 * @param filePath Path to the file
 * @return Existence of the file
 */
bool FileUtil::fileExists(const std::string& filePath) {
    return std::ifstream(filePath.c_str()).good();
}

/**
 * Checks if a certain dir exists on the file system
 *
 * @param dirPath Path to the dir
 * @return Existence of the dir
 */
bool FileUtil::dirExists(const std::string& dirPath) {
    struct stat info;
    if (stat(dirPath.c_str(), &info) != 0)
        return false;
    else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows 
        return true;
    return false;
    //return is_directory(std::filesystem::status(dirPath));
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

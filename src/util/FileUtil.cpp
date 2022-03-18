// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "FileUtil.h"

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <memory>
#include <filesystem>

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
    struct stat info{};
    if (stat(dirPath.c_str(), &info) != 0)
        return false;
    else if(info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows
        return true;
    return false;
}

/**
 * Creates a directories at the specified path (recursively)
 *
 * @param dirPath Path to the dir
 * @return Successful or not
 */
bool FileUtil::createDirs(const std::string& dirPath) {
    return std::filesystem::create_directories(dirPath);
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

/**
 * Execute external command. Used to execute compiled binaries
 *
 * @param cmd Command to execute
 * @return Output of the command as a string
 */
std::string FileUtil::exec(const std::string& cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("Failed to execute command: " + cmd);
    char buffer[128];
    std::string result;
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != nullptr)
            result += buffer;
    }
    return result;
}

char FileUtil::getDirSeparator() {
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}
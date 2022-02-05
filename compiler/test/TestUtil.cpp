// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "TestUtil.h"

#include <fstream>
#include <dirent.h>
#include <sstream>
#include <memory>
#include <cstring>

bool TestUtil::fileExists(const std::string& filePath) {
    return std::ifstream(filePath.c_str()).good();
}

/**
 * Get subdirectories of the given path
 *
 * @param basePath Path to a directory
 * @return Vector of subdirs
 */
std::vector<std::string> TestUtil::getSubdirs(const std::string& basePath) {
    std::vector<std::string> subdirs;
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(basePath.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
                subdirs.emplace_back(ent->d_name);
        }
        closedir(dir);
    }
    return subdirs;
}

/**
 * Retrieve the contents of a file as a string
 *
 * @param filePath File path
 * @return File contents as a string
 */
std::string TestUtil::getFileContent(const std::string& filePath) {
    std::ifstream symbolTableStream;
    symbolTableStream.open(filePath);
    std::ostringstream stringStream;
    stringStream << symbolTableStream.rdbuf();
    return stringStream.str();
}

/**
 * Convert a string to camel case
 *
 * @param input Input string
 * @return Camel-cased string
 */
std::string TestUtil::toCamelCase(std::string input) {
    for (auto it = input.begin(); it != input.end(); it++) {
        if (*it == '-' || *it == '_') {
            it = input.erase(it);
            *it = toupper(*it);
        }
    }
    return input;
}

/**
 * Execute external command. Used to execute compiled binaries
 *
 * @param cmd Command to execute
 * @return Output of the command as a string
 */
std::string TestUtil::exec(const std::string& cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result;
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

/**
 * Get default executable name of the compiled binary
 *
 * @return Name of the executable including the file extension
 */
std::string TestUtil::getDefaultExecutableName() {
    std::string executableName = "./source";
#ifdef OS_Windows
    executableName = ".\\source.exe";
#endif
    return executableName;
}
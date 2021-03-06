// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "FileUtil.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sys/stat.h>

#ifdef __unix__
#define OS_UNIX
#elif defined(_WIN32) || defined(WIN32)
#define OS_WINDOWS
#endif

/**
 * Checks if a certain file exists on the file system
 *
 * @param filePath Path to the file
 * @return Existence of the file
 */
bool FileUtil::fileExists(const std::string &filePath) { return std::ifstream(filePath.c_str()).good(); }

/**
 * Deletes the file at the given location
 *
 * @param filePath Path to the file
 */
void FileUtil::deleteFile(const std::string &filePath) { std::remove(filePath.c_str()); }

/**
 * Checks if a certain dir exists on the file system
 *
 * @param dirPath Path to the dir
 * @return Existence of the dir
 */
bool FileUtil::dirExists(const std::string &dirPath) {
  struct stat info {};
  if (stat(dirPath.c_str(), &info) != 0)
    return false;
  else if (info.st_mode & S_IFDIR)
    return true;
  return false;
}

/**
 * Creates a directories at the specified path (recursively)
 *
 * @param dirPath Path to the dir
 * @return Successful or not
 */
bool FileUtil::createDirs(const std::string &dirPath) { return std::filesystem::create_directories(dirPath); }

/**
 * Creates a file and writes fileContent to it.
 *
 * @param fileName File name
 * @param fileContent String to write into the file
 */
void FileUtil::writeToFile(const std::string &fileName, const std::string &fileContent) {
  std::ofstream file;
  file.open(fileName);
  file << fileContent;
  file.close();
}

/**
 * Extracts the name of a file from its full path and returns it
 *
 * @param filePath Full path to the file (absolute or relative)
 * @return Name of the file
 */
std::string FileUtil::getFileName(const std::string &filePath) { return filePath.substr(filePath.find_last_of("/\\") + 1); }

/**
 * Extracts the directory of a file from its full path and returns it
 *
 * @param filePath Full path to the file (absolute or relative)
 * @return Path of dir
 */
std::string FileUtil::getFileDir(const std::string &filePath) { return filePath.substr(0, filePath.find_last_of("/\\")); }

/**
 * Execute external command. Used to execute compiled binaries
 *
 * @param cmd Command to execute
 * @return Output of the command as a string
 */
std::string FileUtil::exec(const std::string &cmd) {
  std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
  if (!pipe)
    throw std::runtime_error("Failed to execute command: " + cmd);
  char buffer[128];
  std::string result;
  while (!feof(pipe.get())) {
    if (fgets(buffer, 128, pipe.get()) != nullptr)
      result += buffer;
  }
  return result;
}

/**
 * Checks if a certain command is available on the computer
 *
 * @param cmd Command to search for
 * @return Present or not
 */
bool FileUtil::isCommandAvailable(const std::string &cmd) {
  std::string checkCmd = "which " + cmd + " > /dev/null 2>&1";
#ifdef OS_WINDOWS
  checkCmd = "where " + cmd + " > nul 2>&1";
#endif
  return std::system(checkCmd.c_str());
}

/**
 * Retrieve the dir, where output binaries should go when installing them
 *
 * @return Installation directory
 */
std::string FileUtil::getSpiceBinDir() {
#ifdef _WIN32
  return std::string(std::getenv("USERPROFILE")) + R"(\spice\bin\)";
#else
  return "/usr/local/bin/";
#endif
}
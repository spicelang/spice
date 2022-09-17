// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "FileUtil.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

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
 * @return Result struct
 */
ExecResult FileUtil::exec(const std::string &cmd) {
  FILE *pipe = popen(cmd.c_str(), "r");
  if (!pipe)
    throw std::runtime_error("Failed to execute command: " + cmd);
  char buffer[128];
  std::string result;
  while (!feof(pipe)) {
    if (fgets(buffer, 128, pipe) != nullptr)
      result += buffer;
  }
  int exitCode = pclose(pipe) / 256;
  return {result, exitCode};
}

/**
 * Checks if a certain command is available on the computer
 *
 * @param cmd Command to search for
 * @return Present or not
 */
bool FileUtil::isCommandAvailable(const std::string &cmd) {
#if OS_WINDOWS
  std::string checkCmd = "where " + cmd + " > nul 2>&1";
#else
  std::string checkCmd = "which " + cmd + " > /dev/null 2>&1";
#endif
  return std::system(checkCmd.c_str());
}

/**
 * Retrieve the dir, where the standard library lives.
 * Returns an empty string if the std was not found.
 *
 * @return Std directory
 */
std::string FileUtil::getStdDir() {
#ifdef OS_UNIX
  if (FileUtil::fileExists("/usr/lib/spice/std"))
    return "/usr/lib/spice/std/";
#endif
  if (std::getenv("SPICE_STD_DIR") && FileUtil::dirExists(std::string(std::getenv("SPICE_STD_DIR")))) {
    std::string stdPath = std::string(std::getenv("SPICE_STD_DIR"));
    if (stdPath.rfind(FileUtil::DIR_SEPARATOR) != stdPath.size() - 1)
      stdPath += FileUtil::DIR_SEPARATOR;
    return stdPath;
  }
  return "";
}

/**
 * Retrieve the dir, where output binaries should go when installing them
 *
 * @return Installation directory
 */
std::string FileUtil::getSpiceBinDir() {
#if OS_WINDOWS
  return std::string(std::getenv("USERPROFILE")) + R"(\spice\bin\)";
#else
  return "/usr/local/bin/";
#endif
}
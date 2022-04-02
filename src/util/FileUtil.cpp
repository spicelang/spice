// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "FileUtil.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sys/stat.h>

/**
 * Get the concrete import path from an import statement
 *
 * @return File path
 */
std::string FileUtil::getImportPath(const std::string &importPath, const std::string &sourceFile, const ErrorFactory *err,
                                    const antlr4::Token &token, const CliOptions *cliOptions) {
  if (importPath.rfind("std/", 0) == 0) { // Include source file from standard library
    std::string sourceFileIden = importPath.substr(importPath.find("std/") + 4);
    // Find std library
    std::string stdPath;
    if (FileUtil::fileExists("/usr/lib/spice/std")) {
      stdPath = "/usr/lib/spice/std/";
    } else if (FileUtil::dirExists(std::string(std::getenv("SPICE_STD_DIR")))) {
      stdPath = std::string(std::getenv("SPICE_STD_DIR"));
      if (stdPath.rfind('/') != stdPath.size() - 1)
        stdPath += "/";
    } else {
      throw err->get(token, STD_NOT_FOUND, "Standard library could not be found. Check if the env var SPICE_STD_DIR exists");
    }
    // Check if source file exists
    std::string defaultPath = stdPath + sourceFileIden + ".spice";
    std::string osPath = stdPath + sourceFileIden + "_" + cliOptions->targetOs + ".spice";
    std::string osArchPath = stdPath + sourceFileIden + "_" + cliOptions->targetOs + "_" + cliOptions->targetArch + ".spice";

    if (FileUtil::fileExists(defaultPath)) {
      return defaultPath;
    } else if (FileUtil::fileExists(osPath)) {
      return osPath;
    } else if (FileUtil::fileExists(osArchPath)) {
      return osArchPath;
    }
    throw err->get(token, IMPORTED_FILE_NOT_EXISTING,
                   "The source file '" + importPath + ".spice' was not found in the standard library");
  } else { // Include own source file
    // Check in module registry if the file can be imported
    std::string sourceFileDir = FileUtil::getFileDir(sourceFile);
    // Import file
    std::string defaultPath = sourceFileDir + "/" + importPath + ".spice";
    std::string osPath = sourceFileDir + "/" + importPath + "_" + cliOptions->targetOs + ".spice";
    std::string osArchPath =
        sourceFileDir + "/" + importPath + "_" + cliOptions->targetOs + "_" + cliOptions->targetArch + ".spice";

    if (FileUtil::fileExists(defaultPath)) {
      return defaultPath;
    } else if (FileUtil::fileExists(osPath)) {
      return osPath;
    } else if (FileUtil::fileExists(osArchPath)) {
      return osArchPath;
    }
    throw err->get(token, IMPORTED_FILE_NOT_EXISTING, "The source file '" + importPath + ".spice' does not exist");
  }
}

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
  else if (info.st_mode & S_IFDIR) // S_ISDIR() doesn't exist on my windows
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
 * Retrieve the directory separator for the current os
 *
 * @return Directory separator
 */
char FileUtil::getDirSeparator() {
#ifdef _WIN32
  return '\\';
#else
  return '/';
#endif
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

/**
 * Generate a code loc string with for a token
 *
 * @param token Token
 * @return Code loc string
 */
std::string FileUtil::tokenToCodeLoc(const antlr4::Token &token) {
  return "L" + std::to_string(token.getLine()) + "C" + std::to_string(token.getCharPositionInLine());
}
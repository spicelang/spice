// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <vector>
#include <string>

#include <cli/CliInterface.h>
#include <exception/ErrorFactory.h>

class LinkerInterface {
public:
    // Constructors
    explicit LinkerInterface(const ErrorFactory* errorFactory, CliOptions* options): err(errorFactory), cliOptions(options) {};

    // Public methods
    void link();
    void addObjectFilePath(const std::string& objectFilePath);
    void addLinkerFlag(const std::string& linkerFlag);
    void setOutputPath(const std::string& outputPath);
private:
    // Members
    const ErrorFactory* err;
    const CliOptions* cliOptions;
    std::vector<std::string> objectFilePaths;
    std::vector<std::string> linkerFlags = { "-no-pie" };
    std::string outputPath;
};
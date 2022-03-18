// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include <string>
#include <vector>

#include <exception/ErrorFactory.h>

/**
 * Class to remember source files, which have already been imported. This is required to detect circular dependencies
 */
class ModuleRegistry {
public:
    // Public methods
    void addModule(const ErrorFactory* errorFactory, const antlr4::Token& token, const std::string& moduleName);
private:
    // Members
    std::vector<std::string> modulePaths;
};
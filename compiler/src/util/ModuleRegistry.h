// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include <string>
#include <vector>

#include <exception/ErrorFactory.h>

/**
 * Class to remember source files, which have already been imported. This is required to detect circular dependencies
 */
class ModuleRegistry {
public:
    // Constructors
    static ModuleRegistry* getInstance(ErrorFactory* errorFactory);

    // Public methods
    void addModule(const antlr4::Token& token, const std::string& moduleName);
private:
    // Private constructors
    explicit ModuleRegistry() = default;

    // Members
    static ModuleRegistry* instance;
    ErrorFactory* err = nullptr;
    std::vector<std::string> modulePaths;
};
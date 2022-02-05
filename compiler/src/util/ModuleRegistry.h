// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include <string>
#include <vector>

/**
 * Class to remember source files, which have already been imported. This is required to detect circular dependencies
 */
class ModuleRegistry {
public:
    static ModuleRegistry* getInstance();

    // Public methods
    void addModule(const std::string& moduleName);
private:
    explicit ModuleRegistry() = default;

    static ModuleRegistry* instance;
    std::vector<std::string> modulePaths;
};
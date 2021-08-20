// Copyright (c) 2021 ChilliBits. All rights reserved.

#include <string>
#include <vector>

class ModuleRegistry {
public:
    static ModuleRegistry* getInstance();

    // Public methods
    void addModule(const std::string&);
private:
    explicit ModuleRegistry() {}

    static ModuleRegistry* instance;
    std::vector<std::string> modulePaths;
};
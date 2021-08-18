// Copyright (c) 2021 ChilliBits. All rights reserved.

#include <algorithm>
#include <exception/SemanticError.h>
#include "ModuleRegistry.h"

ModuleRegistry* ModuleRegistry::instance = nullptr;

void ModuleRegistry::addModule(const std::string& modulePath) {
    if (std::find(modulePaths.begin(), modulePaths.end(), modulePath) != modulePaths.end())
        throw SemanticError(CIRCULAR_DEPENDENCY, modulePath);
    modulePaths.push_back(modulePath);
}

ModuleRegistry* ModuleRegistry::getInstance() {
    if (!instance) instance = new ModuleRegistry();
    return instance;
}

// Copyright (c) 2021 ChilliBits. All rights reserved.

#include <algorithm>
#include <exception/SemanticError.h>
#include "ModuleRegistry.h"

// Instance of the module registry
ModuleRegistry* ModuleRegistry::instance = nullptr;

/**
 * Add a module path to the module registry
 *
 * @param modulePath Path to a source file / module
 */
void ModuleRegistry::addModule(const std::string& modulePath) {
    if (std::find(modulePaths.begin(), modulePaths.end(), modulePath) != modulePaths.end())
        throw SemanticError(CIRCULAR_DEPENDENCY, "'" + modulePath + ".spice'");
    modulePaths.push_back(modulePath);
}

/**
 * Retrieve an instance of the module registry.
 *
 * @return Instance of the module registry
 */
ModuleRegistry* ModuleRegistry::getInstance() {
    if (!instance) instance = new ModuleRegistry();
    return instance;
}

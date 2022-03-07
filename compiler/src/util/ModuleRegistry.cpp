// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include <algorithm>

#include <util/ModuleRegistry.h>
#include <exception/SemanticError.h>

// Instance of the module registry
ModuleRegistry* ModuleRegistry::instance = nullptr;

/**
 * Add a module path to the module registry
 *
 * @param modulePath Path to a source file / module
 */
void ModuleRegistry::addModule(const antlr4::Token& token, const std::string& modulePath) {
    if (std::find(modulePaths.begin(), modulePaths.end(), modulePath) != modulePaths.end())
        throw err->get(token, CIRCULAR_DEPENDENCY, "'" + modulePath + ".spice'");
    modulePaths.push_back(modulePath);
}

/**
 * Retrieve an instance of the module registry.
 *
 * @return Instance of the module registry
 */
ModuleRegistry* ModuleRegistry::getInstance(ErrorFactory* errorFactory) {
    if (!instance) instance = new ModuleRegistry();
    instance->err = errorFactory;
    return instance;
}

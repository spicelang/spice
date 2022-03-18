// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include <algorithm>

#include <util/ModuleRegistry.h>
#include <exception/SemanticError.h>

/**
 * Add a module path to the module registry
 *
 * @param modulePath Path to a source file / module
 */
void ModuleRegistry::addModule(const ErrorFactory* err, const antlr4::Token& token, const std::string& modulePath) {
    if (std::find(modulePaths.begin(), modulePaths.end(), modulePath) != modulePaths.end())
        throw err->get(token, CIRCULAR_DEPENDENCY, "'" + modulePath + ".spice'");
    modulePaths.push_back(modulePath);
}
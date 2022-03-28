// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ModuleRegistry.h"

#include <algorithm>

void ModuleRegistry::pushToImportPath(const std::string &modulePath) { currentImportPath.push(modulePath); }

void ModuleRegistry::popFromImportPath() { currentImportPath.pop(); }

void ModuleRegistry::addToCompiledModules(const std::string &modulePath, SymbolTable *symbolTable) {
  compiledModules.insert({modulePath, symbolTable});
}

SymbolTable *ModuleRegistry::getSymbolTable(const std::string &modulePath) const {
  // If not available in the current scope, return nullptr
  if (!compiledModules.contains(modulePath))
    return nullptr;
  return compiledModules.at(modulePath);
}

bool ModuleRegistry::causesCircularImport(const std::string &modulePath) const {
  std::stack<std::string> tmpImportPath = currentImportPath;

  // Check if there is a path on the stack which matches with the module path to check
  while (!tmpImportPath.empty()) {
    if (tmpImportPath.top() == modulePath)
      return true;
    tmpImportPath.pop();
  }

  return false;
}

bool ModuleRegistry::isAlreadyCompiled(const std::string &modulePath) const { return compiledModules.contains(modulePath); }
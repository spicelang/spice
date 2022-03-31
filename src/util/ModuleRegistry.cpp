// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ModuleRegistry.h"

#include <algorithm>

void ModuleRegistry::pushToImportPath(const std::string &modulePath) { currentImportPath.push(modulePath); }

void ModuleRegistry::popFromImportPath() { currentImportPath.pop(); }

void ModuleRegistry::addToAnalyzedModules(const std::string &modulePath, SymbolTable *symbolTable) {
  analyzedModules.insert({modulePath, symbolTable});
}

void ModuleRegistry::addToGeneratedModules(const std::string &modulePath) { generatedModules.insert({modulePath, true}); }

SymbolTable *ModuleRegistry::getSymbolTable(const std::string &modulePath) const {
  // If not available in the current scope, return nullptr
  if (!analyzedModules.contains(modulePath))
    return nullptr;
  return analyzedModules.at(modulePath);
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

bool ModuleRegistry::isAlreadyAnalyzed(const std::string &modulePath) const { return analyzedModules.contains(modulePath); }

bool ModuleRegistry::isAlreadyGenerated(const std::string &modulePath) const { return generatedModules.contains(modulePath); }
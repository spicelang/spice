// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "RuntimeModuleManager.h"

#include <SourceFile.h>
#include <exception/CompilerError.h>
#include <symboltablebuilder/Scope.h>
#include <util/FileUtil.h>

namespace spice::compiler {

SourceFile *RuntimeModuleManager::requestModule(SourceFile *sourceFile, RuntimeModule requestedModule) {
  // Check if the requested module is available already
  bool available = isModuleAvailable(requestedModule);

  // If not, try to make it available
  if (!available)
    available = addModule(sourceFile, requestedModule);

  // If the module is still not available, cancel here
  if (!available)
    return nullptr;

  // Merge the module name registry with the one of the source file
  const ModuleNamePair names = resolveNamePair(requestedModule);
  SourceFile *runtimeModuleFile = modules.at(requestedModule);
  sourceFile->mergeNameRegistries(*runtimeModuleFile, names.importName);

  // Tell the source file, that the requested runtime has been imported
  sourceFile->importedRuntimeModules |= requestedModule;

  return runtimeModuleFile;
}

bool RuntimeModuleManager::isModuleAvailable(RuntimeModule requestedModule) const { return modules.contains(requestedModule); }

bool RuntimeModuleManager::addModule(SourceFile *parentSourceFile, RuntimeModule requestedModule) {
  const auto [importName, fileName] = resolveNamePair(requestedModule);

  const std::filesystem::path filePath = FileUtil::getStdDir() / "runtime" / (std::string(fileName) + ".spice");
  if (filePath == parentSourceFile->filePath)
    return false;

  const auto moduleSourceFile = resourceManager.createSourceFile(parentSourceFile, importName, filePath, true);
  parentSourceFile->addDependency(moduleSourceFile, parentSourceFile->ast, importName, filePath.string());

  // Run frontend and type checker for runtime module source file
  const auto runtimeFile = parentSourceFile->dependencies.at(importName).first;
  runtimeFile->runFrontEnd();
  runtimeFile->runTypeCheckerPre();
  modules.emplace(requestedModule, runtimeFile);

  return true;
}

ModuleNamePair RuntimeModuleManager::resolveNamePair(spice::compiler::RuntimeModule runtimeModule) {
  switch (runtimeModule) {
  case STRING_RT:
    return {STRING_RT_IMPORT_NAME, "string_rt"};
  case MEMORY_RT:
    return {MEMORY_RT_IMPORT_NAME, "memory_rt"};
  case ITERATOR_RT:
    return {ITERATOR_RT_IMPORT_NAME, "iterator_rt"};
  default:
    throw CompilerError(INTERNAL_ERROR, "Requested unknown runtime module");
  }
}

} // namespace spice::compiler
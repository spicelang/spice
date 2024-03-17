// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "RuntimeModuleManager.h"

#include <SourceFile.h>
#include <exception/CompilerError.h>
#include <global/GlobalResourceManager.h>
#include <symboltablebuilder/Scope.h>
#include <util/FileUtil.h>

namespace spice::compiler {

SourceFile *RuntimeModuleManager::requestModule(SourceFile *parentSourceFile, RuntimeModule requestedModule) {
  const std::string importName = resolveNamePair(requestedModule).importName;

  // Check if the requested module is available already, if not load it
  auto rtFile = isModuleAvailable(requestedModule) ? getModule(requestedModule) : loadModule(parentSourceFile, requestedModule);

  // Add the dependency to the parent source file
  parentSourceFile->addDependency(rtFile, parentSourceFile->ast, importName, rtFile->filePath.string());
  assert(parentSourceFile->dependencies.contains(importName));
  SourceFile *runtimeFile = parentSourceFile->dependencies.at(importName);
  modules.emplace(requestedModule, runtimeFile);

  // Merge the module name registry with the one of the source file
  parentSourceFile->mergeNameRegistries(*rtFile, importName);

  // Tell the source file, that the requested runtime has been imported
  parentSourceFile->importedRuntimeModules |= requestedModule;

  return rtFile;
}

SourceFile *RuntimeModuleManager::getModule(RuntimeModule requestedModule) const {
  assert(isModuleAvailable(requestedModule));
  return modules.at(requestedModule);
}

bool RuntimeModuleManager::isModuleAvailable(RuntimeModule requestedModule) const { return modules.contains(requestedModule); }

SourceFile *RuntimeModuleManager::loadModule(SourceFile *parentSourceFile, RuntimeModule requestedModule) {
  const auto [importName, fileName] = resolveNamePair(requestedModule);
  const std::string fileNameWithExt = std::string(fileName) + ".spice";
  const std::filesystem::path filePath = FileUtil::getStdDir() / "runtime" / fileNameWithExt;
  assert(filePath != parentSourceFile->filePath);

  // Instruct the global resource manager to create a new source file
  SourceFile *moduleSourceFile = resourceManager.createSourceFile(parentSourceFile, importName, filePath, true);
  moduleSourceFile->mainFile = false;

  // Run frontend and first type checker run for the loaded source file
  moduleSourceFile->runFrontEnd();
  moduleSourceFile->runTypeCheckerPre();

  return moduleSourceFile;
}

ModuleNamePair RuntimeModuleManager::resolveNamePair(RuntimeModule runtimeModule) {
  switch (runtimeModule) {
  case STRING_RT:
    return {STRING_RT_IMPORT_NAME, "string_rt"};
  case RESULT_RT:
    return {RESULT_RT_IMPORT_NAME, "result_rt"};
  case MEMORY_RT:
    return {MEMORY_RT_IMPORT_NAME, "memory_rt"};
  case RTTI_RT:
    return {RTTI_RT_IMPORT_NAME, "rtti_rt"};
  default:
    throw CompilerError(INTERNAL_ERROR, "Requested unknown runtime module");
  }
}

} // namespace spice::compiler
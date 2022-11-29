// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "RuntimeModuleManager.h"

#include <SourceFile.h>
#include <scope/Scope.h>
#include <util/FileUtil.h>

#include <ast/ASTNodes.h>

SourceFile *RuntimeModuleManager::requestModule(SourceFile *sourceFile, const RuntimeModuleName &moduleName) {
  // Make the module available
  bool available = isModuleAvailable(moduleName);
  if (!available)
    available = addModule(sourceFile, moduleName);

  return available ? modules.at(moduleName) : nullptr;
}

Scope *RuntimeModuleManager::getModuleScope(const RuntimeModuleName &moduleName) const {
  assert(modules.contains(moduleName));
  return modules.at(moduleName)->globalScope.get();
}

bool RuntimeModuleManager::isModuleAvailable(const RuntimeModuleName &module) const { return modules.contains(module); }

bool RuntimeModuleManager::addModule(SourceFile *parentSourceFile, const RuntimeModuleName &moduleName) {
  std::string importName;
  std::string fileName;
  switch (moduleName) {
  case STRING_RT:
    importName = STRING_RT_IMPORT_NAME;
    fileName = "string_rt";
    break;
  case THREAD_RT:
    importName = THREAD_RT_IMPORT_NAME;
    fileName = "thread_rt";
    break;
  default:
    throw std::runtime_error("Internal compiler error: Requested unknown runtime module");
  }
  std::string filePath = FileUtil::getStdDir() + "runtime" + FileUtil::DIR_SEPARATOR + fileName + ".spice";
  if (filePath == parentSourceFile->filePath)
    return false;
  const auto moduleSourceFile = parentSourceFile->createSourceFile(importName, filePath, true);
  parentSourceFile->addDependency(moduleSourceFile, parentSourceFile->ast.get(), importName, filePath);

  // Run frontend and type checker for runtime module source file
  const auto runtimeFile = parentSourceFile->dependencies.at(importName).first;
  runtimeFile->runFrontEnd();
  runtimeFile->runTypeChecker();
  modules.emplace(moduleName, runtimeFile.get());

  // Merge the module name registry with the one of the source file
  parentSourceFile->mergeNameRegistries(*runtimeFile, importName);

  return true;
}
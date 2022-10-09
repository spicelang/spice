// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "RuntimeModuleManager.h"

#include <dependency/SourceFile.h>
#include <symbol/SymbolTable.h>
#include <util/FileUtil.h>

#include <ast/AstNodes.h>

SourceFile *RuntimeModuleManager::requestModule(SourceFile *sourceFile, const RuntimeModuleName &moduleName) {
  // Make the module available
  if (!isModuleAvailable(moduleName))
    addModule(sourceFile, moduleName);

  return modules.at(moduleName);
}

SymbolTable *RuntimeModuleManager::getModuleScope(const RuntimeModuleName &moduleName) const {
  assert(modules.contains(moduleName));
  return modules.at(moduleName)->symbolTable.get();
}

bool RuntimeModuleManager::isModuleAvailable(const RuntimeModuleName &module) const { return modules.contains(module); }

void RuntimeModuleManager::addModule(SourceFile *parentSourceFile, const RuntimeModuleName &moduleName) {
  std::string importName;
  std::string fileName;
  switch (moduleName) {
  case STRING_RT:
    importName = "__rt_string";
    fileName = "string_rt";
    break;
  case THREAD_RT:
    importName = "__rt_thread";
    fileName = "thread_rt";
    break;
  default:
    throw std::runtime_error("Internal compiler error: Requested unknown runtime module");
  }
  std::string filePath = FileUtil::getStdDir() + "runtime" + FileUtil::DIR_SEPARATOR + fileName + ".spice";
  const auto moduleSourceFile = parentSourceFile->createSourceFile(importName, filePath, true);
  parentSourceFile->addDependency(moduleSourceFile, parentSourceFile->ast.get(), importName, filePath);

  const auto runtimeFile = parentSourceFile->dependencies.at(importName).first;
  runtimeFile->buildAST();
  runtimeFile->preAnalyze();
  runtimeFile->analyze();
  runtimeFile->reAnalyze();

  modules.emplace(moduleName, runtimeFile.get());
}
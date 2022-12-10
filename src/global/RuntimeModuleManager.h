// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <unordered_map>

// Forward declaration
class SourceFile;
class Scope;

const char *const STRING_RT_IMPORT_NAME = "__rt_string";
const char *const THREAD_RT_IMPORT_NAME = "__rt_thread";

enum RuntimeModuleName { STRING_RT, THREAD_RT };

class RuntimeModuleManager {
public:
  // Constructors
  RuntimeModuleManager() = default;
  RuntimeModuleManager(const RuntimeModuleManager &) = delete;

  // Public methods
  SourceFile *requestModule(SourceFile *sourceFile, const RuntimeModuleName &moduleName);
  [[nodiscard]] Scope *getModuleScope(const RuntimeModuleName &moduleName) const;
  [[nodiscard]] bool isModuleAvailable(const RuntimeModuleName &moduleName) const;

private:
  // Private methods
  bool addModule(SourceFile *parentSourceFile, const RuntimeModuleName &moduleName);

  // Private fields
  std::unordered_map<RuntimeModuleName, SourceFile *> modules;
};
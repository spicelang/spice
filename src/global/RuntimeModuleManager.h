// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <unordered_map>

namespace spice::compiler {

// Forward declaration
class GlobalResourceManager;
class SourceFile;
class Scope;

const char *const STRING_RT_IMPORT_NAME = "__rt_string";
const char *const MEMORY_RT_IMPORT_NAME = "__rt_memory";
const char *const ITERATOR_RT_IMPORT_NAME = "__rt_iterator";

enum RuntimeModule : uint8_t {
  STRING_RT = 1 << 0,
  MEMORY_RT = 1 << 1,
  ITERATOR_RT = 1 << 2,
};

struct ModuleNamePair {
  const char *const importName;
  const char *const fileName;
};

class RuntimeModuleManager {
public:
  // Constructors
  explicit RuntimeModuleManager(GlobalResourceManager &resourceManager) : resourceManager(resourceManager){};
  RuntimeModuleManager(const RuntimeModuleManager &) = delete;

  // Public methods
  SourceFile *requestModule(SourceFile *parentSourceFile, RuntimeModule requestedModule);
  [[nodiscard]] bool isModuleAvailable(RuntimeModule requestedModule) const;

private:
  // Private methods
  SourceFile *loadModule(SourceFile *parentSourceFile, RuntimeModule requestedModule);
  static ModuleNamePair resolveNamePair(RuntimeModule requestedModule);

  // Private members
  GlobalResourceManager &resourceManager;
  std::unordered_map<RuntimeModule, SourceFile *> modules;
};

} // namespace spice::compiler
// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <unordered_map>

namespace spice::compiler {

// Forward declaration
class SourceFile;
class Scope;

const char *const STRING_RT_IMPORT_NAME = "__rt_string";
const char *const THREAD_RT_IMPORT_NAME = "__rt_thread";

enum RuntimeModule : uint8_t {
  STRING_RT = 1 << 0,
  THREAD_RT = 1 << 1,
};

struct ModuleNamePair {
  const char *const importName;
  const char *const fileName;
};

class RuntimeModuleManager {
public:
  // Constructors
  RuntimeModuleManager() = default;
  RuntimeModuleManager(const RuntimeModuleManager &) = delete;

  // Public methods
  SourceFile *requestModule(SourceFile *sourceFile, RuntimeModule requestedModule);
  [[nodiscard]] bool isModuleAvailable(RuntimeModule requestedModule) const;

private:
  // Private methods
  bool addModule(SourceFile *parentSourceFile, RuntimeModule requestedModule);
  static ModuleNamePair resolveNamePair(RuntimeModule requestedModule);

  // Private fields
  std::unordered_map<RuntimeModule, SourceFile *> modules;
};

} // namespace spice::compiler
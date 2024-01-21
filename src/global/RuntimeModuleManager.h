// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <unordered_map>

#include <symboltablebuilder/SymbolType.h>

namespace spice::compiler {

// Forward declaration
class GlobalResourceManager;
class SourceFile;
class Scope;

const char *const STRING_RT_IMPORT_NAME = "__rt_string";
const char *const MEMORY_RT_IMPORT_NAME = "__rt_memory";
const char *const RTTI_RT_IMPORT_NAME = "__rt_rtti";

enum RuntimeModule : uint8_t {
  STRING_RT = 1 << 0,
  MEMORY_RT = 1 << 1,
  RTTI_RT = 1 << 2,
};

const std::unordered_map<const char *, RuntimeModule> TYPE_NAME_TO_RT_MODULE_MAPPING = {{STROBJ_NAME, STRING_RT}};

const std::unordered_map<const char *, RuntimeModule> FCT_NAME_TO_RT_MODULE_MAPPING = {
    {"sAlloc", MEMORY_RT}, {"sRealloc", MEMORY_RT},      {"sCopy", MEMORY_RT},   {"sDealloc", MEMORY_RT},
    {"sNew", MEMORY_RT},   {"sPlacementNew", MEMORY_RT}, {"sDelete", MEMORY_RT},
};

// This serves for the compiler to detect if a source file is a specific runtime module
const std::unordered_map<RuntimeModule, const char *> IDENTIFYING_TOP_LEVEL_NAMES = {
    {STRING_RT, STROBJ_NAME}, // String struct
    {MEMORY_RT, "sAlloc"},    // sAlloc function
    {RTTI_RT, TIOBJ_NAME},    // TypeInfo struct
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
  [[nodiscard]] SourceFile *getModule(RuntimeModule requestedModule) const;
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
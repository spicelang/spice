// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <unordered_map>

#include <symboltablebuilder/QualType.h>

namespace spice::compiler {

// Forward declaration
class GlobalResourceManager;
class SourceFile;
class Scope;

const char *const STRING_RT_IMPORT_NAME = "__rt_string";
const char *const RESULT_RT_IMPORT_NAME = "__rt_result";
const char *const ERROR_RT_IMPORT_NAME = "__rt_error";
const char *const MEMORY_RT_IMPORT_NAME = "__rt_memory";
const char *const RTTI_RT_IMPORT_NAME = "__rt_rtti";

enum RuntimeModule : uint8_t {
  STRING_RT = 1 << 0,
  RESULT_RT = 1 << 1,
  ERROR_RT = 1 << 2,
  MEMORY_RT = 1 << 3,
  RTTI_RT = 1 << 4,
};

const std::unordered_map<const char *, RuntimeModule> TYPE_NAME_TO_RT_MODULE_MAPPING = {
    {STROBJ_NAME, STRING_RT},
    {RESULTOBJ_NAME, RESULT_RT},
    {ERROBJ_NAME, ERROR_RT},
};

const std::unordered_map<const char *, RuntimeModule> FCT_NAME_TO_RT_MODULE_MAPPING = {
    // Memory RT
    {"sAlloc", MEMORY_RT},
    {"sAllocUnsafe", MEMORY_RT},
    {"sRealloc", MEMORY_RT},
    {"sReallocUnsafe", MEMORY_RT},
    {"sCopy", MEMORY_RT},
    {"sCopyUnsafe", MEMORY_RT},
    {"sMove", MEMORY_RT},
    {"sNew", MEMORY_RT},
    {"sPlacementNew", MEMORY_RT},
    {"sDestruct", MEMORY_RT},
    {"sDealloc", MEMORY_RT},
    {"sDelete", MEMORY_RT},
    {"sYieldOwnership", MEMORY_RT},
    {"sCompare", MEMORY_RT},
    // Result RT
    {"ok", RESULT_RT},
    {"err", RESULT_RT},
};

// This serves for the compiler to detect if a source file is a specific runtime module
const std::unordered_map<RuntimeModule, const char *> IDENTIFYING_TOP_LEVEL_NAMES = {
    {STRING_RT, STROBJ_NAME},    // String struct
    {RESULT_RT, RESULTOBJ_NAME}, // Result struct
    {ERROR_RT, ERROBJ_NAME},     // Error struct
    {MEMORY_RT, "sAlloc"},       // sAlloc function
    {RTTI_RT, TIOBJ_NAME},       // TypeInfo struct
};

struct ModuleNamePair {
  const char *const importName;
  const char *const fileName;
};

class RuntimeModuleManager {
public:
  // Constructors
  explicit RuntimeModuleManager(GlobalResourceManager &resourceManager) : resourceManager(resourceManager) {}
  RuntimeModuleManager(const RuntimeModuleManager &) = delete;

  // Public methods
  SourceFile *requestModule(SourceFile *parentSourceFile, RuntimeModule requestedModule);
  [[nodiscard]] SourceFile *getModule(RuntimeModule requestedModule) const;
  [[nodiscard]] bool isModuleAvailable(RuntimeModule requestedModule) const;

private:
  // Private methods
  SourceFile *loadModule(SourceFile *parentSourceFile, RuntimeModule requestedModule) const;
  static ModuleNamePair resolveNamePair(RuntimeModule runtimeModule);

  // Private members
  GlobalResourceManager &resourceManager;
  std::unordered_map<RuntimeModule, SourceFile *> modules;
};

} // namespace spice::compiler
// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <unordered_map>

#include <ast/ASTNodes.h>

namespace spice::compiler {

// Constants
static constexpr const char *const ATTR_CORE_LINKER_FLAG = "core.linker.flag";
static constexpr const char *const ATTR_CORE_LINUX_LINKER_FLAG = "core.linux.linker.flag";
static constexpr const char *const ATTR_CORE_WINDOWS_LINKER_FLAG = "core.windows.linker.flag";
static constexpr const char *const ATTR_CORE_LINKER_ADDITIONAL_SOURCE = "core.linker.additional_source";
static constexpr const char *const ATTR_CORE_LINKER_DLL = "core.linker.dll";
static constexpr const char *const ATTR_CORE_COMPILER_MANGLE = "core.compiler.mangle";
static constexpr const char *const ATTR_CORE_COMPILER_MANGLED_NAME = "core.compiler.mangledName";
static constexpr const char *const ATTR_CORE_COMPILER_KEEP_ON_NAME_COLLISION = "core.compiler.alwaysKeepOnNameCollision";
static constexpr const char *const ATTR_CORE_COMPILER_FIXED_TYPE_ID = "core.compiler.fixedTypeId";
static constexpr const char *const ATTR_CORE_COMPILER_EMIT_VTABLE = "core.compiler.alwaysEmitVTable";
static constexpr const char *const ATTR_CORE_COMPILER_PACKED = "core.compiler.packed";
static constexpr const char *const ATTR_CORE_COMPILER_WARNINGS_IGNORE = "core.compiler.warnings.ignore";
static constexpr const char *const ATTR_TEST = "test";
static constexpr const char *const ATTR_TEST_NAME = "test.name";
static constexpr const char *const ATTR_TEST_SKIP = "test.skip";
static constexpr const char *const ATTR_ASYNC = "async";

static const CompileTimeValue DEFAULT_BOOL_COMPILE_VALUE{.boolValue = true};

// Structs
struct AttrConfigValue {
  const uint8_t target;
  const AttrNode::AttrType type;
};

// Data
static const std::unordered_map<std::string, AttrConfigValue> ATTR_CONFIGS = {
    {
        ATTR_CORE_LINKER_FLAG,
        {
            .target = AttrNode::TARGET_MODULE,
            .type = AttrNode::TYPE_STRING,
        },
    },
    {
        ATTR_CORE_LINUX_LINKER_FLAG,
        {
            .target = AttrNode::TARGET_MODULE,
            .type = AttrNode::TYPE_STRING,
        },
    },
    {
        ATTR_CORE_WINDOWS_LINKER_FLAG,
        {
            .target = AttrNode::TARGET_MODULE,
            .type = AttrNode::TYPE_STRING,
        },
    },
    {
        ATTR_CORE_LINKER_ADDITIONAL_SOURCE,
        {
            .target = AttrNode::TARGET_MODULE,
            .type = AttrNode::TYPE_STRING,
        },
    },
    {
        ATTR_CORE_COMPILER_KEEP_ON_NAME_COLLISION,
        {
            .target = AttrNode::TARGET_MODULE,
            .type = AttrNode::TYPE_BOOL,
        },
    },
    {
        ATTR_CORE_COMPILER_FIXED_TYPE_ID,
        {
            .target = AttrNode::TARGET_STRUCT | AttrNode::TARGET_INTERFACE,
            .type = AttrNode::TYPE_INT,
        },
    },
    {
        ATTR_CORE_LINKER_DLL,
        {
            .target = AttrNode::TARGET_EXT_DECL,
            .type = AttrNode::TYPE_BOOL,
        },
    },
    {
        ATTR_CORE_COMPILER_MANGLE,
        {
            .target = AttrNode::TARGET_FCT_PROC | AttrNode::TARGET_EXT_DECL,
            .type = AttrNode::TYPE_BOOL,
        },
    },
    {
        ATTR_CORE_COMPILER_MANGLED_NAME,
        {
            .target = AttrNode::TARGET_FCT_PROC | AttrNode::TARGET_EXT_DECL,
            .type = AttrNode::TYPE_STRING,
        },
    },
    {
        ATTR_CORE_COMPILER_KEEP_ON_NAME_COLLISION,
        {
            .target = AttrNode::TARGET_MODULE,
            .type = AttrNode::TYPE_BOOL,
        },
    },
    {
        ATTR_CORE_COMPILER_EMIT_VTABLE,
        {
            .target = AttrNode::TARGET_STRUCT,
            .type = AttrNode::TYPE_BOOL,
        },
    },
    {
        ATTR_CORE_COMPILER_PACKED,
        {
            .target = AttrNode::TARGET_STRUCT,
            .type = AttrNode::TYPE_BOOL,
        },
    },
    {
        ATTR_CORE_COMPILER_WARNINGS_IGNORE,
        {
            .target = AttrNode::TARGET_MODULE,
            .type = AttrNode::TYPE_BOOL,
        },
    },
    {
        ATTR_TEST,
        {
            .target = AttrNode::TARGET_FCT_PROC,
            .type = AttrNode::TYPE_BOOL,
        },
    },
    {
        ATTR_TEST_NAME,
        {
            .target = AttrNode::TARGET_FCT_PROC,
            .type = AttrNode::TYPE_STRING,
        },
    },
    {
        ATTR_TEST_SKIP,
        {
            .target = AttrNode::TARGET_FCT_PROC,
            .type = AttrNode::TYPE_BOOL,
        },
    },
    {
        ATTR_ASYNC,
        {
            .target = AttrNode::TARGET_FCT_PROC | AttrNode::TARGET_LAMBDA,
            .type = AttrNode::TYPE_BOOL,
        },
    },
};

} // namespace spice::compiler
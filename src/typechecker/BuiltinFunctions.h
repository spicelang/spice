// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <algorithm>
#include <array>
#include <limits>
#include <string_view>
#include <unordered_map>

namespace spice::compiler {

// Metadata about a builtin function (argument/template counts and flags only, no method pointers)
struct BuiltinFunctionInfo {
  unsigned int minTemplateTypes = 0;
  unsigned int maxTemplateTypes = 0;
  unsigned int minArgTypes = 0;
  unsigned int maxArgTypes = 0;
  bool allTemplateTypesOrAllArgTypes = false;
  bool isFunctionTerminator = false;
};

struct BuiltinFunctionEntry {
  std::string_view name;
  BuiltinFunctionInfo info;
};

// Constants
// Documented builtins
static constexpr std::string_view BUILTIN_FCT_NAME_PRINTF = "printf";
static constexpr std::string_view BUILTIN_FCT_NAME_SIZEOF = "sizeof";
static constexpr std::string_view BUILTIN_FCT_NAME_ALIGNOF = "alignof";
static constexpr std::string_view BUILTIN_FCT_NAME_TYPEID = "typeid";
static constexpr std::string_view BUILTIN_FCT_NAME_TYPENAME = "typename";
static constexpr std::string_view BUILTIN_FCT_NAME_LEN = "len";
static constexpr std::string_view BUILTIN_FCT_NAME_PANIC = "panic";
static constexpr std::string_view BUILTIN_FCT_NAME_SYSCALL = "syscall";
// Undocumented builtins (intended to be primarily used via std wrapper functions)
static constexpr std::string_view BUILTIN_FCT_NAME_OFFSETOF = "__offsetof";
static constexpr std::string_view BUILTIN_FCT_NAME_IS_SAME = "__is_same";
static constexpr std::string_view BUILTIN_FCT_NAME_IMPLEMENTS_INTERFACE = "__implements_interface";
static constexpr std::string_view BUILTIN_FCT_NAME_GET_BUILD_VAR = "__get_build_var";
static constexpr std::string_view BUILTIN_FCT_NAME_IS_TRIVIALLY_CONSTRUCTIBLE = "__is_trivially_constructible";
static constexpr std::string_view BUILTIN_FCT_NAME_IS_TRIVIALLY_COPYABLE = "__is_trivially_copyable";
static constexpr std::string_view BUILTIN_FCT_NAME_IS_TRIVIALLY_DESTRUCTIBLE = "__is_trivially_destructible";
static constexpr std::string_view BUILTIN_FCT_NAME_IS_HEAP = "__is_heap";
static constexpr std::string_view BUILTIN_FCT_NAME_NEW = "__new";
static constexpr std::string_view BUILTIN_FCT_NAME_PLACEMENT_NEW = "__placement_new";

static constexpr std::array BUILTIN_FUNCTIONS = {
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_PRINTF,
        BuiltinFunctionInfo{
            .minArgTypes = 1,
            .maxArgTypes = std::numeric_limits<unsigned int>::max(),
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_SIZEOF,
        BuiltinFunctionInfo{
            .maxTemplateTypes = 1,
            .maxArgTypes = 1,
            .allTemplateTypesOrAllArgTypes = true,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_ALIGNOF,
        BuiltinFunctionInfo{
            .maxTemplateTypes = 1,
            .maxArgTypes = 1,
            .allTemplateTypesOrAllArgTypes = true,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_OFFSETOF,
        BuiltinFunctionInfo{
            .minArgTypes = 2,
            .maxArgTypes = 2,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_TYPEID,
        BuiltinFunctionInfo{
            .maxTemplateTypes = 1,
            .maxArgTypes = 1,
            .allTemplateTypesOrAllArgTypes = true,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_TYPENAME,
        BuiltinFunctionInfo{
            .maxTemplateTypes = 1,
            .maxArgTypes = 1,
            .allTemplateTypesOrAllArgTypes = true,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_LEN,
        BuiltinFunctionInfo{
            .minArgTypes = 1,
            .maxArgTypes = 1,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_PANIC,
        BuiltinFunctionInfo{
            .minArgTypes = 1,
            .maxArgTypes = 1,
            .isFunctionTerminator = true,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_SYSCALL,
        BuiltinFunctionInfo{
            .minArgTypes = 1,
            // According to https://www.chromium.org/chromium-os/developer-library/reference/linux-constants/syscalls/
            .maxArgTypes = 6,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_IS_SAME,
        BuiltinFunctionInfo{
            .minTemplateTypes = 2,
            .maxTemplateTypes = std::numeric_limits<unsigned int>::max(),
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_IMPLEMENTS_INTERFACE,
        BuiltinFunctionInfo{
            .minTemplateTypes = 2,
            .maxTemplateTypes = 2,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_GET_BUILD_VAR,
        BuiltinFunctionInfo{
            .minTemplateTypes = 1,
            .maxTemplateTypes = 1,
            .minArgTypes = 1,
            .maxArgTypes = 2,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_IS_TRIVIALLY_CONSTRUCTIBLE,
        BuiltinFunctionInfo{
            .minTemplateTypes = 1,
            .maxTemplateTypes = 1,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_IS_TRIVIALLY_COPYABLE,
        BuiltinFunctionInfo{
            .minTemplateTypes = 1,
            .maxTemplateTypes = 1,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_IS_TRIVIALLY_DESTRUCTIBLE,
        BuiltinFunctionInfo{
            .minTemplateTypes = 1,
            .maxTemplateTypes = 1,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_IS_HEAP,
        BuiltinFunctionInfo{
            .minTemplateTypes = 1,
            .maxTemplateTypes = 1,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_NEW,
        BuiltinFunctionInfo{
            .minTemplateTypes = 1,
            .maxTemplateTypes = 1,
            .maxArgTypes = std::numeric_limits<unsigned int>::max(),
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_PLACEMENT_NEW,
        BuiltinFunctionInfo{
            .minTemplateTypes = 1,
            .maxTemplateTypes = 1,
            .minArgTypes = 1,
            .maxArgTypes = std::numeric_limits<unsigned int>::max(),
        },
    },
};

static const std::unordered_map<std::string_view, BuiltinFunctionInfo> BUILTIN_FUNCTIONS_MAP = [] {
  std::unordered_map<std::string_view, BuiltinFunctionInfo> map;
  for (const auto &[name, info] : BUILTIN_FUNCTIONS)
    map.emplace(name, info);
  return map;
}(); // LCOV_EXCL_LINE - Coverage tool false positive

// Validate builtins at compile time
static consteval bool validateBuiltins() {
  return std::ranges::all_of(BUILTIN_FUNCTIONS, [](const BuiltinFunctionEntry &entry) {
    const auto &[name, info] = entry;
    return !name.empty() && info.minTemplateTypes <= info.maxTemplateTypes && info.minArgTypes <= info.maxArgTypes;
  });
}
static_assert(validateBuiltins(), "Invalid builtin function definitions");

} // namespace spice::compiler

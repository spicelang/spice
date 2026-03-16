// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <array>

#include <irgenerator/IRGenerator.h>
#include <typechecker/TypeChecker.h>

namespace spice::compiler {

using TypeCheckerVisitMethod = std::any (TypeChecker::*)(FctCallNode *node) const;
using IRGeneratorVisitMethod = std::any (IRGenerator::*)(const FctCallNode *node);

// Represents a compiler builtin function
struct BuiltinFunctionInfo {
  TypeCheckerVisitMethod typeCheckerVisitMethod = nullptr;
  IRGeneratorVisitMethod irGeneratorVisitMethod = nullptr;
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
static constexpr std::string_view BUILTIN_FCT_NAME_LEN = "len";
static constexpr std::string_view BUILTIN_FCT_NAME_PANIC = "panic";
static constexpr std::string_view BUILTIN_FCT_NAME_SYSCALL = "syscall";
// Undocumented builtins (intended to be primarily used via std wrapper functions)
static constexpr std::string_view BUILTIN_FCT_NAME_IS_SAME = "__is_same";
static constexpr std::string_view BUILTIN_FCT_NAME_IMPLEMENTS_INTERFACE = "__implements_interface";
static constexpr std::string_view BUILTIN_FCT_NAME_IS_POINTER_TYPE = "__is_pointer_type";
static constexpr std::string_view BUILTIN_FCT_NAME_IS_REFERENCE_TYPE = "__is_reference_type";
static constexpr std::string_view BUILTIN_FCT_NAME_IS_TRIVIALLY_CONSTRUCTIBLE = "__is_trivially_constructible";
static constexpr std::string_view BUILTIN_FCT_NAME_IS_TRIVIALLY_COPYABLE = "__is_trivially_copyable";
static constexpr std::string_view BUILTIN_FCT_NAME_IS_TRIVIALLY_DESTRUCTIBLE = "__is_trivially_destructible";

static constexpr std::array BUILTIN_FUNCTIONS = {
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_PRINTF,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinPrintfCall,
            .irGeneratorVisitMethod = &IRGenerator::visitBuiltinPrintfCall,
            .minArgTypes = 1,
            .maxArgTypes = std::numeric_limits<unsigned int>::max(),
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_SIZEOF,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinSizeOfCall,
            .maxTemplateTypes = 1,
            .maxArgTypes = 1,
            .allTemplateTypesOrAllArgTypes = true,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_ALIGNOF,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinAlignOfCall,
            .maxTemplateTypes = 1,
            .maxArgTypes = 1,
            .allTemplateTypesOrAllArgTypes = true,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_TYPEID,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinTypeIdCall,
            .maxTemplateTypes = 1,
            .maxArgTypes = 1,
            .allTemplateTypesOrAllArgTypes = true,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_LEN,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinLenCall,
            .irGeneratorVisitMethod = &IRGenerator::visitBuiltinLenCall,
            .minArgTypes = 1,
            .maxArgTypes = 1,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_PANIC,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinPanicCall,
            .irGeneratorVisitMethod = &IRGenerator::visitBuiltinPanicCall,
            .minArgTypes = 1,
            .maxArgTypes = 1,
            .isFunctionTerminator = true,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_SYSCALL,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinSyscallCall,
            .irGeneratorVisitMethod = &IRGenerator::visitBuiltinSyscallCall,
            .minArgTypes = 1,
            // According to https://www.chromium.org/chromium-os/developer-library/reference/linux-constants/syscalls/
            .maxArgTypes = 6,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_IS_SAME,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinIsSameCall,
            .minTemplateTypes = 2,
            .maxTemplateTypes = std::numeric_limits<unsigned int>::max(),
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_IMPLEMENTS_INTERFACE,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinImplementsInterfaceCall,
            .minTemplateTypes = 2,
            .maxTemplateTypes = 2,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_IS_POINTER_TYPE,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinIsPointerTypeCall,
            .minTemplateTypes = 1,
            .maxTemplateTypes = 1,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_IS_REFERENCE_TYPE,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinIsReferenceTypeCall,
            .minTemplateTypes = 1,
            .maxTemplateTypes = 1,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_IS_TRIVIALLY_CONSTRUCTIBLE,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinIsTriviallyConstructibleCall,
            .minTemplateTypes = 1,
            .maxTemplateTypes = 1,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_IS_TRIVIALLY_COPYABLE,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinIsTriviallyCopyableCall,
            .minTemplateTypes = 1,
            .maxTemplateTypes = 1,
        },
    },
    BuiltinFunctionEntry{
        BUILTIN_FCT_NAME_IS_TRIVIALLY_DESTRUCTIBLE,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinIsTriviallyDestructibleCall,
            .minTemplateTypes = 1,
            .maxTemplateTypes = 1,
        },
    },
};

static const std::unordered_map<std::string_view, BuiltinFunctionInfo> BUILTIN_FUNCTIONS_MAP = [] {
  std::unordered_map<std::string_view, BuiltinFunctionInfo> map;
  for (const auto &[name, info] : BUILTIN_FUNCTIONS)
    map.emplace(name, info);
  return map;
}();

// Validate builtins at compile time
static consteval bool validateBuiltins() {
  return std::ranges::all_of(BUILTIN_FUNCTIONS, [](const BuiltinFunctionEntry &entry) {
    const auto &[name, info] = entry;
    return !name.empty() && info.minTemplateTypes <= info.maxTemplateTypes && info.minArgTypes <= info.maxArgTypes;
  });
}
static_assert(validateBuiltins(), "Invalid builtin function definitions");

} // namespace spice::compiler

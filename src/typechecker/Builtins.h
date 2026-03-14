// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <irgenerator/IRGenerator.h>
#include <typechecker/TypeChecker.h>

namespace spice::compiler {

using TypeCheckerVisitMethod = std::any (TypeChecker::*)(FctCallNode *node) const;
using IRGeneratorVisitMethod = std::any (IRGenerator::*)(const FctCallNode *node);

// Represents a compiler builtin function
struct BuiltinFunctionInfo {
  TypeCheckerVisitMethod typeCheckerVisitMethod;
  IRGeneratorVisitMethod irGeneratorVisitMethod;
};

// Constants
// Documented builtins
static constexpr auto BUILTIN_FCT_NAME_PRINTF = "printf";
static constexpr auto BUILTIN_FCT_NAME_SIZEOF = "sizeof";
static constexpr auto BUILTIN_FCT_NAME_ALIGNOF = "alignof";
static constexpr auto BUILTIN_FCT_NAME_TYPEID = "typeid";
static constexpr auto BUILTIN_FCT_NAME_LEN = "len";
static constexpr auto BUILTIN_FCT_NAME_PANIC = "panic";
static constexpr auto BUILTIN_FCT_NAME_SYSCALL = "syscall";
// Undocumented builtins (intended to be primarily used via std wrapper functions)
static constexpr auto BUILTIN_FCT_NAME_IS_SAME = "__is_same";
static constexpr auto BUILTIN_FCT_NAME_IMPLEMENTS_INTERFACE = "__implements_interface";

static const std::unordered_map<std::string, BuiltinFunctionInfo> BUILTIN_FUNCTIONS = {
    {
        BUILTIN_FCT_NAME_PRINTF,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinPrintfCall,
            .irGeneratorVisitMethod = &IRGenerator::visitBuiltinPrintfCall,
        },
    },
    {
        BUILTIN_FCT_NAME_SIZEOF,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinSizeOfCall,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_ALIGNOF,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinAlignOfCall,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_TYPEID,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinTypeIdCall,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_LEN,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinLenCall,
            .irGeneratorVisitMethod = &IRGenerator::visitBuiltinLenCall,
        },
    },
    {
        BUILTIN_FCT_NAME_PANIC,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinPanicCall,
            .irGeneratorVisitMethod = &IRGenerator::visitBuiltinPanicCall,
        },
    },
    {
        BUILTIN_FCT_NAME_SYSCALL,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinSyscallCall,
            .irGeneratorVisitMethod = &IRGenerator::visitBuiltinSyscallCall,
        },
    },
    {
        BUILTIN_FCT_NAME_IS_SAME,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinIsSameCall,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_IMPLEMENTS_INTERFACE,
        BuiltinFunctionInfo{
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinImplementsInterfaceCall,
            .irGeneratorVisitMethod = nullptr,
        },
    },
};

} // namespace spice::compiler

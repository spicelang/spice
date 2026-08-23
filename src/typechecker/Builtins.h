// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <typechecker/BuiltinFunctions.h>

#include <irgenerator/IRGenerator.h>
#include <typechecker/TypeChecker.h>

namespace spice::compiler {

using TypeCheckerVisitMethod = std::any (TypeChecker::*)(FctCallNode *node) const;
using IRGeneratorVisitMethod = std::any (IRGenerator::*)(const FctCallNode *node);

// Method-pointer dispatch entry for one builtin function
struct BuiltinFunctionDispatch {
  TypeCheckerVisitMethod typeCheckerVisitMethod = nullptr;
  IRGeneratorVisitMethod irGeneratorVisitMethod = nullptr;
};

static const std::unordered_map<std::string_view, BuiltinFunctionDispatch> BUILTIN_DISPATCH_MAP = {
    {
        BUILTIN_FCT_NAME_PRINTF,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinPrintfCall,
            .irGeneratorVisitMethod = &IRGenerator::visitBuiltinPrintfCall,
        },
    },
    {
        BUILTIN_FCT_NAME_SIZEOF,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinSizeOfCall,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_ALIGNOF,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinAlignOfCall,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_OFFSETOF,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinOffsetOfCall,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_TYPEID,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinTypeIdCall,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_TYPENAME,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinTypeNameCall,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_LEN,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinLenCall,
            .irGeneratorVisitMethod = &IRGenerator::visitBuiltinLenCall,
        },
    },
    {
        BUILTIN_FCT_NAME_PANIC,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinPanicCall,
            .irGeneratorVisitMethod = &IRGenerator::visitBuiltinPanicCall,
        },
    },
    {
        BUILTIN_FCT_NAME_SYSCALL,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinSyscallCall,
            .irGeneratorVisitMethod = &IRGenerator::visitBuiltinSyscallCall,
        },
    },
    {
        BUILTIN_FCT_NAME_IS_SAME,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinIsSameCall,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_IMPLEMENTS_INTERFACE,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinImplementsInterfaceCall,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_GET_BUILD_VAR,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinGetBuildVarCall,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_IS_TRIVIALLY_CONSTRUCTIBLE,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinIsTriviallyConstructible,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_IS_TRIVIALLY_COPYABLE,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinIsTriviallyCopyable,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_IS_TRIVIALLY_DESTRUCTIBLE,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinIsTriviallyDestructible,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_IS_HEAP,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinIsHeap,
            .irGeneratorVisitMethod = nullptr,
        },
    },
    {
        BUILTIN_FCT_NAME_NEW,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinNewCall,
            .irGeneratorVisitMethod = &IRGenerator::visitBuiltinNewCall,
        },
    },
    {
        BUILTIN_FCT_NAME_PLACEMENT_NEW,
        {
            .typeCheckerVisitMethod = &TypeChecker::visitBuiltinPlacementNewCall,
            .irGeneratorVisitMethod = &IRGenerator::visitBuiltinPlacementNewCall,
        },
    },
};

} // namespace spice::compiler

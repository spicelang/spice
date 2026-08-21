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
    {BUILTIN_FCT_NAME_PRINTF, {&TypeChecker::visitBuiltinPrintfCall, &IRGenerator::visitBuiltinPrintfCall}},
    {BUILTIN_FCT_NAME_SIZEOF, {&TypeChecker::visitBuiltinSizeOfCall, nullptr}},
    {BUILTIN_FCT_NAME_ALIGNOF, {&TypeChecker::visitBuiltinAlignOfCall, nullptr}},
    {BUILTIN_FCT_NAME_OFFSETOF, {&TypeChecker::visitBuiltinOffsetOfCall, nullptr}},
    {BUILTIN_FCT_NAME_TYPEID, {&TypeChecker::visitBuiltinTypeIdCall, nullptr}},
    {BUILTIN_FCT_NAME_TYPENAME, {&TypeChecker::visitBuiltinTypeNameCall, nullptr}},
    {BUILTIN_FCT_NAME_LEN, {&TypeChecker::visitBuiltinLenCall, &IRGenerator::visitBuiltinLenCall}},
    {BUILTIN_FCT_NAME_PANIC, {&TypeChecker::visitBuiltinPanicCall, &IRGenerator::visitBuiltinPanicCall}},
    {BUILTIN_FCT_NAME_SYSCALL, {&TypeChecker::visitBuiltinSyscallCall, &IRGenerator::visitBuiltinSyscallCall}},
    {BUILTIN_FCT_NAME_IS_SAME, {&TypeChecker::visitBuiltinIsSameCall, nullptr}},
    {BUILTIN_FCT_NAME_IMPLEMENTS_INTERFACE, {&TypeChecker::visitBuiltinImplementsInterfaceCall, nullptr}},
    {BUILTIN_FCT_NAME_GET_BUILD_VAR, {&TypeChecker::visitBuiltinGetBuildVarCall, nullptr}},
    {BUILTIN_FCT_NAME_IS_TRIVIALLY_CONSTRUCTIBLE, {&TypeChecker::visitBuiltinIsTriviallyConstructible, nullptr}},
    {BUILTIN_FCT_NAME_IS_TRIVIALLY_COPYABLE, {&TypeChecker::visitBuiltinIsTriviallyCopyable, nullptr}},
    {BUILTIN_FCT_NAME_IS_TRIVIALLY_DESTRUCTIBLE, {&TypeChecker::visitBuiltinIsTriviallyDestructible, nullptr}},
    {BUILTIN_FCT_NAME_IS_HEAP, {&TypeChecker::visitBuiltinIsHeap, nullptr}},
    {BUILTIN_FCT_NAME_NEW, {&TypeChecker::visitBuiltinNewCall, &IRGenerator::visitBuiltinNewCall}},
    {BUILTIN_FCT_NAME_PLACEMENT_NEW, {&TypeChecker::visitBuiltinPlacementNewCall, &IRGenerator::visitBuiltinPlacementNewCall}},
};

} // namespace spice::compiler

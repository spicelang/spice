// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#pragma warning(disable : 4129)

#define SOFT_ERROR_ER(node, type, message)                                                                                       \
  {                                                                                                                              \
    resourceManager.errorManager.addSoftError(node, type, message);                                                              \
    return ExprResult{node->setEvaluatedSymbolType(Type(TY_UNRESOLVED), manIdx)};                                          \
  }

#define SOFT_ERROR_ST(node, type, message)                                                                                       \
  {                                                                                                                              \
    resourceManager.errorManager.addSoftError(node, type, message);                                                              \
    return Type(TY_UNRESOLVED);                                                                                            \
  }

#define SOFT_ERROR_BOOL(node, type, message)                                                                                     \
  {                                                                                                                              \
    resourceManager.errorManager.addSoftError(node, type, message);                                                              \
    return false;                                                                                                                \
  }

#define SOFT_ERROR_VOID(node, type, message)                                                                                     \
  {                                                                                                                              \
    resourceManager.errorManager.addSoftError(node, type, message);                                                              \
    return;                                                                                                                      \
  }

#define HANDLE_UNRESOLVED_TYPE_ER(var)                                                                                           \
  {                                                                                                                              \
    if (var.is(TY_UNRESOLVED))                                                                                                   \
      return ExprResult{node->setEvaluatedSymbolType(Type(TY_UNRESOLVED), manIdx)};                                        \
  }

#define HANDLE_UNRESOLVED_TYPE_ST(var)                                                                                           \
  {                                                                                                                              \
    if (var.is(TY_UNRESOLVED))                                                                                                   \
      return node->setEvaluatedSymbolType(var, manIdx);                                                                          \
  }

#define HANDLE_UNRESOLVED_TYPE_PTR(var)                                                                                          \
  {                                                                                                                              \
    if (var.is(TY_UNRESOLVED))                                                                                                   \
      return nullptr;                                                                                                            \
  }

#define CHECK_ABORT_FLAG_V()                                                                                                     \
  if (resourceManager.abortCompilation)                                                                                          \
    return;

#define CHECK_ABORT_FLAG_B()                                                                                                     \
  if (resourceManager.abortCompilation)                                                                                          \
    return true;
#pragma warning(default : 4129)
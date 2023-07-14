// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

const char *const UNRESOLVED_TYPE_NAME = "unresolved type";

#pragma warning(disable : 4129)

#define SOFT_ERROR_ER(node, type, message)                                                                                       \
  {                                                                                                                              \
    resourceManager.errorManager.addSoftError(node, type, message);                                                              \
    return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_UNRESOLVED), manIdx)};                                          \
  }

#define SOFT_ERROR_ST(node, type, message)                                                                                       \
  {                                                                                                                              \
    resourceManager.errorManager.addSoftError(node, type, message);                                                              \
    return SymbolType(TY_UNRESOLVED);                                                                                            \
  }

#define SOFT_ERROR_BOOL(node, type, message)                                                                                     \
  {                                                                                                                              \
    resourceManager.errorManager.addSoftError(node, type, message);                                                              \
    return false;                                                                                                                \
  }

#define SOFT_ERROR_STR(node, type, message)                                                                                      \
  {                                                                                                                              \
    resourceManager.errorManager.addSoftError(node, type, message);                                                              \
    return UNRESOLVED_TYPE_NAME;                                                                                                 \
  }

#define HANDLE_UNRESOLVED_TYPE_ER(var)                                                                                           \
  {                                                                                                                              \
    if (var.is(TY_UNRESOLVED))                                                                                                   \
      return ExprResult{node->setEvaluatedSymbolType(SymbolType(TY_UNRESOLVED), manIdx)};                                        \
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

#pragma warning(default : 4129)
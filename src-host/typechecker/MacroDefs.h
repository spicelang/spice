// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#pragma warning(disable : 4129)

#define SOFT_ERROR_ER(node, type, message)                                                                                       \
  {                                                                                                                              \
    resourceManager.errorManager.addSoftError(node, type, message);                                                              \
    return ExprResult{QualType(TY_UNRESOLVED)};                                                                                  \
  }

#define SOFT_ERROR_QT(node, type, message)                                                                                       \
  {                                                                                                                              \
    resourceManager.errorManager.addSoftError(node, type, message);                                                              \
    return QualType(TY_UNRESOLVED);                                                                                              \
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

#define HANDLE_UNRESOLVED_TYPE_ER(type)                                                                                          \
  if ((type).is(TY_UNRESOLVED))                                                                                                  \
    return ExprResult{QualType(TY_UNRESOLVED)};

#define HANDLE_UNRESOLVED_TYPE_QT(type)                                                                                          \
  if ((type).is(TY_UNRESOLVED))                                                                                                  \
    return type;

#define HANDLE_UNRESOLVED_TYPE_PTR(type)                                                                                         \
  if ((type).is(TY_UNRESOLVED))                                                                                                  \
    return nullptr;

#define CHECK_ABORT_FLAG_V()                                                                                                     \
  if (resourceManager.abortCompilation)                                                                                          \
    return;

#define CHECK_ABORT_FLAG_B()                                                                                                     \
  if (resourceManager.abortCompilation)                                                                                          \
    return true;

#define RETURN_NULLPTR_IF_NULLPTR(value)                                                                                         \
  if (value == nullptr)                                                                                                          \
    return nullptr;

#pragma warning(default : 4129)
// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#pragma warning(disable : 4129)

#define SOFT_ERROR_ER(node, type, message)                                                                                       \
  {                                                                                                                              \
    resourceManager.errorManager.addSoftError(node, type, message);                                                              \
    if constexpr (std::is_base_of<ExprNode, decltype(node)>()) {                                                                 \
      const auto exprNode = spice_pointer_cast<ExprNode *>(node);                                                                \
      return ExprResult{exprNode->setEvaluatedSymbolType(QualType(TY_UNRESOLVED), manIdx)};                                      \
    } else {                                                                                                                     \
      return ExprResult{QualType(TY_UNRESOLVED)};                                                                                \
    }                                                                                                                            \
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
  if ((type).is(TY_UNRESOLVED)) {                                                                                                \
    if constexpr (std::is_base_of<ExprNode, decltype(node)>()) {                                                                 \
      const auto exprNode = spice_pointer_cast<ExprNode *>(node);                                                                \
      return ExprResult{exprNode->setEvaluatedSymbolType(QualType(TY_UNRESOLVED), manIdx)};                                      \
    } else {                                                                                                                     \
      return ExprResult{QualType(TY_UNRESOLVED)};                                                                                \
    }                                                                                                                            \
  }

#define HANDLE_UNRESOLVED_TYPE_QT(qualType)                                                                                      \
  if ((qualType).is(TY_UNRESOLVED)) {                                                                                            \
    if constexpr (std::is_base_of<ExprNode, decltype(node)>()) {                                                                 \
      const auto exprNode = spice_pointer_cast<ExprNode *>(node);                                                                \
      return exprNode->setEvaluatedSymbolType(qualType, manIdx);                                                                 \
    } else {                                                                                                                     \
      return qualType;                                                                                                           \
    }                                                                                                                            \
  }

#define HANDLE_UNRESOLVED_TYPE_PTR(type)                                                                                         \
  if ((type).is(TY_UNRESOLVED))                                                                                                  \
    return nullptr;

#define CHECK_ABORT_FLAG_V()                                                                                                     \
  if (resourceManager.abortCompilation)                                                                                          \
    return;

#define CHECK_ABORT_FLAG_B()                                                                                                     \
  if (resourceManager.abortCompilation)                                                                                          \
    return true;

#pragma warning(default : 4129)
// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include <symboltablebuilder/QualType.h>
#include <symboltablebuilder/Type.h>

#include "../../lib/json/json.hpp"

namespace spice::compiler {

// Typedefs
using TypeMapping = std::unordered_map</*typeName=*/std::string, /*concreteType=*/QualType>;

class GenericType : public QualType {
public:
  // Constructors
  explicit GenericType(const QualType &type) : QualType(type) {}
  explicit GenericType(const std::string &name) : QualType(TY_GENERIC, name) {}
  GenericType(const std::string &name, QualTypeList typeConditions)
      : QualType(TY_GENERIC, name), typeConditions(std::move(typeConditions)) {}
  GenericType() = default;

  // Public methods
  [[nodiscard]] bool checkConditionsOf(const QualType &requestedType, QualType &substantiation, bool ignoreArraySize = false,
                                       bool ignoreQualifiers = false) const;

  // Public members
  bool used = false;

private:
  // Members
  QualTypeList typeConditions = {QualType(TY_DYN)};

  // Private methods
  [[nodiscard]] bool checkTypeConditionOf(const QualType &requestedType, QualType &substantiation, bool ignoreArraySize,
                                          bool ignoreQualifiers) const;
};

} // namespace spice::compiler

// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

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
  explicit GenericType(const QualType &type) : QualType(type){};
  explicit GenericType(const std::string &name) : QualType(TY_GENERIC, name) {}
  GenericType(const std::string &name, const std::vector<QualType> &typeConditions)
      : QualType(TY_GENERIC, name), typeConditions(typeConditions) {}
  GenericType() = default;

  // Public methods
  [[nodiscard]] [[deprecated]] bool checkConditionsOf(const QualType &type, bool ignoreArraySize = false,
                                                      bool ignoreSpecifiers = false) const;

  // Public members
  bool used = false;

private:
  // Members
  std::vector<QualType> typeConditions = {QualType(TY_DYN)};

  // Private methods
  [[nodiscard]] [[deprecated]] bool checkTypeConditionOf(const QualType &type, bool ignoreArraySize,
                                                         bool ignoreSpecifiers) const;
};

} // namespace spice::compiler
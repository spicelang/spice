// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include <symboltablebuilder/QualType.h>
#include <symboltablebuilder/Type.h>

#include "../../lib/json/json.hpp"

namespace spice::compiler {

// Typedefs
using TypeMapping = std::unordered_map</*typeName=*/std::string, /*concreteType=*/Type>;

class GenericType : public Type {
public:
  // Constructors
  explicit GenericType(const Type &type) : Type(type){};
  explicit GenericType(const std::string &name) : Type(TY_GENERIC, name) {}
  GenericType(const std::string &name, const std::vector<Type> &typeConditions)
      : Type(TY_GENERIC, name), typeConditions(typeConditions) {}
  GenericType() = default;

  // Public methods
  [[nodiscard]] [[deprecated]] bool checkConditionsOf(const Type &type, bool ignoreArraySize = false,
                                                      bool ignoreSpecifiers = false) const;

  // Public members
  bool used = false;

private:
  // Members
  std::vector<Type> typeConditions = {Type(TY_DYN)};

  // Private methods
  [[nodiscard]] [[deprecated]] bool checkTypeConditionOf(const Type &type, bool ignoreArraySize,
                                                         bool ignoreSpecifiers) const;
};

} // namespace spice::compiler
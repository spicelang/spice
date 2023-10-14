// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include "symboltablebuilder/SymbolType.h"

#include "../../lib/json/json.hpp"

namespace spice::compiler {

// Typedefs
using TypeMapping = std::unordered_map</*typeName=*/std::string, /*concreteType=*/SymbolType>;

class GenericType : public SymbolType {
public:
  // Constructors
  explicit GenericType(const SymbolType &type) : SymbolType(type){};
  explicit GenericType(const std::string &name) : SymbolType(TY_GENERIC, name) {}
  GenericType(const std::string &name, const std::vector<SymbolType> &typeConditions)
      : SymbolType(TY_GENERIC, name), typeConditions(typeConditions) {}
  GenericType() = default;

  // Public methods
  [[nodiscard]] bool checkConditionsOf(const SymbolType &symbolType, bool ignoreArraySize = false,
                                       bool ignoreSpecifiers = false) const;

  // Public members
  bool used = false;

  // Json serializer/deserializer
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GenericType, typeChain, typeConditions)

private:
  // Members
  std::vector<SymbolType> typeConditions = {SymbolType(TY_DYN)};

  // Private methods
  [[nodiscard]] bool checkTypeConditionOf(const SymbolType &symbolType, bool ignoreArraySize, bool ignoreSpecifiers) const;
};

} // namespace spice::compiler
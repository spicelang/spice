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
  explicit GenericType(const SymbolType &type);
  explicit GenericType(const std::string &name, const SymbolSpecifiers &specifiers,
                       const std::vector<SymbolType> &typeConditions);
  explicit GenericType(const std::string &name);
  GenericType() = default;

  // Public methods
  [[nodiscard]] bool checkConditionsOf(const SymbolType &symbolType, bool ignoreArraySize = false) const;

  // Json serializer/deserializer
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GenericType, typeChain, typeConditions)

private:
  // Members
  std::vector<SymbolType> typeConditions = {SymbolType(TY_DYN)};

  // Private methods
  [[nodiscard]] bool checkTypeConditionOf(const SymbolType &symbolType, bool ignoreArraySize) const;
};

} // namespace spice::compiler
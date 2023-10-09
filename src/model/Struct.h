// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <model/GenericType.h>
#include <model/StructBase.h>

#include "../../lib/json/json.hpp"

namespace spice::compiler {

class Struct : public StructBase {
public:
  // Constructors
  Struct(std::string name, SymbolTableEntry *entry, Scope *scope, std::vector<SymbolType> fieldTypes,
         std::vector<GenericType> templateTypes, std::vector<SymbolType> interfaceTypes, ASTNode *declNode)
      : StructBase(name, entry, scope, templateTypes, declNode), fieldTypes(std::move(fieldTypes)),
        interfaceTypes(std::move(interfaceTypes)) {}

  // Public methods
  [[nodiscard]] bool hasReferenceFields() const;

  // Public members
  std::vector<SymbolType> fieldTypes;
  std::vector<SymbolType> interfaceTypes;

  // Json serializer/deserializer
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Struct, name, fieldTypes, templateTypes, interfaceTypes, genericSubstantiation, used)
};

} // namespace spice::compiler
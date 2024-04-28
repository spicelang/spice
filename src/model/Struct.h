// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

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
  Struct(std::string name, SymbolTableEntry *entry, Scope *scope, std::vector<Type> fieldTypes,
         std::vector<GenericType> templateTypes, std::vector<Type> interfaceTypes, ASTNode *declNode)
      : StructBase(std::move(name), entry, scope, std::move(templateTypes), declNode), fieldTypes(std::move(fieldTypes)),
        interfaceTypes(std::move(interfaceTypes)) {}

  // Public methods
  [[nodiscard]] bool hasReferenceFields() const;

  // Public members
  std::vector<Type> fieldTypes;
  std::vector<Type> interfaceTypes;

  // Json serializer/deserializer
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Struct, name, fieldTypes, templateTypes, interfaceTypes, genericSubstantiation, used)
};

} // namespace spice::compiler
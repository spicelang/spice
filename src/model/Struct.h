// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <model/GenericType.h>
#include <model/StructBase.h>

namespace spice::compiler {

class Struct : public StructBase {
public:
  // Constructors
  Struct(std::string name, SymbolTableEntry *entry, Scope *scope, std::vector<QualType> fieldTypes,
         std::vector<GenericType> templateTypes, std::vector<QualType> interfaceTypes, ASTNode *declNode)
      : StructBase(std::move(name), entry, scope, std::move(templateTypes), declNode), fieldTypes(std::move(fieldTypes)),
        interfaceTypes(std::move(interfaceTypes)) {}

  // Public methods
  [[nodiscard]] bool hasReferenceFields() const;

  // Public members
  std::vector<QualType> fieldTypes;
  std::vector<QualType> interfaceTypes;
};

} // namespace spice::compiler
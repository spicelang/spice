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
  Struct(std::string name, SymbolTableEntry *entry, Scope *scope, QualTypeList fieldTypes, std::vector<GenericType> templateTypes,
         QualTypeList interfaceTypes, ASTNode *declNode)
      : StructBase(std::move(name), entry, scope, std::move(templateTypes), declNode), fieldTypes(std::move(fieldTypes)),
        interfaceTypes(std::move(interfaceTypes)) {}

  // Public methods
  [[nodiscard]] bool hasReferenceFields() const;

  // Public members
  QualTypeList fieldTypes;
  QualTypeList interfaceTypes;
};

} // namespace spice::compiler
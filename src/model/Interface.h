// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <model/Function.h>
#include <model/GenericType.h>
#include <model/StructBase.h>

#include "../../lib/json/json.hpp"

namespace spice::compiler {

class Interface : public StructBase {
public:
  // Constructors
  Interface(std::string name, SymbolTableEntry *entry, Scope *scope, std::vector<Function *> methods,
            std::vector<GenericType> templateTypes, ASTNode *declNode)
      : StructBase(name, entry, scope, templateTypes, declNode), methods(std::move(methods)) {}

  // Public members
  std::vector<Function *> methods;

  // Json serializer/deserializer
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Interface, name, templateTypes, genericSubstantiation, used)
};

} // namespace spice::compiler
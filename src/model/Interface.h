// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <model/Function.h>
#include <model/GenericType.h>
#include <model/StructBase.h>

namespace spice::compiler {

class Interface : public StructBase {
public:
  // Constructors
  Interface(std::string name, SymbolTableEntry *entry, Scope *scope, std::vector<Function *> methods,
            std::vector<GenericType> templateTypes, ASTNode *declNode)
      : StructBase(std::move(name), entry, scope, std::move(templateTypes), declNode), methods(std::move(methods)) {}

  // Public methods
  [[nodiscard]] std::string getScopeName() const;
  static std::string getScopeName(const std::string &name, const QualTypeList &concreteTemplateTypes = {});

  // Public members
  std::vector<Function *> methods;
};

} // namespace spice::compiler
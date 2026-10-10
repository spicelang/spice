// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <model/StructBase.h>

namespace spice::compiler {

// Forward declarations
class Function;

class Interface : public StructBase {
public:
  // Constructors
  Interface(std::string name, SymbolTableEntry *entry, Scope *scope, std::vector<Function *> methods,
            std::vector<GenericType> templateTypes, ASTNode *declNode);

  // Public methods
  [[nodiscard]] std::string getScopeName() const;
  static std::string getScopeName(const std::string &name, const QualTypeList &concreteTemplateTypes = {});

  // Public members
  std::vector<Function *> methods;
};

} // namespace spice::compiler
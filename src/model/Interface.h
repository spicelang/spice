// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>

#include <model/Function.h>
#include <symboltablebuilder/TypeSpecifiers.h>

#include "../../lib/json/json.hpp"

namespace spice::compiler {

// Forward declaration
class Scope;
class SymbolType;
class ASTNode;
struct CodeLoc;

class Interface {
public:
  // Constructors
  Interface(std::string name, SymbolTableEntry *entry, Scope *interfaceScope, std::vector<Function *> methods,
            std::vector<GenericType> templateTypes, ASTNode *declNode)
      : name(std::move(name)), methods(std::move(methods)), templateTypes(std::move(templateTypes)), entry(entry),
        interfaceScope(interfaceScope), declNode(declNode) {}

  // Public methods
  [[nodiscard]] std::string getSignature() const;
  static std::string getSignature(const std::string &name, const std::vector<SymbolType> &concreteTemplateTypes);
  [[nodiscard]] bool hasSubstantiatedGenerics() const;
  [[nodiscard]] bool isFullySubstantiated() const;
  [[nodiscard]] std::vector<SymbolType> getTemplateTypes() const;

  // Public members
  std::string name;
  std::vector<Function *> methods;
  std::vector<GenericType> templateTypes;
  std::unordered_map<std::string, SymbolType> typeMapping;
  SymbolTableEntry *entry = nullptr;
  Scope *interfaceScope = nullptr;
  ASTNode *declNode;
  size_t manifestationIndex = 0;
  bool genericSubstantiation = false;
  bool used = false;
};

} // namespace spice::compiler
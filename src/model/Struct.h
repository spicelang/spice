// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <model/GenericType.h>
#include <symboltablebuilder/SymbolSpecifiers.h>

#include "../../lib/json/json.hpp"

namespace spice::compiler {

// Forward declaration
class Scope;
class SymbolType;
class SymbolTableEntry;
class Function;
class ASTNode;
struct CodeLoc;

class Struct {
public:
  // Constructors
  Struct(std::string name, SymbolTableEntry *entry, Scope *structScope, std::vector<SymbolType> fieldTypes,
         std::vector<GenericType> templateTypes, std::vector<SymbolType> interfaceTypes, ASTNode *declNode)
      : name(std::move(name)), entry(entry), structScope(structScope), fieldTypes(std::move(fieldTypes)),
        templateTypes(std::move(templateTypes)), interfaceTypes(std::move(interfaceTypes)), declNode(declNode) {}

  // Public methods
  [[nodiscard]] std::string getMangledName() const;
  [[nodiscard]] std::string getSignature() const;
  [[nodiscard]] bool hasSubstantiatedGenerics() const;
  [[nodiscard]] bool isFullySubstantiated() const;
  static std::string getSignature(const std::string &structName, const std::vector<SymbolType> &concreteTemplateTypes);
  //bool hasInfiniteSize(Scope *anchorScope = nullptr) const;

  // Public members
  std::string name;
  std::vector<SymbolType> fieldTypes;
  std::vector<GenericType> templateTypes;
  std::vector<SymbolType> interfaceTypes;
  SymbolTableEntry *entry = nullptr;
  Scope *structScope = nullptr;
  ASTNode *declNode;
  bool genericSubstantiation = false;
  bool used = false;

  // Json serializer/deserializer
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Struct, name, fieldTypes, templateTypes, interfaceTypes, genericSubstantiation, used)
};

} // namespace spice::compiler
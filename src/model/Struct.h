// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <model/GenericType.h>
#include <symboltablebuilder/SymbolSpecifiers.h>

#include "../../lib/json/json.hpp"

// Forward declaration
class Scope;
class SymbolType;
class Function;
class ASTNode;
struct CodeLoc;

class Struct {
public:
  // Constructors
  Struct(std::string name, SymbolSpecifiers specifiers, std::vector<SymbolType> fieldTypes,
         std::vector<GenericType> templateTypes, std::vector<SymbolType> interfaceTypes, const ASTNode *declNode)
      : name(std::move(name)), specifiers(specifiers), fieldTypes(std::move(fieldTypes)), templateTypes(std::move(templateTypes)),
        interfaceTypes(std::move(interfaceTypes)), declNode(declNode) {}

  // Public methods
  [[nodiscard]] std::string getMangledName() const;
  [[nodiscard]] std::string getSignature() const;
  [[nodiscard]] SymbolType getSymbolType() const;
  [[nodiscard]] Struct substantiateGenerics(const std::vector<SymbolType> &concreteTemplateTypes, Scope *scope) const;
  [[nodiscard]] bool hasSubstantiatedGenerics() const;
  [[nodiscard]] bool isFullySubstantiated() const;

  // Public static methods
  static std::string getSignature(const std::string &structName, const std::vector<SymbolType> &concreteTemplateTypes);

  // Public members
  std::string name;
  SymbolSpecifiers specifiers;
  std::vector<SymbolType> fieldTypes;
  std::vector<GenericType> templateTypes;
  std::vector<SymbolType> interfaceTypes;
  Scope *structScope = nullptr;
  const ASTNode *declNode;
  bool isGenericSubstantiation = false;
  bool isUsed = false;

  // Json serializer/deserializer
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Struct, name, specifiers, fieldTypes, templateTypes, interfaceTypes, isGenericSubstantiation,
                                 isUsed)
};
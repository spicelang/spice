// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <model/GenericType.h>
#include <symboltablebuilder/QualType.h>

namespace spice::compiler {

// Forward declarations
class SymbolTableEntry;
class Scope;
class ASTNode;
struct CodeLoc;

class Union {
public:
  // Constructors
  Union(std::string name, SymbolTableEntry *entry, Scope *scope, QualTypeList fieldTypes,
        std::vector<GenericType> templateTypes, ASTNode *declNode);

  // Public methods
  [[nodiscard]] std::string getSignature() const;
  static std::string getSignature(const std::string &name, const QualTypeList &concreteTemplateTypes);
  [[nodiscard]] std::string getScopeName() const;
  static std::string getScopeName(const std::string &name, const QualTypeList &concreteTemplateTypes = {});
  [[nodiscard]] bool hasSubstantiatedGenerics() const;
  [[nodiscard]] bool isFullySubstantiated() const;
  [[nodiscard]] QualTypeList getConcreteTemplateTypes() const;
  [[nodiscard]] QualTypeList getTemplateTypes() const;
  [[nodiscard]] const CodeLoc &getDeclCodeLoc() const;
  [[nodiscard]] bool isGenericSubstantiation() const;

  // Public members
  std::string name;
  std::vector<GenericType> templateTypes;
  TypeMapping typeMapping;
  SymbolTableEntry *entry = nullptr;
  Scope *scope = nullptr;
  ASTNode *declNode;
  size_t manifestationIndex = 0;
  Union *genericPreset = nullptr;
  bool used = false;
  bool isNewlyInserted = false;

  QualTypeList fieldTypes;
  // Index into fieldTypes of the field that carries the union's single allowed default value, or SIZE_MAX if none.
  size_t defaultFieldIndex = SIZE_MAX;
};

} // namespace spice::compiler

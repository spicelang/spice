// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <symbol/Function.h>
#include <symbol/GenericType.h>
#include <symbol/SymbolSpecifiers.h>

class Struct {
public:
  // Constructors
  explicit Struct(std::string name, SymbolSpecifiers specifiers, std::vector<SymbolType> fieldTypes,
                  std::vector<GenericType> templateTypes, std::string definitionCodeLoc)
      : name(name), specifiers(specifiers), fieldTypes(fieldTypes), templateTypes(templateTypes),
        definitionCodeLoc(definitionCodeLoc) {}

  // Public methods
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] SymbolSpecifiers getSpecifiers() const;
  [[nodiscard]] std::vector<SymbolType> getFieldTypes() const;
  [[nodiscard]] std::vector<GenericType> getTemplateTypes() const;
  [[nodiscard]] std::string getMangledName() const;
  [[nodiscard]] std::string getSignature() const;
  [[nodiscard]] SymbolType getSymbolType() const;
  [[nodiscard]] Struct substantiateGenerics(const std::vector<SymbolType> &concreteArgTypes) const;
  [[nodiscard]] bool isFullySubstantiated() const;
  void setUsed();
  [[nodiscard]] bool isUsed() const;
  [[nodiscard]] const std::string &getDefinitionCodeLoc() const;

private:
  std::string name;
  SymbolSpecifiers specifiers;
  std::vector<SymbolType> fieldTypes;
  std::vector<GenericType> templateTypes;
  std::vector<Function *> methods;
  std::string definitionCodeLoc;
  bool used = false;
};
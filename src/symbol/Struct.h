// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <symbol/GenericType.h>
#include <symbol/SymbolSpecifiers.h>

// Forward declaration
class SymbolTable;
class SymbolType;
class Function;
struct CodeLoc;

class Struct {
public:
  // Constructors
  explicit Struct(std::string name, SymbolSpecifiers specifiers, std::vector<SymbolType> fieldTypes,
                  std::vector<GenericType> templateTypes, const CodeLoc &declCodeLoc)
      : name(std::move(name)), specifiers(specifiers), fieldTypes(std::move(fieldTypes)), templateTypes(std::move(templateTypes)),
        declCodeLoc(declCodeLoc) {}

  // Public methods
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] SymbolSpecifiers getSpecifiers() const;
  [[nodiscard]] std::vector<SymbolType> getFieldTypes() const;
  [[nodiscard]] std::vector<GenericType> getTemplateTypes() const;
  [[nodiscard]] std::string getMangledName() const;
  [[nodiscard]] std::string getSignature() const;
  [[nodiscard]] SymbolType getSymbolType() const;
  [[nodiscard]] Struct substantiateGenerics(const std::vector<SymbolType> &concreteTemplateTypes, SymbolTable *structScope) const;
  [[nodiscard]] bool hasSubstantiatedGenerics() const;
  [[nodiscard]] bool isFullySubstantiated() const;
  void setSymbolTable(SymbolTable *symbolTable);
  void setUsed();
  [[nodiscard]] bool isUsed() const;
  [[nodiscard]] const CodeLoc &getDeclCodeLoc() const;

  // Public static methods
  static std::string getSignature(const std::string &structName, const std::vector<SymbolType> &concreteTemplateTypes);

private:
  // Members
  std::string name;
  SymbolSpecifiers specifiers;
  std::vector<SymbolType> fieldTypes;
  std::vector<GenericType> templateTypes;
  SymbolTable *symbolTable = nullptr;
  const CodeLoc &declCodeLoc;
  bool used = false;
};
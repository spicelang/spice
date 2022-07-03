// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <utility>
#include <vector>

#include <Token.h>

#include <symbol/GenericType.h>
#include <symbol/SymbolSpecifiers.h>

// Forward declaration
class SymbolTable;
class SymbolType;
class Function;

class Struct {
public:
  // Constructors
  explicit Struct(std::string name, SymbolSpecifiers specifiers, std::vector<SymbolType> fieldTypes,
                  std::vector<GenericType> templateTypes, const antlr4::Token &definitionToken)
      : name(std::move(name)), specifiers(specifiers), fieldTypes(std::move(fieldTypes)), templateTypes(std::move(templateTypes)),
        definitionToken(definitionToken) {}

  // Public methods
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] SymbolSpecifiers getSpecifiers() const;
  [[nodiscard]] std::vector<SymbolType> getFieldTypes() const;
  [[nodiscard]] std::vector<GenericType> getTemplateTypes() const;
  [[nodiscard]] std::string getMangledName() const;
  [[nodiscard]] std::string getSignature() const;
  [[nodiscard]] SymbolType getSymbolType() const;
  [[nodiscard]] Struct substantiateGenerics(const std::vector<SymbolType> &concreteTemplateTypes, SymbolTable *structScope,
                                            const antlr4::Token &token) const;
  [[nodiscard]] bool hasSubstantiatedGenerics() const;
  [[nodiscard]] bool isFullySubstantiated() const;
  void setSymbolTable(SymbolTable *symbolTable);
  void setUsed();
  [[nodiscard]] bool isUsed() const;
  [[nodiscard]] const antlr4::Token &getDefinitionToken() const;
  const std::string &getDefinitionCodeLoc();

  // Public static methods
  static std::string getSignature(const std::string &structName, const std::vector<SymbolType> &concreteTemplateTypes);

private:
  // Members
  std::string name;
  SymbolSpecifiers specifiers;
  std::vector<SymbolType> fieldTypes;
  std::vector<GenericType> templateTypes;
  SymbolTable *symbolTable = nullptr;
  const antlr4::Token &definitionToken;
  std::string definitionCodeLoc;
  bool used = false;
};
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <symbol/GenericType.h>
#include <symbol/SymbolSpecifiers.h>
#include <symbol/SymbolType.h>

typedef std::vector<std::pair<SymbolType, bool>> ArgList;
typedef std::vector<std::tuple<std::string, SymbolType, bool>> NamedArgList;

class Function {
public:
  // Constructors
  explicit Function(std::string name, SymbolSpecifiers specifiers, SymbolType thisType, SymbolType returnType,
                    std::vector<std::pair<SymbolType, bool>> argTypes, std::vector<GenericType> templateTypes,
                    const antlr4::Token &token)
      : name(std::move(name)), specifiers(specifiers), thisType(std::move(thisType)), returnType(std::move(returnType)),
        argTypes(std::move(argTypes)), templateTypes(std::move(templateTypes)), definitionToken(token) {}

  // Public methods
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] SymbolSpecifiers getSpecifiers() const;
  [[nodiscard]] SymbolType getThisType() const;
  [[nodiscard]] SymbolType getReturnType() const;
  [[nodiscard]] std::vector<SymbolType> getArgTypes() const;
  [[nodiscard]] std::vector<GenericType> getTemplateTypes() const;
  [[nodiscard]] std::string getMangledName() const;
  [[nodiscard]] std::string getSignature() const;
  [[nodiscard]] bool isFunction() const;
  [[nodiscard]] bool isProcedure() const;
  [[nodiscard]] bool isMethodFunction() const;
  [[nodiscard]] bool isMethodProcedure() const;
  [[nodiscard]] SymbolType getSymbolType() const;
  [[nodiscard]] std::vector<Function> substantiateOptionalArgs() const;
  [[nodiscard]] Function substantiateGenerics(const std::vector<SymbolType> &concreteArgTypes) const;
  [[nodiscard]] bool hasSubstantiatedArgs() const;
  [[nodiscard]] bool isFullySubstantiated() const;
  void setUsed();
  [[nodiscard]] bool isUsed() const;
  [[nodiscard]] const antlr4::Token &getDefinitionToken() const;

  std::function<void(const std::vector<GenericTypeReplacement> &)> analyzerCallback =
      [](const std::vector<GenericTypeReplacement> &) {
        throw std::runtime_error("Internal compiler error: Called empty analyzer callback");
      };

private:
  // Members
  std::string name;
  SymbolSpecifiers specifiers;
  SymbolType thisType = SymbolType(TY_DYN);
  SymbolType returnType = SymbolType(TY_DYN);
  std::vector<std::pair<SymbolType, bool>> argTypes;
  std::vector<GenericType> templateTypes;
  const antlr4::Token &definitionToken;
  bool used = false;
};
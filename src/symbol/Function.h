// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <symbol/GenericType.h>
#include <symbol/SymbolSpecifiers.h>
#include <symbol/SymbolType.h>

// Forward declarations
struct AstNode;

typedef std::vector<std::pair<SymbolType, bool>> ArgList;
typedef std::vector<std::tuple<std::string, SymbolType, bool>> NamedParamList;

class Function {
public:
  // Constructors
  explicit Function(std::string name, SymbolSpecifiers specifiers, const SymbolType &thisType, const SymbolType &returnType,
                    std::vector<std::pair<SymbolType, bool>> argTypes, std::vector<GenericType> templateTypes,
                    const AstNode *declNode)
      : name(std::move(name)), specifiers(specifiers), thisType(thisType), returnType(returnType), argList(std::move(argTypes)),
        templateTypes(std::move(templateTypes)), declNode(declNode) {}

  // Public methods
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] SymbolSpecifiers getSpecifiers() const;
  [[nodiscard]] SymbolType getThisType() const;
  [[nodiscard]] SymbolType getReturnType() const;
  [[nodiscard]] std::vector<SymbolType> getArgTypes() const;
  [[nodiscard]] ArgList getArgList() const;
  [[nodiscard]] std::string getMangledName() const;
  [[nodiscard]] std::string getSignature() const;
  [[nodiscard]] bool isFunction() const;
  [[nodiscard]] bool isProcedure() const;
  [[nodiscard]] bool isMethodFunction() const;
  [[nodiscard]] bool isMethodProcedure() const;
  [[nodiscard]] SymbolType getSymbolType() const;
  [[nodiscard]] std::vector<Function> substantiateOptionalArgs() const;
  [[nodiscard]] Function substantiateGenerics(const ArgList &concreteArgList, const SymbolType &concreteThisType,
                                              const std::map<std::string, SymbolType> &concreteGenericTypes) const;
  [[nodiscard]] bool hasSubstantiatedArgs() const;
  [[nodiscard]] bool hasSubstantiatedGenerics() const;
  [[nodiscard]] bool isFullySubstantiated() const;
  void setUsed();
  [[nodiscard]] bool isUsed() const;
  void setAnalyzed();
  [[nodiscard]] bool wasAlreadyAnalyzed() const;
  [[nodiscard]] const AstNode *getDeclNode() const;
  [[nodiscard]] const CodeLoc &getDeclCodeLoc() const;

private:
  // Members
  std::string name;
  SymbolSpecifiers specifiers;
  SymbolType thisType = SymbolType(TY_DYN);
  SymbolType returnType = SymbolType(TY_DYN);
  std::vector<std::pair<SymbolType, bool>> argList;
  std::vector<GenericType> templateTypes;
  const AstNode *declNode;
  bool used = false;
  bool alreadyAnalyzed = false;
};
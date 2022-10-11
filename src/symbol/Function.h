// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <symbol/GenericType.h>
#include <symbol/SymbolSpecifiers.h>
#include <symbol/SymbolType.h>

// Forward declarations
struct AstNode;
struct CodeLoc;

typedef std::vector<std::pair<SymbolType, bool>> ParamList;
typedef std::vector<std::tuple<std::string, SymbolType, bool>> NamedParamList;

class Function {
public:
  // Constructors
  explicit Function(std::string name, SymbolSpecifiers specifiers, const SymbolType &thisType, const SymbolType &returnType,
                    std::vector<std::pair<SymbolType, bool>> paramTypes, std::vector<GenericType> templateTypes,
                    const AstNode *declNode)
      : name(std::move(name)), specifiers(specifiers), thisType(thisType), returnType(returnType),
        paramList(std::move(paramTypes)), templateTypes(std::move(templateTypes)), declNode(declNode) {}

  // Public methods
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] SymbolSpecifiers getSpecifiers() const;
  [[nodiscard]] SymbolType getThisType() const;
  [[nodiscard]] SymbolType getReturnType() const;
  [[nodiscard]] std::vector<SymbolType> getParamTypes() const;
  [[nodiscard]] ParamList getParamList() const;
  [[nodiscard]] std::string getMangledName() const;
  [[nodiscard]] std::string getSignature() const;
  [[nodiscard]] bool isFunction() const;
  [[nodiscard]] bool isProcedure() const;
  [[nodiscard]] bool isMethodFunction() const;
  [[nodiscard]] bool isMethodProcedure() const;
  [[nodiscard]] SymbolType getSymbolType() const;
  [[nodiscard]] std::vector<Function> substantiateOptionalParams() const;
  [[nodiscard]] Function substantiateGenerics(const ParamList &concreteParamList, const SymbolType &concreteThisType,
                                              const std::map<std::string, SymbolType> &concreteGenericTypes) const;
  [[nodiscard]] bool hasSubstantiatedParams() const;
  [[nodiscard]] bool hasSubstantiatedGenerics() const;
  [[nodiscard]] bool isFullySubstantiated() const;
  [[nodiscard]] const AstNode *getDeclNode() const;
  [[nodiscard]] const CodeLoc &getDeclCodeLoc() const;

  // Public members
  bool isGenericSubstantiation = false;
  bool isAlreadyAnalyzed = false;
  bool isUsed = false;

private:
  // Members
  std::string name;
  SymbolSpecifiers specifiers;
  SymbolType thisType = SymbolType(TY_DYN);
  SymbolType returnType = SymbolType(TY_DYN);
  std::vector<std::pair<SymbolType, bool>> paramList;
  std::vector<GenericType> templateTypes;
  const AstNode *declNode;
};
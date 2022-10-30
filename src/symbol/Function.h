// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <symbol/GenericType.h>
#include <symbol/SymbolSpecifiers.h>
#include <symbol/SymbolType.h>

// Forward declarations
class ASTNode;
struct CodeLoc;

struct Param {
  SymbolType type;
  bool isOptional = false;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Param, type, isOptional)
};
struct NamedParam {
  std::string name;
  SymbolType type;
  bool isOptional = false;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(NamedParam, name, type, isOptional)
};
using ParamList = std::vector<Param>;
using NamedParamList = std::vector<NamedParam>;

class Function {
public:
  // Constructors
  Function(std::string name, SymbolSpecifiers specifiers, SymbolType thisType, SymbolType returnType, ParamList paramList,
           std::vector<GenericType> templateTypes, const ASTNode *declNode)
      : name(std::move(name)), specifiers(specifiers), thisType(std::move(thisType)), returnType(std::move(returnType)),
        paramList(std::move(paramList)), templateTypes(std::move(templateTypes)), declNode(declNode) {}
  Function() = default;

  // Public methods
  [[nodiscard]] std::vector<SymbolType> getParamTypes() const;
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
  [[nodiscard]] const CodeLoc &getDeclCodeLoc() const;

  // Public members
  std::string name;
  SymbolSpecifiers specifiers = SymbolSpecifiers(SymbolType(TY_FUNCTION));
  SymbolType thisType = SymbolType(TY_DYN);
  SymbolType returnType = SymbolType(TY_DYN);
  ParamList paramList;
  std::vector<GenericType> templateTypes;
  const ASTNode *declNode;
  bool isGenericSubstantiation = false;
  bool isAlreadyAnalyzed = false;
  bool isUsed = false;

  // Json serializer/deserializer
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Function, name, specifiers, thisType, returnType, paramList, templateTypes)
};
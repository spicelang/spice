// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <model/GenericType.h>
#include <symboltablebuilder/SymbolType.h>
#include <symboltablebuilder/TypeSpecifiers.h>

namespace spice::compiler {

// Forward declarations
class ASTNode;
struct CodeLoc;
class SymbolTableEntry;

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
  Function(std::string name, SymbolTableEntry *entry, SymbolType thisType, SymbolType returnType, ParamList paramList,
           std::vector<GenericType> templateTypes, ASTNode *declNode, bool external)
      : name(std::move(name)), entry(entry), thisType(std::move(thisType)), returnType(std::move(returnType)),
        paramList(std::move(paramList)), templateTypes(std::move(templateTypes)), declNode(declNode), external(external) {}
  Function() = default;

  // Public methods
  [[nodiscard]] std::vector<SymbolType> getParamTypes() const;
  [[nodiscard]] std::string getSignature(bool withThisType = true) const;
  [[nodiscard]] static std::string getSignature(const std::string &name, const SymbolType &thisType, const SymbolType &returnType,
                                                const ParamList &paramList, const std::vector<SymbolType> &concreteTemplateTypes,
                                                bool withThisType = true);
  [[nodiscard]] inline bool isMethod() const { return !thisType.is(TY_DYN); }
  [[nodiscard]] inline bool isFunction() const { return !returnType.is(TY_DYN); }
  [[nodiscard]] inline bool isProcedure() const { return returnType.is(TY_DYN); }
  [[nodiscard]] inline bool isNormalFunction() const { return isFunction() && !isMethod(); }
  [[nodiscard]] inline bool isNormalProcedure() const { return isProcedure() && !isMethod(); }
  [[nodiscard]] inline bool isMethodFunction() const { return isFunction() && isMethod(); }
  [[nodiscard]] inline bool isMethodProcedure() const { return isProcedure() && isMethod(); }
  [[nodiscard]] bool hasSubstantiatedParams() const;
  [[nodiscard]] bool hasSubstantiatedGenerics() const;
  [[nodiscard]] bool isFullySubstantiated() const;
  [[nodiscard]] const CodeLoc &getDeclCodeLoc() const;
  [[nodiscard]] bool isDownCall(const ASTNode *callNode) const;

  // Public members
  std::string name;
  SymbolType thisType = SymbolType(TY_DYN);
  SymbolType returnType = SymbolType(TY_DYN);
  ParamList paramList;
  std::vector<GenericType> templateTypes;
  std::unordered_map<std::string, SymbolType> typeMapping;
  SymbolTableEntry *entry = nullptr;
  ASTNode *declNode = nullptr;
  Scope *bodyScope = nullptr;
  bool mangleFunctionName = true;
  std::string predefinedMangledName;
  bool genericSubstantiation = false;
  bool alreadyTypeChecked = false;
  bool external = false;
  bool used = false;

  // Json serializer/deserializer
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Function, name, thisType, returnType, paramList, templateTypes)
};

} // namespace spice::compiler
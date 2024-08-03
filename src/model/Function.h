// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <model/GenericType.h>
#include <symboltablebuilder/Type.h>
#include <util/GlobalDefinitions.h>

#include <llvm/IR/Function.h>

namespace spice::compiler {

// Forward declarations
class ASTNode;
struct CodeLoc;
class SymbolTableEntry;

struct Param {
  QualType qualType;
  bool isOptional = false;
};
struct NamedParam {
  const char *name = nullptr;
  QualType qualType;
  bool isOptional = false;
};
using ParamList = std::vector<Param>;
using NamedParamList = std::vector<NamedParam>;

class Function {
public:
  // Constructors
  Function(std::string name, SymbolTableEntry *entry, const QualType &thisType, const QualType &returnType, ParamList paramList,
           std::vector<GenericType> templateTypes, ASTNode *declNode)
      : name(std::move(name)), thisType(thisType), returnType(returnType), paramList(std::move(paramList)),
        templateTypes(std::move(templateTypes)), entry(entry), declNode(declNode) {}
  Function() = default;

  // Public methods
  [[nodiscard]] QualTypeList getParamTypes() const;
  [[nodiscard]] std::string getSignature(bool withThisType = true, bool ignorePublic = true) const;
  [[nodiscard]] static std::string getSignature(const std::string &name, const QualType &thisType, const QualType &returnType,
                                                const ParamList &paramList, const QualTypeList &concreteTemplateTypes,
                                                bool withReturnType = true, bool withThisType = true, bool ignorePublic = true);
  [[nodiscard]] std::string getMangledName() const;
  [[nodiscard]] static std::string getSymbolTableEntryName(const std::string &functionName, const CodeLoc &codeLoc);
  [[nodiscard]] ALWAYS_INLINE bool isMethod() const { return !thisType.is(TY_DYN); }
  [[nodiscard]] ALWAYS_INLINE bool isFunction() const { return !returnType.is(TY_DYN); }
  [[nodiscard]] ALWAYS_INLINE bool isProcedure() const { return returnType.is(TY_DYN); }
  [[nodiscard]] ALWAYS_INLINE bool isNormalFunction() const { return isFunction() && !isMethod(); }
  [[nodiscard]] [[maybe_unused]] ALWAYS_INLINE bool isNormalProcedure() const { return isProcedure() && !isMethod(); }
  [[nodiscard]] [[maybe_unused]] ALWAYS_INLINE bool isMethodFunction() const { return isFunction() && isMethod(); }
  [[nodiscard]] [[maybe_unused]] ALWAYS_INLINE bool isMethodProcedure() const { return isProcedure() && isMethod(); }
  [[nodiscard]] ALWAYS_INLINE bool isVirtualMethod() const { return isMethod() && isVirtual; }
  [[nodiscard]] bool hasSubstantiatedParams() const;
  [[nodiscard]] bool hasSubstantiatedGenerics() const;
  [[nodiscard]] bool isFullySubstantiated() const;
  [[nodiscard]] bool isGenericSubstantiation() const;
  [[nodiscard]] const CodeLoc &getDeclCodeLoc() const;

  // Public members
  std::string name;
  QualType thisType = QualType(TY_DYN);
  QualType returnType = QualType(TY_DYN);
  ParamList paramList;
  std::vector<GenericType> templateTypes;
  TypeMapping typeMapping;
  SymbolTableEntry *entry = nullptr;
  ASTNode *declNode = nullptr;
  Scope *bodyScope = nullptr;
  std::string predefinedMangledName;
  std::string mangleSuffix;
  Function *genericPreset = nullptr;
  bool mangleFunctionName = true;
  bool alreadyTypeChecked = false;
  bool used = false;
  bool implicitDefault = false;
  bool isVirtual = false;
  llvm::Function *llvmFunction = nullptr;
  size_t vtableIndex = 0;
};

} // namespace spice::compiler
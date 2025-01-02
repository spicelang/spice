// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <llvm/IR/DebugInfoMetadata.h>

#include <model/GenericType.h>

namespace spice::compiler {

// Forward declarations
class Type;
class SymbolTableEntry;
class Scope;
class ASTNode;
struct CodeLoc;

class StructBase {
public:
  // Constructors
  StructBase(std::string name, SymbolTableEntry *entry, Scope *scope, std::vector<GenericType> templateTypes, ASTNode *declNode)
      : name(std::move(name)), templateTypes(std::move(templateTypes)), entry(entry), scope(scope), declNode(declNode) {}

  // Public methods
  [[nodiscard]] std::string getSignature() const;
  static std::string getSignature(const std::string &name, const QualTypeList &concreteTemplateTypes);
  [[nodiscard]] bool hasSubstantiatedGenerics() const;
  [[nodiscard]] bool isFullySubstantiated() const;
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
  StructBase *genericPreset = nullptr;
  struct {
    llvm::StructType *typeInfoType = nullptr;
    llvm::StructType *vtableType = nullptr;
    llvm::Constant *typeInfoName = nullptr;
    llvm::Constant *typeInfo = nullptr;
    llvm::Constant *vtable = nullptr;
  } vTableData;
  bool used = false;
};

} // namespace spice::compiler
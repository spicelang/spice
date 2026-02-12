// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>

#include <model/StructBase.h>

namespace spice::compiler {

// Forward declarations
enum LifecycleState : uint8_t;

class Struct : public StructBase {
public:
  // Constructors
  Struct(std::string name, SymbolTableEntry *entry, Scope *scope, QualTypeList fieldTypes, std::vector<GenericType> templateTypes,
         QualTypeList interfaceTypes, ASTNode *declNode);

  // Public methods
  [[nodiscard]] std::string getScopeName() const;
  static std::string getScopeName(const std::string &name, const QualTypeList &concreteTemplateTypes = {});
  [[nodiscard]] bool hasReferenceFields() const;
  const SymbolTableEntry *areAllFieldsInState(LifecycleState state) const;
  const SymbolTableEntry *areAllFieldsInitialized() const;
  void resetFieldSymbolsToDeclared(const ASTNode *node) const;

  // Public members
  QualTypeList fieldTypes;
  QualTypeList interfaceTypes;
};

} // namespace spice::compiler
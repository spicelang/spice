// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "Struct.h"

#include <ast/ASTNodes.h>
#include <model/GenericType.h>
#include <symboltablebuilder/Scope.h>

namespace spice::compiler {

static constexpr auto STRUCT_SCOPE_PREFIX = "struct:";

Struct::Struct(std::string name, SymbolTableEntry *entry, Scope *scope, QualTypeList fieldTypes,
               std::vector<GenericType> templateTypes, QualTypeList interfaceTypes, ASTNode *declNode)
    : StructBase(std::move(name), entry, scope, std::move(templateTypes), declNode), fieldTypes(std::move(fieldTypes)),
      interfaceTypes(std::move(interfaceTypes)) {}

/**
 * Retrieve the name of the scope, where members and methods are placed. This is used to navigate to the scope of the struct
 * from the parent scope.
 *
 * @return Name of the struct scope
 */
std::string Struct::getScopeName() const {
  const std::string &appendix = isGenericSubstantiation() ? getSignature() : name;
  return STRUCT_SCOPE_PREFIX + appendix;
}

/**
 * Retrieve the name of the scope, where members and methods are placed. This is used to navigate to the scope of the struct
 * from the parent scope.
 *
 * @return Name of the struct scope
 */
std::string Struct::getScopeName(const std::string &name, const QualTypeList &concreteTemplateTypes) {
  return STRUCT_SCOPE_PREFIX + getSignature(name, concreteTemplateTypes);
}

/**
 * Checks at least one field is a reference.
 * This is used to prohibit constant instantiations.
 *
 * @return Has reference as field type or not
 */
bool Struct::hasReferenceFields() const {
  return std::ranges::any_of(fieldTypes, [](const QualType &fieldType) { return fieldType.isRef(); });
}

/**
 * Check that all fields are in a certain lifecycle state.
 *
 * @param state Lifecycle state to check for
 * @return nullptr if all fields are in this state, otherwise the first mismatched field symbol
 */
const SymbolTableEntry *Struct::areAllFieldsInState(LifecycleState state) const {
  for (size_t i = 0; i < fieldTypes.size(); i++) {
    const SymbolTableEntry *fieldSymbol = scope->lookupField(i);
    assert(fieldSymbol != nullptr);
    if (!fieldSymbol->isImplicitField && fieldSymbol->getLifecycle().getCurrentState() != state)
      return fieldSymbol;
  }
  return nullptr;
}

/**
 * Check that all fields are initialized.
 *
 * @return nullptr if all fields are initialized, otherwise the first uninitialized field symbol
 */
const SymbolTableEntry *Struct::areAllFieldsInitialized() const { return areAllFieldsInState(INITIALIZED); }

/**
 * Reset the initialization state of all fields to DECLARED.
 *
 * @param node AST node to associate with the update
 */
void Struct::resetFieldSymbolsToDeclared(const ASTNode *node) const {
  for (size_t i = 0; i < fieldTypes.size(); i++)
    scope->lookupField(i)->updateState(DECLARED, node);
}

} // namespace spice::compiler
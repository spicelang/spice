// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "GenericType.h"

namespace spice::compiler {

GenericType::GenericType(const SymbolType &type) { this->typeChain = type.typeChain; }

GenericType::GenericType(const std::string &name, const std::vector<SymbolType> &typeConditions) {
  this->typeChain.push_back({TY_GENERIC, name, 0, {}});
  this->typeConditions = typeConditions;
}

GenericType::GenericType(const std::string &name) { this->typeChain.push_back({TY_GENERIC, name, 0, {}}); }

/**
 * Checks if the given symbol type matches all conditions to get a manifestation of the current generic type
 *
 * @param symbolType Symbol type to be checked
 * @param ignoreArraySize Ignore the array size at type comparison
 * @return True or false
 */
bool GenericType::checkConditionsOf(const SymbolType &symbolType, bool ignoreArraySize /*=false*/) const {
  return checkTypeConditionOf(symbolType, ignoreArraySize);
}

/**
 * Checks if the given symbol type matches all type conditions to get a manifestation of the current generic type
 *
 * @param symbolType Symbol type to be checked
 * @param ignoreArraySize Ignore the array size at type comparison
 * @return True or false
 */
bool GenericType::checkTypeConditionOf(const SymbolType &symbolType, bool ignoreArraySize) const {
  // Check type conditions
  return std::ranges::any_of(typeConditions, [&](const SymbolType &typeCondition) {
    if (ignoreArraySize)
      return typeCondition.is(TY_DYN) || typeCondition.equalsIgnoreArraySize(symbolType);
    else
      return typeCondition.is(TY_DYN) || typeCondition == symbolType;
  });
}

} // namespace spice::compiler
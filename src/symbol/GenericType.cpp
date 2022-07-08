// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "GenericType.h"

GenericType::GenericType(const SymbolType &type) { this->typeChain = type.getTypeChain(); }

GenericType::GenericType(const std::string &name, const std::vector<SymbolType> &typeConditions) {
  this->typeChain.push({TY_GENERIC, name, {}, false});
  this->typeConditions = typeConditions;
}

GenericType::GenericType(const std::string &name) { this->typeChain.push({TY_GENERIC, name, {}, false}); }

/**
 * Checks if the given symbol type matches all conditions to get a manifestation of the current generic type
 *
 * @return True or false
 */
bool GenericType::checkConditionsOf(const SymbolType &symbolType) const { return checkTypeConditionOf(symbolType); }

/**
 * Checks if the given symbol type matches all type conditions to get a manifestation of the current generic type
 *
 * @return True or false
 */
bool GenericType::checkTypeConditionOf(const SymbolType &symbolType) const {
  // Check type conditions
  for (const auto &typeCondition : typeConditions) {
    if (typeCondition.is(TY_DYN) || equalsIgnoreArraySizes(typeCondition, symbolType))
      return true;
  }
  return false;
}
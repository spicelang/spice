// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "GenericType.h"

namespace spice::compiler {

/**
 * Checks if the given symbol type matches all conditions to get a manifestation of the current generic type
 *
 * @param symbolType Symbol type to be checked
 * @param ignoreArraySize Ignore the array size for type comparison
 * @param ignoreSpecifiers Ignore the type specifiers for type comparison
 * @return True or false
 */
bool GenericType::checkConditionsOf(const Type &symbolType, bool ignoreArraySize /*=false*/,
                                    bool ignoreSpecifiers /*=false*/) const {
  return checkTypeConditionOf(symbolType, ignoreArraySize, ignoreSpecifiers);
}

/**
 * Checks if the given symbol type matches all type conditions to get a manifestation of the current generic type
 *
 * @param symbolType Symbol type to be checked
 * @param ignoreArraySize Ignore the array size for type comparison
 * @param ignoreSpecifiers Ignore the type specifiers for type comparison
 * @return True or false
 */
bool GenericType::checkTypeConditionOf(const Type &symbolType, bool ignoreArraySize, bool ignoreSpecifiers) const {
  // Succeed if no type conditions are set
  if (typeConditions.empty())
    return true;
  // Check type conditions
  return std::ranges::any_of(typeConditions, [&](const Type &typeCondition) {
    return typeCondition.is(TY_DYN) || typeCondition.matches(symbolType, ignoreArraySize, ignoreSpecifiers, ignoreSpecifiers);
  });
}

} // namespace spice::compiler
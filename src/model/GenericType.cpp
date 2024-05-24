// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "GenericType.h"

namespace spice::compiler {

/**
 * Checks if the given symbol qualType matches all conditions to get a manifestation of the current generic qualType
 *
 * @param qualType Qualified qualType to be checked
 * @param ignoreArraySize Ignore the array size for qualType comparison
 * @param ignoreSpecifiers Ignore the qualType specifiers for qualType comparison
 * @return True or false
 */
bool GenericType::checkConditionsOf(const QualType &qualType, bool ignoreArraySize, bool ignoreSpecifiers) const {
  return checkTypeConditionOf(qualType, ignoreArraySize, ignoreSpecifiers);
}

/**
 * Checks if the given symbol qualType matches all qualType conditions to get a manifestation of the current generic qualType
 *
 * @param qualType Qualified qualType to be checked
 * @param ignoreArraySize Ignore the array size for qualType comparison
 * @param ignoreSpecifiers Ignore the qualType specifiers for qualType comparison
 * @return True or false
 */
bool GenericType::checkTypeConditionOf(const QualType &qualType, bool ignoreArraySize, bool ignoreSpecifiers) const {
  // Succeed if no qualType conditions are set
  if (typeConditions.empty())
    return true;
  // Check type conditions
  return std::ranges::any_of(typeConditions, [&](const QualType &typeCondition) {
    return typeCondition.is(TY_DYN) || typeCondition.matches(qualType, ignoreArraySize, ignoreSpecifiers, ignoreSpecifiers);
  });
}

} // namespace spice::compiler
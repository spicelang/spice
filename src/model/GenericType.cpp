// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "GenericType.h"

namespace spice::compiler {

/**
 * Checks if the given symbol qualType matches all conditions to get a manifestation of the current generic qualType
 *
 * @param qualType Qualified qualType to be checked
 * @param ignoreArraySize Ignore the array size for qualType comparison
 * @param igoreQualifiers Ignore the type qualifiers for qualType comparison
 * @return True or false
 */
bool GenericType::checkConditionsOf(const QualType &qualType, bool ignoreArraySize, bool igoreQualifiers) const {
  return checkTypeConditionOf(qualType, ignoreArraySize, igoreQualifiers);
}

/**
 * Checks if the given qualType matches all qualType conditions to get a manifestation of the current generic qualType
 *
 * @param qualType Qualified type to be checked
 * @param ignoreArraySize Ignore the array size for qualType comparison
 * @param ignoreQualifiers Ignore the type qualifiers for qualType comparison
 * @return True or false
 */
bool GenericType::checkTypeConditionOf(const QualType &qualType, bool ignoreArraySize, bool ignoreQualifiers) const {
  // Succeed if no qualType conditions are set
  if (typeConditions.empty())
    return true;
  // Succeed if the given qual type is generic and matches the current one
  if (qualType.hasAnyGenericParts() && qualType == *this)
    return true;
  // Check type conditions
  return std::ranges::any_of(typeConditions, [&](const QualType &typeCondition) {
    return typeCondition.is(TY_DYN) || typeCondition.matches(qualType, ignoreArraySize, ignoreQualifiers, ignoreQualifiers);
  });
}

} // namespace spice::compiler
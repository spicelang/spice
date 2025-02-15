// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "GenericType.h"

namespace spice::compiler {

/**
 * Checks if the given symbol qualType matches all conditions to get a manifestation of the current generic qualType
 *
 * @param qualType Qualified qualType to be checked
 * @param ignoreArraySize Ignore the array size for qualType comparison
 * @param ignoreQualifiers Ignore the type qualifiers for qualType comparison
 * @return True or false
 */
bool GenericType::checkConditionsOf(const QualType &qualType, bool ignoreArraySize, bool ignoreQualifiers) const {
  return checkTypeConditionOf(qualType, ignoreArraySize, ignoreQualifiers);
}

/**
 * Checks if the given qualType matches all qualType conditions to get a manifestation of the current generic qualType
 *
 * @param requestedType Qualified type to be checked
 * @param ignoreArraySize Ignore the array size for qualType comparison
 * @param ignoreQualifiers Ignore the type qualifiers for qualType comparison
 * @return True or false
 */
bool GenericType::checkTypeConditionOf(const QualType &requestedType, bool ignoreArraySize, bool ignoreQualifiers) const {
  // Succeed if there are no type conditions
  if (typeConditions.empty())
    return true;
  // Succeed if the given qual type is generic and matches the current one
  if (requestedType.hasAnyGenericParts() && requestedType == *this)
    return true;
  // Check type conditions
  return std::ranges::any_of(typeConditions, [&](QualType typeCondition) {
    // If we have a dyn type condition, the conditions are fulfilled immediately
    if (typeCondition.is(TY_DYN))
      return true;

    // In situations like this we need to unwrap: requestedType = const int&, typeCondition = int
    QualType requestedTypeCopy = requestedType;
    unwrapBoth(typeCondition, requestedTypeCopy);
    return typeCondition.matches(requestedTypeCopy, ignoreArraySize, ignoreQualifiers, ignoreQualifiers);
  });
}

} // namespace spice::compiler
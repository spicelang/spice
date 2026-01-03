// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "GenericType.h"

namespace spice::compiler {

/**
 * Checks if the given symbol qualType matches all conditions to get a manifestation of the current generic qualType
 *
 * @param requestedType Qualified qualType to be checked
 * @param substantiation Concrete substantiation of the generic type
 * @param ignoreArraySize Ignore the array size for qualType comparison
 * @param ignoreQualifiers Ignore the type qualifiers for qualType comparison
 * @return True or false
 */
bool GenericType::checkConditionsOf(const QualType &requestedType, QualType &substantiation, bool ignoreArraySize,
                                    bool ignoreQualifiers) const {
  return checkTypeConditionOf(requestedType, substantiation, ignoreArraySize, ignoreQualifiers);
}

/**
 * Checks if the given qualType matches all qualType conditions to get a manifestation of the current generic qualType
 *
 * @param requestedType Qualified type to be checked
 * @param substantiation Concrete substantiation of the generic type
 * @param ignoreArraySize Ignore the array size for qualType comparison
 * @param ignoreQualifiers Ignore the type qualifiers for qualType comparison
 * @return True or false
 */
bool GenericType::checkTypeConditionOf(const QualType &requestedType, QualType &substantiation, bool ignoreArraySize,
                                       bool ignoreQualifiers) const {
  substantiation = requestedType;
  // Succeed if there are no type conditions
  if (typeConditions.empty())
    return true;
  // Succeed if the given qual type is generic and matches the current one
  if (requestedType.hasAnyGenericParts() && requestedType == *this)
    return true;
  // Check type conditions
  for (const QualType &typeCondition : typeConditions) {
    // If we have a dyn type condition, the conditions are fulfilled immediately
    if (typeCondition.is(TY_DYN))
      return true;
    // In situations like this we need to unwrap: requestedType = const int&, typeCondition = int
    QualType typeConditionCopy = typeCondition;
    QualType requestedTypeCopy = requestedType;
    unwrapBothWithRefWrappers(typeConditionCopy, requestedTypeCopy);
    if (typeConditionCopy.matches(requestedTypeCopy, ignoreArraySize, ignoreQualifiers, ignoreQualifiers)) {
      substantiation = typeCondition;
      return true;
    }
  }
  return false;
}

} // namespace spice::compiler
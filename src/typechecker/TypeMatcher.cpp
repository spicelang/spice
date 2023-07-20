// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "TypeMatcher.h"

#include <symboltablebuilder/Scope.h>

namespace spice::compiler {

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

bool TypeMatcher::matchRequestedToCandidateTypes(const std::vector<SymbolType> &candidateTypes,
                                                 const std::vector<SymbolType> &requestedTypes, TypeMapping &typeMapping,
                                                 ResolverFct &resolverFct, bool strictSpecifiers) {
  // Check if the size of template types matches
  if (requestedTypes.size() != candidateTypes.size())
    return false;

  // Loop through both lists at the same time and match each pair of template types individually
  for (size_t i = 0; i < candidateTypes.size(); i++) {
    const SymbolType &requestedType = requestedTypes.at(i);
    const SymbolType &candidateType = candidateTypes.at(i);

    // Match the pair of template types
    if (!matchRequestedToCandidateType(candidateType, requestedType, typeMapping, resolverFct, strictSpecifiers))
      return false;
  }
  return true;
}

bool TypeMatcher::matchRequestedToCandidateType(SymbolType candidateType, SymbolType requestedType, TypeMapping &typeMapping,
                                                ResolverFct &resolverFct, bool strictSpecifiers) {
  // Unwrap both types as far as possible
  while (candidateType.isSameContainerTypeAs(requestedType)) {
    requestedType = requestedType.getContainedTy();
    candidateType = candidateType.getContainedTy();
  }

  // Remove reference wrapper of candidate type if required
  if (candidateType.isRef() && !requestedType.isRef())
    candidateType = candidateType.removeReferenceWrapper();

  // Remove reference wrapper of requested type if required
  if (!candidateType.isRef() && requestedType.isRef() && !candidateType.getBaseType().is(TY_GENERIC))
    requestedType = requestedType.removeReferenceWrapper();

  // If the candidate does not contain any generic parts, we can simply check for type equality
  if (!candidateType.hasAnyGenericParts())
    return candidateType.matches(requestedType, true, !strictSpecifiers, true);

  // Check if the candidate type itself is generic
  if (candidateType.isBaseType(TY_GENERIC)) { // The candidate type itself is generic
    const std::string genericTypeName = candidateType.getBaseType().getSubType();

    // Check if we know the concrete type for that generic type name already
    if (typeMapping.contains(genericTypeName)) {
      SymbolType knownConcreteType = typeMapping.at(genericTypeName);

      // Remove reference wrapper of candidate type if required
      if (!requestedType.isRef())
        knownConcreteType = knownConcreteType.removeReferenceWrapper();

      // Check if the known concrete type matches the requested type
      return knownConcreteType.matches(requestedType, true, !strictSpecifiers, true);
    } else {
      // Retrieve generic candidate type by its name
      const GenericType *genericCandidateType = resolverFct(genericTypeName);
      assert(genericCandidateType != nullptr);

      // Check if the requested type fulfills all conditions of the generic candidate type
      if (!genericCandidateType->checkConditionsOf(requestedType, true, !strictSpecifiers))
        return false;

      // Add to type mapping
      const SymbolType newMappingType = requestedType.hasAnyGenericParts() ? candidateType : requestedType;
      typeMapping.insert({genericTypeName, newMappingType});

      return true; // The type was successfully matched, by enriching the type mapping
    }
  } else { // The candidate type itself is non-generic, but one or several template or param types are
    // Check if supertype and subtype are equal
    if (requestedType.getSuperType() != candidateType.getSuperType())
      return false;
    const bool isFctType = candidateType.isOneOf({TY_FUNCTION, TY_PROCEDURE});
    if (!isFctType && requestedType.getOriginalSubType() != candidateType.getOriginalSubType())
      return false;
    if (!isFctType && requestedType.getBodyScope()->parent != candidateType.getBodyScope()->parent)
      return false;

    // If we have a function/procedure type, check the param and return types. Otherwise, check the template types
    if (isFctType) {
      // Check param  and return types
      const std::vector<SymbolType> &candidatePRTypes = candidateType.getFunctionParamAndReturnTypes();
      const std::vector<SymbolType> &requestedPRTypes = requestedType.getFunctionParamAndReturnTypes();
      if (!matchRequestedToCandidateTypes(candidatePRTypes, requestedPRTypes, typeMapping, resolverFct, strictSpecifiers))
        return false;
    } else {
      // Check template types
      const std::vector<SymbolType> &candidateTTypes = candidateType.getTemplateTypes();
      const std::vector<SymbolType> &requestedTTypes = requestedType.getTemplateTypes();
      if (!matchRequestedToCandidateTypes(candidateTTypes, requestedTTypes, typeMapping, resolverFct, strictSpecifiers))
        return false;
    }

    return true; // All requested template types match to their respective candidate template type -> successfully matched
  }
}

void TypeMatcher::substantiateTypeWithTypeMapping(SymbolType &symbolType, const TypeMapping &typeMapping) {
  assert(symbolType.hasAnyGenericParts());

  // Check if the type itself is generic
  if (symbolType.isBaseType(TY_GENERIC)) { // The symbol type itself is generic
    const std::string genericTypeName = symbolType.getBaseType().getSubType();
    assert(typeMapping.contains(genericTypeName));
    const SymbolType &replacementType = typeMapping.at(genericTypeName);
    symbolType = symbolType.replaceBaseType(replacementType);
  } else { // The symbol type itself is non-generic, but one or several template or param types are
    if (symbolType.isOneOf({TY_FUNCTION, TY_PROCEDURE})) {
      // Substantiate each param type
      std::vector<SymbolType> paramTypes = symbolType.getFunctionParamAndReturnTypes();
      for (SymbolType &paramType : paramTypes)
        if (paramType.hasAnyGenericParts())
          substantiateTypeWithTypeMapping(paramType, typeMapping);
      // Attach the list of concrete param types to the symbol type
      symbolType.setFunctionParamAndReturnTypes(paramTypes);
    } else {
      // Substantiate each template type
      std::vector<SymbolType> templateTypes = symbolType.getBaseType().getTemplateTypes();
      for (SymbolType &templateType : templateTypes)
        if (templateType.hasAnyGenericParts())
          substantiateTypeWithTypeMapping(templateType, typeMapping);
      // Attach the list of concrete template types to the symbol type
      symbolType.setBaseTemplateTypes(templateTypes);
    }
  }
}

#pragma clang diagnostic pop

} // namespace spice::compiler
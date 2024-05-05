// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "TypeMatcher.h"

#include <symboltablebuilder/Scope.h>

namespace spice::compiler {

bool TypeMatcher::matchRequestedToCandidateTypes(const std::vector<QualType> &candidateTypes,
                                                 const std::vector<QualType> &reqTypes, TypeMapping &typeMapping,
                                                 ResolverFct &resolverFct, bool strictSpecifiers) {
  // Check if the size of template types matches
  if (reqTypes.size() != candidateTypes.size())
    return false;

  // Loop through both lists at the same time and match each pair of template types individually
  for (size_t i = 0; i < candidateTypes.size(); i++) {
    const QualType &reqType = reqTypes.at(i);
    const QualType &candidateType = candidateTypes.at(i);

    // Match the pair of template types
    if (!matchRequestedToCandidateType(candidateType, reqType, typeMapping, resolverFct, strictSpecifiers))
      return false;
  }
  return true;
}

bool TypeMatcher::matchRequestedToCandidateType(QualType candidateType, QualType requestedType, TypeMapping &typeMapping,
                                                ResolverFct &resolverFct, bool strictSpecifierMatching) {
  // Unwrap as far as possible and remove reference wrappers if possible
  QualType::unwrapBoth(candidateType, requestedType);

  // If the candidate does not contain any generic parts, we can simply check for type equality
  if (!candidateType.hasAnyGenericParts()) {
    // Check if the right one is a struct that implements the interface on the left
    if (candidateType.getType().matchesInterfaceImplementedByStruct(requestedType.getType()))
      return true;
    // Normal equality check
    return candidateType.matches(requestedType, true, !strictSpecifierMatching, true);
  }

  // Check if the candidate type itself is generic
  if (candidateType.isBase(TY_GENERIC)) { // The candidate type itself is generic
    const std::string genericTypeName = candidateType.getBase().getSubType();

    // Check if we know the concrete type for that generic type name already
    if (typeMapping.contains(genericTypeName)) { // This is a known generic type
      QualType knownConcreteType = typeMapping.at(genericTypeName);

      // Merge specifiers of candidate type and known concrete type together
      knownConcreteType.getSpecifiers() = knownConcreteType.getSpecifiers().merge(candidateType.getSpecifiers());

      // Remove reference wrapper of candidate type if required
      if (!requestedType.isRef())
        knownConcreteType = knownConcreteType.removeReferenceWrapper();

      // Check if the known concrete type matches the requested type
      return knownConcreteType.matches(requestedType, true, !strictSpecifierMatching, true);
    } else { // This is an unknown generic type
      // Retrieve generic candidate type by its name
      const GenericType *genericCandidateType = resolverFct(genericTypeName);
      assert(genericCandidateType != nullptr);

      // Check if the requested type fulfills all conditions of the generic candidate type
      if (!genericCandidateType->checkConditionsOf(requestedType, true, !strictSpecifierMatching))
        return false;

      // Zero out all specifiers in the requested type, that are present in the candidate type
      // This is to set all specifiers that are not present in the candidate type to the generic type replacement
      requestedType.getSpecifiers().eraseWithMask(candidateType.getSpecifiers());

      // Add to type mapping
      const QualType newMappingType = requestedType.hasAnyGenericParts() ? candidateType : requestedType;
      assert(newMappingType.is(TY_GENERIC) ||
             newMappingType.getSpecifiers().isSigned != newMappingType.getSpecifiers().isUnsigned);
      typeMapping.insert({genericTypeName, newMappingType});

      return true; // The type was successfully matched, by enriching the type mapping
    }
  } else { // The candidate type itself is non-generic, but one or several template or param types are
    // Check if supertype and subtype are equal
    if (requestedType.getSuperType() != candidateType.getSuperType())
      return false;

    // If we have a function/procedure type, check the param and return types. Otherwise, check the template types
    if (candidateType.isOneOf({TY_FUNCTION, TY_PROCEDURE})) {
      // Check param  and return types
      const std::vector<QualType> &candidatePRTypes = candidateType.getType().getFunctionParamAndReturnTypes();
      const std::vector<QualType> &requestedPRTypes = requestedType.getType().getFunctionParamAndReturnTypes();
      if (!matchRequestedToCandidateTypes(candidatePRTypes, requestedPRTypes, typeMapping, resolverFct, strictSpecifierMatching))
        return false;
    } else {
      if (requestedType.getSubType() != candidateType.getSubType())
        return false;
      if (requestedType.getType().getBodyScope()->parent != candidateType.getType().getBodyScope()->parent)
        return false;

      // Check template types
      const std::vector<QualType> &candidateTTypes = candidateType.getType().getTemplateTypes();
      const std::vector<QualType> &requestedTTypes = requestedType.getType().getTemplateTypes();
      if (!matchRequestedToCandidateTypes(candidateTTypes, requestedTTypes, typeMapping, resolverFct, strictSpecifierMatching))
        return false;
    }

    return true; // All requested template types match to their respective candidate template type -> successfully matched
  }
}

void TypeMatcher::substantiateTypesWithTypeMapping(std::vector<QualType> &qualTypes, const TypeMapping &typeMapping) {
  for (QualType &qualType : qualTypes)
    if (qualType.hasAnyGenericParts())
      substantiateTypeWithTypeMapping(qualType, typeMapping);
}

void TypeMatcher::substantiateTypeWithTypeMapping(QualType &type, const TypeMapping &typeMapping) { // NOLINT(*-no-recursion)
  assert(type.hasAnyGenericParts());

  // Check if the type itself is generic
  if (type.isBase(TY_GENERIC)) { // The symbol type itself is generic
    const std::string genericTypeName = type.getBase().getSubType();
    assert(typeMapping.contains(genericTypeName));
    const QualType &replacementType = typeMapping.at(genericTypeName);
    type = type.replaceBaseType(replacementType);
  } else { // The symbol type itself is non-generic, but one or several template or param types are
    if (type.getBase().isOneOf({TY_FUNCTION, TY_PROCEDURE})) {
      // Substantiate each param type
      std::vector<QualType> paramTypes = type.getType().getFunctionParamAndReturnTypes();
      for (QualType &paramType : paramTypes)
        if (paramType.hasAnyGenericParts())
          substantiateTypeWithTypeMapping(paramType, typeMapping);
      // Attach the list of concrete param types to the symbol type
      type.getType().setFunctionParamAndReturnTypes(paramTypes);
    } else {
      // Substantiate each template type
      std::vector<QualType> templateTypes = type.getBase().getType().getTemplateTypes();
      for (QualType &templateType : templateTypes)
        if (templateType.hasAnyGenericParts())
          substantiateTypeWithTypeMapping(templateType, typeMapping);
      // Attach the list of concrete template types to the symbol type
      type.getType().setBaseTemplateTypes(templateTypes);
    }
  }
}

} // namespace spice::compiler
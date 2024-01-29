// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "TypeMatcher.h"

#include <symboltablebuilder/Scope.h>

namespace spice::compiler {

bool TypeMatcher::matchRequestedToCandidateTypes(const std::vector<SymbolType> &candidateTypes,
                                                 const std::vector<SymbolType> &reqTypes, TypeMapping &typeMapping,
                                                 ResolverFct &resolverFct, bool strictSpecifiers) {
  // Check if the size of template types matches
  if (reqTypes.size() != candidateTypes.size())
    return false;

  // Loop through both lists at the same time and match each pair of template types individually
  for (size_t i = 0; i < candidateTypes.size(); i++) {
    const SymbolType &reqType = reqTypes.at(i);
    const SymbolType &candidateType = candidateTypes.at(i);

    // Match the pair of template types
    if (!matchRequestedToCandidateType(candidateType, reqType, typeMapping, resolverFct, strictSpecifiers))
      return false;
  }
  return true;
}

bool TypeMatcher::matchRequestedToCandidateType(SymbolType candidateType, SymbolType requestedType, TypeMapping &typeMapping,
                                                ResolverFct &resolverFct, bool strictSpecifierMatching,
                                                bool isRequestedValueTemporary) {
  // Unwrap as far as possible and remove reference wrappers if possible
  SymbolType::unwrapBoth(candidateType, requestedType);

  // If the candidate does not contain any generic parts, we can simply check for type equality
  if (!candidateType.hasAnyGenericParts())
    return candidateType.matches(requestedType, true, !strictSpecifierMatching, true);

  // Check if the candidate type itself is generic
  if (candidateType.isBaseType(TY_GENERIC)) { // The candidate type itself is generic
    const std::string genericTypeName = candidateType.getBaseType().getSubType();

    // Check if we know the concrete type for that generic type name already
    if (typeMapping.contains(genericTypeName)) { // This is a known generic type
      SymbolType knownConcreteType = typeMapping.at(genericTypeName);

      // Merge specifiers of candidate type and known concrete type together
      knownConcreteType.specifiers = knownConcreteType.specifiers.merge(candidateType.specifiers);

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
      requestedType.specifiers.eraseWithMask(candidateType.specifiers);

      // Add to type mapping
      const SymbolType newMappingType = requestedType.hasAnyGenericParts() ? candidateType : requestedType;
      assert(newMappingType.is(TY_GENERIC) || newMappingType.specifiers.isSigned != newMappingType.specifiers.isUnsigned);
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
      const std::vector<SymbolType> &candidatePRTypes = candidateType.getFunctionParamAndReturnTypes();
      const std::vector<SymbolType> &requestedPRTypes = requestedType.getFunctionParamAndReturnTypes();
      if (!matchRequestedToCandidateTypes(candidatePRTypes, requestedPRTypes, typeMapping, resolverFct, strictSpecifierMatching))
        return false;
    } else {
      if (requestedType.getSubType() != candidateType.getSubType())
        return false;
      if (requestedType.getBodyScope()->parent != candidateType.getBodyScope()->parent)
        return false;

      // Check template types
      const std::vector<SymbolType> &candidateTTypes = candidateType.getTemplateTypes();
      const std::vector<SymbolType> &requestedTTypes = requestedType.getTemplateTypes();
      if (!matchRequestedToCandidateTypes(candidateTTypes, requestedTTypes, typeMapping, resolverFct, strictSpecifierMatching))
        return false;
    }

    return true; // All requested template types match to their respective candidate template type -> successfully matched
  }
}

void TypeMatcher::substantiateTypesWithTypeMapping(std::vector<SymbolType> &symbolTypes, const TypeMapping &typeMapping) {
  for (SymbolType &symbolType : symbolTypes)
    if (symbolType.hasAnyGenericParts())
      substantiateTypeWithTypeMapping(symbolType, typeMapping);
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
    if (symbolType.getBaseType().isOneOf({TY_FUNCTION, TY_PROCEDURE})) {
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

} // namespace spice::compiler
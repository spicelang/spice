// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "TypeMatcher.h"

namespace spice::compiler {

bool TypeMatcher::matchRequestedToCandidateType(SymbolType candidateType, SymbolType requestedType, TypeMapping &typeMapping,
                                                std::function<const GenericType *(const std::string &)> &resolveGenericType) {
  // Unwrap both types as far as possible
  while (candidateType.isSameContainerTypeAs(requestedType)) {
    requestedType = requestedType.getContainedTy();
    candidateType = candidateType.getContainedTy();
  }

  // If the candidate does not contain any generic parts, we can simply check for type equality
  if (!candidateType.hasAnyGenericParts())
    return requestedType.equalsIgnoreArraySize(candidateType);

  // Check if the candidate type itself is generic
  if (candidateType.isBaseType(TY_GENERIC)) { // The candidate type itself is generic
    const std::string &genericTypeName = candidateType.getSubType();

    // Check if we know the concrete type for that generic type name already
    if (typeMapping.contains(genericTypeName)) {
      const SymbolType &knownConcreteType = typeMapping.at(candidateType.getSubType());
      // Check if the known concrete type matches the requested type
      return knownConcreteType != requestedType;
    } else {
      // Retrieve generic candidate type by its name
      const GenericType *genericCandidateType = resolveGenericType(genericTypeName);
      assert(genericCandidateType != nullptr);

      // Check if the requested type fulfills all conditions of the generic candidate type
      if (!genericCandidateType->checkConditionsOf(requestedType, true))
        return false;

      // Add to type mapping
      typeMapping.insert({genericTypeName, requestedType});

      return true; // The type was successfully matched, by enriching the type mapping
    }
  } else { // The candidate type itself is non-generic, but one or several template types are
    const std::vector<SymbolType> &requestedTypeTemplateTypes = requestedType.getTemplateTypes();
    const std::vector<SymbolType> &candidateTypeTemplateTypes = candidateType.getTemplateTypes();

    // Check if the size of template type matches
    if (requestedTypeTemplateTypes.size() != candidateTypeTemplateTypes.size())
      return false;

    // Loop through both lists at the same time and match each pair of template types individually
    for (size_t i = 0; i < candidateTypeTemplateTypes.size(); i++) {
      const SymbolType &requestedTypeTemplateType = requestedTypeTemplateTypes.at(i);
      const SymbolType &candidateTypeTemplateType = candidateTypeTemplateTypes.at(i);

      // Match the pair of template types
      if (!matchRequestedToCandidateType(candidateTypeTemplateType, requestedTypeTemplateType, typeMapping, resolveGenericType))
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
    symbolType = symbolType.replaceBaseType(typeMapping.at(genericTypeName));
  } else { // The symbol type itself is non-generic, but one or several template types are
    std::vector<SymbolType> templateTypes = symbolType.getBaseType().getTemplateTypes();
    // Substantiate every template type
    for (SymbolType &templateType : templateTypes)
      substantiateTypeWithTypeMapping(templateType, typeMapping);
    // Attach the list of concrete template types to the symbol type
    symbolType.setBaseTemplateTypes(templateTypes);
  }
}

} // namespace spice::compiler
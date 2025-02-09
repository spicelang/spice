// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "TypeMatcher.h"

#include <exception/SemanticError.h>
#include <symboltablebuilder/Scope.h>

namespace spice::compiler {

bool TypeMatcher::matchRequestedToCandidateTypes(const QualTypeList &candidateTypes, const QualTypeList &reqTypes,
                                                 TypeMapping &typeMapping, ResolverFct &resolverFct, bool strictQualifiers) {
  // Check if the size of template types matches
  if (reqTypes.size() != candidateTypes.size())
    return false;

  // Loop through both lists at the same time and match each pair of template types individually
  for (size_t i = 0; i < candidateTypes.size(); i++) {
    const QualType &reqType = reqTypes.at(i);
    const QualType &candidateType = candidateTypes.at(i);

    // Match the pair of template types
    if (!matchRequestedToCandidateType(candidateType, reqType, typeMapping, resolverFct, strictQualifiers))
      return false;
  }
  return true;
}

bool TypeMatcher::matchRequestedToCandidateType(QualType candidateType, QualType requestedType, TypeMapping &typeMapping,
                                                ResolverFct &resolverFct, bool strictQualifierMatching) {
  // Unwrap as far as possible and remove reference wrappers if possible
  QualType::unwrapBoth(candidateType, requestedType);

  // If the candidate does not contain any generic parts, we can simply check for type equality
  if (!candidateType.hasAnyGenericParts()) {
    // Check if the right one is a struct that implements the interface on the left
    if (candidateType.matchesInterfaceImplementedByStruct(requestedType))
      return true;
    // Normal equality check
    return candidateType.matches(requestedType, true, !strictQualifierMatching, true);
  }

  // Check if the candidate type itself is generic
  if (candidateType.isBase(TY_GENERIC)) { // The candidate type itself is generic
    const std::string genericTypeName = candidateType.getBase().getSubType();

    // Check if we know the concrete type for that generic type name already
    if (typeMapping.contains(genericTypeName)) { // This is a known generic type
      QualType knownConcreteType = typeMapping.at(genericTypeName);

      // Merge qualifiers of candidate type and known concrete type together
      const TypeQualifiers mergedQualifiers = knownConcreteType.getQualifiers().merge(candidateType.getQualifiers());
      knownConcreteType.setQualifiers(mergedQualifiers);

      // Remove reference wrapper of candidate type if required
      if (!requestedType.isRef())
        knownConcreteType = knownConcreteType.removeReferenceWrapper();

      // Check if the known concrete type matches the requested type
      return knownConcreteType.matches(requestedType, true, !strictQualifierMatching, true);
    } else { // This is an unknown generic type
      // Retrieve generic candidate type by its name
      const GenericType *genericCandidateType = resolverFct(genericTypeName);
      assert(genericCandidateType != nullptr);

      // Check if the requested type fulfills all conditions of the generic candidate type
      if (!genericCandidateType->checkConditionsOf(requestedType, true, !strictQualifierMatching))
        return false;

      // Zero out all qualifiers in the requested type, that are present in the candidate type
      // This is to set all qualifiers that are not present in the candidate type to the generic type replacement
      requestedType.getQualifiers().eraseWithMask(candidateType.getQualifiers());

      // Add to type mapping
      const QualType newMappingType = requestedType.hasAnyGenericParts() ? candidateType : requestedType;
      assert(newMappingType.is(TY_GENERIC) || newMappingType.is(TY_INVALID) ||
             newMappingType.getQualifiers().isSigned != newMappingType.getQualifiers().isUnsigned);
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
      const QualTypeList &candidatePRTypes = candidateType.getFunctionParamAndReturnTypes();
      const QualTypeList &requestedPRTypes = requestedType.getFunctionParamAndReturnTypes();
      if (!matchRequestedToCandidateTypes(candidatePRTypes, requestedPRTypes, typeMapping, resolverFct, strictQualifierMatching))
        return false;
    } else {
      if (requestedType.getSubType() != candidateType.getSubType())
        return false;
      if (requestedType.getBodyScope()->parent != candidateType.getBodyScope()->parent)
        return false;

      // Check template types
      const QualTypeList &candidateTTypes = candidateType.getTemplateTypes();
      const QualTypeList &requestedTTypes = requestedType.getTemplateTypes();
      if (!matchRequestedToCandidateTypes(candidateTTypes, requestedTTypes, typeMapping, resolverFct, strictQualifierMatching))
        return false;
    }

    return true; // All requested template types match to their respective candidate template type -> successfully matched
  }
}

void TypeMatcher::substantiateTypesWithTypeMapping(QualTypeList &qualTypes, const TypeMapping &typeMapping, const ASTNode *node) {
  for (QualType &qualType : qualTypes)
    if (qualType.hasAnyGenericParts())
      substantiateTypeWithTypeMapping(qualType, typeMapping, node);
}

void TypeMatcher::substantiateTypeWithTypeMapping(QualType &type, const TypeMapping &typeMapping, // NOLINT(*-no-recursion)
                                                  const ASTNode *node) {
  assert(type.hasAnyGenericParts());

  // Check if the type itself is generic
  if (type.isBase(TY_GENERIC)) { // The symbol type itself is generic
    const std::string genericTypeName = type.getBase().getSubType();
    assert(typeMapping.contains(genericTypeName));
    const QualType &replacementType = typeMapping.at(genericTypeName);
    type = type.replaceBaseType(replacementType);
  } else if (type.getBase().isOneOf({TY_FUNCTION, TY_PROCEDURE})) { // The base type is a function or procedure
    // Substantiate each param or return type
    QualTypeList paramAndReturnTypes = type.getFunctionParamAndReturnTypes();
    for (QualType &paramOrReturnType : paramAndReturnTypes)
      if (paramOrReturnType.hasAnyGenericParts())
        substantiateTypeWithTypeMapping(paramOrReturnType, typeMapping, node);
    // Attach the list of concrete param types to the symbol type
    type = type.getWithFunctionParamAndReturnTypes(paramAndReturnTypes);
  } else { // The base type is a struct or interface
    assert(type.getBase().isOneOf({TY_STRUCT, TY_INTERFACE}));
    // Substantiate each template type
    const QualType baseType = type.getBase();
    QualTypeList templateTypes = baseType.getTemplateTypes();
    for (QualType &templateType : templateTypes)
      if (templateType.hasAnyGenericParts())
        substantiateTypeWithTypeMapping(templateType, typeMapping, node);
    // Attach the list of concrete template types to the symbol type
    type = type.getWithBaseTemplateTypes(templateTypes);
    // Lookup the scope of the concrete struct or interface type
    // Only do this, if the struct or interface is not self-referencing, because in that case we'd end up in an infinite recursion
    if (!baseType.isSelfReferencingStructType()) {
      if (baseType.is(TY_STRUCT)) { // Struct
        const Struct *spiceStruct = baseType.getStruct(node, templateTypes);
        if (!spiceStruct) {
          assert(node != nullptr);
          const std::string signature = Struct::getSignature(baseType.getSubType(), templateTypes);
          throw SemanticError(node, UNKNOWN_DATATYPE, "Could not find struct '" + signature + "'");
        }
        type = type.getWithBodyScope(spiceStruct->scope);
      } else { // Interface
        const Interface *spiceInterface = baseType.getInterface(node, templateTypes);
        if (!spiceInterface) {
          assert(node != nullptr);
          const std::string signature = Interface::getSignature(baseType.getSubType(), templateTypes);
          throw SemanticError(node, UNKNOWN_DATATYPE, "Could not find interface '" + signature + "'");
        }
        type = type.getWithBodyScope(spiceInterface->scope);
      }
    }
  }
}

} // namespace spice::compiler
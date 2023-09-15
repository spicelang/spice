// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "StructManager.h"

#include <ast/ASTNodes.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/TypeMatcher.h>

namespace spice::compiler {

Struct *StructManager::insertStruct(Scope *insertScope, Struct &spiceStruct, std::vector<Struct *> *nodeStructList) {
  // Open a new manifestation list. Which gets filled by the substantiated manifestations of the struct
  insertScope->structs.insert({spiceStruct.declNode->codeLoc.toString(), std::unordered_map<std::string, Struct>()});

  // Save substantiation in declaration node
  Struct *substantiation = insertSubstantiation(insertScope, spiceStruct, spiceStruct.declNode);
  nodeStructList->push_back(substantiation);

  return substantiation;
}

Struct *StructManager::insertSubstantiation(Scope *insertScope, Struct &newManifestation, const ASTNode *declNode) {
  const std::string codeLocStr = declNode->codeLoc.toString();
  const std::string signature = newManifestation.getSignature();

  // Make sure that the manifestation does not exist already
  for (const auto &manifestations : insertScope->structs)
    assert(!manifestations.second.contains(signature));

  // Retrieve the matching manifestation list of the scope
  assert(insertScope->structs.contains(codeLocStr));
  StructManifestationList &manifestationList = insertScope->structs.at(codeLocStr);

  // Add substantiated struct
  newManifestation.manifestationIndex = manifestationList.size();
  manifestationList.emplace(signature, newManifestation);
  return &manifestationList.at(signature);
}

/**
 * Check if there is a struct in this scope, fulfilling all given requirements and if found, return it.
 * If more than one struct matches the requirement, an error gets thrown
 *
 * @param matchScope Scope to match against
 * @param requestedName Struct name requirement
 * @param requestedTemplateTypes Template types to substantiate generic types
 * @param node Instantiation AST node for printing error messages
 * @return Matched struct or nullptr
 */
Struct *StructManager::matchStruct(Scope *matchScope, const std::string &requestedName,
                                   const std::vector<SymbolType> &requestedTemplateTypes, const ASTNode *node) {
  // Copy the registry to prevent iterating over items, that are created within the loop
  StructRegistry structRegistry = matchScope->structs;
  // Loop over struct registry to find structs, that match the requirements of the instantiation
  std::vector<Struct *> matches;
  for (const auto &[defCodeLocStr, m] : structRegistry) {
    // Copy the manifestation list to prevent iterating over items, that are created within the loop
    const StructManifestationList manifestations = m;
    for (const auto &[mangledName, presetStruct] : manifestations) {
      // Copy the struct to be able to substantiate types
      Struct candidate = presetStruct;

      // Skip generic substantiations to prevent double matching of a struct
      if (presetStruct.genericSubstantiation)
        continue;

      // Check name requirement
      if (!matchName(candidate, requestedName))
        break; // Leave the whole manifestation list, because all manifestations in this list have the same name

      // Prepare mapping table from generic type name to concrete type
      TypeMapping &typeMapping = candidate.typeMapping;
      typeMapping.clear();
      typeMapping.reserve(candidate.templateTypes.size());

      // Check template types requirement
      if (!matchTemplateTypes(candidate, requestedTemplateTypes, typeMapping))
        continue; // Leave this manifestation and continue with the next one

      // Map field types from generic to concrete
      substantiateFieldTypes(candidate, typeMapping);

      // We found a match! -> Set the actual candidate and its entry to used
      candidate.used = true;
      candidate.entry->used = true;

      // Check if it needs to be substantiated
      if (presetStruct.templateTypes.empty()) {
        assert(matchScope->structs.contains(defCodeLocStr) && matchScope->structs.at(defCodeLocStr).contains(mangledName));
        matches.push_back(&matchScope->structs.at(defCodeLocStr).at(mangledName));
        matches.back()->used = true;
        continue; // Match was successful -> match the next struct
      }

      // Check if we already have this manifestation and can simply re-use it
      if (manifestations.contains(candidate.getSignature())) {
        matches.push_back(&matchScope->structs.at(defCodeLocStr).at(candidate.getSignature()));
        break; // Leave the whole manifestation list to not double-match the manifestation
      }

      // Insert the substantiated version if required
      Struct *substantiatedStruct = insertSubstantiation(matchScope, candidate, presetStruct.declNode);
      substantiatedStruct->genericSubstantiation = true;
      substantiatedStruct->declNode->getStructManifestations()->push_back(substantiatedStruct);

      // Copy struct entry
      const std::string newSignature = substantiatedStruct->getSignature();
      matchScope->lookupStrict(substantiatedStruct->name)->used = true;
      matchScope->symbolTable.copySymbol(substantiatedStruct->name, newSignature);
      substantiatedStruct->entry = matchScope->lookupStrict(newSignature);
      assert(substantiatedStruct->entry != nullptr);

      // Copy struct scope
      const std::string newScopeName = STRUCT_SCOPE_PREFIX + newSignature;
      matchScope->copyChildScope(STRUCT_SCOPE_PREFIX + presetStruct.name, newScopeName);
      substantiatedStruct->structScope = matchScope->getChildScope(newScopeName);
      assert(substantiatedStruct->structScope != nullptr);
      substantiatedStruct->structScope->isGenericScope = false;

      // Attach the template types to the new struct entry
      SymbolType entryType = substantiatedStruct->entry->getType();
      entryType.setTemplateTypes(substantiatedStruct->getTemplateTypes());
      entryType.setBodyScope(substantiatedStruct->structScope);
      substantiatedStruct->entry->updateType(entryType, true);

      // Replace symbol types of field entries with concrete types
      assert(substantiatedStruct->structScope != nullptr);
      for (size_t i = 0; i < substantiatedStruct->fieldTypes.size(); i++) {
        // Replace field type with concrete template type
        SymbolTableEntry *fieldEntry = substantiatedStruct->structScope->symbolTable.lookupStrictByIndex(i);
        assert(fieldEntry != nullptr && fieldEntry->isField());
        fieldEntry->updateType(substantiatedStruct->fieldTypes.at(i), /*overwriteExistingType=*/true);
      }

      // Add to matched structs
      matches.push_back(substantiatedStruct);
    }
  }

  // If no matches were found, return a nullptr
  if (matches.empty())
    return nullptr;

  // Check if more than one struct matches the requirements
  if (matches.size() > 1)
    throw SemanticError(node, STRUCT_AMBIGUITY, "Multiple structs match the requested signature");

  return matches.front();
}

/**
 * Checks if the matching candidate fulfills the name requirement
 *
 * @param candidate Matching candidate struct
 * @param requestedName Requested struct name
 * @return Fulfilled or not
 */
bool StructManager::matchName(const Struct &candidate, const std::string &requestedName) {
  return candidate.name == requestedName;
}

/**
 * Checks if the matching candidate fulfills the template types requirement
 *
 * @param candidate Matching candidate struct
 * @param requestedTemplateTypes Requested struct template types
 * @return Fulfilled or not
 */
bool StructManager::matchTemplateTypes(Struct &candidate, const std::vector<SymbolType> &requestedTemplateTypes,
                                       TypeMapping &typeMapping) {
  // Check if the number of types match
  const size_t typeCount = requestedTemplateTypes.size();
  if (typeCount != candidate.templateTypes.size())
    return false;

  // Give the type matcher a way to retrieve instances of GenericType by their name
  TypeMatcher::ResolverFct genericTypeResolver = [&](const std::string &genericTypeName) {
    return getGenericTypeOfCandidateByName(candidate, genericTypeName);
  };

  // Loop over all template types
  for (size_t i = 0; i < typeCount; i++) {
    const SymbolType &requestedType = requestedTemplateTypes.at(i);
    SymbolType &candidateType = candidate.templateTypes.at(i);

    // Check if the requested template type matches the candidate template type. The type mapping may be extended
    if (!TypeMatcher::matchRequestedToCandidateType(candidateType, requestedType, typeMapping, genericTypeResolver, false))
      return false;

    // Substantiate the candidate param type, based on the type mapping
    if (candidateType.hasAnyGenericParts())
      TypeMatcher::substantiateTypeWithTypeMapping(candidateType, typeMapping);
  }

  return true;
}

/**
 * Come up with the concrete field types, by applying the type mapping onto the generic field types
 *
 * @param candidate Candidate struct
 * @param typeMapping Generic type mapping
 */
void StructManager::substantiateFieldTypes(Struct &candidate, TypeMapping &typeMapping) {
  // Loop over all field types and substantiate the generic ones
  for (SymbolType &fieldType : candidate.fieldTypes)
    if (fieldType.hasAnyGenericParts())
      TypeMatcher::substantiateTypeWithTypeMapping(fieldType, typeMapping);
}

/**
 * Searches the candidate template types for a generic type object with a certain name and return it
 *
 * @param candidate Matching candidate struct
 * @param templateTypeName Template type name
 * @return Generic type object
 */
const GenericType *StructManager::getGenericTypeOfCandidateByName(const Struct &candidate, const std::string &templateTypeName) {
  for (const GenericType &templateType : candidate.templateTypes) {
    if (!templateType.is(TY_GENERIC))
      continue;
    if (templateType.getSubType() == templateTypeName)
      return &templateType;
  }
  return nullptr;
}

} // namespace spice::compiler
// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "StructManager.h"

#include <ast/ASTNodes.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

Struct *StructManager::insertStruct(Scope *insertScope, const Struct &spiceStruct) {
  // Open a new manifestation list. Which gets filled by the substantiated manifestations of the struct
  insertScope->structs.insert({spiceStruct.declNode->codeLoc.toString(), std::unordered_map<std::string, Struct>()});

  return insertSubstantiation(insertScope, spiceStruct, spiceStruct.declNode);
}

StructManifestationList *StructManager::getManifestationList(Scope *lookupScope, const CodeLoc &defCodeLoc) {
  const std::string codeLocStr = defCodeLoc.toString();
  return lookupScope->structs.contains(codeLocStr) ? &lookupScope->structs.at(codeLocStr) : nullptr;
}

Struct *StructManager::insertSubstantiation(Scope *insertScope, const Struct &newManifestation, const ASTNode *declNode) {
  const std::string mangledStructName = newManifestation.getMangledName();
  const std::string codeLocStr = declNode->codeLoc.toString();
  const std::string signature = newManifestation.getSignature();

  // Check if the struct exists already
  for (const auto &[_, manifestations] : insertScope->structs) {
    if (manifestations.contains(newManifestation.getMangledName()))
      throw SemanticError(declNode, STRUCT_DECLARED_TWICE, "The struct '" + signature + "' is declared twice");
  }

  // Retrieve the matching manifestation list of the scope
  assert(insertScope->structs.contains(codeLocStr));
  StructManifestationList &manifestationList = insertScope->structs.at(codeLocStr);

  // Add substantiated struct
  manifestationList.emplace(mangledStructName, newManifestation);
  return &manifestationList.at(mangledStructName);
}

/**
 * Check if there is a struct in this scope, fulfilling all given requirements and if found, return it.
 * If more than one struct matches the requirement, an error gets thrown
 *
 * @param matchScope Scope to match against
 * @param requestedName Struct name requirement
 * @param requestedTemplateTypes Template types to substantiate generic types
 * @param requestedFieldTypes Struct field types requirement
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
      candidate.typeMapping.clear();
      candidate.typeMapping.reserve(candidate.templateTypes.size());

      // Check template types requirement
      if (!matchTemplateTypes(candidate, requestedTemplateTypes, candidate.typeMapping))
        continue; // Leave this manifestation and continue with the next one

      // We found a match! -> Check if it needs to be substantiated
      if (presetStruct.templateTypes.empty()) {
        assert(matchScope->structs.contains(defCodeLocStr) && matchScope->structs.at(defCodeLocStr).contains(mangledName));
        matches.push_back(&matchScope->structs.at(defCodeLocStr).at(mangledName));
        continue; // Match was successful -> match the next struct
      }

      // Check if we already have this manifestation and can simply re-use it
      if (manifestations.contains(candidate.getMangledName())) {
        matches.push_back(&matchScope->structs.at(defCodeLocStr).at(candidate.getMangledName()));
        break; // Leave the whole manifestation list to not double-match the manifestation
      }

      // Insert the substantiated version if required
      Struct *substantiatedStruct = insertSubstantiation(matchScope, candidate, presetStruct.declNode);
      substantiatedStruct->genericSubstantiation = true;
      substantiatedStruct->declNode->getStructManifestations()->push_back(substantiatedStruct);
      // Copy struct scope
      const std::string newScopeName = STRUCT_SCOPE_PREFIX + substantiatedStruct->getSignature();
      matchScope->copyChildScope(STRUCT_SCOPE_PREFIX + presetStruct.name, newScopeName);

      // Replace field types with concrete template types
      substantiatedStruct->structScope = matchScope->getChildScope(newScopeName);
      assert(substantiatedStruct->structScope != nullptr);
      for (size_t i = 0; i < candidate.fieldTypes.size(); i++) {
        // Replace field type with concrete template type
        SymbolTableEntry *fieldEntry = substantiatedStruct->structScope->symbolTable.lookupByIndex(i);
        assert(fieldEntry != nullptr);
        fieldEntry->updateType(candidate.fieldTypes.at(i), /*overwriteExistingType=*/true);
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

  // Loop over all candidate template types and collect the concrete fill-ins
  std::vector<GenericType> concreteTemplateTypes;
  concreteTemplateTypes.reserve(typeCount);
  for (size_t i = 0; i < typeCount; i++) {
    const SymbolType &requestedType = requestedTemplateTypes.at(i);
    const GenericType &candidateType = candidate.templateTypes.at(i);

    if (candidateType.is(TY_GENERIC)) { // Type is generic
      const std::string candidateTypeName = candidateType.getSubType();

      // Check if the name is already assigned to a type
      if (typeMapping.contains(candidateTypeName) && typeMapping.at(candidateTypeName) != requestedType)
        return false; // Contradictory information

      // Check if the proposal matches the candidates conditions
      if (!candidateType.checkConditionsOf(requestedType))
        return false;

      // Add to the type mapping
      typeMapping.insert({candidateTypeName, requestedType});
    } else { // Type is non-generic
      // Check if the types match
      if (requestedType != candidateType)
        return false;
    }
    // Insert the requested type to the concrete types list
    concreteTemplateTypes.emplace_back(requestedType);
  }

  // Set the concrete type list to the candidate
  candidate.templateTypes = concreteTemplateTypes;

  // Substantiate field types
  for (SymbolType &fieldType : candidate.fieldTypes) {
    if (!fieldType.is(TY_GENERIC))
      continue;
    const std::string genericTypeName = fieldType.getSubType();
    fieldType = fieldType.replaceBaseType(typeMapping.at(genericTypeName));
  }

  return true;
}

} // namespace spice::compiler
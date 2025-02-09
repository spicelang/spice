// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "StructManager.h"

#include <ast/ASTNodes.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/TypeMatcher.h>
#include <util/CodeLoc.h>
#include <util/CustomHashFunctions.h>

namespace spice::compiler {

// Static member initialization
std::unordered_map<uint64_t, Struct *> StructManager::lookupCache = {};
size_t StructManager::lookupCacheHits = 0;
size_t StructManager::lookupCacheMisses = 0;

Struct *StructManager::insert(Scope *insertScope, Struct &spiceStruct, std::vector<Struct *> *nodeStructList) {
  // Open a new manifestation list. Which gets filled by the substantiated manifestations of the struct
  const std::string structId = spiceStruct.name + ":" + spiceStruct.declNode->codeLoc.toPrettyLineAndColumn();
  insertScope->structs.insert({structId, StructManifestationList()});

  // Save substantiation in declaration node
  Struct *substantiation = insertSubstantiation(insertScope, spiceStruct, spiceStruct.declNode);
  nodeStructList->push_back(substantiation);

  return substantiation;
}

Struct *StructManager::insertSubstantiation(Scope *insertScope, Struct &newManifestation, const ASTNode *declNode) {
  const std::string signature = newManifestation.getSignature();

#ifndef NDEBUG
  // Make sure that the manifestation does not exist already
  for (const auto &val : insertScope->structs | std::views::values)
    assert(!val.contains(signature));
#endif

  // Retrieve the matching manifestation list of the scope
  const std::string structId = newManifestation.name + ":" + declNode->codeLoc.toPrettyLineAndColumn();
  assert(insertScope->structs.contains(structId));
  StructManifestationList &manifestationList = insertScope->structs.at(structId);

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
 * @param qt Struct name requirement
 * @param reqTemplateTypes Template types to substantiate generic types
 * @param node Instantiation AST node for printing error messages
 * @return Matched struct or nullptr
 */
Struct *StructManager::match(Scope *matchScope, const std::string &qt, const QualTypeList &reqTemplateTypes,
                             const ASTNode *node) {
  // Do cache lookup
  const uint64_t cacheKey = getCacheKey(matchScope, qt, reqTemplateTypes);
  if (lookupCache.contains(cacheKey)) {
    lookupCacheHits++;
    return lookupCache.at(cacheKey);
  }
  lookupCacheMisses++;

  // Copy the registry to prevent iterating over items, that are created within the loop
  StructRegistry structRegistry = matchScope->structs;
  // Loop over struct registry to find structs, that match the requirements of the instantiation
  std::vector<Struct *> matches;
  for (const auto &[structId, m] : structRegistry) {
    // Copy the manifestation list to prevent iterating over items, that are created within the loop
    const StructManifestationList manifestations = m;
    for (const auto &[mangledName, presetStruct] : manifestations) {
      // Skip generic substantiations to prevent double matching of a struct
      if (presetStruct.isGenericSubstantiation())
        continue;

      // Copy the struct to be able to substantiate types
      Struct candidate = presetStruct;

      // Check name requirement
      if (!matchName(candidate, qt))
        break; // Leave the whole manifestation list, because all manifestations in this list have the same name

      // Prepare mapping table from generic type name to concrete type
      TypeMapping &typeMapping = candidate.typeMapping;
      typeMapping.clear();
      typeMapping.reserve(candidate.templateTypes.size());

      // Check template types requirement
      if (!matchTemplateTypes(candidate, reqTemplateTypes, typeMapping, node))
        continue; // Leave this manifestation and continue with the next one

      // Map field types from generic to concrete
      substantiateFieldTypes(candidate, typeMapping, node);

      // We found a match! -> Set the actual candidate and its entry to used
      candidate.used = true;
      candidate.entry->used = true;

      // Check if it needs to be substantiated
      if (presetStruct.templateTypes.empty()) {
        assert(matchScope->structs.contains(structId) && matchScope->structs.at(structId).contains(mangledName));
        matches.push_back(&matchScope->structs.at(structId).at(mangledName));
        matches.back()->used = true;
        continue; // Match was successful -> match the next struct
      }

      // Check if we already have this manifestation and can simply re-use it
      if (manifestations.contains(candidate.getSignature())) {
        matches.push_back(&matchScope->structs.at(structId).at(candidate.getSignature()));
        break; // Leave the whole manifestation list to not double-match the manifestation
      }

      // Insert the substantiated version if required
      Struct *substantiatedStruct = insertSubstantiation(matchScope, candidate, presetStruct.declNode);
      substantiatedStruct->genericPreset = &matchScope->structs.at(structId).at(mangledName);
      substantiatedStruct->declNode->getStructManifestations()->push_back(substantiatedStruct);

      // Copy struct entry
      const std::string newSignature = substantiatedStruct->getSignature();
      matchScope->lookupStrict(substantiatedStruct->name)->used = true;
      substantiatedStruct->entry = matchScope->symbolTable.copySymbol(substantiatedStruct->name, newSignature);
      assert(substantiatedStruct->entry != nullptr);

      // Copy struct scope
      const std::string oldScopeName = STRUCT_SCOPE_PREFIX + presetStruct.name;
      const std::string newScopeName = STRUCT_SCOPE_PREFIX + newSignature;
      substantiatedStruct->scope = matchScope->copyChildScope(oldScopeName, newScopeName);
      assert(substantiatedStruct->scope != nullptr);
      substantiatedStruct->scope->isGenericScope = false;

      // Attach the template types to the new struct entry
      QualType entryType = substantiatedStruct->entry->getQualType()
                               .getWithTemplateTypes(substantiatedStruct->getTemplateTypes())
                               .getWithBodyScope(substantiatedStruct->scope);
      substantiatedStruct->entry->updateType(entryType, true);

      // Replace symbol types of field entries with concrete types
      assert(substantiatedStruct->scope != nullptr);
      const size_t fieldCount = substantiatedStruct->fieldTypes.size();
      const size_t explicitFieldsStartIdx = substantiatedStruct->scope->getFieldCount() - fieldCount;
      for (size_t i = 0; i < fieldCount; i++) {
        // Replace field type with concrete template type
        SymbolTableEntry *fieldEntry = substantiatedStruct->scope->lookupField(explicitFieldsStartIdx + i);
        assert(fieldEntry != nullptr && fieldEntry->isField());
        QualType &fieldType = substantiatedStruct->fieldTypes.at(i);
        QualType baseType = fieldType.getBase();

        // Set the body scope of fields that are of type <candidate-struct>*
        if (baseType.matches(substantiatedStruct->entry->getQualType(), false, true, true))
          fieldType = fieldType.replaceBaseType(baseType.getWithBodyScope(substantiatedStruct->scope));

        fieldEntry->updateType(fieldType, /*overwriteExistingType=*/true);

        // Instantiate structs
        if (baseType.is(TY_STRUCT))
          (void)baseType.getStruct(node);
      }

      // Instantiate implemented interfaces if required
      for (QualType &interfaceType : substantiatedStruct->interfaceTypes) {
        // Skip non-generic interfaces
        if (!interfaceType.hasAnyGenericParts())
          continue;

        // Build template types
        QualTypeList templateTypes = interfaceType.getTemplateTypes();
        TypeMatcher::substantiateTypesWithTypeMapping(templateTypes, typeMapping, node);

        // Instantiate interface
        Interface *spiceInterface = interfaceType.getInterface(node, templateTypes);
        assert(spiceInterface != nullptr);
        interfaceType = spiceInterface->entry->getQualType();
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

  // Insert into cache
  lookupCache[cacheKey] = matches.front();

  return matches.front();
}

/**
 * Checks if the matching candidate fulfills the name requirement
 *
 * @param candidate Matching candidate struct
 * @param reqName Requested struct name
 * @return Fulfilled or not
 */
bool StructManager::matchName(const Struct &candidate, const std::string &reqName) { return candidate.name == reqName; }

/**
 * Checks if the matching candidate fulfills the template types requirement
 *
 * @param candidate Matching candidate struct
 * @param reqTemplateTypes Requested struct template types
 * @param typeMapping Generic type mapping
 * @param node Instantiation AST node for printing error messages
 * @return Fulfilled or not
 */
bool StructManager::matchTemplateTypes(Struct &candidate, const QualTypeList &reqTemplateTypes, TypeMapping &typeMapping,
                                       const ASTNode *node) {
  // Check if the number of types match
  const size_t typeCount = reqTemplateTypes.size();
  if (typeCount != candidate.templateTypes.size())
    return false;

  // Give the type matcher a way to retrieve instances of GenericType by their name
  TypeMatcher::ResolverFct genericTypeResolver = [&](const std::string &genericTypeName) {
    return getGenericTypeOfCandidateByName(candidate, genericTypeName);
  };

  // Loop over all template types
  for (size_t i = 0; i < typeCount; i++) {
    const QualType &reqType = reqTemplateTypes.at(i);
    QualType &candidateType = candidate.templateTypes.at(i);

    // Check if the requested template type matches the candidate template type. The type mapping may be extended
    if (!TypeMatcher::matchRequestedToCandidateType(candidateType, reqType, typeMapping, genericTypeResolver, false))
      return false;

    // Substantiate the candidate param type, based on the type mapping
    if (candidateType.hasAnyGenericParts())
      TypeMatcher::substantiateTypeWithTypeMapping(candidateType, typeMapping, node);
  }

  return true;
}

/**
 * Come up with the concrete field types, by applying the type mapping onto the generic field types
 *
 * @param candidate Candidate struct
 * @param typeMapping Generic type mapping
 * @param node Instantiation AST node for printing error messages
 */
void StructManager::substantiateFieldTypes(Struct &candidate, const TypeMapping &typeMapping, const ASTNode *node) {
  // Loop over all implicit fields and substantiate the generic ones
  const size_t fieldCount = candidate.scope->getFieldCount() - candidate.fieldTypes.size();
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldEntry = candidate.scope->lookupField(i);
    QualType fieldType = fieldEntry->getQualType();
    if (fieldType.hasAnyGenericParts()) {
      TypeMatcher::substantiateTypeWithTypeMapping(fieldType, typeMapping, node);
      fieldEntry->updateType(fieldType, true);
    }
  }

  // Loop over all explicit field types and substantiate the generic ones
  for (QualType &fieldType : candidate.fieldTypes)
    if (fieldType.hasAnyGenericParts())
      TypeMatcher::substantiateTypeWithTypeMapping(fieldType, typeMapping, node);
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

/**
 * Calculate the cache key for the struct lookup cache
 *
 * @param scope Scope to match against
 * @param name Struct name requirement
 * @param templateTypes Template types to substantiate generic types
 * @return Cache key
 */
uint64_t StructManager::getCacheKey(Scope *scope, const std::string &name, const QualTypeList &templateTypes) {
  const auto pred = [](size_t acc, const QualType &val) {
    // Combine the previous hash value with the current element's hash, adjusted by a prime number to reduce collisions
    return acc * 31 + std::hash<QualType>{}(val);
  };
  // Calculate the cache key
  const uint64_t scopeHash = std::hash<Scope *>{}(scope);
  const uint64_t hashName = std::hash<std::string>{}(name);
  const uint64_t hashTemplateTypes = std::accumulate(templateTypes.begin(), templateTypes.end(), 0u, pred);
  return scopeHash ^ (hashName << 1) ^ (hashTemplateTypes << 2);
}

/**
 * Clear the lookup cache
 */
void StructManager::cleanup() {
  lookupCache.clear();
  lookupCacheHits = 0;
  lookupCacheMisses = 0;
}

/**
 * Dump usage statistics for the lookup cache
 */
std::string StructManager::dumpLookupCacheStatistics() {
  std::stringstream stats;
  stats << "StructManager lookup cache statistics:" << std::endl;
  stats << "  lookup cache entries: " << lookupCache.size() << std::endl;
  stats << "  lookup cache hits: " << lookupCacheHits << std::endl;
  stats << "  lookup cache misses: " << lookupCacheMisses << std::endl;
  return stats.str();
}

} // namespace spice::compiler
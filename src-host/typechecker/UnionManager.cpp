// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "UnionManager.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <exception/SemanticError.h>
#include <model/GenericType.h>
#include <model/Union.h>
#include <symboltablebuilder/Scope.h>
#include <typechecker/TypeMatcher.h>
#include <util/CodeLoc.h>
#include <util/Concurrency.h>
#include <util/CustomHashFunctions.h>

namespace spice::compiler {

// Static member initialization
std::unordered_map<uint64_t, Union *> UnionManager::lookupCache = {};
size_t UnionManager::lookupCacheHits = 0;
size_t UnionManager::lookupCacheMisses = 0;

Union *UnionManager::insert(Scope *insertScope, Union &spiceUnion, std::vector<Union *> *nodeUnionList) {
  // Open a new manifestation list. Which gets filled by the substantiated manifestations of the union
  const std::string unionId = spiceUnion.name + ":" + spiceUnion.declNode->codeLoc.toPrettyLineAndColumn();
  insertScope->unions.emplace(unionId, UnionManifestationList());

  // Save substantiation in declaration node
  Union *substantiation = insertSubstantiation(insertScope, spiceUnion, spiceUnion.declNode);
  nodeUnionList->push_back(substantiation);

  return substantiation;
}

Union *UnionManager::insertSubstantiation(Scope *insertScope, Union &newManifestation, const ASTNode *declNode) {
  const std::string signature = newManifestation.getSignature();

#ifndef NDEBUG
  // Make sure that the manifestation does not exist already
  for (const auto &val : insertScope->unions | std::views::values)
    assert(!val.contains(signature));
#endif

  // Retrieve the matching manifestation list of the scope
  const std::string unionId = newManifestation.name + ":" + declNode->codeLoc.toPrettyLineAndColumn();
  assert(insertScope->unions.contains(unionId));
  UnionManifestationList &manifestationList = insertScope->unions.at(unionId);

  // Add substantiated union
  newManifestation.manifestationIndex = manifestationList.size();
  manifestationList.emplace(signature, newManifestation);
  return &manifestationList.at(signature);
}

/**
 * Check if there is a union in this scope, fulfilling all given requirements and if found, return it.
 * If more than one union matches the requirement, an error gets thrown
 *
 * @param matchScope Scope to match against
 * @param qt Union name requirement
 * @param reqTemplateTypes Template types to substantiate generic types
 * @param node Instantiation AST node for printing error messages
 * @return Matched union or nullptr
 */
Union *UnionManager::match(Scope *matchScope, const std::string &qt, const QualTypeList &reqTemplateTypes, const ASTNode *node) {
  // The IR generator lowers union types through QualType::getUnion, so this may run on multiple threads at once. The
  // lock is recursive, because matching a union recurses into matching its field types.
  const ConditionalLock lock(symbolRegistryMutex);

  // Do cache lookup
  const uint64_t cacheKey = getCacheKey(matchScope, qt, reqTemplateTypes);
  if (const auto it = lookupCache.find(cacheKey); it != lookupCache.end()) {
    lookupCacheHits++;
    return it->second;
  }
  lookupCacheMisses++;

  // Loop over union registry to find unions, that match the requirements of the instantiation
  std::vector<Union *> matches;
  for (auto &[unionId, manifestations] : matchScope->unions) {
    for (const auto &[mangledName, presetUnion] : manifestations) {
      // Skip generic and newly inserted substantiations to prevent double matching of a union
      if (presetUnion.isGenericSubstantiation() || presetUnion.isNewlyInserted)
        continue;

      // Copy the union to be able to substantiate types
      Union candidate = presetUnion;

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
      if (presetUnion.templateTypes.empty()) {
        assert(matchScope->unions.contains(unionId) && matchScope->unions.at(unionId).contains(mangledName));
        Union *match = &matchScope->unions.at(unionId).at(mangledName);
        match->used = true;
        matches.push_back(match);
        continue; // Match was successful -> match the next union
      }

      // Check if we already have this manifestation and can simply re-use it
      if (const auto it = manifestations.find(candidate.getSignature()); it != manifestations.end()) {
        it->second.used = true;
        matches.push_back(&it->second);
        break; // Leave the whole manifestation list to not double-match the manifestation
      }

      // Insert the substantiated version if required
      Union *substantiatedUnion = insertSubstantiation(matchScope, candidate, presetUnion.declNode);
      substantiatedUnion->genericPreset = &matchScope->unions.at(unionId).at(mangledName);
      substantiatedUnion->declNode->getUnionManifestations()->push_back(substantiatedUnion);
      substantiatedUnion->isNewlyInserted = true; // To not iterate over it in the same matching

      // Copy union entry
      const std::string newSignature = substantiatedUnion->getSignature();
      matchScope->lookupStrict(substantiatedUnion->name)->used = true;
      substantiatedUnion->entry = matchScope->symbolTable.copySymbol(substantiatedUnion->name, newSignature);
      assert(substantiatedUnion->entry != nullptr);

      // Copy union scope
      const std::string &oldScopeName = presetUnion.getScopeName();
      const std::string &newScopeName = substantiatedUnion->getScopeName();
      substantiatedUnion->scope = matchScope->copyChildScope(oldScopeName, newScopeName);
      assert(substantiatedUnion->scope != nullptr);
      substantiatedUnion->scope->isGenericScope = false;

      // Attach the template types to the new union entry
      QualType entryType = substantiatedUnion->entry->getQualType()
                               .getWithTemplateTypes(substantiatedUnion->getTemplateTypes())
                               .getWithBodyScope(substantiatedUnion->scope);
      substantiatedUnion->entry->updateType(entryType, true);

      // Replace symbol types of field entries with concrete types
      assert(substantiatedUnion->scope != nullptr);
      const size_t fieldCount = substantiatedUnion->fieldTypes.size();
      const size_t explicitFieldsStartIdx = substantiatedUnion->scope->getFieldCount() - fieldCount;
      for (size_t i = 0; i < fieldCount; i++) {
        // Replace field type with concrete template type
        SymbolTableEntry *fieldEntry = substantiatedUnion->scope->lookupField(explicitFieldsStartIdx + i);
        assert(fieldEntry != nullptr && fieldEntry->isField());
        QualType &fieldType = substantiatedUnion->fieldTypes.at(i);
        QualType baseType = fieldType.getBase();

        // Set the body scope of fields that are of type <candidate-union>*
        if (baseType.matches(substantiatedUnion->entry->getQualType(), false, true, true))
          fieldType = fieldType.replaceBaseType(baseType.getWithBodyScope(substantiatedUnion->scope));

        fieldEntry->updateType(fieldType, /*overwriteExistingType=*/true);

        // Instantiate structs/unions nested by value
        if (baseType.is(TY_STRUCT))
          (void)baseType.getStruct(node);
        else if (baseType.is(TY_UNION))
          (void)baseType.getUnion(node);
      }

      // Add to matched unions
      matches.push_back(substantiatedUnion);
    }
  }

  // If no matches were found, return a nullptr
  if (matches.empty())
    return nullptr;

  // Check if more than one union matches the requirements
  if (matches.size() > 1)
    throw SemanticError(node, UNION_AMBIGUITY, "Multiple unions match the requested signature");
  Union *matchedUnion = matches.front();
  matchedUnion->isNewlyInserted = false;

  // Insert into cache
  lookupCache[cacheKey] = matchedUnion;

  return matchedUnion;
}

/**
 * Checks if the matching candidate fulfills the name requirement
 *
 * @param candidate Matching candidate union
 * @param reqName Requested union name
 * @return Fulfilled or not
 */
bool UnionManager::matchName(const Union &candidate, const std::string &reqName) { return candidate.name == reqName; }

/**
 * Checks if the matching candidate fulfills the template types requirement
 *
 * @param candidate Matching candidate union
 * @param reqTemplateTypes Requested union template types
 * @param typeMapping Generic type mapping
 * @param node Instantiation AST node for printing error messages
 * @return Fulfilled or not
 */
bool UnionManager::matchTemplateTypes(Union &candidate, const QualTypeList &reqTemplateTypes, TypeMapping &typeMapping,
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
 * @param candidate Candidate union
 * @param typeMapping Generic type mapping
 * @param node Instantiation AST node for printing error messages
 */
void UnionManager::substantiateFieldTypes(Union &candidate, const TypeMapping &typeMapping, const ASTNode *node) {
  // Loop over all explicit field types and substantiate the generic ones
  for (QualType &fieldType : candidate.fieldTypes)
    if (fieldType.hasAnyGenericParts())
      TypeMatcher::substantiateTypeWithTypeMapping(fieldType, typeMapping, node);
}

/**
 * Searches the candidate template types for a generic type object with a certain name and return it
 *
 * @param candidate Matching candidate union
 * @param templateTypeName Template type name
 * @return Generic type object
 */
const GenericType *UnionManager::getGenericTypeOfCandidateByName(const Union &candidate, const std::string &templateTypeName) {
  for (const GenericType &templateType : candidate.templateTypes) {
    if (!templateType.is(TY_GENERIC))
      continue;
    if (templateType.getSubType() == templateTypeName)
      return &templateType;
  }
  return nullptr;
}

/**
 * Calculate the cache key for the union lookup cache
 *
 * @param scope Scope to match against
 * @param name Union name requirement
 * @param templateTypes Template types to substantiate generic types
 * @return Cache key
 */
uint64_t UnionManager::getCacheKey(const Scope *scope, const std::string &name, const QualTypeList &templateTypes) {
  uint64_t hash = 0;
  hashCombine64(hash, hashPointer(scope));
  hashCombine64(hash, std::hash<std::string>{}(name));
  hashCombine64(hash, hashVector(templateTypes));
  return hash;
}

/**
 * Clear the lookup cache
 */
void UnionManager::cleanup() {
  lookupCache.clear();
  lookupCacheHits = 0;
  lookupCacheMisses = 0;
}

/**
 * Dump usage statistics for the lookup cache
 */
std::string UnionManager::dumpLookupCacheStatistics() {
  std::stringstream stats;
  stats << "UnionManager lookup cache statistics:" << std::endl;
  stats << "  lookup cache entries: " << lookupCache.size() << std::endl;
  stats << "  lookup cache hits: " << lookupCacheHits << std::endl;
  stats << "  lookup cache misses: " << lookupCacheMisses << std::endl;
  return stats.str();
}

} // namespace spice::compiler

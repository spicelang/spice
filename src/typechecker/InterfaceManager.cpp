// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "InterfaceManager.h"

#include <ast/ASTNodes.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/TypeMatcher.h>
#include <util/CustomHashFunctions.h>

namespace spice::compiler {

// Static member initialization
std::unordered_map<uint64_t, Interface *> InterfaceManager::lookupCache = {};
size_t InterfaceManager::lookupCacheHits = 0;
size_t InterfaceManager::lookupCacheMisses = 0;

Interface *InterfaceManager::insert(Scope *insertScope, Interface &spiceInterface, std::vector<Interface *> *nodeInterfaceList) {
  // Open a new manifestation list. Which gets filled by the substantiated manifestations of the interface
  insertScope->interfaces.insert({spiceInterface.declNode->codeLoc, InterfaceManifestationList()});

  // Save substantiation in declaration node
  Interface *substantiation = insertSubstantiation(insertScope, spiceInterface, spiceInterface.declNode);
  nodeInterfaceList->push_back(substantiation);

  return substantiation;
}

Interface *InterfaceManager::insertSubstantiation(Scope *insertScope, Interface &newManifestation, const ASTNode *declNode) {
  const std::string signature = newManifestation.getSignature();

  // Make sure that the manifestation does not exist already
  for ([[maybe_unused]] const auto &manifestations : insertScope->interfaces)
    assert(!manifestations.second.contains(newManifestation.getSignature()));

  // Retrieve the matching manifestation list of the scope
  assert(insertScope->interfaces.contains(declNode->codeLoc));
  InterfaceManifestationList &manifestationList = insertScope->interfaces.at(declNode->codeLoc);

  // Add substantiated interface
  newManifestation.manifestationIndex = manifestationList.size();
  manifestationList.emplace(signature, newManifestation);
  return &manifestationList.at(signature);
}

/**
 * Check if there is an interface in this scope, fulfilling all given requirements and if found, return it.
 * If more than one interface matches the requirement, an error gets thrown
 *
 * @param matchScope Scope to match against
 * @param reqName Interface name requirement
 * @param reqTemplateTypes Template types to substantiate generic types
 * @param node Instantiation AST node for printing error messages
 * @return Matched interface or nullptr
 */
Interface *InterfaceManager::match(Scope *matchScope, const std::string &reqName, const QualTypeList &reqTemplateTypes,
                                   const ASTNode *node) {
  // Do cache lookup
  const uint64_t cacheKey = getCacheKey(matchScope, reqName, reqTemplateTypes);
  if (lookupCache.contains(cacheKey)) {
    lookupCacheHits++;
    return lookupCache.at(cacheKey);
  }
  lookupCacheMisses++;

  // Copy the registry to prevent iterating over items, that are created within the loop
  InterfaceRegistry interfaceRegistry = matchScope->interfaces;
  // Loop over interface registry to find interfaces, that match the requirements of the instantiation
  std::vector<Interface *> matches;
  for (const auto &[defCodeLocStr, m] : interfaceRegistry) {
    // Copy the manifestation list to prevent iterating over items, that are created within the loop
    const InterfaceManifestationList manifestations = m;
    for (const auto &[mangledName, presetInterface] : manifestations) {
      // Skip generic substantiations to prevent double matching of an interface
      if (presetInterface.isGenericSubstantiation())
        continue;

      // Copy the interface to be able to substantiate types
      Interface candidate = presetInterface;

      // Check name requirement
      if (!matchName(candidate, reqName))
        break; // Leave the whole manifestation list, because all manifestations in this list have the same name

      // Prepare mapping table from generic type name to concrete type
      TypeMapping &typeMapping = candidate.typeMapping;
      typeMapping.clear();
      typeMapping.reserve(candidate.templateTypes.size());

      // Check template types requirement
      if (!matchTemplateTypes(candidate, reqTemplateTypes, typeMapping, node))
        continue; // Leave this manifestation and continue with the next one

      // Map signatures from generic to concrete
      substantiateSignatures(candidate, typeMapping, node);

      // We found a match! -> Set the actual candidate and its entry to used
      candidate.used = true;
      candidate.entry->used = true;

      // Check if it needs to be substantiated
      if (presetInterface.templateTypes.empty()) {
        assert(matchScope->interfaces.contains(defCodeLocStr) && matchScope->interfaces.at(defCodeLocStr).contains(mangledName));
        matches.push_back(&matchScope->interfaces.at(defCodeLocStr).at(mangledName));
        matches.back()->used = true;
        continue; // Match was successful -> match the next interface
      }

      // Check if we already have this manifestation and can simply re-use it
      if (manifestations.contains(candidate.getSignature())) {
        matches.push_back(&matchScope->interfaces.at(defCodeLocStr).at(candidate.getSignature()));
        break; // Leave the whole manifestation list to not double-match the manifestation
      }

      // Insert the substantiated version if required
      Interface *substantiatedInterface = insertSubstantiation(matchScope, candidate, presetInterface.declNode);
      substantiatedInterface->genericPreset = &matchScope->interfaces.at(defCodeLocStr).at(mangledName);
      substantiatedInterface->declNode->getInterfaceManifestations()->push_back(substantiatedInterface);

      // Copy interface entry
      const std::string &newSignature = substantiatedInterface->getSignature();
      matchScope->lookupStrict(substantiatedInterface->name)->used = true;
      substantiatedInterface->entry = matchScope->symbolTable.copySymbol(substantiatedInterface->name, newSignature);
      assert(substantiatedInterface->entry != nullptr);

      // Copy interface scope
      const std::string &oldScopeName = presetInterface.getScopeName();
      const std::string &newScopeName = substantiatedInterface->getScopeName();
      matchScope->copyChildScope(oldScopeName, newScopeName);
      substantiatedInterface->scope = matchScope->getChildScope(newScopeName);
      assert(substantiatedInterface->scope != nullptr);
      substantiatedInterface->scope->isGenericScope = false;

      // Attach the template types to the new interface entry
      QualType entryType = substantiatedInterface->entry->getQualType()
                               .getWithTemplateTypes(substantiatedInterface->getTemplateTypes())
                               .getWithBodyScope(substantiatedInterface->scope);
      substantiatedInterface->entry->updateType(entryType, true);

      // Add to matched interfaces
      matches.push_back(substantiatedInterface);
    }
  }

  // If no matches were found, return a nullptr
  if (matches.empty())
    return nullptr;

  // Check if more than one interface matches the requirements
  if (matches.size() > 1)
    throw SemanticError(node, INTERFACE_AMBIGUITY, "Multiple interfaces match the requested signature");

  // Insert into cache
  lookupCache[cacheKey] = matches.front();

  return matches.front();
}

/**
 * Checks if the matching candidate fulfills the name requirement
 *
 * @param candidate Matching candidate interface
 * @param reqName Requested interface name
 * @return Fulfilled or not
 */
bool InterfaceManager::matchName(const Interface &candidate, const std::string &reqName) { return candidate.name == reqName; }

/**
 * Checks if the matching candidate fulfills the template types requirement
 *
 * @param candidate Matching candidate interface
 * @param reqTemplateTypes Requested interface template types
 * @param typeMapping Generic type mapping
 * @param node Instantiation AST node for printing error messages
 * @return Fulfilled or not
 */
bool InterfaceManager::matchTemplateTypes(Interface &candidate, const QualTypeList &reqTemplateTypes, TypeMapping &typeMapping,
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
 * Come up with the concrete signatures, by applying the type mapping onto the generic signatures
 *
 * @param candidate Candidate interface
 * @param typeMapping Generic type mapping
 * @param node Instantiation AST node for printing error messages
 */
void InterfaceManager::substantiateSignatures(Interface &candidate, const TypeMapping &typeMapping, const ASTNode *node) {
  // Loop over all signatures and substantiate the generic ones
  for (Function *method : candidate.methods) {
    // Skip methods, that are already fully substantiated
    if (method->isFullySubstantiated())
      continue;

    // Substantiate return type
    if (method->isNormalFunction())
      FunctionManager::substantiateReturnType(*method, typeMapping, node);

    // Substantiate param types
    for (auto &[qualType, isOptional] : method->paramList)
      if (qualType.isBase(TY_GENERIC))
        TypeMatcher::substantiateTypeWithTypeMapping(qualType, typeMapping, node);
  }
}

/**
 * Searches the candidate template types for a generic type object with a certain name and return it
 *
 * @param candidate Matching candidate interface
 * @param templateTypeName Template type name
 * @return Generic type object
 */
const GenericType *InterfaceManager::getGenericTypeOfCandidateByName(const Interface &candidate,
                                                                     const std::string &templateTypeName) {
  for (const GenericType &templateType : candidate.templateTypes) {
    if (!templateType.is(TY_GENERIC))
      continue;
    if (templateType.getSubType() == templateTypeName)
      return &templateType;
  }
  return nullptr;
}

/**
 * Calculate the cache key for the interface lookup cache
 *
 * @param scope Scope to match against
 * @param name Interface name requirement
 * @param templateTypes Template types to substantiate generic types
 * @return Cache key
 */
uint64_t InterfaceManager::getCacheKey(const Scope *scope, const std::string &name, const QualTypeList &templateTypes) {
  uint64_t hash = 0;
  hashCombine64(hash, hashPointer(scope));
  hashCombine64(hash, std::hash<std::string>{}(name));
  hashCombine64(hash, hashVector(templateTypes));
  return hash;
}

/**
 * Clear the lookup cache
 */
void InterfaceManager::cleanup() {
  lookupCache.clear();
  lookupCacheHits = 0;
  lookupCacheMisses = 0;
}

/**
 * Dump usage statistics for the lookup cache
 */
std::string InterfaceManager::dumpLookupCacheStatistics() {
  std::stringstream stats;
  stats << "InterfaceManager lookup cache statistics:" << std::endl;
  stats << "  lookup cache entries: " << lookupCache.size() << std::endl;
  stats << "  lookup cache hits: " << lookupCacheHits << std::endl;
  stats << "  lookup cache misses: " << lookupCacheMisses << std::endl;
  return stats.str();
}

} // namespace spice::compiler
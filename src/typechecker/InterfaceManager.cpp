// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "InterfaceManager.h"

#include <ast/ASTNodes.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/TypeMatcher.h>

namespace spice::compiler {

Interface *InterfaceManager::insertInterface(Scope *insertScope, Interface &spiceInterface,
                                             std::vector<Interface *> *nodeInterfaceList) {
  // Open a new manifestation list. Which gets filled by the substantiated manifestations of the interface
  insertScope->interfaces.insert({spiceInterface.declNode->codeLoc.toString(), std::unordered_map<std::string, Interface>()});

  // Save substantiation in declaration node
  Interface *substantiation = insertSubstantiation(insertScope, spiceInterface, spiceInterface.declNode);
  nodeInterfaceList->push_back(substantiation);

  return substantiation;
}

Interface *InterfaceManager::insertSubstantiation(Scope *insertScope, Interface &newManifestation, const ASTNode *declNode) {
  const std::string mangledInterfaceName = newManifestation.getMangledName();
  const std::string codeLocStr = declNode->codeLoc.toString();
  const std::string signature = newManifestation.getSignature();

  // Make sure that the manifestation does not exist already
  for (const auto &manifestations : insertScope->interfaces)
    assert(!manifestations.second.contains(newManifestation.getMangledName()));

  // Retrieve the matching manifestation list of the scope
  assert(insertScope->interfaces.contains(codeLocStr));
  InterfaceManifestationList &manifestationList = insertScope->interfaces.at(codeLocStr);

  // Add substantiated interface
  newManifestation.manifestationIndex = manifestationList.size();
  manifestationList.emplace(mangledInterfaceName, newManifestation);
  return &manifestationList.at(mangledInterfaceName);
}

/**
 * Check if there is an interface in this scope, fulfilling all given requirements and if found, return it.
 * If more than one interface matches the requirement, an error gets thrown
 *
 * @param matchScope Scope to match against
 * @param requestedName Interface name requirement
 * @param requestedTemplateTypes Template types to substantiate generic types
 * @param node Instantiation AST node for printing error messages
 * @return Matched interface or nullptr
 */
Interface *InterfaceManager::matchInterface(Scope *matchScope, const std::string &requestedName,
                                            const std::vector<SymbolType> &requestedTemplateTypes, const ASTNode *node) {
  // Copy the registry to prevent iterating over items, that are created within the loop
  InterfaceRegistry interfaceRegistry = matchScope->interfaces;
  // Loop over interface registry to find interfaces, that match the requirements of the instantiation
  std::vector<Interface *> matches;
  for (const auto &[defCodeLocStr, m] : interfaceRegistry) {
    // Copy the manifestation list to prevent iterating over items, that are created within the loop
    const InterfaceManifestationList manifestations = m;
    for (const auto &[mangledName, presetInterface] : manifestations) {
      // Copy the interface to be able to substantiate types
      Interface candidate = presetInterface;

      // Skip generic substantiations to prevent double matching of an interface
      if (presetInterface.genericSubstantiation)
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

      // Map signatures from generic to concrete
      substantiateSignatures(candidate, candidate.typeMapping);

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
      if (manifestations.contains(candidate.getMangledName())) {
        matches.push_back(&matchScope->interfaces.at(defCodeLocStr).at(candidate.getMangledName()));
        break; // Leave the whole manifestation list to not double-match the manifestation
      }

      // Insert the substantiated version if required
      Interface *substantiatedInterface = insertSubstantiation(matchScope, candidate, presetInterface.declNode);
      substantiatedInterface->genericSubstantiation = true;
      substantiatedInterface->declNode->getInterfaceManifestations()->push_back(substantiatedInterface);

      // Copy interface entry
      const std::string newSignature = substantiatedInterface->getSignature();
      matchScope->lookupStrict(substantiatedInterface->name)->used = true;
      matchScope->symbolTable.copySymbol(substantiatedInterface->name, newSignature);
      substantiatedInterface->entry = matchScope->lookupStrict(newSignature);
      assert(substantiatedInterface->entry != nullptr);

      // Copy interface scope
      const std::string newScopeName = INTERFACE_SCOPE_PREFIX + newSignature;
      matchScope->copyChildScope(INTERFACE_SCOPE_PREFIX + presetInterface.name, newScopeName);
      substantiatedInterface->interfaceScope = matchScope->getChildScope(newScopeName);
      assert(substantiatedInterface->interfaceScope != nullptr);
      substantiatedInterface->interfaceScope->isGenericScope = false;

      // Attach the template types to the new interface entry
      SymbolType entryType = substantiatedInterface->entry->getType();
      entryType.setTemplateTypes(substantiatedInterface->getTemplateTypes());
      entryType.setBodyScope(substantiatedInterface->interfaceScope);
      substantiatedInterface->entry->updateType(entryType, true);

      // Replace symbol types of method entries with concrete types
      /*assert(substantiatedInterface->interfaceScope != nullptr);
      for (size_t i = 0; i < substantiatedInterface->methods.size(); i++) {
        // Replace field type with concrete template type
        SymbolTableEntry *methodEntry = substantiatedInterface->interfaceScope->symbolTable.lookupByIndex(i);
        assert(methodEntry != nullptr);
        methodEntry->updateType(substantiatedInterface->methods.at(i), true);
      }*/

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

  return matches.front();
}

/**
 * Checks if the matching candidate fulfills the name requirement
 *
 * @param candidate Matching candidate interface
 * @param requestedName Requested interface name
 * @return Fulfilled or not
 */
bool InterfaceManager::matchName(const Interface &candidate, const std::string &requestedName) {
  return candidate.name == requestedName;
}

/**
 * Checks if the matching candidate fulfills the template types requirement
 *
 * @param candidate Matching candidate interface
 * @param requestedTemplateTypes Requested interface template types
 * @return Fulfilled or not
 */
bool InterfaceManager::matchTemplateTypes(Interface &candidate, const std::vector<SymbolType> &requestedTemplateTypes,
                                          TypeMapping &typeMapping) {
  // Check if the number of types match
  const size_t typeCount = requestedTemplateTypes.size();
  if (typeCount != candidate.templateTypes.size())
    return false;

  // Loop over all template types
  for (size_t i = 0; i < typeCount; i++) {
    const SymbolType &requestedType = requestedTemplateTypes.at(i);
    SymbolType &candidateType = candidate.templateTypes.at(i);

    // Give the type matcher a way to retrieve instances of GenericType by their name
    std::function<const GenericType *(const std::string &)> genericTypeResolver = [=](const std::string &genericTypeName) {
      return getGenericTypeOfCandidateByName(candidate, genericTypeName);
    };

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
 * Come up with the concrete signatures, by applying the type mapping onto the generic signatures
 *
 * @param candidate Candidate interface
 * @param typeMapping Generic type mapping
 */
void InterfaceManager::substantiateSignatures(Interface &candidate, TypeMapping &typeMapping) {
  // Loop over all signatures and substantiate the generic ones
  for (Function *method : candidate.methods) {
    // Skip methods, that are already fully substantiated
    if (method->isFullySubstantiated())
      continue;

    // Substantiate return type
    if (method->isFunction())
      FunctionManager::substantiateReturnType(*method, typeMapping);

    // Substantiate param types
    for (Param &paramType : method->paramList)
      if (paramType.type.isBaseType(TY_GENERIC))
        TypeMatcher::substantiateTypeWithTypeMapping(paramType.type, typeMapping);
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

} // namespace spice::compiler
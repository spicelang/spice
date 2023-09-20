// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "FunctionManager.h"

#include <ast/ASTNodes.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/TypeMatcher.h>

namespace spice::compiler {

Function *FunctionManager::insertFunction(Scope *insertScope, const Function &baseFunction,
                                          std::vector<Function *> *nodeFunctionList) {
  // Open a new manifestation list for the function definition
  insertScope->functions.insert({baseFunction.declNode->codeLoc, std::unordered_map<std::string, Function>()});

  // Collect substantiations
  std::vector<Function> manifestations;
  substantiateOptionalParams(baseFunction, manifestations);
  assert(!manifestations.empty());

  // Save substantiations in declaration node
  Function *manifestationPtr;
  for (const Function &manifestation : manifestations) {
    manifestationPtr = insertSubstantiation(insertScope, manifestation, baseFunction.declNode);
    assert(manifestationPtr != nullptr);
    if (nodeFunctionList)
      nodeFunctionList->push_back(manifestationPtr);
  }

  if (!nodeFunctionList)
    return manifestationPtr;

  assert(!nodeFunctionList->empty());
  return nodeFunctionList->front();
}

/**
 * Create definite functions from ambiguous ones, in regards to optional arguments.
 *
 * Example:
 * int testFunc(string, int?, double?)
 * gets
 * int testFunc(string)
 * int testFunc(string, int)
 * int testFunc(string, int, double)
 *
 * This method also accepts functions, that are already definite, but does nothing to them.
 *
 * @param baseFunction Ambiguous base function
 * @param manifestations Vector to store the definite manifestations
 * @return True, if there were optional arguments found
 */
void FunctionManager::substantiateOptionalParams(const Function &baseFunction, std::vector<Function> &manifestations) {
  // Handle the case of no parameters -> simply return the base function
  if (baseFunction.paramList.empty()) {
    manifestations.push_back(baseFunction);
    return;
  }

  ParamList currentFunctionParamTypes;
  currentFunctionParamTypes.reserve(baseFunction.paramList.size());
  bool metFirstOptionalParam = false;
  Function manifestation = baseFunction;

  // Loop over all parameters
  for (const Param &param : baseFunction.paramList) {
    // Check if we have a mandatory parameter
    if (!param.isOptional) {
      currentFunctionParamTypes.push_back({param.type, /*optional=*/false});
      continue;
    }

    // Add substantiation without the optional parameter
    if (!metFirstOptionalParam) {
      manifestation.paramList = currentFunctionParamTypes;
      manifestations.push_back(manifestation);
      // Now we cannot accept mandatory parameters anymore
      metFirstOptionalParam = true;
    }

    // Add substantiation with the optional parameter
    currentFunctionParamTypes.push_back({param.type, /*optional=*/false});
    manifestation.paramList = currentFunctionParamTypes;
    manifestations.push_back(manifestation);
  }

  // Ensure at least once manifestation
  if (manifestations.empty())
    manifestations.push_back(baseFunction);
}

Function FunctionManager::createMainFunction(SymbolTableEntry *entry, const std::vector<SymbolType> &paramTypes,
                                             ASTNode *declNode) {
  ParamList paramList;
  for (const SymbolType &paramType : paramTypes)
    paramList.push_back({paramType, false});
  return {MAIN_FUNCTION_NAME, entry, SymbolType(TY_DYN), SymbolType(TY_INT), paramList, {}, declNode};
}

Function *FunctionManager::insertSubstantiation(Scope *insertScope, const Function &newManifestation, const ASTNode *declNode) {
  assert(newManifestation.hasSubstantiatedParams());

  const std::string signature = newManifestation.getSignature();

  // Check if the function exists already
  for (const auto &[_, manifestations] : insertScope->functions)
    if (manifestations.contains(signature))
      throw SemanticError(declNode, FUNCTION_DECLARED_TWICE, "The function/procedure '" + signature + "' is declared twice");

  // Retrieve the matching manifestation list of the scope
  assert(insertScope->functions.contains(declNode->codeLoc));
  FunctionManifestationList &manifestationList = insertScope->functions.at(declNode->codeLoc);

  // Add substantiated function
  manifestationList.emplace(signature, newManifestation);
  return &manifestationList.at(signature);
}

/**
 * Checks if a function exists by matching it, but not setting it to used
 *
 * @param matchScope Scope to match against
 * @param requestedName Function name requirement
 * @param requestedThisType This type requirement
 * @param requestedParamTypes Argument types requirement
 * @param strictSpecifierMatching Match argument and this type specifiers strictly
 * @return Found function or nullptr
 */
const Function *FunctionManager::lookupFunction(Scope *matchScope, const std::string &requestedName,
                                                const SymbolType &requestedThisType,
                                                const std::vector<SymbolType> &requestedParamTypes,
                                                bool strictSpecifierMatching) {
  assert(requestedThisType.isOneOf({TY_DYN, TY_STRUCT}));

  // Copy the registry to prevent iterating over items, that are created within the loop
  FunctionRegistry functionRegistry = matchScope->functions;
  // Loop over function registry to find functions, that match the requirements of the call
  std::vector<const Function *> matches;
  for (const auto &[defCodeLocStr, m] : functionRegistry) {
    // Copy the manifestation list to prevent iterating over items, that are created within the loop
    const FunctionManifestationList manifestations = m;
    for (const auto &[signature, presetFunction] : manifestations) {
      assert(presetFunction.hasSubstantiatedParams()); // No optional params are allowed at this point

      // Only match against fully substantiated versions to prevent double matching of a function
      if (!presetFunction.isFullySubstantiated())
        continue;

      // Copy the function to be able to substantiate types
      Function candidate = presetFunction;

      bool forceSubstantiation = false;
      MatchResult matchResult = matchManifestation(candidate, matchScope, requestedName, requestedThisType, requestedParamTypes,
                                                   strictSpecifierMatching, forceSubstantiation);
      if (matchResult == MatchResult::SKIP_FUNCTION)
        break; // Leave the whole function
      if (matchResult == MatchResult::SKIP_MANIFESTATION)
        continue; // Leave this manifestation and try the next one

      // Add to matches
      matches.push_back(&matchScope->functions.at(defCodeLocStr).at(signature));

      break; // Leave the whole manifestation list to not double-match the manifestation
    }
  }

  // Return the very match or a nullptr
  return !matches.empty() ? matches.front() : nullptr;
}

/**
 * Check if there is a function in the scope, fulfilling all given requirements and if found, return it.
 * If more than one function matches the requirement, an error gets thrown.
 *
 * @param matchScope Scope to match against
 * @param requestedName Function name requirement
 * @param requestedThisType This type requirement
 * @param requestedParamTypes Argument types requirement
 * @param strictSpecifierMatching Match argument and this type specifiers strictly
 * @param callNode Call AST node for printing error messages
 * @return Matched function or nullptr
 */
Function *FunctionManager::matchFunction(Scope *matchScope, const std::string &requestedName, const SymbolType &requestedThisType,
                                         const std::vector<SymbolType> &requestedParamTypes, bool strictSpecifierMatching,
                                         const ASTNode *callNode) {
  assert(requestedThisType.isOneOf({TY_DYN, TY_STRUCT}));

  // Copy the registry to prevent iterating over items, that are created within the loop
  FunctionRegistry functionRegistry = matchScope->functions;
  // Loop over function registry to find functions, that match the requirements of the call
  std::vector<Function *> matches;
  for (const auto &[defCodeLocStr, m] : functionRegistry) {
    // Copy the manifestation list to prevent iterating over items, that are created within the loop
    const FunctionManifestationList manifestations = m;
    for (const auto &[signature, presetFunction] : manifestations) {
      assert(presetFunction.hasSubstantiatedParams()); // No optional params are allowed at this point

      // Skip generic substantiations to prevent double matching of a function
      if (presetFunction.genericSubstantiation)
        continue;

      // Copy the function to be able to substantiate types
      Function candidate = presetFunction;

      bool forceSubstantiation = false;
      MatchResult matchResult = matchManifestation(candidate, matchScope, requestedName, requestedThisType, requestedParamTypes,
                                                   strictSpecifierMatching, forceSubstantiation);
      if (matchResult == MatchResult::SKIP_FUNCTION)
        break; // Leave the whole function
      if (matchResult == MatchResult::SKIP_MANIFESTATION)
        continue; // Leave this manifestation and try the next one

      // We found a match! -> Set the actual candidate and its entry to used
      candidate.used = true;
      candidate.entry->used = true;

      // Check if the function is generic needs to be substantiated
      if (presetFunction.templateTypes.empty() && !forceSubstantiation) {
        assert(matchScope->functions.contains(defCodeLocStr) && matchScope->functions.at(defCodeLocStr).contains(signature));
        matches.push_back(&matchScope->functions.at(defCodeLocStr).at(signature));
        matches.back()->used = true;
        continue; // Match was successful -> match the next function
      }

      // Check if we already have this manifestation and can simply re-use it
      if (matchScope->functions.at(defCodeLocStr).contains(candidate.getSignature())) {
        matches.push_back(&matchScope->functions.at(defCodeLocStr).at(candidate.getSignature()));
        break; // Leave the whole manifestation list to not double-match the manifestation
      }

      // Insert the substantiated version if required
      Function *substantiatedFunction = insertSubstantiation(matchScope, candidate, presetFunction.declNode);
      substantiatedFunction->genericSubstantiation = true;
      substantiatedFunction->alreadyTypeChecked = false;
      substantiatedFunction->declNode->getFctManifestations(requestedName)->push_back(substantiatedFunction);

      // Copy function entry
      const std::string newSignature = substantiatedFunction->getSignature(false);
      matchScope->lookupStrict(presetFunction.entry->name)->used = true;
      matchScope->symbolTable.copySymbol(presetFunction.entry->name, newSignature);
      substantiatedFunction->entry = matchScope->lookupStrict(newSignature);
      assert(substantiatedFunction->entry != nullptr);

      // Copy function scope
      matchScope->copyChildScope(presetFunction.getSignature(false), newSignature);
      Scope *childScope = matchScope->getChildScope(newSignature);
      assert(childScope != nullptr);
      childScope->isGenericScope = false;
      substantiatedFunction->bodyScope = childScope;

      // Insert symbols for generic type names with concrete types into the child block
      for (const auto &[typeName, concreteType] : substantiatedFunction->typeMapping)
        childScope->insertGenericType(typeName, GenericType(concreteType));

      // Substantiate the 'this' entry in the new function scope
      if (presetFunction.isMethod() && !presetFunction.templateTypes.empty()) {
        SymbolTableEntry *thisEntry = childScope->lookupStrict(THIS_VARIABLE_NAME);
        assert(thisEntry != nullptr);
        thisEntry->updateType(candidate.thisType.toPointer(callNode), /*overwriteExistingType=*/true);
      }

      // Add to matched functions
      matches.push_back(substantiatedFunction);

      break; // Leave the whole manifestation list to not double-match the manifestation
    }
  }

  // If no matches were found, return a nullptr
  if (matches.empty())
    return nullptr;

  // Check if more than one function matches the requirements
  if (matches.size() > 1)
    throw SemanticError(callNode, FUNCTION_AMBIGUITY, "Multiple functions match the requested signature");

  // Return the very match
  return matches.front();
}

/**
 * Check if there is a function in the scope, fulfilling all given requirements and if found, return it.
 * If more than one function matches the requirement, an error gets thrown.
 *
 * @param matchScope Scope to match against
 * @param requestedName Function name requirement
 * @param templateTypeHints Template types to substantiate generic types
 * @param requestedParamTypes Argument types requirement
 * @param requestedReturnType Return type requirement
 * @param strictSpecifierMatching Match argument and this type specifiers strictly
 * @param callNode Call AST node for printing error messages
 * @return Matched or not
 */
bool FunctionManager::matchInterfaceMethod(Scope *matchScope, const std::string &requestedName,
                                           const std::vector<SymbolType> &requestedParamTypes,
                                           const SymbolType &requestedReturnType, bool strictSpecifierMatching) {
  // Copy the registry to prevent iterating over items, that are created within the loop
  FunctionRegistry functionRegistry = matchScope->functions;
  // Loop over function registry to find functions, that match the requirements of the call
  std::vector<Function *> matches;
  for (const auto &[defCodeLocStr, m] : functionRegistry) {
    // Copy the manifestation list to prevent iterating over items, that are created within the loop
    const FunctionManifestationList manifestations = m;
    for (const auto &[mangledName, presetFunction] : manifestations) {
      assert(presetFunction.hasSubstantiatedParams()); // No optional params are allowed at this point

      // Skip generic substantiations to prevent double matching of a function
      if (presetFunction.genericSubstantiation)
        continue;

      // Copy the function to be able to substantiate types
      Function candidate = presetFunction;

      // Check name requirement
      if (!matchName(candidate, requestedName))
        break; // Leave the whole manifestation list, because all manifestations in this list have the same name

      // Prepare mapping table from generic type name to concrete type
      TypeMapping &typeMapping = candidate.typeMapping;
      typeMapping.clear();
      typeMapping.reserve(candidate.templateTypes.size());

      // Check arg types requirement
      bool forceSubstantiation = false;
      if (!matchArgTypes(candidate, requestedParamTypes, typeMapping, strictSpecifierMatching, forceSubstantiation))
        continue; // Leave this manifestation and try the next one

      if (!matchReturnType(candidate, requestedReturnType, typeMapping, strictSpecifierMatching))
        continue; // Leave this manifestation and try the next one

      // We found a match
      return true;
    }
  }

  // We did not find a match
  return false;
}

MatchResult FunctionManager::matchManifestation(Function &candidate, Scope *&matchScope, const std::string &requestedName,
                                                const SymbolType &requestedThisType,
                                                const std::vector<SymbolType> &requestedParamTypes, bool strictSpecifierMatching,
                                                bool &forceSubstantiation) {
  // Check name requirement
  if (!matchName(candidate, requestedName))
    return MatchResult::SKIP_FUNCTION; // Leave the whole manifestation list, because all have the same name

  // Prepare mapping table from generic type name to concrete type
  TypeMapping &typeMapping = candidate.typeMapping;
  typeMapping.clear();
  typeMapping.reserve(candidate.templateTypes.size());

  // Check 'this' type requirement
  if (!matchThisType(candidate, requestedThisType, typeMapping, strictSpecifierMatching))
    return MatchResult::SKIP_MANIFESTATION; // Leave this manifestation and try the next one

  // Check arg types requirement
  if (!matchArgTypes(candidate, requestedParamTypes, typeMapping, strictSpecifierMatching, forceSubstantiation))
    return MatchResult::SKIP_MANIFESTATION; // Leave this manifestation and try the next one

  // Substantiate return type
  substantiateReturnType(candidate, typeMapping);

  const SymbolType &thisType = candidate.thisType;
  if (!thisType.is(TY_DYN)) {
    // Update struct scope of 'this' type to the substantiated struct scope
    std::vector<SymbolType> concreteTemplateTypes;
    if (!thisType.hasAnyGenericParts())
      concreteTemplateTypes = thisType.getTemplateTypes();
    const std::string structSignature = Struct::getSignature(thisType.getSubType(), concreteTemplateTypes);
    Scope *substantiatedStructBodyScope = matchScope->parent->getChildScope(STRUCT_SCOPE_PREFIX + structSignature);
    assert(substantiatedStructBodyScope != nullptr);
    candidate.thisType.setBodyScope(substantiatedStructBodyScope);

    // Set match scope to the substantiated struct scope
    matchScope = substantiatedStructBodyScope;
  }

  // Clear template types of candidate, since they are not needed anymore
  candidate.templateTypes.clear();

  return MatchResult::MATCHED;
}

/**
 * Checks if the matching candidate fulfills the name requirement
 *
 * @param candidate Matching candidate function
 * @param requestedName Requested function name
 * @return Fulfilled or not
 */
bool FunctionManager::matchName(const Function &candidate, const std::string &requestedName) {
  return candidate.name == requestedName;
}

/**
 * Checks if the matching candidate fulfills the 'this' type requirement
 *
 * @param candidate Matching candidate function
 * @param requestedThisType Requested 'this' type
 * @param typeMapping Concrete template type mapping
 * @param strictSpecifierMatching Match specifiers strictly
 * @return Fulfilled or not
 */
bool FunctionManager::matchThisType(Function &candidate, const SymbolType &requestedThisType, TypeMapping &typeMapping,
                                    bool strictSpecifierMatching) {
  SymbolType &candidateThisType = candidate.thisType;

  // Give the type matcher a way to retrieve instances of GenericType by their name
  TypeMatcher::ResolverFct genericTypeResolver = [&](const std::string &genericTypeName) {
    return getGenericTypeOfCandidateByName(candidate, genericTypeName);
  };

  // Check if the requested 'this' type matches the candidate 'this' type. The type mapping may be extended
  if (!TypeMatcher::matchRequestedToCandidateType(candidateThisType, requestedThisType, typeMapping, genericTypeResolver,
                                                  strictSpecifierMatching))
    return false;

  // Substantiate the candidate param type, based on the type mapping
  if (candidateThisType.hasAnyGenericParts())
    TypeMatcher::substantiateTypeWithTypeMapping(candidateThisType, typeMapping);

  return true;
}

/**
 * Checks if the matching candidate fulfills the argument types requirement
 *
 * @param candidate Matching candidate function
 * @param requestedArgTypes Requested argument types
 * @param typeMapping Concrete template type mapping
 * @param strictSpecifierMatching Match specifiers strictly
 * @return Fulfilled or not
 */
bool FunctionManager::matchArgTypes(Function &candidate, const std::vector<SymbolType> &requestedArgTypes,
                                    TypeMapping &typeMapping, bool strictSpecifierMatching, bool &needsSubstantiation) {
  // If the number of arguments does not match with the number of params, the matching fails
  if (requestedArgTypes.size() != candidate.paramList.size())
    return false;

  // Give the type matcher a way to retrieve instances of GenericType by their name
  TypeMatcher::ResolverFct genericTypeResolver = [&](const std::string &genericTypeName) {
    return getGenericTypeOfCandidateByName(candidate, genericTypeName);
  };

  // Loop over all parameters
  for (size_t i = 0; i < requestedArgTypes.size(); i++) {
    // Retrieve actual and requested types
    const SymbolType &requestedParamType = requestedArgTypes.at(i);
    assert(!candidate.paramList.at(i).isOptional);
    SymbolType &candidateParamType = candidate.paramList.at(i).type;

    // Check if the requested param type matches the candidate param type. The type mapping may be extended
    if (!TypeMatcher::matchRequestedToCandidateType(candidateParamType, requestedParamType, typeMapping, genericTypeResolver,
                                                    strictSpecifierMatching))
      return false;

    // Substantiate the candidate param type, based on the type mapping
    if (candidateParamType.hasAnyGenericParts())
      TypeMatcher::substantiateTypeWithTypeMapping(candidateParamType, typeMapping);

    // If we have a function/procedure type we need to take care of the information, if it takes captures
    if (requestedParamType.getBaseType().isOneOf({TY_FUNCTION, TY_PROCEDURE}) && requestedParamType.hasLambdaCaptures()) {
      candidateParamType.setHasLambdaCaptures(true);
      needsSubstantiation = true;
    }
  }

  return true;
}

/**
 * Substantiates the candidate return type, based on the given type mapping
 *
 * @param candidate Matching candidate function
 * @param typeMapping Concrete template type mapping
 */
void FunctionManager::substantiateReturnType(Function &candidate, TypeMapping &typeMapping) {
  if (candidate.returnType.hasAnyGenericParts())
    TypeMatcher::substantiateTypeWithTypeMapping(candidate.returnType, typeMapping);
}

/**
 * Checks if the matching candidate fulfills the return type requirement
 *
 * @param candidate Matching candidate function
 * @param requestedReturnType Requested return type
 * @param typeMapping Concrete template type mapping
 * @param strictSpecifierMatching Match specifiers strictly
 * @return Fulfilled or not
 */
bool FunctionManager::matchReturnType(Function &candidate, const SymbolType &requestedReturnType, TypeMapping &typeMapping,
                                      bool strictSpecifierMatching) {
  SymbolType &candidateReturnType = candidate.returnType;

  // Give the type matcher a way to retrieve instances of GenericType by their name
  TypeMatcher::ResolverFct genericTypeResolver = [&](const std::string &genericTypeName) {
    return getGenericTypeOfCandidateByName(candidate, genericTypeName);
  };

  // Check if the requested param type matches the candidate param type. The type mapping may be extended
  if (!TypeMatcher::matchRequestedToCandidateType(candidateReturnType, requestedReturnType, typeMapping, genericTypeResolver,
                                                  strictSpecifierMatching))
    return false;

  // Substantiate the candidate param type, based on the type mapping
  if (candidateReturnType.hasAnyGenericParts())
    TypeMatcher::substantiateTypeWithTypeMapping(candidateReturnType, typeMapping);

  return true;
}

/**
 * Searches the candidate template types for a generic type object with a certain name and return it
 *
 * @param candidate Matching candidate function
 * @param templateTypeName Template type name
 * @return Generic type object
 */
const GenericType *FunctionManager::getGenericTypeOfCandidateByName(const Function &candidate,
                                                                    const std::string &templateTypeName) {
  for (const GenericType &templateType : candidate.templateTypes) {
    if (templateType.getSubType() == templateTypeName)
      return &templateType;
  }
  return nullptr;
}

} // namespace spice::compiler
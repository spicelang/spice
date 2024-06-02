// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "FunctionManager.h"

#include <ast/ASTNodes.h>
#include <exception/SemanticError.h>
#include <model/GenericType.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/ExprResult.h>
#include <typechecker/TypeMatcher.h>
#include <util/CodeLoc.h>
#include <util/CustomHashFunctions.h>

namespace spice::compiler {

// Static member initialization
std::unordered_map<uint64_t, Function *> FunctionManager::lookupCache = {};

Function *FunctionManager::insertFunction(Scope *insertScope, const Function &baseFunction,
                                          std::vector<Function *> *nodeFunctionList) {
  // Open a new manifestation list for the function definition
  const std::string fctId = baseFunction.name + ":" + baseFunction.declNode->codeLoc.toPrettyLineAndColumn();
  insertScope->functions.insert({fctId, FunctionManifestationList()});

  // Collect substantiations
  std::vector<Function> manifestations;
  substantiateOptionalParams(baseFunction, manifestations);
  assert(!manifestations.empty());

  // Save substantiations in declaration node
  Function *manifestationPtr = nullptr;
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
      currentFunctionParamTypes.push_back({param.qualType, /*optional=*/false});
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
    currentFunctionParamTypes.push_back({param.qualType, /*optional=*/false});
    manifestation.paramList = currentFunctionParamTypes;
    manifestations.push_back(manifestation);
  }

  // Ensure at least once manifestation
  if (manifestations.empty())
    manifestations.push_back(baseFunction);
}

Function FunctionManager::createMainFunction(SymbolTableEntry *entry, const QualTypeList &paramTypes, ASTNode *declNode) {
  ParamList paramList;
  for (const QualType &paramType : paramTypes)
    paramList.push_back({paramType, false});
  return {MAIN_FUNCTION_NAME, entry, QualType(TY_DYN), QualType(TY_INT), paramList, {}, declNode};
}

Function *FunctionManager::insertSubstantiation(Scope *insertScope, const Function &newManifestation, const ASTNode *declNode) {
  assert(newManifestation.hasSubstantiatedParams());

  const std::string signature = newManifestation.getSignature();

  // Check if the function exists already
  for (const auto &[_, manifestations] : insertScope->functions) {
    if (manifestations.contains(signature)) {
      if (newManifestation.isFunction())
        throw SemanticError(declNode, FUNCTION_DECLARED_TWICE, "'" + signature + "' is declared twice");
      else
        throw SemanticError(declNode, PROCEDURE_DECLARED_TWICE, "'" + signature + "' is declared twice");
    }
  }

  // Retrieve the matching manifestation list of the scope
  const std::string fctId = newManifestation.name + ":" + declNode->codeLoc.toPrettyLineAndColumn();
  assert(insertScope->functions.contains(fctId));
  FunctionManifestationList &manifestationList = insertScope->functions.at(fctId);

  // Add substantiated function
  manifestationList.emplace(signature, newManifestation);
  return &manifestationList.at(signature);
}

/**
 * Checks if a function exists by matching it, but not setting it to used
 *
 * @param matchScope Scope to match against
 * @param reqName Function name requirement
 * @param reqThisType This type requirement
 * @param reqArgs Argument requirement
 * @param strictSpecifierMatching Match argument and this type specifiers strictly
 * @return Found function or nullptr
 */
const Function *FunctionManager::lookupFunction(Scope *matchScope, const std::string &reqName, const QualType &reqThisType,
                                                const ArgList &reqArgs, bool strictSpecifierMatching) {
  assert(reqThisType.isOneOf({TY_DYN, TY_STRUCT}));

  // Do cache lookup
  const uint64_t cacheKey = getCacheKey(matchScope, reqName, reqThisType, reqArgs, {});
  if (lookupCache.contains(cacheKey))
    return lookupCache.at(cacheKey);

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

      // Create empty type mapping
      TypeMapping &typeMapping = candidate.typeMapping;

      bool forceSubstantiation = false;
      MatchResult matchResult = matchManifestation(candidate, matchScope, reqName, reqThisType, reqArgs, typeMapping,
                                                   strictSpecifierMatching, forceSubstantiation, nullptr);
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
 * @param reqName Function name requirement
 * @param reqThisType This type requirement
 * @param reqArgs Argument requirement
 * @param templateTypeHints Template type requirement
 * @param strictSpecifierMatching Match argument and this type specifiers strictly
 * @param callNode Call AST node for printing error messages
 * @return Matched function or nullptr
 */
Function *FunctionManager::matchFunction(Scope *matchScope, const std::string &reqName, const QualType &reqThisType,
                                         const ArgList &reqArgs, const QualTypeList &templateTypeHints,
                                         bool strictSpecifierMatching, const ASTNode *callNode) {
  assert(reqThisType.isOneOf({TY_DYN, TY_STRUCT, TY_INTERFACE}));

  // Do cache lookup
  const uint64_t cacheKey = getCacheKey(matchScope, reqName, reqThisType, reqArgs, templateTypeHints);
  if (lookupCache.contains(cacheKey))
    return lookupCache.at(cacheKey);

  // Copy the registry to prevent iterating over items, that are created within the loop
  FunctionRegistry functionRegistry = matchScope->functions;
  // Loop over function registry to find functions, that match the requirements of the call
  std::vector<Function *> matches;
  for (const auto &[fctId, m] : functionRegistry) {
    // Copy the manifestation list to prevent iterating over items, that are created within the loop
    const FunctionManifestationList manifestations = m;
    for (const auto &[signature, presetFunction] : manifestations) {
      assert(presetFunction.hasSubstantiatedParams()); // No optional params are allowed at this point

      // Skip generic substantiations to prevent double matching of a function
      if (presetFunction.isGenericSubstantiation())
        continue;

      // Copy the function to be able to substantiate types
      Function candidate = presetFunction;

      // Prepare type mapping, based on the given initial type mapping
      TypeMapping &typeMapping = candidate.typeMapping;
      typeMapping.clear();
      for (size_t i = 0; i < std::min(templateTypeHints.size(), candidate.templateTypes.size()); i++) {
        const std::string &typeName = candidate.templateTypes.at(i).getSubType();
        const QualType &templateType = templateTypeHints.at(i);
        typeMapping.insert({typeName, templateType});
      }

      bool forceSubstantiation = false;
      MatchResult matchResult = matchManifestation(candidate, matchScope, reqName, reqThisType, reqArgs, typeMapping,
                                                   strictSpecifierMatching, forceSubstantiation, callNode);
      if (matchResult == MatchResult::SKIP_FUNCTION)
        break; // Leave the whole function
      if (matchResult == MatchResult::SKIP_MANIFESTATION)
        continue; // Leave this manifestation and try the next one

      // We found a match! -> Set the actual candidate and its entry to used
      candidate.used = true;
      candidate.entry->used = true;

      // Check if the function is generic needs to be substantiated
      if (presetFunction.templateTypes.empty() && !forceSubstantiation) {
        assert(matchScope->functions.contains(fctId) && matchScope->functions.at(fctId).contains(signature));
        matches.push_back(&matchScope->functions.at(fctId).at(signature));
        matches.back()->used = true;
        continue; // Match was successful -> match the next function
      }

      // Check if we already have this manifestation and can simply re-use it
      const std::string nonGenericSignature = candidate.getSignature();
      if (matchScope->functions.at(fctId).contains(nonGenericSignature)) {
        matches.push_back(&matchScope->functions.at(fctId).at(nonGenericSignature));
        break; // Leave the whole manifestation list to not double-match the manifestation
      }

      // Insert the substantiated version if required
      Function *substantiatedFunction = insertSubstantiation(matchScope, candidate, presetFunction.declNode);
      substantiatedFunction->genericPreset = &matchScope->functions.at(fctId).at(signature);
      substantiatedFunction->alreadyTypeChecked = false;
      substantiatedFunction->declNode->getFctManifestations(reqName)->push_back(substantiatedFunction);

      // Copy function entry
      const std::string newSignature = substantiatedFunction->getSignature(false);
      matchScope->lookupStrict(presetFunction.entry->name)->used = true;
      substantiatedFunction->entry = matchScope->symbolTable.copySymbol(presetFunction.entry->name, newSignature);
      assert(substantiatedFunction->entry != nullptr);

      // Copy function scope
      const std::string oldSignature = presetFunction.getSignature(false);
      Scope *childScope = matchScope->copyChildScope(oldSignature, newSignature);
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
        thisEntry->updateType(candidate.thisType.toPtr(callNode), /*overwriteExistingType=*/true);
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
  if (matches.size() > 1) {
    std::stringstream errorMessage;
    errorMessage << "The function/procedure '" << reqName << "' is ambiguous. All of the following match the requested criteria:";
    for (const Function *match : matches)
      errorMessage << "\n  " << match->getSignature();
    throw SemanticError(callNode, FUNCTION_AMBIGUITY, errorMessage.str());
  }

  // Insert into cache
  lookupCache[cacheKey] = matches.front();

  // Return the very match
  return matches.front();
}

MatchResult FunctionManager::matchManifestation(Function &candidate, Scope *&matchScope, const std::string &reqName,
                                                const QualType &reqThisType, const ArgList &reqArgs, TypeMapping &typeMapping,
                                                bool strictSpecifierMatching, bool &forceSubstantiation,
                                                const ASTNode *callNode) {
  // Check name requirement
  if (!matchName(candidate, reqName))
    return MatchResult::SKIP_FUNCTION; // Leave the whole manifestation list, because all have the same name

  // Check 'this' type requirement
  if (!matchThisType(candidate, reqThisType, typeMapping, strictSpecifierMatching))
    return MatchResult::SKIP_MANIFESTATION; // Leave this manifestation and try the next one

  // Check arg types requirement
  if (!matchArgTypes(candidate, reqArgs, typeMapping, strictSpecifierMatching, forceSubstantiation, callNode))
    return MatchResult::SKIP_MANIFESTATION; // Leave this manifestation and try the next one

  // Check if there are unresolved generic types
  if (typeMapping.size() < candidate.templateTypes.size())
    return MatchResult::SKIP_MANIFESTATION; // Leave this manifestation and try the next one

  // Substantiate return type
  substantiateReturnType(candidate, typeMapping);

  const QualType &thisType = candidate.thisType;
  if (!thisType.is(TY_DYN)) {
    // If we only have the generic struct scope, lookup the concrete manifestation scope
    if (matchScope->isGenericScope) {
      const std::string &structName = thisType.getSubType();
      Scope *scope = thisType.getBodyScope()->parent;
      Struct *spiceStruct = StructManager::matchStruct(scope, structName, thisType.getTemplateTypes(), candidate.declNode);
      assert(spiceStruct != nullptr);
      matchScope = spiceStruct->scope;
    }
    candidate.thisType = candidate.thisType.getWithBodyScope(matchScope);
  }

  return MatchResult::MATCHED;
}

/**
 * Checks if the matching candidate fulfills the name requirement
 *
 * @param candidate Matching candidate function
 * @param reqName Requested function name
 * @return Fulfilled or not
 */
bool FunctionManager::matchName(const Function &candidate, const std::string &reqName) { return candidate.name == reqName; }

/**
 * Checks if the matching candidate fulfills the 'this' type requirement
 *
 * @param candidate Matching candidate function
 * @param reqThisType Requested 'this' type
 * @param typeMapping Concrete template type mapping
 * @param strictSpecifierMatching Match specifiers strictly
 * @return Fulfilled or not
 */
bool FunctionManager::matchThisType(Function &candidate, const QualType &reqThisType, TypeMapping &typeMapping,
                                    bool strictSpecifierMatching) {
  QualType &candidateThisType = candidate.thisType;

  // Shortcut for procedures
  if (candidateThisType.is(TY_DYN) && reqThisType.is(TY_DYN))
    return true;

  // Give the type matcher a way to retrieve instances of GenericType by their name
  TypeMatcher::ResolverFct genericTypeResolver = [&](const std::string &genericTypeName) {
    return getGenericTypeOfCandidateByName(candidate, genericTypeName);
  };

  // Check if the requested 'this' type matches the candidate 'this' type. The type mapping may be extended
  if (!TypeMatcher::matchRequestedToCandidateType(candidateThisType, reqThisType, typeMapping, genericTypeResolver,
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
 * @param reqArgs Requested argument types
 * @param typeMapping Concrete template type mapping
 * @param strictSpecifierMatching Match specifiers strictly
 * @param callNode Call AST node for printing error messages
 * @return Fulfilled or not
 */
bool FunctionManager::matchArgTypes(Function &candidate, const ArgList &reqArgs, TypeMapping &typeMapping,
                                    bool strictSpecifierMatching, bool &needsSubstantiation, const ASTNode *callNode) {
  std::vector<Param> &candidateParamList = candidate.paramList;

  // If the number of arguments does not match with the number of params, the matching fails
  if (reqArgs.size() != candidateParamList.size())
    return false;

  // Give the type matcher a way to retrieve instances of GenericType by their name
  TypeMatcher::ResolverFct genericTypeResolver = [&](const std::string &genericTypeName) {
    return getGenericTypeOfCandidateByName(candidate, genericTypeName);
  };

  // Loop over all parameters
  for (size_t i = 0; i < reqArgs.size(); i++) {
    // Retrieve actual and requested types
    assert(!candidateParamList.at(i).isOptional);
    QualType &candidateParamType = candidateParamList.at(i).qualType;
    const Arg &requestedParamType = reqArgs.at(i);
    const QualType &requestedType = requestedParamType.first;
    const bool isArgTemporary = requestedParamType.second;

    // Check if the requested param type matches the candidate param type. The type mapping may be extended
    if (!TypeMatcher::matchRequestedToCandidateType(candidateParamType, requestedType, typeMapping, genericTypeResolver,
                                                    strictSpecifierMatching))
      return false;

    // Substantiate the candidate param type, based on the type mapping
    if (candidateParamType.hasAnyGenericParts())
      TypeMatcher::substantiateTypeWithTypeMapping(candidateParamType, typeMapping);

    // Check if we try to bind a non-ref temporary to a non-const ref parameter
    if (!candidateParamType.canBind(requestedType, isArgTemporary)) {
      if (callNode)
        throw SemanticError(callNode, TEMP_TO_NON_CONST_REF, "Temporary values can only be bound to const reference parameters");
      return false;
    }

    // If we have a function/procedure type we need to take care of the information, if it takes captures
    if (requestedType.getBase().isOneOf({TY_FUNCTION, TY_PROCEDURE}) && requestedType.hasLambdaCaptures()) {
      candidateParamType = candidateParamType.getWithLambdaCaptures();
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

/**
 * Calculate the cache key for the function lookup cache
 *
 * @param scope Scope to match against
 * @param name Function name requirement
 * @param thisType This type requirement
 * @param args Argument requirement
 * @param templateTypes Template type requirement
 * @return Cache key
 */
uint64_t FunctionManager::getCacheKey(Scope *scope, const std::string &name, const QualType &thisType, const ArgList &args,
                                      const QualTypeList &templateTypes) {
  const auto pred1 = [](size_t acc, const Arg &val) {
    // Combine the previous hash value with the current element's hash, adjusted by a prime number to reduce collisions
    const uint64_t typeHash = std::hash<QualType>{}(val.first);
    const uint64_t temporaryHash = std::hash<bool>{}(val.second);
    const uint64_t newHash = typeHash ^ (temporaryHash << 1);
    return acc * 31 + newHash;
  };
  const auto pred2 = [](size_t acc, const QualType &val) {
    // Combine the previous hash value with the current element's hash, adjusted by a prime number to reduce collisions
    return acc * 31 + std::hash<QualType>{}(val);
  };
  // Calculate the cache key
  const uint64_t scopeHash = std::hash<Scope *>{}(scope);
  const uint64_t hashName = std::hash<std::string>{}(name);
  const uint64_t hashThisType = std::hash<QualType>{}(thisType);
  const uint64_t hashArgs = std::accumulate(args.begin(), args.end(), 0u, pred1);
  const uint64_t hashTemplateTypes = std::accumulate(templateTypes.begin(), templateTypes.end(), 0u, pred2);
  return scopeHash ^ (hashName << 1) ^ (hashThisType << 2) ^ (hashArgs << 3) ^ (hashTemplateTypes << 4);
}

/**
 * Clear all statics
 */
void FunctionManager::clear() { lookupCache.clear(); }

} // namespace spice::compiler
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "FunctionManager.h"

#include <ast/ASTNodes.h>
#include <scope/Scope.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/TypeChecker.h>

Function *FunctionManager::insertFunction(Scope *insertScope, const Function &baseFunction, const ASTNode *declNode,
                                          std::vector<Function *> *nodeFunctionList) {
  // Open a new manifestation list for the function definition
  insertScope->functions.insert({baseFunction.declNode->codeLoc.toString(), std::unordered_map<std::string, Function>()});

  // Collect substantiations
  std::vector<Function> manifestations;
  substantiateOptionalParams(baseFunction, manifestations);
  assert(!manifestations.empty());

  // Save substantiations
  for (const Function &manifestation : manifestations) {
    Function *manifestationPtr = insertSubstantiation(insertScope, manifestation, declNode);
    assert(manifestationPtr != nullptr);
    if (nodeFunctionList)
      nodeFunctionList->push_back(manifestationPtr);
  }

  assert(!nodeFunctionList->empty());
  return nodeFunctionList->front();
}

/**
 * Create definite functions from ambiguous ones, in regards to optional arguments.
 *
 * Example:
 * int testFunc(string, int?)
 * gets
 * int testFunc(string)
 * int testFunc(string, int)
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

FunctionManifestationList *FunctionManager::getManifestationList(Scope *lookupScope, const CodeLoc &defCodeLoc) {
  std::string codeLocStr = defCodeLoc.toString();
  return lookupScope->functions.contains(codeLocStr) ? &lookupScope->functions.at(codeLocStr) : nullptr;
}

Function *FunctionManager::insertSubstantiation(Scope *insertScope, const Function &substantiatedFunction,
                                                const ASTNode *declNode) {
  assert(substantiatedFunction.hasSubstantiatedParams());

  const std::string mangledFctName = substantiatedFunction.getMangledName();
  const std::string codeLocStr = declNode->codeLoc.toString();
  const std::string signature = substantiatedFunction.getSignature();

  // Check if the function exists already
  for (const auto &[_, manifestations] : insertScope->functions)
    if (manifestations.contains(mangledFctName))
      throw SemanticError(declNode, FUNCTION_DECLARED_TWICE, "The function/procedure '" + signature + "' is declared twice");

  // Retrieve the matching manifestation list of the scope
  assert(insertScope->functions.contains(codeLocStr));
  FunctionManifestationList &manifestationList = insertScope->functions.at(codeLocStr);

  // Add substantiated function
  manifestationList.emplace(mangledFctName, substantiatedFunction);
  return &manifestationList.at(mangledFctName);
}

/**
 * Check if there is a function in the scope, fulfilling all given requirements and if found, return it.
 * If more than one function matches the requirement, an error gets thrown.
 *
 * @param requestedName Function name requirement
 * @param requestedThisType This type requirement
 * @param templateTypeHints Template types to substantiate generic types
 * @param requestedParamTypes Argument types requirement
 * @param callNode Call AST node for printing error messages
 * @return Matched function or nullptr
 */
Function *FunctionManager::matchFunction(Scope *matchScope, const std::string &requestedName, const SymbolType &requestedThisType,
                                         const std::vector<SymbolType> &templateTypeHints,
                                         const std::vector<SymbolType> &requestedParamTypes, const ASTNode *callNode) {
  // Copy the registry to prevent iterating over items, that are created within the loop
  FunctionRegistry functionRegistry = matchScope->functions;
  // Loop over function registry to find functions, that match the requirements of the call
  std::vector<Function *> matches;
  for (const auto &[defCodeLocStr, m] : functionRegistry) {
    // Copy the manifestation list to prevent iterating over items, that are created within the loop
    const FunctionManifestationList manifestations = m;
    for (const auto &[mangledName, presetFunction] : manifestations) {
      assert(presetFunction.hasSubstantiatedParams()); // No optional params are allowed at this point

      // Copy the function to be able to substantiate types
      Function candidate = presetFunction;

      // Check name requirement
      if (!matchName(candidate, requestedName))
        break; // Leave the whole manifestation list, because all manifestations in this list have the same name

      // Create mapping table from generic type name to concrete type
      std::unordered_map</*name=*/std::string, /*concreteType=*/SymbolType> concreteTemplateTypes;
      concreteTemplateTypes.reserve(templateTypeHints.size());
      for (size_t i = 0; i < templateTypeHints.size(); i++)
        concreteTemplateTypes.insert({candidate.templateTypes.at(i).getName(), templateTypeHints.at(i)});

      // Check 'this' type requirement
      if (!matchThisType(candidate, requestedThisType, concreteTemplateTypes, callNode))
        continue; // Leave this manifestation and try the next one

      // Check arg types requirement
      if (!matchArgTypes(candidate, requestedParamTypes, concreteTemplateTypes, callNode))
        continue; // Leave this manifestation and try the next one

      // We found a match! -> Check if it needs to be substantiated
      if (presetFunction.templateTypes.empty()) {
        assert(functionRegistry.contains(defCodeLocStr) && functionRegistry.at(defCodeLocStr).contains(mangledName));
        matches.push_back(&functionRegistry.at(defCodeLocStr).at(mangledName));
        continue; // Match was successful -> match the next function
      }

      // Clear template types of candidate
      candidate.templateTypes.clear();

      // Check if we already have this manifestation and can simply re-use it
      if (manifestations.contains(candidate.getMangledName())) {
        matches.push_back(&functionRegistry.at(defCodeLocStr).at(candidate.getMangledName()));
        break; // Leave the whole manifestation list to not double-match the manifestation
      }

      // Insert the substantiated version if required
      Function *substantiatedFunction = insertSubstantiation(matchScope, candidate, presetFunction.declNode);
      substantiatedFunction->genericSubstantiation = true;
      // Copy function scope
      matchScope->copyChildScope(presetFunction.getSignature(), substantiatedFunction->getSignature());

      // Insert symbols for generic type names with concrete types into the child block
      Scope *childBlock = matchScope->getChildScope(substantiatedFunction->getSignature());
      for (const auto &[typeName, concreteType] : concreteTemplateTypes)
        childBlock->insertGenericType(typeName, GenericType(concreteType));

      // Substantiate the 'this' symbol table entry in the new function scope
      if (presetFunction.isMethod() && !presetFunction.templateTypes.empty()) {
        SymbolTableEntry *thisEntry = childBlock->lookupStrict(THIS_VARIABLE_NAME);
        assert(thisEntry != nullptr);
        thisEntry->updateType(requestedThisType.toPointer(callNode), /*overwriteExistingType=*/true);
      }

      // Add to matched functions
      matches.push_back(substantiatedFunction);
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
 * Uses the provided template type hints to deduce and fill in the return and argument types of a matching candidate
 *
 * @param candidate Matching candidate function
 * @param templateTypeHints Provided type hints
 */
void FunctionManager::applyTemplateTypeHints(Function &candidate, const std::vector<SymbolType> &templateTypeHints,
                                             TypeMapping &concreteTemplateTypes) {
  assert(templateTypeHints.size() <= candidate.templateTypes.size());

  // Substantiate return type
  const std::string returnTypeName = candidate.returnType.getBaseType().getSubType();
  if (candidate.returnType.isBaseType(TY_GENERIC) && concreteTemplateTypes.contains(returnTypeName))
    candidate.returnType = candidate.returnType.replaceBaseType(concreteTemplateTypes.at(returnTypeName));

  // Substantiate param types
  for (auto &[paramType, optional] : candidate.paramList) {
    assert(!optional); // All parameters should be substantiated at this point
    const std::string paramTypeName = paramType.getBaseType().getSubType();
    // Replace base type with concrete type
    if (paramType.isBaseType(TY_GENERIC) && concreteTemplateTypes.contains(paramTypeName))
      paramType = paramType.replaceBaseType(concreteTemplateTypes.at(paramTypeName));
  }
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
 * @return Fulfilled or not
 */
bool FunctionManager::matchThisType(Function &candidate, const SymbolType &requestedThisType, TypeMapping &typeMapping,
                                    const ASTNode *callNode) {
  assert(candidate.thisType.is(TY_STRUCT) && requestedThisType.is(TY_STRUCT)); // The 'this' type must always be a struct

  // If the candidate 'this' type is non-generic, we can simpy check for equality of the types
  if (candidate.thisType.getTemplateTypes().empty())
    return candidate.thisType == requestedThisType;

  const std::vector<SymbolType> &candidateTemplateTypeList = candidate.thisType.getTemplateTypes();
  if (requestedThisType.getTemplateTypes().size() != candidateTemplateTypeList.size())
    return false;

  // Substantiate 'this' type
  std::vector<SymbolType> concreteTemplateTypes;
  concreteTemplateTypes.reserve(candidate.thisType.getTemplateTypes().size());
  for (size_t i = 0; i < requestedThisType.getTemplateTypes().size(); i++) {
    const SymbolType &templateType = candidateTemplateTypeList.at(i);
    const SymbolType &concreteTemplateType = requestedThisType.getTemplateTypes().at(i);

    // If the template type is non-generic, check it directly
    if (!templateType.is(TY_GENERIC)) {
      if (templateType != concreteTemplateType)
        return false;
      concreteTemplateTypes.push_back(templateType);
      continue;
    }

    // Append to concrete template types mapping
    if (!typeMapping.contains(templateType.getSubType()))
      typeMapping.insert({templateType.getSubType(), concreteTemplateType});
    else if (typeMapping.at(templateType.getSubType()) != concreteTemplateType)
      return false;

    // Add to the list of concrete template types
    concreteTemplateTypes.push_back(concreteTemplateType);
  }

  // Mount the new concrete template type list to the 'this' type
  candidate.thisType.setTemplateTypes(concreteTemplateTypes);

  return true;
}

/**
 * Checks if the matching candidate fulfills the argument types requirement
 *
 * @param candidate Matching candidate function
 * @param requestedParamTypes Requested argument types
 * @return Fulfilled or not
 */
bool FunctionManager::matchArgTypes(Function &candidate, const std::vector<SymbolType> &requestedParamTypes,
                                    TypeMapping &typeMapping, const ASTNode *callNode) {
  // If the number of arguments does not match with the number of params, the matching fails
  if (requestedParamTypes.size() != candidate.paramList.size())
    return false;

  // Loop over all parameters
  for (size_t i = 0; i < requestedParamTypes.size(); i++) {
    // Retrieve actual and requested types
    const SymbolType &requestedParamType = requestedParamTypes.at(i);
    SymbolType &paramType = candidate.paramList.at(i).type;

    // Check if we need to substantiate generic types
    if (!paramType.isBaseType(TY_GENERIC)) {
      if (paramType != requestedParamType)
        return false;
      continue;
    }

    // Check if we have an information mismatch
    if (typeMapping.contains(paramType.getSubType()) && requestedParamType != typeMapping.at(paramType.getSubType()))
      return false;

    // Substantiate generic type with the requested type
    paramType = paramType.replaceBaseType(requestedParamType.getBaseType());
  }

  return true;
}
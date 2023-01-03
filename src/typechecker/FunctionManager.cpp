// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "FunctionManager.h"

#include <ast/ASTNodes.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/TypeChecker.h>

namespace spice::compiler {

Function *FunctionManager::insertFunction(Scope *insertScope, const Function &baseFunction,
                                          std::vector<Function *> *nodeFunctionList) {
  // Open a new manifestation list for the function definition
  insertScope->functions.insert({baseFunction.declNode->codeLoc.toString(), std::unordered_map<std::string, Function>()});

  // Collect substantiations
  std::vector<Function> manifestations;
  substantiateOptionalParams(baseFunction, manifestations);
  assert(!manifestations.empty());

  // Save substantiations
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

FunctionManifestationList *FunctionManager::getManifestationList(Scope *lookupScope, const CodeLoc &defCodeLoc) {
  const std::string codeLocStr = defCodeLoc.toString();
  return lookupScope->functions.contains(codeLocStr) ? &lookupScope->functions.at(codeLocStr) : nullptr;
}

Function FunctionManager::createMainFunction(SymbolTableEntry *entry, const std::vector<SymbolType> &paramTypes,
                                             ASTNode *declNode) {
  ParamList paramList;
  for (const SymbolType &paramType : paramTypes)
    paramList.push_back({paramType, true});
  return {MAIN_FUNCTION_NAME, entry, SymbolType(TY_DYN), SymbolType(TY_INT), paramList, {}, declNode, false};
}

Function *FunctionManager::insertSubstantiation(Scope *insertScope, const Function &newManifestation, const ASTNode *declNode) {
  assert(newManifestation.hasSubstantiatedParams());

  const std::string mangledFctName = newManifestation.getMangledName();
  const std::string codeLocStr = declNode->codeLoc.toString();
  const std::string signature = newManifestation.getSignature();

  // Check if the function exists already
  for (const auto &[_, manifestations] : insertScope->functions)
    if (manifestations.contains(mangledFctName))
      throw SemanticError(declNode, FUNCTION_DECLARED_TWICE, "The function/procedure '" + signature + "' is declared twice");

  // Retrieve the matching manifestation list of the scope
  assert(insertScope->functions.contains(codeLocStr));
  FunctionManifestationList &manifestationList = insertScope->functions.at(codeLocStr);

  // Add substantiated function
  manifestationList.emplace(mangledFctName, newManifestation);
  return &manifestationList.at(mangledFctName);
}

/**
 * Check if there is a function in the scope, fulfilling all given requirements and if found, return it.
 * If more than one function matches the requirement, an error gets thrown.
 *
 * @param matchScope Scope to match against
 * @param requestedName Function name requirement
 * @param requestedThisType This type requirement
 * @param templateTypeHints Template types to substantiate generic types
 * @param requestedParamTypes Argument types requirement
 * @param callNode Call AST node for printing error messages
 * @return Matched function or nullptr
 */
Function *FunctionManager::matchFunction(Scope *matchScope, const std::string &requestedName, const SymbolType &requestedThisType,
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

      // Skip generic substantiations to prevent double matching of a function
      if (presetFunction.genericSubstantiation)
        continue;

      // Copy the function to be able to substantiate types
      Function candidate = presetFunction;

      // Check name requirement
      if (!matchName(candidate, requestedName))
        break; // Leave the whole manifestation list, because all manifestations in this list have the same name

      // Prepare mapping table from generic type name to concrete type
      std::unordered_map</*name=*/std::string, /*concreteType=*/SymbolType> typeMapping;
      typeMapping.reserve(candidate.templateTypes.size());

      // Check 'this' type requirement
      if (!matchThisType(candidate, requestedThisType, typeMapping))
        continue; // Leave this manifestation and try the next one

      // Check arg types requirement
      if (!matchArgTypes(candidate, requestedParamTypes, typeMapping))
        continue; // Leave this manifestation and try the next one

      // We found a match! -> Check if it needs to be substantiated
      if (presetFunction.templateTypes.empty()) {
        assert(matchScope->functions.contains(defCodeLocStr) && matchScope->functions.at(defCodeLocStr).contains(mangledName));
        matches.push_back(&matchScope->functions.at(defCodeLocStr).at(mangledName));
        continue; // Match was successful -> match the next function
      }

      // Clear template types of candidate, since they are not needed anymore
      candidate.templateTypes.clear();

      // Replace return type with concrete type
      if (candidate.returnType.isBaseType(TY_GENERIC)) {
        const std::string genericReturnTypeName = candidate.returnType.getBaseType().getSubType();
        assert(typeMapping.contains(genericReturnTypeName));
        candidate.returnType = candidate.returnType.replaceBaseType(typeMapping.at(genericReturnTypeName));
      }

      // Check if we already have this manifestation and can simply re-use it
      if (manifestations.contains(candidate.getMangledName())) {
        matches.push_back(&matchScope->functions.at(defCodeLocStr).at(candidate.getMangledName()));
        break; // Leave the whole manifestation list to not double-match the manifestation
      }

      // Insert the substantiated version if required
      Function *substantiatedFunction = insertSubstantiation(matchScope, candidate, presetFunction.declNode);
      substantiatedFunction->genericSubstantiation = true;
      substantiatedFunction->declNode->getFctManifestations()->push_back(substantiatedFunction);

      // Copy function scope
      matchScope->copyChildScope(presetFunction.getSignature(), substantiatedFunction->getSignature());

      // Insert symbols for generic type names with concrete types into the child block
      Scope *childBlock = matchScope->getChildScope(substantiatedFunction->getSignature());
      for (const auto &[typeName, concreteType] : typeMapping)
        childBlock->insertGenericType(typeName, GenericType(concreteType));

      // Substantiate the 'this' symbol table entry in the new function scope
      if (presetFunction.isMethod() && !presetFunction.templateTypes.empty()) {
        SymbolTableEntry *thisEntry = childBlock->lookupStrict(THIS_VARIABLE_NAME);
        assert(thisEntry != nullptr);
        thisEntry->updateType(requestedThisType.toPointer(callNode), /*overwriteExistingType=*/true);
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
 * @return Fulfilled or not
 */
bool FunctionManager::matchThisType(Function &candidate, const SymbolType &requestedThisType, TypeMapping &typeMapping) {
  const SymbolType &candidateThisType = candidate.thisType;
  const SymbolType &requestedThisBaseType = requestedThisType.getBaseType();

  // If the candidate 'this' type is non-generic, we can simpy check for equality of the types
  if (candidateThisType.getTemplateTypes().empty())
    return candidateThisType == requestedThisBaseType;

  assert(candidateThisType.is(TY_STRUCT) && requestedThisBaseType.is(TY_STRUCT)); // The 'this' type must always be a struct

  // Compare this type template type list sizes
  const std::vector<SymbolType> &candidateTemplateTypeList = candidateThisType.getTemplateTypes();
  if (requestedThisBaseType.getTemplateTypes().size() != candidateTemplateTypeList.size())
    return false;

  // Substantiate 'this' type
  std::vector<SymbolType> concreteTemplateTypes;
  concreteTemplateTypes.reserve(candidateThisType.getTemplateTypes().size());
  for (size_t i = 0; i < requestedThisBaseType.getTemplateTypes().size(); i++) {
    const SymbolType &templateType = candidateTemplateTypeList.at(i);
    const SymbolType &concreteTemplateType = requestedThisBaseType.getTemplateTypes().at(i);

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
  candidate.thisType.setBaseTemplateTypes(concreteTemplateTypes);

  return true;
}

/**
 * Checks if the matching candidate fulfills the argument types requirement
 *
 * @param candidate Matching candidate function
 * @param requestedParamTypes Requested argument types
 * @param typeMapping Concrete template type mapping
 * @return Fulfilled or not
 */
bool FunctionManager::matchArgTypes(Function &candidate, const std::vector<SymbolType> &requestedParamTypes,
                                    TypeMapping &typeMapping) {
  // If the number of arguments does not match with the number of params, the matching fails
  if (requestedParamTypes.size() != candidate.paramList.size())
    return false;

  // Loop over all parameters
  for (size_t i = 0; i < requestedParamTypes.size(); i++) {
    // Retrieve actual and requested types
    SymbolType requestedParamType = requestedParamTypes.at(i);
    SymbolType paramType = candidate.paramList.at(i).type;

    // Unwrap both types
    while (paramType.isSameContainerTypeAs(requestedParamType)) {
      requestedParamType = requestedParamType.getContainedTy();
      paramType = paramType.getContainedTy();
    }

    // Check if we need to substantiate generic types
    if (!paramType.isBaseType(TY_GENERIC)) {
      if (paramType == requestedParamType)
        continue;
      return false;
    }

    // Check if we have an information mismatch
    const std::string genericTypeName = paramType.getSubType();
    if (typeMapping.contains(genericTypeName)) { // Known type name
      const SymbolType &concreteType = typeMapping.at(paramType.getSubType());

      // Check if the concrete type matches the requested type
      if (concreteType != requestedParamType)
        return false;

      candidate.paramList.at(i) = Param{concreteType, false};
      continue;
    }

    const GenericType *genericType = getGenericTypeByNameFromCandidate(candidate, genericTypeName);
    if (!genericType->checkConditionsOf(requestedParamType))
      return false;

    // Add new type mapping entry
    typeMapping.insert({genericTypeName, requestedParamType});

    // Substantiate generic type with the requested type
    const SymbolType &newParamType = paramType.replaceBaseType(requestedParamType.getBaseType());
    candidate.paramList.at(i) = Param{newParamType, false};
  }

  return true;
}

/**
 * Searches the candidate template types for a generic type object with a certain name and return it
 *
 * @param candidate Matching candidate function
 * @param templateTypeName Template type name
 * @return Generic type object
 */
const GenericType *FunctionManager::getGenericTypeByNameFromCandidate(const Function &candidate,
                                                                      const std::string &templateTypeName) {
  for (const auto &templateType : candidate.templateTypes) {
    if (templateType.getSubType() == templateTypeName)
      return &templateType;
  }
  return nullptr;
}

} // namespace spice::compiler

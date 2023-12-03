// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/ScopeHandle.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/TypeMatcher.h>

namespace spice::compiler {

std::any TypeChecker::visitMainFctDefCheck(MainFctDefNode *node) {
  // Skip if already type-checked
  if (typeCheckedMainFct)
    return nullptr;

  node->resizeToNumberOfManifestations(1);

  // Change to function body scope
  currentScope = node->fctScope;
  // Visit statements in new scope
  visit(node->body());
  // Leave main function body scope
  currentScope = rootScope;

  // Set to type-checked
  typeCheckedMainFct = true;
  return nullptr;
}

std::any TypeChecker::visitFctDefCheck(FctDefNode *node) {
  node->resizeToNumberOfManifestations(node->manifestations.size());
  manIdx = 0; // Reset the manifestation index

  // Get all manifestations for this function definition
  for (Function *manifestation : node->manifestations) {
    // Skip non-substantiated or already checked functions
    if (!manifestation->isFullySubstantiated() || manifestation->alreadyTypeChecked) {
      manIdx++; // Increase the manifestation index
      continue;
    }

    // Change scope to concrete struct specialization scope
    if (node->isMethod) {
      const auto structSignature = Struct::getSignature(node->name->structName, manifestation->thisType.getTemplateTypes());
      changeToScope(STRUCT_SCOPE_PREFIX + structSignature, ScopeType::STRUCT);
    }

    // Change to function scope
    changeToScope(manifestation->getSignature(false), ScopeType::FUNC_PROC_BODY);

    // Mount type mapping for this manifestation
    assert(typeMapping.empty());
    typeMapping = manifestation->typeMapping;

    // Set return type to the result variable
    SymbolTableEntry *resultVarEntry = currentScope->lookupStrict(RETURN_VARIABLE_NAME);
    assert(resultVarEntry != nullptr);
    resultVarEntry->updateType(manifestation->returnType, false);
    resultVarEntry->used = true;

    // Visit parameters
    // This happens once in the type checker prepare stage. This second time is only required if we have a generic function
    if (node->hasParams)
      visit(node->paramLst());

    // Visit statements in new scope
    visit(node->body());

    // Clear type mapping
    typeMapping.clear();

    // Change to root scope
    currentScope = rootScope;
    assert(currentScope->type == ScopeType::GLOBAL);

    // Do not type-check this manifestation again
    manifestation->alreadyTypeChecked = true;

    manIdx++; // Increase the manifestation index
  }
  manIdx = 0; // Reset the manifestation index

  return nullptr;
}

std::any TypeChecker::visitProcDefCheck(ProcDefNode *node) {
  node->resizeToNumberOfManifestations(node->manifestations.size());
  manIdx = 0; // Reset the manifestation index

  // Get all manifestations for this procedure definition
  for (auto &manifestation : node->manifestations) {
    // Skip non-substantiated or already checked procedures
    if (!manifestation->isFullySubstantiated() || manifestation->alreadyTypeChecked) {
      manIdx++; // Increase the manifestation index
      continue;
    }

    // Change scope to concrete struct specialization scope
    if (node->isMethod) {
      const auto structSignature = Struct::getSignature(node->name->structName, manifestation->thisType.getTemplateTypes());
      changeToScope(STRUCT_SCOPE_PREFIX + structSignature, ScopeType::STRUCT);
    }

    // Change to procedure scope
    changeToScope(manifestation->getSignature(false), ScopeType::FUNC_PROC_BODY);

    // Mount type mapping for this manifestation
    assert(typeMapping.empty());
    typeMapping = manifestation->typeMapping;

    // Visit parameters
    // This happens once in the type checker prepare stage. This second time is only required if we have a generic procedure
    if (node->hasParams)
      visit(node->paramLst());

    // Visit statements in new scope
    visit(node->body());

    // Clear type mapping
    typeMapping.clear();

    // Change to root scope
    currentScope = rootScope;
    assert(currentScope != nullptr && currentScope->type == ScopeType::GLOBAL);

    // Do not type-check this manifestation again
    manifestation->alreadyTypeChecked = true;

    manIdx++; // Increase the manifestation index
  }
  manIdx = 0; // Reset the manifestation index

  return nullptr;
}

std::any TypeChecker::visitStructDefCheck(StructDefNode *node) {
  node->resizeToNumberOfManifestations(node->structManifestations.size());
  manIdx = 0; // Reset the manifestation index

  // Get all manifestations for this procedure definition
  for (auto &manifestation : node->structManifestations) {
    // Skip non-substantiated or already checked procedures
    if (!manifestation->isFullySubstantiated()) {
      manIdx++; // Increase the manifestation index
      continue;
    }

    // Change to struct scope
    changeToScope(manifestation->scope, ScopeType::STRUCT);

    // Build struct type
    const SymbolType structType = manifestation->entry->getType();

    // Check if the struct implements all methods of all attached interfaces
    size_t vtableIndex = 0;
    for (const SymbolType &interfaceType : manifestation->interfaceTypes) {
      // Retrieve interface instance
      const std::string interfaceName = interfaceType.getSubType();
      Scope *matchScope = interfaceType.getBodyScope()->parent;
      Interface *interface = InterfaceManager::matchInterface(matchScope, interfaceName, interfaceType.getTemplateTypes(), node);
      assert(interface != nullptr);

      // Check for all methods, that it is implemented by the struct
      for (const Function *expectedMethod : interface->methods) {
        const std::string methodName = expectedMethod->name;
        std::vector<SymbolType> params = expectedMethod->getParamTypes();
        SymbolType returnType = expectedMethod->returnType;

        // Substantiate
        TypeMatcher::substantiateTypesWithTypeMapping(params, interface->typeMapping);
        if (returnType.hasAnyGenericParts())
          TypeMatcher::substantiateTypeWithTypeMapping(returnType, interface->typeMapping);

        Function *spiceFunction = FunctionManager::matchFunction(currentScope, methodName, structType, params, true, nullptr);
        if (!spiceFunction) {
          softError(node, INTERFACE_METHOD_NOT_IMPLEMENTED,
                    "The struct '" + node->structName + "' does not implement method '" + expectedMethod->getSignature() + "'");
          continue;
        }

        // Set to virtual, since it overrides the interface method
        spiceFunction->isVirtual = true;
        spiceFunction->vtableIndex = vtableIndex++;
      }
    }

    // Return to the root scope
    currentScope = rootScope;
    assert(currentScope != nullptr && currentScope->type == ScopeType::GLOBAL);

    manIdx++; // Increase the manifestation index
  }
  manIdx = 0; // Reset the manifestation index

  return nullptr;
}

} // namespace spice::compiler
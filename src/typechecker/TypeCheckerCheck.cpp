// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

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
  node->resizeToNumberOfManifestations(node->fctManifestations.size());
  manIdx = 0; // Reset the manifestation index

  // Get all manifestations for this function definition
  for (Function *manifestation : node->fctManifestations) {
    // Skip non-substantiated or already checked functions
    if (!manifestation->isFullySubstantiated() || manifestation->alreadyTypeChecked) {
      manIdx++; // Increase the manifestation index
      continue;
    }

    // Change scope to concrete struct specialization scope
    if (node->isMethod) {
      const std::string structSignature = Struct::getSignature(node->structName, manifestation->thisType.getTemplateTypes());
      currentScope = rootScope->getChildScope(STRUCT_SCOPE_PREFIX + structSignature);
      assert(currentScope != nullptr && currentScope->type == SCOPE_STRUCT);
    }

    // Change to function scope
    currentScope = currentScope->getChildScope(manifestation->getSignature());
    assert(currentScope != nullptr && currentScope->type == SCOPE_FUNC_PROC_BODY);

    // Mount type mapping for this manifestation
    assert(typeMapping.empty());
    typeMapping = manifestation->typeMapping;

    // Set return type to the result variable
    SymbolTableEntry *resultVarEntry = currentScope->lookupStrict(RETURN_VARIABLE_NAME);
    assert(resultVarEntry != nullptr);
    resultVarEntry->updateType(manifestation->returnType, false);
    resultVarEntry->used = true;

    // Visit parameters
    if (node->hasParams)
      visit(node->paramLst());

    // Visit statements in new scope
    visit(node->body());

    // Clear type mapping
    typeMapping.clear();

    // Change to root scope
    currentScope = rootScope;
    assert(currentScope->type == SCOPE_GLOBAL);

    // Do not type-check this manifestation again
    manifestation->alreadyTypeChecked = true;

    manIdx++; // Increase the manifestation index
  }
  manIdx = 0; // Reset the manifestation index

  return nullptr;
}

std::any TypeChecker::visitProcDefCheck(ProcDefNode *node) {
  node->resizeToNumberOfManifestations(node->procManifestations.size());
  manIdx = 0; // Reset the manifestation index

  // Get all manifestations for this procedure definition
  for (auto &manifestation : node->procManifestations) {
    // Skip non-substantiated or already checked procedures
    if (!manifestation->isFullySubstantiated() || manifestation->alreadyTypeChecked) {
      manIdx++; // Increase the manifestation index
      continue;
    }

    // Change scope to concrete struct specialization scope
    if (node->isMethod) {
      const std::string structSignature = Struct::getSignature(node->structName, manifestation->thisType.getTemplateTypes());
      currentScope = rootScope->getChildScope(STRUCT_SCOPE_PREFIX + structSignature);
      assert(currentScope != nullptr && currentScope->type == SCOPE_STRUCT);
    }

    // Change to procedure scope
    currentScope = currentScope->getChildScope(manifestation->getSignature());
    assert(currentScope != nullptr && currentScope->type == SCOPE_FUNC_PROC_BODY);

    // Mount type mapping for this manifestation
    assert(typeMapping.empty());
    typeMapping = manifestation->typeMapping;

    // Visit parameters
    if (node->hasParams)
      visit(node->paramLst());

    // Visit statements in new scope
    visit(node->body());

    // Clear type mapping
    typeMapping.clear();

    // Change to root scope
    currentScope = rootScope;
    assert(currentScope != nullptr && currentScope->type == SCOPE_GLOBAL);

    // Do not type-check this manifestation again
    manifestation->alreadyTypeChecked = true;

    manIdx++; // Increase the manifestation index
  }
  manIdx = 0; // Reset the manifestation index

  return nullptr;
}

std::any TypeChecker::visitStructDefCheck(StructDefNode *node) {
  // Change to struct scope
  currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + node->structName);
  assert(currentScope != nullptr && currentScope->type == SCOPE_STRUCT);

  // Check if the struct implements all methods of all attached interfaces
  for (const SymbolType &interfaceType : node->spiceStruct->interfaceTypes) {
    // Lookup interface by its name
    const Interface *interface = rootScope->lookupInterface(interfaceType.getSubType());
    assert(interface != nullptr);

    for (const Function *expectedMethod : interface->methods) {
      // Check if the struct implements the method
      Function *actualMethod = FunctionManager::matchFunction(
          currentScope, expectedMethod->name, node->spiceStruct->entry->getType(), expectedMethod->getParamTypes(), node);
      if (!actualMethod)
        throw SemanticError(node, INTERFACE_METHOD_NOT_IMPLEMENTED,
                            "The struct '" + node->structName + "' does not implement the method '" +
                                expectedMethod->getSignature() + "', requested of interface '" + interface->name + "'");
    }
  }

  // Return to the root scope
  currentScope = rootScope;
  assert(currentScope != nullptr && currentScope->type == SCOPE_GLOBAL);

  return nullptr;
}

} // namespace spice::compiler
// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/ScopeHandle.h>
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
      const auto structSignature = Struct::getSignature(node->fctName->structName, manifestation->thisType.getTemplateTypes());
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
      const auto structSignature = Struct::getSignature(node->procName->structName, manifestation->thisType.getTemplateTypes());
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
  // Change to generic struct scope
  currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + node->structName);
  assert(currentScope != nullptr && currentScope->type == ScopeType::STRUCT);

  // Check if the struct implements all methods of all attached interfaces
  for (const SymbolType &interfaceType : node->structManifestations.front()->interfaceTypes) {
    // Retrieve interface instance
    const std::string interfaceName = interfaceType.getOriginalSubType();
    Scope *matchScope = interfaceType.getBodyScope()->parent;
    Interface *interface = InterfaceManager::matchInterface(matchScope, interfaceName, interfaceType.getTemplateTypes(), node);
    assert(interface != nullptr);

    // Check for all methods, that it is implemented by the struct
    for (const Function *expectedMethod : interface->methods) {
      const std::string methodName = expectedMethod->name;
      const std::vector<SymbolType> params = expectedMethod->getParamTypes();
      const SymbolType &returnType = expectedMethod->returnType;
      bool success = FunctionManager::matchInterfaceMethod(currentScope, methodName, params, returnType, true);
      if (!success)
        softError(node, INTERFACE_METHOD_NOT_IMPLEMENTED,
                  "The struct '" + node->structName + "' does not implement the method '" + expectedMethod->getSignature() +
                      "', requested of interface '" + interface->name + "'");
    }
  }

  // Return to the root scope
  currentScope = rootScope;
  assert(currentScope != nullptr && currentScope->type == ScopeType::GLOBAL);

  return nullptr;
}

} // namespace spice::compiler
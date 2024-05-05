// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

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
  currentScope = node->bodyScope;
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
      const auto structSignature =
          Struct::getSignature(node->name->structName, manifestation->thisType.getType().getTemplateTypes());
      changeToScope(STRUCT_SCOPE_PREFIX + structSignature, ScopeType::STRUCT);
    }

    // Change to function scope
    changeToScope(manifestation->bodyScope, ScopeType::FUNC_PROC_BODY);

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
      const auto structSignature =
          Struct::getSignature(node->name->structName, manifestation->thisType.getType().getTemplateTypes());
      changeToScope(STRUCT_SCOPE_PREFIX + structSignature, ScopeType::STRUCT);
    }

    // Change to procedure scope
    changeToScope(manifestation->bodyScope, ScopeType::FUNC_PROC_BODY);

    // Mount type mapping for this manifestation
    assert(typeMapping.empty());
    typeMapping = manifestation->typeMapping;

    // Visit parameters
    // This happens once in the type checker prepare stage. This second time is only required if we have a generic procedure
    if (node->hasParams)
      visit(node->paramLst());

    // Prepare generation of special ctor preamble to store VTable, default field values, etc. if required
    if (node->isCtor)
      createCtorBodyPreamble(node->scope);

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

    // Re-visit all default values. This is required, since the type of the default value might vary for different manifestations
    for (const FieldNode *field : node->fields())
      if (field->defaultValue() != nullptr)
        visit(field->defaultValue());

    // Build struct type
    const QualType structType = manifestation->entry->getQualType();

    // Check if the struct implements all methods of all attached interfaces
    size_t vtableIndex = 0;
    for (const QualType &interfaceType : manifestation->interfaceTypes) {
      // Retrieve interface instance
      const std::string interfaceName = interfaceType.getType().getSubType();
      Scope *matchScope = interfaceType.getType().getBodyScope()->parent;
      Interface *interface =
          InterfaceManager::matchInterface(matchScope, interfaceName, interfaceType.getType().getTemplateTypes(), node);
      assert(interface != nullptr);

      // Check for all methods, that it is implemented by the struct
      for (const Function *expectedMethod : interface->methods) {
        const std::string methodName = expectedMethod->name;
        std::vector<QualType> params = expectedMethod->getParamTypes();
        Type returnType = expectedMethod->returnType;

        // Substantiate
        TypeMatcher::substantiateTypesWithTypeMapping(params, interface->typeMapping);
        if (returnType.hasAnyGenericParts())
          TypeMatcher::substantiateTypeWithTypeMapping(returnType, interface->typeMapping);

        // Build args list
        ArgList args;
        args.reserve(params.size());
        for (const QualType &param : params)
          args.emplace_back(param, nullptr);

        Function *spiceFunction = FunctionManager::matchFunction(currentScope, methodName, structType, args, {}, true, node);
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

    // Generate default ctor body if required
    const Function *ctorFunc = FunctionManager::lookupFunction(currentScope, CTOR_FUNCTION_NAME, structType, {}, true);
    if (ctorFunc != nullptr && ctorFunc->implicitDefault)
      createDefaultCtorBody(ctorFunc);

    // Generate default copy ctor body if required
    const ArgList args = {{structType.toConstRef(node), false /* always non-temporary */}};
    const Function *copyCtorFunc = FunctionManager::lookupFunction(currentScope, CTOR_FUNCTION_NAME, structType, args, true);
    if (copyCtorFunc != nullptr && copyCtorFunc->implicitDefault)
      createDefaultCopyCtorBody(copyCtorFunc);

    // Generate default dtor body if required
    const Function *dtorFunc = FunctionManager::lookupFunction(currentScope, DTOR_FUNCTION_NAME, structType, {}, true);
    if (dtorFunc != nullptr && dtorFunc->implicitDefault)
      createDefaultDtorBody(dtorFunc);

    // Return to the root scope
    currentScope = rootScope;
    assert(currentScope != nullptr && currentScope->type == ScopeType::GLOBAL);

    manIdx++; // Increase the manifestation index
  }
  manIdx = 0; // Reset the manifestation index

  return nullptr;
}

} // namespace spice::compiler
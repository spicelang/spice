// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

std::any TypeChecker::visitMainFctDefCheck(MainFctDefNode *node) {
  // Update main function symbol type
  SymbolTableEntry *functionEntry = rootScope->lookupStrict(node->getSignature());
  assert(functionEntry != nullptr);
  functionEntry->updateType(SymbolType(TY_FUNCTION), false);
  functionEntry->isUsed = true;

  // Change to function body scope
  currentScope = node->fctScope;

  // Set type of 'result' variable to int
  SymbolTableEntry *resultEntry = currentScope->lookupStrict(RETURN_VARIABLE_NAME);
  assert(resultEntry != nullptr);
  resultEntry->updateType(SymbolType(TY_INT), false);
  resultEntry->isUsed = true;

  // Visit param list
  if (node->takesArgs)
    visit(node->paramLst());

  // Visit statements in new scope
  visit(node->stmtLst());

  // Leave main function body scope
  currentScope = rootScope;

  return nullptr;
}

std::any TypeChecker::visitFctDefCheck(FctDefNode *node) {
  // Get all manifestations for this function definition
  Scope *fctParentScope = node->isMethod ? node->structScope : currentScope;
  assert(fctParentScope != nullptr);
  std::unordered_map<std::string, Function> *manifestations = fctParentScope->getFunctionManifestations(node->codeLoc);

  if (manifestations) {
    node->symbolTypeIndex = 0; // Reset the symbolTypeIndex

    for (auto &[mangledName, manifestation] : *manifestations) {
      // Skip non-substantiated or already checked functions
      if (!manifestation.isFullySubstantiated() || manifestation.isAlreadyTypeChecked)
        continue;

      // Change scope to concrete struct specialization scope
      if (node->isMethod) {
        const std::string structSignature = Struct::getSignature(node->structName, manifestation.thisType.getTemplateTypes());
        currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + structSignature);
        assert(currentScope != nullptr);
      }

      // Change to function scope
      currentScope = currentScope->getChildScope(manifestation.getSignature());
      assert(currentScope != nullptr);

      // Substantiate the generic return type if necessary
      SymbolTableEntry *returnVarEntry = currentScope->lookup(RETURN_VARIABLE_NAME);
      if (returnVarEntry->getType().isBaseType(TY_GENERIC)) {
        const SymbolType newReturnType = returnVarEntry->getType().replaceBaseType(manifestation.returnType);
        returnVarEntry->updateType(newReturnType, true);
      }

      // Substantiate all generic parameter types if necessary and save the old types to restore them later
      NamedParamList namedParams;
      if (node->hasParams) {
        const std::vector<SymbolType> newParamTypes = manifestation.getParamTypes();
        for (int i = 0; i < newParamTypes.size(); i++) {
          // Get param declaration node
          const DeclStmtNode *paramDecl = node->paramLst()->params()[i];
          // Retrieve the symbol table entry for that param
          SymbolTableEntry *paramEntry = currentScope->lookup(paramDecl->varName);
          assert(paramEntry != nullptr);
          // Check if we need to substantiate this type
          if (!paramEntry->getType().is(TY_GENERIC))
            continue;
          // Save the old type
          namedParams.push_back({paramDecl->varName, paramEntry->getType()});
          // Substantiate the type
          paramEntry->updateType(newParamTypes[i], true);
        }
      }

      // Visit statements in new scope
      visit(node->body());

      // Reset param types to their generic versions
      for (const NamedParam &param : namedParams) {
        SymbolTableEntry *paramEntry = currentScope->lookup(param.name);
        assert(paramEntry != nullptr);
        paramEntry->updateType(param.type, true);
      }

      // Change to root scope
      currentScope = rootScope;

      // Do not type-check this manifestation again
      manifestation.isAlreadyTypeChecked = true;

      // Increase the symbolTypeIndex
      node->symbolTypeIndex++;
    }
  }

  return nullptr;
}

std::any TypeChecker::visitProcDefCheck(ProcDefNode *node) {
  // Get all manifestations for this procedure definition
  Scope *procParentScope = node->isMethod ? node->structScope : currentScope;
  assert(procParentScope != nullptr);
  std::unordered_map<std::string, Function> *manifestations = procParentScope->getFunctionManifestations(node->codeLoc);

  if (manifestations) {
    node->symbolTypeIndex = 0; // Reset the symbolTypeIndex

    for (auto &[mangledName, manifestation] : *manifestations) {
      // Skip non-substantiated or already checked procedures
      if (!manifestation.isFullySubstantiated() || manifestation.isAlreadyTypeChecked)
        continue;

      // Change scope to concrete struct specialization scope
      if (node->isMethod) {
        const std::string structSignature = Struct::getSignature(node->structName, manifestation.thisType.getTemplateTypes());
        currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + structSignature);
        assert(currentScope != nullptr);
      }

      // Change to procedure scope
      currentScope = currentScope->getChildScope(manifestation.getSignature());
      assert(currentScope != nullptr);

      // Substantiate all generic parameter types if necessary and save the old types to restore them later
      NamedParamList namedParams;
      if (node->hasParams) {
        const std::vector<SymbolType> newParamTypes = manifestation.getParamTypes();
        for (int i = 0; i < newParamTypes.size(); i++) {
          // Get param declaration node
          const DeclStmtNode *paramDecl = node->paramLst()->params()[i];
          // Retrieve the symbol table entry for that param
          SymbolTableEntry *paramEntry = currentScope->lookup(paramDecl->varName);
          assert(paramEntry != nullptr);
          // Check if we need to substantiate this type
          if (!paramEntry->getType().is(TY_GENERIC))
            continue;
          // Save the old type
          namedParams.push_back({paramDecl->varName, paramEntry->getType()});
          // Substantiate the type
          paramEntry->updateType(newParamTypes[i], true);
        }
      }

      // Visit statements in new scope
      visit(node->body());

      // Reset param types to their generic versions
      for (const NamedParam &param : namedParams) {
        SymbolTableEntry *paramEntry = currentScope->lookup(param.name);
        assert(paramEntry != nullptr);
        paramEntry->updateType(param.type, true);
      }

      // Change to root scope
      currentScope = rootScope;

      // Do not type-check this manifestation again
      manifestation.isAlreadyTypeChecked = true;

      // Increase the symbolTypeIndex
      node->symbolTypeIndex++;
    }
  }

  return nullptr;
}

std::any TypeChecker::visitStructDefCheck(StructDefNode *node) {
  // Change to struct scope
  currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + node->structName);

  // Check if the struct implements all methods of all attached interfaces
  for (const SymbolType &interfaceType : node->spiceStruct->interfaceTypes) {
    // Lookup interface by its name
    const Interface *interface = rootScope->lookupInterface(interfaceType.getSubType());
    assert(interface != nullptr);

    for (const Function *expectedMethod : interface->methods) {
      // Check if the struct implements the method
      Function *actualMethod = currentScope->matchFunction(expectedMethod->name, node->spiceStruct->getSymbolType(), {},
                                                           expectedMethod->getParamTypes(), node);
      if (!actualMethod)
        throw SemanticError(node, INTERFACE_METHOD_NOT_IMPLEMENTED,
                            "The struct '" + node->structName + "' does not implement the method '" +
                                expectedMethod->getSignature() + "', requested of interface '" + interface->name + "'");
    }
  }

  // Return to the root scope
  currentScope = rootScope;

  return nullptr;
}
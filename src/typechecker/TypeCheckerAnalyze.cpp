// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

std::any TypeChecker::visitMainFctDefAnalyze(MainFctDefNode *node) {
  // Do down into function scope
  currentScope = node->fctScope;

  // Visit statements in new scope
  visit(node->stmtLst());

  // Return to root scope
  currentScope = rootScope;

  return {};
}

std::any TypeChecker::visitFctDefAnalyze(FctDefNode *node) {
  // Get manifestations of that function
  Scope *fctParentScope = node->isMethod ? node->structScope : currentScope;
  assert(fctParentScope != nullptr);
  std::unordered_map<std::string, Function> *manifestations = fctParentScope->getFunctionManifestations(node->codeLoc);

  // Set the symbolTypeIndex to 0
  node->symbolTypeIndex = 0;

  if (manifestations) {
    for (auto &[mangledName, spiceFunc] : *manifestations) {
      // Check if the function is substantiated
      if (!spiceFunc.isFullySubstantiated())
        continue;

      if (spiceFunc.isAlreadyAnalyzed)
        continue;

      // Change scope to the struct specialization
      if (node->isMethod) {
        std::string structSignature = Struct::getSignature(node->structName, spiceFunc.thisType.getTemplateTypes());
        currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + structSignature);
        assert(currentScope);
      }

      // Change scope to function scope
      currentScope = currentScope->getChildScope(spiceFunc.getSignature());
      assert(currentScope != nullptr);

      // Replace this type
      /*if (spiceFunc.isMethodFunction()) {
        SymbolTableEntry *thisVar = currentScope->lookup(THIS_VARIABLE_NAME);
        assert(thisVar != nullptr);
        thisVar->updateType(spiceFunc.getThisType(), true);
      }*/

      // Morph the generic return type
      SymbolTableEntry *returnVarEntry = currentScope->lookup(RETURN_VARIABLE_NAME);
      if (returnVarEntry->type.is(TY_GENERIC)) {
        SymbolType returnType = spiceFunc.returnType;
        returnVarEntry->updateType(returnType, true);
      }

      // Get argument types
      std::vector<std::pair<std::string, SymbolType>> args;
      if (node->hasParams) {
        for (const auto paramDecl : node->paramLst()->params()) {
          SymbolTableEntry *argEntry = currentScope->lookup(paramDecl->varName);
          assert(argEntry);
          args.emplace_back(paramDecl->varName, argEntry->type);
        }
      }

      // Morph the generic types to the replacements
      std::vector<SymbolType> newArgTypes = spiceFunc.getParamTypes();
      for (int i = 0; i < newArgTypes.size(); i++) {
        SymbolTableEntry *argEntry = currentScope->lookup(args[i].first);
        argEntry->updateType(newArgTypes[i], true);
      }

      // Visit statements in new scope
      visit(node->body());

      // Reset generic types
      for (const auto &arg : args) {
        SymbolTableEntry *argEntry = currentScope->lookup(arg.first);
        assert(argEntry);
        argEntry->updateType(arg.second, true);
      }

      // Leave the function scope
      currentScope = currentScope->parent;

      // Leave the struct scope
      if (node->isMethod)
        currentScope = currentScope->parent;

      spiceFunc.isAlreadyAnalyzed = true;

      // Increase the symbolTypeIndex
      node->symbolTypeIndex++;
    }
  }

  return {};
}

std::any TypeChecker::visitProcDefAnalyze(ProcDefNode *node) {
  // Get manifestations of that procedure
  Scope *procParentScope = node->isMethod ? node->structScope : currentScope;
  assert(procParentScope != nullptr);
  std::unordered_map<std::string, Function> *manifestations = procParentScope->getFunctionManifestations(node->codeLoc);

  // Set the symbolTypeIndex to 0
  node->symbolTypeIndex = 0;

  if (manifestations) {
    for (auto &[mangledName, spiceProc] : *manifestations) {
      // Check if the function is substantiated
      if (!spiceProc.isFullySubstantiated())
        continue;

      if (spiceProc.isAlreadyAnalyzed)
        continue;

      // Change scope to the struct specialization
      if (node->isMethod) {
        std::string structSignature = Struct::getSignature(node->structName, spiceProc.thisType.getTemplateTypes());
        currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + structSignature);
        assert(currentScope);
      }

      // Go down again in scope
      currentScope = currentScope->getChildScope(spiceProc.getSignature());
      assert(currentScope != nullptr);

      // Replace this type
      /*if (spiceProc.isMethodProcedure()) {
        SymbolTableEntry *thisVar = currentScope->lookup(THIS_VARIABLE_NAME);
        assert(thisVar != nullptr);
        thisVar->updateType(spiceProc.getThisType(), true);
      }*/

      // Get parameter types
      std::vector<std::pair<std::string, SymbolType>> params;
      if (node->paramLst()) {
        for (const auto paramDecl : node->paramLst()->params()) {
          SymbolTableEntry *paramEntry = currentScope->lookup(paramDecl->varName);
          assert(paramEntry);
          params.emplace_back(paramDecl->varName, paramEntry->type);
        }
      }

      // Morph the generic types to the replacements
      std::vector<SymbolType> newArgTypes = spiceProc.getParamTypes();
      for (int i = 0; i < newArgTypes.size(); i++) {
        SymbolTableEntry *argEntry = currentScope->lookup(params[i].first);
        argEntry->updateType(newArgTypes[i], true);
      }

      // Visit statements in new scope
      visit(node->body());

      // Reset generic types
      for (const auto &arg : params) {
        SymbolTableEntry *argEntry = currentScope->lookup(arg.first);
        assert(argEntry);
        argEntry->updateType(arg.second, true);
      }

      // Leave the function scope
      currentScope = currentScope->parent;

      // Leave the struct scope
      if (node->isMethod)
        currentScope = currentScope->parent;

      spiceProc.isAlreadyAnalyzed = true;

      // Increase the symbolTypeIndex
      node->symbolTypeIndex++;
    }
  }

  return {};
}

std::any TypeChecker::visitStructDefAnalyze(StructDefNode *node) {
  // Change to struct scope
  currentScope = currentScope->getChildScope(STRUCT_SCOPE_PREFIX + node->structName);

  // Check if the struct implements all methods of all attached interfaces
  for (const SymbolType &interfaceType : node->spiceStruct->interfaceTypes) {
    // Lookup interface by its name
    Interface *interface = rootScope->lookupInterface(interfaceType.getSubType());
    assert(interface != nullptr);

    for (const Function *expectedMethod : interface->methods) {
      // Check if the struct implements the method
      Function *actualMethod = currentScope->matchFunction(expectedMethod->name, node->spiceStruct->getSymbolType(), {},
                                                           expectedMethod->getParamTypes(), node);
      if (actualMethod == nullptr)
        throw SemanticError(node, INTERFACE_METHOD_NOT_IMPLEMENTED,
                            "The struct '" + node->structName + "' does not implement the method '" +
                                expectedMethod->getSignature() + "', requested of interface '" + interface->name + "'");
    }
  }

  // Return to the old scope
  currentScope = currentScope->parent;

  return {};
}
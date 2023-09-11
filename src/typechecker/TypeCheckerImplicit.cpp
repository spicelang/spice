// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

/**
 * Checks if the given struct scope already has a default constructor and creates a default one if not
 *
 * @param entry SymbolTableEntry of the struct
 * @param structScope Scope of the struct
 */
void TypeChecker::createDefaultDtorIfRequired(Struct &spiceStruct, Scope *structScope) {
  ASTNode *node = spiceStruct.declNode;
  assert(structScope != nullptr && structScope->type == ScopeType::STRUCT);

  // Abort if the struct already has a user-defined destructor
  const SymbolTableEntry *structEntry = spiceStruct.entry;
  const SymbolType &thisType = structEntry->getType();
  Function *dtorFunction = FunctionManager::matchFunction(structScope, DTOR_FUNCTION_NAME, thisType, {}, true, nullptr);
  if (dtorFunction)
    return;

  // Insert symbol for function into the symbol table
  const std::string fqFctName = thisType.getOriginalSubType() + MEMBER_ACCESS_TOKEN + DTOR_FUNCTION_NAME;
  SymbolTableEntry *fctEntry = structScope->insert(DTOR_FUNCTION_NAME, structEntry->declNode);

  // Add to external name registry
  assert(sourceFile->getNameRegistryEntry(fqFctName) == nullptr);
  sourceFile->addNameRegistryEntry(fqFctName, fctEntry, structScope, /*keepNewOnCollision=*/true);

  // Create the default destructor function
  const std::vector<GenericType> templateTypes = spiceStruct.templateTypes;
  Function defaultDtor(DTOR_FUNCTION_NAME, fctEntry, thisType, SymbolType(TY_DYN), {}, templateTypes, structEntry->declNode);
  defaultDtor.implicitDefault = true;

  // Create function scope
  Scope *fctScope = structScope->createChildScope(defaultDtor.getSignature(false), ScopeType::FUNC_PROC_BODY, &node->codeLoc);
  defaultDtor.bodyScope = fctScope;

  // Create 'this' symbol in the function scope
  SymbolTableEntry *thisEntry = fctScope->insert(THIS_VARIABLE_NAME, node);
  thisEntry->updateType(thisType.toPointer(node), true);

  // Hand it off to the function manager to register the function
  std::vector<Function *> manifestations;
  FunctionManager::insertFunction(structScope, defaultDtor, &manifestations);

  // Request memory runtime if we have fields, that are allocated on the heap
  bool hasHeapFields = false;
  const size_t fieldCount = structScope->getFieldCount();
  for (size_t i = 0; i < fieldCount; i++)
    hasHeapFields |= structScope->symbolTable.lookupStrictByIndex(i)->getType().isHeap();
  if (hasHeapFields)
    sourceFile->requestRuntimeModule(MEMORY_RT);
}

/**
 * Prepare the generation of a call to the dtor of a given struct
 *
 * @param entry Symbol entry to use as 'this' pointer for the dtor call
 * @param node StmtLstNode for the current scope
 */
void TypeChecker::implicitlyCallStructDtor(SymbolTableEntry *entry, StmtLstNode *node) {
  SymbolType thisType = entry->getType();
  assert(thisType.is(TY_STRUCT));
  Scope *matchScope = thisType.getBodyScope();
  assert(matchScope->type == ScopeType::STRUCT);

  // Search for dtor
  const bool isImported = matchScope->isImportedBy(rootScope);
  if (isImported)
    thisType = mapLocalTypeToImportedScopeType(matchScope, thisType);
  Function *spiceFunc = FunctionManager::matchFunction(matchScope, DTOR_FUNCTION_NAME, thisType, {}, true, node);

  // Add the dtor to the stmt list node to call it later in codegen
  if (spiceFunc != nullptr)
    node->dtorFunctions.at(manIdx).emplace_back(entry, spiceFunc);
}

/**
 * Consider calls to destructors for the given scope
 *
 * @param node StmtLstNode for the current scope
 */
void TypeChecker::doScopeCleanup(StmtLstNode *node) {
  // Get all variables, that are approved for deallocation
  std::vector<SymbolTableEntry *> vars = currentScope->getVarsGoingOutOfScope();
  for (SymbolTableEntry *var : vars) {
    // Only generate dtor call for structs and if not omitted
    if (!var->getType().is(TY_STRUCT) || var->omitDtorCall)
      continue;
    // Variable must be either initialized or a struct field
    if (!var->isInitialized() && var->scope->type != ScopeType::STRUCT)
      continue;
    // Call dtor
    implicitlyCallStructDtor(var, node);
  }
}

} // namespace spice::compiler
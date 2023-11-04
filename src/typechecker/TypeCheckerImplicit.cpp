// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

static const char *const FCT_NAME_DEALLOC = "sDealloc";

/**
 * Create a default struct method
 * Checks if the given struct scope already has an user-defined constructor and creates a default one if not.
 *
 * @param entry SymbolTableEntry of the struct
 * @param structScope Scope of the struct
 * @param params Parameter types of the method
 */
void TypeChecker::createDefaultStructMethod(const Struct &spiceStruct, const std::string &methodName, const ParamList &params) {
  Scope *structScope = spiceStruct.scope;
  ASTNode *node = spiceStruct.declNode;
  const SymbolTableEntry *structEntry = spiceStruct.entry;
  const SymbolType &thisType = structEntry->getType();
  const std::string fqFctName = thisType.getOriginalSubType() + MEMBER_ACCESS_TOKEN + methodName;

  // Procedure type
  SymbolType procedureType(TY_PROCEDURE);
  procedureType.specifiers.isPublic = structEntry->getType().specifiers.isPublic;

  // Insert symbol for function into the symbol table
  const std::string entryName = Function::getSymbolTableEntryName(methodName, node->codeLoc);
  SymbolTableEntry *fctEntry = structScope->insert(entryName, structEntry->declNode);
  fctEntry->updateType(procedureType, true);

  // Add to external name registry
  sourceFile->addNameRegistryEntry(fqFctName, fctEntry, structScope, /*keepNewOnCollision=*/true);

  // Create the default method
  const std::vector<GenericType> templateTypes = spiceStruct.templateTypes;
  const SymbolType returnType(TY_DYN);
  Function defaultMethod(methodName, fctEntry, thisType, returnType, params, templateTypes, structEntry->declNode);
  defaultMethod.implicitDefault = true;

  // Create function scope
  Scope *fctScope = structScope->createChildScope(defaultMethod.getSignature(false), ScopeType::FUNC_PROC_BODY, &node->codeLoc);
  defaultMethod.bodyScope = fctScope;

  // Create 'this' symbol in the function scope
  SymbolTableEntry *thisEntry = fctScope->insert(THIS_VARIABLE_NAME, node);
  thisEntry->updateType(thisType.toPointer(node), true);
  thisEntry->used = true; // Always set to used to not print warnings for non-existing code

  // Hand it off to the function manager to register the function
  FunctionManager::insertFunction(structScope, defaultMethod, structEntry->declNode->getFctManifestations(methodName));
}

/**
 * Checks if the given struct scope already has an user-defined constructor and creates a default one if not.
 *
 * @param entry SymbolTableEntry of the struct
 * @param structScope Scope of the struct
 */
void TypeChecker::createDefaultCtorIfRequired(const Struct &spiceStruct, Scope *structScope) {
  ASTNode *node = spiceStruct.declNode;
  assert(structScope != nullptr && structScope->type == ScopeType::STRUCT);

  // Abort if the struct already has a user-defined constructor
  const SymbolTableEntry *structEntry = spiceStruct.entry;
  const SymbolType &thisType = structEntry->getType();
  const std::string fqFctName = thisType.getOriginalSubType() + MEMBER_ACCESS_TOKEN + CTOR_FUNCTION_NAME;
  if (sourceFile->getNameRegistryEntry(fqFctName))
    return;

  // Check if we have fields, that require us to do anything in the ctor
  const size_t fieldCount = structScope->getFieldCount();
  bool hasFieldsWithDefaultValue = false;
  bool hasFieldsToConstruct = false;
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
    const SymbolType &thisType = fieldSymbol->getType();

    // Abort if we have a field, that is a reference
    if (thisType.isRef())
      return;

    if (auto fieldNode = dynamic_cast<FieldNode *>(fieldSymbol->declNode)) {
      hasFieldsWithDefaultValue |= fieldNode->defaultValue() != nullptr;
      if (fieldSymbol->getType().is(TY_STRUCT)) {
        Scope *fieldScope = fieldSymbol->getType().getBodyScope();
        // Lookup ctor function
        const Function *ctorFct = FunctionManager::matchFunction(fieldScope, CTOR_FUNCTION_NAME, thisType, {}, true, node);
        hasFieldsToConstruct |= ctorFct != nullptr;
        requestRevisitIfRequired(ctorFct);
      }
    } else {
      assert(dynamic_cast<DataTypeNode *>(fieldSymbol->declNode) != nullptr);
      if (fieldSymbol->getType().is(TY_STRUCT)) {
        Scope *fieldScope = fieldSymbol->getType().getBodyScope();
        // Lookup ctor function
        const Function *ctorFct = FunctionManager::matchFunction(fieldScope, CTOR_FUNCTION_NAME, thisType, {}, true, node);
        hasFieldsToConstruct |= ctorFct != nullptr;
        requestRevisitIfRequired(ctorFct);
      }
    }
  }

  // If we don't have any fields, that require us to do anything in the ctor, we can skip it
  if (!hasFieldsWithDefaultValue && !hasFieldsToConstruct)
    return;

  // Create the default ctor function
  createDefaultStructMethod(spiceStruct, CTOR_FUNCTION_NAME, {});
}

/**
 * Checks if the given struct scope already has an user-defined constructor and creates a default one if not.
 *
 * @param entry SymbolTableEntry of the struct
 * @param structScope Scope of the struct
 */
void TypeChecker::createDefaultCopyCtorIfRequired(const Struct &spiceStruct, Scope *structScope) {
  ASTNode *node = spiceStruct.declNode;
  assert(structScope != nullptr && structScope->type == ScopeType::STRUCT);

  // Abort if the struct already has a user-defined constructor
  const SymbolTableEntry *structEntry = spiceStruct.entry;
  const SymbolType &thisType = structEntry->getType();
  const std::string fqFctName = thisType.getOriginalSubType() + MEMBER_ACCESS_TOKEN + CTOR_FUNCTION_NAME;
  if (sourceFile->getNameRegistryEntry(fqFctName))
    return;

  // Check if we have fields, that require us to do anything in the ctor
  const size_t fieldCount = structScope->getFieldCount();
  bool hasFieldsToCopyConstruct = false;
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
    const SymbolType &thisType = fieldSymbol->getType();

    // Abort if we have a field, that is a reference
    if (thisType.isRef())
      return;

    if (fieldSymbol->getType().is(TY_STRUCT)) {
      Scope *fieldScope = fieldSymbol->getType().getBodyScope();
      // Lookup ctor function
      const std::vector<SymbolType> paramTypes = {thisType.toConstReference(node)};
      const Function *ctorFct = FunctionManager::matchFunction(fieldScope, CTOR_FUNCTION_NAME, thisType, paramTypes, true, node);
      hasFieldsToCopyConstruct |= ctorFct != nullptr;
      requestRevisitIfRequired(ctorFct);
    }
  }

  // If we don't have any fields, that require us to do anything in the copy ctor, we can skip it
  if (!hasFieldsToCopyConstruct)
    return;

  // Create the default copy ctor function
  const ParamList paramTypes = {{thisType.toConstReference(node), false}};
  createDefaultStructMethod(spiceStruct, CTOR_FUNCTION_NAME, paramTypes);
}

/**
 * Checks if the given struct scope already has an user-defined destructor and creates a default one if not.
 *
 * @param entry SymbolTableEntry of the struct
 * @param structScope Scope of the struct
 */
void TypeChecker::createDefaultDtorIfRequired(const Struct &spiceStruct, Scope *structScope) {
  ASTNode *node = spiceStruct.declNode;
  assert(structScope != nullptr && structScope->type == ScopeType::STRUCT);

  // Abort if the struct already has a user-defined destructor
  const SymbolTableEntry *structEntry = spiceStruct.entry;
  const SymbolType &thisType = structEntry->getType();
  const std::string fqFctName = thisType.getOriginalSubType() + MEMBER_ACCESS_TOKEN + DTOR_FUNCTION_NAME;
  if (sourceFile->getNameRegistryEntry(fqFctName))
    return;

  // Check we have field types, that require use to do anything in the destructor
  const size_t fieldCount = structScope->getFieldCount();
  bool hasHeapFields = false;
  bool hasFieldsToDestruct = false;
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
    hasHeapFields |= fieldSymbol->getType().isHeap();
    if (fieldSymbol->getType().is(TY_STRUCT)) {
      Scope *fieldScope = fieldSymbol->getType().getBodyScope();
      // Lookup dtor function
      const SymbolType &thisType = fieldSymbol->getType();
      const Function *dtorFct = FunctionManager::matchFunction(fieldScope, DTOR_FUNCTION_NAME, thisType, {}, true, node);
      hasFieldsToDestruct |= dtorFct != nullptr;
      requestRevisitIfRequired(dtorFct);
    }
  }

  // If we don't have any fields, that require us to do anything in the dtor, we can skip it
  if (!hasHeapFields && !hasFieldsToDestruct)
    return;

  // Create the default dtor function
  createDefaultStructMethod(spiceStruct, DTOR_FUNCTION_NAME, {});

  // Request memory runtime if we have fields, that are allocated on the heap
  if (hasHeapFields) {
    SourceFile *memoryRT = sourceFile->requestRuntimeModule(MEMORY_RT);
    assert(memoryRT != nullptr);
    Scope *matchScope = memoryRT->globalScope.get();
    // Set dealloc function to used
    const SymbolType thisType(TY_DYN);
    SymbolType bytePtrRefType = SymbolType(TY_BYTE).toPointer(node).toReference(node);
    bytePtrRefType.specifiers.isHeap = true;
    const std::vector<SymbolType> paramTypes = {bytePtrRefType};
    Function *deallocFct = FunctionManager::matchFunction(matchScope, FCT_NAME_DEALLOC, thisType, paramTypes, true, node);
    assert(deallocFct != nullptr);
    deallocFct->used = true;
  }
}

/**
 * Prepare the generation of a call to a method of a given struct
 *
 * @param entry Symbol entry to use as 'this' pointer for the method call
 * @param methodName Name of the method to call
 * @param paramTypes Parameter types of the method to call
 * @param node AST node
 */
Function *TypeChecker::implicitlyCallStructMethod(SymbolTableEntry *entry, const std::string &methodName,
                                                  const std::vector<SymbolType> &paramTypes, const ASTNode *node) {
  SymbolType thisType = entry->getType();
  assert(thisType.is(TY_STRUCT));
  Scope *matchScope = thisType.getBodyScope();
  assert(matchScope->type == ScopeType::STRUCT);

  // Search for dtor
  const bool isImported = matchScope->isImportedBy(rootScope);
  if (isImported)
    thisType = mapLocalTypeToImportedScopeType(matchScope, thisType);
  return FunctionManager::matchFunction(matchScope, methodName, thisType, paramTypes, true, node);
}

/**
 * Prepare the generation of a call to the copy ctor of a given struct
 *
 * @param entry Symbol entry to use as 'this' pointer for the copy ctor call
 * @param node Current AST node
 */
void TypeChecker::implicitlyCallStructCopyCtor(SymbolTableEntry *entry, const ASTNode *node) {
  implicitlyCallStructMethod(entry, CTOR_FUNCTION_NAME, {entry->getType().toConstReference(node)}, node);
}

/**
 * Prepare the generation of a call to the dtor of a given struct
 *
 * @param entry Symbol entry to use as 'this' pointer for the dtor call
 * @param node StmtLstNode for the current scope
 */
void TypeChecker::implicitlyCallStructDtor(SymbolTableEntry *entry, StmtLstNode *node) {
  // Add the dtor to the stmt list node to call it later in codegen
  Function *spiceFunc = implicitlyCallStructMethod(entry, DTOR_FUNCTION_NAME, {}, node);
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
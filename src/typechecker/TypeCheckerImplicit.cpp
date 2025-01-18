// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <ast/ASTBuilder.h>
#include <ast/ASTNodes.h>
#include <global/GlobalResourceManager.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/TypeMatcher.h>

namespace spice::compiler {

static const char *const FCT_NAME_DEALLOC = "sDealloc";

/**
 * Create a default struct method
 * Checks if the given struct scope already has an user-defined constructor and creates a default one if not.
 *
 * @param spiceStruct Struct instance
 * @param name Name of the method to create
 * @param params Parameter types of the method
 */
void TypeChecker::createDefaultStructMethod(const Struct &spiceStruct, const std::string &name, const ParamList &params) const {
  Scope *structScope = spiceStruct.scope;
  ASTNode *node = spiceStruct.declNode;
  const SymbolTableEntry *structEntry = spiceStruct.entry;
  const QualType &structType = structEntry->getQualType();
  const std::string fqFctName = structType.getSubType() + MEMBER_ACCESS_TOKEN + name;

  // Procedure type
  QualType procedureType(TY_PROCEDURE);
  procedureType.makePublic(); // Always public

  // Insert symbol for function into the symbol table
  const std::string entryName = Function::getSymbolTableEntryName(name, node->codeLoc);
  SymbolTableEntry *procEntry = structScope->insert(entryName, structEntry->declNode);
  procEntry->updateType(procedureType, true);

  // Add to external name registry
  sourceFile->addNameRegistryEntry(fqFctName, TY_PROCEDURE, procEntry, structScope, true);

  // Create the default method
  const std::vector<GenericType> templateTypes = spiceStruct.templateTypes;
  const QualType returnType(TY_DYN);
  Function defaultMethod(name, procEntry, structType, returnType, params, templateTypes, structEntry->declNode);
  defaultMethod.implicitDefault = true;

  // Create function scope
  Scope *procScope = structScope->createChildScope(defaultMethod.getSignature(false), ScopeType::FUNC_PROC_BODY, &node->codeLoc);
  defaultMethod.bodyScope = procScope;

  // Create 'this' symbol in the function scope
  SymbolTableEntry *thisEntry = procScope->insert(THIS_VARIABLE_NAME, node);
  thisEntry->updateType(structType.toPtr(node), true);
  thisEntry->used = true; // Always set to used to not print warnings for non-existing code

  // Hand it off to the function manager to register the function
  FunctionManager::insert(structScope, defaultMethod, structEntry->declNode->getFctManifestations(name));
}

/**
 * Checks if the given struct scope already has an user-defined constructor and creates a default one if not.
 *
 * For generating a default ctor, the following conditions need to be met:
 * - No user-defined constructors
 *
 * @param spiceStruct Struct instance
 * @param structScope Scope of the struct
 */
void TypeChecker::createDefaultCtorIfRequired(const Struct &spiceStruct, Scope *structScope) {
  const auto node = spice_pointer_cast<StructDefNode *>(spiceStruct.declNode);
  assert(structScope != nullptr && structScope->type == ScopeType::STRUCT);

  // Abort if the struct already has a user-defined constructor
  const SymbolTableEntry *structEntry = spiceStruct.entry;
  const QualType &structType = structEntry->getQualType();
  const std::string fqFctName = structType.getSubType() + MEMBER_ACCESS_TOKEN + CTOR_FUNCTION_NAME;
  if (sourceFile->getNameRegistryEntry(fqFctName))
    return;

  // Check if we have fields, that require us to do anything in the ctor
  const size_t fieldCount = structScope->getFieldCount();
  bool hasFieldsWithDefaultValue = false;
  bool hasFieldsToConstruct = false;
  for (size_t i = 0; i < fieldCount; i++) {
    const SymbolTableEntry *fieldSymbol = structScope->lookupField(i);
    assert(fieldSymbol != nullptr);
    const QualType &thisType = fieldSymbol->getQualType();

    // Abort if we have a field, that is a reference
    if (thisType.isRef())
      return;

    if (const auto fieldNode = dynamic_cast<FieldNode *>(fieldSymbol->declNode)) {
      hasFieldsWithDefaultValue |= fieldNode->defaultValue != nullptr;
    } else {
      assert(dynamic_cast<DataTypeNode *>(fieldSymbol->declNode) != nullptr);
    }

    const QualType fieldType = fieldSymbol->getQualType();
    if (fieldType.is(TY_STRUCT)) {
      Scope *bodyScope = fieldType.getBodyScope();
      const Struct *fieldStruct = fieldType.getStruct(node);
      // Check if we are required to call a ctor
      const auto structDeclNode = spice_pointer_cast<StructDefNode *>(fieldStruct->declNode);
      const bool isCtorCallRequired = bodyScope->hasRefFields() || structDeclNode->emitVTable;
      // Lookup ctor function
      const Function *ctorFct = FunctionManager::match(this, bodyScope, CTOR_FUNCTION_NAME, thisType, {}, {}, true, node);
      // If we are required to construct, but no constructor is found, we can't generate a default ctor for the outer struct
      if (!ctorFct && isCtorCallRequired)
        return;
      hasFieldsToConstruct |= ctorFct != nullptr;
    }
  }

  // If we don't have any fields, that require us to do anything in the ctor, we can skip it
  if (!hasFieldsWithDefaultValue && !hasFieldsToConstruct && !node->emitVTable)
    return;

  // Create the default ctor function
  createDefaultStructMethod(spiceStruct, CTOR_FUNCTION_NAME, {});
}

/**
 * Checks if the given struct scope already has an user-defined constructor and creates a default one if not.
 *
 * For generating a default copy ctor, the following conditions need to be met:
 * - No user-defined copy ctor
 * - No user-defined move ctor
 *
 * @param spiceStruct Struct instance
 * @param structScope Scope of the struct
 */
void TypeChecker::createDefaultCopyCtorIfRequired(const Struct &spiceStruct, Scope *structScope) {
  const auto node = spice_pointer_cast<const StructDefNode *>(spiceStruct.declNode);
  assert(structScope != nullptr && structScope->type == ScopeType::STRUCT);

  // Abort if the struct already has a user-defined copy constructor
  const QualType structType = spiceStruct.entry->getQualType();
  const ArgList lookupArgs = {{structType.toConstRef(node), true}};
  const Function *copyCtor = FunctionManager::lookup(structScope, CTOR_FUNCTION_NAME, structType, lookupArgs, true);
  if (copyCtor != nullptr)
    return;

  // Abort if the struct has a user-defined move constructor
  // ToDo: Check for move ctor

  // Check if we have fields, that require us to do anything in the ctor
  const size_t fieldCount = structScope->getFieldCount();
  bool copyCtorRequired = false;
  for (size_t i = 0; i < fieldCount; i++) {
    const SymbolTableEntry *fieldSymbol = structScope->lookupField(i);
    const QualType &fieldType = fieldSymbol->getQualType();

    // If the field is of type struct, check if this struct has a copy ctor that has to be called
    if (fieldType.is(TY_STRUCT)) {
      Scope *bodyScope = fieldType.getBodyScope();
      const Struct *fieldStruct = fieldType.getStruct(node);
      // Check if we are required to call a ctor
      const auto structDeclNode = spice_pointer_cast<StructDefNode *>(fieldStruct->declNode);
      const bool isCtorCallRequired = bodyScope->hasRefFields() || structDeclNode->emitVTable;
      // Lookup copy ctor function
      const ArgList args = {{fieldType.toConstRef(node), false /* we always have the field as storage */}};
      const Function *ctorFct = FunctionManager::match(this, bodyScope, CTOR_FUNCTION_NAME, fieldType, args, {}, true, node);
      // If we are required to construct, but no constructor is found, we can't generate a default ctor for the outer struct
      if (!ctorFct && isCtorCallRequired)
        return;
      copyCtorRequired |= ctorFct != nullptr;
    }

    // If we have a owning heap pointer, we need to do a memcpy of the heap storage and therefore need a default copy ctor
    if (fieldType.isHeap()) {
      assert(fieldType.isPtr());
      copyCtorRequired = true;
    }
  }

  // If we don't have any fields, that require us to do anything in the copy ctor, we can skip it
  if (!copyCtorRequired && !node->emitVTable)
    return;

  // Create the default copy ctor function
  const ParamList paramTypes = {{structType.toConstRef(node), false}};
  createDefaultStructMethod(spiceStruct, CTOR_FUNCTION_NAME, paramTypes);
}

/**
 * Checks if the given struct scope already has an user-defined destructor and creates a default one if not.
 *
 * For generating a default dtor, the following conditions need to be met:
 * - No user-defined dtor
 *
 * @param spiceStruct Struct instance
 * @param structScope Scope of the struct
 */
void TypeChecker::createDefaultDtorIfRequired(const Struct &spiceStruct, Scope *structScope) {
  const ASTNode *node = spiceStruct.declNode;
  assert(structScope != nullptr && structScope->type == ScopeType::STRUCT);

  // Abort if the struct already has a user-defined destructor
  const SymbolTableEntry *structEntry = spiceStruct.entry;
  const QualType &structType = structEntry->getQualType();
  const std::string fqFctName = structType.getSubType() + MEMBER_ACCESS_TOKEN + DTOR_FUNCTION_NAME;
  if (sourceFile->getNameRegistryEntry(fqFctName))
    return;

  // Check we have field types, that require use to do anything in the destructor
  const size_t fieldCount = structScope->getFieldCount();
  bool hasFieldsToDeAllocate = false;
  bool hasFieldsToDestruct = false;
  for (size_t i = 0; i < fieldCount; i++) {
    const SymbolTableEntry *fieldSymbol = structScope->lookupField(i);
    hasFieldsToDeAllocate |= fieldSymbol->getQualType().needsDeAllocation();
    if (fieldSymbol->getQualType().is(TY_STRUCT)) {
      Scope *fieldScope = fieldSymbol->getQualType().getBodyScope();
      // Lookup dtor function
      const QualType &thisType = fieldSymbol->getQualType();
      const Function *dtorFct = FunctionManager::match(this, fieldScope, DTOR_FUNCTION_NAME, thisType, {}, {}, true, node);
      hasFieldsToDestruct |= dtorFct != nullptr;
      requestRevisitIfRequired(dtorFct);
    }
  }

  // If we don't have any fields, that require us to do anything in the dtor, we can skip it
  if (!hasFieldsToDeAllocate && !hasFieldsToDestruct)
    return;

  // Create the default dtor function
  createDefaultStructMethod(spiceStruct, DTOR_FUNCTION_NAME, {});

  // Request memory runtime if we have fields, that are allocated on the heap
  // The string runtime does not use it, but allocates manually to avoid circular dependencies
  if (hasFieldsToDeAllocate && !sourceFile->isStringRT()) {
    const SourceFile *memoryRT = sourceFile->requestRuntimeModule(MEMORY_RT);
    assert(memoryRT != nullptr);
    Scope *matchScope = memoryRT->globalScope.get();
    // Set dealloc function to used
    const QualType thisType(TY_DYN);
    QualType bytePtrRefType = QualType(TY_BYTE).toPtr(node).toRef(node);
    bytePtrRefType.makeHeap();
    const ArgList args = {{bytePtrRefType, false /* we always have the field as storage */}};
    Function *deallocFct = FunctionManager::match(this, matchScope, FCT_NAME_DEALLOC, thisType, args, {}, true, node);
    assert(deallocFct != nullptr);
    deallocFct->used = true;
  }
}

/**
 * Prepare the generation of the ctor body preamble. This preamble is used to initialize the VTable, construct or initialize
 * fields.
 */
void TypeChecker::createCtorBodyPreamble(const Scope *bodyScope) {
  // Retrieve struct scope
  Scope *structScope = bodyScope->parent;
  assert(structScope != nullptr);

  const size_t fieldCount = structScope->getFieldCount();
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->lookupField(i);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());
    if (fieldSymbol->isImplicitField)
      continue;
    QualType fieldType = fieldSymbol->getQualType();
    if (fieldType.hasAnyGenericParts())
      TypeMatcher::substantiateTypeWithTypeMapping(fieldType, typeMapping, fieldSymbol->declNode);

    if (fieldType.is(TY_STRUCT)) {
      const auto fieldNode = spice_pointer_cast<const FieldNode *>(fieldSymbol->declNode);
      // Match ctor function, create the concrete manifestation and set it to used
      Scope *matchScope = fieldType.getBodyScope();
      const Function *spiceFunc =
          FunctionManager::match(this, matchScope, CTOR_FUNCTION_NAME, fieldType, {}, {}, false, fieldNode);
      if (spiceFunc != nullptr)
        fieldSymbol->updateType(fieldType.getWithBodyScope(spiceFunc->thisType.getBodyScope()), true);
    }
  }
}

/**
 * Prepare the generation of the copy ctor body preamble. This preamble is used to initialize the VTable, construct or initialize
 * fields.
 */
void TypeChecker::createCopyCtorBodyPreamble(const Scope *bodyScope) {
  // Retrieve struct scope
  Scope *structScope = bodyScope->parent;
  assert(structScope != nullptr);

  const size_t fieldCount = structScope->getFieldCount();
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->lookupField(i);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());
    if (fieldSymbol->isImplicitField)
      continue;

    QualType fieldType = fieldSymbol->getQualType();
    if (fieldType.hasAnyGenericParts())
      TypeMatcher::substantiateTypeWithTypeMapping(fieldType, typeMapping, fieldSymbol->declNode);

    if (fieldType.is(TY_STRUCT)) {
      const auto fieldNode = spice_pointer_cast<const FieldNode *>(fieldSymbol->declNode);
      // Match ctor function, create the concrete manifestation and set it to used
      Scope *matchScope = fieldType.getBodyScope();
      const ArgList args = {{fieldType.toConstRef(fieldNode), false /* we always have the field as storage */}};
      const Function *copyCtorFct =
          FunctionManager::match(this, matchScope, CTOR_FUNCTION_NAME, fieldType, args, {}, false, fieldNode);
      if (copyCtorFct != nullptr)
        fieldSymbol->updateType(fieldType.getWithBodyScope(copyCtorFct->thisType.getBodyScope()), true);
    }
  }
}

/**
 * Prepare the generation of the dtor body preamble. This preamble is used to destruct all fields and to free all heap fields.
 */
void TypeChecker::createDtorBodyPreamble(const Scope *bodyScope) {
  // Retrieve struct scope
  Scope *structScope = bodyScope->parent;
  assert(structScope != nullptr);

  const size_t fieldCount = structScope->getFieldCount();
  for (size_t i = 0; i < fieldCount; i++) {
    const SymbolTableEntry *fieldSymbol = structScope->lookupField(i);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());
    if (fieldSymbol->isImplicitField)
      continue;
    QualType fieldType = fieldSymbol->getQualType();
    if (fieldType.hasAnyGenericParts())
      TypeMatcher::substantiateTypeWithTypeMapping(fieldType, typeMapping, fieldSymbol->declNode);

    if (fieldType.is(TY_STRUCT)) {
      const auto fieldNode = spice_pointer_cast<const FieldNode *>(fieldSymbol->declNode);
      // Match ctor function, create the concrete manifestation and set it to used
      Scope *matchScope = fieldType.getBodyScope();
      FunctionManager::match(this, matchScope, DTOR_FUNCTION_NAME, fieldType, {}, {}, false, fieldNode);
    }
  }
}

/**
 * Prepare the generation of a call to a method of a given struct
 *
 * @param entry Symbol entry to use as 'this' pointer for the method call
 * @param methodName Name of the method to call
 * @param args Provided arguments by the caller
 * @param node AST node
 */
Function *TypeChecker::implicitlyCallStructMethod(const SymbolTableEntry *entry, const std::string &methodName,
                                                  const ArgList &args, const ASTNode *node) {
  QualType thisType = entry->getQualType().removeReferenceWrapper().toNonConst();
  assert(thisType.is(TY_STRUCT));
  Scope *matchScope = thisType.getBodyScope();
  assert(matchScope->type == ScopeType::STRUCT);

  // Search for dtor
  if (matchScope->isImportedBy(rootScope))
    thisType = mapLocalTypeToImportedScopeType(matchScope, thisType);
  return FunctionManager::match(this, matchScope, methodName, thisType, args, {}, true, node);
}

/**
 * Prepare the generation of a call to the copy ctor of a given struct
 *
 * @param entry Symbol entry to use as 'this' pointer for the copy ctor call
 * @param node Current AST node
 */
Function *TypeChecker::implicitlyCallStructCopyCtor(const SymbolTableEntry *entry, const ASTNode *node) {
  assert(entry != nullptr);
  const QualType argType = entry->getQualType().removeReferenceWrapper().toConstRef(node);
  const ArgList args = {{argType, false /* we always have an entry here */}};
  return implicitlyCallStructMethod(entry, CTOR_FUNCTION_NAME, args, node);
}

/**
 * Prepare the generation of a call to the dtor of a given struct
 *
 * @param entry Symbol entry to use as 'this' pointer for the dtor call
 * @param node StmtLstNode for the current scope
 */
void TypeChecker::implicitlyCallStructDtor(SymbolTableEntry *entry, StmtLstNode *node) {
  // Add the dtor to the stmt list node to call it later in codegen
  if (Function *dtor = implicitlyCallStructMethod(entry, DTOR_FUNCTION_NAME, {}, node))
    node->resourcesToCleanup.at(manIdx).dtorFunctionsToCall.emplace_back(entry, dtor);
}

/**
 * Prepare the generation of a call to the deallocate function for a heap-allocated variable
 *
 * @param node Current AST node for error messages
 */
void TypeChecker::implicitlyCallDeallocate(const ASTNode *node) {
  const SourceFile *memoryRT = sourceFile->requestRuntimeModule(MEMORY_RT);
  assert(memoryRT != nullptr);
  Scope *matchScope = memoryRT->globalScope.get();
  // Set dealloc function to used
  const QualType thisType(TY_DYN);
  QualType bytePtrRefType = QualType(TY_BYTE).toPtr(node).toRef(node);
  bytePtrRefType.makeHeap();
  const ArgList args = {{bytePtrRefType, false /* we always have the field as storage */}};
  Function *deallocFct = FunctionManager::match(this, matchScope, FCT_NAME_DEALLOC, thisType, args, {}, true, node);
  assert(deallocFct != nullptr);
  deallocFct->used = true;
}

/**
 * Consider calls to destructors for the given scope
 *
 * @param node StmtLstNode for the current scope
 */
void TypeChecker::doScopeCleanup(StmtLstNode *node) {
  // Get all variables, that are approved for de-allocation
  std::vector<SymbolTableEntry *> vars = currentScope->getVarsGoingOutOfScope();
  // Sort by reverse declaration order
  auto comp = [](const SymbolTableEntry *a, const SymbolTableEntry *b) { return a->declNode->codeLoc > b->declNode->codeLoc; };
  std::ranges::sort(vars, comp);
  // Call dtor for each variable. We call the dtor in reverse declaration order
  for (SymbolTableEntry *var : vars) {
    // Check if we have a heap-allocated pointer
    if (var->getQualType().isHeap() && var->getQualType().isOneOf({TY_PTR, TY_STRING, TY_FUNCTION, TY_PROCEDURE})) {
      // The memory runtime is ignored, because it manually allocates to avoid circular dependencies.
      // Same goes for the string runtime.
      if (sourceFile->isMemoryRT() || sourceFile->isStringRT())
        continue;
      // If the local variable currently does not have the ownership, we must not deallocate its memory
      if (!var->getLifecycle().isInOwningState())
        continue;

      implicitlyCallDeallocate(node); // Required to request the memory runtime
      node->resourcesToCleanup.at(manIdx).heapVarsToFree.push_back(var);
    }
    // Only generate dtor call for structs and if not omitted
    if (!var->getQualType().is(TY_STRUCT) || var->omitDtorCall)
      continue;
    // Variable must be either initialized or a struct field
    if (!var->getLifecycle().isInitialized() && var->scope->type != ScopeType::STRUCT)
      continue;
    // Call dtor
    implicitlyCallStructDtor(var, node);
  }
}

} // namespace spice::compiler
// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "TypeChecker.h"
#include "TypeMatcher.h"

#include <SourceFile.h>
#include <ast/ASTBuilder.h>
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
  const QualType &structType = structEntry->getQualType();
  const std::string fqFctName = structType.getSubType() + MEMBER_ACCESS_TOKEN + methodName;

  // Procedure type
  Type procedureType(TY_PROCEDURE);
  procedureType.specifiers.isPublic = true; // Always public

  // Insert symbol for function into the symbol table
  const std::string entryName = Function::getSymbolTableEntryName(methodName, node->codeLoc);
  SymbolTableEntry *fctEntry = structScope->insert(entryName, structEntry->declNode);
  fctEntry->updateType(procedureType, true);

  // Add to external name registry
  sourceFile->addNameRegistryEntry(fqFctName, TY_PROCEDURE, fctEntry, structScope, true);

  // Create the default method
  const std::vector<GenericType> templateTypes = spiceStruct.templateTypes;
  const QualType returnType(TY_DYN);
  Function defaultMethod(methodName, fctEntry, structType, returnType, params, templateTypes, structEntry->declNode);
  defaultMethod.implicitDefault = true;

  // Create function scope
  Scope *fctScope = structScope->createChildScope(defaultMethod.getSignature(false), ScopeType::FUNC_PROC_BODY, &node->codeLoc);
  defaultMethod.bodyScope = fctScope;

  // Create 'this' symbol in the function scope
  SymbolTableEntry *thisEntry = fctScope->insert(THIS_VARIABLE_NAME, node);
  thisEntry->updateType(structType.toPtr(node), true);
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
  auto node = spice_pointer_cast<StructDefNode *>(spiceStruct.declNode);
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
    SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
    const QualType &thisType = fieldSymbol->getQualType();

    // Abort if we have a field, that is a reference
    if (thisType.isRef())
      return;

    if (auto fieldNode = dynamic_cast<FieldNode *>(fieldSymbol->declNode)) {
      hasFieldsWithDefaultValue |= fieldNode->defaultValue() != nullptr;
    } else {
      assert(dynamic_cast<DataTypeNode *>(fieldSymbol->declNode) != nullptr);
    }

    const QualType fieldType = fieldSymbol->getQualType();
    if (fieldType.is(TY_STRUCT)) {
      Scope *bodyScope = fieldType.getType().getBodyScope();
      Struct *fieldStruct = fieldType.getType().getStruct(node);
      // Check if we are required to call a ctor
      const auto structDeclNode = spice_pointer_cast<StructDefNode *>(fieldStruct->declNode);
      const bool isCtorCallRequired = bodyScope->hasRefFields() || structDeclNode->emitVTable;
      // Lookup ctor function
      const Function *ctorFct = FunctionManager::matchFunction(bodyScope, CTOR_FUNCTION_NAME, thisType, {}, {}, true, node);
      // If we are required to construct, but no constructor is found, we can't generate a default ctor for the outer struct
      if (!ctorFct && isCtorCallRequired)
        return;
      hasFieldsToConstruct |= ctorFct != nullptr;
      requestRevisitIfRequired(ctorFct);
    }
  }

  // If we don't have any fields, that require us to do anything in the ctor, we can skip it
  if (!hasFieldsWithDefaultValue && !hasFieldsToConstruct && !node->emitVTable)
    return;

  // Create the default ctor function
  createDefaultStructMethod(spiceStruct, CTOR_FUNCTION_NAME, {});
}

void TypeChecker::createDefaultCtorBody(const Function *ctorFunction) { createCtorBodyPreamble(ctorFunction->bodyScope); }

/**
 * Checks if the given struct scope already has an user-defined constructor and creates a default one if not.
 *
 * @param entry SymbolTableEntry of the struct
 * @param structScope Scope of the struct
 */
void TypeChecker::createDefaultCopyCtorIfRequired(const Struct &spiceStruct, Scope *structScope) {
  auto node = spice_pointer_cast<StructDefNode *>(spiceStruct.declNode);
  assert(structScope != nullptr && structScope->type == ScopeType::STRUCT);

  // Abort if the struct already has a user-defined constructor
  const SymbolTableEntry *structEntry = spiceStruct.entry;
  const QualType &structType = structEntry->getQualType();
  const std::string fqFctName = structType.getSubType() + MEMBER_ACCESS_TOKEN + CTOR_FUNCTION_NAME;
  if (sourceFile->getNameRegistryEntry(fqFctName))
    return;

  // Check if we have fields, that require us to do anything in the ctor
  const size_t fieldCount = structScope->getFieldCount();
  bool hasFieldsToCopyConstruct = false;
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
    const QualType &thisType = fieldSymbol->getQualType();

    // Abort if we have a field, that is a reference
    if (thisType.isRef())
      return;

    const QualType fieldType = fieldSymbol->getQualType();
    if (fieldType.is(TY_STRUCT)) {
      Scope *bodyScope = fieldType.getType().getBodyScope();
      Struct *fieldStruct = fieldType.getType().getStruct(node);
      // Check if we are required to call a ctor
      const auto structDeclNode = spice_pointer_cast<StructDefNode *>(fieldStruct->declNode);
      const bool isCtorCallRequired = bodyScope->hasRefFields() || structDeclNode->emitVTable;
      // Lookup copy ctor function
      const ArgList args = {{thisType.toConstRef(node), false /* we always have the field as storage */}};
      const Function *ctorFct = FunctionManager::matchFunction(bodyScope, CTOR_FUNCTION_NAME, thisType, args, {}, true, node);
      // If we are required to construct, but no constructor is found, we can't generate a default ctor for the outer struct
      if (!ctorFct && isCtorCallRequired)
        return;
      hasFieldsToCopyConstruct |= ctorFct != nullptr;
      requestRevisitIfRequired(ctorFct);
    }
  }

  // If we don't have any fields, that require us to do anything in the copy ctor, we can skip it
  if (!hasFieldsToCopyConstruct && !node->emitVTable)
    return;

  // Create the default copy ctor function
  const ParamList paramTypes = {{structType.toConstRef(node), false}};
  createDefaultStructMethod(spiceStruct, CTOR_FUNCTION_NAME, paramTypes);
}

void TypeChecker::createDefaultCopyCtorBody(const Function *copyCtorFunction) {
  createCopyCtorBodyPreamble(copyCtorFunction->bodyScope);
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
  const QualType &structType = structEntry->getQualType();
  const std::string fqFctName = structType.getSubType() + MEMBER_ACCESS_TOKEN + DTOR_FUNCTION_NAME;
  if (sourceFile->getNameRegistryEntry(fqFctName))
    return;

  // Check we have field types, that require use to do anything in the destructor
  const size_t fieldCount = structScope->getFieldCount();
  bool hasHeapFields = false;
  bool hasFieldsToDestruct = false;
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
    hasHeapFields |= fieldSymbol->getQualType().isHeap();
    if (fieldSymbol->getQualType().is(TY_STRUCT)) {
      Scope *fieldScope = fieldSymbol->getQualType().getType().getBodyScope();
      // Lookup dtor function
      const QualType &thisType = fieldSymbol->getQualType();
      const Function *dtorFct = FunctionManager::matchFunction(fieldScope, DTOR_FUNCTION_NAME, thisType, {}, {}, true, node);
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
  // The string runtime does not use it, but allocates manually to avoid circular dependencies
  if (hasHeapFields && !sourceFile->isStringRT()) {
    SourceFile *memoryRT = sourceFile->requestRuntimeModule(MEMORY_RT);
    assert(memoryRT != nullptr);
    Scope *matchScope = memoryRT->globalScope.get();
    // Set dealloc function to used
    const QualType thisType(TY_DYN);
    QualType bytePtrRefType = QualType(TY_BYTE).toPtr(node).toRef(node);
    bytePtrRefType.getType().specifiers.isHeap = true;
    const ArgList args = {{bytePtrRefType, false /* we always have the field as storage */}};
    Function *deallocFct = FunctionManager::matchFunction(matchScope, FCT_NAME_DEALLOC, thisType, args, {}, true, node);
    assert(deallocFct != nullptr);
    deallocFct->used = true;
  }
}

void TypeChecker::createDefaultDtorBody(const Function *dtorFunction) { createDtorBodyPreamble(dtorFunction->bodyScope); }

/**
 * Prepare the generation of the ctor body preamble. This preamble is used to initialize the VTable, construct or initialize
 * fields.
 */
void TypeChecker::createCtorBodyPreamble(Scope *bodyScope) {
  // Retrieve struct scope
  Scope *structScope = bodyScope->parent;
  assert(structScope != nullptr);

  const size_t fieldCount = structScope->getFieldCount();
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());
    if (fieldSymbol->isImplicitField)
      continue;
    QualType fieldType = fieldSymbol->getQualType();
    if (fieldType.hasAnyGenericParts())
      TypeMatcher::substantiateTypeWithTypeMapping(fieldType, typeMapping);

    if (fieldType.is(TY_STRUCT)) {
      auto fieldNode = spice_pointer_cast<FieldNode *>(fieldSymbol->declNode);
      // Match ctor function, create the concrete manifestation and set it to used
      Scope *matchScope = fieldType.getType().getBodyScope();
      Function *spiceFunc = FunctionManager::matchFunction(matchScope, CTOR_FUNCTION_NAME, fieldType, {}, {}, false, fieldNode);
      if (spiceFunc != nullptr) {
        fieldType.getType().setBodyScope(spiceFunc->thisType.getType().getBodyScope());
        fieldSymbol->updateType(fieldType, true);
      }
    }
  }
}

/**
 * Prepare the generation of the copy ctor body preamble. This preamble is used to initialize the VTable, construct or initialize
 * fields.
 */
void TypeChecker::createCopyCtorBodyPreamble(Scope *bodyScope) {
  // Retrieve struct scope
  Scope *structScope = bodyScope->parent;
  assert(structScope != nullptr);

  const size_t fieldCount = structScope->getFieldCount();
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());
    if (fieldSymbol->isImplicitField)
      continue;
    QualType fieldType = fieldSymbol->getQualType();
    if (fieldType.hasAnyGenericParts())
      TypeMatcher::substantiateTypeWithTypeMapping(fieldType, typeMapping);

    if (fieldType.is(TY_STRUCT)) {
      auto fieldNode = spice_pointer_cast<FieldNode *>(fieldSymbol->declNode);
      // Match ctor function, create the concrete manifestation and set it to used
      Scope *matchScope = fieldType.getType().getBodyScope();
      const ArgList args = {{fieldType.toConstRef(fieldNode), false /* we always have the field as storage */}};
      Function *spiceFunc = FunctionManager::matchFunction(matchScope, CTOR_FUNCTION_NAME, fieldType, args, {}, false, fieldNode);
      if (spiceFunc != nullptr) {
        fieldType.getType().setBodyScope(spiceFunc->thisType.getType().getBodyScope());
        fieldSymbol->updateType(fieldType, true);
      }
    }
  }
}

/**
 * Prepare the generation of the dtor body preamble. This preamble is used to destruct all fields and to free all heap fields.
 */
void TypeChecker::createDtorBodyPreamble(Scope *bodyScope) {
  // Retrieve struct scope
  Scope *structScope = bodyScope->parent;
  assert(structScope != nullptr);

  const size_t fieldCount = structScope->getFieldCount();
  for (size_t i = 0; i < fieldCount; i++) {
    SymbolTableEntry *fieldSymbol = structScope->symbolTable.lookupStrictByIndex(i);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());
    if (fieldSymbol->isImplicitField)
      continue;
    QualType fieldType = fieldSymbol->getQualType();
    if (fieldType.hasAnyGenericParts())
      TypeMatcher::substantiateTypeWithTypeMapping(fieldType, typeMapping);

    if (fieldType.is(TY_STRUCT)) {
      auto fieldNode = spice_pointer_cast<FieldNode *>(fieldSymbol->declNode);
      // Match ctor function, create the concrete manifestation and set it to used
      Scope *matchScope = fieldType.getType().getBodyScope();
      FunctionManager::matchFunction(matchScope, DTOR_FUNCTION_NAME, fieldType, {}, {}, false, fieldNode);
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
Function *TypeChecker::implicitlyCallStructMethod(SymbolTableEntry *entry, const std::string &methodName, const ArgList &args,
                                                  const ASTNode *node) {
  QualType thisType = entry->getQualType();
  assert(thisType.is(TY_STRUCT));
  Scope *matchScope = thisType.getType().getBodyScope();
  assert(matchScope->type == ScopeType::STRUCT);

  // Search for dtor
  const bool isImported = matchScope->isImportedBy(rootScope);
  if (isImported)
    thisType = mapLocalTypeToImportedScopeType(matchScope, thisType);
  return FunctionManager::matchFunction(matchScope, methodName, thisType, args, {}, true, node);
}

/**
 * Prepare the generation of a call to the copy ctor of a given struct
 *
 * @param entry Symbol entry to use as 'this' pointer for the copy ctor call
 * @param node Current AST node
 */
void TypeChecker::implicitlyCallStructCopyCtor(SymbolTableEntry *entry, const ASTNode *node) {
  assert(entry != nullptr);
  const ArgList args = {{entry->getQualType().toConstRef(node), false /* we always have an entry here */}};
  implicitlyCallStructMethod(entry, CTOR_FUNCTION_NAME, args, node);
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
  // Sort by reverse declaration order
  auto lambda = [](const SymbolTableEntry *a, const SymbolTableEntry *b) { return a->declNode->codeLoc > b->declNode->codeLoc; };
  std::ranges::sort(vars, lambda);
  // Call dtor for each variable. We call the dtor in reverse declaration order
  for (SymbolTableEntry *var : vars) {
    // Only generate dtor call for structs and if not omitted
    if (!var->getQualType().is(TY_STRUCT) || var->omitDtorCall)
      continue;
    // Variable must be either initialized or a struct field
    if (!var->isInitialized() && var->scope->type != ScopeType::STRUCT)
      continue;
    // Call dtor
    implicitlyCallStructDtor(var, node);
  }
}

} // namespace spice::compiler
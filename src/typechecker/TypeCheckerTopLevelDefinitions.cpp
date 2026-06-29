// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <ast/ASTNodes.h>
#include <global/TypeRegistry.h>

namespace spice::compiler {

std::any TypeChecker::visitMainFctDef(MainFctDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitMainFctDefPrepare(node);
  else
    return visitMainFctDefCheck(node);
}

std::any TypeChecker::visitFctDef(FctDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitFctDefPrepare(node);
  else
    return visitFctDefCheck(node);
}

std::any TypeChecker::visitProcDef(ProcDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitProcDefPrepare(node);
  else
    return visitProcDefCheck(node);
}

std::any TypeChecker::visitStructDef(StructDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitStructDefPrepare(node);
  else
    return visitStructDefCheck(node);
}

std::any TypeChecker::visitInterfaceDef(InterfaceDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitInterfaceDefPrepare(node);
  return nullptr;
}

/**
 * Assign the opaque type to a struct, interface, enum or alias that is referenced before it has been prepared. This
 * effectively acts as an implicit forward declaration and is what makes circular imports work: two types in mutually
 * importing files can reference each other, so neither can be prepared strictly before the other. The full prepare pass
 * (visitStructDefPrepare / visitInterfaceDefPrepare / visitEnumDefPrepare / visitAliasDefPrepare) assigns the identical
 * interned type later and additionally fills in the body scope and manifestations.
 *
 * Only non-generic structs/interfaces can be pre-declared this way, since the opaque type carries no template types.
 *
 * @param entry Symbol table entry of the referenced struct, interface, enum or alias (its type must still be invalid)
 */
void TypeChecker::assignDeferredOpaqueType(SymbolTableEntry *entry) {
  assert(entry->getQualType().is(TY_INVALID));
  ASTNode *declNode = entry->declNode;
  if (const auto *structDef = dynamic_cast<StructDefNode *>(declNode)) {
    if (structDef->hasTemplateTypes)
      return;
    const TypeChainElementData data = {.bodyScope = structDef->structScope};
    const Type *type = TypeRegistry::getOrInsert(TY_STRUCT, structDef->structName, structDef->typeId, data, {});
    entry->updateType(QualType(type, structDef->qualifiers), false);
  } else if (const auto *interfaceDef = dynamic_cast<InterfaceDefNode *>(declNode)) {
    if (interfaceDef->hasTemplateTypes)
      return;
    const TypeChainElementData data = {.bodyScope = interfaceDef->interfaceScope};
    const Type *type = TypeRegistry::getOrInsert(TY_INTERFACE, interfaceDef->interfaceName, interfaceDef->typeId, data, {});
    entry->updateType(QualType(type, interfaceDef->qualifiers), false);
  } else if (const auto *enumDef = dynamic_cast<EnumDefNode *>(declNode)) {
    const TypeChainElementData data = {.bodyScope = enumDef->enumScope};
    const Type *type = TypeRegistry::getOrInsert(TY_ENUM, enumDef->enumName, enumDef->typeId, data, {});
    entry->updateType(QualType(type, enumDef->qualifiers), false);
  } else if (auto *aliasDef = dynamic_cast<AliasDefNode *>(declNode)) {
    // An alias additionally needs its aliased type resolved, so prepare it fully on demand. visitAliasDefPrepare is
    // idempotent: the real prepare pass for the alias's own file skips it once the type is no longer invalid.
    visitAliasDefPrepare(aliasDef);
  }
}

std::any TypeChecker::visitEnumDef(EnumDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitEnumDefPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitGenericTypeDef(GenericTypeDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitGenericTypeDefPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitAliasDef(AliasDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitAliasDefPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitGlobalVarDef(GlobalVarDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitGlobalVarDefPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitExtDecl(ExtDeclNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitExtDeclPrepare(node);
  return nullptr;
}

std::any TypeChecker::visitImportDef(ImportDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitImportDefPrepare(node);
  return nullptr;
}

} // namespace spice::compiler

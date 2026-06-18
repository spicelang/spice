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

std::any TypeChecker::visitForwardDecl(ForwardDeclNode *node) {
  if (typeCheckerMode != TC_MODE_PRE)
    return nullptr;

  assert(node->entry != nullptr && node->typeScope != nullptr);
  // A duplicate forward declaration shares its entry with the original; the entry's type is already populated.
  if (!node->entry->getQualType().isOneOf({TY_INVALID, TY_DYN}))
    return nullptr;
  const TypeChainElementData data = {.bodyScope = node->typeScope};
  const SuperType superType = node->isStruct ? TY_STRUCT : TY_INTERFACE;
  const Type *type = TypeRegistry::getOrInsert(superType, node->typeName, node->typeId, data, {});
  node->entry->updateType(QualType(type, node->qualifiers), false);
  return nullptr;
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

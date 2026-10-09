// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <ast/ASTNodes.h>
#include <exception/SemanticError.h>
#include <global/TypeRegistry.h>
#include <model/Interface.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/FunctionManager.h>

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

std::any TypeChecker::visitUnionDef(UnionDefNode *node) {
  if (typeCheckerMode == TC_MODE_PRE)
    return visitUnionDefPrepare(node);
  else
    return visitUnionDefCheck(node);
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
  } else if (const auto *unionDef = dynamic_cast<UnionDefNode *>(declNode)) {
    if (unionDef->hasTemplateTypes)
      return;
    const TypeChainElementData data = {.bodyScope = unionDef->unionScope};
    const Type *type = TypeRegistry::getOrInsert(TY_UNION, unionDef->unionName, unionDef->typeId, data, {});
    entry->updateType(QualType(type, unionDef->qualifiers), false);
  } else if (const auto *enumDef = dynamic_cast<EnumDefNode *>(declNode)) {
    const TypeChainElementData data = {.bodyScope = enumDef->enumScope};
    const Type *type = TypeRegistry::getOrInsert(TY_ENUM, enumDef->enumName, enumDef->typeId, data, {});
    entry->updateType(QualType(type, enumDef->qualifiers), false);
  } else if (auto *aliasDef = dynamic_cast<AliasDefNode *>(declNode)) {
    // An alias additionally needs its aliased type resolved, so prepare it fully on demand. visitAliasDefPrepare is
    // idempotent: the real prepare pass for the alias's own file skips it once the type is no longer invalid.
    // The aliased type has to be resolved in the context of the file declaring the alias, not the referencing one,
    // because the referencing file does not necessarily import the types the alias refers to.
    TypeChecker declTypeChecker(resourceManager, entry->scope->sourceFile, TC_MODE_PRE);
    declTypeChecker.visitAliasDefPrepare(aliasDef);
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

/**
 * Check if a function or procedure, that is defined as method of an interface, is a valid default method.
 *
 * @param node Function or procedure definition node of the default method
 * @param interfaceType Type of the interface, the default method belongs to
 * @param paramList Parameters of the default method
 */
void TypeChecker::checkDefaultMethod(const FctDefBaseNode *node, const QualType &interfaceType,
                                     const ParamList &paramList) const {
  const std::string &name = node->name->name;
  if (name == CTOR_FUNCTION_NAME || name == DTOR_FUNCTION_NAME)
    throw SemanticError(node, INVALID_DEFAULT_METHOD, "Interfaces cannot have constructors or destructors");
  if (node->hasTemplateTypes || !interfaceType.getTemplateTypes().empty())
    throw SemanticError(node, INVALID_DEFAULT_METHOD, "Default methods of generic interfaces are not supported yet");
  if (std::ranges::any_of(paramList, [](const Param &param) { return param.isOptional; }))
    throw SemanticError(node->paramLst, INVALID_DEFAULT_METHOD, "Default methods cannot have optional parameters");
}

/**
 * Check if a default method implements a method signature of its interface. The parameter and return types have to match
 * exactly, because the default method is called through the vtable slot of the interface method.
 *
 * @param defaultMethod Default method to check
 */
void TypeChecker::checkDefaultMethodImplementsInterfaceMethod(const Function *defaultMethod) const {
  const Interface *interface = defaultMethod->thisType.getInterface(defaultMethod->declNode);
  assert(interface != nullptr);
  const QualTypeList paramTypes = defaultMethod->getParamTypes();
  const auto pred = [&](const Function *method) {
    return method->name == defaultMethod->name && method->getParamTypes() == paramTypes &&
           method->returnType == defaultMethod->returnType;
  };
  if (std::ranges::none_of(interface->methods, pred))
    throw SemanticError(defaultMethod->declNode, INVALID_DEFAULT_METHOD,
                        "The default method '" + defaultMethod->getSignature() +
                            "' does not implement any method of the interface '" + interface->name + "'");
}

/**
 * Let a struct manifestation inherit the default methods of its interfaces for all interface methods, that the struct does
 * not implement itself. This happens as early as possible, so that calls to inherited methods can be type-checked before
 * the struct itself is checked to implement all interface methods. Default methods of interfaces in a circular import
 * might not be prepared yet at this point. Those are inherited when checking the struct (see visitStructDefCheck).
 *
 * @param spiceStruct Struct manifestation
 */
void TypeChecker::inheritDefaultMethods(const Struct &spiceStruct) {
  if (!spiceStruct.isFullySubstantiated())
    return;

  const QualType &structType = spiceStruct.entry->getQualType();
  for (const QualType &interfaceType : spiceStruct.interfaceTypes) {
    // Generic interfaces cannot have default methods
    if (!interfaceType.getTemplateTypes().empty())
      continue;
    const Interface *interface = interfaceType.getInterface(spiceStruct.declNode);
    assert(interface != nullptr);

    for (const Function *method : interface->methods) {
      // Check if the struct implements the method itself
      const QualTypeList paramTypes = method->getParamTypes();
      ArgList args;
      args.reserve(paramTypes.size());
      for (const QualType &paramType : paramTypes)
        args.emplace_back(paramType, false);
      if (FunctionManager::match(spiceStruct.scope, method->name, structType, args, {}, true, spiceStruct.declNode))
        continue;

      inheritDefaultMethod(spiceStruct.scope, structType, interface, method->name, paramTypes, method->returnType);
    }
  }
}

/**
 * Let a struct inherit the default method of an interface for the given method signature, if the interface provides one.
 * The inherited method is inserted into the struct scope, but has no body on its own. It refers to the default method,
 * which is emitted only once and gets called with a pointer to the interface part of the struct.
 *
 * @param structScope Scope of the struct manifestation
 * @param structType Struct type, which implements the interface
 * @param interface Interface, which might provide the default method
 * @param methodName Name of the requested method
 * @param paramTypes Parameter types of the requested method
 * @param returnType Return type of the requested method
 * @return Inherited method or nullptr if the interface has no default method for the signature
 */
Function *TypeChecker::inheritDefaultMethod(Scope *structScope, const QualType &structType, const Interface *interface,
                                            const std::string &methodName, const QualTypeList &paramTypes,
                                            const QualType &returnType) {
  // Search for a default method with the exact signature
  Function *defaultMethod = FunctionManager::lookupDefaultMethod(interface->scope, methodName, paramTypes, returnType);
  if (defaultMethod == nullptr)
    return nullptr;

  // If the struct declares a method with this name, it most likely tries to implement the interface method, but with a
  // signature that does not match. Do not silently inherit the default method in this case, but report the missing method
  if (FunctionManager::hasUserMethodWithName(structScope, methodName))
    return nullptr;

  // Create the inherited method in the struct scope
  Function inheritedMethod = *defaultMethod;
  inheritedMethod.thisType = structType;
  inheritedMethod.bodyScope = nullptr;
  inheritedMethod.isInterfaceDefaultMethod = false;
  inheritedMethod.defaultMethod = defaultMethod;
  inheritedMethod.alreadyTypeChecked = true; // The body is type-checked as part of the default method
  inheritedMethod.used = false;

  // The default method is referenced by the vtable of the struct, so it has to be emitted
  defaultMethod->used = true;
  defaultMethod->entry->used = true;

  return FunctionManager::insert(structScope, inheritedMethod, nullptr);
}

} // namespace spice::compiler

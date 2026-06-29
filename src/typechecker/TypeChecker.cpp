// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <unordered_set>

#include <SourceFile.h>
#include <ast/Attributes.h>
#include <global/GlobalResourceManager.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/FunctionManager.h>

namespace spice::compiler {

TypeChecker::TypeChecker(GlobalResourceManager &resourceManager, SourceFile *sourceFile, TypeCheckerMode typeCheckerMode)
    : CompilerPass(resourceManager, sourceFile), typeCheckerMode(typeCheckerMode), warnings(sourceFile->compilerOutput.warnings) {
}

/**
 * Check whether the given struct and all of its by-value struct fields are manifested, transitively. While a circular
 * import is still being prepared, a by-value struct field may (transitively) reference a struct that is not manifested
 * yet; generating implicit special members for such a struct recurses through isTriviallyConstructible and would
 * dereference a null struct lookup. A genuine infinite-size containment cycle among these structs is reported
 * separately by the infinite-size check during struct preparation, so a containment cycle here is treated as manifested.
 */
static bool structFullyManifested(const Struct *spiceStruct, const ASTNode *node, std::unordered_set<const Scope *> &visited) {
  if (!visited.insert(spiceStruct->scope).second)
    return true; // Containment cycle - reported separately as an infinite-size error
  for (const QualType &fieldType : spiceStruct->fieldTypes) {
    if (!fieldType.is(TY_STRUCT))
      continue;
    const Struct *fieldStruct = fieldType.getStruct(node);
    if (fieldStruct == nullptr || !structFullyManifested(fieldStruct, node, visited))
      return false;
  }
  return true;
}

std::any TypeChecker::visitEntry(EntryNode *node) {
  // Initialize
  currentScope = rootScope;

  // Initialize AST nodes with size of 1
  const bool isPrepare = typeCheckerMode == TC_MODE_PRE;
  if (isPrepare)
    node->resizeToNumberOfManifestations(1);

  // Visit children
  visitChildren(node);

  // Check which implicit structures we need for each struct, defined in this source file
  if (isPrepare) {
    const std::vector<const Struct *> manifestations = rootScope->getAllStructManifestationsInDeclarationOrder();
    for (const Struct *manifestation : manifestations) {
      // Skip while a by-value struct field is (transitively) not manifested yet (circular import still in progress). A
      // genuine infinite-size cycle among such structs is reported by the infinite-size check during preparation.
      std::unordered_set<const Scope *> visitedScopes;
      if (!structFullyManifested(manifestation, node, visitedScopes))
        continue;
      // Check if we need to create a default ctor, copy ctor, move ctor or dtor
      createDefaultCtorIfRequired(*manifestation, manifestation->scope);
      createDefaultCopyCtorIfRequired(*manifestation, manifestation->scope);
      createDefaultMoveCtorIfRequired(*manifestation, manifestation->scope);
      createDefaultDtorIfRequired(*manifestation, manifestation->scope);
    }
  }

  return nullptr;
}

/**
 * Check if the capture rules for async lambdas are enforced if the async attribute is set
 *
 * Only one capture with pointer type, pass-by-val is allowed, since only then we can store it in the second field of the
 * fat pointer and can ensure, that no stack variable is referenced inside the lambda.
 *
 * @param node Lambda base node
 * @param attrs Lambda attributes
 * @return False if the rules are violated, true otherwise
 */
bool TypeChecker::checkAsyncLambdaCaptureRules(const LambdaBaseNode *node, const LambdaAttrNode *attrs) const {
  // If the async attribute is not set, we can return early
  if (!attrs || !attrs->attrLst->hasAttr(ATTR_ASYNC) || !attrs->attrLst->getAttrValueByName(ATTR_ASYNC)->boolValue)
    return true; // Not violated

  // If we don't have any captures, we can return early
  const CaptureMap &captures = node->bodyScope->symbolTable.captures;
  if (captures.empty())
    return true; // Not violated

  // Check for the capture rules
  if (const Capture &capture = captures.begin()->second;
      captures.size() > 1 || !capture.capturedSymbol->getQualType().isPtr() || capture.getMode() != BY_VALUE) {
    const auto warningMessage =
        "Async lambdas can only capture one pointer by value without storing captures in the caller stack frame, which can lead "
        "to bugs due to references, outliving the validity scope of the referenced variable.";
    const CompilerWarning warning(node->codeLoc, ASYNC_LAMBDA_CAPTURE_RULE_VIOLATION, warningMessage);
    currentScope->sourceFile->compilerOutput.warnings.push_back(warning);
  }

  return false; // Violated
}

Function *TypeChecker::matchCopyCtor(const QualType &thisType, const ASTNode *node) const {
  Scope *matchScope = thisType.getBodyScope();
  assert(matchScope != nullptr);
  const ArgList args = {{thisType.toConstRef(node), false}};
  return FunctionManager::match(matchScope, CTOR_FUNCTION_NAME, thisType, args, {}, true, node);
}

Function *TypeChecker::matchMoveCtor(const QualType &thisType, const ASTNode *node) const {
  Scope *matchScope = thisType.getBodyScope();
  assert(matchScope != nullptr);
  const ArgList args = {{thisType.toNonConst().toRef(node), false}};
  return FunctionManager::match(matchScope, CTOR_FUNCTION_NAME, thisType, args, {}, true, node);
}

QualType TypeChecker::mapLocalTypeToImportedScopeType(const Scope *targetScope, const QualType &symbolType) const {
  // Skip all types, except structs
  if (!symbolType.isBase(TY_STRUCT))
    return symbolType;

  // If the target scope is in the current source file, we can return the symbol type as is
  SourceFile *targetSourceFile = targetScope->sourceFile;
  if (targetSourceFile == sourceFile)
    return symbolType;

  // Match the scope of the symbol type against all scopes in the name registry of the target file
  for (const NameRegistryEntry &entry : targetSourceFile->exportedNameRegistry | std::views::values)
    if (entry.targetEntry != nullptr && entry.targetEntry->getQualType().isBase(TY_STRUCT))
      for (const Struct *manifestation : *entry.targetEntry->declNode->getStructManifestations())
        if (manifestation->scope == symbolType.getBase().getBodyScope())
          return symbolType;

  // The target file does not know about the struct at all
  // -> show it how to find the struct
  const std::string structName = symbolType.getBase().getSubType();
  const NameRegistryEntry *origRegistryEntry = sourceFile->getNameRegistryEntry(structName);
  // If even this file does not know the struct by its unqualified name (deep transitive import), there is
  // nothing to copy over. Skip teaching the target file; the type identity itself is unaffected, and member
  // access falls back to the resolved body scope.
  if (origRegistryEntry == nullptr)
    return symbolType;
  // Do not clobber an entry the target file already has under this name (e.g. its OWN same-named struct, like
  // llvm's `Function` vs the model's `Function`). Teaching uses keepNewOnCollision=false, which would otherwise
  // ERASE the target's existing entry and break resolution of its own type. The struct's QualType identity is
  // carried by pointer regardless, so a name that is already taken does not need (re-)teaching here.
  if (targetSourceFile->exportedNameRegistry.contains(structName))
    return symbolType;
  const uint64_t targetTypeId = origRegistryEntry->typeId;
  SymbolTableEntry *targetEntry = origRegistryEntry->targetEntry;
  targetSourceFile->addNameRegistryEntry(structName, targetTypeId, targetEntry, origRegistryEntry->targetScope, false);

  return symbolType;
}

QualType TypeChecker::mapImportedScopeTypeToLocalType(const Scope *sourceScope, const QualType &symbolType) const {
  // Skip all types, except structs
  if (!symbolType.isBase(TY_STRUCT))
    return symbolType;

  // If the given source file is in the current one, we can return the symbol type as is
  const SourceFile *sourceSourceFile = sourceScope->sourceFile;
  if (sourceSourceFile == sourceFile)
    return symbolType;

  // Match the scope of the symbol type against all scopes in the name registry of this source file
  const QualType baseType = symbolType.getBase();
  for (const auto &entry : sourceFile->exportedNameRegistry | std::views::values)
    if (entry.targetEntry != nullptr && entry.targetEntry->getQualType().isBase(TY_STRUCT))
      for (const Struct *manifestation : *entry.targetEntry->declNode->getStructManifestations())
        if (manifestation->scope == baseType.getBodyScope())
          return symbolType;

  // This source file does not know about the struct at all
  // -> show it how to find the struct
  const NameRegistryEntry *origRegistryEntry = sourceSourceFile->getNameRegistryEntry(baseType.getSubType());
  // If even the source file does not know the struct by its unqualified name (deep transitive import), there is
  // nothing to copy over. Skip teaching this file; the type identity itself is unaffected, and member access
  // falls back to the resolved body scope.
  if (origRegistryEntry == nullptr)
    return symbolType;
  // Do not clobber an entry this file already has under this name (see mapLocalTypeToImportedScopeType): teaching
  // with keepNewOnCollision=false would ERASE this file's own same-named struct entry. The QualType identity is
  // carried by pointer, so an already-taken name needs no (re-)teaching here.
  if (sourceFile->exportedNameRegistry.contains(baseType.getSubType()))
    return symbolType;
  const uint64_t typeId = origRegistryEntry->typeId;
  SymbolTableEntry *targetEntry = origRegistryEntry->targetEntry;
  sourceFile->addNameRegistryEntry(baseType.getSubType(), typeId, targetEntry, origRegistryEntry->targetScope, false);

  return symbolType;
}

/**
 * Returns the operator function list for the current manifestation and the given node
 *
 * @param node Node to retrieve the op fct pointer list from
 * @return Op fct pointer list
 */
std::vector<const Function *> &TypeChecker::getOpFctPointers(ASTNode *node) const {
  assert(node->getOpFctPointers()->size() > manIdx);
  return node->getOpFctPointers()->at(manIdx);
}

/**
 * Check if a function has been type-checked already. If not, request a revisit
 *
 * @param fct Function to check
 */
void TypeChecker::requestRevisitIfRequired(const Function *fct) {
  if (fct && !fct->alreadyTypeChecked)
    fct->entry->scope->sourceFile->reVisitRequested = true;
}

/**
 * Check type name against well-known type names that require a runtime import. If found one, auto-import the runtime module.
 *
 * @param typeName Given type name
 */
void TypeChecker::ensureLoadedRuntimeForTypeName(const std::string &typeName) const {
  for (const auto &[wellKnownTypeName, runtimeModule] : TYPE_NAME_TO_RT_MODULE_MAPPING) {
    if (typeName == wellKnownTypeName && !sourceFile->isRT(runtimeModule)) {
      sourceFile->requestRuntimeModule(runtimeModule);
      break;
    }
  }
}

/**
 * Check type name against well-known function names that require a runtime import. If found one, auto-import the runtime module.
 *
 * @param functionName Given function name
 */
void TypeChecker::ensureLoadedRuntimeForFunctionName(const std::string &functionName) const {
  for (const auto &[wellKnownFunctionName, runtimeModule] : FCT_NAME_TO_RT_MODULE_MAPPING) {
    if (functionName == wellKnownFunctionName && !sourceFile->isRT(runtimeModule)) {
      sourceFile->requestRuntimeModule(runtimeModule);
      break;
    }
  }
}

/**
 * Add a soft error to the error list
 */
void TypeChecker::softError(const ASTNode *node, const SemanticErrorType errorType, const std::string &message) const {
  resourceManager.errorManager.addSoftError(node, errorType, message);
}

bool TypeChecker::isCopyCtorCall(const FctCallNode *node, const QualType &thisType) const {
  const FctCallNode::FctCallData &data = node->data.at(manIdx);
  if (data.args.size() != 2)
    return false;
  const QualType &secondArgType = data.args.back().first.removeReferenceWrapper().toNonConst();
  return thisType.matches(secondArgType, false, false, true);
}

} // namespace spice::compiler

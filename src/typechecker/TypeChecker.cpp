// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <SourceFile.h>
#include <ast/Attributes.h>
#include <global/GlobalResourceManager.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

TypeChecker::TypeChecker(GlobalResourceManager &resourceManager, SourceFile *sourceFile, TypeCheckerMode typeCheckerMode)
    : CompilerPass(resourceManager, sourceFile), typeCheckerMode(typeCheckerMode), warnings(sourceFile->compilerOutput.warnings) {
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
      // Check if we need to create a default ctor, copy ctor or dtor
      createDefaultCtorIfRequired(*manifestation, manifestation->scope);
      createDefaultCopyCtorIfRequired(*manifestation, manifestation->scope);
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
  return FunctionManager::match(this, matchScope, CTOR_FUNCTION_NAME, thisType, args, {}, true, node);
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
  for (const auto &entry : targetSourceFile->exportedNameRegistry | std::views::values)
    if (entry.targetEntry != nullptr && entry.targetEntry->getQualType().isBase(TY_STRUCT))
      for (const Struct *manifestation : *entry.targetEntry->declNode->getStructManifestations())
        if (manifestation->scope == symbolType.getBase().getBodyScope())
          return symbolType;

  // The target file does not know about the struct at all
  // -> show it how to find the struct
  const std::string structName = symbolType.getBase().getSubType();
  const NameRegistryEntry *origRegistryEntry = sourceFile->getNameRegistryEntry(structName);
  assert(origRegistryEntry != nullptr);
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
  assert(origRegistryEntry != nullptr);
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

} // namespace spice::compiler

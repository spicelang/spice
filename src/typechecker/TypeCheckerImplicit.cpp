// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "TypeChecker.h"

#include <unordered_set>

#include <SourceFile.h>
#include <ast/ASTBuilder.h>
#include <ast/ASTNodes.h>
#include <global/GlobalResourceManager.h>
#include <model/GenericType.h>
#include <model/Struct.h>
#include <symboltablebuilder/Scope.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/FunctionManager.h>
#include <typechecker/TypeMatcher.h>

namespace spice::compiler {

static const char *const FCT_NAME_DEALLOC = "sDealloc";

/**
 * Check whether the given struct and all of its by-value struct fields are manifested, transitively, and have already
 * decided their own implicit default members. While a circular import is still being prepared, a by-value struct field
 * may (transitively) reference a struct that is not manifested yet; generating implicit special members for such a
 * struct recurses through isTriviallyConstructible and would dereference a null struct lookup. A genuine infinite-size
 * containment cycle among these structs is reported separately by the infinite-size check during struct preparation,
 * so a containment cycle here is treated as manifested.
 *
 * A field struct can also be manifested (the Struct object exists) but not decided yet: a generic struct like
 * Result<T> can be substantiated for T = Url while checking a function body during Url's own file's one-shot sweep
 * (TypeChecker::visitEntry), before that sweep has reached Url's struct declaration and decided that Url needs a
 * dtor/copy ctor. Deciding Result<Url> right then would see Url as trivial (no dtor found yet) and permanently skip
 * generating Result<Url>'s dtor, since createImplicitDefaultMembers only ever runs once per manifestation. Requiring
 * the field struct to have decided its own members first defers Result<Url> the same way as the circular-import case
 * above, to be picked up again by the fallback sweep in visitStructDefCheck once Url has decided.
 */
static bool structFullyManifested(const Struct *spiceStruct, const ASTNode *node, std::unordered_set<const Scope *> &visited) {
  for (const QualType &fieldType : spiceStruct->fieldTypes) {
    if (!fieldType.is(TY_STRUCT))
      continue;
    const Struct *fieldStruct = fieldType.getStruct(node);
    if (fieldStruct == nullptr || !fieldStruct->implicitDefaultMembersDecided)
      return false;
    // Recurse into each field struct once. A containment cycle (already visited) is fine here - it is reported
    // separately as an infinite-size error. Leaf structs never touch the set, so the common case stays allocation-free.
    if (visited.insert(fieldStruct->scope).second && !structFullyManifested(fieldStruct, node, visited))
      return false;
  }
  return true;
}

/**
 * Decide which compiler-generated default members (ctor, copy ctor, move ctor, dtor) the given struct manifestation
 * needs and create them.
 *
 * This decision depends on the concrete field types, so it has to be taken once per manifestation and not once per
 * struct declaration: whether e.g. HashEntry<K, V> needs a dtor is unanswerable while K and V are still generic, but
 * well-defined for HashEntry<int, String>. It is therefore taken as soon as a manifestation comes into existence -
 * either by the one-shot sweep over the manifestations that already exist when a source file is prepared
 * (TypeChecker::visitEntry), or, for manifestations that are substantiated later on, right at the point where
 * StructManager::match creates them. Deciding any later would be too late for all the reactive lookups that ask a
 * struct for its default members (FunctionManager::match/::lookup, doScopeCleanup, ...), since a function body is
 * type-checked exactly once and would not pick up a dtor that appears afterward.
 *
 * The default move ctor is decided about only when the caller asks for it. A manifestation that is substantiated after
 * its file was prepared does not get one, which keeps it at the behavior it had before this decision was taken per
 * manifestation at all. Synthesizing one would require the move ctor of each struct-typed field to be resolvable for
 * the concrete manifestation, which the raw manifestation scan behind it (FunctionManager::findMoveCtor) cannot do for
 * a generic field type - it returns the generic preset that the manifestation scope inherited. Without a move ctor,
 * moving such a struct falls back to copying it, which is correct, just less efficient.
 *
 * @param spiceStruct Struct manifestation to decide the default members for
 * @param node AST node for error messages
 * @param withMoveCtor Also decide about the default move ctor
 */
void TypeChecker::createImplicitDefaultMembers(Struct &spiceStruct, const ASTNode *node, bool withMoveCtor) {
  // Take the decision only once per manifestation
  if (spiceStruct.implicitDefaultMembersDecided)
    return;

  GlobalResourceManager &resourceManager = spiceStruct.scope->sourceFile->resourceManager;

  // Skip while a by-value struct field is (transitively) not manifested yet, or has not decided its own implicit
  // default members yet (circular import still in progress, or a sibling struct's one-shot sweep has not reached it
  // yet - see structFullyManifested). Relying solely on the fallback sweep in visitStructDefCheck to pick this back up
  // can be too late: that sweep only runs during the type-checker POST pass, by which point some other function body
  // elsewhere may already have looked up (and missed) this manifestation's dtor for good, since a function body is
  // type-checked exactly once. Queue the manifestation instead, so it gets retried the moment something else's
  // decision completes (see drainPendingImplicitDefaultMemberDecisions below) - typically still within the same PRE
  // pass that first substantiated it.
  std::unordered_set<const Scope *> visitedScopes;
  if (!structFullyManifested(&spiceStruct, node, visitedScopes)) {
    std::vector<std::pair<Struct *, bool>> &pending = resourceManager.pendingStructDefaultMemberDecisions;
    const auto pred = [&](const auto &entry) { return entry.first == &spiceStruct; };
    const auto it = std::ranges::find_if(pending, pred);
    if (it == pending.end())
      pending.emplace_back(&spiceStruct, withMoveCtor);
    else
      it->second |= withMoveCtor; // Upgrade to deciding the move ctor too if any deferred caller asked for it
    return;
  }

  decideDefaultMembers(spiceStruct, withMoveCtor);

  // This struct just became decided - drain the pending list so it can pick up anything that was blocked on it, directly or
  // transitively (a chain of Result<Outer<Inner>>-shaped dependencies).
  drainPendingImplicitDefaultMemberDecisions(resourceManager);
}

/**
 * Set a struct manifestation's default members as decided and create whichever of them (ctor, copy ctor, move ctor,
 * dtor) it actually needs. Only called once structFullyManifested has confirmed the struct is ready.
 *
 * @param spiceStruct Struct manifestation to decide the default members for
 * @param withMoveCtor Also decide about the default move ctor
 */
void TypeChecker::decideDefaultMembers(Struct &spiceStruct, bool withMoveCtor) {
  spiceStruct.implicitDefaultMembersDecided = true;

  createDefaultCtorIfRequired(spiceStruct, spiceStruct.scope);
  createDefaultCopyCtorIfRequired(spiceStruct, spiceStruct.scope);
  if (withMoveCtor)
    createDefaultMoveCtorIfRequired(spiceStruct, spiceStruct.scope);
  createDefaultDtorIfRequired(spiceStruct, spiceStruct.scope);
}

/**
 * Drain every struct manifestation that createImplicitDefaultMembers previously had to defer because a by-value
 * field struct had not decided its own default members yet. Called whenever a struct's decision completes, since
 * that can unblock others - directly (a struct that was waiting on exactly this one) or transitively (a chain of
 * Result<Outer<Inner>>-shaped dependencies).
 *
 * This calls decideDefaultMembers directly rather than createImplicitDefaultMembers, so draining one entry can never
 * recurse back into this function - no reentrancy guard is needed, the loop below just keeps going until a full pass
 * makes no further progress.
 *
 * @param resourceManager Resource manager that owns the pending list for this compilation
 */
void TypeChecker::drainPendingImplicitDefaultMemberDecisions(GlobalResourceManager &resourceManager) {
  bool progressed;
  do {
    progressed = false;
    // Snapshot before iterating: deciding one pending manifestation can append newly-deferred entries to the very
    // list being iterated (a struct that unblocks can itself be a field of another, still-blocked struct).
    const std::vector<std::pair<Struct *, bool>> snapshot = resourceManager.pendingStructDefaultMemberDecisions;
    for (const auto &[pendingStruct, pendingWithMoveCtor] : snapshot) {
      if (pendingStruct->implicitDefaultMembersDecided)
        continue; // Already resolved by an earlier entry in this same pass

      std::unordered_set<const Scope *> visitedScopes;
      if (!structFullyManifested(pendingStruct, pendingStruct->declNode, visitedScopes))
        continue; // Still blocked on something else; leave it queued

      decideDefaultMembers(*pendingStruct, pendingWithMoveCtor);
      progressed = true;
    }
  } while (progressed);

  // Drop everything that got resolved. Whatever remains is still genuinely blocked (e.g. a circular import still in
  // progress) and stays queued to be retried again the next time some other struct's decision completes.
  const auto pred = [](const std::pair<Struct *, bool> &entry) { return entry.first->implicitDefaultMembersDecided; };
  std::erase_if(resourceManager.pendingStructDefaultMemberDecisions, pred);
}

/**
 * Create a default struct method
 * Checks if the given struct scope already has a user-defined constructor and creates a default one if not.
 *
 * @param spiceStruct Struct instance
 * @param entryName Name of the symbol table entry
 * @param name Name of the method to create
 * @param params Parameter types of the method
 */
void TypeChecker::createDefaultStructMethod(const Struct &spiceStruct, const std::string &entryName, const std::string &name,
                                            const ParamList &params) {
  Scope *structScope = spiceStruct.scope;
  SourceFile *sourceFile = structScope->sourceFile;
  ASTNode *node = spiceStruct.declNode;
  const SymbolTableEntry *structEntry = spiceStruct.entry;
  const QualType &structType = structEntry->getQualType();
  const std::string fqFctName = structType.getSubType() + MEMBER_ACCESS_TOKEN + name;

  // Procedure type
  QualType procedureType(TY_PROCEDURE);
  procedureType.makePublic(); // Always public

  // Insert symbol for function into the symbol table
  SymbolTableEntry *procEntry = structScope->insert(entryName, structEntry->declNode);
  procEntry->updateType(procedureType, true);

  // Add to external name registry. Only the generic preset does this: the registry is keyed by the plain struct name,
  // so an entry added for one manifestation would be indistinguishable from a user-defined member and would wrongly
  // suppress the default member of every sibling manifestation (e.g. HashEntry<int, String> vs. HashEntry<int, int>).
  // Manifestation-level default members are only ever looked up through their struct scope anyway.
  if (!spiceStruct.isGenericSubstantiation())
    sourceFile->addNameRegistryEntry(fqFctName, TY_PROCEDURE, procEntry, structScope, true);

  // Create the default method
  const std::vector<GenericType> templateTypes = spiceStruct.templateTypes;
  const QualType returnType(TY_DYN);
  Function defaultMethod(name, procEntry, structType, returnType, params, templateTypes, structEntry->declNode);
  defaultMethod.implicitDefault = true;

  // Fill type mapping for the case, that the template type list contains non-generic types. Only struct, interface
  // and enum types carry a sub type usable as a key here; a non-generic primitive template argument (e.g. the 'int'
  // in BlockAllocator<int>) has no sub type, so getSubType() would assert on it - skip those.
  for (const GenericType &templateType : templateTypes)
    if (!templateType.is(TY_GENERIC) && templateType.isOneOf({TY_STRUCT, TY_INTERFACE, TY_ENUM}))
      defaultMethod.typeMapping[templateType.getSubType()] = static_cast<QualType>(templateType);

  // Create function scope
  Scope *procScope = structScope->createChildScope(defaultMethod.getScopeName(), ScopeType::FUNC_PROC_BODY, &node->codeLoc);
  defaultMethod.bodyScope = procScope;

  // Create 'this' symbol in the function scope
  SymbolTableEntry *thisEntry = procScope->insert(THIS_VARIABLE_NAME, node);
  thisEntry->updateType(structType.toPtr(node), true);
  thisEntry->used = true; // Always set to used to not print warnings for non-existing code

  // Hand it off to the function manager to register the function
  FunctionManager::insert(structScope, defaultMethod, structEntry->declNode->getFctManifestations(name));

  // The body of a default member is not prepared here, but in visitStructDefCheck, which runs over all manifestations
  // of the struct. For a manifestation that was substantiated after its file was already checked, that run is long
  // over, so ask for another one - otherwise the member would end up without a body preamble, and the IR generator
  // would e.g. not find the copy ctor of a field it is supposed to copy. During the prepare stage the flag is set
  // anyway, so this is a no-op there.
  sourceFile->reVisitRequested = true;
}

/**
 * Checks if the given struct scope already has a user-defined constructor and creates a default one if not.
 *
 * For generating a default ctor, the following conditions need to be met:
 * - No user-defined constructors (incl. copy/move ctors)
 *
 * @param spiceStruct Struct instance
 * @param structScope Scope of the struct
 */
void TypeChecker::createDefaultCtorIfRequired(const Struct &spiceStruct, Scope *structScope) {
  const auto node = spice_pointer_cast<StructDefNode *>(spiceStruct.declNode);
  assert(structScope != nullptr && structScope->type == ScopeType::STRUCT);
  const SourceFile *sourceFile = structScope->sourceFile;

  // Abort if the struct already has a constructor. The name registry is the authoritative source here: it is filled by
  // the symbol table builder, so it already knows about every user-defined ctor, even one that is declared further down
  // in the file and therefore has not been registered in the struct scope yet. It also covers a ctor that the generic
  // preset received, which every manifestation inherits through its copied scope. The scope scan on top of it only
  // guards against creating a second ctor into a scope that visibly has one already.
  const SymbolTableEntry *structEntry = spiceStruct.entry;
  const QualType &structType = structEntry->getQualType();
  const std::string fqFctName = structType.getSubType() + MEMBER_ACCESS_TOKEN + CTOR_FUNCTION_NAME;
  if (sourceFile->getNameRegistryEntry(fqFctName) || FunctionManager::hasAnyCtor(structScope))
    return;

  // Check if we have fields, that require us to do anything in the ctor
  const size_t fieldCount = structScope->getFieldCount();
  bool hasFieldsWithDefaultValue = false;
  bool hasFieldsToConstruct = false;
  for (size_t i = 0; i < fieldCount; i++) {
    const SymbolTableEntry *fieldSymbol = structScope->lookupField(i);
    assert(fieldSymbol != nullptr && fieldSymbol->declNode != nullptr);

    QualType fieldType = fieldSymbol->getQualType();
    if (fieldType.hasAnyGenericParts() && !spiceStruct.typeMapping.empty())
      TypeMatcher::substantiateTypeWithTypeMapping(fieldType, spiceStruct.typeMapping, fieldSymbol->declNode);

    // Abort if we have a field, that is a reference
    if (fieldType.isRef())
      return;

    if (const auto fieldNode = dynamic_cast<FieldNode *>(fieldSymbol->declNode)) {
      hasFieldsWithDefaultValue |= fieldNode->defaultValue != nullptr;
    } else {
      assert(is<DataTypeNode *>(fieldSymbol->declNode));
    }

    if (fieldType.is(TY_STRUCT)) {
      Scope *bodyScope = fieldType.getBodyScope();
      // Check if we are required to call a ctor
      const bool isCtorCallRequired = !fieldType.isTriviallyConstructible(node);
      // While the outer struct is still a generic preset, a field type that is itself generic (e.g. Inner<T, bool>,
      // the internal field of Outer<T>) cannot be matched to a concrete ctor yet. Since a default ctor takes no
      // arguments, matching it against the still-generic 'this' type would partially substantiate a bogus
      // manifestation (e.g. Inner<K, bool>, with K left generic) that is wrongly treated as fully substantiated
      // later on and crashes name mangling during IR generation (see #1255). In that case we only record that the
      // field has a default ctor via a direct scan; the concrete ctor is matched later, per manifestation, in
      // createCtorBodyPreamble.
      if (fieldType.hasAnyGenericParts()) {
        const bool hasDefaultCtor = FunctionManager::hasDefaultCtor(bodyScope);
        if (!hasDefaultCtor && isCtorCallRequired)
          return;
        hasFieldsToConstruct |= hasDefaultCtor;
      } else {
        // Lookup ctor function
        const Function *ctorFct = FunctionManager::match(bodyScope, CTOR_FUNCTION_NAME, fieldType, {}, {}, true, node);
        // If we are required to construct, but no constructor is found, we can't generate a default ctor for the outer struct
        if (!ctorFct && isCtorCallRequired)
          return;
        hasFieldsToConstruct |= ctorFct != nullptr;
      }
    }
  }

  // If we don't have any fields, that require us to do anything in the ctor, we can skip it
  if (!hasFieldsWithDefaultValue && !hasFieldsToConstruct && !node->emitVTable)
    return;

  // Create the default ctor function
  const std::string entryName = Function::getSymbolTableEntryNameDefaultCtor(node->codeLoc);
  createDefaultStructMethod(spiceStruct, entryName, CTOR_FUNCTION_NAME, {});
}

/**
 * Checks if the given struct scope already has a user-defined constructor and creates a default one if not.
 *
 * For generating a default copy ctor, the following conditions need to be met:
 * - No user-defined copy ctor
 *
 * @param spiceStruct Struct instance
 * @param structScope Scope of the struct
 */
void TypeChecker::createDefaultCopyCtorIfRequired(const Struct &spiceStruct, Scope *structScope) {
  const auto node = spice_pointer_cast<const StructDefNode *>(spiceStruct.declNode);
  assert(structScope != nullptr && structScope->type == ScopeType::STRUCT);

  // Abort if the struct already has a user-defined copy constructor
  const QualType structType = spiceStruct.entry->getQualType();
  if (FunctionManager::hasCopyCtor(structScope))
    return;

  // Check if we have fields, that require us to do anything in the ctor
  const size_t fieldCount = structScope->getFieldCount();
  bool copyCtorRequired = false;
  for (size_t i = 0; i < fieldCount; i++) {
    const SymbolTableEntry *fieldSymbol = structScope->lookupField(i);
    assert(fieldSymbol != nullptr && fieldSymbol->declNode != nullptr);

    QualType fieldType = fieldSymbol->getQualType();
    if (fieldType.hasAnyGenericParts() && !spiceStruct.typeMapping.empty())
      TypeMatcher::substantiateTypeWithTypeMapping(fieldType, spiceStruct.typeMapping, fieldSymbol->declNode);

    // If the field is of type struct, check if this struct has a copy ctor that has to be called
    if (fieldType.is(TY_STRUCT)) {
      // A field whose copy is non-trivial forces the outer struct to have a copy ctor as well.
      const bool fieldRequiresCopyCtor = !fieldType.isTriviallyCopyable(node);
      // While the outer struct is still a generic preset, a field type that is itself generic (e.g. Inner<T, bool>,
      // the internal field of Outer<T>) cannot be matched to a concrete copy ctor yet. Matching it via the
      // substantiating FunctionManager::match() below can partially substantiate a bogus manifestation (e.g.
      // Inner<K, bool>, with K left generic) that is wrongly treated as fully substantiated later on and crashes
      // name mangling during IR generation (see #1255). This can happen even though the copy ctor call carries an
      // argument of the same (still-generic) field type, whenever the field struct's own generic type name
      // collides with the outer struct's generic type name (e.g. both named 'V', as in Set<V> wrapping
      // RedBlackTree<K,V>). The concrete copy ctor is matched later, per manifestation, in
      // createCopyCtorBodyPreamble, and its result here is not otherwise consulted while the field type is still
      // generic (fieldRequiresCopyCtor already reflects copy-ctor existence via the non-substantiating
      // isTriviallyCopyable check above) - so skip the substantiating match entirely in that case.
      if (!fieldType.hasAnyGenericParts()) {
        Scope *bodyScope = fieldType.getBodyScope();
        const ArgList args = {{fieldType.toConstRef(node), false /* we always have the field as storage */}};
        const Function *ctorFct = FunctionManager::match(bodyScope, CTOR_FUNCTION_NAME, fieldType, args, {}, true, node);
        // If the field requires a copy ctor, but we proved none exists, we cannot synthesize one for the outer struct.
        if (!ctorFct && fieldRequiresCopyCtor)
          return;
      }
      copyCtorRequired |= fieldRequiresCopyCtor;
    }

    // If we have an owning heap pointer, we need to do a memcpy of the heap storage and therefore need a default copy ctor
    if (fieldType.isHeap()) {
      assert(fieldType.isPtr());
      copyCtorRequired = true;
    }
  }

  // If we don't have any fields, that require us to do anything in the copy ctor, we can skip it
  if (!copyCtorRequired && !node->emitVTable)
    return;

  // Create the default copy ctor function
  const std::string entryName = Function::getSymbolTableEntryNameDefaultCopyCtor(node->codeLoc);
  const ParamList paramTypes = {{structType.toConstRef(node), false}};
  createDefaultStructMethod(spiceStruct, entryName, CTOR_FUNCTION_NAME, paramTypes);
}

/**
 * Checks if the given struct scope already has a user-defined move constructor and creates a default one if not.
 *
 * For generating a default move ctor, the following conditions need to be met:
 * - No user-defined move ctor
 * - At least one field requires non-trivial moving (heap pointer or struct field with move ctor)
 *
 * @param spiceStruct Struct instance
 * @param structScope Scope of the struct
 */
void TypeChecker::createDefaultMoveCtorIfRequired(const Struct &spiceStruct, Scope *structScope) {
  const auto node = spice_pointer_cast<const StructDefNode *>(spiceStruct.declNode);
  assert(structScope != nullptr && structScope->type == ScopeType::STRUCT);

  // Skip generic struct presets - we only synthesize a default move ctor for fully substantiated structs.
  // Reason: each manifestation gets its own scope (deep-copied from the generic), and the implicit move ctor
  // body's preamble writes to field lifecycle state via bodyScope->parent. If we created the move ctor on the
  // generic struct, the deep-copied Function in the manifestation's scope would still carry a bodyScope pointer
  // into the generic struct's scope, causing the preamble to update fields in the wrong scope.
  if (!spiceStruct.isFullySubstantiated())
    return;

  // Abort if the struct already has a user-defined move constructor.
  // We can't just call FunctionManager::lookup with a non-const ref arg here, since the lookup permits
  // const-param-to-non-const-arg matching ("constify") and would return the copy ctor (if one exists) as
  // a false positive. Instead, check the function manifestations directly for a single-self-non-const-ref ctor.
  if (FunctionManager::hasMoveCtor(structScope))
    return;

  // Abort if the struct has a user-defined copy ctor. Reason: in Spice the move-vs-copy tie-breaker picks the
  // non-const-ref (move) candidate for non-const lvalue arguments, so silently auto-generating a move ctor on
  // top of a user-defined copy ctor would change the behavior of existing `T b = T(a)` call sites (a's heap
  // contents would be stolen instead of deep-copied). Users that want a default move ctor in addition to their
  // own copy ctor can write an empty `p T.ctor(T& other) {}` to opt in. We still synthesize the default move
  // ctor when only the auto-generated copy ctor exists (heap-owning struct with no user ctors) - the user did
  // not write any binding semantics in that case, so picking move for non-const lvalues is fine.
  if (FunctionManager::hasUserCopyCtor(structScope))
    return;
  const QualType structType = spiceStruct.entry->getQualType();

  // Check if we have fields, that require us to do anything in the move ctor
  const size_t fieldCount = structScope->getFieldCount();
  bool moveCtorRequired = false;
  for (size_t i = 0; i < fieldCount; i++) {
    const SymbolTableEntry *fieldSymbol = structScope->lookupField(i);
    assert(fieldSymbol != nullptr && fieldSymbol->declNode != nullptr);

    QualType fieldType = fieldSymbol->getQualType();
    if (fieldType.hasAnyGenericParts() && !spiceStruct.typeMapping.empty())
      TypeMatcher::substantiateTypeWithTypeMapping(fieldType, spiceStruct.typeMapping, fieldSymbol->declNode);

    // If the field is of type struct, check whether the field's struct has its own move ctor. We use
    // findMoveCtor (a direct scan of the manifestations) rather than FunctionManager::match here for two
    // reasons: (1) match permits const-param-to-non-const-arg "constify" matching and would return the
    // field's copy ctor as a false positive, and (2) match populates the lookup cache, which would inflate
    // the per-struct cache-miss counts even for structs that have nothing to do with move ctors.
    if (fieldType.is(TY_STRUCT)) {
      Scope *bodyScope = fieldType.getBodyScope();
      moveCtorRequired |= FunctionManager::findMoveCtor(bodyScope) != nullptr;
    }

    // If we have an owning heap pointer, we transfer ownership of the heap storage
    if (fieldType.isHeap()) {
      assert(fieldType.isPtr());
      moveCtorRequired = true;
    }
  }

  // If we don't have any fields that require us to do anything special in the move ctor, we can skip it.
  // Unlike the copy ctor we do NOT generate a default move ctor just because the struct emits a vtable - the
  // vtable pointer is part of the struct layout and gets shallow-copied along with everything else, no
  // special move handling needed. Users that want to move a vtable-bearing struct fall back to the default
  // copy ctor.
  if (!moveCtorRequired)
    return;

  // Create the default move ctor function
  const std::string entryName = Function::getSymbolTableEntryNameDefaultMoveCtor(node->codeLoc);
  const ParamList paramTypes = {{structType.toNonConst().toRef(node), false}};
  createDefaultStructMethod(spiceStruct, entryName, CTOR_FUNCTION_NAME, paramTypes);
}

/**
 * Checks if the given struct scope already has a user-defined destructor and creates a default one if not.
 *
 * For generating a default dtor, the following conditions need to be met:
 * - No user-defined dtor
 *
 * @param spiceStruct Struct instance
 * @param structScope Scope of the struct
 */
void TypeChecker::createDefaultDtorIfRequired(const Struct &spiceStruct, Scope *structScope) {
  const ASTNode *node = spiceStruct.declNode;
  const SourceFile *sourceFile = structScope->sourceFile;
  assert(structScope != nullptr && structScope->type == ScopeType::STRUCT);

  // Abort if the struct already has a destructor. See createDefaultCtorIfRequired for why the name registry is asked
  // first and the struct scope only on top of it.
  const SymbolTableEntry *structEntry = spiceStruct.entry;
  const QualType &structType = structEntry->getQualType();
  const std::string fqFctName = structType.getSubType() + MEMBER_ACCESS_TOKEN + DTOR_FUNCTION_NAME;
  if (sourceFile->getNameRegistryEntry(fqFctName) || FunctionManager::hasDtor(structScope))
    return;

  // Check we have field types, that require use to do anything in the destructor
  const size_t fieldCount = structScope->getFieldCount();
  bool hasFieldsToDeAllocate = false;
  bool hasFieldsToDestruct = false;
  for (size_t i = 0; i < fieldCount; i++) {
    const SymbolTableEntry *fieldSymbol = structScope->lookupField(i);
    assert(fieldSymbol != nullptr && fieldSymbol->declNode != nullptr);

    QualType fieldType = fieldSymbol->getQualType();
    if (fieldType.hasAnyGenericParts() && !spiceStruct.typeMapping.empty())
      TypeMatcher::substantiateTypeWithTypeMapping(fieldType, spiceStruct.typeMapping, fieldSymbol->declNode);

    hasFieldsToDeAllocate |= fieldType.needsDeAllocation();
    if (fieldType.is(TY_STRUCT)) {
      Scope *fieldScope = fieldType.getBodyScope();
      // While the outer struct is still a generic preset, a field type that is itself generic (e.g.
      // RedBlackTree<V, bool>, the internal field of Set<V>) cannot be matched to a concrete dtor yet. Since a
      // dtor takes no arguments, matching it against the still-generic 'this' type would partially substantiate a
      // bogus manifestation (e.g. RedBlackTree<K, bool>, with K left generic) that is wrongly treated as fully
      // substantiated later on and crashes name mangling during IR generation. In that case we only record that
      // the field has a dtor via a direct scan; the concrete dtor is matched later, per manifestation, in
      // createDtorBodyPreamble.
      if (fieldType.hasAnyGenericParts()) {
        hasFieldsToDestruct |= FunctionManager::hasDtor(fieldScope);
      } else {
        // Lookup dtor function
        const Function *dtorFct = FunctionManager::match(fieldScope, DTOR_FUNCTION_NAME, fieldType, {}, {}, true, node);
        hasFieldsToDestruct |= dtorFct != nullptr;
        requestRevisitIfRequired(dtorFct);
      }
    }
  }

  // If we don't have any fields, that require us to do anything in the dtor, we can skip it
  if (!hasFieldsToDeAllocate && !hasFieldsToDestruct)
    return;

  // Create the default dtor function. The memory runtime that the generated body needs for its de-allocations is not
  // requested here, but only when that body is prepared (see createDtorBodyPreamble) - see the comment there.
  const std::string entryName = Function::getSymbolTableEntryNameDefaultDtor(node->codeLoc);
  createDefaultStructMethod(spiceStruct, entryName, DTOR_FUNCTION_NAME, {});
}

/**
 * Prepare the generation of the ctor body preamble. This preamble is used to initialize the VTable, construct or initialize
 * fields.
 */
void TypeChecker::createCtorBodyPreamble(const Scope *bodyScope) const {
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

    // A reference field has no default value to construct - it can only be bound by an explicit assignment in the
    // ctor body (e.g. Pair<K, V&>'s 'this.second = second;'), and that assignment is what should mark the field's
    // lifecycle as initialized (see the isDecl / initial-field-ref-assign logic in TypeChecker::visitAssignExpr and
    // IRGenerator::doAssignment). Marking it initialized here, before that binding assignment runs, would make the
    // compiler treat the binding as a reassignment through the reference instead of the initial binding - illegal
    // for a const reference field, and wrong even for a non-const one.
    if (fieldType.isRef())
      continue;

    if (fieldType.is(TY_STRUCT)) {
      const auto fieldNode = spice_pointer_cast<const FieldNode *>(fieldSymbol->declNode);
      // Match ctor function, create the concrete manifestation and set it to used
      Scope *matchScope = fieldType.getBodyScope();
      const Function *spiceFunc = FunctionManager::match(matchScope, CTOR_FUNCTION_NAME, fieldType, {}, {}, false, fieldNode);
      if (spiceFunc != nullptr)
        fieldSymbol->updateType(fieldType.getWithBodyScope(spiceFunc->thisType.getBodyScope()), true);
      else if (!fieldType.isTriviallyConstructible(fieldNode))
        continue;
    }

    fieldSymbol->updateState(INITIALIZED, fieldSymbol->declNode);
  }
}

/**
 * Prepare the generation of the copy ctor body preamble. This preamble is used to initialize the VTable, construct or initialize
 * fields.
 */
void TypeChecker::createCopyCtorBodyPreamble(const Scope *bodyScope) const {
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
      const Function *copyCtorFct = FunctionManager::match(matchScope, CTOR_FUNCTION_NAME, fieldType, args, {}, false, fieldNode);
      if (copyCtorFct != nullptr)
        fieldSymbol->updateType(fieldType.getWithBodyScope(copyCtorFct->thisType.getBodyScope()), true);
      else if (!fieldType.isTriviallyCopyable(fieldNode))
        continue;
    }

    fieldSymbol->updateState(INITIALIZED, fieldSymbol->declNode);
  }
}

/**
 * Prepare the generation of the move ctor body preamble. This preamble is used to initialize the VTable, move-construct or
 * shallow-copy fields and transfer ownership of heap allocations.
 */
void TypeChecker::createMoveCtorBodyPreamble(const Scope *bodyScope) const {
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
      // Find a move ctor on the field's struct. We use findMoveCtor rather than FunctionManager::match to
      // avoid a constify-based false positive returning the copy ctor. findMoveCtor doesn't mark the
      // function as used, so do that explicitly to ensure the inner move ctor's body is actually emitted.
      Scope *matchScope = fieldType.getBodyScope();
      if (Function *moveCtorFct = FunctionManager::findMoveCtor(matchScope)) {
        moveCtorFct->used = true;
        if (moveCtorFct->entry)
          moveCtorFct->entry->used = true;
        fieldSymbol->updateType(fieldType.getWithBodyScope(moveCtorFct->thisType.getBodyScope()), true);
      }
    }

    fieldSymbol->updateState(INITIALIZED, fieldSymbol->declNode);
  }
}

/**
 * Prepare the generation of the dtor body preamble. This preamble is used to destruct all fields and to free all heap fields.
 *
 * @param bodyScope Body scope of the dtor
 * @param node Struct definition node for error messages
 */
void TypeChecker::createDtorBodyPreamble(const Scope *bodyScope, const ASTNode *node) const {
  // Retrieve struct scope
  Scope *structScope = bodyScope->parent;
  assert(structScope != nullptr);

  const size_t fieldCount = structScope->getFieldCount();
  bool hasFieldsToDeAllocate = false;
  for (size_t i = 0; i < fieldCount; i++) {
    const size_t fieldIdx = fieldCount - 1 - i; // Destruct fields in reverse order
    const SymbolTableEntry *fieldSymbol = structScope->lookupField(fieldIdx);
    assert(fieldSymbol != nullptr && fieldSymbol->isField());
    if (fieldSymbol->isImplicitField)
      continue;

    QualType fieldType = fieldSymbol->getQualType();
    if (fieldType.hasAnyGenericParts())
      TypeMatcher::substantiateTypeWithTypeMapping(fieldType, typeMapping, fieldSymbol->declNode);

    hasFieldsToDeAllocate |= fieldType.needsDeAllocation();

    if (fieldType.is(TY_STRUCT)) {
      const auto fieldNode = spice_pointer_cast<const FieldNode *>(fieldSymbol->declNode);
      // Match ctor function, create the concrete manifestation and set it to used
      Scope *matchScope = fieldType.getBodyScope();
      FunctionManager::match(matchScope, DTOR_FUNCTION_NAME, fieldType, {}, {}, false, fieldNode);
    }
  }

  // Request the memory runtime for the de-allocations that the generated body will contain, and mark its sDealloc as
  // used so that a definition is emitted for it. This deliberately happens here and not where the dtor is created
  // (createDefaultDtorIfRequired): a manifestation can come into existence while memory_rt.spice is itself still being
  // pre-checked - sAlloc() returns Result<heap byte*>, so the very first Result manifestation is substantiated from
  // memory_rt's own signature list. Asking that half-prepared file for sDealloc, which is declared further down in it,
  // would come up empty. By the time a default member's body is prepared, every runtime module is fully pre-checked.
  // The string runtime does not use sDealloc, but frees manually to avoid a circular dependency.
  SourceFile *structSourceFile = structScope->sourceFile;
  if (hasFieldsToDeAllocate && !structSourceFile->isStringRT()) {
    const SourceFile *memoryRT = structSourceFile->requestRuntimeModule(MEMORY_RT);
    assert(memoryRT != nullptr);
    Scope *matchScope = memoryRT->globalScope.get();
    // Set dealloc function to used
    const QualType thisType(TY_DYN);
    QualType bytePtrRefType = QualType(TY_BYTE).toPtr(node).toRef(node);
    bytePtrRefType.makeHeap();
    const ArgList args = {{bytePtrRefType, false /* we always have the field as storage */}};
    Function *deallocFct = FunctionManager::match(matchScope, FCT_NAME_DEALLOC, thisType, args, {}, true, node);
    assert(deallocFct != nullptr);
    deallocFct->used = true;
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
                                                  const ArgList &args, const ASTNode *node) const {
  const QualType thisType = entry->getQualType().removeReferenceWrapper().toNonConst();
  return implicitlyCallStructMethod(thisType, methodName, args, node);
}

/**
 * Prepare the generation of a call to a method of a given struct
 *
 * @param thisType Struct type to call the method on
 * @param methodName Name of the method to call
 * @param args Provided arguments by the caller
 * @param node AST node
 */
Function *TypeChecker::implicitlyCallStructMethod(QualType thisType, const std::string &methodName, const ArgList &args,
                                                  const ASTNode *node) const {
  assert(thisType.is(TY_STRUCT));
  Scope *matchScope = thisType.getBodyScope();
  assert(matchScope->type == ScopeType::STRUCT);

  // Search for dtor
  if (matchScope->isImportedBy(rootScope))
    thisType = mapLocalTypeToImportedScopeType(matchScope, thisType);
  return FunctionManager::match(matchScope, methodName, thisType, args, {}, true, node);
}

/**
 * Prepare the generation of a call to the copy ctor of a given struct
 *
 * @param entry Symbol entry to use as 'this' pointer for the copy ctor call
 * @param node Current AST node
 */
Function *TypeChecker::implicitlyCallStructCopyCtor(const SymbolTableEntry *entry, const ASTNode *node) const {
  assert(entry != nullptr && entry->getQualType().is(TY_STRUCT));
  return implicitlyCallStructCopyCtor(entry->getQualType(), node);
}

/**
 * Prepare the generation of a call to the copy ctor of a given struct
 *
 * @param thisType Struct type to call the copy ctor on
 * @param node Current AST node
 */
Function *TypeChecker::implicitlyCallStructCopyCtor(const QualType &thisType, const ASTNode *node) const {
  const QualType argType = thisType.removeReferenceWrapper().toConstRef(node);
  const ArgList args = {{argType, false /* we always have an entry here */}};
  return implicitlyCallStructMethod(thisType, CTOR_FUNCTION_NAME, args, node);
}

/**
 * Prepare the generation of a call to the move ctor of a given struct
 *
 * @param entry Symbol entry to use as 'this' pointer for the move ctor call
 * @param node Current AST node
 */
Function *TypeChecker::implicitlyCallStructMoveCtor(const SymbolTableEntry *entry, const ASTNode *node) const {
  assert(entry != nullptr && entry->getQualType().is(TY_STRUCT));
  return implicitlyCallStructMoveCtor(entry->getQualType(), node);
}

/**
 * Prepare the generation of a call to the move ctor of a given struct
 *
 * @param thisType Struct type to call the move ctor on
 * @param node Current AST node
 */
Function *TypeChecker::implicitlyCallStructMoveCtor(const QualType &thisType, const ASTNode *node) const {
  const QualType argType = thisType.removeReferenceWrapper().toNonConst().toRef(node);
  const ArgList args = {{argType, false /* we always have an entry here */}};
  return implicitlyCallStructMethod(thisType, CTOR_FUNCTION_NAME, args, node);
}

/**
 * Prepare the generation of a call to the dtor of a given struct
 *
 * @param entry Symbol entry to use as 'this' pointer for the dtor call
 * @param node StmtLstNode for the current scope
 */
void TypeChecker::implicitlyCallStructDtor(SymbolTableEntry *entry, StmtLstNode *node) const {
  // Add the dtor to the stmt list node to call it later in codegen
  if (Function *dtor = implicitlyCallStructMethod(entry, DTOR_FUNCTION_NAME, {}, node))
    node->resourcesToCleanup.at(manIdx).dtorFunctionsToCall.emplace_back(entry, dtor);
}

/**
 * Prepare the generation of a call to the deallocate function for a heap-allocated variable
 *
 * @param node Current AST node for error messages
 */
void TypeChecker::implicitlyCallDeallocate(const ASTNode *node) const {
  const SourceFile *memoryRT = sourceFile->requestRuntimeModule(MEMORY_RT);
  assert(memoryRT != nullptr);
  Scope *matchScope = memoryRT->globalScope.get();
  // Set dealloc function to used
  const QualType thisType(TY_DYN);
  QualType bytePtrRefType = QualType(TY_BYTE).toPtr(node).toRef(node);
  bytePtrRefType.makeHeap();
  const ArgList args = {{bytePtrRefType, false /* we always have the field as storage */}};
  Function *deallocFct = FunctionManager::match(matchScope, FCT_NAME_DEALLOC, thisType, args, {}, true, node);
  assert(deallocFct != nullptr);
  deallocFct->used = true;
}

/**
 * Consider calls to destructors for the given scope
 *
 * @param node StmtLstNode for the current scope
 */
void TypeChecker::doScopeCleanup(StmtLstNode *node) const {
  // Get all variables, that are approved for de-allocation
  std::vector<SymbolTableEntry *> vars = currentScope->getVarsGoingOutOfScope();
  // Sort by reverse declaration order
  const auto comp = [this](const SymbolTableEntry *a, const SymbolTableEntry *b) {
    const ASTNode *aDeclNode = a->declNode;
    const ASTNode *bDeclNode = b->declNode;
    // Primary sort criteria is the code location
    if (aDeclNode->codeLoc != bDeclNode->codeLoc)
      return aDeclNode->codeLoc > bDeclNode->codeLoc;
    // Secondary sort criteria is the node id
    return resourceManager.nodeToNodeId[aDeclNode] > resourceManager.nodeToNodeId[bDeclNode];
  };
  std::ranges::stable_sort(vars, comp);
  // Call the dtor of each variable. We call the dtor in reverse declaration order
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
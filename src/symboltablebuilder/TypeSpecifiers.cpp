// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "TypeSpecifiers.h"

#include <exception/CompilerError.h>
#include <symboltablebuilder/SymbolType.h>

namespace spice::compiler {

TypeSpecifiers TypeSpecifiers::of(uint16_t superType) {
  switch (superType) {
  case TY_DOUBLE: // fall-through
  case TY_INT:    // fall-through
  case TY_SHORT:  // fall-through
  case TY_LONG:
    return {/*const*/ false, /*signed*/ true, /*unsigned*/ false, /*inline*/ false, /*public*/ false, /*heap*/ false};
  case TY_BYTE:   // fall-through
  case TY_CHAR:   // fall-through
  case TY_STRING: // fall-through
  case TY_BOOL:   // fall-through
  case TY_PTR:    // fall-through
  case TY_REF:    // fall-through
  case TY_ARRAY:  // fall-through
    return {/*const*/ false, /*signed*/ false, /*unsigned*/ true, /*inline*/ false, /*public*/ false, /*heap*/ false};
  case TY_GENERIC:
    // Generics must be non-signed and non-unsigned at the same time to ensure a proper function matching
    return {/*const*/ false, /*signed*/ false, /*unsigned*/ false, /*inline*/ false, /*public*/ false, /*heap*/ false};
  case TY_STRUCT: // fall-through
  case TY_INTERFACE:
    return {/*const*/ false, /*signed*/ false, /*unsigned*/ true, /*inline*/ false, /*public*/ false, /*heap*/ false};
  case TY_ENUM: // fall-through
  case TY_ALIAS:
    return {/*const*/ true, /*signed*/ false, /*unsigned*/ true, /*inline*/ false, /*public*/ false, /*heap*/ false};
  case TY_FUNCTION: // fall-through
  case TY_PROCEDURE:
    return {/*const*/ false, /*signed*/ false, /*unsigned*/ true, /*inline*/ false, /*public*/ false, /*heap*/ false};
  case TY_IMPORT:
    return {/*const*/ true, /*signed*/ false, /*unsigned*/ true, /*inline*/ false, /*public*/ false, /*heap*/ false};
  case TY_DYN:
  case TY_INVALID:
  case TY_UNRESOLVED:
    // Return all-false specifiers to not match anything
    return {/*const*/ false, /*signed*/ false, /*unsigned*/ false, /*inline*/ false, /*public*/ false, /*heap*/ false};
  default:
    throw CompilerError(UNHANDLED_BRANCH, "Symbol specifier fallthrough"); // GCOV_EXCL_LINE
  }
}

/**
 * Merge two type specifiers. If possible, prefer the opposite of the default of the super type
 *
 * @param other Other type specifiers object
 * @return Merged specifiers object
 */
TypeSpecifiers TypeSpecifiers::merge(const TypeSpecifiers &other) const {
  TypeSpecifiers result;
  const bool isGeneric = !getBit(BIT_INDEX_SIGNED) && !getBit(BIT_INDEX_UNSIGNED);
  for (uint8_t i = 0; i <= BIT_INDEX_MAX; i++) {
    const bool x = getBit(i);
    const bool y = other.getBit(i);

    if (i == BIT_INDEX_SIGNED || i == BIT_INDEX_UNSIGNED) {
      result.setBit(i, isGeneric ? y : x);
    } else {
      result.setBit(i, x | y);
    }
  }
  return result;
}

/**
 * Check if two type specifiers match
 *
 * @param otherSpecifiers The rhs specifiers
 * @param allowConstify Match when the types are the same, but the lhs type is more const restrictive than the rhs type
 * @return Matching or not
 */
bool TypeSpecifiers::match(TypeSpecifiers otherSpecifiers, bool allowConstify) const {
  TypeSpecifiers thisSpecifiers = *this;

  // Zero out public specifier to ignore it while matching
  thisSpecifiers.isPublic = false;
  otherSpecifiers.isPublic = false;

  // Zero out inline specifier to ignore it while matching
  thisSpecifiers.isInline = false;
  otherSpecifiers.isInline = false;

  // If allowConstify is enabled, only allow to match lhs=const and rhs=non-const
  if (allowConstify && thisSpecifiers.isConst && !otherSpecifiers.isConst)
    otherSpecifiers.isConst = true;

  // Check if specifiers are equal
  return thisSpecifiers == otherSpecifiers;
}

bool operator==(const TypeSpecifiers &lhs, const TypeSpecifiers &rhs) {
  const bool isConst = lhs.isConst == rhs.isConst;
  const bool isSigned = lhs.isSigned == rhs.isSigned;
  const bool isUnsigned = lhs.isUnsigned == rhs.isUnsigned;
  const bool isInline = lhs.isInline == rhs.isInline;
  const bool isPublic = lhs.isPublic == rhs.isPublic;
  const bool isHeap = lhs.isHeap == rhs.isHeap;
  return isConst && isSigned && isUnsigned && isInline && isPublic && isHeap;
}

bool TypeSpecifiers::getBit(uint8_t index) const {
  switch (index) {
  case BIT_INDEX_CONST:
    return isConst;
  case BIT_INDEX_SIGNED:
    return isSigned;
  case BIT_INDEX_UNSIGNED:
    return isUnsigned;
  case BIT_INDEX_INLINE:
    return isInline;
  case BIT_INDEX_PUBLIC:
    return isPublic;
  case BIT_INDEX_HEAP:
    return isHeap;
  default:
    throw CompilerError(UNHANDLED_BRANCH, "Bit index fallthrough"); // GCOV_EXCL_LINE
  }
}

bool TypeSpecifiers::setBit(uint8_t index, bool value) {
  switch (index) {
  case BIT_INDEX_CONST:
    return isConst = value;
  case BIT_INDEX_SIGNED:
    return isSigned = value;
  case BIT_INDEX_UNSIGNED:
    return isUnsigned = value;
  case BIT_INDEX_INLINE:
    return isInline = value;
  case BIT_INDEX_PUBLIC:
    return isPublic = value;
  case BIT_INDEX_HEAP:
    return isHeap = value;
  default:
    throw CompilerError(UNHANDLED_BRANCH, "Bit index fallthrough"); // GCOV_EXCL_LINE
  }
}

} // namespace spice::compiler
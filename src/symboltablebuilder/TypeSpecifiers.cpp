// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "TypeSpecifiers.h"

#include <exception/CompilerError.h>
#include <symboltablebuilder/TypeChain.h>

namespace spice::compiler {

/**
 * Get default type specifiers for a given super type
 *
 * @param superType Super type
 * @return Default type specifiers
 */
TypeSpecifiers TypeSpecifiers::of(uint16_t superType) {
  switch (superType) {
  case TY_DOUBLE: // fall-through
  case TY_INT:    // fall-through
  case TY_SHORT:  // fall-through
  case TY_LONG:
    return {/*const*/ false, /*signed*/ true, /*unsigned*/ false, /*heap*/ false};
  case TY_BYTE:      // fall-through
  case TY_CHAR:      // fall-through
  case TY_STRING:    // fall-through
  case TY_BOOL:      // fall-through
  case TY_PTR:       // fall-through
  case TY_REF:       // fall-through
  case TY_ARRAY:     // fall-through
  case TY_STRUCT:    // fall-through
  case TY_INTERFACE: // fall-through
  case TY_FUNCTION:  // fall-through
  case TY_PROCEDURE:
    return {/*const*/ false, /*signed*/ false, /*unsigned*/ true, /*heap*/ false};
  case TY_GENERIC:
    // Generics must be non-signed and non-unsigned at the same time to ensure a proper function matching
    return {/*const*/ false, /*signed*/ false, /*unsigned*/ false, /*heap*/ false};
  case TY_ENUM:  // fall-through
  case TY_ALIAS: // fall-through
  case TY_IMPORT:
    return {/*const*/ true, /*signed*/ false, /*unsigned*/ true, /*heap*/ false};
  case TY_DYN:     // fall-through
  case TY_INVALID: // fall-through
  case TY_UNRESOLVED:
    // Return all-false specifiers to not match anything
    return {/*const*/ false, /*signed*/ false, /*unsigned*/ false, /*heap*/ false};
  default:
    throw CompilerError(UNHANDLED_BRANCH, "Symbol specifier fallthrough"); // GCOV_EXCL_LINE
  }
}

/**
 * Merge two sets of type specifiers. If possible, prefer the opposite of the default of the super type
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
 * Check if two sets of type specifiers match
 *
 * @param other The rhs specifiers
 * @param allowConstify Match when the types are the same, but the lhs type is more const restrictive than the rhs type
 * @return Matching or not
 */
bool TypeSpecifiers::match(TypeSpecifiers other, bool allowConstify) const {
  const TypeSpecifiers thisSpecifiers = *this;

  // If allowConstify is enabled, only allow to match lhs=const and rhs=non-const
  if (allowConstify && thisSpecifiers.isConst && !other.isConst)
    other.isConst = true;

  // Check if specifiers are equal
  return thisSpecifiers == other;
}

/**
 * Erase all specifiers that are set in the mask. This is used in type matching.
 *
 * @param mask Bitmask to erase with
 */
void TypeSpecifiers::eraseWithMask(const TypeSpecifiers &mask) {
  // Zero out all bits that are set in the mask
  for (uint8_t i = 0; i <= BIT_INDEX_MAX; i++) {
    if (mask.getBit(i)) {
      // Zero out the bit
      setBit(i, false);

      // If we set the signed/unsigned bit to zero, we need to set the other to one
      if (i == BIT_INDEX_SIGNED) {
        setBit(BIT_INDEX_UNSIGNED, true);
      } else if (i == BIT_INDEX_UNSIGNED) {
        setBit(BIT_INDEX_SIGNED, true);
      }
    }
  }
}

bool operator==(const TypeSpecifiers &lhs, const TypeSpecifiers &rhs) {
  const bool isConst = lhs.isConst == rhs.isConst;
  const bool isSigned = lhs.isSigned == rhs.isSigned;
  const bool isUnsigned = lhs.isUnsigned == rhs.isUnsigned;
  const bool isHeap = lhs.isHeap == rhs.isHeap;
  return isConst && isSigned && isUnsigned && isHeap;
}

bool TypeSpecifiers::getBit(uint8_t index) const {
  switch (index) {
  case BIT_INDEX_CONST:
    return isConst;
  case BIT_INDEX_SIGNED:
    return isSigned;
  case BIT_INDEX_UNSIGNED:
    return isUnsigned;
  case BIT_INDEX_HEAP:
    return isHeap;
  case BIT_INDEX_PUBLIC:
    return isPublic;
  case BIT_INDEX_INLINE:
    return isInline;
  case BIT_INDEX_COMPOSITION:
    return isComposition;
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
  case BIT_INDEX_HEAP:
    return isHeap = value;
  case BIT_INDEX_PUBLIC:
    return isPublic = value;
  case BIT_INDEX_INLINE:
    return isInline = value;
  case BIT_INDEX_COMPOSITION:
    return isComposition = value;
  default:
    throw CompilerError(UNHANDLED_BRANCH, "Bit index fallthrough"); // GCOV_EXCL_LINE
  }
}

} // namespace spice::compiler
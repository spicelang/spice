// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "TypeSpecifiers.h"

#include <exception/CompilerError.h>
#include <symboltablebuilder/SymbolType.h>

namespace spice::compiler {

TypeSpecifiers TypeSpecifiers::of(uint16_t superType) {
  switch (superType) {
  case TY_DOUBLE:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_DOUBLE);
  case TY_INT:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_INT);
  case TY_SHORT:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_SHORT);
  case TY_LONG:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_LONG);
  case TY_BYTE:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_BYTE);
  case TY_CHAR:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_CHAR);
  case TY_STRING:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_STRING);
  case TY_BOOL:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_BOOL);
  case TY_PTR:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_PTR);
  case TY_ARRAY:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_ARRAY);
  case TY_GENERIC:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_GENERIC);
  case TY_STRUCT:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_STRUCT);
  case TY_INTERFACE:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_INTERFACE);
  case TY_ENUM:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_ENUM);
  case TY_ALIAS:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_ALIAS);
  case TY_FUNCTION:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_FUNCTION);
  case TY_PROCEDURE:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_PROCEDURE);
  case TY_IMPORT:
    return TypeSpecifiers(SPECIFIER_DEFAULTS_IMPORT);
  case TY_DYN:
  case TY_INVALID:
    return {};
  default:
    throw CompilerError(UNHANDLED_BRANCH, "Symbol specifier fallthrough"); // GCOV_EXCL_LINE
  }
}

/**
 * Set or clear the const flag
 *
 * @param value True or false
 */
void TypeSpecifiers::setConst(bool value) { writeBit(BIT_INDEX_CONST, value); }

/**
 * Check if the const flag is set
 *
 * @return True or false
 */
bool TypeSpecifiers::isConst() const { return getBit(BIT_INDEX_CONST); }

/**
 * Set or clear the signed flag
 *
 * @param value True or false
 */
void TypeSpecifiers::setSigned(bool value) {
  writeBit(BIT_INDEX_SIGNED, value);
  writeBit(BIT_INDEX_UNSIGNED, !value);
}

/**
 * Check if the signed flag is set
 *
 * @return True or false
 */
bool TypeSpecifiers::isSigned() const { return getBit(BIT_INDEX_SIGNED); }

/**
 * Set or clear the inline flag
 *
 * @param value True or false
 */
void TypeSpecifiers::setInline(bool value) { writeBit(BIT_INDEX_INLINE, value); }

/**
 * Check if the inline flag is set
 *
 * @return True or false
 */
bool TypeSpecifiers::isInline() const { return getBit(BIT_INDEX_INLINE); }

/**
 * Set or clear the public flag
 *
 * @param value True or false
 */
void TypeSpecifiers::setPublic(bool value) { writeBit(BIT_INDEX_PUBLIC, value); }

/**
 * Check if the public flag is set
 *
 * @return True or false
 */
bool TypeSpecifiers::isPublic() const { return getBit(BIT_INDEX_PUBLIC); }

/**
 * Set or clear the heap flag
 *
 * @param value True or false
 */
void TypeSpecifiers::setHeap(bool value) { writeBit(BIT_INDEX_HEAP, value); }

/**
 * Check if the heap flag is set
 *
 * @return True or false
 */
bool TypeSpecifiers::isHeap() const { return getBit(BIT_INDEX_HEAP); }

/**
 * Set bit in specifier value
 *
 * @param index Index of the bit to set
 */
void TypeSpecifiers::setBit(unsigned short index) { specifierValue |= (1 << index); }

/**
 * Write the specified boolean value to the bit at the specified index
 *
 * @param index Index of the bit to modify
 */
void TypeSpecifiers::writeBit(unsigned short index, bool value) {
  if (value)
    setBit(index);
  else
    clearBit(index);
}

/**
 * Clear bit in specifier value
 *
 * @param index Index of the bit to clear
 */
void TypeSpecifiers::clearBit(unsigned short index) { specifierValue &= ~(1 << index); }

/**
 * Get the bit at a specific index
 *
 * @param index Index of the bit to get
 * @return True or false
 */
bool TypeSpecifiers::getBit(unsigned short index) const { return ((specifierValue >> index) & 1) == 1; }

/**
 * Merge two type specifiers. If possible, prefer the opposite of the default of the super type
 *
 * @param other Other type specifiers object
 * @return Merged specifiers object
 */
TypeSpecifiers TypeSpecifiers::merge(const TypeSpecifiers &other) const {
  TypeSpecifiers result;
  bool isGeneric = !getBit(BIT_INDEX_SIGNED) && !getBit(BIT_INDEX_UNSIGNED);
  for (short i = 0; i < BIT_INDEX_HEAP; i++) {
    const bool x = getBit(i);
    const bool y = other.getBit(i);

    if (i == BIT_INDEX_SIGNED || i == BIT_INDEX_UNSIGNED) {
      result.writeBit(i, isGeneric ? y : x);
    } else {
      result.writeBit(i, x | y);
    }
  }
  return result;
}

bool operator==(const TypeSpecifiers &lhs, const TypeSpecifiers &rhs) { return lhs.specifierValue == rhs.specifierValue; }

} // namespace spice::compiler
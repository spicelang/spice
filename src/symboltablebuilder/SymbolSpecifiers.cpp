// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "SymbolSpecifiers.h"

#include <exception/CompilerError.h>
#include <symboltablebuilder/SymbolType.h>

namespace spice::compiler {

SymbolSpecifiers SymbolSpecifiers::of(const SymbolType &symbolType) { return of(symbolType.getSuperType()); }

SymbolSpecifiers SymbolSpecifiers::of(SymbolSuperType superType) {
  switch (superType) {
  case TY_DOUBLE:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_DOUBLE);
  case TY_INT:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_INT);
  case TY_SHORT:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_SHORT);
  case TY_LONG:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_LONG);
  case TY_BYTE:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_BYTE);
  case TY_CHAR:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_CHAR);
  case TY_STRING:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_STRING);
  case TY_BOOL:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_BOOL);
  case TY_PTR:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_PTR);
  case TY_ARRAY:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_ARRAY);
  case TY_GENERIC:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_GENERIC);
  case TY_STRUCT:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_STRUCT);
  case TY_INTERFACE:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_INTERFACE);
  case TY_ENUM:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_ENUM);
  case TY_ALIAS:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_ALIAS);
  case TY_FUNCTION:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_FUNCTION);
  case TY_PROCEDURE:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_PROCEDURE);
  case TY_IMPORT:
    return SymbolSpecifiers(SPECIFIER_DEFAULTS_IMPORT);
  case TY_DYN:
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
void SymbolSpecifiers::setConst(bool value) {
  if (value)
    setBit(BIT_INDEX_CONST);
  else
    clearBit(BIT_INDEX_CONST);
}

/**
 * Check if the const flag is set
 *
 * @return True or false
 */
bool SymbolSpecifiers::isConst() const { return getBit(BIT_INDEX_CONST); }

/**
 * Set or clear the signed flag
 *
 * @param value True or false
 */
void SymbolSpecifiers::setSigned(bool value) {
  if (value)
    setBit(BIT_INDEX_SIGNED);
  else
    clearBit(BIT_INDEX_SIGNED);
}

/**
 * Check if the signed flag is set
 *
 * @return True or false
 */
bool SymbolSpecifiers::isSigned() const { return getBit(BIT_INDEX_SIGNED); }

/**
 * Set or clear the inline flag
 *
 * @param value True or false
 */
void SymbolSpecifiers::setInline(bool value) {
  if (value)
    setBit(BIT_INDEX_INLINE);
  else
    clearBit(BIT_INDEX_INLINE);
}

/**
 * Check if the inline flag is set
 *
 * @return True or false
 */
bool SymbolSpecifiers::isInline() const { return getBit(BIT_INDEX_INLINE); }

/**
 * Set or clear the public flag
 *
 * @param value True or false
 */
void SymbolSpecifiers::setPublic(bool value) {
  if (value)
    setBit(BIT_INDEX_PUBLIC);
  else
    clearBit(BIT_INDEX_PUBLIC);
}

/**
 * Check if the public flag is set
 *
 * @return True or false
 */
bool SymbolSpecifiers::isPublic() const { return getBit(BIT_INDEX_PUBLIC); }

/**
 * Set or clear the heap flag
 *
 * @param value True or false
 */
void SymbolSpecifiers::setHeap(bool value) {
  if (value)
    setBit(BIT_INDEX_HEAP);
  else
    clearBit(BIT_INDEX_HEAP);
}

/**
 * Check if the heap flag is set
 *
 * @return True or false
 */
bool SymbolSpecifiers::isHeap() const { return getBit(BIT_INDEX_HEAP); }

/**
 * Set or clear bit in specifier value
 *
 * @param index Index of the bit to set
 */
void SymbolSpecifiers::setBit(unsigned short index) { specifierValue |= (1 << index); }

/**
 * Clear bit in specifier value
 *
 * @param index Index of the bit to clear
 */
void SymbolSpecifiers::clearBit(unsigned short index) { specifierValue &= ~(1 << index); }

/**
 * Get the bit at a specific index
 *
 * @param index Index of the bit to get
 * @return True or false
 */
bool SymbolSpecifiers::getBit(unsigned short index) const { return ((specifierValue >> index) & 1) == 1; }

/**
 * Returns the JSON representation of the specifier collection
 *
 * @return Specifiers as JSON object
 */
nlohmann::ordered_json SymbolSpecifiers::toJSON() const {
  nlohmann::json result;
  result["const"] = isConst();
  result["signed"] = isSigned();
  result["public"] = isPublic();
  return result;
}

} // namespace spice::compiler
// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "SymbolSpecifiers.h"

#include <stdexcept>

#include <symbol/SymbolType.h>

SymbolSpecifiers::SymbolSpecifiers(const SymbolType &type) {
  switch (type.getSuperType()) {
  case TY_DOUBLE:
    specifierValue = SPECIFIER_DEFAULTS_DOUBLE;
    break;
  case TY_INT:
    specifierValue = SPECIFIER_DEFAULTS_INT;
    break;
  case TY_SHORT:
    specifierValue = SPECIFIER_DEFAULTS_SHORT;
    break;
  case TY_LONG:
    specifierValue = SPECIFIER_DEFAULTS_LONG;
    break;
  case TY_BYTE:
    specifierValue = SPECIFIER_DEFAULTS_BYTE;
    break;
  case TY_CHAR:
    specifierValue = SPECIFIER_DEFAULTS_CHAR;
    break;
  case TY_STRING:
    specifierValue = SPECIFIER_DEFAULTS_STRING;
    break;
  case TY_STROBJ:
    specifierValue = SPECIFIER_DEFAULTS_STROBJ;
    break;
  case TY_BOOL:
    specifierValue = SPECIFIER_DEFAULTS_BOOL;
    break;
  case TY_PTR:
    specifierValue = SPECIFIER_DEFAULTS_PTR;
    break;
  case TY_ARRAY:
    specifierValue = SPECIFIER_DEFAULTS_ARRAY;
    break;
  case TY_GENERIC:
    specifierValue = SPECIFIER_DEFAULTS_GENERIC;
    break;
  case TY_STRUCT:
    specifierValue = SPECIFIER_DEFAULTS_STRUCT;
    break;
  case TY_INTERFACE:
    specifierValue = SPECIFIER_DEFAULTS_INTERFACE;
    break;
  case TY_ENUM:
    specifierValue = SPECIFIER_DEFAULTS_ENUM;
    break;
  case TY_FUNCTION:
    specifierValue = SPECIFIER_DEFAULTS_FUNCTION;
    break;
  case TY_PROCEDURE:
    specifierValue = SPECIFIER_DEFAULTS_PROCEDURE;
    break;
  case TY_IMPORT:
    specifierValue = SPECIFIER_DEFAULTS_IMPORT;
    break;
  case TY_DYN:
    break;
  default:                                                    // GCOV_EXCL_LINE
    throw std::runtime_error("Symbol specifier fallthrough"); // GCOV_EXCL_LINE
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
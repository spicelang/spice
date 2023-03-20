// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include "../../lib/json/json.hpp"

namespace spice::compiler {

// Bit indices from right to left
const unsigned short BIT_INDEX_PUBLIC = 0;
const unsigned short BIT_INDEX_UNSIGNED = 1;
const unsigned short BIT_INDEX_SIGNED = 2;
const unsigned short BIT_INDEX_CONST = 3;
const unsigned short BIT_INDEX_INLINE = 4;
const unsigned short BIT_INDEX_HEAP = 5;

// Defaults: Heap: 0, Const: 0, Signed: 1, Unsigned: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_DOUBLE = 0b00100;
// Defaults: Heap: 0, Const: 0, Signed: 1, Unsigned: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_INT = 0b00100;
// Defaults: Heap: 0, Const: 0, Signed: 1, Unsigned: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_SHORT = 0b00100;
// Defaults: Heap: 0, Const: 0, Signed: 1, Unsigned: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_LONG = 0b00100;
// Defaults: Heap: 0, Const: 0, Signed: 0, Unsigned: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_BYTE = 0b00010;
// Defaults: Heap: 0, Const: 0, Signed: 0, Unsigned: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_CHAR = 0b00010;
// Defaults: Heap: 0, Const: 0, Signed: 0, Unsigned: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_STRING = 0b00010;
// Defaults: Heap: 0, Const: 0, Signed: 0, Unsigned: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_BOOL = 0b00010;
// Defaults: Heap: 0, Const: 0, Signed: 0, Unsigned: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_PTR = 0b00010;
// Defaults: Heap: 0, Const: 0, Signed: 0, Unsigned: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_ARRAY = 0b00010;
// Defaults: Heap: 0, Const: 0, Signed: 0, Unsigned: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_GENERIC = 0b00000;
// Defaults: Heap: 0, Const: 0, Signed: 0, Unsigned: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_STRUCT = 0b00010;
// Defaults: Heap: 0, Const: 0; Signed: 0, Unsigned: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_INTERFACE = 0b00010;
// Defaults: Heap: 0, Const: 1, Signed: 0, Unsigned: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_ENUM = 0b01010;
// Defaults: Heap: 0, Const: 1, Signed: 0, Unsigned: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_ALIAS = 0b01010;
// Defaults: Heap: 0, Const: 1, Signed: 0, Unsigned: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_FUNCTION = 0b01010;
// Defaults: Heap: 0, Const: 1, Signed: 0, Unsigned: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_PROCEDURE = 0b01010;
// Defaults: Heap: 0, Const: 1, Signed: 0, Unsigned: 1, Public: 1
const unsigned short SPECIFIER_DEFAULTS_IMPORT = 0b01011;

class TypeSpecifiers {
public:
  // Constructors
  TypeSpecifiers() = default;
  explicit TypeSpecifiers(unsigned short initialValue) : specifierValue(initialValue) {}

  // Public static methods
  static TypeSpecifiers of(uint16_t superType);

  // Public methods
  void setConst(bool value);
  [[nodiscard]] bool isConst() const;
  void setSigned(bool value);
  [[nodiscard]] bool isSigned() const;
  void setInline(bool value);
  [[nodiscard]] bool isInline() const;
  void setPublic(bool value);
  [[nodiscard]] bool isPublic() const;
  void setHeap(bool heap);
  [[nodiscard]] bool isHeap() const;
  [[nodiscard]] TypeSpecifiers merge(const TypeSpecifiers &other) const;

  // Overloaded operators
  friend bool operator==(const TypeSpecifiers &lhs, const TypeSpecifiers &rhs);

  // JSON serializer/deserializer
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(TypeSpecifiers, specifierValue)

private:
  // Private members
  unsigned short specifierValue = 0;
  void setBit(unsigned short index);
  void writeBit(unsigned short index, bool value);
  void clearBit(unsigned short index);
  [[nodiscard]] bool getBit(unsigned short index) const;
};

} // namespace spice::compiler
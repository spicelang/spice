// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include "../../lib/json/json.hpp"
#include "SymbolType.h"

namespace spice::compiler {

// Bit indices from right to left
const unsigned short BIT_INDEX_PUBLIC = 0;
const unsigned short BIT_INDEX_SIGNED = 1;
const unsigned short BIT_INDEX_CONST = 2;
const unsigned short BIT_INDEX_INLINE = 3;
const unsigned short BIT_INDEX_HEAP = 4;

// Defaults: Heap: 0, Const: 0, Signed: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_DOUBLE = 0b0010;
// Defaults: Heap: 0, Const: 0, Signed: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_INT = 0b0010;
// Defaults: Heap: 0, Const: 0, Signed: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_SHORT = 0b0010;
// Defaults: Heap: 0, Const: 0, Signed: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_LONG = 0b0010;
// Defaults: Heap: 0, Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_BYTE = 0b0000;
// Defaults: Heap: 0, Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_CHAR = 0b0000;
// Defaults: Heap: 0, Const: 1, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_STRING = 0b0100;
// Defaults: Heap: 0, Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_BOOL = 0b0000;
// Defaults: Heap: 0, Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_PTR = 0b0000;
// Defaults: Heap: 0, Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_ARRAY = 0b0000;
// Defaults: Heap: 0, Const: 1, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_GENERIC = 0b0000;
// Defaults: Heap: 0, Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_STRUCT = 0b0000;
// Defaults: Heap: 0, Const: 0; Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_INTERFACE = 0b0000;
// Defaults: Heap: 0, Const: 1, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_ENUM = 0b0100;
// Defaults: Heap: 0, Const: 1, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_ALIAS = 0b0100;
// Defaults: Heap: 0, Const: 1, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_FUNCTION = 0b0100;
// Defaults: Heap: 0, Const: 1, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_PROCEDURE = 0b0100;
// Defaults: Heap: 0, Const: 1, Signed: 0, Public: 1
const unsigned short SPECIFIER_DEFAULTS_IMPORT = 0b0101;

class SymbolSpecifiers {
public:
  // Constructors
  SymbolSpecifiers() = default;
  explicit SymbolSpecifiers(unsigned short initialValue) : specifierValue(initialValue) {}

  // Public static methods
  static SymbolSpecifiers of(const SymbolType &superType);
  static SymbolSpecifiers of(SymbolSuperType superType);

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
  [[nodiscard]] nlohmann::ordered_json toJSON() const;

  // JSON serializer/deserializer
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SymbolSpecifiers, specifierValue)

private:
  // Private members
  unsigned short specifierValue = 0;
  void setBit(unsigned short index);
  void clearBit(unsigned short index);
  [[nodiscard]] bool getBit(unsigned short index) const;
};

} // namespace spice::compiler
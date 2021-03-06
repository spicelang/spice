// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include "../../lib/json/json.hpp"

// Forward declarations
class SymbolType;

// Bit indices from right to left
const unsigned short BIT_INDEX_PUBLIC = 0;
const unsigned short BIT_INDEX_SIGNED = 1;
const unsigned short BIT_INDEX_CONST = 2;
const unsigned short BIT_INDEX_INLINE = 3;

// Defaults: Const: 0, Signed: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_DOUBLE = 0b0010;
// Defaults: Const: 0, Signed: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_INT = 0b0010;
// Defaults: Const: 0, Signed: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_SHORT = 0b0010;
// Defaults: Const: 0, Signed: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_LONG = 0b0010;
// Defaults: Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_BYTE = 0b0000;
// Defaults: Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_CHAR = 0b0000;
// Defaults: Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_STRING = 0b0000;
// Defaults: Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_BOOL = 0b0000;
// Defaults: Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_PTR = 0b0000;
// Defaults: Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_ARRAY = 0b0000;
// Defaults: Const: 1, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_GENERIC = 0b0000;
// Defaults: Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_STRUCT = 0b0000;
// Defaults: Const: 1, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_FUNCTION = 0b0100;
// Defaults: Const: 1, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_PROCEDURE = 0b0100;
// Defaults: Const: 1, Signed: 0, Public: 1
const unsigned short SPECIFIER_DEFAULTS_IMPORT = 0b0101;

class SymbolSpecifiers {
public:
  explicit SymbolSpecifiers(const SymbolType &symbolType);
  void setConst(bool value);
  [[nodiscard]] bool isConst() const;
  void setSigned(bool value);
  [[nodiscard]] bool isSigned() const;
  void setInline(bool value);
  [[nodiscard]] bool isInline() const;
  void setPublic(bool value);
  [[nodiscard]] bool isPublic() const;
  [[nodiscard]] nlohmann::ordered_json toJSON() const;

private:
  unsigned short specifierValue = 0;
  void setBit(unsigned short index);
  void clearBit(unsigned short index);
  [[nodiscard]] bool getBit(unsigned short index) const;
};
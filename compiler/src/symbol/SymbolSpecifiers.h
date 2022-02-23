// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <symbol/SymbolType.h>

// Bit indices from right to left
const unsigned short BIT_INDEX_PUBLIC = 0;
const unsigned short BIT_INDEX_SIGNED = 1;
const unsigned short BIT_INDEX_CONST = 2;

// Defaults: Const: 0, Signed: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_DOUBLE = 0b010;
// Defaults: Const: 0, Signed: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_INT = 0b010;
// Defaults: Const: 0, Signed: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_SHORT = 0b010;
// Defaults: Const: 0, Signed: 1, Public: 0
const unsigned short SPECIFIER_DEFAULTS_LONG = 0b010;
// Defaults: Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_BYTE = 0b000;
// Defaults: Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_CHAR = 0b000;
// Defaults: Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_STRING = 0b000;
// Defaults: Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_BOOL = 0b000;
// Defaults: Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_PTR = 0b000;
// Defaults: Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_ARRAY = 0b000;
// Defaults: Const: 0, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_STRUCT = 0b000;
// Defaults: Const: 1, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_FUNCTION = 0b100;
// Defaults: Const: 1, Signed: 0, Public: 0
const unsigned short SPECIFIER_DEFAULTS_PROCEDURE = 0b100;
// Defaults: Const: 1, Signed: 0, Public: 1
const unsigned short SPECIFIER_DEFAULTS_IMPORT = 0b101;

class SymbolSpecifiers {
public:
    explicit SymbolSpecifiers(SymbolType symbolType);
    void setConst(bool value);
    [[nodiscard]] bool isConst() const;
    void setSigned(bool value);
    [[nodiscard]] bool isSigned() const;
    void setPublic(bool value);
    [[nodiscard]] bool isPublic() const;
private:
    unsigned short specifierValue = 0;
    void setBit(unsigned short index);
    void clearBit(unsigned short index);
    [[nodiscard]] bool getBit(unsigned short index) const;
};
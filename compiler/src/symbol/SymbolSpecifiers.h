// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <symbol/SymbolType.h>

// Bit indices from right to left
const unsigned short BIT_INDEX_SIGNED = 0;
const unsigned short BIT_INDEX_CONST = 1;

// Defaults: Const: 0, Signed: 1
const unsigned short SPECIFIER_DEFAULTS_DOUBLE = 0b01;
// Defaults: Const: 0, Signed: 1
const unsigned short SPECIFIER_DEFAULTS_INT = 0b01;
// Defaults: Const: 0, Signed: 1
const unsigned short SPECIFIER_DEFAULTS_SHORT = 0b01;
// Defaults: Const: 0, Signed: 1
const unsigned short SPECIFIER_DEFAULTS_LONG = 0b01;
// Defaults: Const: 0, Signed: 0
const unsigned short SPECIFIER_DEFAULTS_BYTE = 0b00;
// Defaults: Const: 0, Signed: 0
const unsigned short SPECIFIER_DEFAULTS_CHAR = 0b00;
// Defaults: Const: 0, Signed: 0
const unsigned short SPECIFIER_DEFAULTS_STRING = 0b00;
// Defaults: Const: 0, Signed: 0
const unsigned short SPECIFIER_DEFAULTS_BOOL = 0b00;
// Defaults: Const: 0, Signed: 0
const unsigned short SPECIFIER_DEFAULTS_PTR = 0b00;
// Defaults: Const: 0, Signed: 0
const unsigned short SPECIFIER_DEFAULTS_ARRAY = 0b00;
// Defaults: Const: 1, Signed: 0
const unsigned short SPECIFIER_DEFAULTS_STRUCT = 0b10;
// Defaults: Const: 1, Signed: 0
const unsigned short SPECIFIER_DEFAULTS_FUNCTION = 0b10;
// Defaults: Const: 1, Signed: 0
const unsigned short SPECIFIER_DEFAULTS_PROCEDURE = 0b10;
// Defaults: Const: 1, Signed: 0
const unsigned short SPECIFIER_DEFAULTS_IMPORT = 0b10;

class SymbolSpecifiers {
public:
    explicit SymbolSpecifiers(SymbolType);
    void setConst(bool value);
    bool isConst() const;
    void setSigned(bool value);
    bool isSigned() const;
private:
    unsigned short specifierValue = 0;
    void setBit(unsigned short index);
    void clearBit(unsigned short index);
    bool getBit(unsigned short index) const;
};
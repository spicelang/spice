// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "symbol/SymbolSpecifiers.h"

SymbolSpecifiers::SymbolSpecifiers(SymbolType type) {
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
        case TY_BOOL:
            specifierValue = SPECIFIER_DEFAULTS_BOOL;
            break;
        case TY_PTR:
            specifierValue = SPECIFIER_DEFAULTS_PTR;
            break;
        case TY_ARRAY:
            specifierValue = SPECIFIER_DEFAULTS_ARRAY;
            break;
        case TY_STRUCT:
            specifierValue = SPECIFIER_DEFAULTS_STRUCT;
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
        case TY_DYN: break;
        default:
            throw std::runtime_error("Symbol specifier fallthrough");
    }
}

void SymbolSpecifiers::setConst(bool value) {
    if (value)
        setBit(BIT_INDEX_CONST);
    else
        clearBit(BIT_INDEX_CONST);
}

bool SymbolSpecifiers::isConst() const {
    return getBit(BIT_INDEX_CONST);
}

void SymbolSpecifiers::setSigned(bool value) {
    if (value)
        setBit(BIT_INDEX_SIGNED);
    else
        clearBit(BIT_INDEX_SIGNED);
}

bool SymbolSpecifiers::isSigned() const {
    return getBit(BIT_INDEX_SIGNED);
}

void SymbolSpecifiers::setBit(unsigned short index) {
    specifierValue |= (1 << index);
}

void SymbolSpecifiers::clearBit(unsigned short index) {
    specifierValue &= ~(1 << index);
}

bool SymbolSpecifiers::getBit(unsigned short index) const {
    return ((specifierValue >> index) & 1) == 1;
}

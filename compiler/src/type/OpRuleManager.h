// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <tuple>
#include "SymbolType.h"

// Types: double, int, short, long, byte, char, string, bool

// Unary operator rule tuple: lhs type, result type
typedef std::tuple<SymbolType, SymbolType> UnaryOpRule;
// Binary operator rule tuple: lhs type, rhs type, result type
typedef std::tuple<SymbolType, SymbolType, SymbolType> BinaryOpRule;

// Plus op rules
const std::vector<BinaryOpRule> plusOpRules = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE),    // double + double = double
        BinaryOpRule(TY_DOUBLE, TY_INT, TY_DOUBLE),       // double + int = double
        BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_DOUBLE),     // double + short = double
        BinaryOpRule(TY_DOUBLE, TY_LONG, TY_DOUBLE),      // double + long = double
        BinaryOpRule(TY_INT, TY_DOUBLE, TY_DOUBLE),       // int + double = double
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int + int = int
        BinaryOpRule(TY_INT, TY_SHORT, TY_INT),           // int + short = int
        BinaryOpRule(TY_INT, TY_LONG, TY_LONG),           // int + long = long
        BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_DOUBLE),     // short + double = double
        BinaryOpRule(TY_SHORT, TY_INT, TY_INT),           // short + int = int
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short + short = short
        BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG),         // short + long = long
        BinaryOpRule(TY_LONG, TY_DOUBLE, TY_DOUBLE),      // long + double = double
        BinaryOpRule(TY_LONG, TY_INT, TY_LONG),           // long + int = long
        BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG),         // long + short = long
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long + long = long
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),          // byte + byte = byte
        BinaryOpRule(TY_STRING, TY_STRING, TY_STRING),    // string + string = string
};

// Minus op rules
const std::vector<BinaryOpRule> minusOpRules = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE),    // double - double = double
        BinaryOpRule(TY_DOUBLE, TY_INT, TY_DOUBLE),       // double - int = double
        BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_DOUBLE),     // double - short = double
        BinaryOpRule(TY_DOUBLE, TY_LONG, TY_DOUBLE),      // double - long = double
        BinaryOpRule(TY_INT, TY_DOUBLE, TY_DOUBLE),       // int - double = double
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int - int = int
        BinaryOpRule(TY_INT, TY_SHORT, TY_INT),           // int - short = int
        BinaryOpRule(TY_INT, TY_LONG, TY_LONG),           // int - long = long
        BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_DOUBLE),     // short - double = double
        BinaryOpRule(TY_SHORT, TY_INT, TY_INT),           // short - int = int
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short - short = short
        BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG),         // short - long = long
        BinaryOpRule(TY_LONG, TY_DOUBLE, TY_DOUBLE),      // long - double = double
        BinaryOpRule(TY_LONG, TY_INT, TY_LONG),           // long - int = long
        BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG),         // long - short = long
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long - long = long
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),          // byte - byte = byte
};

class OpRuleManager {
public:
    // Public methods
    static SymbolType getPlusResultType(const SymbolType&, const SymbolType&);
    static SymbolType getMinusResultType(const SymbolType&, const SymbolType&);
private:
    static SymbolType validateOperation(const std::vector<BinaryOpRule>&, const std::string&, const SymbolType&, const SymbolType&);
    static SemanticError printErrorMessage(const std::string&, SymbolType, SymbolType);
};

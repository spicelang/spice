// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <tuple>
#include "SymbolType.h"

// Types: double, int, short, long, byte, char, string, bool

// Unary operator rule tuple: lhs type, result type
typedef std::tuple<SymbolType, SymbolType> UnaryOpRule;
// Binary operator rule tuple: lhs type, rhs type, result type
typedef std::tuple<SymbolType, SymbolType, SymbolType> BinaryOpRule;

// Assign op rules
const std::vector<BinaryOpRule> ASSIGN_OP_RULES = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE),    // double = double = double
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int = int = int
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short = short = short
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long = long = long
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),          // byte = byte = byte
        BinaryOpRule(TY_CHAR, TY_CHAR, TY_CHAR),          // char = char = char
        BinaryOpRule(TY_STRING, TY_STRING, TY_STRING),    // string = string = string
        BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL),          // bool = bool = bool
};

// Plus equal op rules
const std::vector<BinaryOpRule> PLUS_EQUAL_OP_RULES = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE),    // double += double = double
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int += int = int
        BinaryOpRule(TY_INT, TY_SHORT, TY_INT),           // int += short = int
        BinaryOpRule(TY_INT, TY_LONG, TY_LONG),           // int += long = long
        BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT),         // short += int = int
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short += short = short
        BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG),         // short += long = long
        BinaryOpRule(TY_LONG, TY_INT, TY_LONG),           // long += int = long
        BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG),         // long += short = long
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long += long = long
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),          // byte += byte = byte
        BinaryOpRule(TY_STRING, TY_STRING, TY_STRING),    // string += string = string
};

// Minus equal op rules
const std::vector<BinaryOpRule> MINUS_EQUAL_OP_RULES = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE),    // double -= double = double
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int -= int = int
        BinaryOpRule(TY_INT, TY_SHORT, TY_INT),           // int -= short = int
        BinaryOpRule(TY_INT, TY_LONG, TY_LONG),           // int -= long = long
        BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT),         // short -= int = int
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short -= short = short
        BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG),         // short -= long = long
        BinaryOpRule(TY_LONG, TY_INT, TY_LONG),           // long -= int = long
        BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG),         // long -= short = long
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long -= long = long
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),          // byte -= byte = byte
};

// Mul equal op rules
const std::vector<BinaryOpRule> MUL_EQUAL_OP_RULES = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE),    // double *= double = double
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int *= int = int
        BinaryOpRule(TY_INT, TY_SHORT, TY_INT),           // int *= short = int
        BinaryOpRule(TY_INT, TY_LONG, TY_LONG),           // int *= long = long
        BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT),         // short *= int = int
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short *= short = short
        BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG),         // short *= long = long
        BinaryOpRule(TY_LONG, TY_INT, TY_LONG),           // long *= int = long
        BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG),         // long *= short = long
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long *= long = long
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),          // byte *= byte = byte
};

// Div equal op rules
const std::vector<BinaryOpRule> DIV_EQUAL_OP_RULES = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE),    // double /= double = double
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int /= int = int
        BinaryOpRule(TY_INT, TY_SHORT, TY_INT),           // int /= short = int
        BinaryOpRule(TY_INT, TY_LONG, TY_LONG),           // int /= long = long
        BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT),         // short /= int = int
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short /= short = short
        BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG),         // short /= long = long
        BinaryOpRule(TY_LONG, TY_INT, TY_LONG),           // long /= int = long
        BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG),         // long /= short = long
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long /= long = long
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),          // byte /= byte = byte
};

// Shl equal op rules
const std::vector<BinaryOpRule> SHL_EQUAL_OP_RULES = {
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int <<= int = int
        BinaryOpRule(TY_INT, TY_SHORT, TY_INT),           // int <<= short = int
        BinaryOpRule(TY_INT, TY_LONG, TY_LONG),           // int <<= long = int
        BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT),         // short <<= int = short
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short <<= short = short
        BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG),         // short <<= long = short
        BinaryOpRule(TY_LONG, TY_INT, TY_LONG),           // long <<= int = long
        BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG),         // long <<= short = long
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long <<= long = long
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),          // byte <<= byte = byte
};

// Shr equal op rules
const std::vector<BinaryOpRule> SHR_EQUAL_OP_RULES = {
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int >>= int = int
        BinaryOpRule(TY_INT, TY_SHORT, TY_INT),           // int >>= short = int
        BinaryOpRule(TY_INT, TY_LONG, TY_LONG),           // int >>= long = int
        BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT),         // short >>= int = short
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short >>= short = short
        BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG),         // short >>= long = short
        BinaryOpRule(TY_LONG, TY_INT, TY_LONG),           // long >>= int = long
        BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG),         // long >>= short = long
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long >>= long = long
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),          // byte >>= byte = byte
};

// Logical and op rules
const std::vector<BinaryOpRule> LOGICAL_AND_OP_RULES = {
        BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL)           // bool && bool = bool
};

// Logical or op rules
const std::vector<BinaryOpRule> LOGICAL_OR_OP_RULES = {
        BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL)           // bool || bool = bool
};

// Bitwise and op rules
const std::vector<BinaryOpRule> BITWISE_AND_OP_RULES = {
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int & int = int
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short & short = short
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long & long = long
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),          // byte & byte = byte
};

// Bitwise or op rules
const std::vector<BinaryOpRule> BITWISE_OR_OP_RULES = {
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int | int = int
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short | short = short
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long | long = long
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),          // byte | byte = byte
};

// Equal op rules
const std::vector<BinaryOpRule> EQUAL_OP_RULES = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_BOOL),      // double == double = bool
        BinaryOpRule(TY_DOUBLE, TY_INT, TY_BOOL),         // double == int = bool
        BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_BOOL),       // double == short = bool
        BinaryOpRule(TY_DOUBLE, TY_LONG, TY_BOOL),        // double == long = bool
        BinaryOpRule(TY_INT, TY_DOUBLE, TY_BOOL),         // int == double = bool
        BinaryOpRule(TY_INT, TY_INT, TY_BOOL),            // int == int = bool
        BinaryOpRule(TY_INT, TY_SHORT, TY_BOOL),          // int == short = bool
        BinaryOpRule(TY_INT, TY_LONG, TY_BOOL),           // int == long = bool
        BinaryOpRule(TY_INT, TY_CHAR, TY_BOOL),           // int == char = bool
        BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_BOOL),       // short == double = bool
        BinaryOpRule(TY_SHORT, TY_INT, TY_BOOL),          // short == int = bool
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_BOOL),        // short == short = bool
        BinaryOpRule(TY_SHORT, TY_LONG, TY_BOOL),         // short == long = bool
        BinaryOpRule(TY_SHORT, TY_CHAR, TY_BOOL),         // short == char = bool
        BinaryOpRule(TY_LONG, TY_DOUBLE, TY_BOOL),        // long == double = bool
        BinaryOpRule(TY_LONG, TY_INT, TY_BOOL),           // long == int = bool
        BinaryOpRule(TY_LONG, TY_SHORT, TY_BOOL),         // long == short = bool
        BinaryOpRule(TY_LONG, TY_LONG, TY_BOOL),          // long == long = bool
        BinaryOpRule(TY_LONG, TY_CHAR, TY_BOOL),          // long == char = bool
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BOOL),          // byte == byte = bool
        BinaryOpRule(TY_CHAR, TY_INT, TY_BOOL),           // char == int = bool
        BinaryOpRule(TY_CHAR, TY_SHORT, TY_BOOL),         // char == short = bool
        BinaryOpRule(TY_CHAR, TY_LONG, TY_BOOL),          // char == long = bool
        BinaryOpRule(TY_CHAR, TY_CHAR, TY_BOOL),          // char == char = bool
        BinaryOpRule(TY_STRING, TY_STRING, TY_BOOL),      // string == string = bool
        BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL),          // bool == bool = bool
};

// Not equal op rules
const std::vector<BinaryOpRule> NOT_EQUAL_OP_RULES = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_BOOL),      // double != double = bool
        BinaryOpRule(TY_DOUBLE, TY_INT, TY_BOOL),         // double != int = bool
        BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_BOOL),       // double != short = bool
        BinaryOpRule(TY_DOUBLE, TY_LONG, TY_BOOL),        // double != long = bool
        BinaryOpRule(TY_INT, TY_DOUBLE, TY_BOOL),         // int != double = bool
        BinaryOpRule(TY_INT, TY_INT, TY_BOOL),            // int != int = bool
        BinaryOpRule(TY_INT, TY_SHORT, TY_BOOL),          // int != short = bool
        BinaryOpRule(TY_INT, TY_LONG, TY_BOOL),           // int != long = bool
        BinaryOpRule(TY_INT, TY_CHAR, TY_BOOL),           // int != char = bool
        BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_BOOL),       // short != double = bool
        BinaryOpRule(TY_SHORT, TY_INT, TY_BOOL),          // short != int = bool
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_BOOL),        // short != short = bool
        BinaryOpRule(TY_SHORT, TY_LONG, TY_BOOL),         // short != long = bool
        BinaryOpRule(TY_SHORT, TY_CHAR, TY_BOOL),         // short != char = bool
        BinaryOpRule(TY_LONG, TY_DOUBLE, TY_BOOL),        // long != double = bool
        BinaryOpRule(TY_LONG, TY_INT, TY_BOOL),           // long != int = bool
        BinaryOpRule(TY_LONG, TY_SHORT, TY_BOOL),         // long != short = bool
        BinaryOpRule(TY_LONG, TY_LONG, TY_BOOL),          // long != long = bool
        BinaryOpRule(TY_LONG, TY_CHAR, TY_BOOL),          // long != char = bool
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BOOL),          // byte != byte = bool
        BinaryOpRule(TY_CHAR, TY_INT, TY_BOOL),           // char != int = bool
        BinaryOpRule(TY_CHAR, TY_SHORT, TY_BOOL),         // char != short = bool
        BinaryOpRule(TY_CHAR, TY_LONG, TY_BOOL),          // char != long = bool
        BinaryOpRule(TY_CHAR, TY_CHAR, TY_BOOL),          // char != char = bool
        BinaryOpRule(TY_STRING, TY_STRING, TY_BOOL),      // string != string = bool
        BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL),          // bool != bool = bool
};

// Less op rules
const std::vector<BinaryOpRule> LESS_OP_RULES = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_BOOL),      // double < double = bool
        BinaryOpRule(TY_DOUBLE, TY_INT, TY_BOOL),         // double < int = bool
        BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_BOOL),       // double < short = bool
        BinaryOpRule(TY_DOUBLE, TY_LONG, TY_BOOL),        // double < long = bool
        BinaryOpRule(TY_INT, TY_DOUBLE, TY_BOOL),         // int < double = bool
        BinaryOpRule(TY_INT, TY_INT, TY_BOOL),            // int < int = bool
        BinaryOpRule(TY_INT, TY_SHORT, TY_BOOL),          // int < short = bool
        BinaryOpRule(TY_INT, TY_LONG, TY_BOOL),           // int < long = bool
        BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_BOOL),       // short < double = bool
        BinaryOpRule(TY_SHORT, TY_INT, TY_BOOL),          // short < int = bool
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_BOOL),        // short < short = bool
        BinaryOpRule(TY_SHORT, TY_LONG, TY_BOOL),         // short < long = bool
        BinaryOpRule(TY_LONG, TY_DOUBLE, TY_BOOL),        // long < double = bool
        BinaryOpRule(TY_LONG, TY_INT, TY_BOOL),           // long < int = bool
        BinaryOpRule(TY_LONG, TY_SHORT, TY_BOOL),         // long < short = bool
        BinaryOpRule(TY_LONG, TY_LONG, TY_BOOL),          // long < long = bool
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BOOL),          // byte < byte = bool
        BinaryOpRule(TY_CHAR, TY_CHAR, TY_BOOL),          // char < char = bool
};

// Greater op rules
const std::vector<BinaryOpRule> GREATER_OP_RULES = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_BOOL),      // double > double = bool
        BinaryOpRule(TY_DOUBLE, TY_INT, TY_BOOL),         // double > int = bool
        BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_BOOL),       // double > short = bool
        BinaryOpRule(TY_DOUBLE, TY_LONG, TY_BOOL),        // double > long = bool
        BinaryOpRule(TY_INT, TY_DOUBLE, TY_BOOL),         // int > double = bool
        BinaryOpRule(TY_INT, TY_INT, TY_BOOL),            // int > int = bool
        BinaryOpRule(TY_INT, TY_SHORT, TY_BOOL),          // int > short = bool
        BinaryOpRule(TY_INT, TY_LONG, TY_BOOL),           // int > long = bool
        BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_BOOL),       // short > double = bool
        BinaryOpRule(TY_SHORT, TY_INT, TY_BOOL),          // short > int = bool
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_BOOL),        // short > short = bool
        BinaryOpRule(TY_SHORT, TY_LONG, TY_BOOL),         // short > long = bool
        BinaryOpRule(TY_LONG, TY_DOUBLE, TY_BOOL),        // long > double = bool
        BinaryOpRule(TY_LONG, TY_INT, TY_BOOL),           // long > int = bool
        BinaryOpRule(TY_LONG, TY_SHORT, TY_BOOL),         // long > short = bool
        BinaryOpRule(TY_LONG, TY_LONG, TY_BOOL),          // long > long = bool
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BOOL),          // byte > byte = bool
        BinaryOpRule(TY_CHAR, TY_CHAR, TY_BOOL),          // char > char = bool
};

// Less equal op rules
const std::vector<BinaryOpRule> LESS_EQUAL_OP_RULES = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_BOOL),      // double <= double = bool
        BinaryOpRule(TY_DOUBLE, TY_INT, TY_BOOL),         // double <= int = bool
        BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_BOOL),       // double <= short = bool
        BinaryOpRule(TY_DOUBLE, TY_LONG, TY_BOOL),        // double <= long = bool
        BinaryOpRule(TY_INT, TY_DOUBLE, TY_BOOL),         // int <= double = bool
        BinaryOpRule(TY_INT, TY_INT, TY_BOOL),            // int <= int = bool
        BinaryOpRule(TY_INT, TY_SHORT, TY_BOOL),          // int <= short = bool
        BinaryOpRule(TY_INT, TY_LONG, TY_BOOL),           // int <= long = bool
        BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_BOOL),       // short <= double = bool
        BinaryOpRule(TY_SHORT, TY_INT, TY_BOOL),          // short <= int = bool
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_BOOL),        // short <= short = bool
        BinaryOpRule(TY_SHORT, TY_LONG, TY_BOOL),         // short <= long = bool
        BinaryOpRule(TY_LONG, TY_DOUBLE, TY_BOOL),        // long <= double = bool
        BinaryOpRule(TY_LONG, TY_INT, TY_BOOL),           // long <= int = bool
        BinaryOpRule(TY_LONG, TY_SHORT, TY_BOOL),         // long <= short = bool
        BinaryOpRule(TY_LONG, TY_LONG, TY_BOOL),          // long <= long = bool
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BOOL),          // byte <= byte = bool
        BinaryOpRule(TY_CHAR, TY_CHAR, TY_BOOL),          // char <= char = bool
};

// Greater equal op rules
const std::vector<BinaryOpRule> GREATER_EQUAL_OP_RULES = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_BOOL),      // double >= double = bool
        BinaryOpRule(TY_DOUBLE, TY_INT, TY_BOOL),         // double >= int = bool
        BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_BOOL),       // double >= short = bool
        BinaryOpRule(TY_DOUBLE, TY_LONG, TY_BOOL),        // double >= long = bool
        BinaryOpRule(TY_INT, TY_DOUBLE, TY_BOOL),         // int >= double = bool
        BinaryOpRule(TY_INT, TY_INT, TY_BOOL),            // int >= int = bool
        BinaryOpRule(TY_INT, TY_SHORT, TY_BOOL),          // int >= short = bool
        BinaryOpRule(TY_INT, TY_LONG, TY_BOOL),           // int >= long = bool
        BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_BOOL),       // short >= double = bool
        BinaryOpRule(TY_SHORT, TY_INT, TY_BOOL),          // short >= int = bool
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_BOOL),        // short >= short = bool
        BinaryOpRule(TY_SHORT, TY_LONG, TY_BOOL),         // short >= long = bool
        BinaryOpRule(TY_LONG, TY_DOUBLE, TY_BOOL),        // long >= double = bool
        BinaryOpRule(TY_LONG, TY_INT, TY_BOOL),           // long >= int = bool
        BinaryOpRule(TY_LONG, TY_SHORT, TY_BOOL),         // long >= short = bool
        BinaryOpRule(TY_LONG, TY_LONG, TY_BOOL),          // long >= long = bool
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BOOL),          // byte >= byte = bool
        BinaryOpRule(TY_CHAR, TY_CHAR, TY_BOOL),          // char >= char = bool
};

// Shift left op rules
const std::vector<BinaryOpRule> SHIFT_LEFT_OP_RULES = {
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int << int = int
        BinaryOpRule(TY_INT, TY_SHORT, TY_INT),           // int << short = int
        BinaryOpRule(TY_INT, TY_LONG, TY_INT),            // int << long = int
        BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT),         // short << int = short
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short << short = short
        BinaryOpRule(TY_SHORT, TY_LONG, TY_SHORT),        // short << long = short
        BinaryOpRule(TY_LONG, TY_INT, TY_LONG),           // long << int = long
        BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG),         // long << short = long
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long << long = long
        BinaryOpRule(TY_BYTE, TY_INT, TY_BYTE),           // byte << int = byte
        BinaryOpRule(TY_BYTE, TY_SHORT, TY_BYTE),         // byte << short = byte
        BinaryOpRule(TY_BYTE, TY_LONG, TY_BYTE),          // byte << long = byte
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),          // byte << byte = byte
};

// Shift right op rules
const std::vector<BinaryOpRule> SHIFT_RIGHT_OP_RULES = {
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int >> int = int
        BinaryOpRule(TY_INT, TY_SHORT, TY_INT),           // int >> short = int
        BinaryOpRule(TY_INT, TY_LONG, TY_INT),            // int >> long = int
        BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT),         // short >> int = short
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short >> short = short
        BinaryOpRule(TY_SHORT, TY_LONG, TY_SHORT),        // short >> long = short
        BinaryOpRule(TY_LONG, TY_INT, TY_LONG),           // long >> int = long
        BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG),         // long >> short = long
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long >> long = long
        BinaryOpRule(TY_BYTE, TY_INT, TY_BYTE),           // byte >> int = byte
        BinaryOpRule(TY_BYTE, TY_SHORT, TY_BYTE),         // byte >> short = byte
        BinaryOpRule(TY_BYTE, TY_LONG, TY_BYTE),          // byte >> long = byte
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),          // byte >> byte = byte
};

// Plus op rules
const std::vector<BinaryOpRule> PLUS_OP_RULES = {
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
const std::vector<BinaryOpRule> MINUS_OP_RULES = {
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

// Mul op rules
const std::vector<BinaryOpRule> MUL_OP_RULES = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE),    // double * double = double
        BinaryOpRule(TY_DOUBLE, TY_INT, TY_DOUBLE),       // double * int = double
        BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_DOUBLE),     // double * short = double
        BinaryOpRule(TY_DOUBLE, TY_LONG, TY_DOUBLE),      // double * long = double
        BinaryOpRule(TY_INT, TY_DOUBLE, TY_DOUBLE),       // int * double = double
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int * int = int
        BinaryOpRule(TY_INT, TY_SHORT, TY_INT),           // int * short = int
        BinaryOpRule(TY_INT, TY_LONG, TY_LONG),           // int * long = long
        BinaryOpRule(TY_INT, TY_CHAR, TY_STRING),         // int * char = string
        BinaryOpRule(TY_INT, TY_STRING, TY_STRING),       // int * string = string
        BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_DOUBLE),     // short * double = double
        BinaryOpRule(TY_SHORT, TY_INT, TY_INT),           // short * int = int
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short * short = short
        BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG),         // short * long = long
        BinaryOpRule(TY_SHORT, TY_CHAR, TY_STRING),       // short * char = string
        BinaryOpRule(TY_SHORT, TY_STRING, TY_STRING),     // short * string = string
        BinaryOpRule(TY_LONG, TY_DOUBLE, TY_DOUBLE),      // long * double = double
        BinaryOpRule(TY_LONG, TY_INT, TY_LONG),           // long * int = long
        BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG),         // long * short = long
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long * long = long
        BinaryOpRule(TY_LONG, TY_CHAR, TY_STRING),        // long * char = string
        BinaryOpRule(TY_LONG, TY_STRING, TY_STRING),      // long * string = string
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),          // byte * byte = byte
        BinaryOpRule(TY_CHAR, TY_INT, TY_STRING),         // char * int = string
        BinaryOpRule(TY_CHAR, TY_SHORT, TY_STRING),       // char * short = string
        BinaryOpRule(TY_CHAR, TY_LONG, TY_STRING),        // char * long = string
        BinaryOpRule(TY_STRING, TY_INT, TY_STRING),       // string * int = string
        BinaryOpRule(TY_STRING, TY_SHORT, TY_STRING),     // string * short = string
        BinaryOpRule(TY_STRING, TY_LONG, TY_STRING),      // string * long = string
};

// Div op rules
const std::vector<BinaryOpRule> DIV_OP_RULES = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE),    // double / double = double
        BinaryOpRule(TY_DOUBLE, TY_INT, TY_DOUBLE),       // double / int = double
        BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_DOUBLE),     // double / short = double
        BinaryOpRule(TY_DOUBLE, TY_LONG, TY_DOUBLE),      // double / long = double
        BinaryOpRule(TY_INT, TY_DOUBLE, TY_DOUBLE),       // int / double = double
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int / int = int
        BinaryOpRule(TY_INT, TY_SHORT, TY_INT),           // int / short = int
        BinaryOpRule(TY_INT, TY_LONG, TY_LONG),           // int / long = long
        BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_DOUBLE),     // short / double = double
        BinaryOpRule(TY_SHORT, TY_INT, TY_INT),           // short / int = int
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short / short = short
        BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG),         // short / long = long
        BinaryOpRule(TY_LONG, TY_DOUBLE, TY_DOUBLE),      // long / double = double
        BinaryOpRule(TY_LONG, TY_INT, TY_LONG),           // long / int = long
        BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG),         // long / short = long
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long / long = long
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),          // byte / byte = byte
};

// Rem op rules
const std::vector<BinaryOpRule> REM_OP_RULES = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE),    // double % double = double
        BinaryOpRule(TY_INT, TY_INT, TY_INT),             // int % int = int
        BinaryOpRule(TY_INT, TY_SHORT, TY_INT),           // int % short = int
        BinaryOpRule(TY_INT, TY_LONG, TY_INT),            // int % long = int
        BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT),         // short % int = short
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),       // short % short = short
        BinaryOpRule(TY_SHORT, TY_LONG, TY_SHORT),        // short % long = short
        BinaryOpRule(TY_LONG, TY_INT, TY_LONG),           // long % int = long
        BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG),         // long % short = long
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),          // long % long = long
};

// Prefix Plus-Plus op rules
const std::vector<UnaryOpRule> PREFIX_PLUS_PLUS_OP_RULES = {
        UnaryOpRule(TY_INT, TY_INT),                      // int++ = int
        UnaryOpRule(TY_SHORT, TY_SHORT),                  // short++ = short
        UnaryOpRule(TY_LONG, TY_LONG),                    // long++ = long
};

// Prefix Minus-Minus op rules
const std::vector<UnaryOpRule> PREFIX_MINUS_MINUS_OP_RULES = {
        UnaryOpRule(TY_INT, TY_INT),                      // int++ = int
        UnaryOpRule(TY_SHORT, TY_SHORT),                  // short++ = short
        UnaryOpRule(TY_LONG, TY_LONG),                    // long++ = long
};

// Postfix Plus-Plus op rules
const std::vector<UnaryOpRule> POSTFIX_PLUS_PLUS_OP_RULES = {
        UnaryOpRule(TY_INT, TY_INT),                      // int++ = int
        UnaryOpRule(TY_SHORT, TY_SHORT),                  // short++ = short
        UnaryOpRule(TY_LONG, TY_LONG),                    // long++ = long
};

// Postfix Minus-Minus op rules
const std::vector<UnaryOpRule> POSTFIX_MINUS_MINUS_OP_RULES = {
        UnaryOpRule(TY_INT, TY_INT),                      // int++ = int
        UnaryOpRule(TY_SHORT, TY_SHORT),                  // short++ = short
        UnaryOpRule(TY_LONG, TY_LONG),                    // long++ = long
};

// Not op rules
const std::vector<UnaryOpRule> NOT_OP_RULES = {
        UnaryOpRule(TY_BOOL, TY_BOOL),                    // !bool = bool
};

// Cast op rules
const std::vector<BinaryOpRule> CAST_OP_RULES = {
        BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE),           // (double) double = double
        BinaryOpRule(TY_INT, TY_INT, TY_INT),                    // (int) int = int
        BinaryOpRule(TY_INT, TY_SHORT, TY_INT),                  // (int) short = int
        BinaryOpRule(TY_INT, TY_LONG, TY_INT),                   // (int) long = int
        BinaryOpRule(TY_INT, TY_CHAR, TY_INT),                   // (int) char = int
        BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT),                // (short) int = short
        BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT),              // (short) short = short
        BinaryOpRule(TY_SHORT, TY_LONG, TY_SHORT),               // (short) long = short
        BinaryOpRule(TY_LONG, TY_INT, TY_LONG),                  // (long) int = long
        BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG),                // (long) short = long
        BinaryOpRule(TY_LONG, TY_LONG, TY_LONG),                 // (long) long = long
        BinaryOpRule(TY_BYTE, TY_INT, TY_BYTE),                  // (byte) int = byte
        BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE),                 // (byte) byte = byte
        BinaryOpRule(TY_CHAR, TY_INT, TY_CHAR),                  // (char) int = char
        BinaryOpRule(TY_CHAR, TY_SHORT, TY_CHAR),                // (char) short = char
        BinaryOpRule(TY_CHAR, TY_LONG, TY_CHAR),                 // (char) long = char
        BinaryOpRule(TY_CHAR, TY_CHAR, TY_CHAR),                 // (char) char = char
        BinaryOpRule(TY_STRING, TY_STRING, TY_STRING),           // (string) string = string
        BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL),                 // (bool) bool = bool
};

class OpRuleManager {
public:
    // Public methods
    static SymbolType getAssignResultType(const antlr4::Token&, SymbolType, const SymbolType&);
    static SymbolType getPlusEqualResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getMinusEqualResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getMulEqualResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getDivEqualResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getSHLEqualResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getSHREqualResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getLogicalAndResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getLogicalOrResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getBitwiseAndResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getBitwiseOrResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getEqualResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getNotEqualResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getLessResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getGreaterResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getLessEqualResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getGreaterEqualResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getShiftLeftResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getShiftRightResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getPlusResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getMinusResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getMulResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getDivResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getRemResultType(const antlr4::Token&, const SymbolType&, const SymbolType&);
    static SymbolType getPrefixPlusPlusResultType(const antlr4::Token&, const SymbolType&);
    static SymbolType getPrefixMinusMinusResultType(const antlr4::Token&, const SymbolType&);
    static SymbolType getPostfixPlusPlusResultType(const antlr4::Token&, const SymbolType&);
    static SymbolType getPostfixMinusMinusResultType(const antlr4::Token&, const SymbolType&);
    static SymbolType getNotResultType(const antlr4::Token&, const SymbolType&);
    static SymbolType getCastResultType(const antlr4::Token&, SymbolType, SymbolType);
private:
    static SymbolType validateBinaryOperation(const antlr4::Token& token, const std::vector<BinaryOpRule>& opRules,
                                              const std::string& opName, const SymbolType& lhs, const SymbolType& rhs);
    static SymbolType validateUnaryOperation(const antlr4::Token& token, const std::vector<UnaryOpRule>& opRules,
                                              const std::string& opName, const SymbolType& lhs);
    static SemanticError printErrorMessageBinary(const antlr4::Token& token, const std::string& operatorName,
                                                 SymbolType lhs, SymbolType rhs);
    static SemanticError printErrorMessageUnary(const antlr4::Token& token, const std::string& operatorName, SymbolType lhs);
};

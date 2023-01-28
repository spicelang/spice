// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <tuple>

#include <Token.h>

#include <exception/SemanticError.h>
#include <irgenerator/OpRuleConversionManager.h>
#include <symboltablebuilder/SymbolType.h>

namespace spice::compiler {

// Forward declarations
class ASTNode;
class TypeChecker;

// Helper macro to get the length of an array
#define arrayLength(array) sizeof(array) / sizeof(*array)

// Unary operator rule:        lhs type, result type, unsafe
using UnaryOpRule = std::tuple<uint32_t, uint32_t, bool>;
// Binary operator rule:        lhs type, rhs type, result type, unsafe
using BinaryOpRule = std::tuple<uint32_t, uint32_t, uint32_t, bool>;

// Assign op rules
const BinaryOpRule ASSIGN_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE, false), // double = double -> double
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),          // int = int -> int
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false),    // short = short -> short
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),       // long = long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),       // byte = byte -> byte
    BinaryOpRule(TY_CHAR, TY_CHAR, TY_CHAR, false),       // char = char -> char
    BinaryOpRule(TY_STRING, TY_STRING, TY_STRING, false), // string = string -> string
    BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL, false),       // bool = bool -> bool
};

// Plus equal op rules
const BinaryOpRule PLUS_EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE, false), // double += double -> double
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),          // int += int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),        // int += short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_INT, false),         // int += long -> int
    BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT, false),      // short += int -> short
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false),    // short += short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_SHORT, false),     // short += long -> short
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),        // long += int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),      // long += short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),       // long += long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),       // byte += byte -> byte
    BinaryOpRule(TY_CHAR, TY_CHAR, TY_CHAR, false),       // char += char -> char
};

// Minus equal op rules
const BinaryOpRule MINUS_EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE, false), // double -= double -> double
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),          // int -= int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),        // int -= short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_INT, false),         // int -= long -> int
    BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT, false),      // short -= int -> short
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false),    // short -= short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_SHORT, false),     // short -= long -> short
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),        // long -= int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),      // long -= short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),       // long -= long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),       // byte -= byte -> byte
    BinaryOpRule(TY_CHAR, TY_CHAR, TY_CHAR, false),       // char -= char -> char
};

// Mul equal op rules
const BinaryOpRule MUL_EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE, false), // double *= double -> double
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),          // int *= int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),        // int *= short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_INT, false),         // int *= long -> int
    BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT, false),      // short *= int -> short
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false),    // short *= short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_SHORT, false),     // short *= long -> short
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),        // long *= int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),      // long *= short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),       // long *= long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),       // byte *= byte -> byte
};

// Div equal op rules
const BinaryOpRule DIV_EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE, false), // double /= double -> double
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),          // int /= int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),        // int /= short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_INT, false),         // int /= long -> int
    BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT, false),      // short /= int -> short
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false),    // short /= short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_SHORT, false),     // short /= long -> short
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),        // long /= int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),      // long /= short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),       // long /= long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),       // byte /= byte -> byte
};

// Rem equal op rules
const BinaryOpRule REM_EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE, false), // double %= double -> double
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),          // int %= int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),        // int %= short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_INT, false),         // int %= long -> int
    BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT, false),      // short %= int -> short
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false),    // short %= short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_SHORT, false),     // short %= long -> short
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),        // long %= int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),      // long %= short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),       // long %= long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),       // byte %= byte -> byte
};

// Shl equal op rules
const BinaryOpRule SHL_EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),       // int <<= int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),     // int <<= short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_LONG, false),     // int <<= long -> int
    BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT, false),   // short <<= int -> short
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false), // short <<= short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG, false),   // short <<= long -> short
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),     // long <<= int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),   // long <<= short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),    // long <<= long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),    // byte <<= byte -> byte
};

// Shr equal op rules
const BinaryOpRule SHR_EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),       // int >>= int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),     // int >>= short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_LONG, false),     // int >>= long -> int
    BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT, false),   // short >>= int -> short
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false), // short >>= short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG, false),   // short >>= long -> short
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),     // long >>= int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),   // long >>= short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),    // long >>= long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),    // byte >>= byte -> byte
};

// And equal op rules
const BinaryOpRule AND_EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),       // int &= int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),     // int &= short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_LONG, false),     // int &= long -> int
    BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT, false),   // short &= int -> short
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false), // short &= short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG, false),   // short &= long -> short
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),     // long &= int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),   // long &= short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),    // long &= long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),    // byte &= byte -> byte
};

// Or equal op rules
const BinaryOpRule OR_EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),       // int |= int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),     // int |= short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_LONG, false),     // int |= long -> int
    BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT, false),   // short |= int -> short
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false), // short |= short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG, false),   // short |= long -> short
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),     // long |= int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),   // long |= short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),    // long |= long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),    // byte |= byte -> byte
};

// Xor equal op rules
const BinaryOpRule XOR_EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),       // int ^= int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),     // int ^= short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_LONG, false),     // int ^= long -> int
    BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT, false),   // short ^= int -> short
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false), // short ^= short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG, false),   // short ^= long -> short
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),     // long ^= int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),   // long ^= short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),    // long ^= long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),    // byte ^= byte -> byte
    BinaryOpRule(TY_CHAR, TY_CHAR, TY_CHAR, false),    // char ^= char -> char
};

// Logical and op rules
const BinaryOpRule LOGICAL_AND_OP_RULES[] = {
    BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL, false) // bool && bool -> bool
};

// Logical or op rules
const BinaryOpRule LOGICAL_OR_OP_RULES[] = {
    BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL, false) // bool || bool -> bool
};

// Bitwise and op rules
const BinaryOpRule BITWISE_AND_OP_RULES[] = {
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),       // int & int -> int
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false), // short & short -> short
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),    // long & long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),    // byte & byte -> byte
};

// Bitwise or op rules
const BinaryOpRule BITWISE_OR_OP_RULES[] = {
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),       // int | int -> int
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false), // short | short -> short
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),    // long | long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),    // byte | byte -> byte
};

// Bitwise xor op rules
const BinaryOpRule BITWISE_XOR_OP_RULES[] = {
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),       // int ^ int -> int
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false), // short ^ short -> short
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),    // long ^ long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),    // byte ^ byte -> byte
};

// Equal op rules
const BinaryOpRule EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_BOOL, false), // double == double -> bool
    BinaryOpRule(TY_DOUBLE, TY_INT, TY_BOOL, false),    // double == int -> bool
    BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_BOOL, false),  // double == short -> bool
    BinaryOpRule(TY_DOUBLE, TY_LONG, TY_BOOL, false),   // double == long -> bool
    BinaryOpRule(TY_INT, TY_DOUBLE, TY_BOOL, false),    // int == double -> bool
    BinaryOpRule(TY_INT, TY_INT, TY_BOOL, false),       // int == int -> bool
    BinaryOpRule(TY_INT, TY_SHORT, TY_BOOL, false),     // int == short -> bool
    BinaryOpRule(TY_INT, TY_LONG, TY_BOOL, false),      // int == long -> bool
    BinaryOpRule(TY_INT, TY_CHAR, TY_BOOL, false),      // int == char -> bool
    BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_BOOL, false),  // short == double -> bool
    BinaryOpRule(TY_SHORT, TY_INT, TY_BOOL, false),     // short == int -> bool
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_BOOL, false),   // short == short -> bool
    BinaryOpRule(TY_SHORT, TY_LONG, TY_BOOL, false),    // short == long -> bool
    BinaryOpRule(TY_SHORT, TY_CHAR, TY_BOOL, false),    // short == char -> bool
    BinaryOpRule(TY_LONG, TY_DOUBLE, TY_BOOL, false),   // long == double -> bool
    BinaryOpRule(TY_LONG, TY_INT, TY_BOOL, false),      // long == int -> bool
    BinaryOpRule(TY_LONG, TY_SHORT, TY_BOOL, false),    // long == short -> bool
    BinaryOpRule(TY_LONG, TY_LONG, TY_BOOL, false),     // long == long -> bool
    BinaryOpRule(TY_LONG, TY_CHAR, TY_BOOL, false),     // long == char -> bool
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BOOL, false),     // byte == byte -> bool
    BinaryOpRule(TY_CHAR, TY_INT, TY_BOOL, false),      // char == int -> bool
    BinaryOpRule(TY_CHAR, TY_SHORT, TY_BOOL, false),    // char == short -> bool
    BinaryOpRule(TY_CHAR, TY_LONG, TY_BOOL, false),     // char == long -> bool
    BinaryOpRule(TY_CHAR, TY_CHAR, TY_BOOL, false),     // char == char -> bool
    BinaryOpRule(TY_STRING, TY_STRING, TY_BOOL, false), // string == string -> bool
    BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL, false),     // bool == bool -> bool
};

// Not equal op rules
const BinaryOpRule NOT_EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_BOOL, false), // double != double -> bool
    BinaryOpRule(TY_DOUBLE, TY_INT, TY_BOOL, false),    // double != int -> bool
    BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_BOOL, false),  // double != short -> bool
    BinaryOpRule(TY_DOUBLE, TY_LONG, TY_BOOL, false),   // double != long -> bool
    BinaryOpRule(TY_INT, TY_DOUBLE, TY_BOOL, false),    // int != double -> bool
    BinaryOpRule(TY_INT, TY_INT, TY_BOOL, false),       // int != int -> bool
    BinaryOpRule(TY_INT, TY_SHORT, TY_BOOL, false),     // int != short -> bool
    BinaryOpRule(TY_INT, TY_LONG, TY_BOOL, false),      // int != long -> bool
    BinaryOpRule(TY_INT, TY_CHAR, TY_BOOL, false),      // int != char -> bool
    BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_BOOL, false),  // short != double -> bool
    BinaryOpRule(TY_SHORT, TY_INT, TY_BOOL, false),     // short != int -> bool
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_BOOL, false),   // short != short -> bool
    BinaryOpRule(TY_SHORT, TY_LONG, TY_BOOL, false),    // short != long -> bool
    BinaryOpRule(TY_SHORT, TY_CHAR, TY_BOOL, false),    // short != char -> bool
    BinaryOpRule(TY_LONG, TY_DOUBLE, TY_BOOL, false),   // long != double -> bool
    BinaryOpRule(TY_LONG, TY_INT, TY_BOOL, false),      // long != int -> bool
    BinaryOpRule(TY_LONG, TY_SHORT, TY_BOOL, false),    // long != short -> bool
    BinaryOpRule(TY_LONG, TY_LONG, TY_BOOL, false),     // long != long -> bool
    BinaryOpRule(TY_LONG, TY_CHAR, TY_BOOL, false),     // long != char -> bool
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BOOL, false),     // byte != byte -> bool
    BinaryOpRule(TY_CHAR, TY_INT, TY_BOOL, false),      // char != int -> bool
    BinaryOpRule(TY_CHAR, TY_SHORT, TY_BOOL, false),    // char != short -> bool
    BinaryOpRule(TY_CHAR, TY_LONG, TY_BOOL, false),     // char != long -> bool
    BinaryOpRule(TY_CHAR, TY_CHAR, TY_BOOL, false),     // char != char -> bool
    BinaryOpRule(TY_STRING, TY_STRING, TY_BOOL, false), // string != string -> bool
    BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL, false),     // bool != bool -> bool
};

// Less op rules
const BinaryOpRule LESS_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_BOOL, false), // double < double -> bool
    BinaryOpRule(TY_DOUBLE, TY_INT, TY_BOOL, false),    // double < int -> bool
    BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_BOOL, false),  // double < short -> bool
    BinaryOpRule(TY_DOUBLE, TY_LONG, TY_BOOL, false),   // double < long -> bool
    BinaryOpRule(TY_INT, TY_DOUBLE, TY_BOOL, false),    // int < double -> bool
    BinaryOpRule(TY_INT, TY_INT, TY_BOOL, false),       // int < int -> bool
    BinaryOpRule(TY_INT, TY_SHORT, TY_BOOL, false),     // int < short -> bool
    BinaryOpRule(TY_INT, TY_LONG, TY_BOOL, false),      // int < long -> bool
    BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_BOOL, false),  // short < double -> bool
    BinaryOpRule(TY_SHORT, TY_INT, TY_BOOL, false),     // short < int -> bool
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_BOOL, false),   // short < short -> bool
    BinaryOpRule(TY_SHORT, TY_LONG, TY_BOOL, false),    // short < long -> bool
    BinaryOpRule(TY_LONG, TY_DOUBLE, TY_BOOL, false),   // long < double -> bool
    BinaryOpRule(TY_LONG, TY_INT, TY_BOOL, false),      // long < int -> bool
    BinaryOpRule(TY_LONG, TY_SHORT, TY_BOOL, false),    // long < short -> bool
    BinaryOpRule(TY_LONG, TY_LONG, TY_BOOL, false),     // long < long -> bool
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BOOL, false),     // byte < byte -> bool
    BinaryOpRule(TY_CHAR, TY_CHAR, TY_BOOL, false),     // char < char -> bool
};

// Greater op rules
const BinaryOpRule GREATER_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_BOOL, false), // double > double -> bool
    BinaryOpRule(TY_DOUBLE, TY_INT, TY_BOOL, false),    // double > int -> bool
    BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_BOOL, false),  // double > short -> bool
    BinaryOpRule(TY_DOUBLE, TY_LONG, TY_BOOL, false),   // double > long -> bool
    BinaryOpRule(TY_INT, TY_DOUBLE, TY_BOOL, false),    // int > double -> bool
    BinaryOpRule(TY_INT, TY_INT, TY_BOOL, false),       // int > int -> bool
    BinaryOpRule(TY_INT, TY_SHORT, TY_BOOL, false),     // int > short -> bool
    BinaryOpRule(TY_INT, TY_LONG, TY_BOOL, false),      // int > long -> bool
    BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_BOOL, false),  // short > double -> bool
    BinaryOpRule(TY_SHORT, TY_INT, TY_BOOL, false),     // short > int -> bool
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_BOOL, false),   // short > short -> bool
    BinaryOpRule(TY_SHORT, TY_LONG, TY_BOOL, false),    // short > long -> bool
    BinaryOpRule(TY_LONG, TY_DOUBLE, TY_BOOL, false),   // long > double -> bool
    BinaryOpRule(TY_LONG, TY_INT, TY_BOOL, false),      // long > int -> bool
    BinaryOpRule(TY_LONG, TY_SHORT, TY_BOOL, false),    // long > short -> bool
    BinaryOpRule(TY_LONG, TY_LONG, TY_BOOL, false),     // long > long -> bool
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BOOL, false),     // byte > byte -> bool
    BinaryOpRule(TY_CHAR, TY_CHAR, TY_BOOL, false),     // char > char -> bool
};

// Less equal op rules
const BinaryOpRule LESS_EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_BOOL, false), // double <= double -> bool
    BinaryOpRule(TY_DOUBLE, TY_INT, TY_BOOL, false),    // double <= int -> bool
    BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_BOOL, false),  // double <= short -> bool
    BinaryOpRule(TY_DOUBLE, TY_LONG, TY_BOOL, false),   // double <= long -> bool
    BinaryOpRule(TY_INT, TY_DOUBLE, TY_BOOL, false),    // int <= double -> bool
    BinaryOpRule(TY_INT, TY_INT, TY_BOOL, false),       // int <= int -> bool
    BinaryOpRule(TY_INT, TY_SHORT, TY_BOOL, false),     // int <= short -> bool
    BinaryOpRule(TY_INT, TY_LONG, TY_BOOL, false),      // int <= long -> bool
    BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_BOOL, false),  // short <= double -> bool
    BinaryOpRule(TY_SHORT, TY_INT, TY_BOOL, false),     // short <= int -> bool
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_BOOL, false),   // short <= short -> bool
    BinaryOpRule(TY_SHORT, TY_LONG, TY_BOOL, false),    // short <= long -> bool
    BinaryOpRule(TY_LONG, TY_DOUBLE, TY_BOOL, false),   // long <= double -> bool
    BinaryOpRule(TY_LONG, TY_INT, TY_BOOL, false),      // long <= int -> bool
    BinaryOpRule(TY_LONG, TY_SHORT, TY_BOOL, false),    // long <= short -> bool
    BinaryOpRule(TY_LONG, TY_LONG, TY_BOOL, false),     // long <= long -> bool
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BOOL, false),     // byte <= byte -> bool
    BinaryOpRule(TY_CHAR, TY_CHAR, TY_BOOL, false),     // char <= char -> bool
};

// Greater equal op rules
const BinaryOpRule GREATER_EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_BOOL, false), // double >= double -> bool
    BinaryOpRule(TY_DOUBLE, TY_INT, TY_BOOL, false),    // double >= int -> bool
    BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_BOOL, false),  // double >= short -> bool
    BinaryOpRule(TY_DOUBLE, TY_LONG, TY_BOOL, false),   // double >= long -> bool
    BinaryOpRule(TY_INT, TY_DOUBLE, TY_BOOL, false),    // int >= double -> bool
    BinaryOpRule(TY_INT, TY_INT, TY_BOOL, false),       // int >= int -> bool
    BinaryOpRule(TY_INT, TY_SHORT, TY_BOOL, false),     // int >= short -> bool
    BinaryOpRule(TY_INT, TY_LONG, TY_BOOL, false),      // int >= long -> bool
    BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_BOOL, false),  // short >= double -> bool
    BinaryOpRule(TY_SHORT, TY_INT, TY_BOOL, false),     // short >= int -> bool
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_BOOL, false),   // short >= short -> bool
    BinaryOpRule(TY_SHORT, TY_LONG, TY_BOOL, false),    // short >= long -> bool
    BinaryOpRule(TY_LONG, TY_DOUBLE, TY_BOOL, false),   // long >= double -> bool
    BinaryOpRule(TY_LONG, TY_INT, TY_BOOL, false),      // long >= int -> bool
    BinaryOpRule(TY_LONG, TY_SHORT, TY_BOOL, false),    // long >= short -> bool
    BinaryOpRule(TY_LONG, TY_LONG, TY_BOOL, false),     // long >= long -> bool
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BOOL, false),     // byte >= byte -> bool
    BinaryOpRule(TY_CHAR, TY_CHAR, TY_BOOL, false),     // char >= char -> bool
};

// Shift left op rules
const BinaryOpRule SHIFT_LEFT_OP_RULES[] = {
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),       // int << int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),     // int << short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_INT, false),      // int << long -> int
    BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT, false),   // short << int -> short
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false), // short << short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_SHORT, false),  // short << long -> short
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),     // long << int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),   // long << short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),    // long << long -> long
    BinaryOpRule(TY_BYTE, TY_INT, TY_BYTE, false),     // byte << int -> byte
    BinaryOpRule(TY_BYTE, TY_SHORT, TY_BYTE, false),   // byte << short -> byte
    BinaryOpRule(TY_BYTE, TY_LONG, TY_BYTE, false),    // byte << long -> byte
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),    // byte << byte -> byte
};

// Shift right op rules
const BinaryOpRule SHIFT_RIGHT_OP_RULES[] = {
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),       // int >> int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),     // int >> short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_INT, false),      // int >> long -> int
    BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT, false),   // short >> int -> short
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false), // short >> short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_SHORT, false),  // short >> long -> short
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),     // long >> int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),   // long >> short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),    // long >> long -> long
    BinaryOpRule(TY_BYTE, TY_INT, TY_BYTE, false),     // byte >> int -> byte
    BinaryOpRule(TY_BYTE, TY_SHORT, TY_BYTE, false),   // byte >> short -> byte
    BinaryOpRule(TY_BYTE, TY_LONG, TY_BYTE, false),    // byte >> long -> byte
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),    // byte >> byte -> byte
};

// Plus op rules
const BinaryOpRule PLUS_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE, false), // double + double -> double
    BinaryOpRule(TY_DOUBLE, TY_INT, TY_DOUBLE, false),    // double + int -> double
    BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_DOUBLE, false),  // double + short -> double
    BinaryOpRule(TY_DOUBLE, TY_LONG, TY_DOUBLE, false),   // double + long -> double
    BinaryOpRule(TY_INT, TY_DOUBLE, TY_DOUBLE, false),    // int + double -> double
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),          // int + int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),        // int + short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_LONG, false),        // int + long -> long
    BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_DOUBLE, false),  // short + double -> double
    BinaryOpRule(TY_SHORT, TY_INT, TY_INT, false),        // short + int -> int
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false),    // short + short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG, false),      // short + long -> long
    BinaryOpRule(TY_LONG, TY_DOUBLE, TY_DOUBLE, false),   // long + double -> double
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),        // long + int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),      // long + short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),       // long + long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),       // byte + byte -> byte
};

// Minus op rules
const BinaryOpRule MINUS_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE, false), // double - double -> double
    BinaryOpRule(TY_DOUBLE, TY_INT, TY_DOUBLE, false),    // double - int -> double
    BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_DOUBLE, false),  // double - short -> double
    BinaryOpRule(TY_DOUBLE, TY_LONG, TY_DOUBLE, false),   // double - long -> double
    BinaryOpRule(TY_INT, TY_DOUBLE, TY_DOUBLE, false),    // int - double -> double
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),          // int - int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),        // int - short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_LONG, false),        // int - long -> long
    BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_DOUBLE, false),  // short - double -> double
    BinaryOpRule(TY_SHORT, TY_INT, TY_INT, false),        // short - int -> int
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false),    // short - short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG, false),      // short - long -> long
    BinaryOpRule(TY_LONG, TY_DOUBLE, TY_DOUBLE, false),   // long - double -> double
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),        // long - int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),      // long - short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),       // long - long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),       // byte - byte -> byte
};

// Mul op rules
const BinaryOpRule MUL_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE, false), // double * double -> double
    BinaryOpRule(TY_DOUBLE, TY_INT, TY_DOUBLE, false),    // double * int -> double
    BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_DOUBLE, false),  // double * short -> double
    BinaryOpRule(TY_DOUBLE, TY_LONG, TY_DOUBLE, false),   // double * long -> double
    BinaryOpRule(TY_INT, TY_DOUBLE, TY_DOUBLE, false),    // int * double -> double
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),          // int * int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),        // int * short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_LONG, false),        // int * long -> long
    BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_DOUBLE, false),  // short * double -> double
    BinaryOpRule(TY_SHORT, TY_INT, TY_INT, false),        // short * int -> int
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false),    // short * short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG, false),      // short * long -> long
    BinaryOpRule(TY_LONG, TY_DOUBLE, TY_DOUBLE, false),   // long * double -> double
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),        // long * int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),      // long * short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),       // long * long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),       // byte * byte -> byte
};

// Div op rules
const BinaryOpRule DIV_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE, false), // double / double -> double
    BinaryOpRule(TY_DOUBLE, TY_INT, TY_DOUBLE, false),    // double / int -> double
    BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_DOUBLE, false),  // double / short -> double
    BinaryOpRule(TY_DOUBLE, TY_LONG, TY_DOUBLE, false),   // double / long -> double
    BinaryOpRule(TY_INT, TY_DOUBLE, TY_DOUBLE, false),    // int / double -> double
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),          // int / int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),        // int / short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_LONG, false),        // int / long -> long
    BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_DOUBLE, false),  // short / double -> double
    BinaryOpRule(TY_SHORT, TY_INT, TY_INT, false),        // short / int -> int
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false),    // short / short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG, false),      // short / long -> long
    BinaryOpRule(TY_LONG, TY_DOUBLE, TY_DOUBLE, false),   // long / double -> double
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),        // long / int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),      // long / short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),       // long / long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),       // byte / byte -> byte
};

// Rem op rules
const BinaryOpRule REM_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE, false), // double % double -> double
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),          // int % int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),        // int % short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_INT, false),         // int % long -> int
    BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT, false),      // short % int -> short
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false),    // short % short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_SHORT, false),     // short % long -> short
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),        // long % int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),      // long % short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),       // long % long -> long
};

// Prefix Minus op rules
const UnaryOpRule PREFIX_MINUS_OP_RULES[] = {
    UnaryOpRule(TY_INT, TY_INT, false),       // -int -> int
    UnaryOpRule(TY_DOUBLE, TY_DOUBLE, false), // -double -> double
    UnaryOpRule(TY_SHORT, TY_SHORT, false),   // -short -> short
    UnaryOpRule(TY_LONG, TY_LONG, false),     // -long -> long
};

// Prefix Plus-Plus op rules
const UnaryOpRule PREFIX_PLUS_PLUS_OP_RULES[] = {
    UnaryOpRule(TY_INT, TY_INT, false),     // int++ -> int
    UnaryOpRule(TY_SHORT, TY_SHORT, false), // short++ -> short
    UnaryOpRule(TY_LONG, TY_LONG, false),   // long++ -> long
};

// Prefix Minus-Minus op rules
const UnaryOpRule PREFIX_MINUS_MINUS_OP_RULES[] = {
    UnaryOpRule(TY_INT, TY_INT, false),     // in-- -> int
    UnaryOpRule(TY_SHORT, TY_SHORT, false), // short-- -> short
    UnaryOpRule(TY_LONG, TY_LONG, false),   // long-- -> long
};

// Prefix not op rules
const UnaryOpRule PREFIX_NOT_OP_RULES[] = {
    UnaryOpRule(TY_BOOL, TY_BOOL, false), // !bool -> bool
};

// Prefix bitwise not op rules
const UnaryOpRule PREFIX_BITWISE_NOT_OP_RULES[] = {
    UnaryOpRule(TY_INT, TY_INT, false),     // ~int -> int
    UnaryOpRule(TY_SHORT, TY_SHORT, false), // ~short -> short
    UnaryOpRule(TY_LONG, TY_LONG, false),   // ~long -> long
    UnaryOpRule(TY_BYTE, TY_BYTE, false),   // ~byte -> byte
};

// Postfix Plus-Plus op rules
const UnaryOpRule POSTFIX_PLUS_PLUS_OP_RULES[] = {
    UnaryOpRule(TY_INT, TY_INT, false),     // int++ -> int
    UnaryOpRule(TY_SHORT, TY_SHORT, false), // short++ -> short
    UnaryOpRule(TY_LONG, TY_LONG, false),   // long++ -> long
};

// Postfix Minus-Minus op rules
const UnaryOpRule POSTFIX_MINUS_MINUS_OP_RULES[] = {
    UnaryOpRule(TY_INT, TY_INT, false),     // int++ -> int
    UnaryOpRule(TY_SHORT, TY_SHORT, false), // short++ -> short
    UnaryOpRule(TY_LONG, TY_LONG, false),   // long++ -> long
};

// Cast op rules
const BinaryOpRule CAST_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE, false), // (double) double -> double
    BinaryOpRule(TY_INT, TY_DOUBLE, TY_INT, false),       // (int) double -> int
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),          // (int) int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),        // (int) short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_INT, false),         // (int) long -> int
    BinaryOpRule(TY_INT, TY_BYTE, TY_INT, false),         // (int) byte -> int
    BinaryOpRule(TY_INT, TY_CHAR, TY_INT, false),         // (int) char -> int
    BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_SHORT, false),   // (short) double -> short
    BinaryOpRule(TY_SHORT, TY_INT, TY_SHORT, false),      // (short) int -> short
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false),    // (short) short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_SHORT, false),     // (short) long -> short
    BinaryOpRule(TY_LONG, TY_DOUBLE, TY_LONG, false),     // (long) double -> long
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),        // (long) int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),      // (long) short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),       // (long) long -> long
    BinaryOpRule(TY_BYTE, TY_INT, TY_BYTE, false),        // (byte) int -> byte
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),       // (byte) byte -> byte
    BinaryOpRule(TY_BYTE, TY_CHAR, TY_BYTE, false),       // (byte) char -> byte
    BinaryOpRule(TY_CHAR, TY_INT, TY_CHAR, false),        // (char) int -> char
    BinaryOpRule(TY_CHAR, TY_SHORT, TY_CHAR, false),      // (char) short -> char
    BinaryOpRule(TY_CHAR, TY_LONG, TY_CHAR, false),       // (char) long -> char
    BinaryOpRule(TY_CHAR, TY_BYTE, TY_CHAR, false),       // (char) byte -> char
    BinaryOpRule(TY_CHAR, TY_CHAR, TY_CHAR, false),       // (char) char -> char
    BinaryOpRule(TY_STRING, TY_STRING, TY_STRING, false), // (string) string -> string
    BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL, false),       // (bool) bool -> bool
};

/**
 * Helper class for the TypeChecker to check whether certain operator/type combinations are valid or not and which result type
 * is produced.
 */
class OpRuleManager {
public:
  // Constructors
  explicit OpRuleManager(TypeChecker *typeChecker) : typeChecker(typeChecker) {}

  // Public methods
  static SymbolType getAssignResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  SymbolType getPlusEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  SymbolType getMinusEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getMulEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getDivEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType hs);
  static SymbolType getRemEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getSHLEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getSHREqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getAndEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getOrEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getXorEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getLogicalAndResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getLogicalOrResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getBitwiseAndResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getBitwiseOrResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getBitwiseXorResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getNotEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getLessResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getGreaterResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getLessEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getGreaterEqualResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getShiftLeftResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getShiftRightResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  SymbolType getPlusResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  SymbolType getMinusResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getMulResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getDivResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getRemResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);
  static SymbolType getPrefixMinusResultType(const ASTNode *node, SymbolType lhs);
  static SymbolType getPrefixPlusPlusResultType(const ASTNode *node, SymbolType lhs);
  static SymbolType getPrefixMinusMinusResultType(const ASTNode *node, SymbolType lhs);
  static SymbolType getPrefixNotResultType(const ASTNode *node, SymbolType lhs);
  static SymbolType getPrefixBitwiseNotResultType(const ASTNode *node, SymbolType lhs);
  static SymbolType getPrefixMulResultType(const ASTNode *node, SymbolType lhs);
  static SymbolType getPrefixBitwiseAndResultType(const ASTNode *node, SymbolType lhs);
  static SymbolType getPostfixPlusPlusResultType(const ASTNode *node, SymbolType lhs);
  static SymbolType getPostfixMinusMinusResultType(const ASTNode *node, SymbolType lhs);
  SymbolType getCastResultType(const ASTNode *node, SymbolType lhs, SymbolType rhs);

private:
  // Members
  TypeChecker *typeChecker;

  // Private methods
  static SymbolType validateBinaryOperation(const ASTNode *node, const BinaryOpRule opRules[], size_t opRulesSize,
                                            const char *name, const SymbolType &lhs, const SymbolType &rhs);
  static SymbolType validateUnaryOperation(const ASTNode *node, const UnaryOpRule opRules[], size_t opRulesSize, const char *name,
                                           const SymbolType &lhs);
  void ensureUnsafeAllowed(const ASTNode *node, const char *name, const SymbolType &lhs, const SymbolType &rhs) const;
  static SemanticError printErrorMessageBinary(const ASTNode *node, const char *name, const SymbolType &lhs,
                                               const SymbolType &rhs);
  static SemanticError printErrorMessageUnary(const ASTNode *node, const char *name, const SymbolType &lhs);
};

} // namespace spice::compiler
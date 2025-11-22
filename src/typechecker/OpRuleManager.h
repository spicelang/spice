// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <tuple>

#include <exception/SemanticError.h>
#include <symboltablebuilder/Type.h>
#include <typechecker/ExprResult.h>

namespace spice::compiler {

// Forward declarations
class ASTNode;
class TypeChecker;
class GlobalResourceManager;
class Function;

// Custom error message prefixes
const char *const ERROR_MSG_RETURN = "Passed wrong data type to return statement";
const char *const ERROR_FOREACH_ITEM = "Passed wrong data type to foreach item";
const char *const ERROR_FIELD_ASSIGN = "Given type of the field value does not match declaration";

// Unary operator rule:        lhs type, result type, unsafe
using UnaryOpRule = std::tuple<SuperType, SuperType, bool>;
// Binary operator rule:        lhs type, rhs type, result type, unsafe
using BinaryOpRule = std::tuple<SuperType, SuperType, SuperType, bool>;

// Assign op rules
static constexpr BinaryOpRule ASSIGN_OP_RULES[] = {
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
static constexpr BinaryOpRule PLUS_EQUAL_OP_RULES[] = {
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
    BinaryOpRule(TY_CHAR, TY_INT, TY_CHAR, false),        // char += int -> char
};

// Minus equal op rules
static constexpr BinaryOpRule MINUS_EQUAL_OP_RULES[] = {
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
    BinaryOpRule(TY_CHAR, TY_INT, TY_CHAR, false),        // char -= int -> char
};

// Mul equal op rules
static constexpr BinaryOpRule MUL_EQUAL_OP_RULES[] = {
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
};

// Div equal op rules
static constexpr BinaryOpRule DIV_EQUAL_OP_RULES[] = {
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
};

// Rem equal op rules
static constexpr BinaryOpRule REM_EQUAL_OP_RULES[] = {
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
static constexpr BinaryOpRule SHL_EQUAL_OP_RULES[] = {
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
static constexpr BinaryOpRule SHR_EQUAL_OP_RULES[] = {
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
static constexpr BinaryOpRule AND_EQUAL_OP_RULES[] = {
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
static constexpr BinaryOpRule OR_EQUAL_OP_RULES[] = {
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
static constexpr BinaryOpRule XOR_EQUAL_OP_RULES[] = {
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
static constexpr BinaryOpRule LOGICAL_AND_OP_RULES[] = {
    BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL, false) // bool && bool -> bool
};

// Logical or op rules
static constexpr BinaryOpRule LOGICAL_OR_OP_RULES[] = {
    BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL, false) // bool || bool -> bool
};

// Bitwise or op rules
static constexpr BinaryOpRule BITWISE_OR_OP_RULES[] = {
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),       // int | int -> int
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false), // short | short -> short
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),    // long | long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),    // byte | byte -> byte
    BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL, false),    // bool | bool -> bool
};

// Bitwise xor op rules
static constexpr BinaryOpRule BITWISE_XOR_OP_RULES[] = {
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),       // int ^ int -> int
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false), // short ^ short -> short
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),    // long ^ long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),    // byte ^ byte -> byte
    BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL, false),    // bool ^ bool -> bool
};

// Bitwise and op rules
static constexpr BinaryOpRule BITWISE_AND_OP_RULES[] = {
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),       // int & int -> int
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false), // short & short -> short
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),    // long & long -> long
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BYTE, false),    // byte & byte -> byte
    BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL, false),    // bool & bool -> bool
};

// Equal op rules
static constexpr BinaryOpRule EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_BOOL, false),       // double == double -> bool
    BinaryOpRule(TY_DOUBLE, TY_INT, TY_BOOL, false),          // double == int -> bool
    BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_BOOL, false),        // double == short -> bool
    BinaryOpRule(TY_DOUBLE, TY_LONG, TY_BOOL, false),         // double == long -> bool
    BinaryOpRule(TY_INT, TY_DOUBLE, TY_BOOL, false),          // int == double -> bool
    BinaryOpRule(TY_INT, TY_INT, TY_BOOL, false),             // int == int -> bool
    BinaryOpRule(TY_INT, TY_SHORT, TY_BOOL, false),           // int == short -> bool
    BinaryOpRule(TY_INT, TY_LONG, TY_BOOL, false),            // int == long -> bool
    BinaryOpRule(TY_INT, TY_CHAR, TY_BOOL, false),            // int == char -> bool
    BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_BOOL, false),        // short == double -> bool
    BinaryOpRule(TY_SHORT, TY_INT, TY_BOOL, false),           // short == int -> bool
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_BOOL, false),         // short == short -> bool
    BinaryOpRule(TY_SHORT, TY_LONG, TY_BOOL, false),          // short == long -> bool
    BinaryOpRule(TY_SHORT, TY_CHAR, TY_BOOL, false),          // short == char -> bool
    BinaryOpRule(TY_LONG, TY_DOUBLE, TY_BOOL, false),         // long == double -> bool
    BinaryOpRule(TY_LONG, TY_INT, TY_BOOL, false),            // long == int -> bool
    BinaryOpRule(TY_LONG, TY_SHORT, TY_BOOL, false),          // long == short -> bool
    BinaryOpRule(TY_LONG, TY_LONG, TY_BOOL, false),           // long == long -> bool
    BinaryOpRule(TY_LONG, TY_CHAR, TY_BOOL, false),           // long == char -> bool
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BOOL, false),           // byte == byte -> bool
    BinaryOpRule(TY_CHAR, TY_INT, TY_BOOL, false),            // char == int -> bool
    BinaryOpRule(TY_CHAR, TY_SHORT, TY_BOOL, false),          // char == short -> bool
    BinaryOpRule(TY_CHAR, TY_LONG, TY_BOOL, false),           // char == long -> bool
    BinaryOpRule(TY_CHAR, TY_CHAR, TY_BOOL, false),           // char == char -> bool
    BinaryOpRule(TY_STRING, TY_STRING, TY_BOOL, false),       // string == string -> bool
    BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL, false),           // bool == bool -> bool
    BinaryOpRule(TY_FUNCTION, TY_FUNCTION, TY_BOOL, false),   // function == function -> bool
    BinaryOpRule(TY_PROCEDURE, TY_PROCEDURE, TY_BOOL, false), // procedure == procedure -> bool
};

// Not equal op rules
static constexpr BinaryOpRule NOT_EQUAL_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_BOOL, false),       // double != double -> bool
    BinaryOpRule(TY_DOUBLE, TY_INT, TY_BOOL, false),          // double != int -> bool
    BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_BOOL, false),        // double != short -> bool
    BinaryOpRule(TY_DOUBLE, TY_LONG, TY_BOOL, false),         // double != long -> bool
    BinaryOpRule(TY_INT, TY_DOUBLE, TY_BOOL, false),          // int != double -> bool
    BinaryOpRule(TY_INT, TY_INT, TY_BOOL, false),             // int != int -> bool
    BinaryOpRule(TY_INT, TY_SHORT, TY_BOOL, false),           // int != short -> bool
    BinaryOpRule(TY_INT, TY_LONG, TY_BOOL, false),            // int != long -> bool
    BinaryOpRule(TY_INT, TY_CHAR, TY_BOOL, false),            // int != char -> bool
    BinaryOpRule(TY_SHORT, TY_DOUBLE, TY_BOOL, false),        // short != double -> bool
    BinaryOpRule(TY_SHORT, TY_INT, TY_BOOL, false),           // short != int -> bool
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_BOOL, false),         // short != short -> bool
    BinaryOpRule(TY_SHORT, TY_LONG, TY_BOOL, false),          // short != long -> bool
    BinaryOpRule(TY_SHORT, TY_CHAR, TY_BOOL, false),          // short != char -> bool
    BinaryOpRule(TY_LONG, TY_DOUBLE, TY_BOOL, false),         // long != double -> bool
    BinaryOpRule(TY_LONG, TY_INT, TY_BOOL, false),            // long != int -> bool
    BinaryOpRule(TY_LONG, TY_SHORT, TY_BOOL, false),          // long != short -> bool
    BinaryOpRule(TY_LONG, TY_LONG, TY_BOOL, false),           // long != long -> bool
    BinaryOpRule(TY_LONG, TY_CHAR, TY_BOOL, false),           // long != char -> bool
    BinaryOpRule(TY_BYTE, TY_BYTE, TY_BOOL, false),           // byte != byte -> bool
    BinaryOpRule(TY_CHAR, TY_INT, TY_BOOL, false),            // char != int -> bool
    BinaryOpRule(TY_CHAR, TY_SHORT, TY_BOOL, false),          // char != short -> bool
    BinaryOpRule(TY_CHAR, TY_LONG, TY_BOOL, false),           // char != long -> bool
    BinaryOpRule(TY_CHAR, TY_CHAR, TY_BOOL, false),           // char != char -> bool
    BinaryOpRule(TY_STRING, TY_STRING, TY_BOOL, false),       // string != string -> bool
    BinaryOpRule(TY_BOOL, TY_BOOL, TY_BOOL, false),           // bool != bool -> bool
    BinaryOpRule(TY_FUNCTION, TY_FUNCTION, TY_BOOL, false),   // function != function -> bool
    BinaryOpRule(TY_PROCEDURE, TY_PROCEDURE, TY_BOOL, false), // procedure != procedure -> bool
};

// Less op rules
static constexpr BinaryOpRule LESS_OP_RULES[] = {
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
static constexpr BinaryOpRule GREATER_OP_RULES[] = {
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
static constexpr BinaryOpRule LESS_EQUAL_OP_RULES[] = {
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
static constexpr BinaryOpRule GREATER_EQUAL_OP_RULES[] = {
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
static constexpr BinaryOpRule SHIFT_LEFT_OP_RULES[] = {
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
static constexpr BinaryOpRule SHIFT_RIGHT_OP_RULES[] = {
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
static constexpr BinaryOpRule PLUS_OP_RULES[] = {
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
static constexpr BinaryOpRule MINUS_OP_RULES[] = {
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
static constexpr BinaryOpRule MUL_OP_RULES[] = {
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
static constexpr BinaryOpRule DIV_OP_RULES[] = {
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
static constexpr BinaryOpRule REM_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE, false), // double % double -> double
    BinaryOpRule(TY_INT, TY_INT, TY_INT, false),          // int % int -> int
    BinaryOpRule(TY_INT, TY_SHORT, TY_INT, false),        // int % short -> int
    BinaryOpRule(TY_INT, TY_LONG, TY_LONG, false),        // int % long -> long
    BinaryOpRule(TY_SHORT, TY_INT, TY_INT, false),        // short % int -> int
    BinaryOpRule(TY_SHORT, TY_SHORT, TY_SHORT, false),    // short % short -> short
    BinaryOpRule(TY_SHORT, TY_LONG, TY_LONG, false),      // short % long -> long
    BinaryOpRule(TY_LONG, TY_INT, TY_LONG, false),        // long % int -> long
    BinaryOpRule(TY_LONG, TY_SHORT, TY_LONG, false),      // long % short -> long
    BinaryOpRule(TY_LONG, TY_LONG, TY_LONG, false),       // long % long -> long
};

// Prefix Minus op rules
static constexpr UnaryOpRule PREFIX_MINUS_OP_RULES[] = {
    UnaryOpRule(TY_DOUBLE, TY_DOUBLE, false), // -double -> double
    UnaryOpRule(TY_INT, TY_INT, false),       // -int -> int
    UnaryOpRule(TY_SHORT, TY_SHORT, false),   // -short -> short
    UnaryOpRule(TY_LONG, TY_LONG, false),     // -long -> long
};

// Prefix Plus-Plus op rules
static constexpr UnaryOpRule PREFIX_PLUS_PLUS_OP_RULES[] = {
    UnaryOpRule(TY_INT, TY_INT, false),     // int++ -> int
    UnaryOpRule(TY_SHORT, TY_SHORT, false), // short++ -> short
    UnaryOpRule(TY_LONG, TY_LONG, false),   // long++ -> long
};

// Prefix Minus-Minus op rules
static constexpr UnaryOpRule PREFIX_MINUS_MINUS_OP_RULES[] = {
    UnaryOpRule(TY_INT, TY_INT, false),     // in-- -> int
    UnaryOpRule(TY_SHORT, TY_SHORT, false), // short-- -> short
    UnaryOpRule(TY_LONG, TY_LONG, false),   // long-- -> long
};

// Prefix not op rules
static constexpr UnaryOpRule PREFIX_NOT_OP_RULES[] = {
    UnaryOpRule(TY_BOOL, TY_BOOL, false), // !bool -> bool
};

// Prefix bitwise not op rules
static constexpr UnaryOpRule PREFIX_BITWISE_NOT_OP_RULES[] = {
    UnaryOpRule(TY_INT, TY_INT, false),     // ~int -> int
    UnaryOpRule(TY_SHORT, TY_SHORT, false), // ~short -> short
    UnaryOpRule(TY_LONG, TY_LONG, false),   // ~long -> long
    UnaryOpRule(TY_BYTE, TY_BYTE, false),   // ~byte -> byte
};

// Postfix Plus-Plus op rules
static constexpr UnaryOpRule POSTFIX_PLUS_PLUS_OP_RULES[] = {
    UnaryOpRule(TY_INT, TY_INT, false),     // int++ -> int
    UnaryOpRule(TY_SHORT, TY_SHORT, false), // short++ -> short
    UnaryOpRule(TY_LONG, TY_LONG, false),   // long++ -> long
};

// Postfix Minus-Minus op rules
static constexpr UnaryOpRule POSTFIX_MINUS_MINUS_OP_RULES[] = {
    UnaryOpRule(TY_INT, TY_INT, false),     // int++ -> int
    UnaryOpRule(TY_SHORT, TY_SHORT, false), // short++ -> short
    UnaryOpRule(TY_LONG, TY_LONG, false),   // long++ -> long
};

// Cast op rules
static constexpr BinaryOpRule CAST_OP_RULES[] = {
    BinaryOpRule(TY_DOUBLE, TY_DOUBLE, TY_DOUBLE, false), // (double) double -> double
    BinaryOpRule(TY_DOUBLE, TY_INT, TY_DOUBLE, false),    // (double) int -> double
    BinaryOpRule(TY_DOUBLE, TY_SHORT, TY_DOUBLE, false),  // (double) short -> double
    BinaryOpRule(TY_DOUBLE, TY_LONG, TY_DOUBLE, false),   // (double) long -> double
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
  explicit OpRuleManager(TypeChecker *typeChecker);

  // Public methods
  std::pair<QualType, Function *> getAssignResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs,
                                                      bool isDecl = false, bool isReturn = false,
                                                      const char *errMsgPrefix = "") const;
  QualType getFieldAssignResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, bool imm,
                                    bool isDecl = false) const;
  ExprResult getPlusEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx);
  ExprResult getMinusEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx);
  ExprResult getMulEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx);
  ExprResult getDivEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx);
  QualType getRemEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) const;
  QualType getSHLEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) const;
  QualType getSHREqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) const;
  QualType getAndEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) const;
  QualType getOrEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) const;
  QualType getXorEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs) const;
  static QualType getLogicalOrResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs);
  static QualType getLogicalAndResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs);
  static QualType getBitwiseOrResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs);
  static QualType getBitwiseXorResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs);
  static QualType getBitwiseAndResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs);
  ExprResult getEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx);
  ExprResult getNotEqualResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx);
  static QualType getLessResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs);
  static QualType getGreaterResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs);
  static QualType getLessEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs);
  static QualType getGreaterEqualResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs);
  ExprResult getShiftLeftResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx);
  ExprResult getShiftRightResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx);
  ExprResult getPlusResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx);
  ExprResult getMinusResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx);
  ExprResult getMulResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx);
  ExprResult getDivResultType(ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, size_t opIdx);
  static ExprResult getRemResultType(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs);
  static QualType getPrefixMinusResultType(const ASTNode *node, const ExprResult &lhs);
  QualType getPrefixPlusPlusResultType(const ASTNode *node, const ExprResult &lhs) const;
  QualType getPrefixMinusMinusResultType(const ASTNode *node, const ExprResult &lhs) const;
  static QualType getPrefixNotResultType(const ASTNode *node, const ExprResult &lhs);
  static QualType getPrefixBitwiseNotResultType(const ASTNode *node, const ExprResult &lhs);
  static QualType getPrefixMulResultType(const ASTNode *node, const ExprResult &lhs);
  static QualType getPrefixBitwiseAndResultType(const ASTNode *node, const ExprResult &lhs);
  ExprResult getPostfixPlusPlusResultType(ASTNode *node, const ExprResult &lhs, size_t opIdx);
  ExprResult getPostfixMinusMinusResultType(ASTNode *node, const ExprResult &lhs, size_t opIdx);
  QualType getCastResultType(const ASTNode *node, QualType lhsType, const ExprResult &rhs) const;

  // Operator overloading
  template <size_t N>
  ExprResult isOperatorOverloadingFctAvailable(ASTNode *node, const char *fctName, const std::array<ExprResult, N> &op,
                                               size_t opIdx);

private:
  // Members
  TypeChecker *typeChecker;
  GlobalResourceManager &resourceManager;

  // Private methods
  static QualType getAssignResultTypeCommon(const ASTNode *node, const ExprResult &lhs, const ExprResult &rhs, bool isDecl,
                                            bool isReturn);

  static QualType validateUnaryOperation(const ASTNode *node, const UnaryOpRule opRules[], size_t opRulesSize, const char *name,
                                         const QualType &lhs);
  static QualType validateBinaryOperation(const ASTNode *node, const BinaryOpRule opRules[], size_t opRulesSize, const char *name,
                                          const QualType &lhs, const QualType &rhs, bool preserveQualifiersFromLhs = false,
                                          const char *customMessagePrefix = "");
  static SemanticError getExceptionUnary(const ASTNode *node, const char *name, const QualType &lhs);
  static SemanticError getExceptionBinary(const ASTNode *node, const char *name, const QualType &lhs, const QualType &rhs,
                                          const char *messagePrefix);
  void ensureUnsafeAllowed(const ASTNode *node, const char *name, const QualType &lhs) const;
  void ensureUnsafeAllowed(const ASTNode *node, const char *name, const QualType &lhs, const QualType &rhs) const;
  void ensureNoConstAssign(const ASTNode *node, const QualType &lhs, bool isDecl = false, bool isReturn = false) const;
};

} // namespace spice::compiler
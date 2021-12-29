// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <tuple>
#include "SymbolType.h"

// Types: double, int, short, long, byte, char, string, bool

// Unary operator rule tuple: lhs type, result type
typedef std::tuple<SymbolType, SymbolType> UnaryOpRule;
// Binary operator rule tuple: lhs type, rhs type, result type
typedef std::tuple<SymbolType, SymbolType, SymbolType> BinaryOpRule;

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
        UnaryOpRule(TY_BOOL, TY_BOOL),                      // !bool = bool
};

class OpRuleManager {
public:
    // Public methods
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
private:
    static SymbolType validateBinaryOperation(const antlr4::Token& token, const std::vector<BinaryOpRule>& opRules,
                                              const std::string& opName, const SymbolType& lhs, const SymbolType& rhs);
    static SymbolType validateUnaryOperation(const antlr4::Token& token, const std::vector<UnaryOpRule>& opRules,
                                              const std::string& opName, const SymbolType& lhs);
    static SemanticError printErrorMessageBinary(const antlr4::Token& token, const std::string& operatorName,
                                                 SymbolType lhs, SymbolType rhs);
    static SemanticError printErrorMessageUnary(const antlr4::Token& token, const std::string& operatorName, SymbolType lhs);
};

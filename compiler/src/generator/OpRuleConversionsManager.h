// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include "exception/IRError.h"
#include "type/SymbolType.h"

enum PrimitiveType {
    P_TY_DOUBLE,
    P_TY_INT,
    P_TY_SHORT,
    P_TY_LONG,
    P_TY_BYTE_OR_CHAR,
    P_TY_STRING,
    P_TY_BOOL
};

#define COMB(en1, en2) ((en1) | ((en2) << 16))

class OpRuleConversionsManager {
public:
    explicit OpRuleConversionsManager(llvm::IRBuilder<>* builder) : builder(builder) {}

    // Public methods
    llvm::Value* getLessInst(llvm::Value*, llvm::Value*);
    llvm::Value* getGreaterInst(llvm::Value*, llvm::Value*);
    llvm::Value* getLessEqualInst(llvm::Value*, llvm::Value*);
    llvm::Value* getGreaterEqualInst(llvm::Value*, llvm::Value*);
    llvm::Value* getShiftLeftInst(llvm::Value*, llvm::Value*);
    llvm::Value* getShiftRightInst(llvm::Value*, llvm::Value*);
    llvm::Value* getPlusInst(llvm::Value*, llvm::Value*);
    llvm::Value* getMinusInst(llvm::Value*, llvm::Value*);
    llvm::Value* getMulInst(llvm::Value*, llvm::Value*);
    llvm::Value* getDivInst(llvm::Value*, llvm::Value*);
    llvm::Value* getRemInst(llvm::Value*, llvm::Value*);
    llvm::Value* getPrefixPlusPlusInst(llvm::Value*);
    llvm::Value* getPrefixMinusMinusInst(llvm::Value*);
    llvm::Value* getPostfixPlusPlusInst(llvm::Value*);
    llvm::Value* getPostfixMinusMinusInst(llvm::Value*);
    llvm::Value* getNotInst(llvm::Value*);
private:
    // Members
    llvm::IRBuilder<>* builder;

    // Private methods
    static PrimitiveType getPrimitiveTypeFromLLVMType(llvm::Type*);
    static bool isDouble(llvm::Type*);
    static bool isInt(llvm::Type*);
    static bool isShort(llvm::Type*);
    static bool isLong(llvm::Type*);
    static bool isByteOrChar(llvm::Type*);
    static bool isString(llvm::Type*);
    static bool isBool(llvm::Type*);
};
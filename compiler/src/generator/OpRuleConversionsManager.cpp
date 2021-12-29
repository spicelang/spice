// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "OpRuleConversionsManager.h"

llvm::Value* OpRuleConversionsManager::getPlusInst(llvm::Value* lhs, llvm::Type* lhsTy, llvm::Value* rhs, llvm::Type* rhsTy) {
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_DOUBLE, P_TY_DOUBLE):
            return builder->CreateFAdd(lhs, rhs);
        case COMB(P_TY_DOUBLE, P_TY_INT): {
            llvm::Value* rhsFP = builder->CreateSIToFP(rhs, lhsTy);
            return builder->CreateFAdd(lhs, rhsFP);
        }
        case COMB(P_TY_DOUBLE, P_TY_SHORT): {
            llvm::Value* rhsFP = builder->CreateSIToFP(rhs, lhsTy);
            return builder->CreateFAdd(lhs, rhsFP);
        }
        case COMB(P_TY_DOUBLE, P_TY_LONG): {
            llvm::Value* rhsFP = builder->CreateSIToFP(rhs, lhsTy);
            return builder->CreateFAdd(lhs, rhsFP);
        }
        case COMB(P_TY_INT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFAdd(lhsFP, rhs);
        }
        case COMB(P_TY_INT, P_TY_INT):
            return builder->CreateAdd(lhs, rhs);
        case COMB(P_TY_INT, P_TY_SHORT): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateAdd(lhs, rhsInt);
        }
        case COMB(P_TY_INT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateAdd(lhsLong, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFAdd(lhsFP, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_INT): {
            llvm::Value* lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateAdd(lhsInt, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_SHORT):
            return builder->CreateAdd(lhs, rhs);
        case COMB(P_TY_SHORT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateAdd(lhsLong, rhs);
        }
        case COMB(P_TY_LONG, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFAdd(lhsFP, rhs);
        }
        case COMB(P_TY_LONG, P_TY_INT): {
            llvm::Value* lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateAdd(lhsInt, rhs);
        }
        case COMB(P_TY_LONG, P_TY_SHORT): {
            llvm::Value* rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateAdd(lhs, rhsLong);
        }
        case COMB(P_TY_LONG, P_TY_LONG):
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateAdd(lhs, rhs);
        case COMB(P_TY_STRING, P_TY_STRING): {
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '+' operator for lhs=string and rhs=string yet");
        }
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: +");
}

llvm::Value* OpRuleConversionsManager::getMinusInst(llvm::Value* lhs, llvm::Type* lhsTy, llvm::Value* rhs, llvm::Type* rhsTy) {
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_DOUBLE, P_TY_DOUBLE):
            return builder->CreateFSub(lhs, rhs);
        case COMB(P_TY_DOUBLE, P_TY_INT): {
            llvm::Value* rhsFP = builder->CreateSIToFP(rhs, lhsTy);
            return builder->CreateFSub(lhs, rhsFP);
        }
        case COMB(P_TY_DOUBLE, P_TY_SHORT): {
            llvm::Value* rhsFP = builder->CreateSIToFP(rhs, lhsTy);
            return builder->CreateFSub(lhs, rhsFP);
        }
        case COMB(P_TY_DOUBLE, P_TY_LONG): {
            llvm::Value* rhsFP = builder->CreateSIToFP(rhs, lhsTy);
            return builder->CreateFSub(lhs, rhsFP);
        }
        case COMB(P_TY_INT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFSub(lhsFP, rhs);
        }
        case COMB(P_TY_INT, P_TY_INT):
            return builder->CreateSub(lhs, rhs);
        case COMB(P_TY_INT, P_TY_SHORT): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateSub(lhs, rhsInt);
        }
        case COMB(P_TY_INT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateSub(lhsLong, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFSub(lhsFP, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_INT): {
            llvm::Value* lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateSub(lhsInt, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_SHORT):
            return builder->CreateSub(lhs, rhs);
        case COMB(P_TY_SHORT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateSub(lhsLong, rhs);
        }
        case COMB(P_TY_LONG, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFSub(lhsFP, rhs);
        }
        case COMB(P_TY_LONG, P_TY_INT): {
            llvm::Value* lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateSub(lhsInt, rhs);
        }
        case COMB(P_TY_LONG, P_TY_SHORT): {
            llvm::Value* rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateSub(lhs, rhsLong);
        }
        case COMB(P_TY_LONG, P_TY_LONG):
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateSub(lhs, rhs);
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: -");
}

PrimitiveType OpRuleConversionsManager::getPrimitiveTypeFromLLVMType(llvm::Type* ty) {
    if (isDouble(ty)) return P_TY_DOUBLE;
    if (isInt(ty)) return P_TY_INT;
    if (isShort(ty)) return P_TY_SHORT;
    if (isLong(ty)) return P_TY_LONG;
    if (isByteOrChar(ty)) return P_TY_BYTE_OR_CHAR;
    if (isString(ty)) return P_TY_STRING;
    if (isBool(ty)) return P_TY_BOOL;
    throw std::runtime_error("Internal compiler error: Cannot find symbol type of llvm type");
}

bool OpRuleConversionsManager::isDouble(llvm::Type* ty) {
    return ty->isDoubleTy();
}

bool OpRuleConversionsManager::isInt(llvm::Type* ty) {
    return ty->isIntegerTy(32);
}

bool OpRuleConversionsManager::isShort(llvm::Type* ty) {
    return ty->isIntegerTy(16);
}

bool OpRuleConversionsManager::isLong(llvm::Type* ty) {
    return ty->isIntegerTy(64);
}

bool OpRuleConversionsManager::isByteOrChar(llvm::Type* ty) {
    return ty->isIntegerTy(8);
}

bool OpRuleConversionsManager::isString(llvm::Type* ty) {
    return ty->isPointerTy() && ty->getPointerElementType()->isIntegerTy(8);
}

bool OpRuleConversionsManager::isBool(llvm::Type* ty) {
    return ty->isIntegerTy(1);
}

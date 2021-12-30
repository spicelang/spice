// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "OpRuleConversionsManager.h"



llvm::Value* OpRuleConversionsManager::getBitwiseAndInst(llvm::Value* lhs, llvm::Value* rhs) {
    llvm::Type* lhsTy = lhs->getType();
    llvm::Type* rhsTy = rhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_INT, P_TY_INT): // fallthrough
        case COMB(P_TY_SHORT, P_TY_SHORT): // fallthrough
        case COMB(P_TY_LONG, P_TY_LONG): // fallthrough
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateAnd(lhs, rhs);
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: &");
}

llvm::Value* OpRuleConversionsManager::getBitwiseOrInst(llvm::Value* lhs, llvm::Value* rhs) {
    llvm::Type* lhsTy = lhs->getType();
    llvm::Type* rhsTy = rhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_INT, P_TY_INT): // fallthrough
        case COMB(P_TY_SHORT, P_TY_SHORT): // fallthrough
        case COMB(P_TY_LONG, P_TY_LONG): // fallthrough
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateOr(lhs, rhs);
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: |");
}

llvm::Value* OpRuleConversionsManager::getEqualInst(llvm::Value* lhs, llvm::Value* rhs) {
    llvm::Type* lhsTy = lhs->getType();
    llvm::Type* rhsTy = rhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_DOUBLE, P_TY_DOUBLE):
            return builder->CreateFCmpOEQ(lhs, rhs);
        case COMB(P_TY_DOUBLE, P_TY_INT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_SHORT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_LONG): {
            llvm::Value* rhsFP = builder->CreateSIToFP(rhs, lhsTy);
            return builder->CreateFCmpOEQ(lhs, rhsFP);
        }
        case COMB(P_TY_INT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpOEQ(lhsFP, rhs);
        }
        case COMB(P_TY_INT, P_TY_INT):
            return builder->CreateICmpEQ(lhs, rhs);
        case COMB(P_TY_INT, P_TY_SHORT): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpEQ(lhs, rhsInt);
        }
        case COMB(P_TY_INT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpEQ(lhsLong, rhs);
        }
        case COMB(P_TY_INT, P_TY_BYTE_OR_CHAR): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpEQ(lhs, rhsInt);
        }
        case COMB(P_TY_SHORT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpOEQ(lhsFP, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_INT): {
            llvm::Value* lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpEQ(lhsInt, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_SHORT):
            return builder->CreateICmpEQ(lhs, rhs);
        case COMB(P_TY_SHORT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpEQ(lhsLong, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_BYTE_OR_CHAR): {
            llvm::Value* rhsShort = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpEQ(lhs, rhsShort);
        }
        case COMB(P_TY_LONG, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpOEQ(lhsFP, rhs);
        }
        case COMB(P_TY_LONG, P_TY_INT): // fallthrough
        case COMB(P_TY_LONG, P_TY_SHORT): {
            llvm::Value* rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpEQ(lhs, rhsLong);
        }
        case COMB(P_TY_LONG, P_TY_LONG):
            return builder->CreateICmpEQ(lhs, rhs);
        case COMB(P_TY_LONG, P_TY_BYTE_OR_CHAR): {
            llvm::Value* rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpEQ(lhs, rhsLong);
        }
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_INT): {
            llvm::Value* lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpEQ(lhsInt, rhs);
        }
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_SHORT): {
            llvm::Value* lhsShort = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpEQ(lhsShort, rhs);
        }
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpEQ(lhsLong, rhs);
        }
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateICmpEQ(lhs, rhs);
        case COMB(P_TY_STRING, P_TY_STRING):
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '==' operator for lhs=string and rhs=string yet");
        case COMB(P_TY_BOOL, P_TY_BOOL):
            return builder->CreateICmpEQ(lhs, rhs);
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: ==");
}

llvm::Value* OpRuleConversionsManager::getNotEqualInst(llvm::Value* lhs, llvm::Value* rhs) {
    llvm::Type* lhsTy = lhs->getType();
    llvm::Type* rhsTy = rhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_DOUBLE, P_TY_DOUBLE):
            return builder->CreateFCmpONE(lhs, rhs);
        case COMB(P_TY_DOUBLE, P_TY_INT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_SHORT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_LONG): {
            llvm::Value* rhsFP = builder->CreateSIToFP(rhs, lhsTy);
            return builder->CreateFCmpONE(lhs, rhsFP);
        }
        case COMB(P_TY_INT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpONE(lhsFP, rhs);
        }
        case COMB(P_TY_INT, P_TY_INT):
            return builder->CreateICmpNE(lhs, rhs);
        case COMB(P_TY_INT, P_TY_SHORT): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpNE(lhs, rhsInt);
        }
        case COMB(P_TY_INT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpNE(lhsLong, rhs);
        }
        case COMB(P_TY_INT, P_TY_BYTE_OR_CHAR): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpNE(lhs, rhsInt);
        }
        case COMB(P_TY_SHORT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpONE(lhsFP, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_INT): {
            llvm::Value* lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpNE(lhsInt, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_SHORT):
            return builder->CreateICmpNE(lhs, rhs);
        case COMB(P_TY_SHORT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpNE(lhsLong, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_BYTE_OR_CHAR): {
            llvm::Value* rhsShort = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpNE(lhs, rhsShort);
        }
        case COMB(P_TY_LONG, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpONE(lhsFP, rhs);
        }
        case COMB(P_TY_LONG, P_TY_INT): // fallthrough
        case COMB(P_TY_LONG, P_TY_SHORT): {
            llvm::Value* rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpNE(lhs, rhsLong);
        }
        case COMB(P_TY_LONG, P_TY_LONG):
            return builder->CreateICmpNE(lhs, rhs);
        case COMB(P_TY_LONG, P_TY_BYTE_OR_CHAR): {
            llvm::Value* rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpNE(lhs, rhsLong);
        }
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_INT): {
            llvm::Value* lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpNE(lhsInt, rhs);
        }
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_SHORT): {
            llvm::Value* lhsShort = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpNE(lhsShort, rhs);
        }
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpNE(lhsLong, rhs);
        }
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateICmpNE(lhs, rhs);
        case COMB(P_TY_STRING, P_TY_STRING):
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '!=' operator for lhs=string and rhs=string yet");
        case COMB(P_TY_BOOL, P_TY_BOOL):
            return builder->CreateICmpNE(lhs, rhs);
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: !=");
}

llvm::Value* OpRuleConversionsManager::getLessInst(llvm::Value* lhs, llvm::Value* rhs) {
    llvm::Type* lhsTy = lhs->getType();
    llvm::Type* rhsTy = rhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_DOUBLE, P_TY_DOUBLE):
            return builder->CreateFCmpOLT(lhs, rhs);
        case COMB(P_TY_DOUBLE, P_TY_INT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_SHORT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_LONG): {
            llvm::Value* rhsFP = builder->CreateSIToFP(rhs, lhsTy);
            return builder->CreateFCmpOLT(lhs, rhsFP);
        }
        case COMB(P_TY_INT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpOLT(lhsFP, rhs);
        }
        case COMB(P_TY_INT, P_TY_INT):
            return builder->CreateICmpSLT(lhs, rhs);
        case COMB(P_TY_INT, P_TY_SHORT): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpSLT(lhs, rhsInt);
        }
        case COMB(P_TY_INT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpSLT(lhsLong, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpOLT(lhsFP, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_INT): {
            llvm::Value* lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpSLT(lhsInt, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_SHORT):
            return builder->CreateICmpSLT(lhs, rhs);
        case COMB(P_TY_SHORT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpSLT(lhsLong, rhs);
        }
        case COMB(P_TY_LONG, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpOLT(lhsFP, rhs);
        }
        case COMB(P_TY_LONG, P_TY_INT): // fallthrough
        case COMB(P_TY_LONG, P_TY_SHORT): {
            llvm::Value* rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpSLT(lhs, rhsLong);
        }
        case COMB(P_TY_LONG, P_TY_LONG): // fallthrough
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateICmpSLT(lhs, rhs);
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: <");
}

llvm::Value* OpRuleConversionsManager::getGreaterInst(llvm::Value* lhs, llvm::Value* rhs) {
    llvm::Type* lhsTy = lhs->getType();
    llvm::Type* rhsTy = rhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_DOUBLE, P_TY_DOUBLE):
            return builder->CreateFCmpOGT(lhs, rhs);
        case COMB(P_TY_DOUBLE, P_TY_INT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_SHORT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_LONG): {
            llvm::Value* rhsFP = builder->CreateSIToFP(rhs, lhsTy);
            return builder->CreateFCmpOGT(lhs, rhsFP);
        }
        case COMB(P_TY_INT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpOGT(lhsFP, rhs);
        }
        case COMB(P_TY_INT, P_TY_INT):
            return builder->CreateICmpSGT(lhs, rhs);
        case COMB(P_TY_INT, P_TY_SHORT): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpSGT(lhs, rhsInt);
        }
        case COMB(P_TY_INT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpSGT(lhsLong, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpOGT(lhsFP, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_INT): {
            llvm::Value* lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpSGT(lhsInt, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_SHORT):
            return builder->CreateICmpSGT(lhs, rhs);
        case COMB(P_TY_SHORT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpSGT(lhsLong, rhs);
        }
        case COMB(P_TY_LONG, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpOGT(lhsFP, rhs);
        }
        case COMB(P_TY_LONG, P_TY_INT): // fallthrough
        case COMB(P_TY_LONG, P_TY_SHORT): {
            llvm::Value* rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpSGT(lhs, rhsLong);
        }
        case COMB(P_TY_LONG, P_TY_LONG): // fallthrough
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateICmpSGT(lhs, rhs);
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: >");
}

llvm::Value* OpRuleConversionsManager::getLessEqualInst(llvm::Value* lhs, llvm::Value* rhs) {
    llvm::Type* lhsTy = lhs->getType();
    llvm::Type* rhsTy = rhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_DOUBLE, P_TY_DOUBLE):
            return builder->CreateFCmpOLE(lhs, rhs);
        case COMB(P_TY_DOUBLE, P_TY_INT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_SHORT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_LONG): {
            llvm::Value* rhsFP = builder->CreateSIToFP(rhs, lhsTy);
            return builder->CreateFCmpOLE(lhs, rhsFP);
        }
        case COMB(P_TY_INT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpOLE(lhsFP, rhs);
        }
        case COMB(P_TY_INT, P_TY_INT):
            return builder->CreateICmpSLE(lhs, rhs);
        case COMB(P_TY_INT, P_TY_SHORT): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpSLE(lhs, rhsInt);
        }
        case COMB(P_TY_INT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpSLE(lhsLong, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpOLE(lhsFP, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_INT): {
            llvm::Value* lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpSLE(lhsInt, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_SHORT):
            return builder->CreateICmpSLE(lhs, rhs);
        case COMB(P_TY_SHORT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpSLE(lhsLong, rhs);
        }
        case COMB(P_TY_LONG, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpOLE(lhsFP, rhs);
        }
        case COMB(P_TY_LONG, P_TY_INT): // fallthrough
        case COMB(P_TY_LONG, P_TY_SHORT): {
            llvm::Value* rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpSLE(lhs, rhsLong);
        }
        case COMB(P_TY_LONG, P_TY_LONG): // fallthrough
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateICmpSLE(lhs, rhs);
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: <=");
}

llvm::Value* OpRuleConversionsManager::getGreaterEqualInst(llvm::Value* lhs, llvm::Value* rhs) {
    llvm::Type* lhsTy = lhs->getType();
    llvm::Type* rhsTy = rhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_DOUBLE, P_TY_DOUBLE):
            return builder->CreateFCmpOGE(lhs, rhs);
        case COMB(P_TY_DOUBLE, P_TY_INT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_SHORT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_LONG): {
            llvm::Value* rhsFP = builder->CreateSIToFP(rhs, lhsTy);
            return builder->CreateFCmpOGE(lhs, rhsFP);
        }
        case COMB(P_TY_INT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpOGE(lhsFP, rhs);
        }
        case COMB(P_TY_INT, P_TY_INT):
            return builder->CreateICmpSGE(lhs, rhs);
        case COMB(P_TY_INT, P_TY_SHORT): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpSGE(lhs, rhsInt);
        }
        case COMB(P_TY_INT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpSGE(lhsLong, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpOGE(lhsFP, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_INT): {
            llvm::Value* lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpSGE(lhsInt, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_SHORT):
            return builder->CreateICmpSGE(lhs, rhs);
        case COMB(P_TY_SHORT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateICmpSGE(lhsLong, rhs);
        }
        case COMB(P_TY_LONG, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFCmpOGE(lhsFP, rhs);
        }
        case COMB(P_TY_LONG, P_TY_INT): // fallthrough
        case COMB(P_TY_LONG, P_TY_SHORT): {
            llvm::Value* rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateICmpSGE(lhs, rhsLong);
        }
        case COMB(P_TY_LONG, P_TY_LONG): // fallthrough
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateICmpSGE(lhs, rhs);
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: >=");
}

llvm::Value* OpRuleConversionsManager::getShiftLeftInst(llvm::Value* lhs, llvm::Value* rhs) {
    llvm::Type* lhsTy = lhs->getType();
    llvm::Type* rhsTy = rhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_INT, P_TY_INT):
            return builder->CreateShl(lhs, rhs);
        case COMB(P_TY_INT, P_TY_SHORT): // fallthrough
        case COMB(P_TY_INT, P_TY_LONG): // fallthrough
        case COMB(P_TY_SHORT, P_TY_INT): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateShl(lhs, rhsInt);
        }
        case COMB(P_TY_SHORT, P_TY_SHORT):
            return builder->CreateShl(lhs, rhs);
        case COMB(P_TY_SHORT, P_TY_LONG): // fallthrough
        case COMB(P_TY_LONG, P_TY_INT): // fallthrough
        case COMB(P_TY_LONG, P_TY_SHORT): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateShl(lhs, rhsInt);
        }
        case COMB(P_TY_LONG, P_TY_LONG):
            return builder->CreateShl(lhs, rhs);
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_INT): // fallthrough
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_SHORT): // fallthrough
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_LONG): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateShl(lhs, rhsInt);
        }
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateShl(lhs, rhs);
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: <<");
}

llvm::Value* OpRuleConversionsManager::getShiftRightInst(llvm::Value* lhs, llvm::Value* rhs) {
    llvm::Type* lhsTy = lhs->getType();
    llvm::Type* rhsTy = rhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_INT, P_TY_INT):
            return builder->CreateLShr(lhs, rhs);
        case COMB(P_TY_INT, P_TY_SHORT): // fallthrough
        case COMB(P_TY_INT, P_TY_LONG): // fallthrough
        case COMB(P_TY_SHORT, P_TY_INT): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateLShr(lhs, rhsInt);
        }
        case COMB(P_TY_SHORT, P_TY_SHORT):
            return builder->CreateLShr(lhs, rhs);
        case COMB(P_TY_SHORT, P_TY_LONG): // fallthrough
        case COMB(P_TY_LONG, P_TY_INT): // fallthrough
        case COMB(P_TY_LONG, P_TY_SHORT): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateLShr(lhs, rhsInt);
        }
        case COMB(P_TY_LONG, P_TY_LONG):
            return builder->CreateLShr(lhs, rhs);
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_INT): // fallthrough
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_SHORT): // fallthrough
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_LONG): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateLShr(lhs, rhsInt);
        }
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateLShr(lhs, rhs);
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: >>");
}

llvm::Value* OpRuleConversionsManager::getPlusInst(llvm::Value* lhs, llvm::Value* rhs) {
    llvm::Type* lhsTy = lhs->getType();
    llvm::Type* rhsTy = rhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_DOUBLE, P_TY_DOUBLE):
            return builder->CreateFAdd(lhs, rhs);
        case COMB(P_TY_DOUBLE, P_TY_INT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_SHORT): // fallthrough
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
        case COMB(P_TY_LONG, P_TY_LONG): // fallthrough
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateAdd(lhs, rhs);
        case COMB(P_TY_STRING, P_TY_STRING): {
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '+' operator for lhs=string and rhs=string yet");
        }
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: +");
}

llvm::Value* OpRuleConversionsManager::getMinusInst(llvm::Value* lhs, llvm::Value* rhs) {
    llvm::Type* lhsTy = lhs->getType();
    llvm::Type* rhsTy = rhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_DOUBLE, P_TY_DOUBLE):
            return builder->CreateFSub(lhs, rhs);
        case COMB(P_TY_DOUBLE, P_TY_INT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_SHORT): // fallthrough
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
        case COMB(P_TY_LONG, P_TY_LONG): // fallthrough
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateSub(lhs, rhs);
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: -");
}

llvm::Value* OpRuleConversionsManager::getMulInst(llvm::Value* lhs, llvm::Value* rhs) {
    llvm::Type* lhsTy = lhs->getType();
    llvm::Type* rhsTy = rhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_DOUBLE, P_TY_DOUBLE):
            return builder->CreateFMul(lhs, rhs);
        case COMB(P_TY_DOUBLE, P_TY_INT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_SHORT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_LONG): {
            llvm::Value* rhsFP = builder->CreateSIToFP(rhs, lhsTy);
            return builder->CreateFMul(lhs, rhsFP);
        }
        case COMB(P_TY_INT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFMul(lhsFP, rhs);
        }
        case COMB(P_TY_INT, P_TY_INT):
            return builder->CreateMul(lhs, rhs);
        case COMB(P_TY_INT, P_TY_SHORT): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateMul(lhs, rhsInt);
        }
        case COMB(P_TY_INT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateMul(lhsLong, rhs);
        }
        case COMB(P_TY_INT, P_TY_BYTE_OR_CHAR): {
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=int and rhs=char yet");
        }
        case COMB(P_TY_INT, P_TY_STRING): {
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=int and rhs=string yet");
        }
        case COMB(P_TY_SHORT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFMul(lhsFP, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_INT): {
            llvm::Value* lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateMul(lhsInt, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_SHORT):
            return builder->CreateMul(lhs, rhs);
        case COMB(P_TY_SHORT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateMul(lhsLong, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_BYTE_OR_CHAR): {
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=short and rhs=char yet");
        }
        case COMB(P_TY_SHORT, P_TY_STRING): {
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=short and rhs=string yet");
        }
        case COMB(P_TY_LONG, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFMul(lhsFP, rhs);
        }
        case COMB(P_TY_LONG, P_TY_INT): // fallthrough
        case COMB(P_TY_LONG, P_TY_SHORT): {
            llvm::Value* rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateMul(lhs, rhsLong);
        }
        case COMB(P_TY_LONG, P_TY_LONG):
            return builder->CreateMul(lhs, rhs);
        case COMB(P_TY_LONG, P_TY_BYTE_OR_CHAR): {
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=long and rhs=char yet");
        }
        case COMB(P_TY_LONG, P_TY_STRING): {
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=long and rhs=string yet");
        }
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateMul(lhs, rhs);
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_INT): {
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=char and rhs=int yet");
        }
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_SHORT): {
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=char and rhs=short yet");
        }
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_LONG): {
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=char and rhs=long yet");
        }
        case COMB(P_TY_STRING, P_TY_INT): {
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=string and rhs=int yet");
        }
        case COMB(P_TY_STRING, P_TY_SHORT): {
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=string and rhs=short yet");
        }
        case COMB(P_TY_STRING, P_TY_LONG): {
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=string and rhs=long yet");
        }
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: *");
}

llvm::Value* OpRuleConversionsManager::getDivInst(llvm::Value* lhs, llvm::Value* rhs) {
    llvm::Type* lhsTy = lhs->getType();
    llvm::Type* rhsTy = rhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_DOUBLE, P_TY_DOUBLE):
            return builder->CreateFDiv(lhs, rhs);
        case COMB(P_TY_DOUBLE, P_TY_INT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_SHORT): // fallthrough
        case COMB(P_TY_DOUBLE, P_TY_LONG): {
            llvm::Value* rhsFP = builder->CreateSIToFP(rhs, lhsTy);
            return builder->CreateFDiv(lhs, rhsFP);
        }
        case COMB(P_TY_INT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFDiv(lhsFP, rhs);
        }
        case COMB(P_TY_INT, P_TY_INT):
            return builder->CreateSDiv(lhs, rhs);
        case COMB(P_TY_INT, P_TY_SHORT): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateSDiv(lhs, rhsInt);
        }
        case COMB(P_TY_INT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateSDiv(lhsLong, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFDiv(lhsFP, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_INT): {
            llvm::Value* lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateSDiv(lhsInt, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_SHORT):
            return builder->CreateSDiv(lhs, rhs);
        case COMB(P_TY_SHORT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateSDiv(lhsLong, rhs);
        }
        case COMB(P_TY_LONG, P_TY_DOUBLE): {
            llvm::Value* lhsFP = builder->CreateSIToFP(lhs, rhsTy);
            return builder->CreateFDiv(lhsFP, rhs);
        }
        case COMB(P_TY_LONG, P_TY_INT): // fallthrough
        case COMB(P_TY_LONG, P_TY_SHORT): {
            llvm::Value* rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateSDiv(lhs, rhsLong);
        }
        case COMB(P_TY_LONG, P_TY_LONG): // fallthrough
        case COMB(P_TY_BYTE_OR_CHAR, P_TY_BYTE_OR_CHAR):
            return builder->CreateSDiv(lhs, rhs);
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: /");
}

llvm::Value* OpRuleConversionsManager::getRemInst(llvm::Value* lhs, llvm::Value* rhs) {
    llvm::Type* lhsTy = lhs->getType();
    llvm::Type* rhsTy = rhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    PrimitiveType rhsPTy = getPrimitiveTypeFromLLVMType(rhsTy);
    switch(COMB(lhsPTy, rhsPTy)) {
        case COMB(P_TY_DOUBLE, P_TY_DOUBLE):
            return builder->CreateFRem(lhs, rhs);
        case COMB(P_TY_INT, P_TY_INT):
            return builder->CreateSRem(lhs, rhs);
        case COMB(P_TY_INT, P_TY_SHORT): {
            llvm::Value* rhsInt = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateSRem(lhs, rhsInt);
        }
        case COMB(P_TY_INT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateSRem(lhsLong, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_INT): {
            llvm::Value* lhsInt = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateSRem(lhsInt, rhs);
        }
        case COMB(P_TY_SHORT, P_TY_SHORT):
            return builder->CreateSRem(lhs, rhs);
        case COMB(P_TY_SHORT, P_TY_LONG): {
            llvm::Value* lhsLong = builder->CreateIntCast(lhs, rhsTy, true);
            return builder->CreateSRem(lhsLong, rhs);
        }
        case COMB(P_TY_LONG, P_TY_INT): // fallthrough
        case COMB(P_TY_LONG, P_TY_SHORT): {
            llvm::Value* rhsLong = builder->CreateIntCast(rhs, lhsTy, true);
            return builder->CreateSRem(lhs, rhsLong);
        }
        case COMB(P_TY_LONG, P_TY_LONG):
            return builder->CreateSRem(lhs, rhs);
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: %");
}

llvm::Value* OpRuleConversionsManager::getPrefixPlusPlusInst(llvm::Value* lhs) {
    llvm::Type* lhsTy = lhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    switch(lhsPTy) {
        case P_TY_INT:
            return builder->CreateAdd(lhs, builder->getInt32(1));
        case P_TY_SHORT:
            return builder->CreateAdd(lhs, builder->getInt16(1));
        case P_TY_LONG:
            return builder->CreateAdd(lhs, builder->getInt64(1));
        default: break;
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: ++");
}

llvm::Value* OpRuleConversionsManager::getPrefixMinusMinusInst(llvm::Value* lhs) {
    llvm::Type* lhsTy = lhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    switch(lhsPTy) {
        case P_TY_INT:
            return builder->CreateSub(lhs, builder->getInt32(1));
        case P_TY_SHORT:
            return builder->CreateSub(lhs, builder->getInt16(1));
        case P_TY_LONG:
            return builder->CreateSub(lhs, builder->getInt64(1));
        default: break;
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: --");
}

llvm::Value* OpRuleConversionsManager::getPostfixPlusPlusInst(llvm::Value* lhs) {
    llvm::Type* lhsTy = lhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    switch(lhsPTy) {
        case P_TY_INT:
            return builder->CreateAdd(lhs, builder->getInt32(1));
        case P_TY_SHORT:
            return builder->CreateAdd(lhs, builder->getInt16(1));
        case P_TY_LONG:
            return builder->CreateAdd(lhs, builder->getInt64(1));
        default: break;
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: ++");
}

llvm::Value* OpRuleConversionsManager::getPostfixMinusMinusInst(llvm::Value* lhs) {
    llvm::Type* lhsTy = lhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    switch(lhsPTy) {
        case P_TY_INT:
            return builder->CreateSub(lhs, builder->getInt32(1));
        case P_TY_SHORT:
            return builder->CreateSub(lhs, builder->getInt16(1));
        case P_TY_LONG:
            return builder->CreateSub(lhs, builder->getInt64(1));
        default: break;
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: --");
}

llvm::Value* OpRuleConversionsManager::getNotInst(llvm::Value* lhs) {
    llvm::Type* lhsTy = lhs->getType();
    PrimitiveType lhsPTy = getPrimitiveTypeFromLLVMType(lhsTy);
    switch(lhsPTy) {
        case P_TY_BOOL:
            return builder->CreateNot(lhs);
        default: break;
    }
    throw std::runtime_error("Internal compiler error: Operator fallthrough: --");
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

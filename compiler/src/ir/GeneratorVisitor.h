// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <SpiceBaseVisitor.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

class GeneratorVisitor : public SpiceBaseVisitor {
public:
    // Constructors
    explicit GeneratorVisitor(): builder(context) {};
    // Public methods
    antlrcpp::Any visitEntry(SpiceParser::EntryContext *ctx) override;
    antlrcpp::Any visitFunctionDef(SpiceParser::FunctionDefContext *ctx) override;
    antlrcpp::Any visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) override;
    antlrcpp::Any visitForLoop(SpiceParser::ForLoopContext *ctx) override;
    /*antlrcpp::Any visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) override;*/
    antlrcpp::Any visitWhileLoop(SpiceParser::WhileLoopContext *ctx) override;
    antlrcpp::Any visitIfStmt(SpiceParser::IfStmtContext *ctx) override;
    antlrcpp::Any visitDeclStmt(SpiceParser::DeclStmtContext *ctx) override;
    antlrcpp::Any visitFunctionCall(SpiceParser::FunctionCallContext *ctx) override;
    antlrcpp::Any visitImportStmt(SpiceParser::ImportStmtContext *ctx) override;
    antlrcpp::Any visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) override;
    antlrcpp::Any visitAssignment(SpiceParser::AssignmentContext *ctx) override;
    antlrcpp::Any visitTernary(SpiceParser::TernaryContext *ctx) override;
    antlrcpp::Any visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) override;
    antlrcpp::Any visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) override;
    antlrcpp::Any visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) override;
    antlrcpp::Any visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) override;
    antlrcpp::Any visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) override;
    antlrcpp::Any visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) override;
    antlrcpp::Any visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) override;
    antlrcpp::Any visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) override;
    antlrcpp::Any visitPrefixUnary(SpiceParser::PrefixUnaryContext *ctx) override;
    antlrcpp::Any visitPostfixUnary(SpiceParser::PostfixUnaryContext *ctx) override;
    antlrcpp::Any visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) override;
    antlrcpp::Any visitValue(SpiceParser::ValueContext *ctx) override;
private:
    // Members
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    std::map<std::string, llvm::Value*> namedValues;
    // Private methods

};
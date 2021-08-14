// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <SpiceBaseVisitor.h>
#include <exception/IRError.h>
#include <analyzer/SymbolTable.h>
#include <util/ScopeIdUtil.h>
#include <analyzer/AnalyzerVisitor.h>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Utils.h>

#include <utility>

class GeneratorVisitor : public SpiceBaseVisitor {
public:
    // Constructors
    explicit GeneratorVisitor(SymbolTable* symbolTable, std::string sourceFile, std::string targetTriple,
                              std::string outputPath, bool debugOutput, int optLevel): currentScope(symbolTable),
                              sourceFile(std::move(sourceFile)), targetTriple(std::move(targetTriple)),
                              outputPath(std::move(outputPath)), debugOutput(debugOutput), optLevel(optLevel) {}

    // Public methods
    void init();
    void optimize();
    void emit();
    void dumpIR();
    std::string getIRString();
    antlrcpp::Any visitEntry(SpiceParser::EntryContext* ctx) override;
    antlrcpp::Any visitMainFunctionDef(SpiceParser::MainFunctionDefContext* ctx) override;
    antlrcpp::Any visitFunctionDef(SpiceParser::FunctionDefContext* ctx) override;
    antlrcpp::Any visitProcedureDef(SpiceParser::ProcedureDefContext* ctx) override;
    antlrcpp::Any visitForLoop(SpiceParser::ForLoopContext* ctx) override;
    antlrcpp::Any visitWhileLoop(SpiceParser::WhileLoopContext* ctx) override;
    antlrcpp::Any visitStmtLst(SpiceParser::StmtLstContext* ctx) override;
    antlrcpp::Any visitIfStmt(SpiceParser::IfStmtContext* ctx) override;
    antlrcpp::Any visitDeclStmt(SpiceParser::DeclStmtContext* ctx) override;
    antlrcpp::Any visitFunctionCall(SpiceParser::FunctionCallContext* ctx) override;
    antlrcpp::Any visitImportStmt(SpiceParser::ImportStmtContext* ctx) override;
    antlrcpp::Any visitReturnStmt(SpiceParser::ReturnStmtContext* ctx) override;
    antlrcpp::Any visitBreakStmt(SpiceParser::BreakStmtContext *ctx) override;
    antlrcpp::Any visitContinueStmt(SpiceParser::ContinueStmtContext* ctx) override;
    antlrcpp::Any visitPrintfStmt(SpiceParser::PrintfStmtContext* ctx) override;
    antlrcpp::Any visitAssignment(SpiceParser::AssignmentContext* ctx) override;
    antlrcpp::Any visitTernary(SpiceParser::TernaryContext* ctx) override;
    antlrcpp::Any visitLogicalOrExpr(SpiceParser::LogicalOrExprContext* ctx) override;
    antlrcpp::Any visitLogicalAndExpr(SpiceParser::LogicalAndExprContext* ctx) override;
    antlrcpp::Any visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext* ctx) override;
    antlrcpp::Any visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext* ctx) override;
    antlrcpp::Any visitEqualityExpr(SpiceParser::EqualityExprContext* ctx) override;
    antlrcpp::Any visitRelationalExpr(SpiceParser::RelationalExprContext* ctx) override;
    antlrcpp::Any visitAdditiveExpr(SpiceParser::AdditiveExprContext* ctx) override;
    antlrcpp::Any visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext* ctx) override;
    antlrcpp::Any visitPrefixUnary(SpiceParser::PrefixUnaryContext* ctx) override;
    antlrcpp::Any visitPostfixUnary(SpiceParser::PostfixUnaryContext* ctx) override;
    antlrcpp::Any visitAtomicExpr(SpiceParser::AtomicExprContext* ctx) override;
    antlrcpp::Any visitValue(SpiceParser::ValueContext* ctx) override;
    antlrcpp::Any visitDataType(SpiceParser::DataTypeContext* ctx) override;
private:
    // Members
    std::string sourceFile;
    std::string targetTriple;
    std::string outputPath;
    bool debugOutput;
    int optLevel;
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    std::unique_ptr<llvm::Module> module;
    std::map<std::string, llvm::AllocaInst*> namedValues;
    std::vector<llvm::Function*> functions;
    SymbolTable* currentScope = new SymbolTable(nullptr);
    std::string currentVar;
    SymbolType currentSymbolType;
    llvm::Value* currentAssignValue = nullptr;
    bool blockAlreadyTerminated = false;
    //llvm::StructType* stringType = nullptr;

    // Private methods
    void initializeExternalFunctions();
    //void initStringType();
    void moveInsertPointToBlock(llvm::BasicBlock*);
    void createBr(llvm::BasicBlock*);
    void createCondBr(llvm::Value*, llvm::BasicBlock*, llvm::BasicBlock*);
};
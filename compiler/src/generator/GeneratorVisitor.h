// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <SpiceBaseVisitor.h>
#include <exception/IRError.h>
#include <analyzer/SymbolTable.h>
#include <util/ScopeIdUtil.h>
#include <util/IdentifierUtil.h>
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
    explicit GeneratorVisitor(SymbolTable* symbolTable, std::string sourceFile, const std::string& targetArch,
                              const std::string& targetVendor, const std::string& targetOs, std::string outputPath,
                              bool debugOutput, int optLevel, bool mustHaveMainFunction) :
            currentScope(symbolTable), mainSourceFile(std::move(sourceFile)), objectDir(std::move(outputPath)),
            debugOutput(debugOutput), optLevel(optLevel), mustHaveMainFunction(mustHaveMainFunction) {
        if (targetArch.empty()) {
            targetTriple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
        } else {
            targetTriple = llvm::Triple(targetArch, targetVendor, targetOs);
        }
    }

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

    antlrcpp::Any visitStructDef(SpiceParser::StructDefContext* ctx) override;

    antlrcpp::Any visitGlobalVarDef(SpiceParser::GlobalVarDefContext* ctx) override;

    antlrcpp::Any visitForLoop(SpiceParser::ForLoopContext* ctx) override;

    antlrcpp::Any visitForeachLoop(SpiceParser::ForeachLoopContext* ctx) override;

    antlrcpp::Any visitWhileLoop(SpiceParser::WhileLoopContext* ctx) override;

    antlrcpp::Any visitStmtLst(SpiceParser::StmtLstContext* ctx) override;

    antlrcpp::Any visitIfStmt(SpiceParser::IfStmtContext* ctx) override;

    antlrcpp::Any visitElseStmt(SpiceParser::ElseStmtContext* ctx) override;

    antlrcpp::Any visitDeclStmt(SpiceParser::DeclStmtContext* ctx) override;

    antlrcpp::Any visitFunctionCall(SpiceParser::FunctionCallContext* ctx) override;

    antlrcpp::Any visitNewStmt(SpiceParser::NewStmtContext* ctx) override;

    antlrcpp::Any visitArrayInitStmt(SpiceParser::ArrayInitStmtContext* ctx) override;

    antlrcpp::Any visitImportStmt(SpiceParser::ImportStmtContext* ctx) override;

    antlrcpp::Any visitReturnStmt(SpiceParser::ReturnStmtContext* ctx) override;

    antlrcpp::Any visitBreakStmt(SpiceParser::BreakStmtContext* ctx) override;

    antlrcpp::Any visitContinueStmt(SpiceParser::ContinueStmtContext* ctx) override;

    antlrcpp::Any visitPrintfStmt(SpiceParser::PrintfStmtContext* ctx) override;

    antlrcpp::Any visitAssignExpr(SpiceParser::AssignExprContext* ctx) override;

    antlrcpp::Any visitTernaryExpr(SpiceParser::TernaryExprContext* ctx) override;

    antlrcpp::Any visitLogicalOrExpr(SpiceParser::LogicalOrExprContext* ctx) override;

    antlrcpp::Any visitLogicalAndExpr(SpiceParser::LogicalAndExprContext* ctx) override;

    antlrcpp::Any visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext* ctx) override;

    antlrcpp::Any visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext* ctx) override;

    antlrcpp::Any visitEqualityExpr(SpiceParser::EqualityExprContext* ctx) override;

    antlrcpp::Any visitRelationalExpr(SpiceParser::RelationalExprContext* ctx) override;

    antlrcpp::Any visitShiftExpr(SpiceParser::ShiftExprContext* ctx) override;

    antlrcpp::Any visitAdditiveExpr(SpiceParser::AdditiveExprContext* ctx) override;

    antlrcpp::Any visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext* ctx) override;

    antlrcpp::Any visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext* ctx) override;

    antlrcpp::Any visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext* ctx) override;

    antlrcpp::Any visitAtomicExpr(SpiceParser::AtomicExprContext* ctx) override;

    antlrcpp::Any visitIdenValue(SpiceParser::IdenValueContext* ctx) override;

    antlrcpp::Any visitValue(SpiceParser::ValueContext* ctx) override;

    antlrcpp::Any visitDataType(SpiceParser::DataTypeContext* ctx) override;

private:
    // Members
    std::string mainSourceFile;
    llvm::Triple targetTriple;
    std::string objectDir;
    bool debugOutput;
    int optLevel;
    bool mustHaveMainFunction = true;
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    std::unique_ptr<llvm::Module> module;
    std::vector<llvm::Function*> functions;
    SymbolTable* currentScope;
    std::string currentVar;
    SymbolType currentSymbolType;
    bool blockAlreadyTerminated = false;

    // Private methods
    void initializeExternalFunctions();

    llvm::Value* createAddInst(llvm::Value*, llvm::Type*, llvm::Value*, llvm::Type*);

    llvm::Value* createSubInst(llvm::Value*, llvm::Type*, llvm::Value*, llvm::Type*);

    llvm::Value* createMulInst(llvm::Value*, llvm::Type*, llvm::Value*, llvm::Type*);

    llvm::Value* createDivInst(llvm::Value*, llvm::Type*, llvm::Value*, llvm::Type*);

    void moveInsertPointToBlock(llvm::BasicBlock*);

    void createBr(llvm::BasicBlock*);

    void createCondBr(llvm::Value*, llvm::BasicBlock*, llvm::BasicBlock*);

    llvm::Type* getTypeFromSymbolType(SymbolType);
};
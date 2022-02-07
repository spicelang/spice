// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <regex>

#include <generator/OpRuleConversionsManager.h>
#include <symbol/SymbolTable.h>
#include <symbol/ScopePath.h>

#include <SpiceBaseVisitor.h>

#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/Host.h>
#include <llvm/Passes/PassBuilder.h>

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
    antlrcpp::Any visitExtDecl(SpiceParser::ExtDeclContext* ctx) override;
    antlrcpp::Any visitStructDef(SpiceParser::StructDefContext* ctx) override;
    antlrcpp::Any visitGlobalVarDef(SpiceParser::GlobalVarDefContext* ctx) override;
    antlrcpp::Any visitForLoop(SpiceParser::ForLoopContext* ctx) override;
    antlrcpp::Any visitForeachLoop(SpiceParser::ForeachLoopContext* ctx) override;
    antlrcpp::Any visitWhileLoop(SpiceParser::WhileLoopContext* ctx) override;
    antlrcpp::Any visitStmtLst(SpiceParser::StmtLstContext* ctx) override;
    antlrcpp::Any visitIfStmt(SpiceParser::IfStmtContext* ctx) override;
    antlrcpp::Any visitElseStmt(SpiceParser::ElseStmtContext* ctx) override;
    antlrcpp::Any visitDeclStmt(SpiceParser::DeclStmtContext* ctx) override;
    antlrcpp::Any visitImportStmt(SpiceParser::ImportStmtContext* ctx) override;
    antlrcpp::Any visitReturnStmt(SpiceParser::ReturnStmtContext* ctx) override;
    antlrcpp::Any visitBreakStmt(SpiceParser::BreakStmtContext* ctx) override;
    antlrcpp::Any visitContinueStmt(SpiceParser::ContinueStmtContext* ctx) override;
    antlrcpp::Any visitBuiltinCall(SpiceParser::BuiltinCallContext* ctx) override;
    antlrcpp::Any visitPrintfCall(SpiceParser::PrintfCallContext* ctx) override;
    antlrcpp::Any visitSizeOfCall(SpiceParser::SizeOfCallContext* ctx) override;
    antlrcpp::Any visitAssignExpr(SpiceParser::AssignExprContext* ctx) override;
    antlrcpp::Any visitTernaryExpr(SpiceParser::TernaryExprContext* ctx) override;
    antlrcpp::Any visitLogicalOrExpr(SpiceParser::LogicalOrExprContext* ctx) override;
    antlrcpp::Any visitLogicalAndExpr(SpiceParser::LogicalAndExprContext* ctx) override;
    antlrcpp::Any visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext* ctx) override;
    antlrcpp::Any visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext* ctx) override;
    antlrcpp::Any visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext* ctx) override;
    antlrcpp::Any visitEqualityExpr(SpiceParser::EqualityExprContext* ctx) override;
    antlrcpp::Any visitRelationalExpr(SpiceParser::RelationalExprContext* ctx) override;
    antlrcpp::Any visitShiftExpr(SpiceParser::ShiftExprContext* ctx) override;
    antlrcpp::Any visitAdditiveExpr(SpiceParser::AdditiveExprContext* ctx) override;
    antlrcpp::Any visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext* ctx) override;
    antlrcpp::Any visitCastExpr(SpiceParser::CastExprContext* ctx) override;
    antlrcpp::Any visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext* ctx) override;
    antlrcpp::Any visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext* ctx) override;
    antlrcpp::Any visitAtomicExpr(SpiceParser::AtomicExprContext* ctx) override;
    antlrcpp::Any visitValue(SpiceParser::ValueContext* ctx) override;
    antlrcpp::Any visitPrimitiveValue(SpiceParser::PrimitiveValueContext* ctx) override;
    antlrcpp::Any visitDataType(SpiceParser::DataTypeContext* ctx) override;
    antlrcpp::Any visitBaseDataType(SpiceParser::BaseDataTypeContext* ctx) override;

private:
    // Members
    std::unique_ptr<OpRuleConversionsManager> conversionsManager;
    std::string mainSourceFile;
    llvm::Triple targetTriple;
    llvm::TargetMachine* targetMachine{};
    std::string objectDir;
    bool debugOutput;
    int optLevel;
    bool mustHaveMainFunction = true;
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    std::unique_ptr<llvm::Module> module;
    std::vector<llvm::Function*> functions;
    SymbolTable* currentScope;
    std::string scopePrefix;
    SymbolType currentSymbolType;
    ScopePath scopePath;
    bool blockAlreadyTerminated = false;
    llvm::Value* currentThisValue = nullptr;
    llvm::BasicBlock* allocaInsertBlock = nullptr;
    llvm::Instruction* allocaInsertInst = nullptr;
    std::stack<llvm::BasicBlock*> breakBlocks;
    std::stack<llvm::BasicBlock*> continueBlocks;
    bool constNegate = false;
    bool allParamsHardcoded = true;
    llvm::Constant* currentConstValue = nullptr;
    bool currentVarSigned = false;
    std::string currentVarName;
    std::string lhsVarName;
    llvm::Type* lhsType = nullptr;
    llvm::Type* structAccessType = nullptr;
    llvm::Value* structAccessAddress = nullptr;
    std::vector<llvm::Value*> structAccessIndices;

    // Private methods
    void initializeExternalFunctions();
    void moveInsertPointToBlock(llvm::BasicBlock* block);
    void createBr(llvm::BasicBlock* targetBlock);
    void createCondBr(llvm::Value* condition, llvm::BasicBlock* trueBlock, llvm::BasicBlock* falseBlock);
    llvm::Value* insertAlloca(llvm::Type* llvmType, const std::string& varName = "", llvm::Value* arraySize = nullptr);
    llvm::Type* getTypeForSymbolType(SymbolType symbolType);
    //llvm::Value* getDefaultValueForType(SymbolType symbolType);
    void initExtStruct(const std::string& oldStructName, const std::string& newStructName);
    bool compareLLVMTypes(llvm::Type* lhs, llvm::Type* rhs);
    llvm::Value* doImplicitCast(llvm::Value* lhs, llvm::Type* rhs);
    [[nodiscard]] llvm::PassBuilder::OptimizationLevel getLLVMOptLevelFromSpiceOptLevel() const;
};
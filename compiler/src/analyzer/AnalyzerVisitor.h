// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include "SpiceBaseVisitor.h"
#include "SpiceLexer.h"

#include <CompilerInstance.h>
#include "SymbolTable.h"
#include "ModuleRegistry.h"
#include <util/ScopeIdUtil.h>
#include <util/FileUtil.h>
#include <exception/SemanticError.h>

#include <llvm/ADT/Triple.h>
#include <llvm/Support/Host.h>

#include <utility>

const static std::string RETURN_VARIABLE_NAME = "result";

class AnalyzerVisitor : public SpiceBaseVisitor {
public:
    // Constructors
    explicit AnalyzerVisitor(std::string sourceFile, std::string targetArch, std::string targetVendor, std::string targetOs,
                             std::string outputPath, bool debugOutput, int optLevel, bool mustHaveMainFunction) :
            mainSourceFile(std::move(sourceFile)),
            targetArch(std::move(targetArch)),
            targetVendor(std::move(targetVendor)),
            targetOs(std::move(targetOs)),
            objectDir(std::move(outputPath)),
            debugOutput(debugOutput),
            optLevel(optLevel),
            mustHaveMainFunction(mustHaveMainFunction) {
        if (targetArch.empty()) {
            llvm::Triple targetTriple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
            targetArch = targetTriple.getArchName();
            targetVendor = targetTriple.getVendorName();
            targetOs = targetTriple.getOSName();
        }
    }

    // Public methods
    antlrcpp::Any visitEntry(SpiceParser::EntryContext* ctx) override;

    antlrcpp::Any visitMainFunctionDef(SpiceParser::MainFunctionDefContext* ctx) override;

    antlrcpp::Any visitFunctionDef(SpiceParser::FunctionDefContext* ctx) override;

    antlrcpp::Any visitProcedureDef(SpiceParser::ProcedureDefContext* ctx) override;

    antlrcpp::Any visitStructDef(SpiceParser::StructDefContext* ctx) override;

    antlrcpp::Any visitForLoop(SpiceParser::ForLoopContext* ctx) override;

    antlrcpp::Any visitWhileLoop(SpiceParser::WhileLoopContext* ctx) override;

    antlrcpp::Any visitIfStmt(SpiceParser::IfStmtContext* ctx) override;

    antlrcpp::Any visitElseStmt(SpiceParser::ElseStmtContext* ctx) override;

    antlrcpp::Any visitParamLstDef(SpiceParser::ParamLstDefContext* ctx) override;

    antlrcpp::Any visitDeclStmt(SpiceParser::DeclStmtContext* ctx) override;

    antlrcpp::Any visitFunctionCall(SpiceParser::FunctionCallContext* ctx) override;

    antlrcpp::Any visitImportStmt(SpiceParser::ImportStmtContext* ctx) override;

    antlrcpp::Any visitReturnStmt(SpiceParser::ReturnStmtContext* ctx) override;

    antlrcpp::Any visitBreakStmt(SpiceParser::BreakStmtContext* ctx) override;

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

    antlrcpp::Any visitValue(SpiceParser::ValueContext* ctx) override;

private:
    // Members
    std::string mainSourceFile;
    std::string targetArch;
    std::string targetVendor;
    std::string targetOs;
    std::string objectDir;
    bool debugOutput;
    int optLevel;
    bool mustHaveMainFunction = true;
    SymbolTable* currentScope = new SymbolTable(nullptr);
    bool parameterMode = false;
    bool hasMainFunction = false;
    int nestedLoopCounter = 0;

    // Private functions
    static SymbolType getSymbolTypeFromDataType(SpiceParser::DataTypeContext*);
};
// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include "SpiceBaseVisitor.h"
#include "SpiceLexer.h"

#include <CompilerInstance.h>
#include "SymbolTable.h"
#include "ModuleRegistry.h"
#include <util/ScopeIdUtil.h>
#include <util/FileUtil.h>
#include <util/IdentifierUtil.h>
#include <exception/SemanticError.h>

#include <llvm/ADT/Triple.h>
#include <llvm/Support/Host.h>

#include <utility>

const static std::string RETURN_VARIABLE_NAME = "result";
const static std::string TMP_VARIABLE_NAME = "tmp";

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

    antlrcpp::Any visitGlobalVarDef(SpiceParser::GlobalVarDefContext* ctx) override;

    antlrcpp::Any visitForLoop(SpiceParser::ForLoopContext* ctx) override;

    antlrcpp::Any visitWhileLoop(SpiceParser::WhileLoopContext* ctx) override;

    antlrcpp::Any visitIfStmt(SpiceParser::IfStmtContext* ctx) override;

    antlrcpp::Any visitElseStmt(SpiceParser::ElseStmtContext* ctx) override;

    antlrcpp::Any visitParamLstDef(SpiceParser::ParamLstDefContext* ctx) override;

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
    std::string targetArch;
    std::string targetVendor;
    std::string targetOs;
    std::string objectDir;
    bool debugOutput;
    int optLevel;
    bool mustHaveMainFunction = true;
    SymbolTable* currentScope = new SymbolTable(nullptr, mustHaveMainFunction);
    bool parameterMode = false;
    bool hasMainFunction = false;
    int nestedLoopCounter = 0;
};
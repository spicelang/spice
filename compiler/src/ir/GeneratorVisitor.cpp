// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "GeneratorVisitor.h"

GeneratorVisitor::GeneratorVisitor() {
    // Initialize LLVM
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    // Configure output target
    // ToDo: Make target customizable by setting an cli arg or similar
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    module->setTargetTriple(targetTriple);

    // Search after selected target
    std::string error;
    auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
    if (!target) throw IRError(TARGET_NOT_AVAILABLE, "Selected target was not found: " + error);

    auto cpu = "generic";
    auto features = "";

    llvm::TargetOptions opt;
    auto rm = llvm::Optional<llvm::Reloc::Model>();
    auto targetMachine = target->createTargetMachine(targetTriple, cpu, features, opt, rm);

    module->setDataLayout(targetMachine->createDataLayout());

    std::string filename = "output.o";
    std::error_code errorCode;
    llvm::raw_fd_ostream dest(filename, errorCode, llvm::sys::fs::OF_None);
    if (errorCode) throw IRError(CANT_OPEN_OUTPUT_FILE, "File '" + filename + "' could not be opened");

    llvm::legacy::PassManager pass;
    auto FileType = llvm::CGFT_ObjectFile;
    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType))
        throw IRError(WRONG_TYPE, "Target machine can't emit a file of this type");

    pass.run(*module);
    dest.flush();
}

antlrcpp::Any GeneratorVisitor::visitEntry(SpiceParser::EntryContext *ctx) {
    return getIRString();
}

antlrcpp::Any GeneratorVisitor::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
    return SpiceBaseVisitor::visitFunctionDef(ctx);
}

antlrcpp::Any GeneratorVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
    return SpiceBaseVisitor::visitProcedureDef(ctx);
}

antlrcpp::Any GeneratorVisitor::visitForLoop(SpiceParser::ForLoopContext *ctx) {
    return SpiceBaseVisitor::visitForLoop(ctx);
}

antlrcpp::Any GeneratorVisitor::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
    return SpiceBaseVisitor::visitWhileLoop(ctx);
}

antlrcpp::Any GeneratorVisitor::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
    return SpiceBaseVisitor::visitIfStmt(ctx);
}

antlrcpp::Any GeneratorVisitor::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
    return SpiceBaseVisitor::visitDeclStmt(ctx);
}

antlrcpp::Any GeneratorVisitor::visitFunctionCall(SpiceParser::FunctionCallContext *ctx) {
    return SpiceBaseVisitor::visitFunctionCall(ctx);
}

antlrcpp::Any GeneratorVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {
    return SpiceBaseVisitor::visitImportStmt(ctx);
}

antlrcpp::Any GeneratorVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
    return SpiceBaseVisitor::visitReturnStmt(ctx);
}

antlrcpp::Any GeneratorVisitor::visitAssignment(SpiceParser::AssignmentContext *ctx) {
    return SpiceBaseVisitor::visitAssignment(ctx);
}

antlrcpp::Any GeneratorVisitor::visitTernary(SpiceParser::TernaryContext *ctx) {
    return SpiceBaseVisitor::visitTernary(ctx);
}

antlrcpp::Any GeneratorVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
    return SpiceBaseVisitor::visitLogicalOrExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
    return SpiceBaseVisitor::visitLogicalAndExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
    return SpiceBaseVisitor::visitBitwiseOrExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
    return SpiceBaseVisitor::visitBitwiseAndExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) {
    return SpiceBaseVisitor::visitEqualityExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {
    return SpiceBaseVisitor::visitRelationalExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {
    return SpiceBaseVisitor::visitAdditiveExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
    return SpiceBaseVisitor::visitMultiplicativeExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitPrefixUnary(SpiceParser::PrefixUnaryContext *ctx) {
    return SpiceBaseVisitor::visitPrefixUnary(ctx);
}

antlrcpp::Any GeneratorVisitor::visitPostfixUnary(SpiceParser::PostfixUnaryContext *ctx) {
    return SpiceBaseVisitor::visitPostfixUnary(ctx);
}

antlrcpp::Any GeneratorVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
    return SpiceBaseVisitor::visitAtomicExpr(ctx);
}

antlrcpp::Any GeneratorVisitor::visitValue(SpiceParser::ValueContext *ctx) {
    if (ctx->DOUBLE())
        return llvm::ConstantFP::get(*context, llvm::APFloat(std::stod(ctx->DOUBLE()->toString())));

    if (ctx->INTEGER())
        return llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context),
                                            std::stoi(ctx->INTEGER()->toString()));
    if (ctx->STRING()) {
        std::string value = ctx->STRING()->toString();
        auto charType = llvm::IntegerType::get(*context, 8);
        std::vector<llvm::Constant *> chars(value.size());
        for(unsigned int i = 0; i < value.size(); i++)
            chars[i] = llvm::ConstantInt::get(charType, value[i]);
        return llvm::ConstantArray::get(llvm::ArrayType::get(charType, chars.size()), chars);
    }

    if (ctx->TRUE() || ctx->FALSE())
        return llvm::ConstantInt::getSigned((llvm::Type::getInt1Ty(*context)), ctx->TRUE() ? 1 : 0);

    if (ctx->IDENTIFIER()) {
        llvm::Value* var = namedValues[ctx->IDENTIFIER()->toString()];
        if (!var) throw std::runtime_error("Internal compiler error - Variable not found in code generation step");
        return builder->CreateLoad(llvm::Type::getDoubleTy(*context), var,
                                   ctx->IDENTIFIER()->toString().c_str());
    }

    return nullptr;
}

std::string GeneratorVisitor::getIRString() {
    std::string output;
    llvm::raw_string_ostream oss(output);
    module->print(oss, nullptr);
    return oss.str();
}

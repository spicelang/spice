// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "GeneratorVisitor.h"

void GeneratorVisitor::init() {
    // Initialize LLVM
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
}

void GeneratorVisitor::optimize() {
    /*// Register optimization passes
    std::unique_ptr<llvm::legacy::FunctionPassManager> functionPassManager =
            llvm::make_unique<llvm::legacy::FunctionPassManager>(module.get());

    // Promote allocas to registers.
    functionPassManager->add(llvm::createPromoteMemoryToRegisterPass());
    // Do simple "peephole" optimizations
    functionPassManager->add(llvm::createInstructionCombiningPass());
    // Reassociate expressions.
    functionPassManager->add(llvm::createReassociatePass());
    // Eliminate Common SubExpressions.
    functionPassManager->add(llvm::createGVNPass());
    // Simplify the control flow graph (deleting unreachable blocks etc).
    functionPassManager->add(llvm::createCFGSimplificationPass());

    functionPassManager->doInitialization();

    for (auto &function : functions) {
        llvm::Function *llvmFun =
                module->getFunction(llvm::StringRef(function->functionName));
        functionPassManager->run(*llvmFun);
    }

    llvm::Function *llvmMainFun = module->getFunction(llvm::StringRef("main"));
    functionPassManager->run(*llvmMainFun);*/
}

void GeneratorVisitor::emit() {
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

    // Open file output stream
    std::string filename = "output.o";
    std::error_code errorCode;
    llvm::raw_fd_ostream dest(filename, errorCode, llvm::sys::fs::OF_None);
    if (errorCode) throw IRError(CANT_OPEN_OUTPUT_FILE, "File '" + filename + "' could not be opened");

    llvm::legacy::PassManager pass;
    auto FileType = llvm::CGFT_ObjectFile;
    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType))
        throw IRError(WRONG_TYPE, "Target machine can't emit a file of this type");

    // Emit object file
    pass.run(*module);
    dest.flush();
}

void GeneratorVisitor::dumpIR() {
    module->print(llvm::outs(), nullptr);
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
    if (ctx->multiplicativeExpr().size() > 1) {
        auto lhs = visit(ctx->multiplicativeExpr()[0]).as<llvm::Value*>();
        for (int i = 1; i < ctx->multiplicativeExpr().size(); i++) {
            auto rhs = visit(ctx->multiplicativeExpr()[i]).as<llvm::Value*>();
            if (ctx->PLUS()[i-1])
                lhs = builder->CreateAdd(lhs, rhs, "add");
            else
                lhs = builder->CreateSub(lhs, rhs, "sub");
        }
    }
    return visit(ctx->multiplicativeExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
    if (ctx->prefixUnary().size() > 1) {
        auto lhs = visit(ctx->prefixUnary()[0]).as<llvm::Value*>();
        for (int i = 1; i < ctx->prefixUnary().size(); i++) {
            auto rhs = visit(ctx->prefixUnary()[i]).as<llvm::Value*>();
            if (ctx->MUL()[i-1])
                lhs = builder->CreateMul(lhs, rhs, "mul");
            else
                lhs = builder->CreateSDiv(lhs, rhs, "div");
        }
        return lhs;
    }
    return visit(ctx->prefixUnary()[0]);
}

antlrcpp::Any GeneratorVisitor::visitPrefixUnary(SpiceParser::PrefixUnaryContext *ctx) {
    auto value = visit(ctx->postfixUnary()).as<llvm::Value*>();

    // Prefix unary is: PLUS_PLUS postfixUnary
    if (ctx->PLUS_PLUS()) return builder->CreateAdd(value, builder->getInt32(1), "++ prefix");

    // Prefix unary is: MINUS_MINUS postfixUnary
    if (ctx->MINUS_MINUS()) return builder->CreateSub(value, builder->getInt32(1), "-- prefix");

    // Prefix unary is: NOT postfixUnary
    return builder->CreateNot(value, "not");
}

antlrcpp::Any GeneratorVisitor::visitPostfixUnary(SpiceParser::PostfixUnaryContext *ctx) {
    auto value = visit(ctx->atomicExpr()).as<llvm::Value*>();

    // Postfix unary is: PLUS_PLUS atomicExpr
    if (ctx->PLUS_PLUS()) return builder->CreateAdd(value, builder->getInt32(1), "++ postfix");

    // Postfix unary is: MINUS_MINUS atomicExpr
    return builder->CreateSub(value, builder->getInt32(1), "-- postfix");
}

antlrcpp::Any GeneratorVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
    // Atomic expr is: LPAREN value RPAREN
    if (ctx->LPAREN()) return visit(ctx->assignment());

    // Atomic expr is: value
    return visit(ctx->value());
}

antlrcpp::Any GeneratorVisitor::visitValue(SpiceParser::ValueContext *ctx) {
    // Value is a double constant
    if (ctx->DOUBLE())
        return llvm::ConstantFP::get(*context, llvm::APFloat(std::stod(ctx->DOUBLE()->toString())));

    // Value is an integer constant
    if (ctx->INTEGER())
        return llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context),
                                            std::stoi(ctx->INTEGER()->toString()));

    // Value is a string constant
    if (ctx->STRING()) {
        std::string value = ctx->STRING()->toString();
        auto charType = llvm::IntegerType::get(*context, 8);
        std::vector<llvm::Constant *> chars(value.size());
        for(unsigned int i = 0; i < value.size(); i++)
            chars[i] = llvm::ConstantInt::get(charType, value[i]);
        return llvm::ConstantArray::get(llvm::ArrayType::get(charType, chars.size()), chars);
    }

    // Value is a boolean constant
    if (ctx->TRUE() || ctx->FALSE())
        return llvm::ConstantInt::getSigned((llvm::Type::getInt1Ty(*context)), ctx->TRUE() ? 1 : 0);

    // Value is an identifier
    if (ctx->IDENTIFIER()) {
        llvm::Value* var = namedValues[ctx->IDENTIFIER()->toString()];
        if (!var) throw std::runtime_error("Internal compiler error - Variable not found in code generation step");
        return var;
    }

    // Value is a function call


    return nullptr;
}

std::string GeneratorVisitor::getIRString() {
    std::string output;
    llvm::raw_string_ostream oss(output);
    module->print(oss, nullptr);
    return oss.str();
}

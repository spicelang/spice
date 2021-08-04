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
    // Register optimization passes
    auto functionPassManager = std::make_unique<llvm::legacy::FunctionPassManager>(module.get());

    // Promote allocas to registers.
    functionPassManager->add(llvm::createPromoteMemoryToRegisterPass());
    // Do simple "peephole" optimizations
    functionPassManager->add(llvm::createInstructionCombiningPass());
    // Re-associate expressions.
    functionPassManager->add(llvm::createReassociatePass());
    // Eliminate Common SubExpressions.
    functionPassManager->add(llvm::createGVNPass());
    // Simplify the control flow graph (deleting unreachable blocks etc).
    //functionPassManager->add(llvm::createCFGSimplificationPass()); // Breaks recursion in fibonacci.spice

    // Run optimizing passes for all functions
    functionPassManager->doInitialization();
    for (auto& fct : functions) functionPassManager->run(*fct);
}

void GeneratorVisitor::emit(std::string targetTriple, const std::string& outputPath) {
    // Configure output target
    if (targetTriple.empty()) targetTriple = llvm::sys::getDefaultTargetTriple();
    module->setTargetTriple(targetTriple);

    std::cout << "Emitting executable for following triplet '" << targetTriple << "' to following path: " << outputPath << std::endl;

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
    std::error_code errorCode;
    llvm::raw_fd_ostream dest(outputPath, errorCode, llvm::sys::fs::OF_None);
    if (errorCode) throw IRError(CANT_OPEN_OUTPUT_FILE, "File '" + outputPath + "' could not be opened");

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

antlrcpp::Any GeneratorVisitor::visitEntry(SpiceParser::EntryContext* ctx) {
    // Generate code for external functions
    initializeExternalFunctions();

    return SpiceBaseVisitor::visitEntry(ctx);
}

antlrcpp::Any GeneratorVisitor::visitMainFunctionDef(SpiceParser::MainFunctionDefContext* ctx) {
    // Build function itself
    std::string functionName = "main";
    auto returnType = llvm::IntegerType::getInt32Ty(*context);
    auto mainType = llvm::FunctionType::get(returnType, std::vector<llvm::Type*>(), false);
    auto fct = llvm::Function::Create(mainType, llvm::Function::ExternalLinkage, functionName, module.get());
    auto bMain = llvm::BasicBlock::Create(*context, "main_entry", fct);
    builder->SetInsertPoint(bMain);

    // Change scope
    namedValues.clear();
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->getChild(scopeId);

    // Declare result variable
    namedValues["result"] = builder->CreateAlloca(returnType, nullptr, "result");

    // Generate IR for function body
    visit(ctx->stmtLst());

    // Verify function
    llvm::verifyFunction(*fct);

    // Add function to function list
    functions.push_back(fct);

    // Change scope back
    currentScope = currentScope->getParent();

    // Return true as result for the function definition
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context), 1);
}

antlrcpp::Any GeneratorVisitor::visitFunctionDef(SpiceParser::FunctionDefContext* ctx) {
    std::string functionName = ctx->IDENTIFIER()->toString();

    // Change scope
    FunctionSignature signature = currentScope->popSignature();
    currentScope = currentScope->getChild(signature.toString());
    namedValues.clear();

    // Get return type
    currentVar = RETURN_VARIABLE_NAME;
    auto returnType = visit(ctx->dataType()).as<llvm::Type*>();

    // Create function itself
    std::vector<std::string> paramNames;
    std::vector<llvm::Type*> paramTypes;
    for (auto& param : ctx->paramLstDef()->declStmt()) { // Parameters without default value
        currentVar = param->IDENTIFIER()->toString();
        paramNames.push_back(currentVar);
        auto paramType = visit(param->dataType()).as<llvm::Type*>();
        paramTypes.push_back(paramType);
    }
    for (auto& param : ctx->paramLstDef()->assignment()) { // Parameters with default value
        currentVar = param->declStmt()->IDENTIFIER()->toString();
        paramNames.push_back(currentVar);
        auto paramType = visit(param->declStmt()->dataType()).as<llvm::Type*>();
        paramTypes.push_back(paramType);
    }
    auto fctType = llvm::FunctionType::get(returnType, paramTypes, false);
    auto fct = llvm::Function::Create(fctType, llvm::Function::ExternalLinkage, signature.toString(), module.get());

    // Create entry block
    auto bEntry = llvm::BasicBlock::Create(*context, "entry");
    fct->getBasicBlockList().push_back(bEntry);
    builder->SetInsertPoint(bEntry);

    // Store function params
    for (auto& param : fct->args()) {
        auto paramNo = param.getArgNo();
        if (paramNo < ctx->paramLstDef()->declStmt().size()) {
            auto paramName = paramNames[paramNo];
            llvm::Type* paramType = fct->getFunctionType()->getParamType(paramNo);
            namedValues[paramName] = builder->CreateAlloca(paramType, nullptr, paramName);
            builder->CreateStore(&param, namedValues[paramName]);
        } else {
            auto paramName = paramNames[paramNo];
            llvm::Type* paramType = fct->getFunctionType()->getParamType(paramNo);
            namedValues[paramName] = builder->CreateAlloca(paramType, nullptr, paramName);
            builder->CreateStore(&param, namedValues[paramName]);
        }
    }

    // Declare result variable
    namedValues["result"] = builder->CreateAlloca(returnType, nullptr, "result");

    // Generate IR for function body
    visit(ctx->stmtLst());

    // Verify function
    llvm::verifyFunction(*fct);

    // Add function to function list
    functions.push_back(fct);

    // Change scope back
    currentScope = currentScope->getParent();

    // Return true as result for the function definition
    return llvm::ConstantInt::get((llvm::Type::getInt1Ty(*context)), 1);
}

antlrcpp::Any GeneratorVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext* ctx) {
    auto procedureName = ctx->IDENTIFIER()->toString();

    // Change scope
    FunctionSignature signature = currentScope->popSignature();
    currentScope = currentScope->getChild(signature.toString());
    namedValues.clear();

    // Create procedure itself
    std::vector<std::string> paramNames;
    std::vector<llvm::Type*> paramTypes;
    for (auto& param : ctx->paramLstDef()->declStmt()) { // Parameters without default value
        currentVar = param->IDENTIFIER()->toString();
        paramNames.push_back(currentVar);
        auto paramType = visit(param->dataType()).as<llvm::Type*>();
        paramTypes.push_back(paramType);
    }
    for (auto& param : ctx->paramLstDef()->assignment()) { // Parameters with default value
        currentVar = param->declStmt()->IDENTIFIER()->toString();
        paramNames.push_back(currentVar);
        auto paramType = visit(param->declStmt()->dataType()).as<llvm::Type*>();
        paramTypes.push_back(paramType);
    }
    auto procType = llvm::FunctionType::get(llvm::Type::getVoidTy(*context), paramTypes, false);
    auto proc = llvm::Function::Create(procType, llvm::Function::ExternalLinkage, signature.toString(), module.get());

    // Create entry block
    auto bEntry = llvm::BasicBlock::Create(*context, "entry");
    proc->getBasicBlockList().push_back(bEntry);
    builder->SetInsertPoint(bEntry);

    // Store procedure params
    for (auto& param : proc->args()) {
        auto paramNo = param.getArgNo();
        if (paramNo < ctx->paramLstDef()->declStmt().size()) {
            auto paramName = paramNames[paramNo];
            llvm::Type* paramType = proc->getFunctionType()->getParamType(paramNo);
            namedValues[paramName] = builder->CreateAlloca(paramType, nullptr, paramName);
            builder->CreateStore(&param, namedValues[paramName]);
        } else {
            auto paramName = paramNames[paramNo];
            llvm::Type* paramType = proc->getFunctionType()->getParamType(paramNo);
            namedValues[paramName] = builder->CreateAlloca(paramType, nullptr, paramName);
            builder->CreateStore(&param, namedValues[paramName]);
        }
    }

    // Generate IR for procedure body
    visit(ctx->stmtLst());

    // Create return
    builder->CreateRetVoid();

    // Verify procedure
    llvm::verifyFunction(*proc);

    // Add function to function list
    functions.push_back(proc);

    // Change scope back
    currentScope = currentScope->getParent();

    // Return true as result for the function definition
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context), 1);
}

antlrcpp::Any GeneratorVisitor::visitForLoop(SpiceParser::ForLoopContext* ctx) {
    auto parentFct = builder->GetInsertBlock()->getParent();

    // Create blocks
    auto bCond = llvm::BasicBlock::Create(*context, "for_cond");
    auto bLoop = llvm::BasicBlock::Create(*context, "for");
    auto bLoopEnd = llvm::BasicBlock::Create(*context, "for_end");

    // Change scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->getChild(scopeId);

    // Execute pre-loop stmts
    visit(ctx->assignment()[0]);
    // Jump in condition block
    builder->CreateBr(bCond);

    // Fill condition block
    parentFct->getBasicBlockList().push_back(bCond);
    builder->SetInsertPoint(bCond);
    auto conditionValue = visit(ctx->assignment()[1]).as<llvm::Value*>();
    // Jump to loop body or to loop end
    builder->CreateCondBr(conditionValue, bLoop, bLoopEnd);

    // Fill loop block
    parentFct->getBasicBlockList().push_back(bLoop);
    builder->SetInsertPoint(bLoop);
    // Generate IR for nested statements
    visit(ctx->stmtLst());
    // Run post-loop actions
    visit(ctx->assignment()[2]);
    // Jump in condition block
    builder->CreateBr(bCond);

    // Fill loop end block
    parentFct->getBasicBlockList().push_back(bLoopEnd);
    builder->SetInsertPoint(bLoopEnd);

    // Change scope back
    currentScope = currentScope->getParent();

    // Return true as result for the loop
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context), 1);
}

antlrcpp::Any GeneratorVisitor::visitWhileLoop(SpiceParser::WhileLoopContext* ctx) {
    auto conditionValue = visit(ctx->assignment()).as<llvm::Value*>();
    auto parentFct = builder->GetInsertBlock()->getParent();

    // Create blocks
    auto bLoop = llvm::BasicBlock::Create(*context, "while");
    auto bLoopEnd = llvm::BasicBlock::Create(*context, "while_end");

    // Check if entering the loop is necessary
    builder->CreateCondBr(conditionValue, bLoop, bLoopEnd);

    // Change scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->getChild(scopeId);

    // Fill loop block
    parentFct->getBasicBlockList().push_back(bLoop);
    builder->SetInsertPoint(bLoop);
    // Generate IR for nested statements
    visit(ctx->stmtLst());
    // Visit condition again
    conditionValue = visit(ctx->assignment()).as<llvm::Value*>();
    // Check if condition is now false
    bLoop = builder->GetInsertBlock();
    builder->CreateCondBr(conditionValue, bLoop, bLoopEnd);

    // Fill loop end block
    parentFct->getBasicBlockList().push_back(bLoopEnd);
    builder->SetInsertPoint(bLoopEnd);

    // Change scope back
    currentScope = currentScope->getParent();

    // Return true as result for the loop
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context), 1);
}

antlrcpp::Any GeneratorVisitor::visitIfStmt(SpiceParser::IfStmtContext* ctx) {
    auto conditionValue = visit(ctx->assignment()).as<llvm::Value*>();
    auto parentFct = builder->GetInsertBlock()->getParent();

    // Create blocks
    auto bThen = llvm::BasicBlock::Create(*context, "then");
    auto bEnd = llvm::BasicBlock::Create(*context, "end");

    // Check if if condition is fulfilled
    builder->CreateCondBr(conditionValue, bThen, bEnd);

    // Change scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->getChild(scopeId);

    // Fill then block
    parentFct->getBasicBlockList().push_back(bThen);
    builder->SetInsertPoint(bThen);
    // Generate IR for nested statements
    visit(ctx->stmtLst());
    builder->CreateBr(bEnd);

    // Fill end block
    parentFct->getBasicBlockList().push_back(bEnd);
    builder->SetInsertPoint(bEnd);

    // Change scope back
    currentScope = currentScope->getParent();

    // Return conditional value as result for the if stmt
    return conditionValue;
}

antlrcpp::Any GeneratorVisitor::visitDeclStmt(SpiceParser::DeclStmtContext* ctx) {
    std::string varName = currentVar = ctx->IDENTIFIER()->toString();

    llvm::Type* varType = visit(ctx->dataType()).as<llvm::Type*>();

    llvm::Function* parentFunction = builder->GetInsertBlock()->getParent();
    llvm::IRBuilder<> tmpBuilder(&parentFunction->getEntryBlock(), parentFunction->getEntryBlock().begin());
    llvm::AllocaInst* var = tmpBuilder.CreateAlloca(varType, nullptr, varName);
    namedValues[varName] = var;
    return varName;
}

antlrcpp::Any GeneratorVisitor::visitFunctionCall(SpiceParser::FunctionCallContext* ctx) {
    auto fctName = ctx->IDENTIFIER()->toString();

    // Get function by signature
    FunctionSignature signature = currentScope->popSignature();
    auto fct = module->getFunction(signature.toString());
    auto fctType = fct->getFunctionType();

    // Fill parameter list
    std::vector<llvm::Value*> argValues;
    if (ctx->paramLstCall()) {
        for (int i = 0; i < ctx->paramLstCall()->assignment().size(); i++) {
            auto argValue = visit(ctx->paramLstCall()->assignment()[i]).as<llvm::Value*>();
            auto argType = fctType->getParamType(i);
            auto bitCastArgValue = builder->CreateBitCast(argValue, argType);
            argValues.push_back(bitCastArgValue);
        }
    }

    return (llvm::Value*) builder->CreateCall(fct, argValues);
}

antlrcpp::Any GeneratorVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext* ctx) {
    auto returnValue = visit(ctx->assignment()).as<llvm::Value*>();
    // Build return value
    builder->CreateRet(returnValue);
    return returnValue;
}

antlrcpp::Any GeneratorVisitor::visitPrintfStmt(SpiceParser::PrintfStmtContext* ctx) {
    auto printf = module->getFunction("printf");
    std::vector<llvm::Value*> printfArgs;
    auto stringTemplate = ctx->STRING()->toString();
    stringTemplate = stringTemplate.erase(stringTemplate.size() -1).erase(0, 1);
    printfArgs.push_back(builder->CreateGlobalStringPtr(stringTemplate));
    for (auto &arg : ctx->assignment()) {
        auto argVal = visit(arg).as<llvm::Value*>();
        if (argVal == nullptr) throw std::runtime_error("Printf has null arg");
        printfArgs.push_back(argVal);
    }
    return builder->CreateCall(printf, printfArgs);
}

antlrcpp::Any GeneratorVisitor::visitAssignment(SpiceParser::AssignmentContext* ctx) {
    if (ctx->declStmt() || ctx->IDENTIFIER()) {
        std::string varName = ctx->declStmt() ? visit(ctx->declStmt()).as<std::string>() : ctx->IDENTIFIER()->toString();

        // Get value of left and right side
        auto rhs = visit(ctx->ternary()).as<llvm::Value*>();
        auto lhs = namedValues[varName];
        if (!lhs) throw std::runtime_error("Internal compiler error - Variable '" + varName +
            "' not found in code generation step");
        if (ctx->ASSIGN_OP()) {
            // Store right side on the left one
            builder->CreateStore(rhs, lhs);
        } else if (ctx->PLUS_EQUAL()) {
            auto loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
            rhs = builder->CreateAdd(loadLhs, rhs, "ple");
            builder->CreateStore(rhs, lhs);
        } else if (ctx->MINUS_EQUAL()) {
            auto loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
            rhs = builder->CreateSub(loadLhs, rhs, "mie");
            builder->CreateStore(rhs, lhs);
        } else if (ctx->MUL_EQUAL()) {
            auto loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
            rhs = builder->CreateMul(loadLhs, rhs, "mue");
            builder->CreateStore(rhs, lhs);
        } else if (ctx->DIV_EQUAL()) {
            auto loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
            rhs = builder->CreateSDiv(loadLhs, rhs, "die");
            builder->CreateStore(rhs, lhs);
        }
        // Return value of the right side
        return rhs;
    }
    return visit(ctx->ternary());
}

antlrcpp::Any GeneratorVisitor::visitTernary(SpiceParser::TernaryContext* ctx) {
    if (ctx->logicalOrExpr().size() > 1) {
        auto conditionValue = visit(ctx->logicalOrExpr()[0]).as<llvm::Value*>();
        auto parentFct = builder->GetInsertBlock()->getParent();

        // Create blocks
        auto bThen = llvm::BasicBlock::Create(*context, "then");
        auto bElse = llvm::BasicBlock::Create(*context, "else");
        auto bEnd = llvm::BasicBlock::Create(*context, "end");

        // Conditional jump to respective block
        builder->CreateCondBr(conditionValue, bThen, bElse);

        // Fill then block
        parentFct->getBasicBlockList().push_back(bThen);
        builder->SetInsertPoint(bThen);
        auto thenValue = visit(ctx->logicalOrExpr()[1]).as<llvm::Value*>();
        builder->CreateBr(bEnd);

        // Fill else block
        parentFct->getBasicBlockList().push_back(bElse);
        builder->SetInsertPoint(bElse);
        auto elseValue = visit(ctx->logicalOrExpr()[2]).as<llvm::Value*>();
        builder->CreateBr(bEnd);

        // Fill end block
        parentFct->getBasicBlockList().push_back(bEnd);
        builder->SetInsertPoint(bEnd);
        // if either is void or their types don't match (which indicates one of them
        // returned the null value for void, then don't construct a phi node)
        if (thenValue->getType() == llvm::Type::getVoidTy(*context) ||
            elseValue->getType() == llvm::Type::getVoidTy(*context) ||
            thenValue->getType() != elseValue->getType()) {
            return llvm::Constant::getNullValue(llvm::Type::getInt32Ty(*context));
        }
        // Setup phi value
        auto phi = builder->CreatePHI(thenValue->getType(), 2, "phi");
        phi->addIncoming(thenValue, bThen);
        phi->addIncoming(elseValue, bElse);
        return (llvm::Value*) phi;
    }
    return visit(ctx->logicalOrExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext* ctx) {
    if (ctx->logicalAndExpr().size() > 1) {
        auto lhs = visit(ctx->logicalAndExpr()[0]).as<llvm::Value*>();
        for (int i = 1; i < ctx->logicalAndExpr().size(); i++) {
            auto rhs = visit(ctx->logicalAndExpr()[i]).as<llvm::Value*>();
            lhs = builder->CreateLogicalOr(lhs, rhs, "log_or");
        }
        return lhs;
    }
    return visit(ctx->logicalAndExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext* ctx) {
    if (ctx->bitwiseOrExpr().size() > 1) {
        auto lhs = visit(ctx->bitwiseOrExpr()[0]).as<llvm::Value*>();
        for (int i = 1; i < ctx->bitwiseOrExpr().size(); i++) {
            auto rhs = visit(ctx->bitwiseOrExpr()[i]).as<llvm::Value*>();
            lhs = builder->CreateLogicalAnd(lhs, rhs, "log_and");
        }
        return lhs;
    }
    return visit(ctx->bitwiseOrExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext* ctx) {
    if (ctx->bitwiseAndExpr().size() > 1) {
        auto lhs = visit(ctx->bitwiseAndExpr()[0]).as<llvm::Value*>();
        for (int i = 1; i < ctx->bitwiseAndExpr().size(); i++) {
            auto rhs = visit(ctx->bitwiseAndExpr()[i]).as<llvm::Value*>();
            lhs = builder->CreateOr(lhs, rhs, "bw_or");
        }
        return lhs;
    }
    return visit(ctx->bitwiseAndExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext* ctx) {
    if (ctx->equalityExpr().size() > 1) {
        auto lhs = visit(ctx->equalityExpr()[0]).as<llvm::Value*>();
        for (int i = 1; i < ctx->equalityExpr().size(); i++) {
            auto rhs = visit(ctx->equalityExpr()[i]).as<llvm::Value*>();
            lhs = builder->CreateAnd(lhs, rhs, "bw_and");
        }
        return lhs;
    }
    return visit(ctx->equalityExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitEqualityExpr(SpiceParser::EqualityExprContext* ctx) {
    if (ctx->children.size() > 1) {
        auto lhs = visit(ctx->relationalExpr()[0]).as<llvm::Value*>();
        auto rhs = visit(ctx->relationalExpr()[1]).as<llvm::Value*>();

        // Equality expr is: relationalExpr EQUAL relationalExpr
        if (ctx->EQUAL()) return builder->CreateICmpEQ(lhs, rhs, "eq");

        // Equality expr is: relationalExpr NOT_EQUAL relationalExpr
        return builder->CreateICmpNE(lhs, rhs, "ne");
    }
    return visit(ctx->relationalExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext* ctx) {
    if (ctx->children.size() > 1) {
        auto lhs = visit(ctx->additiveExpr()[0]).as<llvm::Value*>();
        auto rhs = visit(ctx->additiveExpr()[1]).as<llvm::Value*>();

        // Relational expr is: additiveExpr LESS additiveExpr
        if (ctx->LESS()) return builder->CreateICmpSLT(lhs, rhs, "lt");

        // Relational expr is: additiveExpr GREATER additiveExpr
        if (ctx->GREATER()) return builder->CreateICmpSGT(lhs, rhs, "gt");

        // Relational expr is: additiveExpr LESS_EQUAL additiveExpr
        if (ctx->LESS_EQUAL()) return builder->CreateICmpSLE(lhs, rhs, "le");

        // Relational expr is: additiveExpr GREATER_EQUAL additiveExpr
        return builder->CreateICmpSGE(lhs, rhs, "ge");
    }
    return visit(ctx->additiveExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext* ctx) {
    if (ctx->multiplicativeExpr().size() > 1) {
        auto lhs = visit(ctx->multiplicativeExpr()[0]).as<llvm::Value*>();
        for (int i = 1; i < ctx->multiplicativeExpr().size(); i++) {
            auto rhs = visit(ctx->multiplicativeExpr()[i]).as<llvm::Value*>();
            if (ctx->PLUS(i-1))
                lhs = builder->CreateAdd(lhs, rhs, "add");
            else
                lhs = builder->CreateSub(lhs, rhs, "sub");
        }
        return lhs;
    }
    return visit(ctx->multiplicativeExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext* ctx) {
    if (ctx->prefixUnary().size() > 1) {
        auto lhs = visit(ctx->prefixUnary()[0]).as<llvm::Value*>();
        for (int i = 1; i < ctx->prefixUnary().size(); i++) {
            auto rhs = visit(ctx->prefixUnary()[i]).as<llvm::Value*>();
            if (ctx->MUL(i-1))
                lhs = builder->CreateMul(lhs, rhs, "mul");
            else
                lhs = builder->CreateSDiv(lhs, rhs, "div");
        }
        return lhs;
    }
    return visit(ctx->prefixUnary()[0]);
}

antlrcpp::Any GeneratorVisitor::visitPrefixUnary(SpiceParser::PrefixUnaryContext* ctx) {
    auto value = visit(ctx->postfixUnary());

    // Prefix unary is: PLUS_PLUS postfixUnary
    if (ctx->PLUS_PLUS()) {
        auto llvmValue = value.as<llvm::Value*>();
        auto rhs = builder->CreateAdd(llvmValue, builder->getInt32(1), "pre_pp");
        auto lhs = namedValues[ctx->postfixUnary()->atomicExpr()->value()->IDENTIFIER()->toString()];
        builder->CreateStore(rhs, lhs);
        return lhs;
    }

    // Prefix unary is: MINUS_MINUS postfixUnary
    if (ctx->MINUS_MINUS()) {
        auto llvmValue = value.as<llvm::Value*>();
        auto rhs = builder->CreateSub(llvmValue, builder->getInt32(1), "pre_mm");
        auto lhs = namedValues[ctx->postfixUnary()->atomicExpr()->value()->IDENTIFIER()->toString()];
        builder->CreateStore(rhs, lhs);
        return lhs;
    }

    // Prefix unary is: NOT postfixUnary
    if (ctx->NOT()) return builder->CreateNot(value.as<llvm::Value*>(), "not");

    return value;
}

antlrcpp::Any GeneratorVisitor::visitPostfixUnary(SpiceParser::PostfixUnaryContext* ctx) {
    auto value = visit(ctx->atomicExpr());

    // Postfix unary is: PLUS_PLUS atomicExpr
    if (ctx->PLUS_PLUS()) {
        auto llvmValue = value.as<llvm::Value*>();
        auto rhs = builder->CreateAdd(llvmValue, builder->getInt32(1), "post_pp");
        auto lhs = namedValues[ctx->atomicExpr()->value()->IDENTIFIER()->toString()];
        builder->CreateStore(rhs, lhs);
        return rhs;
    }

    // Postfix unary is: MINUS_MINUS atomicExpr
    if (ctx->MINUS_MINUS()) {
        auto llvmValue = value.as<llvm::Value*>();
        auto rhs = builder->CreateSub(llvmValue, builder->getInt32(1), "post_mm");
        auto lhs = namedValues[ctx->atomicExpr()->value()->IDENTIFIER()->toString()];
        builder->CreateStore(rhs, lhs);
        return rhs;
    }

    return value;
}

antlrcpp::Any GeneratorVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext* ctx) {
    // Atomic expr is: LPAREN value RPAREN
    if (ctx->LPAREN()) return visit(ctx->assignment()).as<llvm::Value*>();

    // Atomic expr is: value
    return visit(ctx->value());
}

antlrcpp::Any GeneratorVisitor::visitValue(SpiceParser::ValueContext* ctx) {
    // Value is a double constant
    if (ctx->DOUBLE()) {
        currentSymbolType = TYPE_DOUBLE;
        double value = std::stod(ctx->DOUBLE()->toString());
        return (llvm::Value*) llvm::ConstantFP::get(*context, llvm::APFloat(value));
    }

    // Value is an integer constant
    if (ctx->INTEGER()) {
        currentSymbolType = TYPE_INT;
        int value = std::stoi(ctx->INTEGER()->toString());
        return (llvm::Value*) llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), value);
    }

    // Value is a string constant
    if (ctx->STRING()) {
        currentSymbolType = TYPE_STRING;
        std::string value = ctx->STRING()->toString();
        auto charType = llvm::IntegerType::get(*context, 8);
        std::vector<llvm::Constant*> chars(value.size());
        for(unsigned int i = 0; i < value.size(); i++) chars[i] = llvm::ConstantInt::get(charType, value[i]);
        return (llvm::Value*) llvm::ConstantArray::get(llvm::ArrayType::get(charType, chars.size()), chars);
    }

    // Value is a boolean constant
    if (ctx->TRUE() || ctx->FALSE()) {
        currentSymbolType = TYPE_BOOL;
        bool value = ctx->TRUE() ? 1 : 0;
        return (llvm::Value*) llvm::ConstantInt::getSigned(llvm::Type::getInt1Ty(*context), value);
    }

    // Value is an identifier
    if (ctx->IDENTIFIER()) {
        std::string variableName = ctx->IDENTIFIER()->toString();
        currentSymbolType = currentScope->lookup(variableName)->getType();
        llvm::Value* var = namedValues[variableName];
        if (!var) throw std::runtime_error("Internal compiler error - Variable '" + variableName +
            "' not found in code generation step");
        return (llvm::Value*) builder->CreateLoad(var->getType()->getPointerElementType(), var);
    }

    // Value is a function call
    return visit(ctx->functionCall());
}

antlrcpp::Any GeneratorVisitor::visitDataType(SpiceParser::DataTypeContext* ctx) {
    // Data type is double
    if (ctx->TYPE_DOUBLE()) {
        currentSymbolType = TYPE_DOUBLE;
        return (llvm::Type*) llvm::Type::getDoubleTy(*context);
    }

    // Data type is int
    if (ctx->TYPE_INT()) {
        currentSymbolType = TYPE_INT;
        return (llvm::Type*) llvm::Type::getInt32Ty(*context);
    }

    // Data type is string
    if (ctx->TYPE_STRING()) {
        currentSymbolType = TYPE_STRING;
        return (llvm::Type*) llvm::Type::getInt8Ty(*context)->getPointerTo();
    }

    // Data type is bool
    if (ctx->TYPE_BOOL()) {
        currentSymbolType = TYPE_BOOL;
        return (llvm::Type*) llvm::Type::getInt1Ty(*context);
    }

    // Data type is dyn
    if (ctx->TYPE_DYN()) {
        auto symbolTableEntry = currentScope->lookup(currentVar);
        currentSymbolType = symbolTableEntry->getType();
        switch (currentSymbolType) {
            case TYPE_DOUBLE: return (llvm::Type*) llvm::Type::getDoubleTy(*context);
            case TYPE_INT: return (llvm::Type*) llvm::Type::getInt32Ty(*context);
            case TYPE_STRING: return (llvm::Type*) llvm::Type::getInt8Ty(*context)->getPointerTo();
            case TYPE_BOOL: return (llvm::Type*) llvm::Type::getInt1Ty(*context);
            default: throw IRError(UNEXPECTED_DYN_TYPE,
                                   "Dyn was " + std::to_string(symbolTableEntry->getType()));
        }
    }

    return (llvm::Type*) nullptr;
}

std::string GeneratorVisitor::getIRString() {
    std::string output;
    llvm::raw_string_ostream oss(output);
    module->print(oss, nullptr);
    return oss.str();
}

void GeneratorVisitor::initializeExternalFunctions() {
    module->getOrInsertFunction("printf", llvm::FunctionType::get(
            llvm::IntegerType::getInt32Ty(*context),
            llvm::Type::getInt8Ty(*context)->getPointerTo(), true));
}

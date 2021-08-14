// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "GeneratorVisitor.h"

void GeneratorVisitor::init(const std::string& sourceFileName) {
    // Create LLVM base components
    context = std::make_unique<llvm::LLVMContext>();
    builder = std::make_unique<llvm::IRBuilder<>>(*context);
    module = std::make_unique<llvm::Module>(sourceFileName, *context);

    // Initialize LLVM
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
}

void GeneratorVisitor::optimize(int optimizerLevel) {
    std::cout << "Optimizing with optLevel " + std::to_string(optimizerLevel) << " ..." << std::endl;

    // Declare map with all optimization passes in the required order
    llvm::Pass* passes[] = {
            llvm::createCFGSimplificationPass(),
            llvm::createSROAPass(),
            llvm::createEarlyCSEPass(),
            llvm::createPromoteMemoryToRegisterPass(),
            llvm::createInstructionCombiningPass(true),
            llvm::createCFGSimplificationPass(),
            llvm::createSROAPass(),
            llvm::createEarlyCSEPass(true),
            llvm::createSpeculativeExecutionIfHasBranchDivergencePass(),
            llvm::createJumpThreadingPass(),
            llvm::createCorrelatedValuePropagationPass(),
            llvm::createCFGSimplificationPass(),
            llvm::createInstructionCombiningPass(true),
            llvm::createLibCallsShrinkWrapPass(),
            llvm::createTailCallEliminationPass(),
            llvm::createCFGSimplificationPass(),
            llvm::createReassociatePass(),
            llvm::createLoopRotatePass(-1),
            llvm::createGVNPass(),
            llvm::createLICMPass(),
            llvm::createLoopUnswitchPass(),
            llvm::createCFGSimplificationPass(),
            llvm::createInstructionCombiningPass(true),
            llvm::createIndVarSimplifyPass(),
            llvm::createLoopIdiomPass(),
            llvm::createLoopDeletionPass(),
            llvm::createCFGSimplificationPass(),
            llvm::createSimpleLoopUnrollPass(optimizerLevel),
            llvm::createMergedLoadStoreMotionPass(),
            llvm::createGVNPass(),
            llvm::createMemCpyOptPass(),
            llvm::createSCCPPass(),
            llvm::createBitTrackingDCEPass(),
            llvm::createInstructionCombiningPass(true),
            llvm::createJumpThreadingPass(),
            llvm::createCorrelatedValuePropagationPass(),
            llvm::createDeadStoreEliminationPass(),
            llvm::createLICMPass(),
            llvm::createAggressiveDCEPass(),
            llvm::createCFGSimplificationPass(),
            llvm::createInstructionCombiningPass(true),
            llvm::createFloat2IntPass()
    };

    // Register optimization passes
    auto fpm = std::make_unique<llvm::legacy::FunctionPassManager>(module.get());
    for (llvm::Pass* pass : passes) fpm->add(pass);

    // Run optimizing passes for all functions
    fpm->doInitialization();
    for (llvm::Function* fct : functions) fpm->run(*fct);
}

void GeneratorVisitor::emit(std::string targetTriple, const std::string& outputPath) {
    // Configure output target
    if (targetTriple.empty()) targetTriple = llvm::sys::getDefaultTargetTriple();
    module->setTargetTriple(targetTriple);

    std::cout << "\nEmitting executable for triplet '" << targetTriple << "' to path: " << outputPath << std::endl;

    // Search after selected target
    std::string error;
    auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
    if (!target) throw IRError(TARGET_NOT_AVAILABLE, "Selected target was not found: " + error);

    std::string cpu = "generic";
    std::string features = "";

    llvm::TargetOptions opt;
    llvm::Optional rm = llvm::Optional<llvm::Reloc::Model>();
    auto targetMachine = target->createTargetMachine(targetTriple, cpu, features, opt, rm);

    module->setDataLayout(targetMachine->createDataLayout());

    // Open file output stream
    std::error_code errorCode;
    llvm::raw_fd_ostream dest(outputPath, errorCode, llvm::sys::fs::OF_None);
    if (errorCode) throw IRError(CANT_OPEN_OUTPUT_FILE, "File '" + outputPath + "' could not be opened");

    llvm::legacy::PassManager pass;
    auto fileType = llvm::CGFT_ObjectFile;
    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType))
        throw IRError(WRONG_TYPE, "Target machine can't emit a file of this type");

    // Emit object file
    pass.run(*module);
    dest.flush();
}

void GeneratorVisitor::dumpIR() {
    module->print(llvm::outs(), nullptr);
}

std::string GeneratorVisitor::getIRString() {
    std::string output;
    llvm::raw_string_ostream oss(output);
    module->print(oss, nullptr);
    return oss.str();
}

antlrcpp::Any GeneratorVisitor::visitEntry(SpiceParser::EntryContext* ctx) {
    // Generate code for external functions
    initializeExternalFunctions();

    auto result = SpiceBaseVisitor::visitEntry(ctx);

    // Verify module to detect IR code bugs
    std::string output;
    llvm::raw_string_ostream oss(output);
    if (llvm::verifyModule(*module, &oss)) throw IRError(*ctx->start, INVALID_MODULE, oss.str());

    return result;
}

antlrcpp::Any GeneratorVisitor::visitMainFunctionDef(SpiceParser::MainFunctionDefContext* ctx) {
    // Build function itself
    std::string functionName = "main";
    llvm::Type* returnType = llvm::IntegerType::getInt32Ty(*context);
    auto mainType = llvm::FunctionType::get(returnType, std::vector<llvm::Type*>(), false);
    auto fct = llvm::Function::Create(mainType, llvm::Function::ExternalLinkage, functionName, module.get());
    llvm::BasicBlock* bMain = llvm::BasicBlock::Create(*context, "main_entry");
    fct->getBasicBlockList().push_back(bMain);
    moveInsertPointToBlock(bMain);

    // Change scope
    namedValues.clear();
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->getChild(scopeId);

    // Declare result variable and set it to 0 for positive return code
    namedValues[RETURN_VARIABLE_NAME] = builder->CreateAlloca(returnType, nullptr, RETURN_VARIABLE_NAME);
    builder->CreateStore(llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), 0),
                         namedValues[RETURN_VARIABLE_NAME]);

    // Generate IR for function body
    visit(ctx->stmtLst());

    // Generate return statement for result variable
    if (!blockAlreadyTerminated) {
        llvm::Value* result = namedValues[RETURN_VARIABLE_NAME];
        builder->CreateRet(builder->CreateLoad(result->getType()->getPointerElementType(), result));
    }

    // Verify function
    std::string output;
    llvm::raw_string_ostream oss(output);
    if (llvm::verifyFunction(*fct, &oss)) throw IRError(*ctx->start, INVALID_FUNCTION, oss.str());

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
    llvm::Type* returnType = visit(ctx->dataType()).as<llvm::Type*>();

    // Create function itself
    std::vector<std::string> paramNames;
    std::vector<llvm::Type*> paramTypes;
    for (auto& param : ctx->paramLstDef()->declStmt()) { // Parameters without default value
        currentVar = param->IDENTIFIER()->toString();
        paramNames.push_back(currentVar);
        llvm::Type* paramType = visit(param->dataType()).as<llvm::Type*>();
        paramTypes.push_back(paramType);
    }
    for (auto& param : ctx->paramLstDef()->assignment()) { // Parameters with default value
        currentVar = param->declStmt()->IDENTIFIER()->toString();
        paramNames.push_back(currentVar);
        llvm::Type* paramType = visit(param->declStmt()->dataType()).as<llvm::Type*>();
        paramTypes.push_back(paramType);
    }
    auto fctType = llvm::FunctionType::get(returnType, paramTypes, false);
    auto fct = llvm::Function::Create(fctType, llvm::Function::ExternalLinkage, signature.toString(), module.get());

    // Create entry block
    llvm::BasicBlock* bEntry = llvm::BasicBlock::Create(*context, "entry");
    fct->getBasicBlockList().push_back(bEntry);
    moveInsertPointToBlock(bEntry);

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
    namedValues[RETURN_VARIABLE_NAME] = builder->CreateAlloca(returnType, nullptr, RETURN_VARIABLE_NAME);

    // Generate IR for function body
    visit(ctx->stmtLst());

    // Generate return statement for result variable
    if (!blockAlreadyTerminated) {
        llvm::Value* result = namedValues[RETURN_VARIABLE_NAME];
        builder->CreateRet(builder->CreateLoad(result->getType()->getPointerElementType(), result));
    }

    // Verify function
    std::string output;
    llvm::raw_string_ostream oss(output);
    if (llvm::verifyFunction(*fct, &oss)) throw IRError(*ctx->start, INVALID_FUNCTION, oss.str());

    // Add function to function list
    functions.push_back(fct);

    // Change scope back
    currentScope = currentScope->getParent();

    // Return true as result for the function definition
    return llvm::ConstantInt::get((llvm::Type::getInt1Ty(*context)), 1);
}

antlrcpp::Any GeneratorVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext* ctx) {
    std::string procedureName = ctx->IDENTIFIER()->toString();

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
    llvm::BasicBlock* bEntry = llvm::BasicBlock::Create(*context, "entry");
    proc->getBasicBlockList().push_back(bEntry);
    moveInsertPointToBlock(bEntry);

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
    std::string output;
    llvm::raw_string_ostream oss(output);
    if (llvm::verifyFunction(*proc, &oss)) throw IRError(*ctx->start, INVALID_FUNCTION, oss.str());

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
    llvm::BasicBlock* bCond = llvm::BasicBlock::Create(*context, "for_cond");
    llvm::BasicBlock* bLoop = llvm::BasicBlock::Create(*context, "for");
    llvm::BasicBlock* bLoopEnd = llvm::BasicBlock::Create(*context, "for_end");

    // Change scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->getChild(scopeId);
    currentScope->setContinueBlock(bCond);
    currentScope->setBreakBlock(bLoopEnd);

    // Execute pre-loop stmts
    visit(ctx->assignment()[0]);
    // Jump into condition block
    createBr(bCond);

    // Fill condition block
    parentFct->getBasicBlockList().push_back(bCond);
    moveInsertPointToBlock(bCond);
    llvm::Value* conditionValue = visit(ctx->assignment()[1]).as<llvm::Value*>();
    // Jump to loop body or to loop end
    createCondBr(conditionValue, bLoop, bLoopEnd);

    // Fill loop block
    parentFct->getBasicBlockList().push_back(bLoop);
    moveInsertPointToBlock(bLoop);
    // Generate IR for nested statements
    visit(ctx->stmtLst());
    // Run post-loop actions
    visit(ctx->assignment()[2]);
    // Jump into condition block
    createBr(bCond);

    // Fill loop end block
    parentFct->getBasicBlockList().push_back(bLoopEnd);
    moveInsertPointToBlock(bLoopEnd);

    // Change scope back
    currentScope = currentScope->getParent();

    // Return true as result for the loop
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context), 1);
}

antlrcpp::Any GeneratorVisitor::visitWhileLoop(SpiceParser::WhileLoopContext* ctx) {
    auto parentFct = builder->GetInsertBlock()->getParent();

    // Create blocks
    llvm::BasicBlock* bCond = llvm::BasicBlock::Create(*context, "while_cond");
    llvm::BasicBlock* bLoop = llvm::BasicBlock::Create(*context, "while");
    llvm::BasicBlock* bLoopEnd = llvm::BasicBlock::Create(*context, "while_end");

    // Change scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->getChild(scopeId);
    currentScope->setContinueBlock(bCond);
    currentScope->setBreakBlock(bLoopEnd);

    // Jump into condition block
    createBr(bCond);

    // Fill condition block
    parentFct->getBasicBlockList().push_back(bCond);
    moveInsertPointToBlock(bCond);
    llvm::Value* conditionValue = visit(ctx->assignment()).as<llvm::Value*>();
    createCondBr(conditionValue, bLoop, bLoopEnd);

    // Fill loop block
    parentFct->getBasicBlockList().push_back(bLoop);
    moveInsertPointToBlock(bLoop);
    // Generate IR for nested statements
    visit(ctx->stmtLst());
    // Jump into condition block
    createBr(bCond);

    // Fill loop end block
    parentFct->getBasicBlockList().push_back(bLoopEnd);
    moveInsertPointToBlock(bLoopEnd);

    // Change scope back
    currentScope = currentScope->getParent();

    // Return true as result for the loop
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context), 1);
}

antlrcpp::Any GeneratorVisitor::visitStmtLst(SpiceParser::StmtLstContext* ctx) {
    for (auto& child : ctx->children) {
        if (!blockAlreadyTerminated) visit(child);
    }
    return nullptr;
}

antlrcpp::Any GeneratorVisitor::visitIfStmt(SpiceParser::IfStmtContext* ctx) {
    llvm::Value* conditionValue = visit(ctx->assignment()).as<llvm::Value*>();
    auto parentFct = builder->GetInsertBlock()->getParent();

    // Create blocks
    llvm::BasicBlock* bThen = llvm::BasicBlock::Create(*context, "then");
    llvm::BasicBlock* bEnd = llvm::BasicBlock::Create(*context, "end");

    // Check if condition is fulfilled
    createCondBr(conditionValue, bThen, bEnd);

    // Change scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->getChild(scopeId);

    // Fill then block
    parentFct->getBasicBlockList().push_back(bThen);
    moveInsertPointToBlock(bThen);
    // Generate IR for nested statements
    visit(ctx->stmtLst());
    createBr(bEnd);

    // Fill end block
    parentFct->getBasicBlockList().push_back(bEnd);
    moveInsertPointToBlock(bEnd);

    // Change scope back
    currentScope = currentScope->getParent();

    // Return conditional value as result for the if stmt
    return conditionValue;
}

antlrcpp::Any GeneratorVisitor::visitDeclStmt(SpiceParser::DeclStmtContext* ctx) {
    currentVar = ctx->IDENTIFIER()->toString();
    llvm::Type* varType = visit(ctx->dataType()).as<llvm::Type*>();

    // Is this the global scope?
    if (!currentScope->getParent()) { // Global scope
        module->getOrInsertGlobal(currentVar, varType);
        llvm::GlobalVariable* global = module->getNamedGlobal(currentVar);
        global->setLinkage(llvm::GlobalValue::ExternalLinkage);
        global->setConstant(ctx->CONST());
        global->setAlignment(llvm::MaybeAlign(4));
        if (currentAssignValue) global->setInitializer((llvm::Constant*) currentAssignValue);
    } else { // Local scope
        namedValues[currentVar] = builder->CreateAlloca(varType, nullptr, currentVar);
    }
    return currentVar;
}

antlrcpp::Any GeneratorVisitor::visitFunctionCall(SpiceParser::FunctionCallContext* ctx) {
    std::string fctName = ctx->IDENTIFIER()->toString();

    // Get function by signature
    FunctionSignature signature = currentScope->popSignature();
    auto fct = module->getFunction(signature.toString());
    auto fctType = fct->getFunctionType();

    // Fill parameter list
    std::vector<llvm::Value*> argValues;
    if (ctx->paramLstCall()) {
        for (int i = 0; i < ctx->paramLstCall()->assignment().size(); i++) {
            llvm::Value* argValue = visit(ctx->paramLstCall()->assignment()[i]).as<llvm::Value*>();
            llvm::Type* argType = fctType->getParamType(i);
            llvm::Value* bitCastArgValue = builder->CreateBitCast(argValue, argType);
            argValues.push_back(bitCastArgValue);
        }
    }

    return (llvm::Value*) builder->CreateCall(fct, argValues);
}

antlrcpp::Any GeneratorVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {
    // Ignore sub-tree
    return nullptr;
}

antlrcpp::Any GeneratorVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext* ctx) {
    llvm::Value* returnValue = visit(ctx->assignment()).as<llvm::Value*>();
    // Build return value
    if (!blockAlreadyTerminated) {
        builder->CreateRet(returnValue);
        blockAlreadyTerminated = true;
    }
    return returnValue;
}

antlrcpp::Any GeneratorVisitor::visitBreakStmt(SpiceParser::BreakStmtContext* ctx) {
    // Get number, how many loops we want to break
    int breakCount = 1;
    if (ctx->INTEGER()) breakCount = std::stoi(ctx->INTEGER()->toString());

    // Get destination block
    SymbolTable* scope = currentScope;
    while (!scope->getBreakBlock()) scope = scope->getParent();
    for (int i = 1; i < breakCount; i++) {
        scope = scope->getParent();
        while (!scope->getBreakBlock()) scope = scope->getParent();
    }
    llvm::BasicBlock* destinationBlock = scope->getBreakBlock();

    // Jump to destination block
    createBr(destinationBlock);
    return nullptr;
}

antlrcpp::Any GeneratorVisitor::visitContinueStmt(SpiceParser::ContinueStmtContext* ctx) {
    // Get number, how many loops we want to continue
    int continueCount = 1;
    if (ctx->INTEGER()) continueCount = std::stoi(ctx->INTEGER()->toString());

    // Get destination block
    SymbolTable* scope = currentScope;
    while (!scope->getBreakBlock()) scope = scope->getParent();
    for (int i = 1; i < continueCount; i++) {
        scope = scope->getParent();
        while (!scope->getBreakBlock()) scope = scope->getParent();
    }
    llvm::BasicBlock* destinationBlock = scope->getContinueBlock();

    // Jump to destination block
    createBr(destinationBlock);
    return nullptr;
}

antlrcpp::Any GeneratorVisitor::visitPrintfStmt(SpiceParser::PrintfStmtContext* ctx) {
    auto printf = module->getFunction("printf");
    std::vector<llvm::Value*> printfArgs;
    std::string stringTemplate = ctx->STRING()->toString();
    stringTemplate = stringTemplate.substr(1, stringTemplate.size() - 2);
    printfArgs.push_back(builder->CreateGlobalStringPtr(stringTemplate));
    for (auto &arg : ctx->assignment()) {
        llvm::Value* argVal = visit(arg).as<llvm::Value*>();
        if (argVal == nullptr) throw IRError(*arg->start, PRINTF_NULL_TYPE, "'" + arg->getText() + "' is null");
        printfArgs.push_back(argVal);
    }
    return builder->CreateCall(printf, printfArgs);
}

antlrcpp::Any GeneratorVisitor::visitAssignment(SpiceParser::AssignmentContext* ctx) {
    if (ctx->declStmt() || ctx->IDENTIFIER()) {
        // Get value of right side
        llvm::Value* rhs = currentAssignValue = visit(ctx->ternary()).as<llvm::Value*>();

        // Get value of left side
        std::string varName = ctx->declStmt() ? visit(ctx->declStmt()).as<std::string>() : ctx->IDENTIFIER()->toString();
        bool isLocal = namedValues.find(varName) != namedValues.end();

        if (ctx->ASSIGN_OP()) {
            // Store right side on the left one
            if (isLocal) { // Local variable
                llvm::AllocaInst* lhs = namedValues[varName];
                builder->CreateStore(rhs, lhs);
            } else if (currentScope->getParent()) { // Global variable
                llvm::GlobalVariable* global = module->getNamedGlobal(varName);
                builder->CreateStore(rhs, global);
            }
        } else if (ctx->PLUS_EQUAL()) {
            if (isLocal) { // Local variable
                llvm::AllocaInst* lhs = namedValues[varName];
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = builder->CreateAdd(loadLhs, rhs, "ple");
                builder->CreateStore(rhs, lhs);
            } else { // Global variable
                llvm::GlobalVariable* lhs = module->getNamedGlobal(varName);
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = builder->CreateAdd(loadLhs, rhs, "ple");
                builder->CreateStore(rhs, lhs);
            }
        } else if (ctx->MINUS_EQUAL()) {
            if (isLocal) { // Local variable
                llvm::AllocaInst* lhs = namedValues[varName];
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = builder->CreateSub(loadLhs, rhs, "mie");
                builder->CreateStore(rhs, lhs);
            } else { // Global variable
                llvm::GlobalVariable* lhs = module->getNamedGlobal(varName);
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = builder->CreateSub(loadLhs, rhs, "mie");
                builder->CreateStore(rhs, lhs);
            }
        } else if (ctx->MUL_EQUAL()) {
            if (isLocal) { // Local variable
                llvm::AllocaInst* lhs = namedValues[varName];
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = builder->CreateMul(loadLhs, rhs, "mue");
                builder->CreateStore(rhs, lhs);
            } else { // Global variable
                llvm::GlobalVariable* lhs = module->getNamedGlobal(varName);
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = builder->CreateMul(loadLhs, rhs, "mue");
                builder->CreateStore(rhs, lhs);
            }
        } else if (ctx->DIV_EQUAL()) {
            if (isLocal) { // Local variable
                llvm::AllocaInst* lhs = namedValues[varName];
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = builder->CreateSDiv(loadLhs, rhs, "die");
                builder->CreateStore(rhs, lhs);
            } else { // Global variable
                llvm::GlobalVariable* lhs = module->getNamedGlobal(varName);
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = builder->CreateSDiv(loadLhs, rhs, "die");
                builder->CreateStore(rhs, lhs);
            }
        }
        // Return value of the right side
        return rhs;
    }
    return visit(ctx->ternary());
}

antlrcpp::Any GeneratorVisitor::visitTernary(SpiceParser::TernaryContext* ctx) {
    if (ctx->logicalOrExpr().size() > 1) {
        llvm::Value* conditionValue = visit(ctx->logicalOrExpr()[0]).as<llvm::Value*>();
        auto parentFct = builder->GetInsertBlock()->getParent();

        // Create blocks
        llvm::BasicBlock* bThen = llvm::BasicBlock::Create(*context, "then");
        llvm::BasicBlock* bElse = llvm::BasicBlock::Create(*context, "else");
        llvm::BasicBlock* bEnd = llvm::BasicBlock::Create(*context, "end");

        // Conditional jump to respective block
        createCondBr(conditionValue, bThen, bElse);

        // Fill then block
        parentFct->getBasicBlockList().push_back(bThen);
        moveInsertPointToBlock(bThen);
        llvm::Value* thenValue = visit(ctx->logicalOrExpr()[1]).as<llvm::Value*>();
        createBr(bEnd);

        // Fill else block
        parentFct->getBasicBlockList().push_back(bElse);
        moveInsertPointToBlock(bElse);
        llvm::Value* elseValue = visit(ctx->logicalOrExpr()[2]).as<llvm::Value*>();
        createBr(bEnd);

        // Fill end block
        parentFct->getBasicBlockList().push_back(bEnd);
        moveInsertPointToBlock(bEnd);
        // if either is void or their types don't match (which indicates one of them
        // returned the null value for void, then don't construct a phi node)
        if (thenValue->getType() == llvm::Type::getVoidTy(*context) ||
            elseValue->getType() == llvm::Type::getVoidTy(*context) ||
            thenValue->getType() != elseValue->getType()) {
            return llvm::Constant::getNullValue(llvm::Type::getInt32Ty(*context));
        }
        // Setup phi value
        llvm::PHINode* phi = builder->CreatePHI(thenValue->getType(), 2, "phi");
        phi->addIncoming(thenValue, bThen);
        phi->addIncoming(elseValue, bElse);
        return (llvm::Value*) phi;
    }
    return visit(ctx->logicalOrExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext* ctx) {
    if (ctx->logicalAndExpr().size() > 1) {
        llvm::Value* lhs = visit(ctx->logicalAndExpr()[0]).as<llvm::Value*>();
        for (int i = 1; i < ctx->logicalAndExpr().size(); i++) {
            llvm::Value* rhs = visit(ctx->logicalAndExpr()[i]).as<llvm::Value*>();
            lhs = builder->CreateLogicalOr(lhs, rhs, "log_or");
        }
        return lhs;
    }
    return visit(ctx->logicalAndExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext* ctx) {
    if (ctx->bitwiseOrExpr().size() > 1) {
        llvm::Value* lhs = visit(ctx->bitwiseOrExpr()[0]).as<llvm::Value*>();
        for (int i = 1; i < ctx->bitwiseOrExpr().size(); i++) {
            llvm::Value* rhs = visit(ctx->bitwiseOrExpr()[i]).as<llvm::Value*>();
            lhs = builder->CreateLogicalAnd(lhs, rhs, "log_and");
        }
        return lhs;
    }
    return visit(ctx->bitwiseOrExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext* ctx) {
    if (ctx->bitwiseAndExpr().size() > 1) {
        llvm::Value* lhs = visit(ctx->bitwiseAndExpr()[0]).as<llvm::Value*>();
        for (int i = 1; i < ctx->bitwiseAndExpr().size(); i++) {
            llvm::Value* rhs = visit(ctx->bitwiseAndExpr()[i]).as<llvm::Value*>();
            lhs = builder->CreateOr(lhs, rhs, "bw_or");
        }
        return lhs;
    }
    return visit(ctx->bitwiseAndExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext* ctx) {
    if (ctx->equalityExpr().size() > 1) {
        llvm::Value* lhs = visit(ctx->equalityExpr()[0]).as<llvm::Value*>();
        for (int i = 1; i < ctx->equalityExpr().size(); i++) {
            llvm::Value* rhs = visit(ctx->equalityExpr()[i]).as<llvm::Value*>();
            lhs = builder->CreateAnd(lhs, rhs, "bw_and");
        }
        return lhs;
    }
    return visit(ctx->equalityExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitEqualityExpr(SpiceParser::EqualityExprContext* ctx) {
    if (ctx->children.size() > 1) {
        llvm::Value* lhs = visit(ctx->relationalExpr()[0]).as<llvm::Value*>();
        llvm::Value* rhs = visit(ctx->relationalExpr()[1]).as<llvm::Value*>();

        // Equality expr is: relationalExpr EQUAL relationalExpr
        if (ctx->EQUAL()) {
            if (lhs->getType()->isFloatTy()) return builder->CreateFCmpOEQ(lhs, rhs, "eq");
            return builder->CreateICmpEQ(lhs, rhs, "eq");
        }

        // Equality expr is: relationalExpr NOT_EQUAL relationalExpr
        if (ctx->NOT_EQUAL()) {
            if (lhs->getType()->isFloatTy()) return builder->CreateFCmpONE(lhs, rhs, "ne");
            return builder->CreateICmpNE(lhs, rhs, "ne");
        }
    }
    return visit(ctx->relationalExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext* ctx) {
    if (ctx->children.size() > 1) {
        llvm::Value* lhs = visit(ctx->additiveExpr()[0]).as<llvm::Value*>();
        llvm::Value* rhs = visit(ctx->additiveExpr()[1]).as<llvm::Value*>();

        // Relational expr is: additiveExpr LESS additiveExpr
        if (ctx->LESS()) {
            if (lhs->getType()->isFloatTy()) return builder->CreateFCmpOLT(lhs, rhs, "lt");
            return builder->CreateICmpSLT(lhs, rhs, "lt");
        }

        // Relational expr is: additiveExpr GREATER additiveExpr
        if (ctx->GREATER()) {
            if (lhs->getType()->isFloatTy()) return builder->CreateFCmpOGT(lhs, rhs, "gt");
            return builder->CreateICmpSGT(lhs, rhs, "gt");
        }

        // Relational expr is: additiveExpr LESS_EQUAL additiveExpr
        if (ctx->LESS_EQUAL()) {
            if (lhs->getType()->isFloatTy()) return builder->CreateFCmpOLE(lhs, rhs, "le");
            return builder->CreateICmpSLE(lhs, rhs, "le");
        }

        // Relational expr is: additiveExpr GREATER_EQUAL additiveExpr
        if (ctx->GREATER_EQUAL()) {
            if (lhs->getType()->isFloatTy()) return builder->CreateFCmpOGE(lhs, rhs, "ge");
            return builder->CreateICmpSGE(lhs, rhs, "ge");
        }
    }
    return visit(ctx->additiveExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext* ctx) {
    if (ctx->multiplicativeExpr().size() > 1) {
        llvm::Value* lhs = visit(ctx->multiplicativeExpr()[0]).as<llvm::Value*>();
        for (int i = 1; i < ctx->multiplicativeExpr().size(); i++) {
            llvm::Value* rhs = visit(ctx->multiplicativeExpr()[i]).as<llvm::Value*>();
            if (lhs->getType()->isDoubleTy()) {
                if (rhs->getType()->isDoubleTy()) { // double + double / double - double
                    if (ctx->PLUS(i-1))
                        lhs = builder->CreateFAdd(lhs, rhs, "add");
                    else
                        lhs = builder->CreateFSub(lhs, rhs, "sub");
                } else if (rhs->getType()->isIntegerTy(32)) { // double + int / double - int
                    if (ctx->PLUS(i-1))
                        lhs = builder->CreateFAdd(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "add");
                    else
                        lhs = builder->CreateFSub(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "sub");
                } else if (rhs->getType()->isPointerTy()) { // double + string / double - string

                }
            } else if (lhs->getType()->isIntegerTy(32)) {
                if (rhs->getType()->isDoubleTy()) { // int + double / int - double
                    if (ctx->PLUS(i-1))
                        lhs = builder->CreateFAdd(builder->CreateSIToFP(rhs, rhs->getType()), rhs, "add");
                    else
                        lhs = builder->CreateFSub(builder->CreateSIToFP(rhs, rhs->getType()), rhs, "sub");
                } else if (rhs->getType()->isIntegerTy(32)) { // int + int / int - int
                    if (ctx->PLUS(i-1))
                        lhs = builder->CreateAdd(lhs, rhs, "add");
                    else
                        lhs = builder->CreateSub(lhs, rhs, "sub");
                } else if (rhs->getType()->isPointerTy()) { // int + string / int - string

                }
            } else if (lhs->getType()->isPointerTy()) {
                if (rhs->getType()->isDoubleTy()) { // string + double / string - double

                } else if (rhs->getType()->isIntegerTy(32)) { // string + int / string - int

                } else if (rhs->getType()->isPointerTy()) { // string + string / string - string

                }
            }
        }
        return lhs;
    }
    return visit(ctx->multiplicativeExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext* ctx) {
    if (ctx->prefixUnary().size() > 1) {
        llvm::Value* lhs = visit(ctx->prefixUnary()[0]).as<llvm::Value*>();
        for (int i = 1; i < ctx->prefixUnary().size(); i++) {
            llvm::Value* rhs = visit(ctx->prefixUnary()[i]).as<llvm::Value*>();
            if (lhs->getType()->isDoubleTy()) {
                if (rhs->getType()->isDoubleTy()) { // double * double / double : double
                    if (ctx->MUL(i-1))
                        lhs = builder->CreateFMul(lhs, rhs, "mul");
                    else
                        lhs = builder->CreateFDiv(lhs, rhs, "div");
                } else if (rhs->getType()->isIntegerTy(32)) { // double * int / double : int
                    if (ctx->MUL(i-1))
                        lhs = builder->CreateFMul(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "mul");
                    else
                        lhs = builder->CreateFDiv(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "div");
                }
            } else if (lhs->getType()->isIntegerTy(32)) {
                if (rhs->getType()->isDoubleTy()) { // int * double / int : double
                    if (ctx->MUL(i-1))
                        lhs = builder->CreateFMul(builder->CreateSIToFP(rhs, rhs->getType()), rhs, "mul");
                    else
                        lhs = builder->CreateFDiv(builder->CreateSIToFP(rhs, rhs->getType()), rhs, "div");
                } else if (rhs->getType()->isIntegerTy(32)) { // int * int / int : int
                    if (ctx->MUL(i-1))
                        lhs = builder->CreateMul(lhs, rhs, "mul");
                    else
                        lhs = builder->CreateSDiv(lhs, rhs, "div");
                } else if (rhs->getType()->isPointerTy()) { // int * string / int : string

                }
            } else if (lhs->getType()->isPointerTy()) {
                if (rhs->getType()->isIntegerTy(32)) { // string * int / string : int

                }
            }
        }
        return lhs;
    }
    return visit(ctx->prefixUnary()[0]);
}

antlrcpp::Any GeneratorVisitor::visitPrefixUnary(SpiceParser::PrefixUnaryContext* ctx) {
    auto value = visit(ctx->postfixUnary());

    // Prefix unary is: PLUS_PLUS postfixUnary
    if (ctx->PLUS_PLUS()) {
        llvm::Value* llvmValue = value.as<llvm::Value*>();
        auto rhs = builder->CreateAdd(llvmValue, builder->getInt32(1), "pre_pp");
        auto lhs = namedValues[ctx->postfixUnary()->atomicExpr()->value()->IDENTIFIER()->toString()];
        builder->CreateStore(rhs, lhs);
        return lhs;
    }

    // Prefix unary is: MINUS_MINUS postfixUnary
    if (ctx->MINUS_MINUS()) {
        llvm::Value* llvmValue = value.as<llvm::Value*>();
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
        llvm::Value* llvmValue = value.as<llvm::Value*>();
        auto rhs = builder->CreateAdd(llvmValue, builder->getInt32(1), "post_pp");
        auto lhs = namedValues[ctx->atomicExpr()->value()->IDENTIFIER()->toString()];
        builder->CreateStore(rhs, lhs);
        return rhs;
    }

    // Postfix unary is: MINUS_MINUS atomicExpr
    if (ctx->MINUS_MINUS()) {
        llvm::Value* llvmValue = value.as<llvm::Value*>();
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
        value = value.substr(1, value.size() - 2);
        /*initStringType();
        return (llvm::Value*) builder->CreateAlloca(stringType, );*/
        return (llvm::Value*) builder->CreateGlobalStringPtr(value);
    }

    // Value is a boolean constant
    if (ctx->TRUE() || ctx->FALSE()) {
        currentSymbolType = TYPE_BOOL;
        bool value = ctx->TRUE();
        return (llvm::Value*) llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context), value);
    }

    // Value is an identifier
    if (ctx->IDENTIFIER()) {
        std::string variableName = ctx->IDENTIFIER()->toString();
        currentSymbolType = currentScope->lookup(variableName)->getType();
        if (namedValues.find(variableName) != namedValues.end()) { // Local variable
            llvm::Value* var = namedValues[variableName];
            if (!var) throw IRError(*ctx->IDENTIFIER()->getSymbol(), VARIABLE_NOT_FOUND,
                                    "Variable '" + variableName +"' not found in code generation step");
            return (llvm::Value*) builder->CreateLoad(var->getType()->getPointerElementType(), var);
        } else { // Global variable
            llvm::GlobalVariable* var = module->getNamedGlobal(variableName);
            if (!var) throw IRError(*ctx->IDENTIFIER()->getSymbol(), VARIABLE_NOT_FOUND,
                                    "Variable '" + variableName +"' not found in code generation step");
            return (llvm::Value*) builder->CreateLoad(var->getType()->getPointerElementType(), var);
        }
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
        SymbolTableEntry* symbolTableEntry = currentScope->lookup(currentVar);
        currentSymbolType = symbolTableEntry->getType();
        switch (currentSymbolType) {
            case TYPE_DOUBLE: return (llvm::Type*) llvm::Type::getDoubleTy(*context);
            case TYPE_INT: return (llvm::Type*) llvm::Type::getInt32Ty(*context);
            case TYPE_STRING: return (llvm::Type*) llvm::Type::getInt8Ty(*context)->getPointerTo();
            case TYPE_BOOL: return (llvm::Type*) llvm::Type::getInt1Ty(*context);
            default: throw IRError(*ctx->TYPE_DYN()->getSymbol(), UNEXPECTED_DYN_TYPE,
                                   "Dyn was " + std::to_string(symbolTableEntry->getType()));
        }
    }

    return (llvm::Type*) nullptr;
}

void GeneratorVisitor::initializeExternalFunctions() {
    module->getOrInsertFunction("printf", llvm::FunctionType::get(
            llvm::IntegerType::getInt32Ty(*context),
            llvm::Type::getInt8Ty(*context)->getPointerTo(), true));
}

/*void GeneratorVisitor::initStringType() {
    if (!stringType) {
        // Create string type
        llvm::ArrayRef<llvm::Type*> types = {
                llvm::PointerType::getInt8Ty(*context), // Pointer to the char buffer
                llvm::Type::getInt32Ty(*context), // Length in chars
                llvm::Type::getInt32Ty(*context), // Max length in chars
                llvm::Type::getInt32Ty(*context) // Growth factor to preallocate when growing
        };
        stringType = llvm::StructType::create(*context, types, "string", false);


    }
}*/

void GeneratorVisitor::createBr(llvm::BasicBlock* targetBlock) {
    if (!blockAlreadyTerminated) {
        builder->CreateBr(targetBlock);
        blockAlreadyTerminated = true;
    }
}

void GeneratorVisitor::createCondBr(llvm::Value* condition, llvm::BasicBlock* trueBlock, llvm::BasicBlock* falseBlock) {
    if (!blockAlreadyTerminated) {
        builder->CreateCondBr(condition, trueBlock, falseBlock);
        blockAlreadyTerminated = true;
    }
}

void GeneratorVisitor::moveInsertPointToBlock(llvm::BasicBlock* block) {
    builder->SetInsertPoint(block);
    blockAlreadyTerminated = false;
}

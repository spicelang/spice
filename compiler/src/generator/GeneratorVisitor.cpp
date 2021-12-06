// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "GeneratorVisitor.h"

void GeneratorVisitor::init() {
    // Create LLVM base components
    context = std::make_unique<llvm::LLVMContext>();
    builder = std::make_unique<llvm::IRBuilder<>>(*context);
    module = std::make_unique<llvm::Module>(FileUtil::getFileName(mainSourceFile), *context);

    // Initialize LLVM
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
}

void GeneratorVisitor::optimize() {
    if (debugOutput) std::cout << "\nOptimizing on level " + std::to_string(optLevel) << " ..." << std::endl;

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
            llvm::createSimpleLoopUnrollPass(optLevel),
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
    std::unique_ptr<llvm::legacy::FunctionPassManager> fpm =
            std::make_unique<llvm::legacy::FunctionPassManager>(module.get());
    for (llvm::Pass* pass : passes) fpm->add(pass);

    // Run optimizing passes for all functions
    fpm->doInitialization();
    for (llvm::Function* fct : functions) fpm->run(*fct);
}

void GeneratorVisitor::emit() {
    // Configure output target
    std::string tripletString = targetTriple.getTriple();
    module->setTargetTriple(tripletString);

    if (debugOutput)
        std::cout << "\nEmitting executable for triplet '" << tripletString << "' to path: " << objectDir << std::endl;

    // Search after selected target
    std::string error;
    const llvm::Target* target = llvm::TargetRegistry::lookupTarget(tripletString, error);
    if (!target) throw IRError(TARGET_NOT_AVAILABLE, "Selected target was not found: " + error);

    std::string cpu = "generic";

    llvm::TargetOptions opt;
    llvm::Optional rm = llvm::Optional<llvm::Reloc::Model>();
    llvm::TargetMachine* targetMachine = target->createTargetMachine(tripletString, cpu, "", opt, rm);

    module->setDataLayout(targetMachine->createDataLayout());

    // Open file output stream
    std::error_code errorCode;
    llvm::raw_fd_ostream dest(objectDir, errorCode, llvm::sys::fs::OF_None);
    if (errorCode) throw IRError(CANT_OPEN_OUTPUT_FILE, "File '" + objectDir + "' could not be opened");

    llvm::legacy::PassManager pass;
    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CGFT_ObjectFile))
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

    antlrcpp::Any result = SpiceBaseVisitor::visitEntry(ctx);

    // Verify module to detect IR code bugs
    std::string output;
    llvm::raw_string_ostream oss(output);
    if (llvm::verifyModule(*module, &oss)) throw IRError(*ctx->start, INVALID_MODULE, oss.str());

    return result;
}

antlrcpp::Any GeneratorVisitor::visitMainFunctionDef(SpiceParser::MainFunctionDefContext* ctx) {
    if (mustHaveMainFunction) { // Only create main function when it is required
        // Build function itself
        std::string functionName = "main";
        llvm::Type* returnType = llvm::Type::getInt32Ty(*context);
        llvm::FunctionType* fctType = llvm::FunctionType::get(returnType, std::vector<llvm::Type*>(), false);
        llvm::Function* fct = llvm::Function::Create(fctType, llvm::Function::ExternalLinkage,
                                                     functionName, module.get());
        llvm::BasicBlock* bMain = llvm::BasicBlock::Create(*context, "entry");
        fct->getBasicBlockList().push_back(bMain);
        moveInsertPointToBlock(bMain);

        // Change scope
        std::string scopeId = ScopeIdUtil::getScopeId(ctx);
        currentScope = currentScope->getChild(scopeId);

        // Declare result variable and set it to 0 for positive return code
        llvm::Value* memAddress = builder->CreateAlloca(returnType, nullptr, RETURN_VARIABLE_NAME);
        currentScope->lookup(RETURN_VARIABLE_NAME)->updateAddress(memAddress);
        currentScope->lookup(RETURN_VARIABLE_NAME)->updateLLVMType(returnType);
        builder->CreateStore(builder->getInt32(0), currentScope->lookup(RETURN_VARIABLE_NAME)->getAddress());

        // Generate IR for function body
        visit(ctx->stmtLst());

        // Generate return statement for result variable
        if (!blockAlreadyTerminated) {
            llvm::Value* result = currentScope->lookup(RETURN_VARIABLE_NAME)->getAddress();
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
        return (llvm::Value*) builder->getTrue();
    }
    // Return false as result for the function definition
    return (llvm::Value*) builder->getFalse();
}

antlrcpp::Any GeneratorVisitor::visitFunctionDef(SpiceParser::FunctionDefContext* ctx) {
    std::string functionName = ctx->IDENTIFIER()->toString();

    // Change scope
    FunctionSignature signature = currentScope->popSignature();
    currentScope = currentScope->getChild(signature.toString());

    // Get return type
    currentVar = RETURN_VARIABLE_NAME;
    llvm::Type* returnType = visit(ctx->dataType()).as<llvm::Type*>();
    std::vector<SymbolType> symbolTypes;
    symbolTypes.push_back(currentSymbolType);

    // Create function itself
    std::vector<std::string> paramNames;
    std::vector<llvm::Type*> paramTypes;
    if (ctx->paramLstDef()) {
        for (auto& param : ctx->paramLstDef()->declStmt()) { // Parameters without default value
            currentVar = param->IDENTIFIER()->toString();
            paramNames.push_back(currentVar);
            llvm::Type* paramType = visit(param->dataType()).as<llvm::Type*>();
            paramTypes.push_back(paramType);
            symbolTypes.push_back(currentSymbolType);
        }
        for (auto& param : ctx->paramLstDef()->assignExpr()) { // Parameters with default value
            currentVar = param->declStmt()->IDENTIFIER()->toString();
            paramNames.push_back(currentVar);
            llvm::Type* paramType = visit(param->declStmt()->dataType()).as<llvm::Type*>();
            paramTypes.push_back(paramType);
            symbolTypes.push_back(currentSymbolType);
        }
    }
    llvm::FunctionType* fctType = llvm::FunctionType::get(returnType, paramTypes, false);
    llvm::Function* fct = llvm::Function::Create(fctType, llvm::Function::ExternalLinkage,
                                                 signature.toString(), module.get());

    // Create entry block
    llvm::BasicBlock* bEntry = llvm::BasicBlock::Create(*context, "entry");
    fct->getBasicBlockList().push_back(bEntry);
    moveInsertPointToBlock(bEntry);

    // Store function params
    for (auto& param : fct->args()) {
        unsigned paramNo = param.getArgNo();
        if (paramNo < ctx->paramLstDef()->declStmt().size()) {
            std::string paramName = paramNames[paramNo];
            llvm::Type* paramType = fct->getFunctionType()->getParamType(paramNo);
            llvm::Value* memAddress = builder->CreateAlloca(paramType, nullptr, paramName);
            currentScope->lookup(paramName)->updateAddress(memAddress);
            currentScope->lookup(paramName)->updateLLVMType(paramType);
            builder->CreateStore(&param, memAddress);
        } else {
            std::string paramName = paramNames[paramNo];
            llvm::Type* paramType = fct->getFunctionType()->getParamType(paramNo);
            llvm::Value* memAddress = builder->CreateAlloca(paramType, nullptr, paramName);
            currentScope->lookup(paramName)->updateAddress(memAddress);
            currentScope->lookup(paramName)->updateLLVMType(paramType);
            builder->CreateStore(&param, memAddress);
        }
    }

    // Declare result variable
    llvm::Value* returnMemAddress = builder->CreateAlloca(returnType, nullptr, RETURN_VARIABLE_NAME);
    currentScope->lookup(RETURN_VARIABLE_NAME)->updateAddress(returnMemAddress);
    currentScope->lookup(RETURN_VARIABLE_NAME)->updateLLVMType(returnType);

    // Generate IR for function body
    visit(ctx->stmtLst());

    // Generate return statement for result variable
    if (!blockAlreadyTerminated) {
        llvm::Value* result = currentScope->lookup(RETURN_VARIABLE_NAME)->getAddress();
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

    // Insert function declaration to symbol table
    currentScope->insertFunctionDeclaration(signature.toString(), symbolTypes);

    // Return true as result for the function definition
    return (llvm::Value*) builder->getTrue();
}

antlrcpp::Any GeneratorVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext* ctx) {
    std::string procedureName = ctx->IDENTIFIER()->toString();

    // Change scope
    FunctionSignature signature = currentScope->popSignature();
    currentScope = currentScope->getChild(signature.toString());

    // Create procedure itself
    std::vector<std::string> paramNames;
    std::vector<llvm::Type*> paramTypes;
    std::vector<SymbolType> symbolTypes;
    if (ctx->paramLstDef()) {
        for (auto& param : ctx->paramLstDef()->declStmt()) { // Parameters without default value
            currentVar = param->IDENTIFIER()->toString();
            paramNames.push_back(currentVar);
            llvm::Type* paramType = visit(param->dataType()).as<llvm::Type*>();
            paramTypes.push_back(paramType);
            symbolTypes.push_back(currentSymbolType);
        }
        for (auto& param : ctx->paramLstDef()->assignExpr()) { // Parameters with default value
            currentVar = param->declStmt()->IDENTIFIER()->toString();
            paramNames.push_back(currentVar);
            llvm::Type* paramType = visit(param->declStmt()->dataType()).as<llvm::Type*>();
            paramTypes.push_back(paramType);
            symbolTypes.push_back(currentSymbolType);
        }
    }
    llvm::FunctionType* procType = llvm::FunctionType::get(llvm::Type::getVoidTy(*context),
                                                           paramTypes, false);
    llvm::Function* proc = llvm::Function::Create(procType, llvm::Function::ExternalLinkage,
                                                  signature.toString(), module.get());

    // Create entry block
    llvm::BasicBlock* bEntry = llvm::BasicBlock::Create(*context, "entry");
    proc->getBasicBlockList().push_back(bEntry);
    moveInsertPointToBlock(bEntry);

    // Store procedure params
    for (auto& param : proc->args()) {
        unsigned paramNo = param.getArgNo();
        if (paramNo < ctx->paramLstDef()->declStmt().size()) {
            std::string paramName = paramNames[paramNo];
            llvm::Type* paramType = proc->getFunctionType()->getParamType(paramNo);
            llvm::Value* memAddress = builder->CreateAlloca(paramType, nullptr, paramName);
            currentScope->lookup(paramName)->updateAddress(memAddress);
            currentScope->lookup(paramName)->updateLLVMType(paramType);
            builder->CreateStore(&param, memAddress);
        } else {
            std::string paramName = paramNames[paramNo];
            llvm::Type* paramType = proc->getFunctionType()->getParamType(paramNo);
            llvm::Value* memAddress = builder->CreateAlloca(paramType, nullptr, paramName);
            currentScope->lookup(paramName)->updateAddress(memAddress);
            currentScope->lookup(paramName)->updateLLVMType(paramType);
            builder->CreateStore(&param, memAddress);
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

    // Insert function declaration to symbol table
    currentScope->insertProcedureDeclaration(signature.toString(), symbolTypes);

    // Return true as result for the function definition
    return (llvm::Value*) builder->getTrue();
}

antlrcpp::Any GeneratorVisitor::visitStructDef(SpiceParser::StructDefContext* ctx) {
    std::string structName = ctx->IDENTIFIER()->toString();

    // Collect member types
    std::vector<llvm::Type*> memberTypes;
    for (auto& field : ctx->fieldLst()->declStmt()) {
        llvm::Type* memberType = visit(field->dataType()).as<llvm::Type*>();
        memberTypes.push_back(memberType);
    }

    // Create global struct
    llvm::StructType* structType = llvm::StructType::create(*context, memberTypes, structName);
    currentScope->lookup(structName)->updateLLVMType(structType);

    // Return true as result for the struct definition
    return (llvm::Value*) builder->getTrue();
}

antlrcpp::Any GeneratorVisitor::visitGlobalVarDef(SpiceParser::GlobalVarDefContext* ctx) {
    std::string varName = ctx->IDENTIFIER()->toString();
    llvm::Type* varType = visit(ctx->dataType()).as<llvm::Type*>();

    // Create global variable
    llvm::Value* memAddress = module->getOrInsertGlobal(varName, varType);
    currentScope->lookup(varName)->updateAddress(memAddress);
    currentScope->lookup(varName)->updateLLVMType(varType);
    // Set some attributes to it
    llvm::GlobalVariable* global = module->getNamedGlobal(varName);
    //global->setLinkage(llvm::GlobalValue::ExternalWeakLinkage);
    global->setConstant(ctx->CONST());
    global->setDSOLocal(true);

    if (ctx->value()) { // Variable is initialized here
        llvm::Value* value = visit(ctx->value()).as<llvm::Value*>();
        global->setInitializer((llvm::Constant*) value);
    }

    // Return true as result for the global variable definition
    return (llvm::Value*) builder->getTrue();
}

antlrcpp::Any GeneratorVisitor::visitForLoop(SpiceParser::ForLoopContext* ctx) {
    llvm::Function* parentFct = builder->GetInsertBlock()->getParent();

    // Create blocks
    llvm::BasicBlock* bCond = llvm::BasicBlock::Create(*context, "for.cond");
    llvm::BasicBlock* bLoop = llvm::BasicBlock::Create(*context, "for");
    llvm::BasicBlock* bEnd = llvm::BasicBlock::Create(*context, "for.end");

    // Change scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->getChild(scopeId);
    currentScope->setContinueBlock(bCond);
    currentScope->setBreakBlock(bEnd);

    // Execute pre-loop stmts
    visit(ctx->assignExpr()[0]);
    // Jump into condition block
    createBr(bCond);

    // Fill condition block
    parentFct->getBasicBlockList().push_back(bCond);
    moveInsertPointToBlock(bCond);
    llvm::Value* condValuePtr = visit(ctx->assignExpr()[1]).as<llvm::Value*>();
    llvm::Value* condValue = builder->CreateLoad(condValuePtr->getType()->getPointerElementType(), condValuePtr);
    // Jump to loop body or to loop end
    createCondBr(condValue, bLoop, bEnd);

    // Fill loop block
    parentFct->getBasicBlockList().push_back(bLoop);
    moveInsertPointToBlock(bLoop);
    // Generate IR for nested statements
    visit(ctx->stmtLst());
    // Run post-loop actions
    visit(ctx->assignExpr()[2]);
    // Jump into condition block
    createBr(bCond);

    // Fill loop end block
    parentFct->getBasicBlockList().push_back(bEnd);
    moveInsertPointToBlock(bEnd);

    // Change scope back
    currentScope = currentScope->getParent();

    // Return true as result for the loop
    return (llvm::Value*) builder->getTrue();
}

antlrcpp::Any GeneratorVisitor::visitForeachLoop(SpiceParser::ForeachLoopContext* ctx) {
    llvm::Function* parentFct = builder->GetInsertBlock()->getParent();
    bool hasIndexVariable = ctx->foreachHead()->declStmt().size() >= 2 || ctx->foreachHead()->assignExpr().size() >= 2;

    // Create blocks
    llvm::BasicBlock* bLoop = llvm::BasicBlock::Create(*context, "foreach.loop");
    llvm::BasicBlock* bInc = llvm::BasicBlock::Create(*context, "foreach.inc");
    llvm::BasicBlock* bEnd = llvm::BasicBlock::Create(*context, "foreach.end");

    // Change scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->getChild(scopeId);
    currentScope->setContinueBlock(bInc);
    currentScope->setBreakBlock(bEnd);

    // Initialize loop variables
    llvm::Value* indexVariablePtr;
    if (ctx->foreachHead()->declStmt().size() >= 2) { // declStmt COMMA declStmt COLON assignExpr
        std::string indexVariableName = visit(ctx->foreachHead()->declStmt().front()).as<std::string>();
        indexVariablePtr = currentScope->lookup(indexVariableName)->getAddress();
        // Initialize variable with 0
        builder->CreateStore(builder->getInt32(0), indexVariablePtr);
    } else if (ctx->foreachHead()->assignExpr().size() >= 2) { // assignExpr COMMA declStmt COLON assignExpr
        visit(ctx->foreachHead()->assignExpr().front()).as<llvm::Value*>();
        std::string indexVariableName = ctx->foreachHead()->assignExpr().front()->declStmt()->IDENTIFIER()->toString();
        indexVariablePtr = currentScope->lookup(indexVariableName)->getAddress();
    } else { // declStmt COLON assignExpr
        std::string indexVariableName = FOREACH_DEFAULT_IDX_VARIABLE_NAME;
        // Create local variable for
        llvm::Type* indexVariableType = llvm::Type::getInt32Ty(*context);
        indexVariablePtr = builder->CreateAlloca(indexVariableType, nullptr, indexVariableName);
        SymbolTableEntry* entry = currentScope->lookup(indexVariableName);
        entry->updateAddress(indexVariablePtr);
        entry->updateLLVMType(indexVariableType);
        entry->setUsed();
        // Initialize variable with 0
        builder->CreateStore(builder->getInt32(0), indexVariablePtr);
    }
    std::string itemVariableName = visit(ctx->foreachHead()->declStmt().back()).as<std::string>();
    llvm::Value* itemVariablePtr = currentScope->lookup(itemVariableName)->getAddress();

    // Do loop variable initialization
    llvm::Value* arrayValuePtr = visit(ctx->foreachHead()->assignExpr().back()).as<llvm::Value*>();
    llvm::Value* arrayValue = builder->CreateLoad(arrayValuePtr->getType()->getPointerElementType(), arrayValuePtr);
    llvm::Value* maxIndex = builder->getInt32(arrayValue->getType()->getArrayNumElements() -1);
    // Load the first item into item variable
    llvm::Value* indexValue = builder->CreateLoad(indexVariablePtr->getType()->getPointerElementType(), indexVariablePtr);
    llvm::Value* gepInst = builder->CreateInBoundsGEP(arrayValuePtr->getType()->getPointerElementType(),
                                                      arrayValuePtr, { builder->getInt32(0), indexValue });
    llvm::Value* newItemValue = builder->CreateLoad(gepInst->getType()->getPointerElementType(), gepInst);
    builder->CreateStore(newItemValue, itemVariablePtr);
    createBr(bLoop);

    // Fill loop block
    parentFct->getBasicBlockList().push_back(bLoop);
    moveInsertPointToBlock(bLoop);
    // Generate IR for nested statements
    visit(ctx->stmtLst());
    // Check if the index variable reached the size -2
    indexValue = builder->CreateLoad(indexVariablePtr->getType()->getPointerElementType(), indexVariablePtr);
    llvm::Value* cond = builder->CreateICmpSLT(indexValue, maxIndex, "foreach_idx_cmp");
    createCondBr(cond, bInc, bEnd);

    // Fill inc block
    parentFct->getBasicBlockList().push_back(bInc);
    moveInsertPointToBlock(bInc);
    // Increment index variable
    indexValue = builder->CreateLoad(indexVariablePtr->getType()->getPointerElementType(), indexVariablePtr);
    indexValue = builder->CreateAdd(indexValue, builder->getInt32(1), "foreach_idx_inc");
    builder->CreateStore(indexValue, indexVariablePtr);
    // Load new item into item variable
    gepInst = builder->CreateInBoundsGEP(arrayValuePtr->getType()->getPointerElementType(),
                                         arrayValuePtr, { builder->getInt32(0), indexValue });
    newItemValue = builder->CreateLoad(gepInst->getType()->getPointerElementType(), gepInst);
    builder->CreateStore(newItemValue, itemVariablePtr);
    createBr(bLoop);

    // Fill loop end block
    parentFct->getBasicBlockList().push_back(bEnd);
    moveInsertPointToBlock(bEnd);

    // Change scope back
    currentScope = currentScope->getParent();

    // Return true as result for the loop
    return (llvm::Value*) builder->getTrue();
}

antlrcpp::Any GeneratorVisitor::visitWhileLoop(SpiceParser::WhileLoopContext* ctx) {
    llvm::Function* parentFct = builder->GetInsertBlock()->getParent();

    // Create blocks
    llvm::BasicBlock* bCond = llvm::BasicBlock::Create(*context, "while.cond");
    llvm::BasicBlock* bLoop = llvm::BasicBlock::Create(*context, "while");
    llvm::BasicBlock* bLoopEnd = llvm::BasicBlock::Create(*context, "while.end");

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
    llvm::Value* condValuePtr = visit(ctx->assignExpr()).as<llvm::Value*>();
    llvm::Value* condValue = builder->CreateLoad(condValuePtr->getType()->getPointerElementType(), condValuePtr);
    createCondBr(condValue, bLoop, bLoopEnd);

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
    return (llvm::Value*) builder->getTrue();
}

antlrcpp::Any GeneratorVisitor::visitStmtLst(SpiceParser::StmtLstContext* ctx) {
    for (auto& child : ctx->children) {
        if (!blockAlreadyTerminated) visit(child);
    }
    return nullptr;
}

antlrcpp::Any GeneratorVisitor::visitIfStmt(SpiceParser::IfStmtContext* ctx) {
    // Change scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->getChild(scopeId);

    llvm::Value* condValuePtr = visit(ctx->assignExpr()).as<llvm::Value*>();
    llvm::Value* condValue = builder->CreateLoad(condValuePtr->getType()->getPointerElementType(), condValuePtr);
    llvm::Function* parentFct = builder->GetInsertBlock()->getParent();

    // Create blocks
    llvm::BasicBlock* bThen = llvm::BasicBlock::Create(*context, "if.then");
    llvm::BasicBlock* bElse = llvm::BasicBlock::Create(*context, "if.else");
    llvm::BasicBlock* bEnd = llvm::BasicBlock::Create(*context, "if.end");

    // Check if condition is fulfilled
    createCondBr(condValue, bThen, ctx->elseStmt() ? bElse : bEnd);

    // Fill then block
    parentFct->getBasicBlockList().push_back(bThen);
    moveInsertPointToBlock(bThen);
    // Generate IR for nested statements
    visit(ctx->stmtLst());
    createBr(bEnd);

    // Change scope back
    currentScope = currentScope->getParent();

    // Fill else block
    if (ctx->elseStmt()) {
        parentFct->getBasicBlockList().push_back(bElse);
        moveInsertPointToBlock(bElse);
        visit(ctx->elseStmt()); // Generate IR for else block
        createBr(bEnd);
    }

    // Fill end block
    parentFct->getBasicBlockList().push_back(bEnd);
    moveInsertPointToBlock(bEnd);

    // Return conditional value as result for the if stmt
    return condValue;
}

antlrcpp::Any GeneratorVisitor::visitElseStmt(SpiceParser::ElseStmtContext* ctx) {
    if (ctx->ifStmt()) { // It is an else if branch
        visit(ctx->ifStmt());
    } else { // It is an else branch
        // Change scope
        std::string scopeId = ScopeIdUtil::getScopeId(ctx);
        currentScope = currentScope->getChild(scopeId);

        // Generate IR for nested statements
        visit(ctx->stmtLst());

        // Change scope back
        currentScope = currentScope->getParent();
    }
    return nullptr;
}

antlrcpp::Any GeneratorVisitor::visitDeclStmt(SpiceParser::DeclStmtContext* ctx) {
    currentVar = ctx->IDENTIFIER()->toString();
    llvm::Type* varType = visit(ctx->dataType()).as<llvm::Type*>();

    // Create local variable
    llvm::Value* memAddress = builder->CreateAlloca(varType, nullptr, currentVar);
    SymbolTableEntry* entry = currentScope->lookup(currentVar);
    entry->updateAddress(memAddress);
    entry->updateLLVMType(varType);

    // Return the variable name
    return currentVar;
}

antlrcpp::Any GeneratorVisitor::visitFunctionCall(SpiceParser::FunctionCallContext* ctx) {
    std::vector<std::string> functionNamespace;
    for (auto& segment : ctx->IDENTIFIER()) functionNamespace.push_back(segment->toString());
    std::string functionName = functionNamespace.back();

    // Get function by signature
    FunctionSignature signature = currentScope->popSignature();
    functionNamespace.back() = signature.toString();
    // Check if function exists in module
    bool functionFound = false;
    for (auto& function : module->getFunctionList()) {
        if (function.getName() == signature.toString()) {
            functionFound = true;
            break;
        }
    }
    if (!functionFound) { // Not found => Declare function, which will be linked to later
        SymbolTable* table = currentScope->lookupTableWithSymbol(functionNamespace);
        // Check if it is a function or a procedure
        if (!table->getFunctionDeclaration(signature.toString()).empty()) {
            std::vector<SymbolType> symbolTypes = table->getFunctionDeclaration(signature.toString());

            // Get return type
            llvm::Type* returnType = getTypeForSymbolType(symbolTypes[0]);
            if (!returnType) throw std::runtime_error("Internal error");

            // Get parameter types
            std::vector<llvm::Type*> paramTypes;
            for (int i = 1; i < symbolTypes.size(); i++) {
                llvm::Type* paramType = getTypeForSymbolType(symbolTypes[i]);
                if (!paramType) throw std::runtime_error("Internal error");
                paramTypes.push_back(paramType);
            }

            llvm::FunctionType* fctType = llvm::FunctionType::get(returnType, paramTypes, false);
            module->getOrInsertFunction(signature.toString(), fctType);
        } else if (!table->getProcedureDeclaration(signature.toString()).empty()) {
            std::vector<SymbolType> symbolTypes = table->getProcedureDeclaration(signature.toString());

            // Get parameter types
            std::vector<llvm::Type*> paramTypes;
            for (int i = 1; i < symbolTypes.size(); i++) {
                llvm::Type* paramType = getTypeForSymbolType(symbolTypes[i]);
                if (!paramType) throw std::runtime_error("Internal error");
                paramTypes.push_back(paramType);
            }

            llvm::FunctionType* procType = llvm::FunctionType::get(llvm::Type::getVoidTy(*context),
                                                                   paramTypes, false);
            module->getOrInsertFunction(signature.toString(), procType);
        }
    }
    llvm::Function* fct = module->getFunction(signature.toString());
    llvm::FunctionType* fctType = fct->getFunctionType();

    // Fill parameter list
    std::vector<llvm::Value*> argValues;
    if (ctx->paramLst()) {
        for (int i = 0; i < ctx->paramLst()->assignExpr().size(); i++) {
            llvm::Value* argValuePtr = visit(ctx->paramLst()->assignExpr()[i]).as<llvm::Value*>();
            llvm::Value* argValue = builder->CreateLoad(argValuePtr->getType()->getPointerElementType(), argValuePtr);
            llvm::Type* argType = fctType->getParamType(i);
            llvm::Value* bitCastArgValue = builder->CreateBitCast(argValue, argType);
            argValues.push_back(bitCastArgValue);
        }
    }

    llvm::Value* callResult = builder->CreateCall(fct, argValues);
    if (callResult->getType()->isSized()) {
        llvm::Value* callResultPtr = builder->CreateAlloca(callResult->getType());
        builder->CreateStore(callResult, callResultPtr);
        return callResultPtr;
    }
    return (llvm::Value*) builder->getTrue();
}

antlrcpp::Any GeneratorVisitor::visitNewStmt(SpiceParser::NewStmtContext* ctx) {
    std::string variableName = ctx->IDENTIFIER()[0]->toString();
    std::string structName = ctx->IDENTIFIER()[1]->toString();
    std::string structScope = ScopeIdUtil::getScopeId(ctx);

    // Get data type
    llvm::Type* structType;
    if (ctx->dataType()->TYPE_DYN()) {
        SymbolType dataType = SymbolType(TYPE_STRUCT, structName);
        structType = getTypeForSymbolType(dataType);
    } else {
        structType = visit(ctx->dataType()).as<llvm::Type*>();
    }

    // Allocate space for the struct in memory
    llvm::Value* structAddress = builder->CreateAlloca(structType, nullptr, currentVar);
    currentScope->lookup(variableName)->updateAddress(structAddress);
    currentScope->lookup(variableName)->updateLLVMType(structType);

    // Fill the struct with the stated values
    SymbolTable* structSymbolTable = currentScope->lookupTable({ structScope });
    for (unsigned int i = 0; i < ctx->paramLst()->assignExpr().size(); i++) {
        // Visit assignment
        llvm::Value* assignmentPtr = visit(ctx->paramLst()->assignExpr()[i]).as<llvm::Value*>();
        llvm::Value* assignment = builder->CreateLoad(assignmentPtr->getType()->getPointerElementType(), assignmentPtr);
        // Get pointer to struct element
        llvm::Value* fieldAddress = builder->CreateStructGEP(structType, structAddress, i);
        // Store value to address
        builder->CreateStore(assignment, fieldAddress);
    }

    return structAddress;
}

antlrcpp::Any GeneratorVisitor::visitArrayInitStmt(SpiceParser::ArrayInitStmtContext* ctx) {
    // Get size and data type
    std::string varName = ctx->IDENTIFIER()->toString();
    unsigned int currentArraySize = std::stoi(ctx->value()->INTEGER()->toString());

    // Get data type
    llvm::Type* baseType;
    llvm::Type* arrayType;
    if (ctx->dataType()->TYPE_DYN()) {
        SymbolType dataType = currentScope->lookup(varName)->getType();
        arrayType = getTypeForSymbolType(dataType);
        baseType = arrayType->getArrayElementType();
    } else {
        baseType = visit(ctx->dataType()).as<llvm::Type*>();
        arrayType = llvm::ArrayType::get(baseType, currentArraySize);
    }

    // Allocate array
    llvm::Value* arrayAddress = builder->CreateAlloca(arrayType);

    // Fill items with the stated values
    if (ctx->paramLst()) {
        for (unsigned int i = 0; i < ctx->paramLst()->assignExpr().size(); i++) {
            llvm::Value* itemValuePtr = visit(ctx->paramLst()->assignExpr()[i]).as<llvm::Value*>();
            llvm::Value* itemValue = builder->CreateLoad(itemValuePtr->getType()->getPointerElementType(), itemValuePtr);
            // Calculate item address
            std::vector<llvm::Value*> indices = { builder->getInt32(0), builder->getInt32(i) };
            llvm::Value* itemAddress = builder->CreateInBoundsGEP(arrayType, arrayAddress, indices);
            // Store value to item address
            builder->CreateStore(itemValue, itemAddress);
        }
    }

    // Update address in symbol table
    currentScope->lookup(varName)->updateAddress(arrayAddress);
    currentScope->lookup(varName)->updateLLVMType(arrayType);

    return (llvm::Value*) builder->getTrue();
}

antlrcpp::Any GeneratorVisitor::visitImportStmt(SpiceParser::ImportStmtContext* ctx) {
    // Ignore sub-tree
    return nullptr;
}

antlrcpp::Any GeneratorVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext* ctx) {
    // Check if a value is attached to the return statement
    llvm::Value* returnValuePtr;
    if (ctx->assignExpr()) {
        returnValuePtr = visit(ctx->assignExpr()).as<llvm::Value*>();
    } else {
        returnValuePtr = currentScope->lookup(RETURN_VARIABLE_NAME)->getAddress();
    }
    llvm::Value* returnValue = builder->CreateLoad(returnValuePtr->getType()->getPointerElementType(), returnValuePtr);

    // Build return value
    if (!blockAlreadyTerminated) {
        builder->CreateRet(returnValue);
        blockAlreadyTerminated = true;
    }
    return returnValuePtr;
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
    llvm::Function* printf = module->getFunction("printf");
    std::vector<llvm::Value*> printfArgs;
    std::string stringTemplate = ctx->STRING()->toString();
    stringTemplate = stringTemplate.substr(1, stringTemplate.size() - 2);
    printfArgs.push_back(builder->CreateGlobalStringPtr(stringTemplate));
    for (auto& arg : ctx->assignExpr()) {
        llvm::Value* argValPtr = visit(arg).as<llvm::Value*>();
        llvm::Value* argVal = builder->CreateLoad(argValPtr->getType()->getPointerElementType(), argValPtr);
        if (argVal->getType()->isIntegerTy(1))
            argVal = builder->CreateZExt(argVal, llvm::Type::getInt32Ty(*context));
        if (argVal == nullptr) throw IRError(*arg->start, PRINTF_NULL_TYPE, "'" + arg->getText() + "' is null");
        printfArgs.push_back(argVal);
    }
    return builder->CreateCall(printf, printfArgs);
}

antlrcpp::Any GeneratorVisitor::visitAssignExpr(SpiceParser::AssignExprContext* ctx) {
    // Get value of right side
    llvm::Value* rhsPtr = visit(ctx->ternaryExpr()).as<llvm::Value*>();

    if (ctx->declStmt() || ctx->idenValue()) {
        // Load right side value
        llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);

        // Get value of left side
        std::string varName;
        SymbolTableEntry* variableEntry;
        llvm::Value* lhsPtr;
        if (ctx->declStmt()) { // Variable was declared in this line
            varName = visit(ctx->declStmt()).as<std::string>();
            // Get symbol table entry
            variableEntry = currentScope->lookup(varName);
            lhsPtr = variableEntry->getAddress();
        } else { // Variable was declared before and is referenced here
            varName = ctx->idenValue()->IDENTIFIER()[0]->toString();
            // Get symbol table entry
            variableEntry = currentScope->lookup(varName);
            lhsPtr = visit(ctx->idenValue()).as<llvm::Value*>();
        }

        if (ctx->ASSIGN_OP()) {
            // Store right side on the left one
            if (variableEntry->isLocal()) { // Local variable
                builder->CreateStore(rhs, lhsPtr);
            } else { // Global variable
                llvm::GlobalVariable* lhs = module->getNamedGlobal(varName);
                if (ctx->declStmt()) {
                    lhs->setInitializer((llvm::Constant*) rhs);
                } else {
                    builder->CreateStore(rhs, lhs);
                }
            }
        } else if (ctx->PLUS_EQUAL()) {
            if (variableEntry->isLocal()) { // Local variable
                llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
                rhs = createAddInst(lhs, lhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhsPtr);
            } else { // Global variable
                llvm::GlobalVariable* lhs = module->getNamedGlobal(varName);
                rhs = createAddInst(lhs, lhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhsPtr);
            }
        } else if (ctx->MINUS_EQUAL()) {
            if (variableEntry->isLocal()) { // Local variable
                llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
                rhs = createSubInst(lhs, lhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhsPtr);
            } else { // Global variable
                llvm::GlobalVariable* lhs = module->getNamedGlobal(varName);
                rhs = createSubInst(lhs, lhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhsPtr);
            }
        } else if (ctx->MUL_EQUAL()) {
            if (variableEntry->isLocal()) { // Local variable
                llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
                rhs = createMulInst(lhs, lhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhsPtr);
            } else { // Global variable
                llvm::GlobalVariable* lhs = module->getNamedGlobal(varName);
                rhs = createMulInst(lhs, lhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhsPtr);
            }
        } else if (ctx->DIV_EQUAL()) {
            if (variableEntry->isLocal()) { // Local variable
                llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
                rhs = createDivInst(lhs, lhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhsPtr);
            } else { // Global variable
                llvm::GlobalVariable* lhs = module->getNamedGlobal(varName);
                rhs = createDivInst(lhs, lhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhsPtr);
            }
        } else if (ctx->SHL_EQUAL()) {
            if (variableEntry->isLocal()) { // Local variable
                llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
                rhs = builder->CreateShl(lhs, rhs, "shl");
                builder->CreateStore(rhs, lhsPtr);
            } else { // Global variable
                llvm::GlobalVariable* lhs = module->getNamedGlobal(varName);
                rhs = builder->CreateShl(lhs, rhs, "shl");
                builder->CreateStore(rhs, lhsPtr);
            }
        } else if (ctx->SHR_EQUAL()) {
            if (variableEntry->isLocal()) { // Local variable
                llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
                rhs = builder->CreateLShr(lhs, rhs, "shr");
                builder->CreateStore(rhs, lhsPtr);
            } else { // Global variable
                llvm::GlobalVariable* lhs = module->getNamedGlobal(varName);
                rhs = builder->CreateLShr(lhs, rhs, "shr");
                builder->CreateStore(rhs, lhsPtr);
            }
        } else {
            if (ctx->declStmt()) {
                // Store the default value to the variable
                builder->CreateStore(getDefaultValueForSymbolType(variableEntry->getType()), rhsPtr);
            }
        }
    }

    // Return the value on the right side
    return rhsPtr;
}

antlrcpp::Any GeneratorVisitor::visitTernaryExpr(SpiceParser::TernaryExprContext* ctx) {
    if (ctx->logicalOrExpr().size() > 1) {
        llvm::Value* conditionPtr = visit(ctx->logicalOrExpr()[0]).as<llvm::Value*>();
        llvm::Value* condition = builder->CreateLoad(conditionPtr->getType()->getPointerElementType(), conditionPtr);
        llvm::Function* parentFct = builder->GetInsertBlock()->getParent();

        // Create blocks
        llvm::BasicBlock* bThen = llvm::BasicBlock::Create(*context, "tern.then");
        llvm::BasicBlock* bElse = llvm::BasicBlock::Create(*context, "tern.else");
        llvm::BasicBlock* bEnd = llvm::BasicBlock::Create(*context, "tern.end");

        // Conditional jump to respective block
        createCondBr(condition, bThen, bElse);

        // Fill then block
        parentFct->getBasicBlockList().push_back(bThen);
        moveInsertPointToBlock(bThen);
        llvm::Value* thenValuePtr = visit(ctx->logicalOrExpr()[1]).as<llvm::Value*>();
        llvm::Value* thenValue = builder->CreateLoad(thenValuePtr->getType()->getPointerElementType(), thenValuePtr);
        createBr(bEnd);

        // Fill else block
        parentFct->getBasicBlockList().push_back(bElse);
        moveInsertPointToBlock(bElse);
        llvm::Value* elseValuePtr = visit(ctx->logicalOrExpr()[2]).as<llvm::Value*>();
        llvm::Value* elseValue = builder->CreateLoad(elseValuePtr->getType()->getPointerElementType(), elseValuePtr);
        createBr(bEnd);

        // Fill end block
        parentFct->getBasicBlockList().push_back(bEnd);
        moveInsertPointToBlock(bEnd);

        // Setup phi value
        llvm::PHINode* phi = builder->CreatePHI(thenValue->getType(), 2, "phi");
        phi->addIncoming(thenValue, bThen);
        phi->addIncoming(elseValue, bElse);

        llvm::Value* resultPtr = builder->CreateAlloca(phi->getType());
        builder->CreateStore(phi, resultPtr);
        return resultPtr;
    }
    return visit(ctx->logicalOrExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext* ctx) {
    if (ctx->logicalAndExpr().size() > 1) {
        // Prepare for short-circuiting
        std::tuple<llvm::Value*, llvm::BasicBlock*> incomingBlocks[ctx->logicalAndExpr().size()];
        llvm::BasicBlock* bEnd = llvm::BasicBlock::Create(*context, "lor.end");
        llvm::Function* parentFunction = builder->GetInsertBlock()->getParent();
        parentFunction->getBasicBlockList().push_back(bEnd);

        // Visit the first condition
        llvm::Value* lhsPtr = visit(ctx->logicalAndExpr()[0]).as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);

        // Prepare the blocks
        incomingBlocks[0] = { lhs, builder->GetInsertBlock() };
        for (int i = 1; i < ctx->logicalAndExpr().size(); i++) {
            llvm::BasicBlock* bb = llvm::BasicBlock::Create(*context, "lor." + std::to_string(i));
            parentFunction->getBasicBlockList().push_back(bb);
            incomingBlocks[i] = { nullptr, bb };
        }
        createCondBr(lhs, bEnd, std::get<1>(incomingBlocks[1]));

        // Create a block for every other condition
        for (int i = 1; i < ctx->logicalAndExpr().size(); i++) {
            moveInsertPointToBlock(std::get<1>(incomingBlocks[i]));
            llvm::Value* rhsPtr = visit(ctx->logicalAndExpr()[i]).as<llvm::Value*>();
            llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
            std::get<0>(incomingBlocks[i]) = rhs;
            if (i < ctx->logicalAndExpr().size() -1) {
                createCondBr(rhs, bEnd, std::get<1>(incomingBlocks[i + 1]));
            } else {
                createBr(bEnd);
            }
        }

        // Get the result with the phi node
        moveInsertPointToBlock(bEnd);
        llvm::PHINode* phi = builder->CreatePHI(lhs->getType(), ctx->logicalAndExpr().size(), "lor_phi");
        for (const auto& tuple : incomingBlocks)
            phi->addIncoming(std::get<0>(tuple), std::get<1>(tuple));

        // Store the result
        llvm::Value* resultPtr = builder->CreateAlloca(phi->getType());
        builder->CreateStore(phi, resultPtr);
        return resultPtr;
    }
    return visit(ctx->logicalAndExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext* ctx) {
    if (ctx->bitwiseOrExpr().size() > 1) {
        // Prepare for short-circuiting
        std::tuple<llvm::Value*, llvm::BasicBlock*> incomingBlocks[ctx->bitwiseOrExpr().size()];
        llvm::BasicBlock* bEnd = llvm::BasicBlock::Create(*context, "land.end");
        llvm::Function* parentFunction = builder->GetInsertBlock()->getParent();
        parentFunction->getBasicBlockList().push_back(bEnd);

        // Visit the first condition
        llvm::Value* lhsPtr = visit(ctx->bitwiseOrExpr()[0]).as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);

        // Prepare the blocks
        incomingBlocks[0] = { lhs, builder->GetInsertBlock() };
        for (int i = 1; i < ctx->bitwiseOrExpr().size(); i++) {
            llvm::BasicBlock* bb = llvm::BasicBlock::Create(*context, "land." + std::to_string(i));
            parentFunction->getBasicBlockList().push_back(bb);
            incomingBlocks[i] = { nullptr, bb };
        }
        createCondBr(lhs, std::get<1>(incomingBlocks[1]), bEnd);

        // Create a block for every other condition
        for (int i = 1; i < ctx->bitwiseOrExpr().size(); i++) {
            moveInsertPointToBlock(std::get<1>(incomingBlocks[i]));
            llvm::Value* rhsPtr = visit(ctx->bitwiseOrExpr()[i]).as<llvm::Value*>();
            llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
            std::get<0>(incomingBlocks[i]) = rhs;
            if (i < ctx->bitwiseOrExpr().size() -1) {
                createCondBr(rhs, std::get<1>(incomingBlocks[i + 1]), bEnd);
            } else {
                createBr(bEnd);
            }
        }

        // Get the result with the phi node
        moveInsertPointToBlock(bEnd);
        llvm::PHINode* phi = builder->CreatePHI(lhs->getType(), ctx->bitwiseOrExpr().size(), "land_phi");
        for (const auto& tuple : incomingBlocks)
            phi->addIncoming(std::get<0>(tuple), std::get<1>(tuple));

        // Store the result
        llvm::Value* resultPtr = builder->CreateAlloca(phi->getType());
        builder->CreateStore(phi, resultPtr);
        return resultPtr;
    }
    return visit(ctx->bitwiseOrExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext* ctx) {
    if (ctx->bitwiseAndExpr().size() > 1) {
        llvm::Value* lhsPtr = visit(ctx->bitwiseAndExpr()[0]).as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        for (int i = 1; i < ctx->bitwiseAndExpr().size(); i++) {
            llvm::Value* rhsPtr = visit(ctx->bitwiseAndExpr()[i]).as<llvm::Value*>();
            llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
            lhs = builder->CreateOr(lhs, rhs, "bw_or");
        }
        llvm::Value* resultPtr = builder->CreateAlloca(lhs->getType());
        builder->CreateStore(lhs, resultPtr);
        return resultPtr;
    }
    return visit(ctx->bitwiseAndExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext* ctx) {
    if (ctx->equalityExpr().size() > 1) {
        llvm::Value* lhsPtr = visit(ctx->equalityExpr()[0]).as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        for (int i = 1; i < ctx->equalityExpr().size(); i++) {
            llvm::Value* rhsPtr = visit(ctx->equalityExpr()[i]).as<llvm::Value*>();
            llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
            lhs = builder->CreateAnd(lhs, rhs, "bw_and");
        }
        llvm::Value* resultPtr = builder->CreateAlloca(lhs->getType());
        builder->CreateStore(lhs, resultPtr);
        return resultPtr;
    }
    return visit(ctx->equalityExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitEqualityExpr(SpiceParser::EqualityExprContext* ctx) {
    if (ctx->relationalExpr().size() > 1) {
        llvm::Value* lhsPtr = visit(ctx->relationalExpr()[0]).as<llvm::Value*>();
        llvm::Value* rhsPtr = visit(ctx->relationalExpr()[1]).as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);

        // Equality expr is: relationalExpr EQUAL relationalExpr
        if (ctx->EQUAL()) {
            llvm::Value* cmpInst;
            if (lhs->getType()->isDoubleTy()) {
                cmpInst = builder->CreateFCmpOEQ(lhs, rhs, "eq");
            } else {
                cmpInst = builder->CreateICmpEQ(lhs, rhs, "eq");
            }
            llvm::Value* resultPtr = builder->CreateAlloca(cmpInst->getType());
            builder->CreateStore(cmpInst, resultPtr);
            return resultPtr;
        }

        // Equality expr is: relationalExpr NOT_EQUAL relationalExpr
        if (ctx->NOT_EQUAL()) {
            llvm::Value* cmpInst;
            if (lhs->getType()->isDoubleTy()) {
                cmpInst = builder->CreateFCmpONE(lhs, rhs, "ne");
            } else {
                cmpInst = builder->CreateICmpNE(lhs, rhs, "ne");
            }
            llvm::Value* resultPtr = builder->CreateAlloca(cmpInst->getType());
            builder->CreateStore(cmpInst, resultPtr);
            return resultPtr;
        }
    }
    return visit(ctx->relationalExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext* ctx) {
    if (ctx->shiftExpr().size() > 1) {
        llvm::Value* lhsPtr = visit(ctx->shiftExpr()[0]).as<llvm::Value*>();
        llvm::Value* rhsPtr = visit(ctx->shiftExpr()[1]).as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);

        // Relational expr is: shiftExpr LESS shiftExpr
        if (ctx->LESS()) {
            llvm::Value* cmpInst;
            if (lhs->getType()->isDoubleTy()) {
                cmpInst = builder->CreateFCmpOLT(lhs, rhs, "lt");
            } else {
                cmpInst = builder->CreateICmpSLT(lhs, rhs, "lt");
            }
            llvm::Value* resultPtr = builder->CreateAlloca(cmpInst->getType());
            builder->CreateStore(cmpInst, resultPtr);
            return resultPtr;
        }

        // Relational expr is: shiftExpr GREATER shiftExpr
        if (ctx->GREATER()) {
            llvm::Value* cmpInst;
            if (lhs->getType()->isDoubleTy()) {
                cmpInst = builder->CreateFCmpOGT(lhs, rhs, "gt");
            } else {
                cmpInst = builder->CreateICmpSGT(lhs, rhs, "gt");
            }
            llvm::Value* resultPtr = builder->CreateAlloca(cmpInst->getType());
            builder->CreateStore(cmpInst, resultPtr);
            return resultPtr;
        }

        // Relational expr is: shiftExpr LESS_EQUAL shiftExpr
        if (ctx->LESS_EQUAL()) {
            llvm::Value* cmpInst;
            if (lhs->getType()->isDoubleTy()) {
                cmpInst = builder->CreateFCmpOLE(lhs, rhs, "le");
            } else {
                cmpInst = builder->CreateICmpSLE(lhs, rhs, "le");
            }
            llvm::Value* resultPtr = builder->CreateAlloca(cmpInst->getType());
            builder->CreateStore(cmpInst, resultPtr);
            return resultPtr;
        }

        // Relational expr is: shiftExpr GREATER_EQUAL shiftExpr
        if (ctx->GREATER_EQUAL()) {
            llvm::Value* cmpInst;
            if (lhs->getType()->isDoubleTy()) {
                cmpInst = builder->CreateFCmpOGE(lhs, rhs, "ge");
            } else {
                cmpInst = builder->CreateICmpSGE(lhs, rhs, "ge");
            }
            llvm::Value* resultPtr = builder->CreateAlloca(cmpInst->getType());
            builder->CreateStore(cmpInst, resultPtr);
            return resultPtr;
        }
    }
    return visit(ctx->shiftExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitShiftExpr(SpiceParser::ShiftExprContext* ctx) {
    // Check if there is a shift operation attached
    if (ctx->additiveExpr().size() > 1) {
        llvm::Value* lhsPtr = visit(ctx->additiveExpr()[0]).as<llvm::Value*>();
        llvm::Value* rhsPtr = visit(ctx->additiveExpr()[1]).as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);

        // Shift expr is: additiveExpr SHL additiveExpr
        if (ctx->SHL()) {
            llvm::Value* shlInst = builder->CreateShl(lhs, rhs, "shl");
            llvm::Value* resultPtr = builder->CreateAlloca(shlInst->getType());
            builder->CreateStore(shlInst, resultPtr);
            return resultPtr;
        }

        // Relational expr is: additiveExpr SHR additiveExpr
        if (ctx->SHR()) {
            llvm::Value* shrInst = builder->CreateLShr(lhs, rhs, "shr");
            llvm::Value* resultPtr = builder->CreateAlloca(shrInst->getType());
            builder->CreateStore(shrInst, resultPtr);
            return resultPtr;
        }
    }
    return visit(ctx->additiveExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext* ctx) {
    // Check if at least one additive operator is applied
    if (ctx->multiplicativeExpr().size() > 1) {
        llvm::Value* lhsPtr = visit(ctx->multiplicativeExpr()[0]).as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        llvm::Type* lhsType = lhs->getType();
        unsigned int operatorIndex = 1;
        for (int i = 1; i < ctx->multiplicativeExpr().size(); i++) {
            auto* op = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[operatorIndex]);
            llvm::Value* rhsPtr = visit(ctx->multiplicativeExpr()[i]).as<llvm::Value*>();
            llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
            llvm::Type* rhsType = rhs->getType();

            if (op->getSymbol()->getType() == SpiceParser::PLUS)
                lhs = createAddInst(lhs, lhsType, rhs, rhsType);
            else
                lhs = createSubInst(lhs, lhsType, rhs, rhsType);

            operatorIndex += 2;
        }

        llvm::Value* resultPtr = builder->CreateAlloca(lhs->getType());
        builder->CreateStore(lhs, resultPtr);
        return resultPtr;
    }
    return visit(ctx->multiplicativeExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext* ctx) {
    // Check if at least one multiplicative operator is applied
    if (ctx->prefixUnaryExpr().size() > 1) {
        llvm::Value* lhsPtr = visit(ctx->prefixUnaryExpr()[0]).as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        llvm::Type* lhsType = lhs->getType();
        unsigned int operatorIndex = 1;
        for (int i = 1; i < ctx->prefixUnaryExpr().size(); i++) {
            auto* op = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[operatorIndex]);
            llvm::Value* rhsPtr = visit(ctx->prefixUnaryExpr()[i]).as<llvm::Value*>();
            llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
            llvm::Type* rhsType = rhs->getType();

            if (op->getSymbol()->getType() == SpiceParser::MUL)
                lhs = createMulInst(lhs, lhsType, rhs, rhsType);
            else
                lhs = createDivInst(lhs, lhsType, rhs, rhsType);

            operatorIndex += 2;
        }

        llvm::Value* resultPtr = builder->CreateAlloca(lhs->getType());
        builder->CreateStore(lhs, resultPtr);
        return resultPtr;
    }
    return visit(ctx->prefixUnaryExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext* ctx) {
    auto value = visit(ctx->postfixUnaryExpr());

    // Prefix unary is: PLUS_PLUS postfixUnary
    if (ctx->PLUS_PLUS()) {
        llvm::Value* lhsPtr = value.as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        llvm::Value* rhs = builder->CreateAdd(lhs, builder->getInt32(1), "pre_pp");
        builder->CreateStore(rhs, lhsPtr);
    }

    // Prefix unary is: MINUS_MINUS postfixUnary
    if (ctx->MINUS_MINUS()) {
        llvm::Value* lhsPtr = value.as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        llvm::Value* rhs = builder->CreateSub(lhs, builder->getInt32(1), "pre_mm");
        builder->CreateStore(rhs, lhsPtr);
    }

    // Prefix unary is: NOT postfixUnary
    if (ctx->NOT()) {
        llvm::Value* lhsPtr = value.as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        builder->CreateStore(builder->CreateNot(lhs, "not"), lhsPtr);
    }

    return value;
}

antlrcpp::Any GeneratorVisitor::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext* ctx) {
    auto value = visit(ctx->atomicExpr());

    // Postfix unary is: PLUS_PLUS atomicExpr
    if (ctx->PLUS_PLUS()) {
        llvm::Value* lhsPtr = value.as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        llvm::Value* rhs = builder->CreateAdd(lhs, builder->getInt32(1), "post_pp");
        builder->CreateStore(rhs, lhsPtr);
    }

    // Postfix unary is: MINUS_MINUS atomicExpr
    if (ctx->MINUS_MINUS()) {
        llvm::Value* lhsPtr = value.as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        llvm::Value* rhs = builder->CreateSub(lhs, builder->getInt32(1), "post_mm");
        builder->CreateStore(rhs, lhsPtr);
    }

    return value;
}

antlrcpp::Any GeneratorVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext* ctx) {
    if (ctx->value()) return visit(ctx->value());
    if (ctx->idenValue()) return visit(ctx->idenValue());
    if (ctx->functionCall()) return visit(ctx->functionCall());
    return visit(ctx->assignExpr());
}

// Returns pointer
antlrcpp::Any GeneratorVisitor::visitIdenValue(SpiceParser::IdenValueContext* ctx) {
    llvm::Type* baseType;
    llvm::Value* basePtr;
    std::vector<llvm::Value*> indices;
    SymbolTableEntry* entry;
    int tokenCounter = 0;
    int assignCounter = 0;
    bool applyReference = false;
    bool applyDereference = false;
    SymbolTable* scope = currentScope;

    if (ctx->BITWISE_AND()) { // Consider referencing operator
        applyReference = true;
        tokenCounter++;
    }

    if (ctx->MUL()) { // Consider de-referencing operator
        applyDereference = true;
        tokenCounter++;
    }

    // Loop through children
    while (tokenCounter < ctx->children.size()) {
        auto* token = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[tokenCounter]);
        if (token->getSymbol()->getType() == SpiceParser::IDENTIFIER) { // Consider identifier
            // Apply field
            std::string variableName = token->toString();
            entry = scope->lookup(variableName);
            if (scope == currentScope) { // This is the current scope
                baseType = entry->getLLVMType();
                basePtr = entry->getAddress();
                indices.push_back(builder->getInt32(0));
            } else { // This is a struct
                indices.push_back(builder->getInt32(entry->getOrderIndex()));
            }
        } else if (token->getSymbol()->getType() == SpiceParser::DOT) { // Consider dot operator
            // De-reference automatically if it is a struct pointer
            if (entry->getType().is(TYPE_STRUCT_PTR)) {
                basePtr = builder->CreateInBoundsGEP(baseType, basePtr, indices);
                basePtr = builder->CreateLoad(basePtr->getType()->getPointerElementType(), basePtr);
                indices.clear();
                indices.push_back(builder->getInt32(0));
            }
            // Change to new scope
            std::string structName = entry->getType().getSubType();
            scope = scope->lookupTable("struct:" + structName);
            // Check if the table exists
            if (!scope)
                throw IRError(*token->getSymbol(), VARIABLE_NOT_FOUND,
                              "Compiler error: Referenced undefined struct '" + structName + "'");
            // Conclude auto-de-referencing
            if (entry->getType().is(TYPE_STRUCT_PTR)) baseType = scope->lookup(structName)->getLLVMType();
        } else if (token->getSymbol()->getType() == SpiceParser::LBRACKET) { // Consider subscript operator
            // Get the index value
            llvm::Value* indexValue = visit(ctx->assignExpr()[assignCounter]).as<llvm::Value*>();
            indexValue = builder->CreateLoad(indexValue->getType()->getPointerElementType(), indexValue);
            indices.push_back(indexValue);
            // Increase counters
            assignCounter++;
            tokenCounter += 2; // To consume the assignExpr and the RBRACKET
        }
        // Increase counter
        tokenCounter++;
    }

    // Build GEP instruction
    llvm::Value* returnValue = builder->CreateInBoundsGEP(baseType, basePtr, indices);

    // If the de-referencing operator is present, add a zero index at the end of the gep instruction
    if (applyDereference) returnValue = builder->CreateLoad(returnValue->getType()->getPointerElementType(), returnValue);

    // If the referencing operator is present, store the calculated address into memory and return the address of that location
    if (applyReference) {
        llvm::Value* ptrLocation = builder->CreateAlloca(basePtr->getType());
        builder->CreateStore(basePtr, ptrLocation);
        return ptrLocation;
    }

    // Otherwise, return the calculated memory address
    return returnValue;
}

antlrcpp::Any GeneratorVisitor::visitValue(SpiceParser::ValueContext* ctx) {
    llvm::Value* llvmValue;

    // Value is a double constant
    if (ctx->DOUBLE()) {
        currentSymbolType = SymbolType(TYPE_DOUBLE);
        double value = std::stod(ctx->DOUBLE()->toString());
        llvmValue = llvm::ConstantFP::get(*context, llvm::APFloat(value));
    }

    // Value is an integer constant
    if (ctx->INTEGER()) {
        currentSymbolType = SymbolType(TYPE_INT);
        int v = std::stoi(ctx->INTEGER()->toString());
        llvmValue = llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), v);
    }

    // Value is a string constant
    if (ctx->STRING()) {
        currentSymbolType = SymbolType(TYPE_STRING);
        std::string value = ctx->STRING()->toString();
        value = value.substr(1, value.size() - 2);
        llvmValue = builder->CreateGlobalStringPtr(value, "", 0, module.get());
    }

    // Value is a boolean constant with value false
    if (ctx->FALSE()) {
        currentSymbolType = SymbolType(TYPE_BOOL);
        llvmValue = builder->getFalse();
    }

    // Value is a boolean constant with value true
    if (ctx->TRUE()) {
        currentSymbolType = SymbolType(TYPE_BOOL);
        llvmValue = builder->getTrue();
    }

    // If global variable value -> return value immediately
    if (!currentScope->getParent()) return llvmValue;

    // Store the value to a tmp variable
    llvm::Value* llvmValuePtr = builder->CreateAlloca(llvmValue->getType());
    builder->CreateStore(llvmValue, llvmValuePtr);
    return llvmValuePtr;
}

antlrcpp::Any GeneratorVisitor::visitDataType(SpiceParser::DataTypeContext* ctx) {
    if (ctx->TYPE_DOUBLE()) { // Data type is double
        currentSymbolType = SymbolType(TYPE_DOUBLE);
    } else if (ctx->TYPE_INT()) { // Data type is int
        currentSymbolType = SymbolType(TYPE_INT);
    } else if (ctx->TYPE_STRING()) { // Data type is string
        currentSymbolType = SymbolType(TYPE_STRING);
    } else if (ctx->TYPE_BOOL()) { // Data type is bool
        currentSymbolType = SymbolType(TYPE_BOOL);
    } else if (ctx->TYPE_DYN()) { // Data type is dyn
        SymbolTableEntry* symbolTableEntry = currentScope->lookup(currentVar);
        currentSymbolType = symbolTableEntry->getType();
    } else if (ctx->IDENTIFIER()) { // Custom data type
        currentSymbolType = SymbolType(TYPE_STRUCT, ctx->IDENTIFIER()->toString());
    }

    // Consider pointer
    if (ctx->MUL()) currentSymbolType = currentSymbolType.getPointerType();

    // Consider array brackets
    if (ctx->LBRACKET()) currentSymbolType = currentSymbolType.getArrayType();

    // Come up with the LLVM type
    llvm::Type* type = getTypeForSymbolType(currentSymbolType);
    // Throw an error if something went wrong.
    // This should technically never occur because of the semantic analysis
    if (!type) throw IRError(*ctx->TYPE_DYN()->getSymbol(), UNEXPECTED_DYN_TYPE, "Dyn was other");
    return type;
}

void GeneratorVisitor::initializeExternalFunctions() {
    // printf function
    module->getOrInsertFunction("printf", llvm::FunctionType::get(
            llvm::Type::getInt32Ty(*context),
            llvm::Type::getInt8Ty(*context)->getPointerTo(),
            true));
    // malloc function
    module->getOrInsertFunction("malloc", llvm::FunctionType::get(
            llvm::Type::getInt8Ty(*context)->getPointerTo(),
            llvm::Type::getInt32Ty(*context),
            false));
    // free function
    module->getOrInsertFunction("free", llvm::FunctionType::get(
            llvm::Type::getVoidTy(*context),
            llvm::Type::getInt8Ty(*context)->getPointerTo(),
            false));
    // memcpy function
    std::vector<llvm::Type*> paramTypes = {
            llvm::Type::getInt8Ty(*context)->getPointerTo(),
            llvm::Type::getInt8Ty(*context)->getPointerTo(),
            llvm::Type::getInt32Ty(*context)
    };
    module->getOrInsertFunction("memcpy", llvm::FunctionType::get(
            llvm::Type::getInt8Ty(*context)->getPointerTo(), paramTypes, false));
}

llvm::Value* GeneratorVisitor::createAddInst(llvm::Value* lhs, llvm::Type* lhsType, llvm::Value* rhs, llvm::Type* rhsType) {
    if (lhsType->isDoubleTy()) {
        if (rhsType->isDoubleTy()) {
            // double + double
            return builder->CreateFAdd(lhs, rhs, "add");
        } else if (rhsType->isIntegerTy(32)) {
            // double + int
            return builder->CreateFAdd(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "add");
        } else if (rhsType->isPointerTy() && rhsType->getPointerElementType()->isIntegerTy(8)) {
            // double + string
            // ToDo(@marcauberer): Insert call to toString(double) and concatStrings
            throw IRError(COMING_SOON_IR, "The compiler does not support the '+' operator for lhs=double and rhs=string yet");
        }
    } else if (lhsType->isIntegerTy(32)) {
        if (rhsType->isDoubleTy()) {
            // int + double
            return builder->CreateFAdd(builder->CreateSIToFP(rhs, rhs->getType()), rhs, "add");
        } else if (rhsType->isIntegerTy(32)) {
            // int + int
            return builder->CreateAdd(lhs, rhs, "add");
        } else if (rhsType->isPointerTy() && rhsType->getPointerElementType()->isIntegerTy(8)) {
            // int + string
            // ToDo(@marcauberer): Insert call to toString(int) and concatStrings
            throw IRError(COMING_SOON_IR, "The compiler does not support the '+' operator for lhs=int and rhs=string yet");
        }
    } else if (lhsType->isPointerTy() && lhsType->getPointerElementType()->isIntegerTy(8)) {
        if (rhsType->isDoubleTy()) {
            // string + double
            // ToDo(@marcauberer): Insert call to toString(double) and concatStrings
            throw IRError(COMING_SOON_IR, "The compiler does not support the '+' operator for lhs=string and rhs=double yet");
        } else if (rhsType->isIntegerTy(32)) {
            // string + int
            // ToDo(@marcauberer): Insert call to toString(int) and concatStrings
            throw IRError(COMING_SOON_IR, "The compiler does not support the '+' operator for lhs=string and rhs=int yet");
        } else if (rhsType->isPointerTy() && rhsType->getPointerElementType()->isIntegerTy(8)) {
            // string + string
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '+' operator for lhs=string and rhs=string yet");
        }
    }
    return lhs;
}

llvm::Value* GeneratorVisitor::createSubInst(llvm::Value* lhs, llvm::Type* lhsType, llvm::Value* rhs, llvm::Type* rhsType) {
    if (lhsType->isDoubleTy()) {
        if (rhsType->isDoubleTy()) {
            // double - double
            return builder->CreateFSub(lhs, rhs, "sub");
        } else if (rhsType->isIntegerTy(32)) {
            // double - int
            return builder->CreateFSub(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "sub");
        }
    } else if (lhsType->isIntegerTy(32)) {
        if (rhsType->isDoubleTy()) {
            // int - double
            return builder->CreateFSub(builder->CreateSIToFP(rhs, rhs->getType()), rhs, "sub");
        } else if (rhsType->isIntegerTy(32)) {
            // int - int
            return builder->CreateSub(lhs, rhs, "sub");
        }
    }
    return lhs;
}

llvm::Value* GeneratorVisitor::createMulInst(llvm::Value* lhs, llvm::Type* lhsType, llvm::Value* rhs, llvm::Type* rhsType) {
    if (lhsType->isDoubleTy()) {
        if (rhsType->isDoubleTy()) {
            // double * double
            lhs = builder->CreateFMul(lhs, rhs, "mul");
        } else if (rhsType->isIntegerTy(32)) {
            // double * int
            lhs = builder->CreateFMul(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "mul");
        }
    } else if (lhsType->isIntegerTy(32)) {
        if (rhsType->isDoubleTy()) {
            // int * double
            lhs = builder->CreateFMul(builder->CreateSIToFP(rhs, rhs->getType()), rhs, "mul");
        } else if (rhsType->isIntegerTy(32)) {
            // int * int
            lhs = builder->CreateMul(lhs, rhs, "mul");
        } else if (rhsType->isPointerTy() && rhsType->getPointerElementType()->isIntegerTy(8)) {
            // int * string
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=int and rhs=string yet");
        }
    } else if (lhsType->isPointerTy() && lhsType->getPointerElementType()->isIntegerTy(8)) {
        if (rhsType->isIntegerTy(32)) {
            // string * int
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=string and rhs=int yet");
        }
    }
    return lhs;
}

llvm::Value* GeneratorVisitor::createDivInst(llvm::Value* lhs, llvm::Type* lhsType, llvm::Value* rhs, llvm::Type* rhsType) {
    if (lhsType->isDoubleTy()) {
        if (rhsType->isDoubleTy()) {
            // double : double
            lhs = builder->CreateFDiv(lhs, rhs, "div");
        } else if (rhsType->isIntegerTy(32)) {
            // double : int
            lhs = builder->CreateFDiv(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "div");
        }
    } else if (lhsType->isIntegerTy(32)) {
        if (rhsType->isDoubleTy()) {
            // int : double
            lhs = builder->CreateFDiv(builder->CreateSIToFP(rhs, rhs->getType()), rhs, "div");
        } else if (rhsType->isIntegerTy(32)) {
            // int : int
            lhs = builder->CreateSDiv(lhs, rhs, "div");
        }
    }
    return lhs;
}

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

llvm::Type* GeneratorVisitor::getTypeForSymbolType(SymbolType symbolType) {
    currentSymbolType = symbolType;
    switch (symbolType.getSuperType()) {
        case TYPE_DOUBLE: return llvm::Type::getDoubleTy(*context);
        case TYPE_DOUBLE_PTR: return llvm::Type::getDoublePtrTy(*context);
        case TYPE_DOUBLE_ARRAY: return llvm::ArrayType::getDoubleTy(*context);
        case TYPE_DOUBLE_PTR_ARRAY: return llvm::ArrayType::getDoublePtrTy(*context);
        case TYPE_INT: return llvm::Type::getInt32Ty(*context);
        case TYPE_INT_PTR: return llvm::Type::getInt32PtrTy(*context);
        case TYPE_INT_ARRAY: return llvm::ArrayType::getInt32Ty(*context);
        case TYPE_INT_PTR_ARRAY: return llvm::ArrayType::getInt32PtrTy(*context);
        case TYPE_STRING: return llvm::Type::getInt8PtrTy(*context);
        case TYPE_STRING_PTR: return llvm::Type::getInt8PtrTy(*context)->getPointerTo();
        case TYPE_STRING_ARRAY: return llvm::ArrayType::getInt8PtrTy(*context);
        case TYPE_STRING_PTR_ARRAY: throw std::runtime_error("String ptr array support coming soon");
        case TYPE_BOOL: return llvm::Type::getInt1Ty(*context);
        case TYPE_BOOL_PTR: return llvm::Type::getInt1PtrTy(*context);
        case TYPE_BOOL_ARRAY: return llvm::ArrayType::getInt1Ty(*context);
        case TYPE_BOOL_PTR_ARRAY: return llvm::ArrayType::getInt1PtrTy(*context);
        case TYPE_STRUCT: return currentScope->lookup(symbolType.getSubType())->getLLVMType();
        case TYPE_STRUCT_PTR: return currentScope->lookup(symbolType.getSubType())->getLLVMType()->getPointerTo();
        case TYPE_STRUCT_ARRAY: throw std::runtime_error("Struct array support coming soon");
        case TYPE_STRUCT_PTR_ARRAY: throw std::runtime_error("Struct ptr array support coming soon");
        default: throw std::runtime_error("Internal compiler error: Cannot determine LLVM type of " + symbolType.getName());
    }
}

llvm::Value* GeneratorVisitor::getDefaultValueForSymbolType(SymbolType symbolType) {
    switch (symbolType.getSuperType()) {
        case TYPE_DOUBLE: return llvm::ConstantFP::get(*context, llvm::APFloat(0.0));
        case TYPE_INT: return llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), 0);
        case TYPE_STRING: return builder->CreateGlobalStringPtr("", "", 0, module.get());
        case TYPE_BOOL: return builder->getFalse();
        default:
            throw std::runtime_error("Internal compiler error: Cannot determine default value of " + symbolType.getName());
    }
}
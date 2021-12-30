// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "GeneratorVisitor.h"

void GeneratorVisitor::init() {
    // Create LLVM base components
    context = std::make_unique<llvm::LLVMContext>();
    builder = std::make_unique<llvm::IRBuilder<>>(*context);
    module = std::make_unique<llvm::Module>(FileUtil::getFileName(mainSourceFile), *context);
    conversionsManager = std::make_unique<OpRuleConversionsManager>(builder.get());

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
        // Change scope
        std::string scopeId = ScopeIdUtil::getScopeId(ctx);
        currentScope = currentScope->getChild(scopeId);

        // Visit parameters
        std::vector<SymbolType> symbolTypes;
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

        // Build function itself
        llvm::Type* returnType = llvm::Type::getInt32Ty(*context);
        llvm::FunctionType* fctType = llvm::FunctionType::get(returnType, paramTypes, false);
        llvm::Function* fct = llvm::Function::Create(fctType, llvm::Function::ExternalLinkage,
                                                     MAIN_FUNCTION_NAME, module.get());
        llvm::BasicBlock* bMain = llvm::BasicBlock::Create(*context, "entry");
        fct->getBasicBlockList().push_back(bMain);
        moveInsertPointToBlock(bMain);

        // Store function params
        unsigned int declStmtCount = ctx->paramLstDef() ? ctx->paramLstDef()->declStmt().size() : 0;
        for (auto& param : fct->args()) {
            unsigned paramNo = param.getArgNo();
            std::string paramName = paramNames[paramNo];
            llvm::Type* paramType = fct->getFunctionType()->getParamType(paramNo);
            llvm::Value* memAddress = builder->CreateAlloca(paramType, nullptr, paramName);
            currentScope->lookup(paramName)->updateAddress(memAddress);
            currentScope->lookup(paramName)->updateLLVMType(paramType);
            builder->CreateStore(&param, memAddress);
        }

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
    // Save the old scope to restore later
    SymbolTable* oldScope = currentScope;

    // Check if this is a global function or a method
    bool isMethod = false;
    std::string functionName = ctx->IDENTIFIER().back()->toString();
    if (ctx->IDENTIFIER().size() > 1) { // Method
        isMethod = true;
        // Change to the struct scope
        currentScope = currentScope->lookupTable("struct:" + ctx->IDENTIFIER()[0]->toString());
    }

    // Change scope
    FunctionSignature signature = currentScope->popSignature();
    currentScope = currentScope->getChild(signature.toString());

    // Get return type
    currentVar = RETURN_VARIABLE_NAME;
    llvm::Type* returnType = visit(ctx->dataType()).as<llvm::Type*>();
    std::vector<SymbolType> symbolTypes;
    symbolTypes.push_back(currentSymbolType);

    // Create parameter list
    std::vector<std::string> paramNames;
    std::vector<llvm::Type*> paramTypes;
    // This variable (struct ptr of the parent struct)
    if (isMethod) {
        paramNames.push_back(THIS_VARIABLE_NAME);
        SymbolTableEntry* thisEntry = currentScope->getParent()->lookup(ctx->IDENTIFIER()[0]->toString());
        llvm::Type* paramType = thisEntry->getLLVMType()->getPointerTo();
        paramTypes.push_back(paramType);
        symbolTypes.push_back(thisEntry->getType().toPointer());
    }
    // Parameters
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

    // Create function itself
    llvm::FunctionType* fctType = llvm::FunctionType::get(returnType, paramTypes, false);
    llvm::Function* fct = llvm::Function::Create(fctType, llvm::Function::ExternalLinkage,
                                                 signature.toString(), module.get());
    fct->addFnAttr(llvm::Attribute::NoUnwind);

    // Create entry block
    llvm::BasicBlock* bEntry = llvm::BasicBlock::Create(*context, "entry");
    fct->getBasicBlockList().push_back(bEntry);
    moveInsertPointToBlock(bEntry);

    // Store function params
    unsigned int declStmtCount = ctx->paramLstDef() ? ctx->paramLstDef()->declStmt().size() : 0;
    for (auto& param : fct->args()) {
        unsigned paramNo = param.getArgNo();
        std::string paramName = paramNames[paramNo];
        llvm::Type* paramType = fct->getFunctionType()->getParamType(paramNo);
        llvm::Value* memAddress = builder->CreateAlloca(paramType, nullptr, paramName);
        currentScope->lookup(paramName)->updateAddress(memAddress);
        currentScope->lookup(paramName)->updateLLVMType(paramType);
        builder->CreateStore(&param, memAddress);
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

    // Insert function declaration into symbol table
    currentScope->insertFunctionDeclaration(signature.toString(), symbolTypes);

    // Restore old scope
    currentScope = oldScope;

    // Return true as result for the function definition
    return (llvm::Value*) builder->getTrue();
}

antlrcpp::Any GeneratorVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext* ctx) {
    // Save the old scope to restore later
    SymbolTable* oldScope = currentScope;

    // Check if this is a global function or a method
    bool isMethod = false;
    std::string procedureName = ctx->IDENTIFIER().back()->toString();
    if (ctx->IDENTIFIER().size() > 1) { // Method
        isMethod = true;
        // Change to the struct scope
        currentScope = currentScope->lookupTable("struct:" + ctx->IDENTIFIER()[0]->toString());
    }

    // Change scope
    FunctionSignature signature = currentScope->popSignature();
    currentScope = currentScope->getChild(signature.toString());

    // Create parameter list
    std::vector<std::string> paramNames;
    std::vector<llvm::Type*> paramTypes;
    std::vector<SymbolType> symbolTypes;
    // This variable (struct ptr of the parent struct)
    if (isMethod) {
        paramNames.push_back(THIS_VARIABLE_NAME);
        SymbolTableEntry* thisEntry = currentScope->getParent()->lookup(ctx->IDENTIFIER()[0]->toString());
        llvm::Type* paramType = thisEntry->getLLVMType()->getPointerTo();
        paramTypes.push_back(paramType);
        symbolTypes.push_back(thisEntry->getType().toPointer());
    }
    // Parameters
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

    // Create procedure itself
    llvm::FunctionType* procType = llvm::FunctionType::get(llvm::Type::getVoidTy(*context),
                                                           paramTypes, false);
    llvm::Function* proc = llvm::Function::Create(procType, llvm::Function::ExternalLinkage,
                                                  signature.toString(), module.get());
    proc->addFnAttr(llvm::Attribute::NoUnwind);

    // Create entry block
    llvm::BasicBlock* bEntry = llvm::BasicBlock::Create(*context, "entry");
    proc->getBasicBlockList().push_back(bEntry);
    moveInsertPointToBlock(bEntry);

    // Store procedure params
    for (auto& param : proc->args()) {
        unsigned paramNo = param.getArgNo();
        std::string paramName = paramNames[paramNo];
        llvm::Type* paramType = proc->getFunctionType()->getParamType(paramNo);
        llvm::Value* memAddress = builder->CreateAlloca(paramType, nullptr, paramName);
        currentScope->lookup(paramName)->updateAddress(memAddress);
        currentScope->lookup(paramName)->updateLLVMType(paramType);
        builder->CreateStore(&param, memAddress);
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

    // Insert function declaration into symbol table
    currentScope->insertProcedureDeclaration(signature.toString(), symbolTypes);

    // Restore old scope
    currentScope = oldScope;

    // Return true as result for the function definition
    return (llvm::Value*) builder->getTrue();
}

antlrcpp::Any GeneratorVisitor::visitExtDecl(SpiceParser::ExtDeclContext* ctx) {
    // Get function name
    std::string functionName = ctx->IDENTIFIER()->toString();
    std::vector<SymbolType> symbolTypes;

    // Pop function signature from the signature stack
    FunctionSignature signature = currentScope->popSignature();

    // Get LLVM return type
    llvm::Type* returnType;
    if (ctx->dataType()) {
        returnType = visit(ctx->dataType()).as<llvm::Type*>();
        SymbolTable* functionTable = currentScope->getChild(signature.toString());
        SymbolTableEntry* returnEntry = functionTable->lookup(RETURN_VARIABLE_NAME);
        symbolTypes.push_back(returnEntry->getType());
    } else {
        returnType = llvm::Type::getVoidTy(*context);
    }

    // Get LLVM param types
    std::vector<llvm::Type*> paramTypes;
    if (ctx->typeLst()) {
        for (auto& param : ctx->typeLst()->dataType()) {
            llvm::Type* paramType = visit(param).as<llvm::Type*>();
            paramTypes.push_back(paramType);
        }
    }
    std::vector<SymbolType> paramSymbolTypes = signature.getParamTypes();
    symbolTypes.insert(std::end(symbolTypes), std::begin(paramSymbolTypes), std::end(paramSymbolTypes));

    // Get vararg
    bool isVararg = ctx->typeLst() && ctx->typeLst()->ELLIPSIS();

    // Declare function
    llvm::FunctionType* functionType = llvm::FunctionType::get(returnType, paramTypes, isVararg);
    module->getOrInsertFunction(functionName, functionType);

    if (ctx->dataType()) { // Function
        currentScope->insertFunctionDeclaration(signature.toString(), symbolTypes);
    } else { // Procedure
        currentScope->insertProcedureDeclaration(signature.toString(), symbolTypes);
    }

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
    bool isMethod = currentThisValue != nullptr;
    std::string functionName = ctx->IDENTIFIER()->toString();

    if (isMethod) {
        std::string structName = currentSymbolType.getSubType();
        functionName = structName + "." + functionName;
    }

    // Get function by signature
    FunctionSignature signature = functionCallParentScope->popSignature();
    // Check if function exists in module
    bool functionFound = false;
    std::string fctIdentifier = signature.toString();
    for (auto& function : module->getFunctionList()) {
        if (function.getName() == signature.toString()) {
            functionFound = true;
            break;
        } else if (function.getName() == signature.getFunctionName()) {
            functionFound = true;
            fctIdentifier = signature.getFunctionName();
            break;
        }
    }
    if (!functionFound) { // Not found => Declare function, which will be linked in
        SymbolTable* table = functionCallParentScope->lookupTableWithSymbol({ signature.toString() });
        // Check if it is a function or a procedure
        if (!table->getFunctionDeclaration(signature.toString()).empty()) {
            std::vector<SymbolType> symbolTypes = table->getFunctionDeclaration(signature.toString());

            // Get return type
            llvm::Type* returnType = getTypeForSymbolType(symbolTypes[0]);
            if (!returnType) throw std::runtime_error("Internal compiler error: Could not find return type of function call");

            // Get parameter types
            std::vector<llvm::Type*> paramTypes;
            for (int i = 1; i < symbolTypes.size(); i++) {
                llvm::Type* paramType = getTypeForSymbolType(symbolTypes[i]);
                if (!paramType) throw std::runtime_error("Internal compiler error: Could not get parameter type of function call");
                paramTypes.push_back(paramType);
            }

            llvm::FunctionType* fctType = llvm::FunctionType::get(returnType, paramTypes, false);
            module->getOrInsertFunction(fctIdentifier, fctType);
        } else if (!table->getProcedureDeclaration(signature.toString()).empty()) {
            std::vector<SymbolType> symbolTypes = table->getProcedureDeclaration(signature.toString());

            // Get parameter types
            std::vector<llvm::Type*> paramTypes;
            for (auto& symbolType : symbolTypes) {
                llvm::Type* paramType = getTypeForSymbolType(symbolType);
                if (!paramType) throw std::runtime_error("Internal compiler error");
                paramTypes.push_back(paramType);
            }

            llvm::FunctionType* procType = llvm::FunctionType::get(llvm::Type::getVoidTy(*context),
                                                                   paramTypes, false);
            module->getOrInsertFunction(fctIdentifier, procType);
        }
    }
    llvm::Function* fct = module->getFunction(fctIdentifier);
    llvm::FunctionType* fctType = fct->getFunctionType();

    // Fill parameter list
    int paramIndex = 0;
    std::vector<llvm::Value*> argValues;
    if (isMethod) {
        argValues.push_back(currentThisValue);
        paramIndex++;
    }
    if (ctx->paramLst()) {
        for (int i = 0; i < ctx->paramLst()->assignExpr().size(); i++) {
            llvm::Value* argValuePtr = visit(ctx->paramLst()->assignExpr()[i]).as<llvm::Value*>();
            llvm::Type* argType = fctType->getParamType(paramIndex);
            if (argValuePtr->getType()->getPointerElementType() != argType) {
                if (argType->isPointerTy() && argValuePtr->getType()->getPointerElementType()->isArrayTy()) {
                    std::vector<llvm::Value*> indices = { builder->getInt32(0), builder->getInt32(0) };
                    llvm::Type* targetType = argValuePtr->getType()->getPointerElementType();
                    llvm::Value* ptr = builder->CreateInBoundsGEP(targetType, argValuePtr, indices);
                    argValues.push_back(ptr);
                } else {
                    llvm::Value* argValue = builder->CreateLoad(argValuePtr->getType()->getPointerElementType(), argValuePtr);
                    argValues.push_back(builder->CreateBitCast(argValue, argType));
                }
            } else {
                llvm::Value* argValue = builder->CreateLoad(argValuePtr->getType()->getPointerElementType(), argValuePtr);
                argValues.push_back(argValue);
            }
            paramIndex++;
        }
    }

    llvm::Value* callResult = builder->CreateCall(fct, argValues);
    llvm::Value* callResultPtr;
    if (callResult->getType()->isSized()) {
        callResultPtr = builder->CreateAlloca(callResult->getType());
    } else {
        callResult = builder->getTrue();
        callResultPtr = builder->CreateAlloca(callResult->getType());
    }
    builder->CreateStore(callResult, callResultPtr);
    return callResultPtr;
}

antlrcpp::Any GeneratorVisitor::visitNewStmt(SpiceParser::NewStmtContext* ctx) {
    std::string variableName = ctx->IDENTIFIER()[0]->toString();
    std::string structName = ctx->IDENTIFIER()[1]->toString();
    std::string structScope = ScopeIdUtil::getScopeId(ctx);

    // Get data type
    llvm::Type* structType;
    if (ctx->dataType()->TYPE_DYN()) {
        SymbolType dataType = SymbolType(TY_STRUCT, structName);
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
    // Ignore subtree and return true
    return (llvm::Value*) builder->getTrue();
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

antlrcpp::Any GeneratorVisitor::visitBuiltinCall(SpiceParser::BuiltinCallContext* ctx) {
    if (ctx->printfCall()) return visit(ctx->printfCall());
    if (ctx->sizeOfCall()) return visit(ctx->sizeOfCall());
    throw std::runtime_error("Internal compiler error: Could not find builtin function");
}

antlrcpp::Any GeneratorVisitor::visitPrintfCall(SpiceParser::PrintfCallContext* ctx) {
    llvm::Function* printf = module->getFunction("printf");
    std::vector<llvm::Value*> printfArgs;
    std::string stringTemplate = ctx->STRING()->toString();
    stringTemplate = std::regex_replace(stringTemplate, std::regex("\\\\n"), "\n");
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
    return (llvm::Value*) builder->CreateCall(printf, printfArgs);
}

antlrcpp::Any GeneratorVisitor::visitSizeOfCall(SpiceParser::SizeOfCallContext* ctx) {
    // Visit the parameter
    llvm::Value* valuePtr = visit(ctx->assignExpr()).as<llvm::Value*>();
    llvm::Value* value = builder->CreateLoad(valuePtr->getType()->getPointerElementType(), valuePtr);

    unsigned int size = value->getType()->isArrayTy() ?
            size = value->getType()->getArrayNumElements() :
            size = value->getType()->getScalarSizeInBits();
    llvm::Value* result = builder->getInt32(size);
    llvm::Value* resultPtr = builder->CreateAlloca(result->getType());
    builder->CreateStore(result, resultPtr);
    return resultPtr;
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
            lhs = conversionsManager->getBitwiseOrInst(lhs, rhs);
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
            lhs = conversionsManager->getBitwiseAndInst(lhs, rhs);
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

        if (ctx->EQUAL()) { // Equality expr is: relationalExpr EQUAL relationalExpr
            llvm::Value* result = conversionsManager->getEqualInst(lhs, rhs);
            llvm::Value* resultPtr = builder->CreateAlloca(result->getType());
            builder->CreateStore(result, resultPtr);
            return resultPtr;
        } else if (ctx->NOT_EQUAL()) { // Equality expr is: relationalExpr NOT_EQUAL relationalExpr
            llvm::Value* result = conversionsManager->getNotEqualInst(lhs, rhs);
            llvm::Value* resultPtr = builder->CreateAlloca(result->getType());
            builder->CreateStore(result, resultPtr);
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

        if (ctx->LESS()) { // Relational expr is: shiftExpr LESS shiftExpr
            llvm::Value* result = conversionsManager->getLessInst(lhs, rhs);
            llvm::Value* resultPtr = builder->CreateAlloca(result->getType());
            builder->CreateStore(result, resultPtr);
            return resultPtr;
        } else if (ctx->GREATER()) { // Relational expr is: shiftExpr GREATER shiftExpr
            llvm::Value* result = conversionsManager->getGreaterInst(lhs, rhs);
            llvm::Value* resultPtr = builder->CreateAlloca(result->getType());
            builder->CreateStore(result, resultPtr);
            return resultPtr;
        } else if (ctx->LESS_EQUAL()) { // Relational expr is: shiftExpr LESS_EQUAL shiftExpr
            llvm::Value* result = conversionsManager->getLessEqualInst(lhs, rhs);
            llvm::Value* resultPtr = builder->CreateAlloca(result->getType());
            builder->CreateStore(result, resultPtr);
            return resultPtr;
        } else if (ctx->GREATER_EQUAL()) { // Relational expr is: shiftExpr GREATER_EQUAL shiftExpr
            llvm::Value* result = conversionsManager->getGreaterEqualInst(lhs, rhs);
            llvm::Value* resultPtr = builder->CreateAlloca(result->getType());
            builder->CreateStore(result, resultPtr);
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

        if (ctx->SHL()) { // Shift expr is: additiveExpr SHL additiveExpr
            llvm::Value* result = conversionsManager->getShiftLeftInst(lhs, rhs);
            llvm::Value* resultPtr = builder->CreateAlloca(result->getType());
            builder->CreateStore(result, resultPtr);
            return resultPtr;
        } else if (ctx->SHR()) { // Shift expr is: additiveExpr SHR additiveExpr
            llvm::Value* result = conversionsManager->getShiftRightInst(lhs, rhs);
            llvm::Value* resultPtr = builder->CreateAlloca(result->getType());
            builder->CreateStore(result, resultPtr);
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
        unsigned int operatorIndex = 1;
        for (int i = 1; i < ctx->multiplicativeExpr().size(); i++) {
            auto* op = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[operatorIndex]);
            llvm::Value* rhsPtr = visit(ctx->multiplicativeExpr()[i]).as<llvm::Value*>();
            llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);

            if (op->getSymbol()->getType() == SpiceParser::PLUS)
                lhs = conversionsManager->getPlusInst(lhs, rhs);
            else
                lhs = conversionsManager->getMinusInst(lhs, rhs);

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
        unsigned int operatorIndex = 1;
        for (int i = 1; i < ctx->prefixUnaryExpr().size(); i++) {
            auto* op = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[operatorIndex]);
            llvm::Value* rhsPtr = visit(ctx->prefixUnaryExpr()[i]).as<llvm::Value*>();
            llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);

            if (op->getSymbol()->getType() == SpiceParser::MUL)
                lhs = conversionsManager->getMulInst(lhs, rhs);
            else if (op->getSymbol()->getType() == SpiceParser::DIV)
                lhs = conversionsManager->getDivInst(lhs, rhs);
            else
                lhs = conversionsManager->getRemInst(lhs, rhs);

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
        llvm::Value* result = conversionsManager->getPrefixPlusPlusInst(lhs);
        builder->CreateStore(result, lhsPtr);
    }

    // Prefix unary is: MINUS_MINUS postfixUnary
    if (ctx->MINUS_MINUS()) {
        llvm::Value* lhsPtr = value.as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        llvm::Value* result = conversionsManager->getPrefixMinusMinusInst(lhs);
        builder->CreateStore(result, lhsPtr);
    }

    // Prefix unary is: NOT postfixUnary
    if (ctx->NOT()) {
        llvm::Value* lhsPtr = value.as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        llvm::Value* result = conversionsManager->getNotInst(lhs);
        builder->CreateStore(result, lhsPtr);
    }

    return value;
}

antlrcpp::Any GeneratorVisitor::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext* ctx) {
    auto value = visit(ctx->castExpr());

    // Postfix unary is: PLUS_PLUS atomicExpr
    if (ctx->PLUS_PLUS()) {
        llvm::Value* lhsPtr = value.as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        llvm::Value* result = conversionsManager->getPostfixPlusPlusInst(lhs);
        builder->CreateStore(result, lhsPtr);
    }

    // Postfix unary is: MINUS_MINUS atomicExpr
    if (ctx->MINUS_MINUS()) {
        llvm::Value* lhsPtr = value.as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        llvm::Value* result = conversionsManager->getPostfixMinusMinusInst(lhs);
        builder->CreateStore(result, lhsPtr);
    }

    return value;
}

antlrcpp::Any GeneratorVisitor::visitCastExpr(SpiceParser::CastExprContext* ctx) {
    auto value = visit(ctx->atomicExpr());

    if (ctx->LPAREN()) { // Cast operator is applied
        llvm::Type* dstTy = visit(ctx->dataType()).as<llvm::Type*>();
        llvm::Value* rhsPtr = value.as<llvm::Value*>();
        llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
        llvm::Value* result = conversionsManager->getCastInst(dstTy, rhs);
        llvm::Value* resultPtr = builder->CreateAlloca(result->getType());
        builder->CreateStore(result, resultPtr);
        return resultPtr;
    }

    return value;
}

antlrcpp::Any GeneratorVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext* ctx) {
    if (ctx->value()) return visit(ctx->value());
    if (ctx->idenValue()) return visit(ctx->idenValue());
    if (ctx->builtinCall()) return visit(ctx->builtinCall());
    return visit(ctx->assignExpr());
}

// Returns pointer
antlrcpp::Any GeneratorVisitor::visitIdenValue(SpiceParser::IdenValueContext* ctx) {
    llvm::Type* baseType;
    llvm::Value* basePtr;
    std::vector<llvm::Value*> indices;
    SymbolTableEntry* entry = nullptr;
    unsigned int tokenCounter = 0;
    unsigned int assignCounter = 0;
    unsigned int functionCallCounter = 0;
    unsigned int referenceOperations = 0;
    unsigned int dereferenceOperations = 0;
    bool metStruct = false;
    SymbolTable* scope = currentScope;

    // Consider referencing operators
    referenceOperations += ctx->BITWISE_AND().size();
    tokenCounter += referenceOperations;

    // Consider de-referencing operator
    dereferenceOperations += ctx->MUL().size();
    tokenCounter += dereferenceOperations;

    // Loop through children
    while (tokenCounter < ctx->children.size()) {
        auto* token = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[tokenCounter]);
        if (!token) { // Got rule context / non terminal symbol
            auto* rule = dynamic_cast<antlr4::RuleContext*>(ctx->children[tokenCounter]);
            unsigned int ruleIndex = rule->getRuleIndex();
            if (ruleIndex == SpiceParser::RuleFunctionCall) { // Consider function call
                if (entry && (entry->getType().is(TY_STRUCT) || entry->getType().isPointerOf(TY_STRUCT))) {
                    currentThisValue = basePtr;
                    currentSymbolType = entry->getType();
                }
                // Set function call parent scope
                functionCallParentScope = scope;
                // Visit function call
                basePtr = visit(ctx->functionCall()[functionCallCounter]).as<llvm::Value*>();
                baseType = basePtr->getType()->getPointerElementType();
                currentThisValue = nullptr;
                indices.clear();
                indices.push_back(builder->getInt32(0));
                functionCallCounter++;
            }
        } else if (token->getSymbol()->getType() == SpiceParser::IDENTIFIER) { // Consider identifier
            // Apply field
            std::string variableName = token->toString();
            entry = scope->lookup(variableName);
            if (metStruct) { // Struct
                indices.push_back(builder->getInt32(entry->getOrderIndex()));
            } else { // Local, global or imported global variable
                if (scope->isImported()) { // Imported global variable
                    // Initialize external global variable
                    baseType = getTypeForSymbolType(entry->getType());
                    basePtr = module->getOrInsertGlobal(variableName, baseType);
                    // Set some attributes to it
                    llvm::GlobalVariable* global = module->getNamedGlobal(variableName);
                    //global->setLinkage(llvm::GlobalValue::ExternalWeakLinkage);
                    global->setDSOLocal(true);
                    global->setExternallyInitialized(true);
                } else { // Local or global variable
                    baseType = entry->getLLVMType();
                    basePtr = entry->getAddress();
                    indices.push_back(builder->getInt32(0));
                    metStruct = entry->getType().is(TY_STRUCT) || entry->getType().isPointerOf(TY_STRUCT);
                }
            }
        } else if (token->getSymbol()->getType() == SpiceParser::DOT) { // Consider dot operator
            // De-reference automatically if it is a struct pointer
            if (entry->getType().is(TY_STRUCT) || entry->getType().isPointerOf(TY_STRUCT)) {
                SymbolType symbolType = entry->getType();
                // Start auto-de-referencing
                if (entry->getType().isPointerOf(TY_STRUCT)) {
                    basePtr = builder->CreateInBoundsGEP(baseType, basePtr, indices);
                    basePtr = builder->CreateLoad(basePtr->getType()->getPointerElementType(), basePtr);
                    indices.clear();
                    indices.push_back(builder->getInt32(0));
                    symbolType = symbolType.getContainedTy();
                }
                // Change to new scope
                std::string structName = symbolType.getSubType();
                scope = scope->lookupTable("struct:" + structName);
                // Check if the table exists
                if (!scope)
                    throw IRError(*token->getSymbol(), VARIABLE_NOT_FOUND,
                                  "Compiler error: Referenced undefined struct '" + structName + "'");
                // Conclude auto-de-referencing
                if (entry->getType().isPointerOf(TY_STRUCT))
                    baseType = scope->lookup(structName)->getLLVMType();
            } else if (entry->getType().is(TY_IMPORT)) {
                // Change to new scope
                std::string importName = entry->getName();
                scope = scope->lookupTable(importName);
            }
        } else if (token->getSymbol()->getType() == SpiceParser::LBRACKET) { // Consider subscript operator
            // Get the index value
            llvm::Value* indexValue = visit(ctx->assignExpr()[assignCounter]).as<llvm::Value*>();
            indexValue = builder->CreateLoad(indexValue->getType()->getPointerElementType(), indexValue);
            if (entry->getType().isPointer()) {
                indices.back() = indexValue;
            } else {
                indices.push_back(indexValue);
            }
            // Increase counters
            assignCounter++;
            tokenCounter += 2; // To consume the assignExpr and the RBRACKET
        }
        // Increase counter
        tokenCounter++;
    }

    // Build GEP instruction
    llvm::Value* returnAddress = builder->CreateInBoundsGEP(baseType, basePtr, indices);

    // If de-referencing operators are present, add a zero index at the end of the gep instruction for each
    for (unsigned int i = 0; i < dereferenceOperations; i++) {
        returnAddress = builder->CreateLoad(returnAddress->getType()->getPointerElementType(), returnAddress);
    }

    // If referencing operators are present, store the calculated address into memory for each
    for (unsigned int i = 0; i < referenceOperations; i++) {
        returnAddress = builder->CreateAlloca(basePtr->getType());
        builder->CreateStore(basePtr, returnAddress);
    }

    // Return the calculated memory address
    return returnAddress;
}

antlrcpp::Any GeneratorVisitor::visitValue(SpiceParser::ValueContext* ctx) {
    llvm::Value* llvmValue;

    // Value is a double constant
    if (ctx->DOUBLE()) {
        currentSymbolType = SymbolType(TY_DOUBLE);
        double value = std::stod(ctx->DOUBLE()->toString());
        llvmValue = llvm::ConstantFP::get(*context, llvm::APFloat(value));
    }

    // Value is an integer constant
    if (ctx->INTEGER()) {
        currentSymbolType = SymbolType(TY_INT);
        int v = std::stoi(ctx->INTEGER()->toString());
        llvmValue = llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), v);
    }

    // Value is a char constant
    if (ctx->CHAR()) {
        currentSymbolType = SymbolType(TY_CHAR);
        char value = ctx->CHAR()->toString()[1];
        llvmValue = llvm::ConstantInt::getSigned(llvm::Type::getInt8Ty(*context), value);
    }

    // Value is a string constant
    if (ctx->STRING()) {
        currentSymbolType = SymbolType(TY_STRING);
        std::string value = ctx->STRING()->toString();
        value = value.substr(1, value.size() - 2);
        llvmValue = builder->CreateGlobalStringPtr(value, "", 0, module.get());
    }

    // Value is a boolean constant with value false
    if (ctx->FALSE()) {
        currentSymbolType = SymbolType(TY_BOOL);
        llvmValue = builder->getFalse();
    }

    // Value is a boolean constant with value true
    if (ctx->TRUE()) {
        currentSymbolType = SymbolType(TY_BOOL);
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
        currentSymbolType = SymbolType(TY_DOUBLE);
    } else if (ctx->TYPE_INT()) { // Data type is int
        currentSymbolType = SymbolType(TY_INT);
    } else if (ctx->TYPE_SHORT()) { // Data type is short
        currentSymbolType = SymbolType(TY_SHORT);
    } else if (ctx->TYPE_LONG()) { // Data type is long
        currentSymbolType = SymbolType(TY_LONG);
    } else if (ctx->TYPE_BYTE()) { // Data type is byte
        currentSymbolType = SymbolType(TY_BYTE);
    } else if (ctx->TYPE_CHAR()) { // Data type is char
        currentSymbolType = SymbolType(TY_CHAR);
    } else if (ctx->TYPE_STRING()) { // Data type is string
        currentSymbolType = SymbolType(TY_STRING);
    } else if (ctx->TYPE_BOOL()) { // Data type is bool
        currentSymbolType = SymbolType(TY_BOOL);
    } else if (ctx->TYPE_DYN()) { // Data type is dyn
        SymbolTableEntry* symbolTableEntry = currentScope->lookup(currentVar);
        currentSymbolType = symbolTableEntry->getType();
    } else if (ctx->IDENTIFIER()) { // Custom data type
        currentSymbolType = SymbolType(TY_STRUCT, ctx->IDENTIFIER()->toString());
    }

    // Check for de-referencing operators
    for (unsigned int i = 0; i < ctx->MUL().size(); i++)
        currentSymbolType = currentSymbolType.toPointer();

    // Check for array brackets pairs
    for (unsigned int i = 0; i < ctx->LBRACKET().size(); i++)
        currentSymbolType = currentSymbolType.toArray();

    // Come up with the LLVM type
    llvm::Type* type = getTypeForSymbolType(currentSymbolType);
    // Throw an error if something went wrong.
    // This should technically never occur because of the semantic analysis
    if (!type) throw IRError(*ctx->TYPE_DYN()->getSymbol(), UNEXPECTED_DYN_TYPE_IR, "Dyn was other");
    return type;
}

void GeneratorVisitor::initializeExternalFunctions() {
    // printf function
    llvm::FunctionType* fctTy = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context),
                                                        llvm::Type::getInt8PtrTy(*context), true);
    module->getOrInsertFunction("printf", fctTy);
}

llvm::Value* GeneratorVisitor::createAddInst(llvm::Value* lhs, llvm::Type* lhsType, llvm::Value* rhs, llvm::Type* rhsType) {
    if (lhsType->isDoubleTy()) { // double
        if (rhsType->isDoubleTy()) { // double
            // double + double
            return builder->CreateFAdd(lhs, rhs, "add");
        } else if (rhsType->isIntegerTy(32)) { // int
            // double + int
            return builder->CreateFAdd(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "add");
        } else if (rhsType->isIntegerTy(8)) { // byte
            // double + byte
            return builder->CreateFAdd(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "add");
        } else if (rhsType->isPointerTy() && rhsType->getPointerElementType()->isIntegerTy(8)) { // string
            // double + string
            // ToDo(@marcauberer): Insert call to toString(double) and concatStrings
            throw IRError(COMING_SOON_IR, "The compiler does not support the '+' operator for lhs=double and rhs=string yet");
        }
    } else if (lhsType->isIntegerTy(32)) { // int
        if (rhsType->isDoubleTy()) { // double
            // int + double
            return builder->CreateFAdd(builder->CreateSIToFP(rhs, rhs->getType()), rhs, "add");
        } else if (rhsType->isIntegerTy(32)) { // int
            // int + int
            return builder->CreateAdd(lhs, rhs, "add");
        } else if (rhsType->isIntegerTy(8)) { // byte
            // int + byte
            return builder->CreateAdd(lhs, rhs, "add");
        } else if (rhsType->isPointerTy() && rhsType->getPointerElementType()->isIntegerTy(8)) {
            // int + string
            // ToDo(@marcauberer): Insert call to toString(int) and concatStrings
            throw IRError(COMING_SOON_IR, "The compiler does not support the '+' operator for lhs=int and rhs=string yet");
        }
    } else if (lhsType->isIntegerTy(8)) { // byte
        if (rhsType->isDoubleTy()) { // double
            // byte + double
            return builder->CreateFAdd(builder->CreateSIToFP(rhs, rhs->getType()), rhs, "add");
        } else if (rhsType->isIntegerTy(32)) { // int
            // byte + int
            return builder->CreateAdd(lhs, rhs, "add");
        } else if (rhsType->isIntegerTy(8)) { // byte
            // byte + byte
            return builder->CreateAdd(lhs, rhs, "add");
        } else if (rhsType->isPointerTy() && rhsType->getPointerElementType()->isIntegerTy(8)) { // string
            // byte + string
            // ToDo(@marcauberer): Insert call to toString(byte) and concatStrings
            throw IRError(COMING_SOON_IR, "The compiler does not support the '+' operator for lhs=byte and rhs=string yet");
        }
    } else if (lhsType->isPointerTy() && lhsType->getPointerElementType()->isIntegerTy(8)) { // string
        if (rhsType->isDoubleTy()) { // double
            // string + double
            // ToDo(@marcauberer): Insert call to toString(double) and concatStrings
            throw IRError(COMING_SOON_IR, "The compiler does not support the '+' operator for lhs=string and rhs=double yet");
        } else if (rhsType->isIntegerTy(32)) { // int
            // string + int
            // ToDo(@marcauberer): Insert call to toString(int) and concatStrings
            throw IRError(COMING_SOON_IR, "The compiler does not support the '+' operator for lhs=string and rhs=int yet");
        } else if (rhsType->isIntegerTy(8)) { // byte
            // string + byte
            // ToDo(@marcauberer): Insert call to toString(byte) and concatStrings
            throw IRError(COMING_SOON_IR, "The compiler does not support the '+' operator for lhs=string and rhs=byte yet");
        } else if (rhsType->isPointerTy() && rhsType->getPointerElementType()->isIntegerTy(8)) { // string
            // string + string
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '+' operator for lhs=string and rhs=string yet");
        }
    }
    return lhs;
}

llvm::Value* GeneratorVisitor::createSubInst(llvm::Value* lhs, llvm::Type* lhsType, llvm::Value* rhs, llvm::Type* rhsType) {
    if (lhsType->isDoubleTy()) { // double
        if (rhsType->isDoubleTy()) { // double
            // double - double
            return builder->CreateFSub(lhs, rhs, "sub");
        } else if (rhsType->isIntegerTy(32)) { // int
            // double - int
            return builder->CreateFSub(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "sub");
        } else if (rhsType->isIntegerTy(8)) { // byte
            // double - byte
            return builder->CreateFSub(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "sub");
        }
    } else if (lhsType->isIntegerTy(32)) { // int
        if (rhsType->isDoubleTy()) { // double
            // int - double
            return builder->CreateFSub(builder->CreateSIToFP(rhs, rhs->getType()), rhs, "sub");
        } else if (rhsType->isIntegerTy(32)) { // int
            // int - int
            return builder->CreateSub(lhs, rhs, "sub");
        } else if (rhsType->isIntegerTy(8)) { // byte
            // int - byte
            return builder->CreateSub(lhs, rhs, "sub");
        }
    } else if (lhsType->isIntegerTy(8)) { // byte
        if (rhsType->isDoubleTy()) { // double
            // byte - double
            return builder->CreateFSub(lhs, rhs, "sub");
        } else if (rhsType->isIntegerTy(32)) { // int
            // byte - int
            return builder->CreateFSub(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "sub");
        } else if (rhsType->isIntegerTy(8)) { // byte
            // byte - byte
            return builder->CreateFSub(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "sub");
        }
    }
    return lhs;
}

llvm::Value* GeneratorVisitor::createMulInst(llvm::Value* lhs, llvm::Type* lhsType, llvm::Value* rhs, llvm::Type* rhsType) {
    if (lhsType->isDoubleTy()) { // double
        if (rhsType->isDoubleTy()) { // double
            // double * double
            lhs = builder->CreateFMul(lhs, rhs, "mul");
        } else if (rhsType->isIntegerTy(32)) { // int
            // double * int
            lhs = builder->CreateFMul(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "mul");
        } else if (rhsType->isIntegerTy(8)) { // byte
            // double * int
            lhs = builder->CreateFMul(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "mul");
        }
    } else if (lhsType->isIntegerTy(32)) { // int
        if (rhsType->isDoubleTy()) { // double
            // int * double
            lhs = builder->CreateFMul(builder->CreateSIToFP(rhs, rhs->getType()), rhs, "mul");
        } else if (rhsType->isIntegerTy(32)) { // int
            // int * int
            lhs = builder->CreateMul(lhs, rhs, "mul");
        } else if (rhsType->isIntegerTy(8)) { // byte
            // int * byte
            lhs = builder->CreateMul(lhs, rhs, "mul");
        } else if (rhsType->isPointerTy() && rhsType->getPointerElementType()->isIntegerTy(8)) { // string
            // int * string
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=int and rhs=string yet");
        }
    } else if (lhsType->isPointerTy() && lhsType->getPointerElementType()->isIntegerTy(8)) { // string
        if (rhsType->isIntegerTy(32)) { // int
            // string * int
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON_IR, "The compiler does not support the '*' operator for lhs=string and rhs=int yet");
        }
    }
    return lhs;
}

llvm::Value* GeneratorVisitor::createDivInst(llvm::Value* lhs, llvm::Type* lhsType, llvm::Value* rhs, llvm::Type* rhsType) {
    if (lhsType->isDoubleTy()) { // double
        if (rhsType->isDoubleTy()) { // double
            // double : double
            lhs = builder->CreateFDiv(lhs, rhs, "div");
        } else if (rhsType->isIntegerTy(32)) { // int
            // double : int
            lhs = builder->CreateFDiv(lhs, builder->CreateSIToFP(rhs, lhs->getType()), "div");
        }
    } else if (lhsType->isIntegerTy(32)) { // int
        if (rhsType->isDoubleTy()) { // double
            // int : double
            lhs = builder->CreateFDiv(builder->CreateSIToFP(rhs, rhs->getType()), rhs, "div");
        } else if (rhsType->isIntegerTy(32)) { // int
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

    // Get base symbol type
    std::stack<SymbolSuperType> pointerArrayList;
    while (symbolType.isPointer() || symbolType.isArray()) {
        pointerArrayList.push(symbolType.isPointer() ? TY_PTR : TY_ARRAY);
        symbolType = symbolType.getContainedTy();
    }

    llvm::Type* llvmBaseType;
    switch (symbolType.getSuperType()) {
        case TY_DOUBLE: {
            llvmBaseType = llvm::Type::getDoubleTy(*context);
            break;
        }
        case TY_INT: {
            llvmBaseType = llvm::Type::getInt32Ty(*context);
            break;
        }
        case TY_SHORT: {
            llvmBaseType = llvm::Type::getInt16Ty(*context);
            break;
        }
        case TY_LONG: {
            llvmBaseType = llvm::Type::getInt64Ty(*context);
            break;
        }
        case TY_BYTE:
        case TY_CHAR: {
            llvmBaseType = llvm::Type::getInt8Ty(*context);
            break;
        }
        case TY_STRING: {
            llvmBaseType = llvm::Type::getInt8PtrTy(*context);
            break;
        }
        case TY_BOOL: {
            llvmBaseType = llvm::Type::getInt1Ty(*context);
            break;
        }
        case TY_STRUCT: {
            llvmBaseType = currentScope->lookup(symbolType.getSubType())->getLLVMType();
            break;
        }
        default: throw std::runtime_error("Internal compiler error: Cannot determine LLVM type of " + symbolType.getName());
    }

    // Consider pointer/array hierarchy
    while (!pointerArrayList.empty()) {
        if (pointerArrayList.top() == TY_PTR) { // Pointer
            llvmBaseType = llvmBaseType->getPointerTo();
        } else { // Array
            llvmBaseType = llvm::ArrayType::get(llvmBaseType, 0);
        }
        pointerArrayList.pop();
    }

    return llvmBaseType;
}

llvm::Value* GeneratorVisitor::getDefaultValueForSymbolType(SymbolType symbolType) {
    switch (symbolType.getSuperType()) {
        case TY_DOUBLE:
            return llvm::ConstantFP::get(*context, llvm::APFloat(0.0));
        case TY_INT:
            return llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), 0);
        case TY_SHORT:
            return llvm::ConstantInt::getSigned(llvm::Type::getInt16Ty(*context), 0);
        case TY_LONG:
            return llvm::ConstantInt::getSigned(llvm::Type::getInt64Ty(*context), 0);
        case TY_BYTE:
        case TY_CHAR:
            return llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context), 0);
        case TY_STRING:
            return builder->CreateGlobalStringPtr("", "", 0, module.get());
        case TY_BOOL:
            return builder->getFalse();
        default:
            throw std::runtime_error("Internal compiler error: Cannot determine default value of " + symbolType.getName());
    }
}

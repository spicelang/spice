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
        llvm::Type* returnType = getTypeFromSymbolType(SymbolType(TYPE_INT));
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
        llvm::Value* zero = llvm::ConstantInt::getSigned(getTypeFromSymbolType(SymbolType(TYPE_INT)), 0);
        builder->CreateStore(zero, currentScope->lookup(RETURN_VARIABLE_NAME)->getAddress());

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
        return (llvm::Value*) llvm::ConstantInt::getTrue(*context);
    }
    // Return false as result for the function definition
    return (llvm::Value*) llvm::ConstantInt::getFalse(*context);
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
            builder->CreateStore(&param, memAddress);
        } else {
            std::string paramName = paramNames[paramNo];
            llvm::Type* paramType = fct->getFunctionType()->getParamType(paramNo);
            llvm::Value* memAddress = builder->CreateAlloca(paramType, nullptr, paramName);
            currentScope->lookup(paramName)->updateAddress(memAddress);
            builder->CreateStore(&param, memAddress);
        }
    }

    // Declare result variable
    llvm::Value* returnMemAddress = builder->CreateAlloca(returnType, nullptr, RETURN_VARIABLE_NAME);
    currentScope->lookup(RETURN_VARIABLE_NAME)->updateAddress(returnMemAddress);

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
    return (llvm::Value*) llvm::ConstantInt::getTrue(*context);
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
            builder->CreateStore(&param, memAddress);
        } else {
            std::string paramName = paramNames[paramNo];
            llvm::Type* paramType = proc->getFunctionType()->getParamType(paramNo);
            llvm::Value* memAddress = builder->CreateAlloca(paramType, nullptr, paramName);
            currentScope->lookup(paramName)->updateAddress(memAddress);
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
    return (llvm::Value*) llvm::ConstantInt::getTrue(*context);
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
    return (llvm::Value*) llvm::ConstantInt::getTrue(*context);
}

antlrcpp::Any GeneratorVisitor::visitForLoop(SpiceParser::ForLoopContext* ctx) {
    llvm::Function* parentFct = builder->GetInsertBlock()->getParent();

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
    visit(ctx->assignExpr()[0]);
    // Jump into condition block
    createBr(bCond);

    // Fill condition block
    parentFct->getBasicBlockList().push_back(bCond);
    moveInsertPointToBlock(bCond);
    llvm::Value* conditionValue = visit(ctx->assignExpr()[1]).as<llvm::Value*>();
    // Jump to loop body or to loop end
    createCondBr(conditionValue, bLoop, bLoopEnd);

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
    parentFct->getBasicBlockList().push_back(bLoopEnd);
    moveInsertPointToBlock(bLoopEnd);

    // Change scope back
    currentScope = currentScope->getParent();

    // Return true as result for the loop
    return (llvm::Value*) llvm::ConstantInt::getTrue(*context);
}

antlrcpp::Any GeneratorVisitor::visitWhileLoop(SpiceParser::WhileLoopContext* ctx) {
    llvm::Function* parentFct = builder->GetInsertBlock()->getParent();

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
    llvm::Value* conditionValue = visit(ctx->assignExpr()).as<llvm::Value*>();
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
    return llvm::ConstantInt::getTrue(*context);
}

antlrcpp::Any GeneratorVisitor::visitStmtLst(SpiceParser::StmtLstContext* ctx) {
    for (auto& child : ctx->children) {
        if (!blockAlreadyTerminated) visit(child);
    }
    return nullptr;
}

antlrcpp::Any GeneratorVisitor::visitIfStmt(SpiceParser::IfStmtContext* ctx) {
    llvm::Value* conditionValue = visit(ctx->assignExpr()).as<llvm::Value*>();
    llvm::Function* parentFct = builder->GetInsertBlock()->getParent();

    // Create blocks
    llvm::BasicBlock* bThen = llvm::BasicBlock::Create(*context, "then");
    llvm::BasicBlock* bElse = llvm::BasicBlock::Create(*context, "else");
    llvm::BasicBlock* bEnd = llvm::BasicBlock::Create(*context, "end");

    // Check if condition is fulfilled
    createCondBr(conditionValue, bThen, ctx->elseStmt() ? bElse : bEnd);

    // Change scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->getChild(scopeId);

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
    return conditionValue;
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

    // Is this the global scope?
    if (currentScope->getParent()) { // Local scope
        // Create local variable
        llvm::Value* memAddress = builder->CreateAlloca(varType, nullptr, currentVar);
        currentScope->lookup(currentVar)->updateAddress(memAddress);
    } else { // Global scope
        // Create global variable
        llvm::Value* memAddress = module->getOrInsertGlobal(currentVar, varType);
        currentScope->lookup(currentVar)->updateAddress(memAddress);
        // Set some attributes to it
        llvm::GlobalVariable* global = module->getNamedGlobal(currentVar);
        //global->setLinkage(llvm::GlobalValue::ExternalWeakLinkage);
        global->setConstant(ctx->CONST());
        global->setDSOLocal(true);
    }

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
            llvm::Type* returnType = getTypeFromSymbolType(symbolTypes[0]);
            if (!returnType) throw std::runtime_error("Internal error");

            // Get parameter types
            std::vector<llvm::Type*> paramTypes;
            for (int i = 1; i < symbolTypes.size(); i++) {
                llvm::Type* paramType = getTypeFromSymbolType(symbolTypes[i]);
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
                llvm::Type* paramType = getTypeFromSymbolType(symbolTypes[i]);
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
            llvm::Value* argValue = visit(ctx->paramLst()->assignExpr()[i]).as<llvm::Value*>();
            llvm::Type* argType = fctType->getParamType(i);
            llvm::Value* bitCastArgValue = builder->CreateBitCast(argValue, argType);
            argValues.push_back(bitCastArgValue);
        }
    }

    return (llvm::Value*) builder->CreateCall(fct, argValues);
}

antlrcpp::Any GeneratorVisitor::visitNewStmt(SpiceParser::NewStmtContext* ctx) {
    std::string structName = ctx->IDENTIFIER()->toString();
    std::string structScope = ScopeIdUtil::getScopeId(ctx);

    // Get struct type from symbol table
    llvm::Type* structType = currentScope->lookup(structName)->getLLVMType();

    // Allocate space for struct
    llvm::Value* structAddress = builder->CreateAlloca(structType, nullptr);

    // Fill the struct with the stated values
    SymbolTable* structSymbolTable = currentScope->lookupTable({ structScope });
    for (unsigned int i = 0; i < ctx->paramLst()->assignExpr().size(); i++) {
        // Visit assignment
        llvm::Value* assignment = visit(ctx->paramLst()->assignExpr()[i]).as<llvm::Value*>();
        // Get pointer to struct element
        llvm::Value* fieldAddress = builder->CreateStructGEP(structType, structAddress, i);
        // Store value to address
        builder->CreateStore(assignment, fieldAddress);
    }

    return (llvm::Value*) builder->CreateLoad(structType, structAddress, structName);
}

antlrcpp::Any GeneratorVisitor::visitArrayInitStmt(SpiceParser::ArrayInitStmtContext* ctx) {
    // Get size and data type
    unsigned int currentArraySize = std::stoi(ctx->value()->INTEGER()->toString());
    llvm::Type* baseType = visit(ctx->dataType()).as<llvm::Type*>();
    llvm::Type* arrayType = llvm::ArrayType::get(baseType, currentArraySize);

    // Allocate array
    llvm::Value* arrayAddress = builder->CreateAlloca(arrayType);

    // Fill items with the stated values
    if (ctx->paramLst()) {
        for (unsigned int i = 0; i < ctx->paramLst()->assignExpr().size(); i++) {
            llvm::Value* itemValue = visit(ctx->paramLst()->assignExpr()[i]).as<llvm::Value*>();
            // Calculate item address
            llvm::Value* index = llvm::ConstantInt::get(getTypeFromSymbolType(SymbolType(TYPE_INT)), i);
            std::vector<llvm::Value*> indices;
            indices.push_back(llvm::ConstantInt::get(getTypeFromSymbolType(SymbolType(TYPE_INT)), 0));
            indices.push_back(llvm::ConstantInt::get(getTypeFromSymbolType(SymbolType(TYPE_INT)), i));
            llvm::Value* itemAddress = builder->CreateInBoundsGEP(arrayType, arrayAddress, indices);
            // Store value to item address
            builder->CreateStore(itemValue, itemAddress);
        }
    }

    // Update address in symbol table
    std::string varName = ctx->IDENTIFIER()->toString();
    currentScope->lookup(varName)->updateAddress(arrayAddress);

    return (llvm::Value*) llvm::ConstantInt::getTrue(*context);
}

antlrcpp::Any GeneratorVisitor::visitImportStmt(SpiceParser::ImportStmtContext* ctx) {
    // Ignore sub-tree
    return nullptr;
}

antlrcpp::Any GeneratorVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext* ctx) {
    llvm::Value* returnValue = visit(ctx->assignExpr()).as<llvm::Value*>();
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
    llvm::Function* printf = module->getFunction("printf");
    std::vector<llvm::Value*> printfArgs;
    std::string stringTemplate = ctx->STRING()->toString();
    stringTemplate = stringTemplate.substr(1, stringTemplate.size() - 2);
    printfArgs.push_back(builder->CreateGlobalStringPtr(stringTemplate));
    for (auto& arg : ctx->assignExpr()) {
        llvm::Value* argVal = visit(arg).as<llvm::Value*>();
        if (argVal->getType()->isIntegerTy(1))
            argVal = builder->CreateZExt(argVal, llvm::Type::getInt32Ty(*context));
        if (argVal == nullptr) throw IRError(*arg->start, PRINTF_NULL_TYPE, "'" + arg->getText() + "' is null");
        printfArgs.push_back(argVal);
    }
    return builder->CreateCall(printf, printfArgs);
}

antlrcpp::Any GeneratorVisitor::visitAssignExpr(SpiceParser::AssignExprContext* ctx) {
    // Get value of right side
    llvm::Value* rhs;
    if (ctx->ternaryExpr()) rhs = visit(ctx->ternaryExpr()).as<llvm::Value*>();
    if (ctx->newStmt()) rhs = visit(ctx->newStmt()).as<llvm::Value*>();

    if (ctx->declStmt() || !ctx->IDENTIFIER().empty()) {
        // Get value of left side
        std::string varName;
        SymbolTableEntry* variableEntry;
        std::vector<antlr4::tree::TerminalNode*> idenList;
        if (ctx->declStmt()) { // Variable was declared in this line
            varName = visit(ctx->declStmt()).as<std::string>();
            // Get symbol table entry
            variableEntry = currentScope->lookup(varName);
            idenList.push_back(ctx->declStmt()->IDENTIFIER());
        } else { // Variable was declared before and is referenced here
            varName = ctx->IDENTIFIER()[0]->toString();
            // Get symbol table entry
            variableEntry = currentScope->lookup(varName);
            idenList = ctx->IDENTIFIER();
        }

        if (ctx->ASSIGN_OP()) {
            // Store right side on the left one
            if (variableEntry->isLocal()) { // Local variable
                llvm::Value* lhs = getAddressByIdenList(currentScope, idenList);
                if (ctx->MUL()) lhs = builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                builder->CreateStore(rhs, lhs);
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
                llvm::Value* lhs = getAddressByIdenList(currentScope, idenList);
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = createAddInst(loadLhs, loadLhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhs);
            } else { // Global variable
                llvm::GlobalVariable* lhs = module->getNamedGlobal(varName);
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = createAddInst(loadLhs, loadLhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhs);
            }
        } else if (ctx->MINUS_EQUAL()) {
            if (variableEntry->isLocal()) { // Local variable
                llvm::Value* lhs = getAddressByIdenList(currentScope, idenList);
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = createSubInst(lhs, lhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhs);
            } else { // Global variable
                llvm::GlobalVariable* lhs = module->getNamedGlobal(varName);
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = createSubInst(lhs, lhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhs);
            }
        } else if (ctx->MUL_EQUAL()) {
            if (variableEntry->isLocal()) { // Local variable
                llvm::Value* lhs = getAddressByIdenList(currentScope, idenList);
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = createMulInst(lhs, lhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhs);
            } else { // Global variable
                llvm::GlobalVariable* lhs = module->getNamedGlobal(varName);
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = createMulInst(lhs, lhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhs);
            }
        } else if (ctx->DIV_EQUAL()) {
            if (variableEntry->isLocal()) { // Local variable
                llvm::Value* lhs = getAddressByIdenList(currentScope, idenList);
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = createDivInst(lhs, lhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhs);
            } else { // Global variable
                llvm::GlobalVariable* lhs = module->getNamedGlobal(varName);
                llvm::Value* loadLhs = (llvm::Value*) builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
                rhs = createDivInst(lhs, lhs->getType(), rhs, rhs->getType());
                builder->CreateStore(rhs, lhs);
            }
        }
    }

    // Return the value on the right side
    return rhs;
}

antlrcpp::Any GeneratorVisitor::visitTernaryExpr(SpiceParser::TernaryExprContext* ctx) {
    if (ctx->logicalOrExpr().size() > 1) {
        llvm::Value* conditionValue = visit(ctx->logicalOrExpr()[0]).as<llvm::Value*>();
        llvm::Function* parentFct = builder->GetInsertBlock()->getParent();

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
            return llvm::Constant::getNullValue(getTypeFromSymbolType(SymbolType(TYPE_INT)));
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
            if (lhs->getType()->isDoubleTy()) return builder->CreateFCmpOEQ(lhs, rhs, "eq");
            return builder->CreateICmpEQ(lhs, rhs, "eq");
        }

        // Equality expr is: relationalExpr NOT_EQUAL relationalExpr
        if (ctx->NOT_EQUAL()) {
            if (lhs->getType()->isDoubleTy()) return builder->CreateFCmpONE(lhs, rhs, "ne");
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
            if (lhs->getType()->isDoubleTy()) return builder->CreateFCmpOLT(lhs, rhs, "lt");
            return builder->CreateICmpSLT(lhs, rhs, "lt");
        }

        // Relational expr is: additiveExpr GREATER additiveExpr
        if (ctx->GREATER()) {
            if (lhs->getType()->isDoubleTy()) return builder->CreateFCmpOGT(lhs, rhs, "gt");
            return builder->CreateICmpSGT(lhs, rhs, "gt");
        }

        // Relational expr is: additiveExpr LESS_EQUAL additiveExpr
        if (ctx->LESS_EQUAL()) {
            if (lhs->getType()->isDoubleTy()) return builder->CreateFCmpOLE(lhs, rhs, "le");
            return builder->CreateICmpSLE(lhs, rhs, "le");
        }

        // Relational expr is: additiveExpr GREATER_EQUAL additiveExpr
        if (ctx->GREATER_EQUAL()) {
            if (lhs->getType()->isDoubleTy()) return builder->CreateFCmpOGE(lhs, rhs, "ge");
            return builder->CreateICmpSGE(lhs, rhs, "ge");
        }
    }
    return visit(ctx->additiveExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext* ctx) {
    // Check if there is an additive operation attached
    if (ctx->multiplicativeExpr().size() > 1) {
        llvm::Value* lhs = visit(ctx->multiplicativeExpr()[0]).as<llvm::Value*>();
        llvm::Type* lhsType = lhs->getType();
        unsigned int operatorIndex = 1;
        for (int i = 1; i < ctx->multiplicativeExpr().size(); i++) {
            auto* op = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[operatorIndex]);
            auto rhs = visit(ctx->multiplicativeExpr()[i]).as<llvm::Value*>();
            llvm::Type* rhsType = rhs->getType();

            if (op == ctx->getToken(SpiceParser::PLUS, 0))
                lhs = createAddInst(lhs, lhsType, rhs, rhsType);
            else
                lhs = createSubInst(lhs, lhsType, rhs, rhsType);

            operatorIndex += 2;
        }
        return lhs;
    }
    return visit(ctx->multiplicativeExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext* ctx) {
    // Check if there is a multiplicative operation attached
    if (ctx->prefixUnaryExpr().size() > 1) {
        llvm::Value* lhs = visit(ctx->prefixUnaryExpr()[0]).as<llvm::Value*>();
        llvm::Type* lhsType = lhs->getType();
        unsigned int operatorIndex = 1;
        for (int i = 1; i < ctx->prefixUnaryExpr().size(); i++) {
            auto* op = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[operatorIndex]);
            auto rhs = visit(ctx->prefixUnaryExpr()[i]).as<llvm::Value*>();
            llvm::Type* rhsType = rhs->getType();

            if (op == ctx->getToken(SpiceParser::MUL, 0))
                lhs = createMulInst(lhs, lhsType, rhs, rhsType);
            else
                lhs = createDivInst(lhs, lhsType, rhs, rhsType);

            operatorIndex += 2;
        }
        return lhs;
    }
    return visit(ctx->prefixUnaryExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext* ctx) {
    auto value = visit(ctx->postfixUnaryExpr());

    // Prefix unary is: PLUS_PLUS postfixUnary
    if (ctx->PLUS_PLUS()) {
        if (!ctx->postfixUnaryExpr()->atomicExpr()->idenValue())
            throw IRError(*ctx->postfixUnaryExpr()->atomicExpr()->start, BRANCH_NOT_FOUND, "idenValue() was null");
        llvm::Value* rhs = builder->CreateAdd(value.as<llvm::Value*>(), builder->getInt32(1), "pre_pp");
        llvm::Value* lhs = getAddressByIdenList(currentScope,
                                                ctx->postfixUnaryExpr()->atomicExpr()->idenValue()->IDENTIFIER());
        builder->CreateStore(rhs, lhs);
        return lhs;
    }

    // Prefix unary is: MINUS_MINUS postfixUnary
    if (ctx->MINUS_MINUS()) {
        if (!ctx->postfixUnaryExpr()->atomicExpr()->idenValue())
            throw IRError(*ctx->postfixUnaryExpr()->atomicExpr()->start, BRANCH_NOT_FOUND, "idenValue() was null");
        llvm::Value* rhs = builder->CreateSub(value.as<llvm::Value*>(), builder->getInt32(1), "pre_mm");
        llvm::Value* lhs = getAddressByIdenList(currentScope,
                                                ctx->postfixUnaryExpr()->atomicExpr()->idenValue()->IDENTIFIER());
        builder->CreateStore(rhs, lhs);
        return lhs;
    }

    // Prefix unary is: NOT postfixUnary
    if (ctx->NOT()) return builder->CreateNot(value.as<llvm::Value*>(), "not");

    return value;
}

antlrcpp::Any GeneratorVisitor::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext* ctx) {
    auto value = visit(ctx->atomicExpr());

    // Postfix unary is: PLUS_PLUS atomicExpr
    if (ctx->PLUS_PLUS()) {
        if (!ctx->atomicExpr()->idenValue()) throw IRError(*ctx->atomicExpr()->start, BRANCH_NOT_FOUND, "idenValue() was null");
        llvm::Value* rhs = builder->CreateAdd(value.as<llvm::Value*>(), builder->getInt32(1), "post_pp");
        llvm::Value* lhs = getAddressByIdenList(currentScope, ctx->atomicExpr()->idenValue()->IDENTIFIER());
        builder->CreateStore(rhs, lhs);
        return rhs;
    }

    // Postfix unary is: MINUS_MINUS atomicExpr
    if (ctx->MINUS_MINUS()) {
        if (!ctx->atomicExpr()->idenValue()) throw IRError(*ctx->atomicExpr()->start, BRANCH_NOT_FOUND, "idenValue() was null");
        llvm::Value* rhs = builder->CreateSub(value.as<llvm::Value*>(), builder->getInt32(1), "post_mm");
        llvm::Value* lhs = getAddressByIdenList(currentScope, ctx->atomicExpr()->idenValue()->IDENTIFIER());
        builder->CreateStore(rhs, lhs);
        return rhs;
    }

    return value;
}

antlrcpp::Any GeneratorVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext* ctx) {
    if (ctx->value()) return visit(ctx->value());
    if (ctx->idenValue()) return visit(ctx->idenValue());
    if (ctx->functionCall()) return visit(ctx->functionCall());
    return visit(ctx->assignExpr());
}

antlrcpp::Any GeneratorVisitor::visitIdenValue(SpiceParser::IdenValueContext* ctx) {
    std::string variableName = IdentifierUtil::getVarNameFromIdentList(ctx->IDENTIFIER());
    SymbolTableEntry* variableSymbol = IdentifierUtil::getSymbolTableEntryByIdenList(currentScope, ctx->IDENTIFIER());
    currentSymbolType = variableSymbol->getType();

    llvm::Value* var;
    if (variableSymbol->isLocal()) { // Local variable
        var = getAddressByIdenList(currentScope, ctx->IDENTIFIER());
    } else { // Global variable
        var = module->getNamedGlobal(variableName);
    }

    // Throw an error when the variable is null
    if (!var)
        throw IRError(*ctx->IDENTIFIER()[0]->getSymbol(), VARIABLE_NOT_FOUND,
                      "Variable '" + variableName + "' not found in code generation step");

    // Consider subscript operator
    if (ctx->LBRACKET()) {
        // Calculate item address
        std::vector<llvm::Value*> indices;
        indices.push_back(llvm::ConstantInt::get(getTypeFromSymbolType(SymbolType(TYPE_INT)), 0));
        indices.push_back(visit(ctx->assignExpr()).as<llvm::Value*>());
        var = builder->CreateInBoundsGEP(getTypeFromSymbolType(variableSymbol->getType()), var, indices);
    }

    // If the reference operator is attached, return immediately. Load and return otherwise
    if (ctx->BITWISE_AND()) return var;

    // Load value
    var = builder->CreateLoad(var->getType()->getPointerElementType(), var);

    // If the de-reference operator is attached load twice, otherwise load once
    if (ctx->MUL()) var = builder->CreateLoad(var->getType()->getPointerElementType(), var);

    return var;
}

antlrcpp::Any GeneratorVisitor::visitValue(SpiceParser::ValueContext* ctx) {
    // Value is a double constant
    if (ctx->DOUBLE()) {
        currentSymbolType = SymbolType(TYPE_DOUBLE);
        double value = std::stod(ctx->DOUBLE()->toString());
        return (llvm::Value*) llvm::ConstantFP::get(*context, llvm::APFloat(value));
    }

    // Value is an integer constant
    if (ctx->INTEGER()) {
        currentSymbolType = SymbolType(TYPE_INT);
        int v = std::stoi(ctx->INTEGER()->toString());
        return (llvm::Value*)
            llvm::ConstantInt::getSigned(getTypeFromSymbolType(SymbolType(TYPE_INT)), v);
    }

    // Value is a string constant
    if (ctx->STRING()) {
        currentSymbolType = SymbolType(TYPE_STRING);
        std::string value = ctx->STRING()->toString();
        value = value.substr(1, value.size() - 2);
        return (llvm::Value*) builder->CreateGlobalStringPtr(value, "", 0, module.get());
    }

    // Value is a boolean constant with value false
    if (ctx->FALSE()) {
        currentSymbolType = SymbolType(TYPE_BOOL);
        return (llvm::Value*) llvm::ConstantInt::getFalse(*context);
    }

    // Value is a boolean constant with value true
    if (ctx->TRUE()) {
        currentSymbolType = SymbolType(TYPE_BOOL);
        return (llvm::Value*) llvm::ConstantInt::getTrue(*context);
    }

    return nullptr;
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
    llvm::Type* type = getTypeFromSymbolType(currentSymbolType);
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
            throw IRError(COMING_SOON, "The compiler does not support the '+' operator for lhs=double and rhs=string yet");
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
            throw IRError(COMING_SOON, "The compiler does not support the '+' operator for lhs=int and rhs=string yet");
        }
    } else if (lhsType->isPointerTy() && lhsType->getPointerElementType()->isIntegerTy(8)) {
        if (rhsType->isDoubleTy()) {
            // string + double
            // ToDo(@marcauberer): Insert call to toString(double) and concatStrings
            throw IRError(COMING_SOON, "The compiler does not support the '+' operator for lhs=string and rhs=double yet");
        } else if (rhsType->isIntegerTy(32)) {
            // string + int
            // ToDo(@marcauberer): Insert call to toString(int) and concatStrings
            throw IRError(COMING_SOON, "The compiler does not support the '+' operator for lhs=string and rhs=int yet");
        } else if (rhsType->isPointerTy() && rhsType->getPointerElementType()->isIntegerTy(8)) {
            // string + string
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON, "The compiler does not support the '+' operator for lhs=string and rhs=string yet");
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
            throw IRError(COMING_SOON, "The compiler does not support the '*' operator for lhs=int and rhs=string yet");
        }
    } else if (lhsType->isPointerTy() && lhsType->getPointerElementType()->isIntegerTy(8)) {
        if (rhsType->isIntegerTy(32)) {
            // string * int
            // ToDo(@marcauberer): Insert call to concatStrings in the runtime lib
            throw IRError(COMING_SOON, "The compiler does not support the '*' operator for lhs=string and rhs=int yet");
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

llvm::Type* GeneratorVisitor::getTypeFromSymbolType(SymbolType symbolType) {
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

llvm::Value* GeneratorVisitor::getAddressByIdenList(SymbolTable* subTable, std::vector<antlr4::tree::TerminalNode*> idenList) {
    // If it is a one-dimensional identifier return the address immediately
    SymbolTableEntry* symbolTableEntry = subTable->lookup(idenList[0]->toString());
    if (idenList.size() <= 1) return symbolTableEntry->getAddress();

    // Get address for (nested) structs
    llvm::Value* currentAddress = symbolTableEntry->getAddress();

    // Additional load for pointers
    if (symbolTableEntry->getType().isPointer())
        currentAddress = builder->CreateLoad(getTypeFromSymbolType(symbolTableEntry->getType()), currentAddress);

    for (unsigned int i = 1; i < idenList.size(); i++) {
        // Get name of struct
        std::string structName = symbolTableEntry->getType().getSubType();
        // Get symbol table entry of struct
        SymbolTableEntry* structEntry = subTable->lookup(structName);
        // Get symbol table of struct
        SymbolTable* structTable = subTable->lookupTable("struct:" + structName);
        // Get field index of next identifier segment
        symbolTableEntry = structTable->lookup(idenList[i]->toString());
        // Calculate field address
        currentAddress = builder->CreateStructGEP(structEntry->getLLVMType(), currentAddress,
                                                  symbolTableEntry->getOrderIndex());
        // If the result is a pointer -> de-reference it
        if (symbolTableEntry->getType().isPointer() && i < idenList.size() -1)
            currentAddress = builder->CreateLoad(getTypeFromSymbolType(symbolTableEntry->getType()),
                                                 currentAddress);
    }
    return currentAddress;
}

/*llvm::Value* GeneratorVisitor::getAddressByIdenList(SymbolTable* subTable, std::vector<antlr4::tree::TerminalNode*> idenList) {
    std::vector<llvm::Value*> indices;
    llvm::Type* type;
    llvm::Value* ptr;


    // Calculate address with GEP instruction
    llvm::Value* address = builder->CreateInBoundsGEP(type, ptr, indices);
    return address;
}*/
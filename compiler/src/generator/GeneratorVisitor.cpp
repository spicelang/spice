// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "GeneratorVisitor.h"

#include <util/FileUtil.h>
#include <util/ScopeIdUtil.h>
#include <util/CompilerWarning.h>
#include <exception/IRError.h>

#include <analyzer/AnalyzerVisitor.h>
#include <exception/SemanticError.h>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Analysis/AliasAnalysis.h>

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

    // Configure output target
    std::string tripletString = targetTriple.getTriple();
    module->setTargetTriple(tripletString);

    // Search after selected target
    std::string error;
    const llvm::Target* target = llvm::TargetRegistry::lookupTarget(tripletString, error);
    if (!target) throw IRError(TARGET_NOT_AVAILABLE, "Selected target was not found: " + error);

    llvm::TargetOptions opt;
    llvm::Optional rm = llvm::Optional<llvm::Reloc::Model>();
    targetMachine = target->createTargetMachine(tripletString, "generic", "", opt, rm);

    module->setDataLayout(targetMachine->createDataLayout());
}

void GeneratorVisitor::optimize() {
    if (debugOutput) std::cout << "\nOptimizing on level " + std::to_string(optLevel) << " ..." << std::endl;

    llvm::LoopAnalysisManager loopAnalysisMgr;
    llvm::FunctionAnalysisManager functionAnalysisMgr;
    llvm::CGSCCAnalysisManager cgsccAnalysisMgr;
    llvm::ModuleAnalysisManager moduleAnalysisMgr;
    llvm::PassBuilder passBuilder(targetMachine);

    functionAnalysisMgr.registerPass([&] {
        return passBuilder.buildDefaultAAPipeline();
    });

    passBuilder.registerModuleAnalyses(moduleAnalysisMgr);
    passBuilder.registerCGSCCAnalyses(cgsccAnalysisMgr);
    passBuilder.registerFunctionAnalyses(functionAnalysisMgr);
    passBuilder.registerLoopAnalyses(loopAnalysisMgr);
    passBuilder.crossRegisterProxies(loopAnalysisMgr, functionAnalysisMgr, cgsccAnalysisMgr, moduleAnalysisMgr);

    // Run passes
    llvm::PassBuilder::OptimizationLevel llvmOptLevel = getLLVMOptLevelFromSpiceOptLevel();
    llvm::ModulePassManager modulePassMgr = passBuilder.buildPerModuleDefaultPipeline(llvmOptLevel);
    modulePassMgr.run(*module, moduleAnalysisMgr);
}

void GeneratorVisitor::emit() {
    if (debugOutput)
        std::cout << "\nEmitting executable for triplet '" << targetTriple.getTriple() << "' to path: " << objectDir << std::endl;

    // Open file output stream
    std::error_code errorCode;
    llvm::raw_fd_ostream dest(objectDir, errorCode, llvm::sys::fs::OF_None);
    if (errorCode) throw IRError(CANT_OPEN_OUTPUT_FILE, "File '" + objectDir + "' could not be opened");

    llvm::legacy::PassManager passManager;
    if (targetMachine->addPassesToEmitFile(passManager, dest, nullptr, llvm::CGFT_ObjectFile))
        throw IRError(WRONG_TYPE, "Target machine can't emit a file of this type");

    // Emit object file
    passManager.run(*module);
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
            for (auto& param : ctx->paramLstDef()->declStmt()) {
                currentVarName = param->IDENTIFIER()->toString();
                paramNames.push_back(currentVarName);
                llvm::Type* paramType = visit(param->dataType()).as<llvm::Type*>();
                paramTypes.push_back(paramType);
                symbolTypes.push_back(currentSymbolType);
            }
        }

        // Build function itself
        llvm::Type* returnType = builder->getInt32Ty();
        llvm::FunctionType* fctType = llvm::FunctionType::get(returnType, paramTypes, false);
        llvm::Function* fct = llvm::Function::Create(fctType, llvm::Function::ExternalLinkage,
                                                     MAIN_FUNCTION_NAME, module.get());
        llvm::BasicBlock* bEntry = allocaInsertBlock = llvm::BasicBlock::Create(*context, "entry");
        allocaInsertInst = nullptr;
        fct->getBasicBlockList().push_back(bEntry);
        moveInsertPointToBlock(bEntry);

        // Store function params
        unsigned int declStmtCount = ctx->paramLstDef() ? ctx->paramLstDef()->declStmt().size() : 0;
        for (auto& param : fct->args()) {
            unsigned paramNo = param.getArgNo();
            std::string paramName = paramNames[paramNo];
            llvm::Type* paramType = fct->getFunctionType()->getParamType(paramNo);
            llvm::Value* memAddress = insertAlloca(paramType, paramName);
            SymbolTableEntry* paramSymbol = currentScope->lookup(paramName);
            assert(paramSymbol != nullptr);
            paramSymbol->updateAddress(memAddress);
            paramSymbol->updateLLVMType(paramType);
            builder->CreateStore(&param, memAddress);
        }

        // Declare result variable and set it to 0 for positive return code
        llvm::Value* memAddress = insertAlloca(returnType, RETURN_VARIABLE_NAME);
        SymbolTableEntry* returnSymbol = currentScope->lookup(RETURN_VARIABLE_NAME);
        assert(returnSymbol != nullptr);
        returnSymbol->updateAddress(memAddress);
        returnSymbol->updateLLVMType(returnType);
        builder->CreateStore(builder->getInt32(0), returnSymbol->getAddress());

        // Generate IR for function body
        visit(ctx->stmtLst());

        // Generate return statement for result variable
        if (!blockAlreadyTerminated) {
            llvm::Value* result = returnSymbol->getAddress();
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
        assert(currentScope != nullptr);
    }

    return nullptr;
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
        assert(currentScope != nullptr);
    }

    // Change scope
    FunctionSignature signature = currentScope->popSignature();
    currentScope = currentScope->getChild(signature.toString());

    // Get return type
    currentVarName = RETURN_VARIABLE_NAME;
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
        assert(thisEntry != nullptr);
        llvm::Type* paramType = thisEntry->getLLVMType()->getPointerTo();
        paramTypes.push_back(paramType);
        symbolTypes.push_back(thisEntry->getType().toPointer());
    }
    // Parameters
    if (ctx->paramLstDef()) {
        for (auto& param : ctx->paramLstDef()->declStmt()) {
            currentVarName = param->IDENTIFIER()->toString();
            paramNames.push_back(currentVarName);
            llvm::Type* paramType = visit(param->dataType()).as<llvm::Type*>();
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
    llvm::BasicBlock* bEntry = allocaInsertBlock = llvm::BasicBlock::Create(*context, "entry");
    allocaInsertInst = nullptr;
    fct->getBasicBlockList().push_back(bEntry);
    moveInsertPointToBlock(bEntry);

    // Store function params
    unsigned int declStmtCount = ctx->paramLstDef() ? ctx->paramLstDef()->declStmt().size() : 0;
    for (auto& param : fct->args()) {
        unsigned paramNo = param.getArgNo();
        std::string paramName = paramNames[paramNo];
        llvm::Type* paramType = fct->getFunctionType()->getParamType(paramNo);
        llvm::Value* memAddress = insertAlloca(paramType, paramName);
        SymbolTableEntry* paramEntry = currentScope->lookup(paramName);
        assert(paramEntry != nullptr);
        paramEntry->updateAddress(memAddress);
        paramEntry->updateLLVMType(paramType);
        builder->CreateStore(&param, memAddress);
    }

    // Declare result variable
    llvm::Value* returnMemAddress = insertAlloca(returnType, RETURN_VARIABLE_NAME);
    SymbolTableEntry* returnSymbol = currentScope->lookup(RETURN_VARIABLE_NAME);
    assert(returnSymbol != nullptr);
    returnSymbol->updateAddress(returnMemAddress);
    returnSymbol->updateLLVMType(returnType);

    // Generate IR for function body
    visit(ctx->stmtLst());

    // Generate return statement for result variable
    if (!blockAlreadyTerminated) {
        llvm::Value* result = returnSymbol->getAddress();
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
    assert(currentScope != nullptr);

    // Insert function declaration into symbol table
    currentScope->insertFunctionDeclaration(signature.toString(), symbolTypes);

    // Restore old scope
    currentScope = oldScope;

    return nullptr;
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
        assert(currentScope != nullptr);
    }

    // Change scope
    FunctionSignature signature = currentScope->popSignature();
    currentScope = currentScope->getChild(signature.toString());
    assert(currentScope != nullptr);

    // Create parameter list
    std::vector<std::string> paramNames;
    std::vector<llvm::Type*> paramTypes;
    std::vector<SymbolType> symbolTypes;
    // This variable (struct ptr of the parent struct)
    if (isMethod) {
        paramNames.push_back(THIS_VARIABLE_NAME);
        SymbolTableEntry* thisEntry = currentScope->getParent()->lookup(ctx->IDENTIFIER()[0]->toString());
        assert(thisEntry != nullptr);
        llvm::Type* paramType = thisEntry->getLLVMType()->getPointerTo();
        paramTypes.push_back(paramType);
        symbolTypes.push_back(thisEntry->getType().toPointer());
    }
    // Parameters
    if (ctx->paramLstDef()) {
        for (auto& param : ctx->paramLstDef()->declStmt()) {
            currentVarName = param->IDENTIFIER()->toString();
            paramNames.push_back(currentVarName);
            llvm::Type* paramType = visit(param->dataType()).as<llvm::Type*>();
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
    llvm::BasicBlock* bEntry = allocaInsertBlock = llvm::BasicBlock::Create(*context, "entry");
    allocaInsertInst = nullptr;
    proc->getBasicBlockList().push_back(bEntry);
    moveInsertPointToBlock(bEntry);

    // Store procedure params
    for (auto& param : proc->args()) {
        unsigned paramNo = param.getArgNo();
        std::string paramName = paramNames[paramNo];
        llvm::Type* paramType = proc->getFunctionType()->getParamType(paramNo);
        llvm::Value* memAddress = insertAlloca(paramType, paramName);
        SymbolTableEntry* paramSymbol = currentScope->lookup(paramName);
        assert(paramSymbol != nullptr);
        paramSymbol->updateAddress(memAddress);
        paramSymbol->updateLLVMType(paramType);
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
    assert(currentScope != nullptr);

    // Insert function declaration into symbol table
    currentScope->insertProcedureDeclaration(signature.toString(), symbolTypes);

    // Restore old scope
    currentScope = oldScope;

    return nullptr;
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
        assert(functionTable != nullptr);
        SymbolTableEntry* returnEntry = functionTable->lookup(RETURN_VARIABLE_NAME);
        assert(returnEntry != nullptr);
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
    if (ctx->DLL()) module->getFunction(functionName)->setDLLStorageClass(llvm::GlobalValue::DLLImportStorageClass);

    if (ctx->dataType()) { // Function
        currentScope->insertFunctionDeclaration(signature.toString(), symbolTypes);
    } else { // Procedure
        currentScope->insertProcedureDeclaration(signature.toString(), symbolTypes);
    }

    return nullptr;
}

antlrcpp::Any GeneratorVisitor::visitStructDef(SpiceParser::StructDefContext* ctx) {
    std::string structName = ctx->IDENTIFIER()->toString();

    // Collect member types
    std::vector<llvm::Type*> memberTypes;
    for (auto& field : ctx->field())
        memberTypes.push_back(visit(field->dataType()).as<llvm::Type*>());

    // Create global struct
    llvm::StructType* structType = llvm::StructType::create(*context, memberTypes, structName);
    SymbolTableEntry* structSymbol = currentScope->lookup(structName);
    assert(structSymbol != nullptr);
    structSymbol->updateLLVMType(structType);

    return nullptr;
}

antlrcpp::Any GeneratorVisitor::visitGlobalVarDef(SpiceParser::GlobalVarDefContext* ctx) {
    std::string varName = ctx->IDENTIFIER()->toString();

    // Get symbol table entry and the symbol specifiers
    SymbolTableEntry* symbolTableEntry = currentScope->lookup(varName);
    assert(symbolTableEntry != nullptr);
    SymbolSpecifiers specifiers = symbolTableEntry->getSpecifiers();

    // Create correctly signed LLVM type from the data type
    currentVarSigned = specifiers.isSigned();
    llvm::Type* varType = visit(ctx->dataType()).as<llvm::Type*>();
    symbolTableEntry->updateLLVMType(varType);

    // Create global variable
    llvm::Value* memAddress = module->getOrInsertGlobal(varName, varType);
    symbolTableEntry->updateAddress(memAddress);
    // Set some attributes to it
    llvm::GlobalVariable* global = module->getNamedGlobal(varName);
    global->setConstant(specifiers.isConst());
    global->setDSOLocal(true);

    if (ctx->value()) { // Variable is initialized here
        visit(ctx->value());
        constNegate = ctx->MINUS();
        global->setInitializer(currentConstValue);
        constNegate = false;
    }

    return nullptr;
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
    assert(currentScope != nullptr);
    currentScope->setContinueBlock(bCond);
    currentScope->setBreakBlock(bEnd);

    // Execute pre-loop stmts
    visit(ctx->declStmt());
    // Jump into condition block
    createBr(bCond);

    // Fill condition block
    parentFct->getBasicBlockList().push_back(bCond);
    moveInsertPointToBlock(bCond);
    llvm::Value* condValuePtr = visit(ctx->assignExpr()[0]).as<llvm::Value*>();
    llvm::Value* condValue = builder->CreateLoad(condValuePtr->getType()->getPointerElementType(), condValuePtr);
    // Jump to loop body or to loop end
    createCondBr(condValue, bLoop, bEnd);

    // Fill loop block
    parentFct->getBasicBlockList().push_back(bLoop);
    moveInsertPointToBlock(bLoop);
    // Generate IR for nested statements
    visit(ctx->stmtLst());
    // Run post-loop actions
    visit(ctx->assignExpr()[1]);
    // Jump into condition block
    createBr(bCond);

    // Fill loop end block
    parentFct->getBasicBlockList().push_back(bEnd);
    moveInsertPointToBlock(bEnd);

    // Change scope back
    currentScope = currentScope->getParent();
    assert(currentScope != nullptr);

    // Return true as result for the loop
    return (llvm::Value*) builder->getTrue();
}

antlrcpp::Any GeneratorVisitor::visitForeachLoop(SpiceParser::ForeachLoopContext* ctx) {
    auto head = ctx->foreachHead();

    // Create blocks
    llvm::BasicBlock* bLoop = llvm::BasicBlock::Create(*context, "foreach.loop");
    llvm::BasicBlock* bInc = llvm::BasicBlock::Create(*context, "foreach.inc");
    llvm::BasicBlock* bEnd = llvm::BasicBlock::Create(*context, "foreach.end");

    // Change scope
    std::string scopeId = ScopeIdUtil::getScopeId(ctx);
    currentScope = currentScope->getChild(scopeId);
    assert(currentScope != nullptr);
    currentScope->setContinueBlock(bInc);
    currentScope->setBreakBlock(bEnd);

    // Initialize loop variables
    llvm::Value* indexVariablePtr;
    if (head->declStmt().size() >= 2) {
        std::string indexVariableName = visit(ctx->foreachHead()->declStmt().front()).as<std::string>();
        SymbolTableEntry* indexVariableEntry = currentScope->lookup(indexVariableName);
        assert(indexVariableEntry != nullptr);
        indexVariablePtr = indexVariableEntry->getAddress();

        // Initialize variable with 0
        if (!head->declStmt().front()->assignExpr())
            builder->CreateStore(builder->getInt32(0), indexVariablePtr);
    } else {
        std::string indexVariableName = FOREACH_DEFAULT_IDX_VARIABLE_NAME;
        // Create local variable for
        llvm::Type* indexVariableType = llvm::Type::getInt32Ty(*context);
        indexVariablePtr = insertAlloca(indexVariableType, indexVariableName);
        SymbolTableEntry* indexVariableEntry = currentScope->lookup(indexVariableName);
        assert(indexVariableEntry != nullptr);
        indexVariableEntry->updateAddress(indexVariablePtr);
        indexVariableEntry->updateLLVMType(indexVariableType);
        indexVariableEntry->setUsed();
        // Initialize variable with 0
        builder->CreateStore(builder->getInt32(0), indexVariablePtr);
    }
    std::string itemVariableName = visit(ctx->foreachHead()->declStmt().back()).as<std::string>();
    SymbolTableEntry* itemVariableEntry = currentScope->lookup(itemVariableName);
    assert(itemVariableEntry != nullptr);
    llvm::Value* itemVariablePtr = itemVariableEntry->getAddress();

    // Do loop variable initialization
    llvm::Value* arrayValuePtr = visit(ctx->foreachHead()->assignExpr()).as<llvm::Value*>();
    llvm::Value* arrayValue = builder->CreateLoad(arrayValuePtr->getType()->getPointerElementType(), arrayValuePtr);
    llvm::Value* maxIndex = builder->getInt32(arrayValue->getType()->getArrayNumElements() -1);
    // Load the first item into item variable
    llvm::Value* indexValue = builder->CreateLoad(indexVariablePtr->getType()->getPointerElementType(), indexVariablePtr);
    llvm::Value* gepInst = builder->CreateInBoundsGEP(arrayValuePtr->getType()->getPointerElementType(),
                                                      arrayValuePtr, { builder->getInt32(0), indexValue });
    llvm::Value* newItemValue = builder->CreateLoad(gepInst->getType()->getPointerElementType(), gepInst);
    builder->CreateStore(newItemValue, itemVariablePtr);
    createBr(bLoop);

    // Get parent function
    llvm::Function* parentFct = builder->GetInsertBlock()->getParent();

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
    assert(currentScope != nullptr);

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
    assert(currentScope != nullptr);
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
    assert(currentScope != nullptr);

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
    assert(currentScope != nullptr);

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
    assert(currentScope != nullptr);

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
        assert(currentScope != nullptr);

        // Generate IR for nested statements
        visit(ctx->stmtLst());

        // Change scope back
        currentScope = currentScope->getParent();
        assert(currentScope != nullptr);
    }
    return nullptr;
}

antlrcpp::Any GeneratorVisitor::visitDeclStmt(SpiceParser::DeclStmtContext* ctx) {
    currentVarName = lhsVarName = ctx->IDENTIFIER()->toString();
    llvm::Type* varType = lhsType = visit(ctx->dataType()).as<llvm::Type*>();

    // Get variable entry
    SymbolTableEntry* entry = currentScope->lookup(currentVarName);
    assert(entry != nullptr);
    assert(!currentVarName.empty()); // Empty var names cause problems
    llvm::Value* memAddress = insertAlloca(varType, currentVarName);
    if (ctx->assignExpr()) {
        // Visit right side
        llvm::Value* rhsPtr = visit(ctx->assignExpr()).as<llvm::Value*>();
        llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
        builder->CreateStore(rhs, memAddress);
    }
    entry->updateAddress(memAddress);
    entry->updateLLVMType(varType);

    lhsType = nullptr; // Reset nullptr

    // Return the variable name
    return lhsVarName;
}

antlrcpp::Any GeneratorVisitor::visitImportStmt(SpiceParser::ImportStmtContext* ctx) {
    return nullptr; // Noop
}

antlrcpp::Any GeneratorVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext* ctx) {
    if (blockAlreadyTerminated) return nullptr;

    SymbolTableEntry* returnVarEntry = currentScope->lookup(RETURN_VARIABLE_NAME);

    // Check if a value is attached to the return statement
    llvm::Value* returnValuePtr = nullptr;
    if (ctx->assignExpr()) {
        assert(returnVarEntry != nullptr);
        // Set the expected type of the value
        lhsType = getTypeForSymbolType(returnVarEntry->getType());
        // Visit return value
        returnValuePtr = visit(ctx->assignExpr()).as<llvm::Value*>();
    } else if (returnVarEntry != nullptr) { // Function. Procedures do not have a return variable
        returnValuePtr = returnVarEntry->getAddress();
    }

    // No return value
    if (returnValuePtr == nullptr) {
        builder->CreateRetVoid();
        return nullptr;
    }

    // Return value
    llvm::Value* returnValue = builder->CreateLoad(returnValuePtr->getType()->getPointerElementType(), returnValuePtr);
    builder->CreateRet(returnValue);
    blockAlreadyTerminated = true;
    return returnValuePtr;
}

antlrcpp::Any GeneratorVisitor::visitBreakStmt(SpiceParser::BreakStmtContext* ctx) {
    // Get number, how many loops we want to break
    int breakCount = 1;
    if (ctx->INTEGER()) breakCount = std::stoi(ctx->INTEGER()->toString());

    // Get destination block
    SymbolTable* scope = currentScope;
    assert(scope != nullptr);
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
    assert(scope != nullptr);
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
    std::string stringTemplate = ctx->STRING_LITERAL()->toString();
    stringTemplate = std::regex_replace(stringTemplate, std::regex("\\\\n"), "\n");
    stringTemplate = stringTemplate.substr(1, stringTemplate.size() - 2);
    printfArgs.push_back(builder->CreateGlobalStringPtr(stringTemplate));
    for (auto& arg : ctx->assignExpr()) {
        llvm::Value* argValPtr = visit(arg).as<llvm::Value*>();

        llvm::Value* argVal;
        if (argValPtr->getType()->getPointerElementType()->isArrayTy()) { // Convert array type to pointer type
            std::vector<llvm::Value*> indices = { builder->getInt32(0), builder->getInt32(0) };
            llvm::Type* targetType = argValPtr->getType()->getPointerElementType();
            argVal = builder->CreateInBoundsGEP(targetType, argValPtr, indices);
        } else {
            argVal = builder->CreateLoad(argValPtr->getType()->getPointerElementType(), argValPtr);
        }

        if (argVal == nullptr) throw IRError(*arg->start, PRINTF_NULL_TYPE, "'" + arg->getText() + "' is null");

        // Cast all integer types to 32 bit
        if (argVal->getType()->isIntegerTy(1) || argVal->getType()->isIntegerTy(8) ||
            argVal->getType()->isIntegerTy(16) || argVal->getType()->isIntegerTy(64))
            argVal = builder->CreateZExtOrTrunc(argVal, llvm::Type::getInt32Ty(*context));

        printfArgs.push_back(argVal);
    }
    return (llvm::Value*) builder->CreateCall(printf, printfArgs);
}

antlrcpp::Any GeneratorVisitor::visitSizeOfCall(SpiceParser::SizeOfCallContext* ctx) {
    // Visit the parameter
    llvm::Value* valuePtr = visit(ctx->assignExpr()).as<llvm::Value*>();
    llvm::Value* value = builder->CreateLoad(valuePtr->getType()->getPointerElementType(), valuePtr);
    llvm::Type* valueTy = value->getType();

    // Calculate size at compile-time
    unsigned int size = valueTy->isArrayTy() ? valueTy->getArrayNumElements() : valueTy->getScalarSizeInBits();
    llvm::Value* resultPtr = insertAlloca(builder->getInt32Ty());
    builder->CreateStore(builder->getInt32(size), resultPtr);

    // Return address to where the size is saved
    return resultPtr;
}

antlrcpp::Any GeneratorVisitor::visitAssignExpr(SpiceParser::AssignExprContext* ctx) {
    // Visit the right side
    currentVarName = ""; // Reset the current variable name
    scopePrefix = ""; // Reset the scope prefix
    scopePath.clear(); // Clear the scope path
    structAccessIndices.clear(); // Clear struct access indices
    currentThisValue = nullptr; // Reset this value

    // Check if there is an assign operator applied
    if (ctx->assignOp()) { // This is an assignment or compound assignment
        lhsType = nullptr; // Reset lhs type

        // Get value of right side
        llvm::Value* rhsPtr = visit(ctx->assignExpr()).as<llvm::Value*>();
        llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);

        // Visit the left side
        currentVarName = ""; // Reset the current variable name
        scopePrefix = ""; // Reset the scope prefix
        scopePath.clear(); // Clear the scope path
        structAccessIndices.clear(); // Clear struct access indices
        currentThisValue = nullptr; // Reset this value
        llvm::Value* lhsPtr = visit(ctx->prefixUnaryExpr()).as<llvm::Value*>();
        lhsVarName = currentVarName;

        // Take a look at the operator
        if (ctx->assignOp()->ASSIGN()) { // Simple assign
            builder->CreateStore(rhs, lhsPtr);
        } else { // Compound assign
            // Get symbol table entry
            SymbolTableEntry* variableEntry = currentScope->lookup(lhsVarName);
            assert(variableEntry != nullptr);

            // Get value of left side
            llvm::Value* lhs;
            if (variableEntry->isLocal()) {
                lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
            } else {
                lhs = module->getNamedGlobal(lhsVarName);
            }
            // Decide what to do, based on the operator
            if (ctx->assignOp()->PLUS_EQUAL()) {
                rhs = conversionsManager->getPlusEqualInst(lhs, rhs);
            } else if (ctx->assignOp()->MINUS_EQUAL()) {
                rhs = conversionsManager->getMinusEqualInst(lhs, rhs);
            } else if (ctx->assignOp()->MUL_EQUAL()) {
                rhs = conversionsManager->getMulEqualInst(lhs, rhs);
            } else if (ctx->assignOp()->DIV_EQUAL()) {
                rhs = conversionsManager->getDivEqualInst(lhs, rhs);
            } else if (ctx->assignOp()->REM_EQUAL()) {
                rhs = conversionsManager->getRemEqualInst(lhs, rhs);
            } else if (ctx->assignOp()->SHL_EQUAL()) {
                rhs = conversionsManager->getSHLEqualInst(lhs, rhs);
            } else if (ctx->assignOp()->SHR_EQUAL()) {
                rhs = conversionsManager->getSHREqualInst(lhs, rhs);
            } else if (ctx->assignOp()->AND_EQUAL()) {
                rhs = conversionsManager->getAndEqualInst(lhs, rhs);
            } else if (ctx->assignOp()->OR_EQUAL()) {
                rhs = conversionsManager->getOrEqualInst(lhs, rhs);
            } else if (ctx->assignOp()->XOR_EQUAL()) {
                rhs = conversionsManager->getXorEqualInst(lhs, rhs);
            }
            builder->CreateStore(rhs, lhsPtr);
        }
        return lhsPtr;
    }

    antlrcpp::Any rhs = visit(ctx->ternaryExpr());
    lhsType = nullptr; // Reset lhs type
    return rhs;
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

        llvm::Value* resultPtr = insertAlloca(phi->getType());
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
        llvm::Value* resultPtr = insertAlloca(phi->getType());
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
        llvm::Value* resultPtr = insertAlloca(phi->getType());
        builder->CreateStore(phi, resultPtr);
        return resultPtr;
    }
    return visit(ctx->bitwiseOrExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext* ctx) {
    if (ctx->bitwiseXorExpr().size() > 1) {
        llvm::Value* lhsPtr = visit(ctx->bitwiseXorExpr()[0]).as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        for (int i = 1; i < ctx->bitwiseXorExpr().size(); i++) {
            llvm::Value* rhsPtr = visit(ctx->bitwiseXorExpr()[i]).as<llvm::Value*>();
            llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
            lhs = conversionsManager->getBitwiseOrInst(lhs, rhs);
        }
        llvm::Value* resultPtr = insertAlloca(lhs->getType());
        builder->CreateStore(lhs, resultPtr);
        return resultPtr;
    }
    return visit(ctx->bitwiseXorExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext* ctx) {
    if (ctx->bitwiseAndExpr().size() > 1) {
        llvm::Value* lhsPtr = visit(ctx->bitwiseAndExpr()[0]).as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        for (int i = 1; i < ctx->bitwiseAndExpr().size(); i++) {
            llvm::Value* rhsPtr = visit(ctx->bitwiseAndExpr()[i]).as<llvm::Value*>();
            llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
            lhs = conversionsManager->getBitwiseXorInst(lhs, rhs);
        }
        llvm::Value* resultPtr = insertAlloca(lhs->getType());
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
        llvm::Value* resultPtr = insertAlloca(lhs->getType());
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
            llvm::Value* resultPtr = insertAlloca(result->getType());
            builder->CreateStore(result, resultPtr);
            return resultPtr;
        } else if (ctx->NOT_EQUAL()) { // Equality expr is: relationalExpr NOT_EQUAL relationalExpr
            llvm::Value* result = conversionsManager->getNotEqualInst(lhs, rhs);
            llvm::Value* resultPtr = insertAlloca(result->getType());
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
            llvm::Value* resultPtr = insertAlloca(result->getType());
            builder->CreateStore(result, resultPtr);
            return resultPtr;
        } else if (ctx->GREATER()) { // Relational expr is: shiftExpr GREATER shiftExpr
            llvm::Value* result = conversionsManager->getGreaterInst(lhs, rhs);
            llvm::Value* resultPtr = insertAlloca(result->getType());
            builder->CreateStore(result, resultPtr);
            return resultPtr;
        } else if (ctx->LESS_EQUAL()) { // Relational expr is: shiftExpr LESS_EQUAL shiftExpr
            llvm::Value* result = conversionsManager->getLessEqualInst(lhs, rhs);
            llvm::Value* resultPtr = insertAlloca(result->getType());
            builder->CreateStore(result, resultPtr);
            return resultPtr;
        } else if (ctx->GREATER_EQUAL()) { // Relational expr is: shiftExpr GREATER_EQUAL shiftExpr
            llvm::Value* result = conversionsManager->getGreaterEqualInst(lhs, rhs);
            llvm::Value* resultPtr = insertAlloca(result->getType());
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
            llvm::Value* resultPtr = insertAlloca(result->getType());
            builder->CreateStore(result, resultPtr);
            return resultPtr;
        } else if (ctx->SHR()) { // Shift expr is: additiveExpr SHR additiveExpr
            llvm::Value* result = conversionsManager->getShiftRightInst(lhs, rhs);
            llvm::Value* resultPtr = insertAlloca(result->getType());
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
            else if (op->getSymbol()->getType() == SpiceParser::MINUS)
                lhs = conversionsManager->getMinusInst(lhs, rhs);

            operatorIndex += 2;
        }

        llvm::Value* resultPtr = insertAlloca(lhs->getType());
        builder->CreateStore(lhs, resultPtr);
        return resultPtr;
    }
    return visit(ctx->multiplicativeExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext* ctx) {
    // Check if at least one multiplicative operator is applied
    if (ctx->castExpr().size() > 1) {
        llvm::Value* lhsPtr = visit(ctx->castExpr()[0]).as<llvm::Value*>();
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);
        unsigned int operatorIndex = 1;
        for (int i = 1; i < ctx->castExpr().size(); i++) {
            auto* op = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[operatorIndex]);
            llvm::Value* rhsPtr = visit(ctx->castExpr()[i]).as<llvm::Value*>();
            llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);

            if (op->getSymbol()->getType() == SpiceParser::MUL)
                lhs = conversionsManager->getMulInst(lhs, rhs);
            else if (op->getSymbol()->getType() == SpiceParser::DIV)
                lhs = conversionsManager->getDivInst(lhs, rhs);
            else if (op->getSymbol()->getType() == SpiceParser::REM)
                lhs = conversionsManager->getRemInst(lhs, rhs);

            operatorIndex += 2;
        }

        llvm::Value* resultPtr = insertAlloca(lhs->getType());
        builder->CreateStore(lhs, resultPtr);
        return resultPtr;
    }
    return visit(ctx->castExpr()[0]);
}

antlrcpp::Any GeneratorVisitor::visitCastExpr(SpiceParser::CastExprContext* ctx) {
    auto value = visit(ctx->prefixUnaryExpr());

    if (ctx->LPAREN()) { // Cast operator is applied
        llvm::Type* dstTy = visit(ctx->dataType()).as<llvm::Type*>();
        llvm::Value* rhsPtr = value.as<llvm::Value*>();
        llvm::Value* rhs = builder->CreateLoad(rhsPtr->getType()->getPointerElementType(), rhsPtr);
        llvm::Value* result = conversionsManager->getCastInst(dstTy, rhs);
        llvm::Value* resultPtr = insertAlloca(result->getType());
        builder->CreateStore(result, resultPtr);
        return resultPtr;
    }

    return value;
}

antlrcpp::Any GeneratorVisitor::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext* ctx) {
    llvm::Value* lhsPtr = visit(ctx->postfixUnaryExpr()).as<llvm::Value*>();

    if (!ctx->prefixUnaryOp().empty()) {
        // Load the value
        llvm::Value* lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);

        unsigned int tokenCounter = 0;
        while (tokenCounter < ctx->children.size() -1) {
            auto* token = dynamic_cast<SpiceParser::PrefixUnaryOpContext*>(ctx->children[tokenCounter]);

            // Insert conversion instructions depending on the used operator
            if (token->MINUS()) { // Consider - operator
                lhs = conversionsManager->getPrefixMinusInst(lhs);
            } else if (token->PLUS_PLUS()) { // Consider ++ operator
                lhs = conversionsManager->getPrefixPlusPlusInst(lhs);
            } else if (token->MINUS_MINUS()) { // Consider -- operator
                lhs = conversionsManager->getPrefixMinusMinusInst(lhs);
            } else if (token->NOT()) { // Consider ! operator
                lhs = conversionsManager->getPrefixNotInst(lhs);
            } else if (token->BITWISE_NOT()) { // Consider ~ operator
                lhs = conversionsManager->getPrefixBitwiseNotInst(lhs);
            } else if (token->MUL()) { // Consider * operator
                // De-reference
                lhsPtr = lhs;
                lhs = builder->CreateLoad(lhs->getType()->getPointerElementType(), lhs);
            } else if (token->BITWISE_AND()) { // Consider & operator
                // Create a reference
                lhs = lhsPtr;
                lhsPtr = insertAlloca(lhs->getType());
                builder->CreateStore(lhs, lhsPtr);
            } else if (token->LOGICAL_AND()) { // Consider doubled & operator
                // First reference
                lhs = lhsPtr;
                lhsPtr = insertAlloca(lhs->getType());
                builder->CreateStore(lhs, lhsPtr);
                // Second reference
                lhs = lhsPtr;
                lhsPtr = insertAlloca(lhs->getType());
                builder->CreateStore(lhs, lhsPtr);
            }
            tokenCounter++;
        }

        // Store the value back again
        builder->CreateStore(lhs, lhsPtr);
    }

    return lhsPtr;
}

antlrcpp::Any GeneratorVisitor::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext* ctx) {
    llvm::Value* lhsPtr = visit(ctx->atomicExpr()).as<llvm::Value*>();

    if (ctx->children.size() > 1) {
        // Load the value
        llvm::Value* lhs = nullptr;
        if (lhsPtr) lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);

        unsigned int tokenCounter = 1;
        while (tokenCounter < ctx->children.size()) {
            auto* token = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[tokenCounter]);

            // Insert conversion instructions depending on the used operator
            unsigned long long symbolType = token->getSymbol()->getType();
            if (symbolType == SpiceParser::LBRACKET) { // Consider subscript operator
                tokenCounter++; // Consume LBRACKET

                std::string arrayName = currentVarName; // Save array name
                ScopePath scopePathBackup = scopePath; // Save scope path
                scopePrefix += "[idx]";

                // Get the index value
                auto* assignExpr = dynamic_cast<SpiceParser::AssignExprContext*>(ctx->children[tokenCounter]);
                llvm::Value* indexValue = visit(assignExpr).as<llvm::Value*>();
                indexValue = builder->CreateLoad(indexValue->getType()->getPointerElementType(), indexValue);
                tokenCounter++; // Consume assignExpr

                // Get array item
                if (lhsPtr->getType()->getPointerElementType()->isArrayTy()) {
                    std::vector<llvm::Value*> indices = { builder->getInt32(0), indexValue };
                    lhsPtr = builder->CreateInBoundsGEP(lhsPtr->getType()->getPointerElementType(), lhsPtr, indices);
                } else {
                    if (lhs == nullptr) lhs = builder->CreateLoad(lhsPtr->getType()->getPointerElementType(), lhsPtr);

                    std::vector<llvm::Value*> indices = { indexValue };
                    lhsPtr = builder->CreateInBoundsGEP(lhs->getType()->getPointerElementType(), lhs, indices);
                }

                currentVarName = arrayName; // Restore current var name
                scopePath = scopePathBackup; // Restore scope path

                lhs = nullptr; // Set lhs to nullptr to prevent a store
            } else if (symbolType == SpiceParser::LPAREN) { // Consider function call
                tokenCounter++; // Consume LPAREN

                // Check if the function is a method and retrieve the function name based on that
                bool isMethod = currentThisValue != nullptr;
                std::string functionName = isMethod ? scopePrefix + "." + currentVarName : currentVarName;

                // Get function by signature
                FunctionSignature signature = currentScope->popSignature();
                // Check if function exists in the current module
                bool functionFound = false;
                std::string fctIdentifier = signature.toString();
                for (auto& function : module->getFunctionList()) { // Search for function definition
                    if (function.getName() == signature.toString()) {
                        functionFound = true;
                        break;
                    } else if (function.getName() == signature.getFunctionName()) {
                        functionFound = true;
                        fctIdentifier = signature.getFunctionName();
                        break;
                    }
                }

                // Retrieve the access scope
                SymbolTable* accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;

                if (!functionFound) { // Not found => Declare function, which will be linked in
                    // Get the symbol table of the module where the function is defined
                    SymbolTable* table = accessScope->lookupTableWithSignature(signature.toString());
                    assert(table != nullptr);
                    // Check if it is a function or a procedure
                    if (!table->getFunctionDeclaration(signature.toString()).empty()) { // Function
                        std::vector<SymbolType> symbolTypes = table->getFunctionDeclaration(signature.toString());

                        // Get return type
                        SymbolType returnSymbolType = symbolTypes[0];
                        // Check if the return type is an imported struct. Due to not supporting pointers as return values, we do
                        // not have to check for nested structs in pointers here.
                        if (accessScope->isImported() && returnSymbolType.is(TY_STRUCT)) {
                            // If the return type is an imported struct, initialize it first
                            initExtStruct(returnSymbolType.getSubType(), scopePrefix + "." + returnSymbolType.getSubType());
                            returnSymbolType = SymbolType(TY_STRUCT, scopePrefix + "." + returnSymbolType.getSubType());
                        }
                        llvm::Type* returnType = getTypeForSymbolType(returnSymbolType);
                        if (!returnType) throw std::runtime_error("Internal compiler error: Could not find return type of function call");

                        // Get parameter types
                        std::vector<llvm::Type*> paramTypes;
                        for (int i = 1; i < symbolTypes.size(); i++) {
                            SymbolType paramSymbolType = symbolTypes[i];
                            // Check if it is an imported struct. If so, replace the subtype with the scope prefix
                            if (accessScope->isImported() && paramSymbolType.isBaseType(TY_STRUCT))
                                paramSymbolType = paramSymbolType.replaceSubType(scopePrefix);
                            // Retrieve the LLVM type of the symbol type
                            llvm::Type* paramType = getTypeForSymbolType(paramSymbolType);
                            if (!paramType) throw std::runtime_error("Internal compiler error: Could not get parameter type of function call");
                            paramTypes.push_back(paramType);
                        }

                        // Declare the function
                        llvm::FunctionType* fctType = llvm::FunctionType::get(returnType, paramTypes, false);
                        module->getOrInsertFunction(fctIdentifier, fctType);
                    } else if (!table->getProcedureDeclaration(signature.toString()).empty()) { // Procedure
                        std::vector<SymbolType> symbolTypes = table->getProcedureDeclaration(signature.toString());

                        // Get parameter types
                        std::vector<llvm::Type*> paramTypes;
                        for (auto& paramSymbolType : symbolTypes) {
                            // Check if it is an imported struct. If so, replace the subtype with the scope prefix
                            if (accessScope->isImported() && paramSymbolType.isBaseType(TY_STRUCT))
                                paramSymbolType = paramSymbolType.replaceSubType(scopePrefix);
                            // Retrieve the LLVM type of the symbol type
                            llvm::Type* paramType = getTypeForSymbolType(paramSymbolType);
                            if (!paramType) throw std::runtime_error("Internal compiler error");
                            paramTypes.push_back(paramType);
                        }

                        // Declare the procedure
                        llvm::Type* returnType = llvm::Type::getVoidTy(*context);
                        llvm::FunctionType* procType = llvm::FunctionType::get(returnType, paramTypes, false);
                        module->getOrInsertFunction(fctIdentifier, procType);
                    }
                }

                // Get the declared function and its type
                llvm::Function* fct = module->getFunction(fctIdentifier);
                llvm::FunctionType* fctType = fct->getFunctionType();

                // Fill parameter list
                int paramIndex = 0;
                std::vector<llvm::Value*> argValues;
                if (isMethod) { // If it is a method, pass 'this' as implicit first argument
                    argValues.push_back(currentThisValue);
                    paramIndex++;
                }
                auto* paramLst = dynamic_cast<SpiceParser::ParamLstContext*>(ctx->children[tokenCounter]);
                if (paramLst != nullptr) {
                    for (auto& param : paramLst->assignExpr()) {
                        // Get expected arg type
                        llvm::Type* expectedArgType = fctType->getParamType(paramIndex);
                        // Get the actual arg value
                        llvm::Value* actualArgPtr = visit(param).as<llvm::Value*>();
                        if (!compareLLVMTypes(actualArgPtr->getType()->getPointerElementType(), expectedArgType)) {
                            argValues.push_back(doImplicitCast(actualArgPtr, expectedArgType));
                        } else {
                            argValues.push_back(builder->CreateLoad(actualArgPtr->getType()->getPointerElementType(), actualArgPtr));
                        }
                        paramIndex++;
                    }
                }
                tokenCounter++; // Consume paramLst

                // Create the function call
                lhs = builder->CreateCall(fct, argValues);
                if (!lhs->getType()->isSized()) lhs = builder->getTrue();

                // Insert alloca
                lhsPtr = insertAlloca(lhs->getType());
            } else if (symbolType == SpiceParser::DOT) { // Consider member access
                tokenCounter++; // Consume dot
                auto* postfixUnary = dynamic_cast<SpiceParser::PostfixUnaryExprContext*>(ctx->children[tokenCounter]);
                lhsPtr = visit(postfixUnary).as<llvm::Value*>();
                lhs = nullptr;
            } else if (symbolType == SpiceParser::PLUS_PLUS) { // Consider ++ operator
                assert(lhs != nullptr);
                lhs = conversionsManager->getPostfixPlusPlusInst(lhs);
            } else if (symbolType == SpiceParser::MINUS_MINUS) { // Consider -- operator
                assert(lhs != nullptr);
                lhs = conversionsManager->getPostfixMinusMinusInst(lhs);
            }
            tokenCounter++;
        }

        // Store the value back again
        if (lhs != nullptr) builder->CreateStore(lhs, lhsPtr);
    }

    return lhsPtr;
}

antlrcpp::Any GeneratorVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext* ctx) {
    if (ctx->value()) return visit(ctx->value());
    allParamsHardcoded = false; // To prevent arrays from being defined globally when depending on other values (vars, calls, etc.)
    if (ctx->IDENTIFIER()) {
        currentVarName = ctx->IDENTIFIER()->toString();
        scopePrefix += currentVarName;

        // Load symbol table entry
        SymbolTable* accessScope = scopePath.getCurrentScope() ? scopePath.getCurrentScope() : currentScope;
        SymbolTableEntry* entry = accessScope->lookup(currentVarName);

        if (!entry) return (llvm::Value*) nullptr;

        llvm::Value* memAddress = entry->getAddress();
        if (entry->getType().isBaseType(TY_STRUCT)) { // If base type is a struct
            if (structAccessIndices.empty()) { // No struct was seen before
                // Initialize GEP calculation
                structAccessIndices.push_back(builder->getInt32(0)); // To de-reference pointer input of GEP
                // Set the access type and address
                structAccessType = entry->getLLVMType();
                structAccessAddress = currentThisValue = entry->getAddress();
            } else { // This is a struct field in a struct
                // Just add the index to the index list
                unsigned int fieldIndex = entry->getOrderIndex();
                structAccessIndices.push_back(builder->getInt32(fieldIndex));
                SymbolType tmpType = entry->getType();
                while (tmpType.isPointer()) {
                    // Execute GEP with the collected indices to de-reference the pointer
                    structAccessAddress = builder->CreateInBoundsGEP(structAccessType, structAccessAddress,
                                                             structAccessIndices);
                    // Load the value and store it as new address
                    structAccessAddress = currentThisValue = builder->CreateLoad(
                            structAccessAddress->getType()->getPointerElementType(), structAccessAddress);
                    // Set new struct access type
                    SymbolTableEntry* nestedStructEntry = accessScope->lookup(entry->getType().getBaseType().getSubType());
                    assert(nestedStructEntry != nullptr);
                    structAccessType = nestedStructEntry->getLLVMType();
                    // Reset index list
                    structAccessIndices.clear();
                    structAccessIndices.push_back(builder->getInt32(0));
                    // Unpack symbol type
                    tmpType = tmpType.getContainedTy();
                }
            }
        } else if (!structAccessIndices.empty()) { // A struct was met already, so this is a struct field
            unsigned int fieldIndex = entry->getOrderIndex();
            // Push field index to index list
            structAccessIndices.push_back(builder->getInt32(fieldIndex));
            // Unpack address
            while (structAccessType->isPointerTy()) {
                structAccessAddress = builder->CreateLoad(structAccessType, structAccessAddress);
                structAccessType = structAccessType->getPointerElementType();
            }
            // Execute GEP calculation
            memAddress = builder->CreateInBoundsGEP(structAccessType, structAccessAddress, structAccessIndices);
            // Clear index list
            structAccessIndices.clear();
        } else {
            // For the case that in the current scope there is a variable with the same name, but it is initialized later, so the
            // symbol above in the hierarchy is meant to be used.
            while (entry && !entry->getAddress() && !entry->getType().is(TY_IMPORT))
                entry = accessScope->getParent()->lookup(currentVarName);

            if (!entry) return (llvm::Value*) nullptr;
        }

        // Retrieve scope for the new scope path fragment
        SymbolTable* newAccessScope = accessScope;
        if (entry->getType().is(TY_IMPORT)) { // Import
            newAccessScope = accessScope->lookupTable(entry->getName());
        } else if (entry->getType().isBaseType(TY_STRUCT)) { // Struct
            newAccessScope = accessScope->lookupTable("struct:" + entry->getType().getBaseType().getSubType());
        }
        assert(newAccessScope != nullptr);

        // Otherwise, push the current scope to the scope path
        scopePath.pushFragment(currentVarName, newAccessScope);

        return memAddress;
    }
    if (ctx->builtinCall()) return visit(ctx->builtinCall());
    return visit(ctx->assignExpr());
}

antlrcpp::Any GeneratorVisitor::visitValue(SpiceParser::ValueContext* ctx) {
    if (ctx->primitiveValue()) { // Primitive value
        // Visit the primitive value
        currentConstValue = visit(ctx->primitiveValue()).as<llvm::Constant*>();

        // If global variable value, return value immediately, because it is already a pointer
        if (!currentScope->getParent()) return currentConstValue;

        // Store the value to a tmp variable
        llvm::Value* llvmValuePtr = insertAlloca(currentConstValue->getType());
        builder->CreateStore(currentConstValue, llvmValuePtr);
        return llvmValuePtr;
    }

    if (ctx->NIL()) { // Value is nil
        llvm::Type* nilType = visit(ctx->dataType()).as<llvm::Type*>();
        currentConstValue = llvm::Constant::getNullValue(nilType);

        // If global variable value, return value immediately, because it is already a pointer
        if (!currentScope->getParent()) return currentConstValue;

        // Store the value to a tmp variable
        llvm::Value* llvmValuePtr = insertAlloca(currentConstValue->getType());
        builder->CreateStore(currentConstValue, llvmValuePtr);
        return llvmValuePtr;
    }

    if (!ctx->IDENTIFIER().empty()) { // Struct instantiation
        std::string variableName = currentVarName = ctx->IDENTIFIER()[0]->toString();

        // Get struct name in format a.b.c and struct scope
        std::string structName;
        SymbolTable* structScope = currentScope;
        for (unsigned int i = 0; i < ctx->IDENTIFIER().size(); i++) {
            std::string iden = ctx->IDENTIFIER()[i]->toString();
            structName += structName.empty() ? iden : "." + iden;
            if (i < ctx->IDENTIFIER().size() -1) {
                SymbolTableEntry* entry = structScope->lookup(iden);
                if (!entry)
                    throw SemanticError(*ctx->IDENTIFIER()[1]->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                                        "Struct '" + structName + "' was used before defined");
                if (entry->getType().is(TY_IMPORT)) {
                    structScope = structScope->lookupTable(iden);
                } else if (entry->getType().is(TY_STRUCT)) {
                    structScope = structScope->lookupTable("struct:" + iden);
                } else {
                    throw SemanticError(*ctx->IDENTIFIER()[1]->getSymbol(), REFERENCED_UNDEFINED_STRUCT,
                                        "The variable '" + iden + "' is of type " + entry->getType().getName(false) +
                                        ". Expected struct or import");
                }
            }
        }

        // Check if the struct is defined
        if (structScope->isImported()) initExtStruct(ctx->IDENTIFIER().back()->toString(), structName);
        SymbolTableEntry* structSymbol = currentScope->lookup(structName);
        assert(structSymbol != nullptr);
        llvm::Type* structType = structSymbol->getLLVMType();

        // Allocate space for the struct in memory
        assert(!currentVarName.empty()); // Empty var names cause problems
        llvm::Value* structAddress = insertAlloca(structType);
        SymbolTableEntry* variableSymbol = currentScope->lookup(variableName);
        variableSymbol->updateAddress(structAddress);
        variableSymbol->updateLLVMType(structType);

        // Get struct table
        SymbolTable* structTable = currentScope->lookupTable("struct:" + structName);
        assert(structTable != nullptr);

        // Fill the struct with the stated values
        if (ctx->paramLst()) {
            for (unsigned int i = 0; i < ctx->paramLst()->assignExpr().size(); i++) {
                // Set address to the struct instance field
                SymbolTableEntry* fieldEntry = structTable->lookupByIndexInCurrentScope(i);
                assert(fieldEntry != nullptr);
                // Visit assignment
                llvm::Value* assignmentPtr = visit(ctx->paramLst()->assignExpr()[i]).as<llvm::Value*>();
                llvm::Value* assignment = builder->CreateLoad(assignmentPtr->getType()->getPointerElementType(), assignmentPtr);
                // Get pointer to struct element
                llvm::Value* fieldAddress = builder->CreateStructGEP(structType, structAddress, i);
                fieldEntry->updateAddress(fieldAddress);
                // Store value to address
                builder->CreateStore(assignment, fieldAddress);
            }
        }

        return structAddress;
    }

    if (ctx->LBRACE()) { // Array initialization
        // Get data type
        size_t actualItemCount = ctx->paramLst() ? ctx->paramLst()->assignExpr().size() : 0;
        size_t arraySize = lhsType != nullptr && lhsType->isArrayTy() ? lhsType->getArrayNumElements() : actualItemCount;
        llvm::Type* arrayType = lhsType;

        // Fill items with the stated values
        if (ctx->paramLst()) {
            // Visit all params to check if they are hardcoded or not
            std::vector<llvm::Value*> itemValuePointers;
            std::vector<llvm::Constant*> itemConstants;
            allParamsHardcoded = true;
            for (size_t i = 0; i < std::min(ctx->paramLst()->assignExpr().size(), arraySize); i++) {
                llvm::Value* itemValuePtr = visit(ctx->paramLst()->assignExpr()[i]).as<llvm::Value*>();
                itemValuePointers.push_back(itemValuePtr);
                itemConstants.push_back(currentConstValue);
            }

            if (arrayType == nullptr) {
                llvm::Type* itemType = itemValuePointers[0]->getType()->getPointerElementType();
                arrayType = llvm::ArrayType::get(itemType, arraySize);
            }

            // Decide if the array can be defined globally
            llvm::Value* arrayAddress;
            if (allParamsHardcoded) { // All params hardcoded => array can be defined globally
                // Create hardcoded array
                llvm::Constant* constArray = llvm::ConstantArray::get((llvm::ArrayType*) arrayType, itemConstants);
                // Create global variable
                std::string globalVarName = lhsVarName;
                if (globalVarName.empty()) { // Get unused anonymous global var name
                    unsigned int suffixNumber = 0;
                    do {
                        globalVarName = "anonymous." + std::to_string(suffixNumber);
                        suffixNumber++;
                    } while (module->getNamedGlobal(globalVarName) != nullptr);
                }
                arrayAddress = module->getOrInsertGlobal(globalVarName, arrayType);
                // Set some attributes to it
                llvm::GlobalVariable* global = module->getNamedGlobal(globalVarName);
                global->setConstant(true);
                global->setDSOLocal(true);
                global->setInitializer(constArray);
            } else { // Some params are not hardcoded => fallback to individual indexing
                // Allocate array
                arrayAddress = insertAlloca(arrayType, currentVarName);

                // Insert all given values
                size_t valueIndex = 0;
                for (; valueIndex < std::min(ctx->paramLst()->assignExpr().size(), arraySize); valueIndex++) {
                    llvm::Value* itemValuePtr = itemValuePointers[valueIndex];
                    llvm::Value* itemValue = builder->CreateLoad(itemValuePtr->getType()->getPointerElementType(), itemValuePtr);
                    // Calculate item address
                    std::vector<llvm::Value*> indices = { builder->getInt32(0), builder->getInt32(valueIndex) };
                    llvm::Value* itemAddress = builder->CreateInBoundsGEP(arrayType, arrayAddress, indices);
                    // Store value to item address
                    builder->CreateStore(itemValue, itemAddress);
                }

                // Fill up the rest of the values with default values
                for (; valueIndex < arraySize; valueIndex++) {
                    // ToDo: Fill up with default values
                }
            }
            return arrayAddress;
        }

        // Empty array: '{}'
        assert(arrayType != nullptr);
        assert(!currentVarName.empty()); // Empty var names cause problems
        return insertAlloca(arrayType, currentVarName);
    }

    return nullptr;
}

antlrcpp::Any GeneratorVisitor::visitPrimitiveValue(SpiceParser::PrimitiveValueContext* ctx) {
    // Value is a double constant
    if (ctx->DOUBLE()) {
        currentSymbolType = SymbolType(TY_DOUBLE);
        double value = std::stod(ctx->DOUBLE()->toString());
        if (constNegate) value *= -1;
        return (llvm::Constant*) llvm::ConstantFP::get(*context, llvm::APFloat(value));
    }

    // Value is an integer constant
    if (ctx->INTEGER()) {
        currentSymbolType = SymbolType(TY_INT);
        int value = std::stoi(ctx->INTEGER()->toString());
        if (constNegate) value *= -1;
        if (currentVarSigned) {
            return (llvm::Constant*) llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), value);
        } else {
            return (llvm::Constant*) llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), value);
        }
    }

    // Value is a short constant
    if (ctx->SHORT()) {
        currentSymbolType = SymbolType(TY_SHORT);
        int value = std::stoi(ctx->SHORT()->toString());
        if (constNegate) value *= -1;
        if (currentVarSigned) {
            return (llvm::Constant*) llvm::ConstantInt::getSigned(llvm::Type::getInt16Ty(*context), value);
        } else {
            return (llvm::Constant*) llvm::ConstantInt::get(llvm::Type::getInt16Ty(*context), value);
        }
    }

    // Value is a long constant
    if (ctx->LONG()) {
        currentSymbolType = SymbolType(TY_LONG);
        long long value = std::stoll(ctx->LONG()->toString());
        if (constNegate) value = -value;
        if (currentVarSigned) {
            return (llvm::Constant*) llvm::ConstantInt::getSigned(llvm::Type::getInt64Ty(*context), value);
        } else {
            return (llvm::Constant*) llvm::ConstantInt::get(llvm::Type::getInt64Ty(*context), value);
        }
    }

    // Value is a char constant
    if (ctx->CHAR_LITERAL()) {
        currentSymbolType = SymbolType(TY_CHAR);
        char value = ctx->CHAR_LITERAL()->toString()[1];
        if (currentVarSigned) {
            return (llvm::Constant*) llvm::ConstantInt::getSigned(llvm::Type::getInt8Ty(*context), value);
        } else {
            return (llvm::Constant*) llvm::ConstantInt::get(llvm::Type::getInt8Ty(*context), value);
        }
    }

    // Value is a string constant
    if (ctx->STRING_LITERAL()) {
        currentSymbolType = SymbolType(TY_STRING);
        std::string value = ctx->STRING_LITERAL()->toString();
        value = std::regex_replace(value, std::regex("\\\\n"), "\n");
        value = value.substr(1, value.size() - 2);
        return (llvm::Constant*) builder->CreateGlobalStringPtr(value, "", 0, module.get());
    }

    // Value is a boolean constant with value false
    if (ctx->FALSE()) {
        currentSymbolType = SymbolType(TY_BOOL);
        return (llvm::Constant*) builder->getFalse();
    }

    // Value is a boolean constant with value true
    if (ctx->TRUE()) {
        currentSymbolType = SymbolType(TY_BOOL);
        return (llvm::Constant*) builder->getTrue();
    }

    throw std::runtime_error("Internal compiler error: Primitive data type generator fall-through");
}

antlrcpp::Any GeneratorVisitor::visitDataType(SpiceParser::DataTypeContext* ctx) {
    currentSymbolType = visit(ctx->baseDataType()).as<SymbolType>();

    size_t tokenCounter = 1;
    while (tokenCounter < ctx->children.size()) {
        auto* token = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[tokenCounter]);
        if (token->getSymbol()->getType() == SpiceParser::MUL) { // Consider de-referencing operators
            currentSymbolType = currentSymbolType.toPointer();
        } else if (token->getSymbol()->getType() == SpiceParser::LBRACKET) { // Consider array bracket pairs
            tokenCounter++; // Consume LBRACKET
            token = dynamic_cast<antlr4::tree::TerminalNode*>(ctx->children[tokenCounter]);
            unsigned int size = 0; // Default to 0 when no size is attached
            if (token->getSymbol()->getType() == SpiceParser::INTEGER) { // Size is attached
                size = std::stoi(token->toString());
                tokenCounter++; // Consume INTEGER
            }
            currentSymbolType = currentSymbolType.toArray(size);
        }
        tokenCounter++;
    }

    // Come up with the LLVM type
    llvm::Type* type = getTypeForSymbolType(currentSymbolType);
    // Throw an error if something went wrong.
    // This should technically never occur because of the semantic analysis
    if (!type) throw IRError(*ctx->baseDataType()->getStart(), UNEXPECTED_DYN_TYPE_IR, "Dyn was other");
    return type;
}

antlrcpp::Any GeneratorVisitor::visitBaseDataType(SpiceParser::BaseDataTypeContext* ctx) {
    if (ctx->TYPE_DOUBLE()) return SymbolType(TY_DOUBLE);
    if (ctx->TYPE_INT()) return SymbolType(TY_INT);
    if (ctx->TYPE_SHORT()) return SymbolType(TY_SHORT);
    if (ctx->TYPE_LONG()) return SymbolType(TY_LONG);
    if (ctx->TYPE_BYTE()) return SymbolType(TY_BYTE);
    if (ctx->TYPE_CHAR()) return SymbolType(TY_CHAR);
    if (ctx->TYPE_STRING()) return SymbolType(TY_STRING);
    if (ctx->TYPE_BOOL()) return SymbolType(TY_BOOL);
    if (ctx->TYPE_DYN()) { // Data type is type inferred
        SymbolTableEntry* symbolTableEntry = currentScope->lookup(currentVarName);
        assert(symbolTableEntry != nullptr);
        return symbolTableEntry->getType();
    }
    if (!ctx->IDENTIFIER().empty()) { // Custom data type
        // Get type name in format: a.b.c
        std::string typeName = ctx->IDENTIFIER()[0]->toString();
        for (size_t i = 1; i < ctx->IDENTIFIER().size(); i++) typeName += "." + ctx->IDENTIFIER()[i]->toString();

        return SymbolType(TY_STRUCT, typeName);
    }
    throw std::runtime_error("Internal compiler error: Base datatype generator fall-through");
}

void GeneratorVisitor::initializeExternalFunctions() {
    // printf function
    llvm::FunctionType* fctTy = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context),
                                                        llvm::Type::getInt8PtrTy(*context), true);
    module->getOrInsertFunction("printf", fctTy);
}

void GeneratorVisitor::createBr(llvm::BasicBlock* targetBlock) {
    if (blockAlreadyTerminated) return;
    builder->CreateBr(targetBlock);
    blockAlreadyTerminated = true;
}

void GeneratorVisitor::createCondBr(llvm::Value* condition, llvm::BasicBlock* trueBlock, llvm::BasicBlock* falseBlock) {
    if (blockAlreadyTerminated) return;
    builder->CreateCondBr(condition, trueBlock, falseBlock);
    blockAlreadyTerminated = true;
}

void GeneratorVisitor::moveInsertPointToBlock(llvm::BasicBlock* block) {
    builder->SetInsertPoint(block);
    blockAlreadyTerminated = false;
}

llvm::Value* GeneratorVisitor::insertAlloca(llvm::Type* llvmType, const std::string& varName, llvm::Value* arraySize) {
    if (allocaInsertInst != nullptr) {
        llvm::AllocaInst* allocaInst = builder->CreateAlloca(llvmType, arraySize, varName);
        allocaInst->moveAfter(allocaInsertInst);
        allocaInsertInst = allocaInst;
    } else {
        // Save current basic block and move insert cursor to entry block of the current function
        llvm::BasicBlock* currentBlock = builder->GetInsertBlock();
        builder->SetInsertPoint(allocaInsertBlock);

        allocaInsertInst = builder->CreateAlloca(llvmType, arraySize, varName);

        // Restore old basic block
        builder->SetInsertPoint(currentBlock);
    }
    return (llvm::Value*) allocaInsertInst;
}

llvm::Type* GeneratorVisitor::getTypeForSymbolType(SymbolType symbolType) {
    currentSymbolType = symbolType;

    // Get base symbol type
    std::stack<std::pair<SymbolSuperType, size_t>> pointerArrayList;
    while (symbolType.isPointer() || symbolType.isArray()) {
        SymbolSuperType superType = symbolType.isPointer() ? TY_PTR : TY_ARRAY;
        size_t arraySize = symbolType.isPointer() ? 0 : symbolType.getArraySize();
        pointerArrayList.push(std::make_pair(superType, arraySize));
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
        case TY_BYTE: // fallthrough
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
            assert(llvmBaseType != nullptr);
            break;
        }
        default:
            throw std::runtime_error("Internal compiler error: Cannot determine LLVM type of " + symbolType.getName(true));
    }

    // Consider pointer/array hierarchy
    while (!pointerArrayList.empty()) {
        if (pointerArrayList.top().first == TY_PTR) { // Pointer
            llvmBaseType = llvmBaseType->getPointerTo();
        } else if (pointerArrayList.top().second == 0) { // If size is 0, use the pointer type
            llvmBaseType = llvmBaseType->getPointerTo();
        } else { // Otherwise, use the array type with a fixed size
            llvmBaseType = llvm::ArrayType::get(llvmBaseType, pointerArrayList.top().second);
        }
        pointerArrayList.pop();
    }

    return llvmBaseType;
}

/*llvm::Value* GeneratorVisitor::getDefaultValueForType(SymbolType symbolType) {
    // Unpack symbol type
    std::stack<SymbolSuperType> ptrArrayStack;
    while (symbolType.isPointer() || symbolType.isArray()) {
        ptrArrayStack.push(symbolType.getSuperType());
        symbolType = symbolType.getContainedTy();
    }

    llvm::Value* defaultValue;
    switch (symbolType.getSuperType()) {
        case TY_DOUBLE:
            defaultValue = llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), 0);
            break;
        case TY_INT:
            defaultValue = builder->getInt32(0);
            break;
        case TY_SHORT:
            defaultValue = builder->getInt16(0);
            break;
        case TY_LONG:
            defaultValue = builder->getInt64(0);
            break;
        case TY_BYTE: // fall-through
        case TY_CHAR:
            defaultValue = builder->getInt8(0);
            break;
        case TY_STRING:
            defaultValue = builder->CreateGlobalStringPtr("", "", 0, module.get());
            break;
        case TY_BOOL:
            defaultValue = builder->getInt1(false);
            break;
        case TY_STRUCT: {
            llvm::Type* structType = getTypeForSymbolType(symbolType);
            llvm::Value* structAddress = insertAlloca(structType, currentVarName);

            break;
        }
        default:
            throw std::runtime_error("Internal compiler error. Cannot determine default value for type" +
                symbolType.getName(true));
    }

    // Pack value
    while (!ptrArrayStack.empty()) {

        ptrArrayStack.pop();
    }

    return defaultValue;
}*/

void GeneratorVisitor::initExtStruct(const std::string& oldStructName, const std::string& newStructName) {
    SymbolTableEntry* newStructSymbol = currentScope->lookup(newStructName);
    assert(newStructSymbol != nullptr);
    if (newStructSymbol->getState() == DECLARED) { // Only initialize if it was not initialized yet
        SymbolTable* structTable = currentScope->lookupTable("struct:" + newStructName);
        assert(structTable != nullptr);

        // Get field types
        std::vector<llvm::Type*> memberTypes;
        for (unsigned int i = 0; i < structTable->getFieldCount(); i++) {
            SymbolType fieldType = structTable->lookupByIndexInCurrentScope(i)->getType();
            if (fieldType.is(TY_STRUCT)) {
                std::string structName = fieldType.getSubType();
                initExtStruct(structName, scopePrefix + "." + structName);
                fieldType = SymbolType(TY_STRUCT, scopePrefix + "." + structName);
            } else if (fieldType.isPointerOf(TY_STRUCT)) {
                std::string structName = fieldType.getContainedTy().getSubType();
                initExtStruct(structName, scopePrefix + "." + structName);
                fieldType = SymbolType(TY_STRUCT, scopePrefix + "." + structName);
                fieldType = fieldType.toPointer();
            }
            // ToDo: Support double, triple, etc. pointers
            memberTypes.push_back(getTypeForSymbolType(fieldType));
        }

        // Create global struct
        llvm::StructType* structType = llvm::StructType::create(*context, memberTypes, newStructName);
        newStructSymbol->updateLLVMType(structType);
        newStructSymbol->updateState(INITIALIZED, newStructSymbol->getDefinitionToken());
    }
}

bool GeneratorVisitor::compareLLVMTypes(llvm::Type* lhs, llvm::Type* rhs) {
    if (lhs->getTypeID() != rhs->getTypeID()) return false;
    if (lhs->getTypeID() == llvm::Type::PointerTyID)
        return compareLLVMTypes(lhs->getPointerElementType(), rhs->getPointerElementType());
    if (lhs->getTypeID() == llvm::Type::ArrayTyID)
        return compareLLVMTypes(lhs->getArrayElementType(), rhs->getArrayElementType());
    return true;
}

llvm::Value* GeneratorVisitor::doImplicitCast(llvm::Value* srcValue, llvm::Type* dstType) {
    // Unpack the pointers until a pointer of another type is met
    unsigned int loadCounter = 0;
    while (srcValue->getType()->getPointerElementType()->isPointerTy()) {
        srcValue = builder->CreateLoad(srcValue->getType()->getPointerElementType(), srcValue);
        loadCounter++;
    }
    // GEP or bit-cast
    if (dstType->isPointerTy() && srcValue->getType()->getPointerElementType()->isArrayTy()) {
        std::vector<llvm::Value*> indices = { builder->getInt32(0), builder->getInt32(0) };
        llvm::Type* actualArgType = srcValue->getType()->getPointerElementType();
        srcValue = builder->CreateInBoundsGEP(actualArgType, srcValue, indices);
    } else {
        llvm::Type* actualArgType = srcValue->getType()->getPointerElementType();
        srcValue = builder->CreateLoad(actualArgType, srcValue);
        srcValue = builder->CreateBitCast(srcValue, dstType);
    }
    // Pack the pointers together again
    for (;loadCounter > 0; loadCounter--) {
        llvm::Value* newActualArg = insertAlloca(srcValue->getType());
        builder->CreateStore(srcValue, newActualArg);
        srcValue = newActualArg;
    }
    return srcValue;
}

llvm::PassBuilder::OptimizationLevel GeneratorVisitor::getLLVMOptLevelFromSpiceOptLevel() const {
    switch (optLevel) { // Get LLVM opt level from Spice opt level
        case 1: return llvm::PassBuilder::OptimizationLevel::O1;
        case 2: return llvm::PassBuilder::OptimizationLevel::O2;
        case 3: return llvm::PassBuilder::OptimizationLevel::O3;
        default: case 0: return llvm::PassBuilder::OptimizationLevel::O0;
    }
}
